#!/usr/bin/env python3
##===================================================================##
 # SPDX-License-Identifier: GPL-3.0-or-later                         #
 # Copyright (c) 2025 Jimmy Källhagen                                #
 # Part of Yggdrasil - Nordix desktop environment                    #
 # Nordix and Yggdrasil are registered trademarks of Jimmy Källhagen # 
##===================================================================##

"""
nordix-shutdown-download.py

Popup dialog shown when a significant download is detected (>40% of
connection speed) during Nordix graceful shutdown.

Reads env var NORDIX_DL_SPEED for the measured speed in Mbit/s.

Three options:
  1. Wait until download finishes, then continue shutdown automatically
  2. Continue shutdown anyway (ignore download)
  3. Cancel shutdown entirely

Exit codes:
  0 = continue shutdown
  1 = abort shutdown
"""

import gi
gi.require_version("Gtk", "3.0")
from gi.repository import Gtk, GLib
import os
import sys
import subprocess

SPEED = os.environ.get("NORDIX_DL_SPEED", "?")

# We consider the download "finished" when speed drops below this for
# two consecutive checks (Mbit/s).  40% of 100 = 40.
LOW_THRESHOLD_MBIT = 5.0
CHECK_INTERVAL_SEC = 10


def get_default_iface():
    try:
        out = subprocess.check_output(
            ["sh", "-c", "ip route show default | awk '{print $5; exit}'"],
            text=True, stderr=subprocess.DEVNULL
        )
        return out.strip() or None
    except Exception:
        return None


def read_rx_bytes(iface):
    path = f"/sys/class/net/{iface}/statistics/rx_bytes"
    try:
        with open(path) as f:
            return int(f.read().strip())
    except Exception:
        return None


def measure_speed_mbit(iface):
    import time
    rx1 = read_rx_bytes(iface)
    if rx1 is None:
        return -1.0
    t1 = time.monotonic()
    time.sleep(1.0)
    t2 = time.monotonic()
    rx2 = read_rx_bytes(iface)
    if rx2 is None:
        return -1.0
    elapsed = t2 - t1
    return ((rx2 - rx1) / elapsed) * 8.0 / 1e6


class DownloadDialog(Gtk.Window):
    def __init__(self):
        super().__init__(title="Nordix Shutdown")
        self.set_default_size(520, 300)
        self.set_position(Gtk.WindowPosition.CENTER)
        self.set_resizable(False)
        self.set_keep_above(True)
        self.set_deletable(False)

        self.result = 1  # default: abort
        self.low_count = 0

        vbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=16)
        vbox.set_margin_top(24)
        vbox.set_margin_bottom(24)
        vbox.set_margin_start(24)
        vbox.set_margin_end(24)
        self.add(vbox)

        icon = Gtk.Image.new_from_icon_name("network-transmit-receive",
                                            Gtk.IconSize.DIALOG)
        vbox.pack_start(icon, False, False, 0)

        header = Gtk.Label()
        header.set_markup("<big><b>Aktiv nedladdning pågår</b></big>")
        vbox.pack_start(header, False, False, 0)

        info = Gtk.Label()
        info.set_markup(
            f"Nedladdningshastighet: <b>{SPEED} Mbit/s</b>\n\n"
            "En nedladdning upptäcktes som överstiger tröskelvärdet.\n"
            "Välj hur du vill fortsätta:"
        )
        info.set_line_wrap(True)
        info.set_justify(Gtk.Justification.CENTER)
        vbox.pack_start(info, False, False, 0)

        # Status label for wait mode
        self.status_label = Gtk.Label(label="")
        vbox.pack_start(self.status_label, False, False, 0)

        self.spinner = Gtk.Spinner()
        vbox.pack_start(self.spinner, False, False, 0)

        # Buttons
        btn_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=12)
        btn_box.set_halign(Gtk.Align.CENTER)
        vbox.pack_end(btn_box, False, False, 0)

        btn_wait = Gtk.Button(label="Vänta tills klar")
        btn_wait.get_style_context().add_class("suggested-action")
        btn_wait.connect("clicked", self.on_wait)
        btn_box.pack_start(btn_wait, False, False, 0)

        btn_ignore = Gtk.Button(label="Fortsätt ändå")
        btn_ignore.connect("clicked", self.on_ignore)
        btn_box.pack_start(btn_ignore, False, False, 0)

        btn_cancel = Gtk.Button(label="Avbryt shutdown")
        btn_cancel.get_style_context().add_class("destructive-action")
        btn_cancel.connect("clicked", self.on_cancel)
        btn_box.pack_start(btn_cancel, False, False, 0)

        self.btn_box = btn_box
        self.show_all()
        self.spinner.hide()

    def on_wait(self, _btn):
        self.status_label.set_markup(
            "<i>Väntar på att nedladdningen ska bli klar...</i>\n"
            "<small>Shutdown fortsätter automatiskt.</small>"
        )
        self.spinner.show()
        self.spinner.start()
        self.btn_box.set_sensitive(False)
        self.low_count = 0
        GLib.timeout_add_seconds(CHECK_INTERVAL_SEC, self._check_speed)

    def _check_speed(self):
        iface = get_default_iface()
        if not iface:
            self.result = 0
            Gtk.main_quit()
            return False

        speed = measure_speed_mbit(iface)
        self.status_label.set_markup(
            f"<i>Aktuell hastighet: {speed:.1f} Mbit/s</i>\n"
            "<small>Shutdown fortsätter när hastigheten sjunker.</small>"
        )

        if speed < LOW_THRESHOLD_MBIT:
            self.low_count += 1
        else:
            self.low_count = 0

        # Require 2 consecutive low readings
        if self.low_count >= 2:
            self.result = 0
            Gtk.main_quit()
            return False

        return True  # keep checking

    def on_ignore(self, _btn):
        self.result = 0
        Gtk.main_quit()

    def on_cancel(self, _btn):
        self.result = 1
        Gtk.main_quit()


if __name__ == "__main__":
    win = DownloadDialog()
    win.connect("destroy", Gtk.main_quit)
    Gtk.main()
    sys.exit(win.result)
