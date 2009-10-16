include "helper.pyx"

from libosso cimport osso_return_t, osso_statusbar_send_event, OSSO_OK, osso_rpc_t
from context cimport Context
from exceptions import OssoException

cdef class StatusBar:
    def __cinit__(self, Context context not None):
        self.ctx = context.ctx

    def statusbar_send_event(self, char *name, int argument1, int argument2, char *argument3):
        cdef osso_return_t ret
        cdef osso_rpc_t rpc

        ret = osso_statusbar_send_event(self.ctx, name, argument1, argument2, argument3, &rpc)
        if ret != OSSO_OK:
            _set_exception(ret, &rpc)

        return _rpc_t_to_python(&rpc)

