#!/bin/bash

EXTRA_PATH="$HOME/klipper/klippy/extras/zerod.py"
if [ -e "$EXTRA_PATH" ]; then
  rm "$EXTRA_PATH"
fi
ln -s "$(pwd)/$(dirname "$0")/extra/zerod.py" "$EXTRA_PATH"

if ! grep -q "klippy/extras/zerod.py" "$HOME/klipper/.git/info/exclude"; then
  echo "klippy/extras/zerod.py" >> "$HOME/klipper/.git/info/exclude"
fi
