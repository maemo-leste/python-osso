#!/usr/bin/python2.5

import osso
import gobject

# Example of using osso.DeviceState.display_blanking_pause
# You need to set the display brightness period to 10 seconds. 
# This example will request once to pause the blaking.

def quit_cb(loop):
    loop.quit()
    return False

def main():
    global count
    loop = gobject.MainLoop()
    osso_c = osso.Context("osso_test_device_on", "0.0.1", False)
    device = osso.DeviceState(osso_c)

    device.display_blanking_pause()
    
    gobject.timeout_add(15000, quit_cb, loop)

    loop.run()

if __name__ == "__main__":
    main()
