from libosso cimport osso_context_t

cdef class SystemNote:
    cdef osso_context_t *ctx
