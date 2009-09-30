cdef extern from "dbus/dbus.h":
    ctypedef unsigned int dbus_bool_t
    ctypedef struct DBusMessage

    enum:
        DBUS_TYPE_STRING
        DBUS_TYPE_UINT32
        DBUS_TYPE_INT32
        DBUS_TYPE_BOOLEAN
        DBUS_TYPE_DOUBLE
        DBUS_TYPE_STRING
        DBUS_TYPE_INVALID

    dbus_bool_t dbus_message_append_args(DBusMessage *message,
                                         int first_arg_type, ...)
