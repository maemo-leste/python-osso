from libosso cimport osso_context_t

cdef class Context:
    cdef osso_context_t *ctx
