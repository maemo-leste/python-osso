include "helper.pyx"
from libosso cimport osso_return_t, OSSO_OK, osso_cp_plugin_execute, osso_cp_plugin_save_state
from context cimport Context
from python_object cimport PyObject_HasAttrString


cdef class Plugin:
    def __cinit__(self, Context context not None):
        self.ctx = context.ctx

    def plugin_execute(self, filename, user_activated, user_data=None):
        cdef void *data
        cdef osso_return_t ret
        
        data = NULL
        ret = OSSO_OK

        if user_data != None:
            if PyObject_HasAttrString(user_data, "__gtype__"):
                obj = user_data.obj
                data = <void *>obj

        ret = osso_cp_plugin_execute(self.ctx, filename, data, user_activated)

        if ret != OSSO_OK:
            _set_exception(ret, NULL) 

    def plugin_save_state(self, filename, user_data=None):
        cdef osso_return_t ret

        ret = osso_cp_plugin_save_state(self.ctx, filename, <void *>user_data)
        if ret != OSSO_OK:
            _set_exception(ret, NULL)





