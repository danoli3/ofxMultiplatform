// ofxAppManager.cpp
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
//--------------------------------------------------------------
#include "ofxAppManager.h"
#include "ofGLBaseTypes.h"

//--------------------------------------------------------------
ofxAppManager::ofxAppManager() {
	app = nullptr;
	bDebug = true;
	clickedTime = 0.f;
	transition = ofxSceneTransition::None;
	hasSnapshot = false;
}

ofxAppManager::~ofxAppManager() {
	ofRemoveListener(ofxAppEvent::events, this, &ofxAppManager::triggerEvent);
	killApp();
	clearSnapshot();
}

//--------------------------------------------------------------
void ofxAppManager::registerDefaultScenes() {
	// Project-specific factories live in ofxRegisterScenes.cpp (main app)
	// or test/src/ofxRegisterTestScenes.cpp (test app).
	ofxRegisterProjectScenes();
}

//--------------------------------------------------------------
void ofxAppManager::setup() {
	registerDefaultScenes();
	ofAddListener(ofxAppEvent::events, this, &ofxAppManager::triggerEvent);

	// Boot scene — change bootSceneId / autoBoot or call loadApp yourself.
	if(autoBoot) {
		loadApp(bootSceneId);
	}
}

//--------------------------------------------------------------
void ofxAppManager::exit() {
	ofRemoveListener(ofxAppEvent::events, this, &ofxAppManager::triggerEvent);
	killApp();
	clearSnapshot();
}

//--------------------------------------------------------------
void ofxAppManager::triggerEvent(ofxAppEvent & e) {
	if(e.packetID == ofxAppEventID::ChangeApp) {
		loadApp(e.message);
	} else if(e.packetID == ofxAppEventID::Platform) {
		// reserved — platform layers can listen separately
	}
}

//--------------------------------------------------------------
void ofxAppManager::loadApp(std::string appID) {
	if(appID.empty()) {
		ofLogWarning("ofxAppManager") << "loadApp: empty id";
		return;
	}

	if(app && app->getAppName() == appID && transition == ofxSceneTransition::None) {
		ofLogVerbose("ofxAppManager") << "already on scene " << appID;
		return;
	}

	if(!ofxSceneRegistry::has(appID)) {
		ofLogError("ofxAppManager") << "unknown scene id \"" << appID
			<< "\" — register it via ofxRegisterProjectScenes()";
		return;
	}

	// Queue if mid-transition (latest request wins).
	if(transition != ofxSceneTransition::None) {
		nextAppToLoad = appID;
		ofLogNotice("ofxAppManager") << "queued scene " << appID << " (transition in progress)";
		return;
	}

	// First load: no fade-out of a previous scene.
	if(!app) {
		ofxBaseApp * created = ofxSceneRegistry::create(appID);
		if(!created) {
			ofLogError("ofxAppManager") << "factory failed for " << appID;
			return;
		}
		inManagerMutation = true;
		app.reset(created);
		// Set before setup/onEnter so a nested loadApp queues instead of swapping now.
		transition = ofxSceneTransition::FadingIn;
		fadeElapsed = 0.f;

		enterSceneCall();
		app->setup();
		leaveSceneCall();

		if(app) {
			app->setPhase(ofxScenePhase::Entering);
			enterSceneCall();
			app->onEnter();
			leaveSceneCall();
		}

		if(app) {
			fadeInDuration = app->enterDuration();
			if(fadeInDuration < 0.f) {
				fadeInDuration = defaultFadeIn;
			}
			if(fadeInDuration <= 0.001f) {
				app->setPhase(ofxScenePhase::Active);
				finishTransition();
			} else {
				fadeTimer = fadeInDuration;
			}
			ofLogNotice("ofxAppManager") << "boot scene → " << appID;
		}
		inManagerMutation = false;
		flushDeferredSwaps();
		return;
	}

	beginTransitionTo(appID);
}

//--------------------------------------------------------------
void ofxAppManager::beginTransitionTo(const std::string & appID) {
	nextAppToLoad = appID;

	fadeOutDuration = app ? app->exitDuration() : defaultFadeOut;
	if(fadeOutDuration < 0.f) {
		fadeOutDuration = defaultFadeOut;
	}

	// Queue nested requestAppChange calls and block input before the scene callback.
	transition = ofxSceneTransition::FadingOut;
	fadeElapsed = 0.f;
	fadeTimer = fadeOutDuration;

	if(app) {
		ofxBaseApp * leaving = app.get();
		leaving->setPhase(ofxScenePhase::Exiting);
		// exit(ms) is the legacy bridge; its default forwards to onExit.
		enterSceneCall();
		leaving->exit(fadeOutDuration * 1000.f);
		leaveSceneCall();
	}

	if(takeSnapShot) {
		captureSnapshot();
	}

	if(fadeOutDuration <= 0.001f) {
		// Destroy only after the requesting callback has returned.
		if(sceneCallDepth > 0 || inManagerMutation) {
			swapAfterCallback = true;
		} else {
			performSwap();
			flushDeferredSwaps();
		}
		return;
	}

	ofLogNotice("ofxAppManager") << "fade out → " << appID;
}

//--------------------------------------------------------------
void ofxAppManager::performSwap() {
	inManagerMutation = true;
	transition = ofxSceneTransition::Swap;

	const std::string target = nextAppToLoad;
	nextAppToLoad.clear();

	// Caller guarantees this is not inside a method of the scene being destroyed.
	if(app) {
		app->setPhase(ofxScenePhase::Dead);
		app.reset();
	}

	if(target.empty()) {
		ofLogError("ofxAppManager") << "swap aborted, empty scene id";
		transition = ofxSceneTransition::None;
		clearSnapshot();
		inManagerMutation = false;
		return;
	}

	ofxBaseApp * created = ofxSceneRegistry::create(target);
	if(!created) {
		ofLogError("ofxAppManager") << "swap failed, unknown or null factory: " << target;
		transition = ofxSceneTransition::None;
		clearSnapshot();
		inManagerMutation = false;
		return;
	}

	app.reset(created);
	transition = ofxSceneTransition::FadingIn;
	fadeElapsed = 0.f;

	enterSceneCall();
	app->setup();
	leaveSceneCall();

	if(app) {
		app->setPhase(ofxScenePhase::Entering);
		enterSceneCall();
		app->onEnter();
		leaveSceneCall();
	}

	if(!app) {
		inManagerMutation = false;
		return;
	}

	fadeInDuration = app->enterDuration();
	if(fadeInDuration < 0.f) {
		fadeInDuration = defaultFadeIn;
	}

	if(fadeInDuration <= 0.001f) {
		app->setPhase(ofxScenePhase::Active);
		finishTransition();
		ofLogNotice("ofxAppManager") << "swapped → " << target << " (no fade-in)";
	} else {
		fadeTimer = fadeInDuration;
		ofLogNotice("ofxAppManager") << "swapped → " << target << " (fading in)";
	}

	inManagerMutation = false;
}

//--------------------------------------------------------------
void ofxAppManager::finishTransition() {
	clearSnapshot();
	transition = ofxSceneTransition::None;
	fadeElapsed = 0.f;

	// Apply any scene that was requested mid-transition.
	if(!nextAppToLoad.empty()) {
		const std::string queued = nextAppToLoad;
		nextAppToLoad.clear();
		loadApp(queued);
	}
}

//--------------------------------------------------------------
void ofxAppManager::killApp() {
	if(!app) {
		return;
	}
	app->setPhase(ofxScenePhase::Dead);
	sceneCallDepth++;
	app->exit(0.f);
	sceneCallDepth--;
	swapAfterCallback = false;
	nextAppToLoad.clear();
	app.reset();
	transition = ofxSceneTransition::None;
}

//--------------------------------------------------------------
void ofxAppManager::enterSceneCall() {
	sceneCallDepth++;
}

void ofxAppManager::leaveSceneCall() {
	if(sceneCallDepth > 0) {
		sceneCallDepth--;
	}
	if(sceneCallDepth == 0 && !inManagerMutation) {
		flushDeferredSwaps();
	}
}

void ofxAppManager::flushDeferredSwaps() {
	if(sceneCallDepth > 0 || inManagerMutation) {
		return;
	}
	int guard = 0;
	while(swapAfterCallback && guard < 8) {
		swapAfterCallback = false;
		performSwap();
		guard++;
	}
	if(swapAfterCallback) {
		ofLogWarning("ofxAppManager") << "stopped after 8 instant scene changes; continuing next update";
	}
}

//--------------------------------------------------------------
std::string ofxAppManager::getAppID() const {
	if(!app) {
		return "";
	}
	return app->getAppName();
}

//--------------------------------------------------------------
void ofxAppManager::captureSnapshot() {
	clearSnapshot();
	std::shared_ptr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(!renderer) {
		ofLogWarning("ofxAppManager") << "snapshot skipped, no GL renderer";
		hasSnapshot = false;
		return;
	}
	renderer->saveFullViewport(lastAppPixels);
	if(lastAppPixels.getWidth() <= 0 || lastAppPixels.getHeight() <= 0) {
		ofLogWarning("ofxAppManager") << "snapshot skipped, empty viewport";
		hasSnapshot = false;
		return;
	}
	lastAppTexture.allocate(lastAppPixels);
	lastAppTexture.loadData(lastAppPixels);
	hasSnapshot = lastAppTexture.isAllocated();
	if(!hasSnapshot) {
		ofLogWarning("ofxAppManager") << "snapshot failed";
	}
}

//--------------------------------------------------------------
void ofxAppManager::clearSnapshot() {
	if(lastAppTexture.isAllocated()) {
		lastAppTexture.clear();
	}
	lastAppPixels.clear();
	hasSnapshot = false;
}

//--------------------------------------------------------------
void ofxAppManager::update() {
	flushDeferredSwaps();
	const float dt = ofGetLastFrameTime();

	// Drive transition timers.
	if(transition == ofxSceneTransition::FadingOut) {
		fadeElapsed += dt;
		if(fadeElapsed >= fadeTimer) {
			performSwap();
			flushDeferredSwaps();
		}
	} else if(transition == ofxSceneTransition::FadingIn) {
		fadeElapsed += dt;
		if(fadeElapsed >= fadeTimer) {
			if(app) {
				app->setPhase(ofxScenePhase::Active);
			}
			finishTransition();
		}
	}

	if(app) {
		ofxBaseApp * current = app.get();
		enterSceneCall();
		current->update();
		leaveSceneCall();

		// Auto-advance when the scene that just updated reports it has finished.
		if(app && app.get() == current && transition == ofxSceneTransition::None && app->hasEnded()) {
			std::string next = app->nextAppName();
			if(!next.empty()) {
				ofLogNotice("ofxAppManager") << app->getAppName()
					<< " hasEnded → " << next;
				loadApp(next);
			}
		}
	}
}

//--------------------------------------------------------------
void ofxAppManager::drawTransitionOverlay() {
	if(transition == ofxSceneTransition::None) {
		return;
	}

	ofPushStyle();
	ofEnableAlphaBlending();

	float t = 0.f;
	if(fadeTimer > 0.f) {
		t = ofClamp(fadeElapsed / fadeTimer, 0.f, 1.f);
	}

	if(transition == ofxSceneTransition::FadingOut) {
		// Hold last frame if we have it, then fade to black.
		if(hasSnapshot && lastAppTexture.isAllocated()) {
			ofSetColor(255);
			lastAppTexture.draw(0, 0, ofGetWidth(), ofGetHeight());
		}
		ofSetColor(0, 0, 0, (int)(t * 255));
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	} else if(transition == ofxSceneTransition::FadingIn) {
		// Fade from black over the new scene.
		ofSetColor(0, 0, 0, (int)((1.f - t) * 255));
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	}

	ofPopStyle();
}

//--------------------------------------------------------------
void ofxAppManager::draw() {
	// During fade-out we still draw the old scene under the overlay
	// (unless we already snapped — then snapshot is enough).
	if(app) {
		if(transition == ofxSceneTransition::FadingOut && hasSnapshot) {
			// snapshot drawn in overlay path
		} else {
			ofxBaseApp * current = app.get();
			enterSceneCall();
			current->draw();
			leaveSceneCall();
		}
	}

	drawTransitionOverlay();

	if(bDebug) {
		ofPushStyle();
		ofSetColor(255);
		std::string label = ofToString((int)ofGetFrameRate()) + " fps";
		if(app) {
			label += "  |  " + app->getAppName();
		}
		if(isTransitioning()) {
			label += "  *";
		}
		ofDrawBitmapString(label, ofGetWidth() - 220, 20);
		ofPopStyle();
	}
}

//--------------------------------------------------------------
bool ofxAppManager::inputAllowed() const {
	if(blockInputDuringTransition && isTransitioning()) {
		return false;
	}
	if(app && !app->acceptsInput()) {
		return false;
	}
	return true;
}

//--------------------------------------------------------------
void ofxAppManager::touchDown(int x, int y, int id) {
	if(inputAllowed() && app) {
		ofxBaseApp * current = app.get();
		enterSceneCall();
		current->touchDown(x, y, id);
		leaveSceneCall();
	}
}

void ofxAppManager::touchMoved(int x, int y, int id) {
	if(inputAllowed() && app) {
		ofxBaseApp * current = app.get();
		enterSceneCall();
		current->touchMoved(x, y, id);
		leaveSceneCall();
	}
}

void ofxAppManager::touchUp(int x, int y, int id) {
	if(inputAllowed() && app) {
		ofxBaseApp * current = app.get();
		enterSceneCall();
		current->touchUp(x, y, id);
		leaveSceneCall();
	}
}

void ofxAppManager::touchDoubleTap(int x, int y, int id) {
	if(inputAllowed() && app) {
		ofxBaseApp * current = app.get();
		enterSceneCall();
		current->touchDoubleTap(x, y, id);
		leaveSceneCall();
	}
}

void ofxAppManager::touchCancelled(int x, int y, int id) {
	if(inputAllowed() && app) {
		ofxBaseApp * current = app.get();
		enterSceneCall();
		current->touchCancelled(x, y, id);
		leaveSceneCall();
	}
}

//--------------------------------------------------------------
void ofxAppManager::keyPressed(int key) {
	// Global debug toggle always available.
	if(key == 'd' || key == 'D') {
		bDebug = !bDebug;
	}

	if(inputAllowed() && app) {
		ofxBaseApp * current = app.get();
		enterSceneCall();
		current->keyPressed(key);
		leaveSceneCall();
	}
}

void ofxAppManager::keyReleased(int key) {
	if(inputAllowed() && app) {
		ofxBaseApp * current = app.get();
		enterSceneCall();
		current->keyReleased(key);
		leaveSceneCall();
	}
}

//--------------------------------------------------------------
void ofxAppManager::mouseMoved(int x, int y) {
	if(inputAllowed() && app) {
		ofxBaseApp * current = app.get();
		enterSceneCall();
		current->mouseMoved(x, y);
		leaveSceneCall();
	}
}

void ofxAppManager::mousePressed(int x, int y, int button) {
	if(!inputAllowed() || !app) {
		return;
	}
	ofxBaseApp * target = app.get();
	enterSceneCall();
	target->mousePressed(x, y, button);
	leaveSceneCall();
	// A swap replaces the scene; do not deliver the rest of this gesture to it.
	if(app.get() != target || !inputAllowed()) {
		return;
	}
	target = app.get();
	enterSceneCall();
	target->touchDown(x, y, button);
	leaveSceneCall();
}

void ofxAppManager::mouseDragged(int x, int y, int button) {
	if(!inputAllowed() || !app) {
		return;
	}
	ofxBaseApp * target = app.get();
	enterSceneCall();
	target->mouseDragged(x, y, button);
	leaveSceneCall();
	if(app.get() != target || !inputAllowed()) {
		return;
	}
	target = app.get();
	enterSceneCall();
	target->touchMoved(x, y, button);
	leaveSceneCall();
}

void ofxAppManager::mouseReleased(int x, int y, int button) {
	if(!inputAllowed() || !app) {
		return;
	}
	ofxBaseApp * target = app.get();
	enterSceneCall();
	target->mouseReleased(x, y, button);
	leaveSceneCall();

	const float currentTime = ofGetElapsedTimef();
	const bool doubleTap = (currentTime - clickedTime) <= 0.24f;
	clickedTime = currentTime;

	if(app.get() != target || !inputAllowed()) {
		return;
	}
	target = app.get();
	enterSceneCall();
	target->touchUp(x, y, button);
	leaveSceneCall();

	if(!doubleTap || app.get() != target || !inputAllowed()) {
		return;
	}
	target = app.get();
	enterSceneCall();
	target->touchDoubleTap(x, y, button);
	leaveSceneCall();
}

//--------------------------------------------------------------
void ofxAppManager::windowResized(int w, int h) {
	if(app) {
		ofxBaseApp * current = app.get();
		enterSceneCall();
		current->windowResized(w, h);
		leaveSceneCall();
	}
}

void ofxAppManager::pause() {
	if(!app) {
		return;
	}
	ofxBaseApp * current = app.get();
	enterSceneCall();
	current->onPause();
	leaveSceneCall();
}

void ofxAppManager::resume() {
	if(!app) {
		return;
	}
	ofxBaseApp * current = app.get();
	enterSceneCall();
	current->onResume();
	leaveSceneCall();
}

bool ofxAppManager::backPressed() {
	if(app) {
		ofxBaseApp * current = app.get();
		enterSceneCall();
		const bool handled = current->onBackPressed();
		leaveSceneCall();
		if(handled || app.get() != current) {
			return true;
		}
	}
	if(backSceneId.empty() || getAppID() == backSceneId || !ofxSceneRegistry::has(backSceneId)) {
		return false;
	}
	loadApp(backSceneId);
	return true;
}

void ofxAppManager::gotMessage(ofMessage msg) {
	if(app) {
		ofxBaseApp * current = app.get();
		enterSceneCall();
		current->gotMessage(msg);
		leaveSceneCall();
	}
}

void ofxAppManager::dragEvent(ofDragInfo dragInfo) {
	if(app) {
		ofxBaseApp * current = app.get();
		enterSceneCall();
		current->dragEvent(dragInfo);
		leaveSceneCall();
	}
}
