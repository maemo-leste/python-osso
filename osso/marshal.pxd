cdef extern from "marshal.h":
    object PyMarshal_WriteObjectToString(object, int)
    object PyMarshal_ReadObjectFromString(char *, Py_ssize_t)
