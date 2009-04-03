
CFLAGS=`pkg-config --cflags libosso glib-2.0`
LDFLAGS=`pkg-config --libs libosso glib-2.0`

# This can be found through pkg-config osso-af-desktop --variable=hildondesktoplibdir
PLUGIN_DIR=/usr/lib/hildon-control-panel

all: libtestplugin.so testapp

libtestplugin.so: testplugin.c
	gcc -shared testplugin.c -o libtestplugin.so $(CFLAGS) $(LDFLAGS)

testapp: exec_test.c
	gcc -o testapp exec_test.c $(CFLAGS) $(LDFLAGS)

install:
	cp libtestplugin.so $(PLUGIN_DIR)

uninstall:
	rm $(PLUGIN_DIR)/libtestplugin.so

clean:
	rm libtestplugin.so testapp
