#!/usr/bin/python2.5

import osso
import hildon
import gtk

# Enable '?' button in the titlebar
def button_handler(widget, osso_hlp):
    dialog = gtk.AboutDialog()
    dialog.set_name("Help example")
    osso_hlp.dialog_help_enable(dialog, "Example_MaemoPad_Content")
    dialog.run()

# Show the selected help topic
def show_help(widget, osso_hlp):
    osso_hlp.show("Example_MaemoPad_Content", 1)

# The "?" only shows up when the referenced help topic is installed.
# Here we use the MaemoPad help file.
def main():
    program  = hildon.Program()
    window = hildon.Window()

    window.connect('destroy', gtk.main_quit)
    window.set_title ("Help example")

    osso_c = osso.Context("osso_test_help", "0.0.1", False)
    osso_hlp = osso.Help(osso_c)

    vbox = gtk.VBox(True)

    button = gtk.Button("Click to open a dialog with help button")
    button.connect("clicked", button_handler, osso_hlp)
    vbox.pack_start(button)

    button = gtk.Button("Click to open help")
    button.connect("clicked", show_help, osso_hlp)
    vbox.pack_start(button)

    window.add(vbox)

    window.show_all()
    gtk.main()

if __name__ == "__main__":
    main()
