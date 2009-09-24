#!/usr/bin/python2.5
import osso
import hildon
import gtk

"Tests osso.Application.application_top()"

def send_rpc(widget, osso_c):
    osso_app = osso.Application(osso_c)
    osso_app.application_top("spam.eggs.osso_test_app", None)
    print "RPC sent"

def main():
    osso_c = osso.Context("osso_test_sender", "0.0.1", False)
    window = hildon.Window()
    window.connect("destroy", gtk.main_quit)
    send_button = gtk.Button("Put example app to foreground")
    window.add(send_button)
    send_button.connect("clicked", send_rpc, osso_c)
    window.show_all()
    gtk.main()

if __name__ == "__main__":
    main()
