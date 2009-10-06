from libosso cimport osso_return_t, OSSO_OK, osso_rpc_t
include "helper.pyx"

from context cimport Context
from exceptions import OssoException
from libosso cimport osso_time_set, osso_time_set_notification_cb
from traceback import print_exc

cdef void _time_callback_handler(void *data) with gil:
    time = <Time>data
    try:
        time.cb_data[0](<object>time.cb_data[1])
    except:
        #print_exc()
        pass


cdef class Time:
    def __cinit__(self, Context context not None):
        self.ctx = context.ctx

    def set_time(self, new_time):
        cdef osso_return_t ret

        ret = osso_time_set(self.ctx, new_time)
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


