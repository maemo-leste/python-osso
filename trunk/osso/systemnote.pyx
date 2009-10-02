from libosso cimport osso_return_t, osso_system_note_type_t, osso_rpc_t, OSSO_OK, osso_system_note_dialog, osso_system_note_infoprint
from context cimport Context
from exceptions import OssoException

cdef class SystemNote:
    def __cinit__(self, Context context not None):
        self.ctx = context.ctx

    def system_note_dialog(self, char *message, osso_system_note_type_t type):
        cdef osso_return_t ret
        cdef osso_rpc_t retval
        ret = osso_system_note_dialog(self.ctx, message, type, &retval)
        if ret != OSSO_OK:
            raise OssoException, "OSSO Error."
    def system_note_infoprint(self, char *text):
        cdef osso_return_t ret
        cdef osso_rpc_t retval
        ret = osso_system_note_infoprint(self.ctx, text, &retval)
        if ret != OSSO_OK:
            raise OssoException, "OSSO Error."
