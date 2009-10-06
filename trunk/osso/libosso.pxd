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

    ctypedef struct osso_state_t:
        unsigned int state_size
        void *state_data

    ctypedef void osso_application_autosave_cb_f(void *data)
    
    ctypedef void osso_rpc_argfill(DBusMessage *msg, void *data)

    ctypedef enum osso_return_t:
        OSSO_OK
        OSSO_ERROR
        OSSO_INVALID
        OSSO_RPC_ERROR
        OSSO_ERROR_NO_STATE


    ctypedef enum osso_system_note_type_t:
        OSSO_GN_WARNING
        OSSO_GN_ERROR
        OSSO_GN_NOTICE
        OSSO_GN_WAIT

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

    osso_return_t osso_rpc_get_timeout(osso_context_t *osso, int *timeout)
    osso_return_t osso_rpc_set_timeout(osso_context_t *osso, int timeout)

    osso_return_t osso_system_note_dialog(osso_context_t *osso,
                                          char *message,
                                          osso_system_note_type_t type,
                                          osso_rpc_t *retval)
    osso_return_t osso_system_note_infoprint(osso_context_t *osso,
                                             char *text,
                                             osso_rpc_t *retval)

    osso_return_t osso_application_set_autosave_cb(osso_context_t *osso,
                                                   osso_application_autosave_cb_f *cb,
                                                   void *data)

    osso_return_t osso_application_unset_autosave_cb(osso_context_t *osso,
                                                     osso_application_autosave_cb_f *cb,
                                                     void *data)
    osso_return_t osso_application_userdata_changed(osso_context_t *osso)
    osso_return_t osso_application_autosave_force(osso_context_t *osso)
    char *osso_application_name_get(osso_context_t *osso)
    char *osso_application_version_get(osso_context_t *osso)

    osso_return_t osso_state_write(osso_context_t *osso, osso_state_t *state)
    osso_return_t osso_state_read(osso_context_t *osso, osso_state_t *state)

    osso_return_t osso_statusbar_send_event(osso_context_t *osso,
                                            char *name,
                                            int argument1,
                                            int argument2,
                                            char *argument3,
                                            osso_rpc_t *retval)

    osso_return_t osso_time_set(osso_context_t *osso,
                                int new_time)

    ctypedef void osso_time_cb_f(void *data)
    osso_return_t osso_time_set_notification_cb(osso_context_t *osso,
                                                osso_time_cb_f *cb,
                                                void *data)

    osso_return_t osso_cp_plugin_execute (osso_context_t *osso, char *filename, void *data, int user_activated)
    osso_return_t osso_cp_plugin_save_state (osso_context_t *osso, char *filename, void* data)

    #Locale
    cdef extern from *:
        ctypedef char* const_char_ptr "const char*"

    ctypedef void osso_locale_change_cb_f(const_char_ptr new_locale, void *data)
    osso_return_t osso_locale_change_set_notification_cb (osso_context_t *osso,
                                                          osso_locale_change_cb_f *cb,
                                                          void *data)
    osso_return_t osso_locale_set (osso_context_t *osso, char *new_locale)
