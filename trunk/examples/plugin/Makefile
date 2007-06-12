
CFLAGS=`pkg-config --cflags libosso glib-2.0`
LDFLAGS=`pkg-config --libs libosso glib-2.0`

all: libtestplugin.so

libtestplugin.so: testplugin.c
	gcc -shared testplugin.c -o libtestplugin.so $(CFLAGS) $(LDFLAGS)

testapp: exec_test.c
	gcc -o testapp exec_test.c $(CFLAGS) $(LDFLAGS)

clean:
	rm libtestplugin.so testapp
