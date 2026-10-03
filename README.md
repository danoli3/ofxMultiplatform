# ofxMultiPlatform

[![CI](https://github.com/danoli3/ofxMultiplatform/actions/workflows/ci.yml/badge.svg?branch=main&event=push)](https://github.com/danoli3/ofxMultiplatform/actions/workflows/ci.yml)
[![PG nightly](https://github.com/danoli3/ofxMultiplatform/actions/workflows/pg-nightly.yml/badge.svg?branch=main&event=push)](https://github.com/danoli3/ofxMultiplatform/actions/workflows/pg-nightly.yml)

**CI** is the headless tests on Linux and macOS. **PG nightly** is the compile on Linux, Windows, macOS, Emscripten, and Android.

openFrameworks multi-platform app template (macOS / iOS / Android / Windows / Linux) with a **scene manager**.

Platform shells (`ofxAppOSXLayer`, `ofxAppiOSLayer`, …) own window/lifecycle and forward events into **`ofxAppManager`**, which owns the current **scene** (`ofxBaseApp` subclass).

---

## Scene flow (demo)

```
ofxExampleStart  →  ofxExampleMenu  →  ofApp / ofxDefaultApp / …
     intro              hub                 your scenes
```

- **Intro** fades in, then auto-advances after ~2.5s (or click / space).
- **Menu** lists registered scenes; click or press `1`–`3`.
- **ofApp** — `M` menu, `I` intro.
- **D** toggles the debug HUD (fps + scene name).

---

## Architecture

| Piece | Role |
|--------|------|
| `ofxAppManager` | Owns current scene, transitions, input gate, debug HUD |
| `ofxSceneRegistry` | Factory map: string id → `new Scene()` |
| `ofxBaseApp` | Scene base: touch + lifecycle + `requestAppChange` |
| `ofxRequestAppChange(id)` | Global event helper (any code can change scene) |
| Platform `*Layer` | Thin OF app that hosts the manager |

### Transition state machine

```
None → FadingOut → Swap → FadingIn → None
         ↑__________________|  (queued loadApp while busy)
```

- **FadingOut** — `onExit`, optional viewport snapshot, black overlay in.
- **Swap** — `unique_ptr` destroy old scene, factory-create new, `setup` + `onEnter`.
- **FadingIn** — black overlay out; then phase `Active`.
- Mid-transition requests are **queued** (latest wins) and applied when idle.

### Scene lifecycle hooks

```cpp
virtual void onEnter();
virtual void onExit(float durationSec);
virtual bool hasEnded() const;           // auto-advance when true
virtual std::string nextAppName() const; // target when hasEnded()
virtual float enterDuration() const;     // <0 → manager default
virtual float exitDuration() const;
```

---

## Add a new scene

**1. Create a class** (e.g. `src/Apps/MyGame.h` / `.cpp`):

```cpp
#pragma once
#include "ofxBaseApp.h"

class MyGame : public ofxBaseApp {
public:
    MyGame() : ofxBaseApp("MyGame") {}
    std::string getClassName() { return "MyGame"; }

    void setup() override;
    void update() override;
    void draw() override;

    void onEnter() override { /* start music, reset state */ }
    void onExit(float t) override { /* stop music */ }

    // Optional auto-chain:
    // bool hasEnded() const override { return scoreDone; }
    // std::string nextAppName() const override { return "ofxExampleMenu"; }
};
```

**2. Register it** in `src/ofxRegisterScenes.cpp` (`registerDefaultScenes()` only calls that function):

```cpp
#include "MyGame.h"

void ofxRegisterProjectScenes() {
    ofxRegisterScene<MyGame>("MyGame");
    // ...existing scenes
}
```

**3. Change to it** from any scene:

```cpp
requestAppChange("MyGame");
// or
ofxRequestAppChange("MyGame");
// or
// manager->loadApp("MyGame");
```

Add the `.cpp` to your Xcode / VS / Makefile project target.

---

## Change boot scene

`bootSceneId` is read from `setup()`. Set it before then. After setup, call `loadApp`:

```cpp
manager->bootSceneId = "ofxExampleMenu"; // before setup()
// or, any time:
manager->loadApp("MyGame");
```

The demo menu buttons are the three built-in scenes. The registry names drawn under them are display-only; add a button in `ofxExampleMenu::setup` to make a new id selectable.

---

## Tips

- Prefer **`requestAppChange`** over holding a raw manager pointer in scenes.
- Keep heavy loads in `setup` / `onEnter`; free them in `onExit` if the next scene does not need them.
- `unique_ptr<ofxBaseApp>` + virtual destructor — no more casted `delete`.
- Snapshot cross-fade: `takeSnapShot = true` (default). Disable if GL readback is expensive on device.

---

## Layout

```
src/
  Manager/     ofxAppManager, registry, events, globals
  Apps/        scenes (ofxBaseApp subclasses)
  OSX|iOS|…    platform host layers
  ofApp.*      sample main scene
  ofxRegisterScenes.cpp   main-app scene registration
  main.cpp     desktop entry (or platform main)
test/          headless system tests (ofxUnitTests + ofAppNoWindow)
.github/workflows/ci.yml
.github/workflows/pg-nightly.yml
```

## Tests & CI

System tests live in **`test/`** (scene registry, events, manager lifecycle).  
They use OF’s **`ofxUnitTests`** addon and run **headless**.

```bash
# from apps/myApps/ofxMultiPlatform
./scripts/ci/run_tests.sh Debug
# or
cd test && make -j OF_ROOT=/path/to/openFrameworks Debug && ./bin/test_debug
```

The badges at the top track the two workflows below.

GitHub Actions (`.github/workflows/ci.yml`) on push/PR:

1. Checks out this repo + **openframeworks/openFrameworks** (`master` by default)
2. Installs deps / downloads libs (`latest`)
3. Builds OF core + the test app on **ubuntu-24.04** and **macos-15**
4. Runs tests (exit code = failures)

Manual run: **Actions → CI → Run workflow** (optional OF ref / libs tag).

A second workflow (`.github/workflows/pg-nightly.yml`) downloads the openFrameworks **nightly** release package, runs the Project Generator command-line tool, and compiles this app on Linux, Windows, macOS, and Android. The Linux package is also built for Emscripten with `emmake`. It does not run the unit tests. Manual run: **Actions → PG nightly → Run workflow**.

See [test/README.md](test/README.md).
