include "helper.pyx"
from libglib cimport GArray, const_gchar
from libdbus cimport DBusMessage, dbus_message_append_args
from libdbus cimport DBUS_TYPE_STRING, DBUS_TYPE_UINT32, DBUS_TYPE_INT32, DBUS_TYPE_BOOLEAN, DBUS_TYPE_DOUBLE, DBUS_TYPE_STRING, DBUS_TYPE_INVALID
from libosso cimport osso_return_t, osso_rpc_t, osso_rpc_run_system_with_argfill, osso_rpc_run_with_argfill, osso_rpc_set_cb_f, osso_rpc_unset_cb_f
from libosso cimport osso_rpc_async_run_with_argfill, osso_rpc_set_timeout, osso_rpc_get_timeout
from libosso cimport OSSO_OK, OSSO_ERROR, OSSO_INVALID, OSSO_RPC_ERROR
from context cimport Context
from exceptions import OssoException, OssoInvalidException, OssoRPCException
from traceback import print_exc

cdef int _rpc_callback_handler(const_gchar *interface, const_gchar *method, GArray *arguments,
                               void *data, osso_rpc_t *retval) with gil:
    rpc = <Rpc>data
    try:
        ret = rpc.cb_data[0](<char *>interface, <char *>method,
                            _rpc_args_c_to_py(arguments),
                            rpc.cb_data[1])
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
    cb_data = <object>data
    try:
        ret = cb_data[0](<char *>interface, <char *>method,
                         _rpc_t_to_python(retval), cb_data[1])
    except:
        print_exc()

# callback data for async RPC calls need to be global, otherwise it may get
# garbage collected
# FIXME: this is thread unsafe
_async_cb_data = None

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
            _set_exception(ret, &retval)

    def rpc_async_run(self, char *service, char *object_path, char *interface, char *method,
                      callback, user_data=None, rpc_args=tuple()):
        if not isinstance(rpc_args, tuple):
            raise TypeError, "RPC arguments must be in a tuple."
        cdef osso_return_t ret
        if not callable(callback):
            raise TypeError, "callback parameter must be callable"
        global _async_cb_data
        _async_cb_data = (callback, user_data)
        ret = osso_rpc_async_run_with_argfill(self.ctx,
                                              service,
                                              object_path,
                                              interface,
                                              method,
                                              _wrap_rpc_async_handler,
                                              <void *>_async_cb_data,
                                              _argfill,
                                              <void *>rpc_args)
        if ret != OSSO_OK:
            _set_exception(ret, NULL)

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
            _set_exception(ret, NULL)

    def set_rpc_timeout(self, int timeout):
        cdef osso_return_t ret
        ret = osso_rpc_set_timeout(self.ctx, timeout)
        if ret != OSSO_OK:
            _set_exception(ret, NULL)

    def get_rpc_timeout(self):
        cdef osso_return_t ret
        cdef int timeout
        ret = osso_rpc_get_timeout(self.ctx, &timeout)
        if ret != OSSO_OK:
            _set_exception(ret, NULL)
        else:
            return timeout
