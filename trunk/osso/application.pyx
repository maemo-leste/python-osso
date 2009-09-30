from libosso cimport osso_return_t, OSSO_OK, osso_application_top
from context cimport Context
from exceptions import OssoException

cdef class Application:
    def __cinit__(self, Context context not None):
        self.ctx = context.ctx

    def application_top(self, char *application, char *arguments=NULL):
        cdef osso_return_t ret
        ret = osso_application_top(self.ctx, application, arguments)
        if ret != OSSO_OK:
            raise OssoException, "OSSO Error."
