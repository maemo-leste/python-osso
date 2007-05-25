import osso.ic
import gnome.gconf
import gobject 
import gtk
import hildon

class ExampleIc(hildon.Program):
    def __init__(self):
        hildon.Program.__init__(self)

        self.window = hildon.Window()
        self.window.connect("destroy", self._exit)

        self.buttons = gtk.HBox()
        self.bt_connect = gtk.Button(stock=gtk.STOCK_CONNECT)
        self.bt_disconnect = gtk.Button(stock=gtk.STOCK_DISCONNECT)

        self.buttons.pack_start(self.bt_connect)
        self.buttons.pack_start(self.bt_disconnect)

        self.bt_disconnect.set_sensitive(False)

        self.bt_connect.connect("clicked", self._bt_connect_cb)
        self.bt_disconnect.connect("clicked", self._bt_disconnect_cb)

        self.vbox = gtk.VBox()

        self.cells = gtk.HBox()
        self.labels = gtk.VBox()
        self.entries = gtk.VBox()

        self.labels.pack_start(gtk.Label("IAP Name"))
        self.labels.pack_start(gtk.Label("Status"))
        self.labels.pack_start(gtk.Label("Time Active"))
        self.labels.pack_start(gtk.Label("Signal Strength"))
        self.labels.pack_start(gtk.Label("Received packets"))
        self.labels.pack_start(gtk.Label("Transmitted packets"))
        self.labels.pack_start(gtk.Label("Received bytes"))
        self.labels.pack_start(gtk.Label("Transmitted bytes"))

        self.entry_name = gtk.Entry()
        self.entries.pack_start(self.entry_name)

        self.status = gtk.Entry()
        self.entries.pack_start(self.status)

        self.time = gtk.Entry()
        self.entries.pack_start(self.time)

        self.signal = gtk.Entry()
        self.entries.pack_start(self.signal)

        self.rx_packets = gtk.Entry()
        self.entries.pack_start(self.rx_packets)

        self.tx_packets = gtk.Entry()
        self.entries.pack_start(self.tx_packets)

        self.rx_bytes = gtk.Entry()
        self.entries.pack_start(self.rx_bytes)

        self.tx_bytes = gtk.Entry()
        self.entries.pack_start(self.tx_bytes)


        self.cells.pack_start(self.labels)
        self.cells.pack_start(self.entries)
        self.vbox.pack_start(self.cells)
        self.vbox.pack_start(self.buttons)

        self.window.add(self.vbox)

        osso.ic.set_callback(self._ic_cb)

        self.window.show_all()
    
    def _exit(self, widget):
        gtk.main_quit()

    def run(self):
        gtk.main()

    def _ic_cb(self, evt, *args):
        if evt.type == osso.ic.IAP_CONNECTED:
            self.bt_connect.set_sensitive(False)
            self.bt_disconnect.set_sensitive(True)
        elif evt.type == osso.ic.IAP_DISCONNECTED:
            self.bt.disconnect.set_sensitive(False)
            self.bt_connect.set_sensitive(True)
        elif evt.type == osso.ic.IAP_STATISTICS:
            self._update_stats(evt)
        elif evt.type == osso.ic.IAP_ERROR:
            pass

    def _bt_connect_cb(self, bt):
        osso.ic.connect(osso.ic.IAP_ANY, osso.ic.IAP_REQUESTED_CONNECTION)

    def _bt_disconnect_cb(self, bt):
        pass

    def _update_stats(self, evt):
        pass

if __name__ == "__main__":
    main = ExampleIc()
    main.run()
