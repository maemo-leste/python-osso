from libosso cimport osso_context_t

cdef class StateSaving:
    cdef osso_context_t *ctx
