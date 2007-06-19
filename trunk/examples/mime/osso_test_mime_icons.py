#!/usr/bin/python2.5

import sys
import hildon
import gtk
from gnome import gnomevfs
import osso

class IconTest(hildon.Program):
    def __init__(self):
        hildon.Program.__init__(self)

        self.window = hildon.Window()
        self.window.connect("delete-event", gtk.main_quit)
        self.add_window(self.window)

        # Osso stuff
        self.osso_c = osso.Context("osso_test_mime_icons", "0.0.1", False)
        self.mime = osso.Mime(self.osso_c)

        # Remaining widgets
        vbox = gtk.VBox()

        self.entry = gtk.Entry()
        vbox.pack_start(self.entry, expand=False, fill=True)

        self.button = gtk.Button("Ok")
        self.button.connect("clicked", self.button_cb)
        vbox.pack_start(self.button, expand=False, fill=True)

        scroll = gtk.ScrolledWindow()
        vbox.pack_start(scroll)

        self.model = gtk.ListStore(gtk.gdk.Pixbuf, str)

        self.iconview = gtk.IconView(self.model)
        self.iconview.set_pixbuf_column(0)
        self.iconview.set_text_column(1)
        self.iconview.set_margin(12)
        self.iconview.set_item_width(125)
        scroll.add(self.iconview)


        self.window.add(vbox)
        self.window.show_all()

    def button_cb(self, button):
        print "Button clicked"
        fileinfo = gnomevfs.FileInfo()
        fileinfo.valid_fields |= gnomevfs.FILE_INFO_FIELDS_TYPE
        fileinfo.type = gnomevfs.FILE_TYPE_DIRECTORY

        names = self.mime.get_icon_names(self.entry.get_text())

        icontheme = gtk.IconTheme()

        self.model.clear()

        for i in names:
            supported = icontheme.has_icon(i)
            print "Found icon %s. Supported: " % (i,), supported
            if supported:
                pixbuf = icontheme.load_icon(i, 48, gtk.ICON_LOOKUP_USE_BUILTIN)
                #TODO Set the model
                self.model.append((pixbuf, i,))

    def run(self):
        gtk.main()

if __name__ == "__main__":
    prog = IconTest()
    prog.run()
    
