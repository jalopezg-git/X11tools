#!/usr/bin/perl
use X11::Protocol;
use X11::AtomConstants;

# Announce fake support for `_GTK_FRAME_EXTENTS` even if the WM does not support it.
# This has the effect of enabling full GTK client-side decoration.
$X = X11::Protocol->new();
$X->ChangeProperty($X->{"root"}, $X->atom("_NET_SUPPORTED"),
		   X11::AtomConstants::ATOM, 32,
		   "Append", pack("L", $X->atom("_GTK_FRAME_EXTENTS")));
