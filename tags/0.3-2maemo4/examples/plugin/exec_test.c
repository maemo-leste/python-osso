#include <stdio.h>
#include <libosso.h>

int main(int argc, char *argv[])
{
    osso_context_t *osso;
    gint ret;

    osso = osso_initialize("unit_test_plugin", "0.0.1", FALSE, NULL);
    
    ret = osso_cp_plugin_execute(osso, "./libtestplugin.so",
                                 NULL, FALSE);

    osso_deinitialize(osso);

    if (ret == OSSO_OK)
        return 1;
    else
        return 0;
}
