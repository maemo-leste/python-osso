include "helper.pyx"

from context cimport Context
from libosso cimport osso_return_t, OSSO_OK
from libosso cimport osso_mime_cb_f, osso_mime_set_cb, osso_mime_unset_cb, osso_mime_unset_cb_full
from traceback import print_exc


cdef void _mime_callback_handler(void *data, int argc, char **argv) with gil:
    mime = <Mime>data

    args = []
    for i in range(argc):
        args.append(argv[i])

    try:
        mime.cb_data[0](<object>mime.cb_data[1], tuple(args))
    except:
        print_exc()
        pass


cdef class Mime:
    def __cinit__(self, Context context not None):
        self.ctx = context.ctx

    def set_mime_callback(self, callback, user_data=None):
        cdef osso_return_t ret

        if callback:
            if not callable(callback):
                raise TypeError, "callback parameter must be callable"

            self.cb_data = (callback, user_data)
            ret = osso_mime_set_cb(self.ctx, _mime_callback_handler, <void*>self)

            if ret != OSSO_OK:
                _set_exception(ret, NULL)

    def unset_mime_callback(self):
        cdef osso_return_t ret

        ret = osso_mime_unset_cb(self.ctx)
        if ret != OSSO_OK:
            _set_exception(ret, NULL)

    def unset_mime_callback_full(self, callback, user_data=None):
        cdef osso_return_t ret

        ret = osso_mime_unset_cb_full(self.ctx, _mime_callback_handler, <void*>self)
        if ret != OSSO_OK:
            _set_exception(ret, NULL)
