#!/usr/bin/env bash
# Build and run ofxMultiPlatform system tests against an openFrameworks tree.
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"
TEST_DIR="${REPO_ROOT}/test"

OF_ROOT="${OF_ROOT:-}"
if [[ -z "$OF_ROOT" ]]; then
	# apps/myApps/ofxMultiPlatform → ../../../
	if [[ -d "${REPO_ROOT}/../../../libs/openFrameworks" ]]; then
		OF_ROOT="$(cd "${REPO_ROOT}/../../.." && pwd)"
	elif [[ -d "${REPO_ROOT}/../openFrameworks/libs/openFrameworks" ]]; then
		OF_ROOT="$(cd "${REPO_ROOT}/../openFrameworks" && pwd)"
	elif [[ -d "${REPO_ROOT}/../../openFrameworks/libs/openFrameworks" ]]; then
		OF_ROOT="$(cd "${REPO_ROOT}/../../openFrameworks" && pwd)"
	else
		echo "error: set OF_ROOT to your openFrameworks path" >&2
		exit 1
	fi
fi

OF_ROOT="$(cd "$OF_ROOT" && pwd)"
JOBS="${OF_JOBS:-$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)}"
CONFIG="${1:-Debug}"

echo "==> OF_ROOT=${OF_ROOT}"
echo "==> test dir=${TEST_DIR}"
echo "==> config=${CONFIG} jobs=${JOBS}"

if [[ ! -f "${OF_ROOT}/libs/openFrameworksCompiled/project/makefileCommon/compile.project.mk" ]]; then
	echo "error: OF_ROOT does not look like openFrameworks: ${OF_ROOT}" >&2
	exit 1
fi

if [[ ! -d "${OF_ROOT}/addons/ofxUnitTests" ]]; then
	echo "error: ofxUnitTests not found under ${OF_ROOT}/addons" >&2
	exit 1
fi

cd "$TEST_DIR"

echo "==> Building tests…"
make -j"${JOBS}" OF_ROOT="${OF_ROOT}" "${CONFIG}"

# Resolve binary (linux: bin/test_debug; macOS: bin/test_debug.app/Contents/MacOS/test_debug)
BIN=""
# Prefer macOS .app first when present
APP=$(find "${TEST_DIR}/bin" -maxdepth 2 -type d -name '*.app' 2>/dev/null | head -1 || true)
if [[ -n "$APP" ]]; then
	if [[ "$(uname -s)" == "Darwin" ]]; then
		BIN=$(find "$APP/Contents/MacOS" -type f -perm +111 2>/dev/null | head -1 || true)
	else
		BIN=$(find "$APP/Contents/MacOS" -type f -executable 2>/dev/null | head -1 || true)
	fi
fi
if [[ -z "$BIN" ]]; then
	for candidate in \
		"${TEST_DIR}/bin/test_debug" \
		"${TEST_DIR}/bin/test" \
		"${TEST_DIR}/bin/"*_debug \
		"${TEST_DIR}/bin/"*
	do
		if [[ -x "$candidate" && ! -d "$candidate" ]]; then
			BIN="$candidate"
			break
		fi
	done
fi

if [[ -z "$BIN" || ! -x "$BIN" ]]; then
	echo "error: could not find built test binary under ${TEST_DIR}/bin" >&2
	ls -laR "${TEST_DIR}/bin" 2>/dev/null || true
	exit 1
fi

echo "==> Running ${BIN}"
# Headless tests use ofAppNoWindow — no xvfb required, but allow it if present.
if command -v xvfb-run >/dev/null 2>&1 && [[ "$(uname -s)" == "Linux" ]]; then
	xvfb-run -a "$BIN"
else
	"$BIN"
fi

echo "==> Tests passed"
