include "helper.pyx"
from libosso cimport osso_return_t, osso_state_t, OSSO_OK, OSSO_ERROR_NO_STATE, osso_state_write, osso_state_read
from context cimport Context
from exceptions import OssoNoStateException
from marshal cimport PyMarshal_WriteObjectToString, PyMarshal_ReadObjectFromString
from python_string cimport PyString_AsStringAndSize

cdef Py_MARSHAL_VERSION = 2 #defined in marshal.h

cdef class StateSaving:
    def __cinit__(self, Context context not None):
        self.ctx = context.ctx

    def state_write(self, state):
        cdef osso_state_t osso_state
        cdef char *marshal_state
        cdef osso_return_t ret

        py_marshal_state = PyMarshal_WriteObjectToString(state, Py_MARSHAL_VERSION)
        PyString_AsStringAndSize(py_marshal_state, &marshal_state, <Py_ssize_t *>&osso_state.state_size)
        osso_state.state_data = <void *>marshal_state
        ret = osso_state_write(self.ctx, &osso_state)
        if ret != OSSO_OK:
            _set_exception(ret, NULL)

    def state_read(self):
        cdef osso_state_t osso_state
        cdef osso_return_t ret

        osso_state.state_size = 0
        osso_state.state_data = NULL
        ret = osso_state_read(self.ctx, &osso_state)
        if ret == OSSO_ERROR_NO_STATE:
            raise OssoNoStateException, "No state found."
        elif ret != OSSO_OK:
            _set_exception(ret, NULL)

        py_marshal_state = PyMarshal_ReadObjectFromString(<char *>osso_state.state_data, osso_state.state_size)

        return py_marshal_state
