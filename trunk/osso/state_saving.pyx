include "helper.pyx"
from libosso cimport osso_return_t, osso_state_t, osso_rpc_t, OSSO_OK, OSSO_ERROR_NO_STATE, OSSO_ERROR, OSSO_INVALID, OSSO_RPC_ERROR, osso_state_write, osso_state_read
from libdbus cimport DBUS_TYPE_STRING
from context cimport Context
from exceptions import OssoException, OssoNoStateException, OssoInvalidException, OssoRPCException
from marshal cimport PyMarshal_WriteObjectToString, PyMarshal_ReadObjectFromString
from python_string cimport PyString_AsStringAndSize

cdef Py_MARSHAL_VERSION = 2 #defined in marshal.h


#FIXME: yanked from rpc.pyx temporarily, will be moved later to the proper place
cdef _set_exception(osso_return_t err, osso_rpc_t *retval):
    cdef char *err_msg = NULL
    if retval != NULL and retval.type == DBUS_TYPE_STRING and retval.value.s:
        err_msg = retval.value.s

    if err == OSSO_ERROR:
        raise OssoException, err_msg if err_msg else "OSSO error."
    elif err == OSSO_INVALID:
        raise OssoInvalidException, err_msg if err_msg else "Invalid parameter."
    elif err == OSSO_RPC_ERROR:
        raise OssoRPCException, err_msg if err_msg else "Error in RPC method call."
    else:
        raise OssoException, "Unknown OSSO error (%d)" % err


cdef class StateSaving:
    def __cinit__(self, Context context not None):
        self.ctx = context.ctx

    def state_write(self, state):
        cdef osso_state_t osso_state
        cdef char *marshal_state
        cdef osso_return_t ret = OSSO_OK

        py_marshal_state = PyMarshal_WriteObjectToString(state, Py_MARSHAL_VERSION)
        PyString_AsStringAndSize(py_marshal_state, &marshal_state, <Py_ssize_t *>&osso_state.state_size);
        osso_state.state_data = <void *> marshal_state;
        ret = osso_state_write(self.ctx, &osso_state)
        if ret != OSSO_OK:
            _set_exception(ret, NULL)

    def state_read(self):
        cdef osso_state_t osso_state
        cdef osso_return_t ret = OSSO_OK
        py_marshal_state = None
        osso_state.state_size = 0
        osso_state.state_data = NULL
        ret = osso_state_read(self.ctx, &osso_state)
        if ret == OSSO_ERROR_NO_STATE:
            raise OssoNoStateException, "No state found."
            return None
        elif ret != OSSO_OK:
            _set_exception(ret, NULL)
            return None
        py_marshal_state = PyMarshal_ReadObjectFromString(<char *>osso_state.state_data, osso_state.state_size)
        if py_marshal_state == None:
            return None

        return py_marshal_state




