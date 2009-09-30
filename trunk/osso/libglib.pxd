cdef extern from "glib/garray.h":
    ctypedef struct GArray:
        char *data
        unsigned int len

cdef extern from "glib/gtypes.h":
    ctypedef char const_gchar "const gchar"
