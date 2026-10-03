#!/usr/bin/env bash
# Run the Project Generator CLI against apps/myApps/ofxMultiPlatform.
#
# Usage: run_project_generator.sh <osx|linux64|vs>
# Requires OF_ROOT and PROJECT_DIR (set by fetch_of_nightly.sh).
# PG_RELEASE (default: nightly) is the projectGenerator release used when the
# openFrameworks package has no working command-line binary.
set -euo pipefail

PLATFORM="${1:?pg platform required (osx, linux64, or vs)}"
OF_ROOT="${OF_ROOT:?set OF_ROOT}"
PROJECT_DIR="${PROJECT_DIR:?set PROJECT_DIR}"
PG_RELEASE="${PG_RELEASE:-nightly}"

case "$PLATFORM" in
	osx|linux64|vs) ;;
	*)
		echo "error: unsupported platform ${PLATFORM}" >&2
		exit 1
		;;
esac

# CocoaTouch storyboards live under src/iOS. Project Generator adds every
# storyboard to the macOS target's Copy Bundle Resources phase, and ibtool
# then rejects them. Desktop builds do not use these files. Dot-directories
# are skipped by Project Generator and by the Linux makefile scan.
if [[ "$PLATFORM" == "osx" ]]; then
	stash="${PROJECT_DIR}/.ios-storyboards"
	found=0
	while IFS= read -r storyboard; do
		[[ -n "$storyboard" ]] || continue
		rel="${storyboard#"${PROJECT_DIR}/src/"}"
		mkdir -p "${stash}/$(dirname "$rel")"
		mv "$storyboard" "${stash}/${rel}"
		echo "==> stashed iOS storyboard out of src/: ${rel}"
		found=1
	done < <(find "${PROJECT_DIR}/src" -name '*.storyboard' -type f)
	if [[ "$found" -eq 0 ]]; then
		echo "==> no iOS storyboards to stash"
	fi
fi

native_path() {
	if [[ "${RUNNER_OS:-}" == "Windows" ]]; then
		cygpath -w "$1"
	else
		printf '%s\n' "$1"
	fi
}

# True when `bin --help` looks like the command-line Project Generator.
# The Electron GUI host answers slowly and is not a generator.
pg_is_cli() {
	local bin="$1"
	[[ -f "$bin" ]] || return 1
	case "$bin" in
		*/Contents/MacOS/projectGenerator) return 1 ;;
		*/projectGenerator.exe)
			# Root GUI exe. The CLI copy lives under resources/app/app/.
			[[ "$bin" == */resources/app/app/projectGenerator.exe ]] || return 1
			;;
	esac
	chmod +x "$bin" 2>/dev/null || true
	local py=python3
	command -v python3 >/dev/null 2>&1 || py=python
	"$py" -c '
import subprocess, sys
bin = sys.argv[1]
for flag in ("--help", "/help", "-h"):
    try:
        p = subprocess.run([bin, flag], capture_output=True, text=True, timeout=25)
    except Exception:
        continue
    out = ((p.stdout or "") + (p.stderr or "")).lower()
    if "platform" in out or "ofpath" in out:
        sys.exit(0)
sys.exit(1)
' "$bin"
}

# Search order matches scripts/of.sh findPGBinary: never the Electron host.
find_bundled_cli() {
	local root="$1"
	local c
	for c in \
		"${root}/projectGeneratorCmd.exe" \
		"${root}/projectGeneratorCmd" \
		"${root}/resources/app/app/projectGenerator.exe" \
		"${root}/resources/app/app/projectGenerator" \
		"${root}/projectGenerator.app/Contents/Resources/app/app/projectGenerator"
	do
		if pg_is_cli "$c"; then
			printf '%s\n' "$c"
			return 0
		fi
	done
	return 1
}

download_cli() {
	local asset dest archive unpack c
	case "$PLATFORM" in
		linux64) asset="projectGenerator-linux.tar.bz2" ;;
		vs) asset="projectGenerator-vs.zip" ;;
		osx) asset="projectGenerator-osx.zip" ;;
	esac
	dest="${RUNNER_TEMP:-/tmp}/ofxmp-pg-cli"
	rm -rf "$dest"
	mkdir -p "$dest"
	echo "==> downloading ${asset} from projectGenerator ${PG_RELEASE}" >&2
	gh release download "$PG_RELEASE" \
		--repo openframeworks/projectGenerator \
		--pattern "$asset" \
		--dir "$dest" \
		--clobber
	archive="$(find "$dest" -maxdepth 1 -type f -name "$asset" -print -quit)"
	unpack="${dest}/unpack"
	mkdir -p "$unpack"
	case "$archive" in
		*.zip) unzip -q "$archive" -d "$unpack" ;;
		*.tar.bz2) tar -xjf "$archive" -C "$unpack" ;;
		*.gz|*.tar.gz) tar -xzf "$archive" -C "$unpack" ;;
		*)
			echo "error: unsupported projectGenerator archive ${archive}" >&2
			return 1
			;;
	esac
	# vs CLI zip is a single projectGenerator.exe. That root exe is the CLI,
	# unlike the GUI package where the root exe is Electron.
	if [[ "$PLATFORM" == "vs" && -f "${unpack}/projectGenerator.exe" ]]; then
		printf '%s\n' "${unpack}/projectGenerator.exe"
		return 0
	fi
	if [[ "$PLATFORM" == "linux64" && -f "${unpack}/projectGenerator" ]]; then
		chmod +x "${unpack}/projectGenerator"
		printf '%s\n' "${unpack}/projectGenerator"
		return 0
	fi
	c="$(find "$unpack" -type f -path '*/Contents/Resources/app/app/projectGenerator' -print -quit || true)"
	if [[ -n "$c" ]]; then
		chmod +x "$c"
		printf '%s\n' "$c"
		return 0
	fi
	echo "error: no command-line projectGenerator in ${asset}" >&2
	find "$unpack" -maxdepth 4 -print >&2 || true
	return 1
}

PG=""
if [[ -d "${OF_ROOT}/projectGenerator" ]]; then
	PG="$(find_bundled_cli "${OF_ROOT}/projectGenerator" || true)"
fi
if [[ -z "$PG" ]]; then
	echo "==> no CLI in the openFrameworks package; using the projectGenerator release"
	PG="$(download_cli)"
fi
if [[ -z "$PG" || ! -f "$PG" ]]; then
	echo "error: Project Generator CLI not found" >&2
	exit 1
fi
chmod +x "$PG" 2>/dev/null || true
echo "==> projectGenerator ${PG}"
echo "==> platform ${PLATFORM}"

log="${RUNNER_TEMP:-/tmp}/pg-nightly.log"
set +e
if [[ "${RUNNER_OS:-}" == "Windows" ]]; then
	# This CLI rewrites arguments that start with / into drive paths, so
	# /ofPath=D:\a\... arrives as D:\ofPath=D:\a\... and is used as the
	# project path. The parser accepts the same dash options as on Unix.
	of_win="$(native_path "$OF_ROOT")"
	proj_win="$(native_path "$PROJECT_DIR")"
	export PG_OF_PATH="$of_win"
	echo "==> PG_OF_PATH=${PG_OF_PATH}"
	"$PG" --ofPath "$of_win" --platforms "$PLATFORM" --verbose "$proj_win" | tee "$log"
else
	"$PG" -o"$OF_ROOT" -p"$PLATFORM" -v "$PROJECT_DIR" | tee "$log"
fi
status=${PIPESTATUS[0]}
set -e
if [[ "$status" -ne 0 ]] || grep -q 'EXIT_FAILURE' "$log"; then
	echo "error: Project Generator failed (exit ${status})" >&2
	exit 1
fi

# Project Generator replaces config.make from the platform template, so the
# Linux exclusions in the repo copy do not survive this update.
if [[ "$PLATFORM" == "linux64" || "$PLATFORM" == "linux" ]]; then
	make_file="${PROJECT_DIR}/config.make"
	if ! grep -q 'src/iOS/%' "$make_file" 2>/dev/null; then
		cat >> "$make_file" <<'EOF'

# Linux g++ has no Objective-C++ frontend (cc1objplus).
ifeq ($(shell uname -s),Linux)
PROJECT_EXCLUSIONS += $(PROJECT_ROOT)/src/OSX
PROJECT_EXCLUSIONS += $(PROJECT_ROOT)/src/OSX/%
PROJECT_EXCLUSIONS += $(PROJECT_ROOT)/src/iOS
PROJECT_EXCLUSIONS += $(PROJECT_ROOT)/src/iOS/%
endif
EOF
		echo "==> excluded src/OSX and src/iOS from the Linux makefile"
	fi
fi
echo "==> Project Generator updated ${PROJECT_DIR}"
