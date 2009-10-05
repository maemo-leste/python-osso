from libosso cimport osso_context_t

cdef class Plugin:
    cdef osso_context_t *ctx

