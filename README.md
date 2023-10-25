# X11tools
This repository contains a number of X11 utilities and/or experiments.  The author assumes no responsibility for the malicious / inappropriate use of these programs.

- *keylogger_Xi.c*: a XInput `XGrabDevice()`-based keylogger.
- *keylogger_xinput.sh*: abuse `xinput test` (see xinput(1)) to log keypresses.
- *fake_GTK_FRAME_EXTENTS.pl*: announce (fake) support for `GTK_FRAME_EXTENTS` as part of the `NET_SUPPORTED` property.  If a compositing manager is running, this can be used to enable full GTK client-side decoration on some WMs, e.g. openbox (see also [this](https://github.com/danakj/openbox/pull/41) related pull request).
