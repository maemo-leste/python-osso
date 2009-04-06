#include <stdio.h>
#include <libosso.h>

struct plugin_data {
        gint action;
};

osso_return_t execute(osso_context_t *osso, gpointer data,
                      gboolean user_activated);

gchar *get_service_name(osso_context_t *osso, gpointer data);

osso_return_t execute(osso_context_t *osso, gpointer data,
                      gboolean user_activated)
{
    printf("This is execute()\n");
    fflush(stdout);

    return OSSO_OK;
}

gchar *get_service_name(osso_context_t *osso, gpointer data)
{
    return "unit_test_plugin";
}
