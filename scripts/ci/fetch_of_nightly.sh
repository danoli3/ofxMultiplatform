#!/usr/bin/env bash
# Download one openFrameworks nightly (or other) release package and copy this
# repo to apps/myApps/ofxMultiPlatform inside it.
#
# Usage: fetch_of_nightly.sh <gh-release-pattern>
#   e.g. fetch_of_nightly.sh '*_linux64_gcc6_release.tar.gz'
#
# OF_RELEASE (default: nightly) selects the GitHub release tag.
# On GitHub Actions, OF_ROOT and PROJECT_DIR are appended to $GITHUB_ENV.
set -euo pipefail

PATTERN="${1:?release asset pattern required}"
OF_RELEASE="${OF_RELEASE:-nightly}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"

WORK="${RUNNER_TEMP:-/tmp}/ofxmp-nightly"
DL="${WORK}/download"
DEST="${WORK}/src"
rm -rf "$DL" "$DEST"
mkdir -p "$DL" "$DEST"

echo "==> openFrameworks release '${OF_RELEASE}' pattern ${PATTERN}"
gh release download "$OF_RELEASE" \
	--repo openframeworks/openFrameworks \
	--pattern "$PATTERN" \
	--dir "$DL" \
	--clobber

archive=""
count=0
for f in "$DL"/*; do
	[[ -f "$f" ]] || continue
	case "$f" in
		*.sha256|*.md5|*.sha1) continue ;;
	esac
	archive="$f"
	count=$((count + 1))
done
if [[ "$count" -ne 1 ]]; then
	echo "error: expected 1 archive for ${PATTERN}, found ${count} in ${DL}" >&2
	ls -la "$DL" >&2 || true
	exit 1
fi

echo "==> extracting $(basename "$archive")"
case "$archive" in
	*.zip) unzip -q "$archive" -d "$DEST" ;;
	*.tar.gz|*.tgz) tar -xzf "$archive" -C "$DEST" ;;
	*)
		echo "error: unsupported archive ${archive}" >&2
		exit 1
		;;
esac

of_lib="$(find "$DEST" -type d -path '*/libs/openFrameworks' -print -quit)"
if [[ -z "$of_lib" ]]; then
	echo "error: libs/openFrameworks not found in ${archive}" >&2
	exit 1
fi
OF_ROOT="$(cd "${of_lib}/../.." && pwd)"
if [[ ! -d "${OF_ROOT}/addons" || ! -d "${OF_ROOT}/scripts/templates" ]]; then
	echo "error: ${OF_ROOT} is not an openFrameworks release root" >&2
	exit 1
fi

PROJECT_DIR="${OF_ROOT}/apps/myApps/ofxMultiPlatform"
rm -rf "$PROJECT_DIR"
mkdir -p "$PROJECT_DIR"
# Copy the template without build output or VCS metadata.
tar -C "$REPO_ROOT" \
	--exclude .git \
	--exclude bin \
	--exclude obj \
	--exclude build \
	-cf - . | tar -C "$PROJECT_DIR" -xf -

echo "==> OF_ROOT=${OF_ROOT}"
echo "==> PROJECT_DIR=${PROJECT_DIR}"

if [[ -n "${GITHUB_ENV:-}" ]]; then
	{
		echo "OF_ROOT=${OF_ROOT}"
		echo "PROJECT_DIR=${PROJECT_DIR}"
	} >> "$GITHUB_ENV"
fi
