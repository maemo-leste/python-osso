from libdbus cimport DBusMessage
from libglib cimport GArray

cdef extern from "libosso.h":
    ctypedef struct osso_context_t

    cdef union value:
        unsigned int u
        int i
        int b
        double d
        char *s

    ctypedef struct osso_rpc_t:
        int type
        value value

    ctypedef void osso_rpc_argfill (DBusMessage *msg, void *data)

    ctypedef enum osso_return_t:
        OSSO_OK
        OSSO_ERROR
        OSSO_INVALID
        OSSO_RPC_ERROR

    osso_return_t osso_application_top(osso_context_t *osso,
                                       char *application,
                                       char *arguments)
    osso_context_t *osso_initialize(char *application,
                                    char *version,
                                    int activation,
                                    void *context)
    void osso_deinitialize(osso_context_t *osso)

    osso_return_t osso_rpc_run_system_with_argfill(osso_context_t *osso,
                                                   char *service,
                                                   char *object_path,
                                                   char *interface,
                                                   char *method,
                                                   osso_rpc_t *retval,
                                                   osso_rpc_argfill *argfill,
                                                   void *argfill_data)
    osso_return_t osso_rpc_run_with_argfill(osso_context_t *osso,
                                            char *service,
                                            char *object_path,
                                            char *interface,
                                            char *method,
                                            osso_rpc_t *retval,
                                            osso_rpc_argfill *argfill,
                                            void *argfill_data)

    ctypedef int osso_rpc_cb_f(char *interface, char *method,
                               GArray *arguments, void *data,
                               osso_rpc_t *retval)
    osso_return_t osso_rpc_set_cb_f(osso_context_t *osso,
                                    char *service,
                                    char *object_path,
                                    char *interface,
                                    osso_rpc_cb_f *cb,
                                    void *data)
    osso_return_t osso_rpc_unset_cb_f(osso_context_t *osso,
                                      char *service,
                                      char *object_path,
                                      char *interface,
                                      osso_rpc_cb_f *cb,
                                      void *data)

    ctypedef void osso_rpc_async_f(char *interface, char *method,
                                   osso_rpc_t *retval, void *data) 
    osso_return_t osso_rpc_async_run_with_argfill(osso_context_t *osso,
                                                  char *service,
                                                  char *object_path,
                                                  char *interface,
                                                  char *method,
                                                  osso_rpc_async_f *async_cb,
                                                  void *data,
                                                  osso_rpc_argfill *argfill,
                                                  void *argfill_data)   
