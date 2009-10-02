from libosso cimport osso_rpc_t, osso_return_t
from libosso cimport OSSO_OK, OSSO_ERROR, OSSO_INVALID, OSSO_RPC_ERROR
from libdbus cimport DBusMessage, dbus_message_append_args
from libdbus cimport DBUS_TYPE_STRING, DBUS_TYPE_UINT32, DBUS_TYPE_INT32, DBUS_TYPE_BOOLEAN, DBUS_TYPE_DOUBLE, DBUS_TYPE_STRING, DBUS_TYPE_INVALID
from libglib cimport GArray
from exceptions import OssoException, OssoInvalidException, OssoRPCException

cdef void _python_to_rpc_t(object py_arg, osso_rpc_t *rpc_arg):
    if isinstance(py_arg, str):
        rpc_arg.type = DBUS_TYPE_STRING
        rpc_arg.value.s = py_arg
    elif isinstance(py_arg, bool):
        rpc_arg.type = DBUS_TYPE_BOOLEAN
        rpc_arg.value.b = py_arg
    elif isinstance(py_arg, int):
        if py_arg < 0:
            rpc_arg.type = DBUS_TYPE_INT32
            rpc_arg.value.i = py_arg
        else:
            rpc_arg.type = DBUS_TYPE_UINT32
            rpc_arg.value.u = py_arg
    elif isinstance(py_arg, float):
        rpc_arg.type = DBUS_TYPE_DOUBLE
        rpc_arg.value.d = py_arg
    else:
        rpc_arg.type = DBUS_TYPE_INVALID
        rpc_arg.value.i = 0

cdef _rpc_t_to_python(osso_rpc_t *arg):
    if arg.type == DBUS_TYPE_BOOLEAN:
        py_arg = bool(arg.value.b)
    elif arg.type == DBUS_TYPE_DOUBLE:
        py_arg = arg.value.d
    elif arg.type == DBUS_TYPE_INT32:
        py_arg = arg.value.i
    elif arg.type == DBUS_TYPE_UINT32:
        py_arg = arg.value.u
    elif arg.type == DBUS_TYPE_STRING:
        if arg.value.s == NULL:
            py_arg = None
        else:
            py_arg = arg.value.s
    else:
        py_arg = None
    return py_arg

cdef void _argfill(DBusMessage *msg, void *raw_tuple):
    cdef object tuple = <object>raw_tuple
    cdef osso_rpc_t arg

    for py_arg in tuple:
        _python_to_rpc_t(py_arg, &arg)
        dbus_message_append_args(msg, arg.type, &arg.value, DBUS_TYPE_INVALID)

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

cdef _rpc_args_c_to_py(GArray *args):
    cdef int i
    ret = []
    cdef osso_rpc_t *data
    for i in range(0, args.len):
        data = <osso_rpc_t *>args.data
        ret.append(_rpc_t_to_python(&data[i]))
    return ret
