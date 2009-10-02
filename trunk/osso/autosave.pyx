from libosso cimport osso_return_t, OSSO_OK, osso_application_set_autosave_cb, osso_application_userdata_changed, osso_application_autosave_force, osso_application_name_get, osso_application_version_get, osso_application_unset_autosave_cb
from context cimport Context
from exceptions import OssoException
from collections import Callable
from traceback import print_exc

cdef void _wrap_autosave_callback_wrapper(void *data) with gil:
    autosave = <Autosave>data
    try:
        autosave.cb_data[0](<object>autosave.cb_data[1])
    except:
        print_exc() 

cdef class Autosave:
    def __cinit__(self, Context context not None):
        self.ctx = context.ctx
    
    def set_autosave_callback(self, cb, data=None):
        cdef osso_return_t ret
        
        if cb != None:
            self.cb_data = (cb, data)
            ret = osso_application_set_autosave_cb(self.ctx, _wrap_autosave_callback_wrapper, <void *> self)
        else:
            self.cb_data = None
            ret = osso_application_unset_autosave_cb(self.ctx, _wrap_autosave_callback_wrapper, <void *> self)
        
        
        if ret != OSSO_OK:
            raise OssoException, "OSSO error: osso_application_set_autosave_cb"

    def userdata_changed(self):
        cdef osso_return_t ret
        ret = osso_application_userdata_changed(self.ctx)
    
    def force_autosave(self):
        cdef osso_return_t ret
        ret = osso_application_autosave_force(self.ctx)

    def get_name(self):
        return osso_application_name_get(self.ctx)

    def get_version(self):
        return osso_application_version_get(self.ctx)



