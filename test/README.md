# ofxMultiPlatform tests

Headless unit tests for **generic systems**: scene registry, app events, manager lifecycle, auto-advance, and event-bus scene changes.

Uses openFrameworks **`ofxUnitTests`** + **`ofAppNoWindow`** (no display required).

## What is covered

| Area | Checks |
|------|--------|
| `ofxSceneRegistry` | register / has / create / missing |
| `ofxRequestAppChange` | event notify payload |
| `ofxAppManager` | load, no-op same id, switch, lifecycle counts |
| Auto-advance | `hasEnded()` → `nextAppName()` |
| Event routing | scene `requestAppChange` → manager |
| Robustness | unknown id, kill/exit |

## Local run

From an openFrameworks tree where this repo is at  
`apps/myApps/ofxMultiPlatform` (or set `OF_ROOT`):

```bash
cd test
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu) Debug
# binary:
./bin/test_debug          # name may match folder: ofxMultiPlatform_test or test
# or:
make RunDebug
```

If `OF_ROOT` is not `../../../..` from `test/`:

```bash
make OF_ROOT=/path/to/openFrameworks Debug
```

Exit code = number of failed tests (`0` = all passed).

## CI

GitHub Actions (`.github/workflows/ci.yml`) clones **latest openFrameworks**, installs Linux deps, downloads libs, builds this test app, and runs it.

## Layout

```
test/
  src/
    main.cpp                 # ofAppNoWindow entry
    ofApp.h / ofApp.cpp      # ofxUnitTestsApp::run()
    TestScenes.h / .cpp      # lightweight scenes
    ofxRegisterTestScenes.cpp
  config.make                # pulls ../src/Manager (excludes main registration)
  addons.make                # ofxUnitTests
```

Main-app scene registration stays in `../src/ofxRegisterScenes.cpp`.  
Tests provide `ofxRegisterProjectScenes()` via `ofxRegisterTestScenes.cpp`.
