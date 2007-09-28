# Simple example of using osso.ic module
# Author: Lauro Moura <lauro.neto@indt.org.br>


import osso.ic
import gnome.gconf
import gobject 
import gtk
import hildon

class ExampleIc(hildon.Program):
    def __init__(self):
        hildon.Program.__init__(self)

        self.stat_handle = None

        self.window = hildon.Window()
        self.window.connect("destroy", self._exit)

        self.buttons = gtk.HBox()
        self.bt_connect = gtk.Button(stock=gtk.STOCK_CONNECT)
        self.bt_disconnect = gtk.Button(stock=gtk.STOCK_DISCONNECT)

        self.buttons.pack_start(self.bt_connect)
        self.buttons.pack_start(self.bt_disconnect)

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

        print "_ic_cb()"
        print "evt = ", evt
        print args

        if evt.type == osso.ic.CONNECTED:
            self.status.set_text("Connected")

            if self.stat_handle == None:
                self.stat_handle = gobject.timeout_add(1000, self._req_stats)

        elif evt.type == osso.ic.DISCONNECTED:
            self.status.set_text("Disconnected")

            if self.stat_handle != None:
                gobject.source_remove(self.stat_handle)
                self.stat_handle = None

        elif evt.type == osso.ic.STATISTICS:
            print "Statistics event"
            self._update_stats(evt)

        elif evt.type == osso.ic.ERROR:
            print "Error connecting"
            print evt.error_code

    def _bt_connect_cb(self, bt):
        print "_bt_connect_cb"
        print osso.ic.connect(osso.ic.IAP_ANY, osso.ic.REQUESTED_CONNECT)
        self.status.set_text("Connecting...")

    def _bt_disconnect_cb(self, bt):
        print "_bt_disconnect_cb"
        print osso.ic.disconnect(osso.ic.IAP_ANY)
        self.status.set_text("Disconnecting...")

    def _update_stats(self, evt):
        print "_update_stats"

        print evt.time_active
        print evt.signal_strength
        print evt.rx_packets
        print evt.tx_packets
        print evt.rx_bytes
        print evt.tx_bytes

        self.time.set_text(str(evt.time_active))
        self.signal.set_text(str(evt.signal_strength))
        self.rx_packets.set_text(str(evt.rx_packets))
        self.tx_packets.set_text(str(evt.tx_packets))
        self.tx_bytes.set_text(str(evt.tx_bytes))
        self.rx_bytes.set_text(str(evt.rx_bytes))

    def _req_stats(self):
        if self.status.get_text() == "Connected": # Ugly...
            osso.ic.get_statistics()
        else:
            return False

        return True

if __name__ == "__main__":
    main = ExampleIc()
    main.run()
