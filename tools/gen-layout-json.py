#!/usr/bin/env python3
"""Generate layers.json for the Omarchy bar widget from keymap.c.

Parses the radbug Sofle keymap (one visual row per source line inside each
`LAYOUT(...)` block) and emits the pretty-printed, display-ready form the
alvarosaavedra.sofle-layer bar widget renders:

    { "layers": [ { "name": "QWERTY", "rows": [ [ {"t": "Esc", "c": "k"}, ...
                                               {"t": "", "c": "gap"} ... ] ] } ] }

Key classes (c): k plain, mod modifier, mo layer switch, media, nav, fn,
trans transparent (label inherited from the base layer, drawn dimmed),
blank KC_NO, gap spacer between the halves, enc encoder-push key.
Wide keys (Space/Enter) carry "w": 1.6.

Only the keycode vocabulary actually used by this keymap is translated;
anything unknown aborts with an error so silent drift can't creep in.

Usage: gen-layout-json.py [keymap.c] [-o out.json] (default: stdout)
"""

import argparse
import json
import os
import re
import sys

PLAIN = {
    "KC_ESC": "Esc", "KC_BSPC": "Bksp", "KC_TAB": "Tab", "KC_CAPS": "Caps",
    "KC_DEL": "Del", "KC_PSCR": "PSc", "KC_ENT": "Enter", "KC_SPACE": "Space",
    "KC_MINS": "-", "KC_EQL": "=", "KC_GRV": "`", "KC_BSLS": "\\",
    "KC_LBRC": "[", "KC_RBRC": "]", "KC_SCLN": ";", "KC_QUOT": "'",
    "KC_COMM": ",", "KC_DOT": ".", "KC_SLSH": "/", "KC_INT3": "\u00a5",
    "KC_LSFT": "Shift", "KC_RSFT": "Shift", "KC_LCTL": "Ctrl",
    "KC_RCTL": "Ctrl", "KC_LALT": "Alt", "KC_RALT": "Alt",
    "KC_LGUI": "Super", "KC_RGUI": "Super",
    "KC_LEFT": "\u2190", "KC_DOWN": "\u2193", "KC_UP": "\u2191",
    "KC_RIGHT": "\u2192", "KC_HOME": "Home", "KC_END": "End",
    "KC_PGUP": "PgUp", "KC_PGDN": "PgDn",
    "KC_MUTE": "Mute", "KC_VOLD": "Vol-", "KC_VOLU": "Vol+",
    "KC_MPLY": "Play", "KC_MRWD": "Prev", "KC_MFFD": "Next",
}

SHIFTED = {
    "KC_1": "!", "KC_2": "@", "KC_3": "#", "KC_4": "$", "KC_5": "%",
    "KC_6": "^", "KC_7": "&", "KC_8": "*", "KC_9": "(", "KC_0": ")",
    "KC_MINS": "_", "KC_EQL": "+", "KC_GRV": "~", "KC_BSLS": "|",
    "KC_LBRC": "{", "KC_RBRC": "}", "KC_SCLN": ":", "KC_QUOT": '"',
    "KC_COMM": "<", "KC_DOT": ">", "KC_SLSH": "?",
}

MOD_CLASS = {
    "Shift", "Ctrl", "Alt", "Super", "Caps", "Lower", "Raise", "Adjust",
}

MEDIA_LABELS = {"Mute", "Vol-", "Vol+", "Play", "Prev", "Next"}
NAV_LABELS = {"\u2190", "\u2193", "\u2191", "\u2192", "Home", "End", "PgUp", "PgDn", "PSc", "Del"}
WIDE_LABELS = {"Space", "Enter"}

# Row shapes of the sofle LAYOUT macro: (entries, gap after index or None,
# encoder-push indexes). Row 3's two center entries are the encoder pushes.
ROW_SHAPES = [
    (12, 5, ()),      # numbers / qwerty / home
    (12, 5, ()),
    (12, 5, ()),
    (14, None, (6, 7)),  # shifted row + MUTE/PLAY encoder pushes in the middle
    (10, (3, 5), ()),    # thumbs: 4 left | space/enter | 4 right
]


def split_top_level(s):
    parts, depth, cur = [], 0, ""
    for ch in s:
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        if ch == "," and depth == 0:
            parts.append(cur.strip())
            cur = ""
        else:
            cur += ch
    if cur.strip():
        parts.append(cur.strip())
    return parts


def label_for(token, layer_names):
    if token == "_______":
        return None  # transparent, resolved against the base layer
    if token == "XXXXXXX":
        return {"t": "", "c": "blank"}
    m = re.fullmatch(r"MO\((\d+)\)", token)
    if m:
        idx = int(m.group(1))
        name = layer_names[idx] if idx < len(layer_names) else str(idx)
        return {"t": name, "c": "mo"}
    m = re.fullmatch(r"S\((KC_\w+)\)", token)
    if m:
        if m.group(1) not in SHIFTED:
            raise SystemExit(f"no shifted label for {token}")
        return {"t": SHIFTED[m.group(1)], "c": "k"}
    m = re.fullmatch(r"KC_F(\d+)", token)
    if m:
        return {"t": f"F{m.group(1)}", "c": "fn"}
    if token in PLAIN:
        text, cls = PLAIN[token], "k"
        if text in MOD_CLASS:
            cls = "mod"
        elif text in MEDIA_LABELS:
            cls = "media"
        elif text in NAV_LABELS:
            cls = "nav"
        return {"t": text, "c": cls}
    if re.fullmatch(r"KC_[A-Z0-9]", token):
        return {"t": token[3:], "c": "k"}
    raise SystemExit(f"unknown keycode token: {token}")


def parse_keymap(source):
    layer_names = re.findall(r"^\s*_([A-Z]+),", source, re.M)
    blocks = re.findall(r"\[_\w+\]\s*=\s*LAYOUT\((.*?)\n\s*\),", source, re.S)
    if len(blocks) != len(layer_names):
        raise SystemExit(f"found {len(blocks)} LAYOUT blocks vs {len(layer_names)} layers")

    layers = []
    for name, block in zip(layer_names, blocks):
        rows = [split_top_level(line) for line in block.splitlines() if line.strip()]
        if [len(r) for r in rows] != [s[0] for s in ROW_SHAPES]:
            raise SystemExit(f"layer {name}: row shapes {[len(r) for r in rows]} don't match sofle geometry")
        layers.append({"name": name.title(), "raw": rows})
    return layers


def build(layers):
    out = []
    for index, layer in enumerate(layers):
        display_rows = []
        for row, (entries, gap, encs) in zip(layer["raw"], ROW_SHAPES):
            gaps = set(gap) if isinstance(gap, tuple) else ({gap} if gap is not None else set())
            cells = []
            for i, token in enumerate(row):
                if i in gaps:
                    cells.append({"t": "", "c": "gap"})
                key = label_for(token, [l["name"] for l in layers])
                if key is None:  # _______: label resolved against the base layer
                    key = {"t": "", "c": "trans"}
                if i in encs and key["c"] not in ("trans", "blank"):
                    key["c"] = "enc"
                cells.append(key)
            display_rows.append(cells)
        out.append({"name": layer["name"], "index": index, "rows": display_rows})

    # second pass: fill trans labels from the base layer now that it exists
    base_rows = out[0]["rows"]
    for layer in out:
        for r, row in enumerate(layer["rows"]):
            for c, key in enumerate(row):
                if key["c"] == "trans":
                    key["t"] = base_rows[r][c]["t"]
    for layer in out:
        for row in layer["rows"]:
            for key in row:
                if key["t"] in WIDE_LABELS:
                    key["w"] = 1.6
    return {"layers": out}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("keymap", nargs="?", default="keyboards/sofle/keymaps/radbug/keymap.c")
    ap.add_argument("-o", "--output", help="output path (default: stdout)")
    args = ap.parse_args()

    with open(args.keymap) as f:
        source = f.read()
    data = build(parse_keymap(source))
    text = json.dumps(data, ensure_ascii=False, indent=1) + "\n"

    if args.output:
        os.makedirs(os.path.dirname(args.output), exist_ok=True)
        with open(args.output, "w") as f:
            f.write(text)
        print(f"wrote {args.output}", file=sys.stderr)
    else:
        sys.stdout.write(text)


if __name__ == "__main__":
    main()
