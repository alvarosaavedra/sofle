# Sofle v2 — radbug keymap

QMK keymap for my [Sofle v2](https://github.com/josefadamcik/SofleKeyboard) split keyboard
(`fc32:0287`, Pro Micro build), maintained as a QMK userspace repo and tuned for
[Omarchy](https://omarchy.org/) (Hyprland).

The baseline was transcribed byte-for-byte from the keyboard's stock firmware over the
VIA protocol — see `docs/firmware-snapshot.json` for the raw dump.

## Layers

### Base (Layer 0)
```
,-----------------------------------------.                    ,-----------------------------------------.
| ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | BSPC |
|------+------+------+------+------+------|                    |------+------+------+------+------+------|
| TAB  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  -_  |
|------+------+------+------+------+------|                    |------+------+------+------+------+------|
| LCTL |   A  |   S  |   D  |   F  |   G  |-------.  ,---------|   H  |   J  |   K  |   L  |   ;  |  '"  |
`------+   Z  |   X  |   C  |   V  |   B  | MUTE  |  | PLAY   |   N  |   M  |   ,  |   .  |   /  |------'
       `----------------------------------/       /   \        \-----------------------------------------'
                  | LALT | LGUI |LOWER |  / SPACE /     \ ENTER \  | RAISE| RGUI | RALT |
                  `----------------------------'       \       -----------------+--------------------'
```

### Lower (Layer 1, hold left thumb `[{`)
Media cluster, F1–F12, shifted symbols, `Del`, `Caps Lock` (which Omarchy remaps to Compose).

### Raise (Layer 2, hold right thumb `}]`)
F1–F12, arrows on HJKL, symbols, and a nav cluster on Z/X/C/V (`Home`/`End`/`PgUp`/`PgDn`).

### Omarchy bindings this keymap is built around

| Binding | Keys |
|---|---|
| Screenshot | `Lower + 6` (Print) |
| Color picker / screenrecord | `Super + Print` / `Alt + Print` |
| Window focus / swap | `Super + HJKL arrows` (Raise) |
| Workspaces 1–10 | `Super + number row` |
| Launcher / window ops | `Super` on both thumb keys |

## Build

Requires `qmk` CLI, `avr-gcc`, `avr-libc`, and a clone of `qmk_firmware` at `~/qmk_firmware`:

```bash
qmk compile -kb sofle/rev1 -km radbug
```

## Flash

Reset the left half (double-tap reset or short RST-GND twice on Pro Micro) while it's
the only half plugged in, then:

```bash
qmk flash -kb sofle/rev1 -km radbug -bl caterina
```

Repeat with the right half. (Each half carries the full keymap; QMK's split handling
routes events between them.)

## Live editing (without reflashing)

The firmware keeps `VIA_ENABLE = yes`, so the layout can also be tweaked at runtime in
the browser at <https://usevia.app> (needs the udev rule below). Runtime changes are
*not* version-controlled here — to make a change permanent, edit `keymap.c`, commit,
and flash.

```udev
# /etc/udev/rules.d/99-vial.rules
KERNEL=="hidraw*", SUBSYSTEM=="hidraw", ATTRS{idVendor}=="fc32", ATTRS{idProduct}=="0287", MODE="0660", TAG+="uaccess", RUN{builtin}+="uaccess"
```

## License

GPL-2.0-or-later (QMK derivative). See `keymap.c` headers.
