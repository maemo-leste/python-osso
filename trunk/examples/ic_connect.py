# Tests osso.ic.connect and callback functions
# Author: Lauro Moura <lauro.neto@indt.org.br>


import osso.ic
import gobject # For the main Loop

def ic_cb(evt, data):
    print "osso.ic Python callback"

    print "User data = ", data
    
    if evt.type == osso.ic.CONNECTED:
        print "connected"
    elif evt.type == osso.ic.DISCONNECTED:
        print "disconnected"
    elif evt.type == osso.ic.STATISTICS:
        print "statistics event"
    elif evt.type == osso.ic.ERROR:
        print "error event - code %x" % evt.error_code
    else:
        print "Unknown event"

def connect():
    osso.ic.set_callback(ic_cb)
    print osso.ic.connect(osso.ic.IAP_ANY, osso.ic.REQUESTED_CONNECT, ("Foo data", 3, 4, True))
    return False

def main():
    loop = gobject.MainLoop()
    gobject.idle_add(connect)
    loop.run()

if __name__ == "__main__":
    main()
