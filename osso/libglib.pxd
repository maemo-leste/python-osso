cdef extern from "glib.h":
    ctypedef struct GArray:
        char *data
        unsigned int len

cdef extern from "glib.h":
    ctypedef char const_gchar "const gchar"
