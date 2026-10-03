#!/usr/bin/env bash
# Compile this app for Emscripten against the openFrameworks tree in OF_ROOT.
#
# The Linux nightly package already downloads the Emscripten libraries and
# keeps the emscripten makefiles. Project Generator has no emscripten project
# class (getTargetProject returns empty), so this uses the repo Makefile.
#
# Requires OF_ROOT and PROJECT_DIR from fetch_of_nightly.sh.
set -euo pipefail

OF_ROOT="${OF_ROOT:?set OF_ROOT}"
PROJECT_DIR="${PROJECT_DIR:?set PROJECT_DIR}"
OF_JOBS="${OF_JOBS:-2}"

version=""
install_sh="${OF_ROOT}/scripts/emscripten/install_emscripten.sh"
if [[ -f "$install_sh" ]]; then
	version="$(sed -n 's/^VERSION=//p' "$install_sh" | head -1 | tr -d '\"')"
fi
if [[ -z "$version" ]]; then
	version="6.0.6"
fi
echo "==> Emscripten ${version}"

emsdk_dir="${RUNNER_TEMP:-/tmp}/emsdk"
if [[ ! -x "${emsdk_dir}/emsdk" ]]; then
	git clone --depth 1 https://github.com/emscripten-core/emsdk.git "$emsdk_dir"
fi
(
	cd "$emsdk_dir"
	./emsdk install "$version"
	./emsdk activate "$version"
)
# shellcheck disable=SC1091
source "${emsdk_dir}/emsdk_env.sh"
emcc --version

em_lib="$(find "$OF_ROOT/libs" -type d -path '*/lib/emscripten' -print -quit 2>/dev/null || true)"
if [[ -z "$em_lib" ]]; then
	echo "==> Emscripten libs not in the package; downloading"
	if [[ -f "${OF_ROOT}/scripts/dev/download_libs.sh" ]]; then
		"${OF_ROOT}/scripts/emscripten/download_libs.sh" -n -t latest
	elif [[ -f "${OF_ROOT}/scripts/developer/download_libs.sh" ]]; then
		(
			cd "${OF_ROOT}/scripts/developer"
			./download_libs.sh -p emscripten -n -t latest
		)
	else
		echo "error: no download_libs.sh for emscripten" >&2
		exit 1
	fi
else
	echo "==> Emscripten libs at ${em_lib}"
fi

make_file="${PROJECT_DIR}/config.make"
if ! grep -q 'src/iOS/%' "$make_file" 2>/dev/null; then
	cat >> "$make_file" <<'EOF'

# emcc has no Objective-C++ frontend. Apple sources stay for the macOS and iOS projects.
ifeq ($(shell uname -s),Linux)
PROJECT_EXCLUSIONS += $(PROJECT_ROOT)/src/OSX
PROJECT_EXCLUSIONS += $(PROJECT_ROOT)/src/OSX/%
PROJECT_EXCLUSIONS += $(PROJECT_ROOT)/src/iOS
PROJECT_EXCLUSIONS += $(PROJECT_ROOT)/src/iOS/%
endif
EOF
fi

cd "$PROJECT_DIR"
emmake make -j"$OF_JOBS" Release
html="$(find bin/em -name index.html -type f -print -quit)"
if [[ -z "$html" ]]; then
	echo "error: bin/em/*/index.html was not produced" >&2
	find bin -maxdepth 3 -type f -print >&2 || true
	exit 1
fi
echo "built $(pwd)/${html}"
