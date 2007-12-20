import time
from osso.alarmd import *

# set alarm for 60 seconds in the future
alarm_time=long(time.time()+5)
#recurrence=0
#recurrence_count=0
#snooze=0
title='testing'
message='batman is here!'
sound="/dev/null"
icon='/usr/share/pixmaps/carman.png'
#dbus_interface=None
#dbus_service=None
#dbus_path=None
#dbus_name=None
#exec_name=None
flags=568

id=add_alarm(alarm_time, snooze=1, title=title, message=message, sound=sound)

# take a look at /var/lib/alarmd/alarm_queue.xml to see if the alarm was created,
# or wait for the dialog to appear

#print get_alarm(id)

#time.sleep(20)

#cancel_alarm(id)
