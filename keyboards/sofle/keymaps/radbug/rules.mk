# Keep VIA support so the keymap stays editable in-browser (usevia.app)
VIA_ENABLE = yes

# Debug console: layer_state_set_user() prints "sofle layer=N" on every layer
# change; the host daemon (sofle-layerd) reads it to drive the bar widget.
CONSOLE_ENABLE = yes

# Link-time optimization: the split-sync + OLED + console feature set doesn't
# fit in the Pro Micro's 28K without it.
LTO_ENABLE = yes

# Size trims to fit OLED + split sync + console in 28K: the board has a
# physical reset button (no bootmagic needed) and magic keycodes are unused.
BOOTMAGIC_ENABLE = no
MAGIC_ENABLE = no

# OLED status: WPM counter + split sync so the slave (right) half's OLED
# mirrors layer/mods/WPM state. The actual defines live in config.h —
# SPLIT_*_ENABLE are not rules.mk options.
WPM_ENABLE = yes

# Encoder map defined in keymap.c (left: volume, right: media)
ENCODER_MAP_ENABLE = yes
