#!/usr/bin/env bash
# Build the Gradle project Project Generator wrote for this app.
#
# Requires OF_ROOT and PROJECT_DIR. Expects a macOS host with the Android
# SDK (GitHub macos runners provide one). The openFrameworks Android template
# asks for compileSdk 34, build-tools 35.0.0, and NDK 28.2.13676358.
# Only arm64-v8a is built: the other ABIs compile the same sources again.
#
# The openFrameworks android/cmake.sh script picks the newest NDK under the
# SDK and its toolchain file looks for a darwin-arm64 host. GitHub macOS
# runners ship the NDK as darwin-x86_64 (NDK 29 is newest, and that host tag
# is missing). This script pins NDK 28.2, the version the template names,
# and points darwin-arm64 at the x86_64 tools.
set -euo pipefail

OF_ROOT="${OF_ROOT:?set OF_ROOT}"
PROJECT_DIR="${PROJECT_DIR:?set PROJECT_DIR}"

if [[ -n "${ANDROID_HOME:-}" && -d "$ANDROID_HOME" ]]; then
	sdk="$ANDROID_HOME"
elif [[ -d "${HOME}/Library/Android/sdk" ]]; then
	sdk="${HOME}/Library/Android/sdk"
else
	echo "error: Android SDK not found (set ANDROID_HOME)" >&2
	exit 1
fi
export ANDROID_HOME="$sdk"
export ANDROID_SDK_ROOT="$sdk"
echo "==> Android SDK ${sdk}"

sdkmanager="$(find "$sdk/cmdline-tools" -type f -name sdkmanager -print -quit 2>/dev/null || true)"
if [[ -z "$sdkmanager" ]]; then
	echo "error: sdkmanager not found under ${sdk}/cmdline-tools" >&2
	exit 1
fi
set +o pipefail
yes | "$sdkmanager" --sdk_root="$sdk" --licenses >/dev/null || true
set -o pipefail
ndk_version="28.2.13676358"
"$sdkmanager" --sdk_root="$sdk" \
	"platforms;android-34" \
	"build-tools;35.0.0" \
	"ndk;${ndk_version}"
export ANDROID_NDK_HOME="${sdk}/ndk/${ndk_version}"
export ANDROID_NDK_ROOT="$ANDROID_NDK_HOME"

# openFrameworks' toolchain selects darwin-arm64 on Apple Silicon. The macOS
# NDK package installs its compilers under darwin-x86_64.
prebuilt="${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt"
wrapper="aarch64-linux-android34-clang"
if [[ -e "${prebuilt}/darwin-arm64/bin/${wrapper}" ]]; then
	echo "==> NDK host toolchain darwin-arm64"
elif [[ -e "${prebuilt}/darwin-x86_64/bin/${wrapper}" ]]; then
	if [[ -e "${prebuilt}/darwin-arm64" ]]; then
		echo "error: ${prebuilt}/darwin-arm64 exists but has no ${wrapper}" >&2
		exit 1
	fi
	ln -s darwin-x86_64 "${prebuilt}/darwin-arm64"
	echo "==> linked NDK darwin-arm64 -> darwin-x86_64"
else
	echo "error: ${wrapper} not found under ${prebuilt}" >&2
	ls -la "$prebuilt" >&2 || true
	exit 1
fi

if /usr/libexec/java_home -v 21 >/dev/null 2>&1; then
	JAVA_HOME="$(/usr/libexec/java_home -v 21)"
	export JAVA_HOME
	echo "==> JAVA_HOME ${JAVA_HOME}"
fi

python3 - "$OF_ROOT" "$PROJECT_DIR" <<'PY'
import os, re, sys
from pathlib import Path
of_root, project = Path(sys.argv[1]), Path(sys.argv[2])
of_project = of_root / "libs/openFrameworksCompiled/project"
settings = project / "settings.gradle"
if not settings.is_file():
    sys.exit("settings.gradle missing; Project Generator did not create an Android project")
rel = os.path.relpath(of_project, project)
lines = []
for line in settings.read_text().splitlines(True):
    if line.startswith("def openFrameworksProjectPath"):
        line = f"def openFrameworksProjectPath = '{rel}'\n"
    lines.append(line)
settings.write_text("".join(lines))
print(f"==> openFrameworksProjectPath {rel}")

gradle = project / "ofApp/build.gradle"
text = gradle.read_text()
abi = "'armeabi-v7a', 'arm64-v8a', 'x86_64'"
if abi not in text:
    sys.exit(f"abiFilters list not found in {gradle}")
text = text.replace(abi, "'arm64-v8a'")
known = 'def knownABIs = ["arm64-v8a", "armeabi-v7a", "x86_64"]'
if known not in text:
    sys.exit(f"knownABIs list not found in {gradle}")
text = text.replace(known, 'def knownABIs = ["arm64-v8a"]')
# playstoreDebug inherits the release signing config. The template leaves the
# debug-keystore passwords commented out, so signing fails. Use the standard
# debug keystore password.
signing = text.split("keyAlias 'androiddebugkey'", 1)[-1].split("buildTypes", 1)[0]
if not re.search(r"(?m)^\s*storePassword ", signing):
    needle = "keyAlias 'androiddebugkey'\n"
    if needle not in text:
        sys.exit(f"debug key alias not found in {gradle}")
    text = text.replace(
        needle,
        needle + "            storePassword 'android'\n            keyPassword 'android'\n",
        1,
    )
cmake_version = "version '3.22.1'"
if cmake_version not in text:
    sys.exit(f"CMake version pin not found in {gradle}")
# The runner image ships this CMake with the Android SDK. 3.22.1 is not installed.
text = text.replace(cmake_version, "version '3.31.5'", 1)
# ofxXmlSettings keeps tinyxml.h and its .cpp files directly in libs/.
# The template only searches libs/<name>/include and addon/src.
libs_walker = """\t\t\t\taddonIncludeDirs.add(includeDir.absolutePath)
\t\t\t}
\t\t}
\t}
}"""
libs_walker_fixed = """\t\t\t\taddonIncludeDirs.add(includeDir.absolutePath)
\t\t\t}
\t\t}
\t\tdef libsHasHeaders = false
\t\tlibsDir.eachFile { file ->
\t\t\tif (!file.isFile()) {
\t\t\t\treturn
\t\t\t}
\t\t\tif (file.name.endsWith(".h") || file.name.endsWith(".hpp")) {
\t\t\t\tlibsHasHeaders = true
\t\t\t}
\t\t\tif (file.name.endsWith(".cpp") || file.name.endsWith(".c")) {
\t\t\t\tprintln "Found libs C/CPP source: ${file}"
\t\t\t\taddonSourceFiles.add(file.absolutePath)
\t\t\t}
\t\t}
\t\tif (libsHasHeaders) {
\t\t\tprintln "Found libs include: ${libsDir}"
\t\t\taddonIncludeDirs.add(libsDir.absolutePath)
\t\t}
\t}
}"""
if libs_walker not in text:
    sys.exit("addon libs walker not found in ofApp/build.gradle")
text = text.replace(libs_walker, libs_walker_fixed, 1)
gradle.write_text(text)

cmake_sh = of_root / "libs/openFrameworksCompiled/project/android/cmake.sh"
script = cmake_sh.read_text()
ndk_pick = 'ANDROID_NDK_PATH=$(ls -d "$ANDROID_SDK_PATH/ndk/"* 2>/dev/null | sort -V | tail -n 1)\nexport ANDROID_NDK_ROOT=$ANDROID_NDK_PATH'
ndk_pinned = """if [ -n "${ANDROID_NDK_HOME:-}" ] && [ -d "$ANDROID_NDK_HOME" ]; then
    ANDROID_NDK_PATH="$ANDROID_NDK_HOME"
else
    ANDROID_NDK_PATH=$(ls -d "$ANDROID_SDK_PATH/ndk/"* 2>/dev/null | sort -V | tail -n 1)
fi
export ANDROID_NDK_ROOT=$ANDROID_NDK_PATH"""
if ndk_pick not in script:
    sys.exit(f"NDK discovery snippet not found in {cmake_sh}")
script = script.replace(ndk_pick, ndk_pinned, 1)
ninja = 'ninja -j "$NUM_CORES"'
if ninja not in script:
    sys.exit(f"ninja invocation not found in {cmake_sh}")
script = script.replace(ninja, 'ninja -j "${NUM_CORES:-${PARALLEL_MAKE:-2}}"', 1)
cmake_sh.write_text(script)
print(f"==> pinned NDK in {cmake_sh.name}")

# The desktop makefile adds every source directory as -I. The Android
# template only adds src/, so headers in src/Apps, src/Manager, and
# src/Android are invisible.
cmake_lists = project / "ofApp/src/CMakeLists.txt"
if not cmake_lists.is_file():
    sys.exit(f"CMakeLists missing: {cmake_lists}")
cl = cmake_lists.read_text()
marker = "# ofxMultiPlatform source include dirs"
if marker not in cl:
    cl += """
# ofxMultiPlatform source include dirs
file(GLOB_RECURSE _APP_HEADERS "${OF_APP_SRC_PATH}/*.h")
set(_app_inc)
foreach(_hdr ${_APP_HEADERS})
    get_filename_component(_dir "${_hdr}" DIRECTORY)
    list(APPEND _app_inc "${_dir}")
endforeach()
list(REMOVE_DUPLICATES _app_inc)
target_include_directories(${PROJECT_NAME} PRIVATE ${_app_inc})
"""
    cmake_lists.write_text(cl)
print("==> source include dirs cover src subfolders")

sdk = os.environ["ANDROID_HOME"]
(project / "local.properties").write_text(f"sdk.dir={sdk}\n")
PY

keystore="${HOME}/.android/debug.keystore"
if [[ ! -f "$keystore" ]]; then
	mkdir -p "${HOME}/.android"
	keytool -genkeypair -keystore "$keystore" -storepass android -keypass android \
		-alias androiddebugkey -keyalg RSA -keysize 2048 -validity 10000 \
		-dname "CN=Android Debug,O=Android,C=US"
	echo "==> created ${keystore}"
fi

export ARCH=arm64-v8a
export NUM_CORES="${OF_JOBS:-2}"

cd "$PROJECT_DIR"
java -classpath gradle/wrapper/gradle-wrapper.jar org.gradle.wrapper.GradleWrapperMain \
	assembleDebug --no-daemon --stacktrace
apk="$(find "$PROJECT_DIR" -path '*/outputs/apk/debug/*.apk' -type f -print -quit)"
if [[ -z "$apk" ]]; then
	echo "error: debug apk was not produced" >&2
	find "$PROJECT_DIR" -name '*.apk' -print >&2 || true
	exit 1
fi
echo "built ${apk}"
