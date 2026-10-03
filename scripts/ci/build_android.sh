#!/usr/bin/env bash
# Build the Gradle project Project Generator wrote for this app.
#
# Requires OF_ROOT and PROJECT_DIR. Expects a macOS host with the Android
# SDK (GitHub macos runners provide one). The openFrameworks Android template
# asks for compileSdk 34, build-tools 35.0.0, and NDK 28.2.13676358.
# Only arm64-v8a is built: the other ABIs compile the same sources again.
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
"$sdkmanager" --sdk_root="$sdk" \
	"platforms;android-34" \
	"build-tools;35.0.0" \
	"ndk;28.2.13676358"
export ANDROID_NDK_HOME="${sdk}/ndk/28.2.13676358"

if /usr/libexec/java_home -v 21 >/dev/null 2>&1; then
	JAVA_HOME="$(/usr/libexec/java_home -v 21)"
	export JAVA_HOME
	echo "==> JAVA_HOME ${JAVA_HOME}"
fi

python3 - "$OF_ROOT" "$PROJECT_DIR" <<'PY'
import os, sys
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
text = text.replace("'armeabi-v7a', 'arm64-v8a', 'x86_64'", "'arm64-v8a'")
gradle.write_text(text)

sdk = os.environ["ANDROID_HOME"]
(project / "local.properties").write_text(f"sdk.dir={sdk}\n")
PY

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
