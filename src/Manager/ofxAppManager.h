// ofxAppManager.h
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
// Enhanced: scene registry, transition state machine, auto-advance.
//--------------------------------------------------------------
#pragma once

#ifndef __ofxAppManager_h__
#define __ofxAppManager_h__

#include "ofMain.h"
#include "ofxBaseApp.h"
#include "ofxAppGlobals.h"
#include "ofxMultiPlatformEvent.h"
#include "ofxSceneRegistry.h"
#include "ofTexture.h"
#include "ofPixels.h"

#include <memory>
#include <string>

/// Transition state machine for scene changes.
enum class ofxSceneTransition {
	None = 0,
	FadingOut,  // old scene still drawn + overlay rising
	Swap,       // destroy old / create new (same frame)
	FadingIn    // new scene + overlay falling
};

class ofxAppManager : public ofBaseApp {

public:

	ofxAppManager();
	~ofxAppManager() override;

	void setup() override;
	void exit() override;

	void update() override;
	void draw() override;

	/// Scene change by registry id.
	/// A request made from inside the current scene is applied after that callback returns.
	void loadApp(std::string appID);
	/// Alias with clearer naming.
	void changeScene(const std::string & appID) { loadApp(appID); }

	void killApp();
	std::string getAppID() const;

	bool isTransitioning() const { return transition != ofxSceneTransition::None; }

	void touchDown(int x, int y, int id) override;
	void touchMoved(int x, int y, int id) override;
	void touchUp(int x, int y, int id) override;
	void touchDoubleTap(int x, int y, int id) override;
	void touchCancelled(int x, int y, int id) override;

	void keyPressed(int key) override;
	void keyReleased(int key) override;

	void mouseMoved(int x, int y) override;
	void mousePressed(int x, int y, int button) override;
	void mouseDragged(int x, int y, int button) override;
	void mouseReleased(int x, int y, int button) override;

	void windowResized(int w, int h) override;
	void dragEvent(ofDragInfo dragInfo) override;
	void gotMessage(ofMessage msg) override;

	/// Forward focus loss / Android pause to the current scene.
	void pause();
	/// Forward focus gain / Android resume to the current scene.
	void resume();
	/// Scene first, then backSceneId. False means the platform may exit.
	bool backPressed();

	void triggerEvent(ofxAppEvent & e);

	bool bDebug = true;
	float clickedTime = 0.f;

	/// Default fade lengths when a scene does not override.
	float defaultFadeOut = 0.35f;
	float defaultFadeIn = 0.35f;
	/// Capture viewport of outgoing scene as a texture (nice cross-fade).
	bool takeSnapShot = true;
	/// If true, input is blocked for the whole transition.
	bool blockInputDuringTransition = true;
	/// Starting scene id.
	std::string bootSceneId = "ofxExampleStart";
	/// Scene loaded by backPressed when the current scene does not handle it.
	std::string backSceneId = "ofxExampleMenu";

	/// When false, setup() registers scenes but does not load bootSceneId
	/// (useful for unit tests that drive loadApp themselves).
	bool autoBoot = true;

	/// Disable snapshot during tests / headless (GL readback).
	void setTakeSnapshot(bool v) { takeSnapShot = v; }

private:
	void registerDefaultScenes();
	void beginTransitionTo(const std::string & appID);
	void performSwap();
	void finishTransition();
	void clearSnapshot();
	void captureSnapshot();
	void drawTransitionOverlay();
	bool inputAllowed() const;
	/// Depth of calls into the live scene. Destruction waits until this is 0.
	void enterSceneCall();
	void leaveSceneCall();
	void flushDeferredSwaps();

	std::unique_ptr<ofxBaseApp> app;

	std::string nextAppToLoad;
	ofxSceneTransition transition = ofxSceneTransition::None;
	int sceneCallDepth = 0;
	bool inManagerMutation = false;
	bool swapAfterCallback = false;

	float fadeTimer = 0.f;      // current phase duration target
	float fadeElapsed = 0.f;    // time in current phase
	float fadeOutDuration = 0.35f;
	float fadeInDuration = 0.35f;

	ofPixels lastAppPixels;
	ofTexture lastAppTexture;
	bool hasSnapshot = false;
};

#endif
