// ofxSceneRegistry.h
// ofxMultiPlatform - https://www.github.com/danoli3/ofxMultiPlatform
// Scene factory registry — register apps once, create by string id.
//--------------------------------------------------------------
#pragma once

#include "ofxBaseApp.h"
#include <functional>
#include <map>
#include <string>
#include <vector>

/// Creates ofxBaseApp subclasses by string id (class name / scene id).
class ofxSceneRegistry {
public:
	using Factory = std::function<ofxBaseApp *()>;

	static void add(const std::string & id, Factory factory) {
		getMap()[id] = std::move(factory);
	}

	static bool has(const std::string & id) {
		return getMap().count(id) > 0;
	}

	static ofxBaseApp * create(const std::string & id) {
		auto it = getMap().find(id);
		if(it == getMap().end()) {
			return nullptr;
		}
		return it->second();
	}

	static std::vector<std::string> ids() {
		std::vector<std::string> out;
		out.reserve(getMap().size());
		for(const auto & pair : getMap()) {
			out.push_back(pair.first);
		}
		return out;
	}

private:
	static std::map<std::string, Factory> & getMap() {
		static std::map<std::string, Factory> factories;
		return factories;
	}
};

/// Convenience: register Class under #Class string (and optional aliases).
template <typename T>
void ofxRegisterScene(const std::string & id) {
	ofxSceneRegistry::add(id, []() -> ofxBaseApp * { return new T(); });
}

/// Implemented by the host project (main app or test app).
/// Called from ofxAppManager::setup() via registerDefaultScenes().
void ofxRegisterProjectScenes();
