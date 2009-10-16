include "helper.pyx"
from libosso cimport osso_return_t, osso_system_note_type_t, osso_rpc_t, OSSO_OK, osso_system_note_dialog, osso_system_note_infoprint, OSSO_GN_WARNING, OSSO_GN_ERROR, OSSO_GN_WAIT, OSSO_GN_NOTICE
from context cimport Context
from exceptions import OssoException

cdef class SystemNote:
    def __cinit__(self, Context context not None):
        self.ctx = context.ctx

    def system_note_dialog(self, char *message, char *type_string = NULL):
        cdef osso_return_t ret
        cdef osso_rpc_t retval
        cdef osso_system_note_type_t type = OSSO_GN_NOTICE 

        if type_string:
            if type_string == "warning":
                type = OSSO_GN_WARNING
            elif type_string == "error":
                type = OSSO_GN_ERROR
            elif type_string == "wait":
                type = OSSO_GN_WAIT

        ret = osso_system_note_dialog(self.ctx, message, type, &retval)

        if ret != OSSO_OK:
            _set_exception(ret, &retval)
        else:
           return _rpc_t_to_python(&retval)

    def system_note_infoprint(self, char *text):
        cdef osso_return_t ret
        cdef osso_rpc_t retval
        ret = osso_system_note_infoprint(self.ctx, text, &retval)
        if ret != OSSO_OK:
            _set_exception(ret, &retval)
        else:
           return _rpc_t_to_python(&retval)
