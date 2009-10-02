from libosso cimport osso_context_t, osso_application_autosave_cb_f
from python cimport PyObject

cdef class Autosave:
    cdef osso_context_t *ctx
    cdef public object cb_data

