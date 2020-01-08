from libosso cimport osso_context_t

cdef class StatusBar:
    cdef osso_context_t *ctx
