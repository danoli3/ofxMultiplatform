// Lightweight scenes for unit tests (no heavy assets / GL requirements)
#pragma once

#include "ofxBaseApp.h"
#include <string>

/// Scene A — can auto-advance to B when marked ended.
class TestSceneA : public ofxBaseApp {
public:
	TestSceneA() : ofxBaseApp("TestSceneA") {}
	std::string getClassName() override { return "TestSceneA"; }

	void setup() override { setupCount++; }
	void update() override { frames++; }
	void draw() override {}
	void onEnter() override { enterCount++; }
	void onExit(float) override { exitCount++; }

	bool hasEnded() const override { return ended; }
	std::string nextAppName() const override { return "TestSceneB"; }

	float enterDuration() const override { return 0.f; } // instant for tests
	float exitDuration() const override { return 0.f; }

	void markEnded() { ended = true; }

	static int setupCount;
	static int enterCount;
	static int exitCount;
	int frames = 0;
	bool ended = false;
};

/// Scene B — terminal scene in the test graph.
class TestSceneB : public ofxBaseApp {
public:
	TestSceneB() : ofxBaseApp("TestSceneB") {}
	std::string getClassName() override { return "TestSceneB"; }

	void setup() override { setupCount++; }
	void update() override {}
	void draw() override {}
	void onEnter() override { enterCount++; }
	void onExit(float) override { exitCount++; }

	float enterDuration() const override { return 0.f; }
	float exitDuration() const override { return 0.f; }

	static int setupCount;
	static int enterCount;
	static int exitCount;
};

/// Scene that requests a change via the event bus.
class TestSceneEvent : public ofxBaseApp {
public:
	TestSceneEvent() : ofxBaseApp("TestSceneEvent") {}
	std::string getClassName() override { return "TestSceneEvent"; }

	void setup() override {}
	void update() override {
		if(!fired) {
			fired = true;
			inUpdate = true;
			requestAppChange("TestSceneB");
			inUpdate = false;
		}
	}
	void draw() override {}
	float enterDuration() const override { return 0.f; }
	float exitDuration() const override { return 0.f; }
	~TestSceneEvent() override {
		if(inUpdate) {
			destroyedDuringUpdate++;
		}
	}
	bool fired = false;
	bool inUpdate = false;
	static int destroyedDuringUpdate;
};
