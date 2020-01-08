from libosso cimport osso_context_t

cdef class Application:
    cdef osso_context_t *ctx
