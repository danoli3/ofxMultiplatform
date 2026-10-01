// ofxBaseApp
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Created by Daniel Rosser on 25/05/2014.
// Enhanced: scene lifecycle, auto-advance, requestAppChange.
//--------------------------------------------------------------
#pragma once

#include "ofMain.h"
#include "ofxMultiPlatformEvent.h"
#include "ofxAppGlobals.h"

// GameControllerEvent is optional (iOS). Guard so desktop still compiles
// if the header is only in the iOS tree.
#if defined(TARGET_OF_IOS) || defined(TARGET_IOS)
#include "GameControllerEvent.h"
#else
// Stub so pure virtual isn't needed on desktop
struct GameControllerEvent {
	int type = 0;
};
#endif

#include <string>

/// Lifecycle phase while this scene is managed by ofxAppManager.
enum class ofxScenePhase {
	None = 0,
	Entering,  // fade-in / first frames
	Active,    // normal play
	Exiting,   // fade-out before destroy
	Dead
};

// ----------------
// Subclass this (not ofBaseApp) so the manager can route touch + scenes.
//

class ofxBaseApp : public ofBaseApp {

public:

	ofxBaseApp()
	: appName("")
	, phase(ofxScenePhase::None) {}

	explicit ofxBaseApp(std::string name)
	: appName(std::move(name))
	, phase(ofxScenePhase::None) {}

	// Virtual dtor so manager can delete via ofxBaseApp* safely.
	virtual ~ofxBaseApp() {}

	virtual std::string getAppName() { return appName; }

	// --- Touch (also forwarded from mouse on desktop) ---
	virtual void touchDown(int x, int y, int id) {}
	virtual void touchMoved(int x, int y, int id) {}
	virtual void touchUp(int x, int y, int id) {}
	virtual void touchDoubleTap(int x, int y, int id) {}
	virtual void touchCancelled(int x, int y, int id) {}

	virtual void gameControllerEvent(GameControllerEvent & event) {}

	// --- Scene lifecycle (called by ofxAppManager) ---

	/// After setup(), when the scene becomes current (start of fade-in).
	virtual void onEnter() {}

	/// About to leave: durationSec is planned fade-out length. Free heavy
	/// resources here if needed; object is destroyed after the fade.
	virtual void onExit(float durationSec) { (void)durationSec; }

	/// Legacy alias used by older scenes.
	virtual void exit(float willExitSceneInMS) {
		onExit(willExitSceneInMS / 1000.f);
	}

	/// Return true to auto-advance to nextAppName() (or a default).
	virtual bool hasEnded() const { return false; }

	/// Scene id to load when hasEnded() is true. Empty = no auto-advance.
	virtual std::string nextAppName() const { return ""; }

	/// Suggested fade durations (manager may override with its own defaults).
	virtual float enterDuration() const { return -1.f; } // <0 → use manager default
	virtual float exitDuration() const { return -1.f; }

	/// While true, manager will not deliver input (used during transitions).
	virtual bool acceptsInput() const {
		return phase == ofxScenePhase::Active || phase == ofxScenePhase::Entering;
	}

	ofxScenePhase getPhase() const { return phase; }

	/// Request a scene change via the global event bus (safe from any scene).
	void requestAppChange(const std::string & appID) {
		ofxRequestAppChange(appID);
	}

	// Manager only:
	void setPhase(ofxScenePhase p) { phase = p; }

protected:
	std::string appName;
	ofxScenePhase phase;

	virtual std::string getClassName() {
		ofLogWarning("ofxBaseApp") << "getClassName needs implementing in child";
		return "";
	}
};
