include "helper.pyx"

from context cimport Context
from libosso cimport osso_return_t, OSSO_OK
from libosso cimport osso_locale_change_set_notification_cb, osso_locale_set, const_char_ptr
from traceback import print_exc

cdef void _locale_callback_handler(const_char_ptr new_locale, void *data) with gil:
    locale = <Locale>data
    try:
        locale.cb_data[0](new_locale, <object>locale.cb_data[1])
    except:
        print_exc()
        pass


cdef class Locale:
    def __cinit__(self, Context context not None):
        self.ctx = context.ctx

    def set_locale_notification_callback(self, callback, user_data=None):
        cdef osso_return_t ret

        if callback:
            if not callable(callback):
                raise TypeError, "callback parameter must be callable"

            self.cb_data = (callback, user_data)
            ret = osso_locale_change_set_notification_cb(self.ctx, _locale_callback_handler, <void*>self)

            if ret != OSSO_OK:
                _set_exception(ret, NULL)

    def set_locale(self, new_locale):
        cdef osso_return_t ret

        ret = osso_locale_set(self.ctx, new_locale)
        if ret != OSSO_OK:
            _set_exception(ret, NULL)
