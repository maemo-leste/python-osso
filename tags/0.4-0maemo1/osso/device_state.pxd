from libosso cimport osso_context_t

cdef class DeviceState:
    cdef osso_context_t *ctx
    cdef public object cb_data
    cdef public object display_cb_data
