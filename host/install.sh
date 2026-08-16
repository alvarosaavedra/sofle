#!/usr/bin/env bash
# Install the Sofle layer widget toolchain on an Omarchy machine:
#   - symlinks the shell plugin into ~/.config/omarchy/plugins/
#   - installs the sofle-layerd daemon + systemd user unit
#   (re)generates the layer diagram data from keymap.c
#
# After running, register the widget in ~/.config/omarchy/shell.json:
#   - add { "id": "alvarosaavedra.sofle-layer" } to "plugins"
#   - add the same entry to the desired "bar.layout" section
# and flash firmware built from this repo (layer reporting comes from it).

set -euo pipefail
repo="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

plugins=~/.config/omarchy/plugins
mkdir -p "$plugins" ~/.local/bin ~/.config/systemd/user

ln -sfn "$repo/host/plugin" "$plugins/alvarosaavedra.sofle-layer"
ln -sfn "$repo/host/sofle-layerd" ~/.local/bin/sofle-layerd
ln -sfn "$repo/host/sofle-layer.service" ~/.config/systemd/user/sofle-layer.service

python3 "$repo/tools/gen-layout-json.py" "$repo/keyboards/sofle/keymaps/radbug/keymap.c" \
    -o ~/.local/share/sofle/layers.json

systemctl --user daemon-reload
systemctl --user enable --now sofle-layer.service

echo "Installed. Register alvarosaavedra.sofle-layer in shell.json (see script header) and restart the shell with: omarchy restart shell"
