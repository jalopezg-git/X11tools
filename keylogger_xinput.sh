#!/bin/bash

# Matches the `Class originated from: <id>` for the first device whose class is `XIKeyClass`, i.e.
# the id of the slave device that gives its class to the first master keyboard.
KEYBOARD_ID=$(LANG=C xinput list --long | grep -Fw --max-count=1 XIKeyClass | grep -Eo '[0-9]+')

{ xmodmap -pke; xinput test $KEYBOARD_ID; } | awk '
/keycode +[0-9]+/ {    # in `xmodmap -pke` output
	 keycode[$2] = $4;
}
/key press +[0-9]+/ {	# in `xinput test` output
	 c = keycode[$3];
	 printf(length(c) == 1 ? "%s" : "<%s>", c);
}'
