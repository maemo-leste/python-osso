from libosso cimport osso_initialize, osso_deinitialize
from exceptions import OssoException

cdef class Context:
    def __cinit__(self, char *application, char *version, object activation=None):
        self.ctx = osso_initialize(application, version, 0, NULL);
        if self.ctx == NULL:
            raise OssoException, "Cannot initialize context."

    def __dealloc__(self):
        if self.ctx == NULL:
            return
        osso_deinitialize(self.ctx)
