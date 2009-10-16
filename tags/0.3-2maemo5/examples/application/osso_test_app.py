#!/usr/bin/python2.5
import osso
import hildon
import gtk

def main():
    osso_c = osso.Context("osso_test_app", "0.0.1", False)
    print "osso_test_app started"
    program  = hildon.Program()
    window = hildon.Window()
    label = gtk.Label("Sample app")
    window.add(label)

    window.show_all()
    gtk.main()

main()
