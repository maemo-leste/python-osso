from libosso cimport osso_context_t

cdef class Time:
    cdef osso_context_t *ctx
    cdef public object cb_data
