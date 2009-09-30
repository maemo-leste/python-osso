from libglib cimport GArray, const_gchar
from libdbus cimport DBusMessage, dbus_message_append_args
from libdbus cimport DBUS_TYPE_STRING, DBUS_TYPE_UINT32, DBUS_TYPE_INT32, DBUS_TYPE_BOOLEAN, DBUS_TYPE_DOUBLE, DBUS_TYPE_STRING, DBUS_TYPE_INVALID
from libosso cimport osso_return_t, osso_rpc_t, osso_rpc_run_system_with_argfill, osso_rpc_run_with_argfill, osso_rpc_set_cb_f, osso_rpc_unset_cb_f
from libosso cimport osso_rpc_async_run_with_argfill
from libosso cimport OSSO_OK, OSSO_ERROR, OSSO_INVALID, OSSO_RPC_ERROR
from context cimport Context
from exceptions import OssoException, OssoInvalidException, OssoRPCException
from traceback import print_exc

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

cdef _rpc_exception(osso_return_t err, osso_rpc_t *retval):
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

cdef int _rpc_callback_handler(const_gchar *interface, const_gchar *method, GArray *arguments,
                               void *data, osso_rpc_t *retval) with gil:
    rpc = <Rpc>data
    try:
        ret = rpc.cb_data[0](<char *>interface, <char *>method,
                            _rpc_args_c_to_py(arguments),
                            <object>rpc.cb_data[1])
    except:
        # print exception on receiver, and return DBUS message to sender
        print_exc()
        retval.type = DBUS_TYPE_STRING
        retval.value.s = "Exception occurred during callback execution."
        return OSSO_ERROR
    _python_to_rpc_t(ret, retval)
    return OSSO_OK

cdef void _wrap_rpc_async_handler(const_gchar *interface, const_gchar *method,
                                  osso_rpc_t *retval, void *data) with gil:
    rpc = <Rpc>data
    try:
        ret = rpc.cb_data[0](<char *>interface, <char *>method,
                             _rpc_t_to_python(retval),
                             <object>rpc.cb_data[1])
    except:
        print_exc()

cdef class Rpc:
    def __cinit__(self, Context context not None):
        self.ctx = context.ctx

    def rpc_run(self, char *service, char *object_path, char *interface,
                char *method, rpc_args=tuple(), bint wait_reply=False, bint use_system_bus=False):
        if not isinstance(rpc_args, tuple):
            raise TypeError, "RPC arguments must be in a tuple."
        cdef osso_rpc_t retval
        cdef osso_return_t ret
        if use_system_bus:
            ret = osso_rpc_run_system_with_argfill(self.ctx,
                                                   service,
                                                   object_path,
                                                   interface,
                                                   method,
                                                   &retval if wait_reply else NULL,
                                                   _argfill,
                                                   <void *>rpc_args)
        else:
            ret = osso_rpc_run_with_argfill(self.ctx,
                                            service,
                                            object_path,
                                            interface,
                                            method,
                                            &retval if wait_reply else NULL,
                                            _argfill,
                                            <void *>rpc_args)
        if ret != OSSO_OK:
            _rpc_exception(ret, &retval)

    def rpc_async_run(self, char *service, char *object_path, char *interface, char *method,
                      callback, user_data=None, rpc_args=tuple()):
        if not isinstance(rpc_args, tuple):
            raise TypeError, "RPC arguments must be in a tuple."
        cdef osso_return_t ret
        if not callable(callback):
            raise TypeError, "callback parameter must be callable"
        self.cb_data = (callback, user_data)
        ret = osso_rpc_async_run_with_argfill(self.ctx,
                                              service,
                                              object_path,
                                              interface,
                                              method,
                                              _wrap_rpc_async_handler,
                                              <void *>self,
                                              _argfill,
                                              <void *>rpc_args)
        if ret != OSSO_OK:
            _rpc_exception(ret, NULL)

    def set_rpc_callback(self, char *service, char *object_path, char *interface,
                         callback, user_data=None):
        cdef osso_return_t ret
        if callback:
            if not callable(callback):
                raise TypeError, "callback parameter must be callable"
            self.cb_data = (callback, user_data)
            ret = osso_rpc_set_cb_f(self.ctx,
                                    service,
                                    object_path,
                                    interface,
                                    _rpc_callback_handler,
                                    <void *>self)
        else:
            ret = osso_rpc_unset_cb_f(self.ctx,
                                      service,
                                      object_path,
                                      interface,
                                      _rpc_callback_handler,
                                      <void *>self)
            self.cb_data = None
        if ret != OSSO_OK:
            _rpc_exception(ret, NULL)
