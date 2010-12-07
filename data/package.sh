#!/bin/sh

_dirname() { if [[ "$1" =~ / ]]; then echo "${1%/*}"; else echo .; fi; }

extdir="$(_dirname "$0")/fatrat-chrome/"
cp "$1" "$extdir"

if which chrome; then
		cmd="chrome"
elif which chromium; then
		cmd="chromium"
elif which chromium-browser; then
		cmd="chromium-browser"
else
		echo "Google Chrome not found, the extension cannot be packaged"
		exit 1
fi

$cmd "--pack-extension=$extdir" --no-message-box
