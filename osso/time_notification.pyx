include "helper.pyx"
from libosso cimport osso_return_t, OSSO_OK, osso_rpc_t
from context cimport Context
from exceptions import OssoException
from libosso cimport osso_time_set, osso_time_set_notification_cb
from traceback import print_exc
import time

cdef void _time_callback_handler(void *data) with gil:
    time = <TimeNotification>data
    try:
        time.cb_data[0](<object>time.cb_data[1])
    except:
        print_exc()

cdef class TimeNotification:
    def __cinit__(self, Context context not None):
        self.ctx = context.ctx

    def set_time(self, new_time):
        if not isinstance(new_time, (tuple, time.struct_time)):
            raise TypeError, "time argument must be a tuple."

        cdef tm t
        (t.tm_year, t.tm_mon, t.tm_mday,
         t.tm_hour, t.tm_min, t.tm_sec,
         t.tm_wday, t.tm_yday, t.tm_isdst) = new_time

        # remove Python adjustments
        t.tm_year = t.tm_year - 1900
        t.tm_mon -= 1

        cdef time_t epoch = mktime(&t)
        cdef osso_return_t ret
        ret = osso_time_set(self.ctx, epoch)
        if ret != OSSO_OK:
            _set_exception(ret, NULL)

    def set_time_notification_callback(self, callback, user_data=None):
        cdef osso_return_t ret

        if callback:
            if not callable(callback):
                raise TypeError, "callback parameter must be callable"

            self.cb_data = (callback, user_data)
            ret = osso_time_set_notification_cb(self.ctx, _time_callback_handler, <void*>self)

            if ret != OSSO_OK:
                _set_exception(ret, NULL)
