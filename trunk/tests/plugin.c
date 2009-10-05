#include <stdio.h>
#include <libosso.h>

/* This file is used bt test_osso_plugin.py
 * Just run "make plugin" to build it.
 */

struct plugin_data {
        gint action;
};

osso_return_t execute(osso_context_t *osso, gpointer data,
                      gboolean user_activated);

gchar *get_service_name(osso_context_t *osso, gpointer data);

osso_return_t execute(osso_context_t *osso, gpointer data,
                      gboolean user_activated)
{
    printf("Executing test plugin for python-osso plugin unit test\n");
    fflush(stdout);

    return OSSO_OK;
}

gchar *get_service_name(osso_context_t *osso, gpointer data)
{
    return "plugin";
}
