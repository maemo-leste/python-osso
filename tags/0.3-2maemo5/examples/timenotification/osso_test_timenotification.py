#!/usr/bin/python2.5

import osso
import time

import gobject

def time_cb(time_seq):
    print time_seq

def main():
    
    osso_c = osso.Context("osso_test_timenotification", "0.0.1", False)
    timenot = osso.TimeNotification(osso_c)

    timenot.set_time_notification_callback(time_cb)
   
    old_time = time.localtime()

    time.sleep(2)

    timenot.set_time(old_time)
    
    loop = gobject.MainLoop()
    loop.run()

if __name__ == "__main__":
    main()
