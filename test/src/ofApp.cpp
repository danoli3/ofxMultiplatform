// ofxMultiPlatform — unit tests for scene registry, events, manager lifecycle
#include "ofApp.h"

#include "ofxAppManager.h"
#include "ofxSceneRegistry.h"
#include "ofxMultiPlatformEvent.h"
#include "TestScenes.h"

namespace {

void resetSceneCounters() {
	TestSceneA::setupCount = 0;
	TestSceneA::enterCount = 0;
	TestSceneA::exitCount = 0;
	TestSceneA::pauseCount = 0;
	TestSceneA::resumeCount = 0;
	TestSceneA::backCount = 0;
	TestSceneA::handleBack = false;
	TestSceneB::setupCount = 0;
	TestSceneB::enterCount = 0;
	TestSceneB::exitCount = 0;
}

/// Drive manager update/draw for n frames (simulates main loop).
void pump(ofxAppManager & mgr, int frames = 3) {
	for(int i = 0; i < frames; ++i) {
		mgr.update();
		mgr.draw();
	}
}

} // namespace

//--------------------------------------------------------------
void ofApp::run() {

	// ------------------------------------------------------------------
	// Registry
	// ------------------------------------------------------------------
	{
		ofxRegisterProjectScenes();

		ofxTest(ofxSceneRegistry::has("TestSceneA"), "registry has TestSceneA");
		ofxTest(ofxSceneRegistry::has("TestSceneB"), "registry has TestSceneB");
		ofxTest(ofxSceneRegistry::has("TestSceneEvent"), "registry has TestSceneEvent");
		ofxTest(!ofxSceneRegistry::has("DoesNotExist"), "registry missing id");

		ofxBaseApp * a = ofxSceneRegistry::create("TestSceneA");
		ofxTest(a != nullptr, "factory create TestSceneA");
		if(a) {
			ofxTestEq(a->getAppName(), std::string("TestSceneA"), "created scene name");
			delete a;
		}

		ofxBaseApp * missing = ofxSceneRegistry::create("Nope");
		ofxTest(missing == nullptr, "factory null for unknown");
	}

	// ------------------------------------------------------------------
	// Events — ofxRequestAppChange
	// ------------------------------------------------------------------
	{
		std::string received;
		int hits = 0;
		ofEventListener listener = ofxAppEvent::events.newListener([&](ofxAppEvent & e) {
			if(e.packetID == ofxAppEventID::ChangeApp) {
				received = e.message;
				hits++;
			}
		});

		ofxRequestAppChange("TestSceneB");
		ofxTestEq(hits, 1, "requestAppChange notifies once");
		ofxTestEq(received, std::string("TestSceneB"), "requestAppChange message");
		// listener RAII unsubscribes
	}

	// ------------------------------------------------------------------
	// Manager — boot, load, lifecycle
	// ------------------------------------------------------------------
	{
		resetSceneCounters();

		ofxAppManager mgr;
		mgr.autoBoot = false;
		mgr.takeSnapShot = false;
		mgr.defaultFadeIn = 0.f;
		mgr.defaultFadeOut = 0.f;
		mgr.setup();

		ofxTestEq(mgr.getAppID(), std::string(""), "no boot when autoBoot=false");

		mgr.loadApp("TestSceneA");
		pump(mgr, 2);

		ofxTestEq(mgr.getAppID(), std::string("TestSceneA"), "loadApp sets current id");
		ofxTest(TestSceneA::setupCount >= 1, "TestSceneA setup called");
		ofxTest(TestSceneA::enterCount >= 1, "TestSceneA onEnter called");
		ofxTest(!mgr.isTransitioning(), "instant transition finished");

		const int setupsBefore = TestSceneA::setupCount;
		mgr.loadApp("TestSceneA");
		ofxTestEq(TestSceneA::setupCount, setupsBefore, "reload same scene is no-op");

		mgr.loadApp("TestSceneB");
		pump(mgr, 2);
		ofxTestEq(mgr.getAppID(), std::string("TestSceneB"), "switched to TestSceneB");
		ofxTest(TestSceneA::exitCount >= 1, "TestSceneA onExit called");
		ofxTest(TestSceneB::enterCount >= 1, "TestSceneB onEnter called");
	}

	// ------------------------------------------------------------------
	// Manager — hasEnded auto-advance
	// ------------------------------------------------------------------
	{
		resetSceneCounters();

		ofxAppManager mgr;
		mgr.autoBoot = false;
		mgr.takeSnapShot = false;
		mgr.defaultFadeIn = 0.f;
		mgr.defaultFadeOut = 0.f;
		mgr.setup();

		ofxSceneRegistry::add("TestSceneAEnds", []() -> ofxBaseApp * {
			auto * s = new TestSceneA();
			s->ended = true;
			return s;
		});

		mgr.loadApp("TestSceneAEnds");
		pump(mgr, 3);
		ofxTestEq(mgr.getAppID(), std::string("TestSceneB"), "hasEnded auto-advances to nextAppName");
	}

	// ------------------------------------------------------------------
	// Manager — ofxRequestAppChange routed through manager
	// ------------------------------------------------------------------
	{
		resetSceneCounters();

		ofxAppManager mgr;
		mgr.autoBoot = false;
		mgr.takeSnapShot = false;
		mgr.defaultFadeIn = 0.f;
		mgr.defaultFadeOut = 0.f;
		mgr.setup();
		TestSceneEvent::destroyedDuringUpdate = 0;
		mgr.loadApp("TestSceneEvent");
		pump(mgr, 4);
		ofxTestEq(mgr.getAppID(), std::string("TestSceneB"), "event bus changeScene works");
		ofxTestEq(TestSceneEvent::destroyedDuringUpdate, 0, "scene lives until update returns");
	}

	// ------------------------------------------------------------------
	// Manager — unknown id does not crash
	// ------------------------------------------------------------------
	{
		ofxAppManager mgr;
		mgr.autoBoot = false;
		mgr.takeSnapShot = false;
		mgr.setup();
		mgr.loadApp("TotallyMissing");
		ofxTestEq(mgr.getAppID(), std::string(""), "unknown id leaves empty app");
	}

	// ------------------------------------------------------------------
	// Manager — kill / exit cleanup
	// ------------------------------------------------------------------
	{
		resetSceneCounters();
		ofxAppManager mgr;
		mgr.autoBoot = false;
		mgr.takeSnapShot = false;
		mgr.defaultFadeIn = 0.f;
		mgr.defaultFadeOut = 0.f;
		mgr.setup();
		mgr.loadApp("TestSceneA");
		pump(mgr, 1);
		mgr.killApp();
		ofxTestEq(mgr.getAppID(), std::string(""), "killApp clears current");
		mgr.exit();
	}

	// ------------------------------------------------------------------
	// Pause, resume, and back
	// ------------------------------------------------------------------
	{
		resetSceneCounters();
		ofxAppManager mgr;
		mgr.autoBoot = false;
		mgr.takeSnapShot = false;
		mgr.defaultFadeIn = 0.f;
		mgr.defaultFadeOut = 0.f;
		mgr.backSceneId = "TestSceneB";
		mgr.setup();
		mgr.loadApp("TestSceneA");
		pump(mgr, 1);
		mgr.pause();
		mgr.resume();
		ofxTestEq(TestSceneA::pauseCount, 1, "pause reaches the scene");
		ofxTestEq(TestSceneA::resumeCount, 1, "resume reaches the scene");

		TestSceneA::handleBack = true;
		ofxTest(mgr.backPressed(), "scene can consume back");
		ofxTestEq(mgr.getAppID(), std::string("TestSceneA"), "consumed back stays on the scene");
		TestSceneA::handleBack = false;

		ofxTest(mgr.backPressed(), "back returns to backSceneId");
		ofxTestEq(mgr.getAppID(), std::string("TestSceneB"), "back loads the home scene");
		ofxTest(!mgr.backPressed(), "back on the home scene is not handled");
		ofxTestEq(mgr.getAppID(), std::string("TestSceneB"), "unhandled back leaves the home scene");
	}

	ofLogNotice("ofxMultiPlatformTests") << "system tests finished";
}
