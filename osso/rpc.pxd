from libosso cimport osso_context_t

cdef class Rpc:
    cdef osso_context_t *ctx
    cdef public object cb_data
