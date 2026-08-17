# host — bar widget + daemon for this keymap

The Omarchy bar widget now lives in its own repo:
<https://github.com/alvarosaavedra/omarchy-sofle-layer>

What remains here:

- `sofle-layerd` / `sofle-layer.service` — unchanged canonical copies kept
  for this machine; the plugin repo ships identical ones. The live
  deployment on this machine runs from the plugin repo (see install.sh).
- `install.sh` — wires this machine to the plugin repo layout.
