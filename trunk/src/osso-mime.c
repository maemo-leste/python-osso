/**
 * osso-mime.c
 * Python bindings for libosso components.
 *
 * Copyright (C) 2005-2007 INdT - Instituto Nokia de Tecnologia
 *
 * Authors:
 * Osvaldo Santana Neto <osvaldo.santana@indt.org.br>
 * Daniel d'Andrada T. de Carvalho <daniel.carvalho@indt.org.br>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include "osso.h"

#include <osso-mime.h>
#include <dbus/dbus.h>

static PyObject *mime_callback = NULL;

/* ----------------------------------------------- */
/* Mime type default methods                       */
/* ----------------------------------------------- */

PyObject *
Mime_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	Context *self;

	self = (Context *)type->tp_alloc(type, 0);
	if (self != NULL) {
		self->context = NULL;
	}

	return (PyObject *)self;
}


int
Mime_init(Context *self, PyObject *args, PyObject *kwds)
{
	PyObject *ossocontext = NULL;
	Context *fullcontext = NULL;
	
	if (!PyArg_ParseTuple(args, "O", &ossocontext))
		return -1;

	fullcontext = (Context *)ossocontext;
	self->context = fullcontext->context;
	
	if (self->context == NULL) {
		PyErr_SetString(OssoException, "Context not initialize yet.");
		return -1;
	}

	return 0;
}


PyObject *
Mime_close(Context *self)
{
	if (!_check_context(self->context)) return 0;
	self->context = NULL;
	Py_RETURN_NONE;
}


void
Mime_dealloc(Context *self)
{
	if (self->context == NULL)
		return;

	self->context = NULL;
	return;
}


static struct PyMethodDef Mime_methods[] = {
	/* MIME */
	{"set_mime_callback", (PyCFunction)Context_set_mime_callback, METH_VARARGS | METH_KEYWORDS,
		"osso.Mime.set_mime_callback(callback[, user_data])\n\n"
		"This method registers a MIME callback function that Libosso calls\n"
		"when the user wants the application to open file(s) of a MIME type\n"
		"handled by the application. Use in callback parameter to unset this\n"
		"callback. The callback will receive a parameter with a list of URIs and\n"
		"user_data.\n"},
	{"open_file", (PyCFunction)Context_mime_open_file, METH_VARARGS | METH_KEYWORDS,
		"osso.Mime.open_file(uri)\n\n"
		"This method opens a file using the application that is\n"
		"registered as the handler for the file's mime type.\n"
		"The 'uri' parameter must be a string representation of the\n"
		"GnomeVFS URI of the file to be opened (UTF-8).\n"
		"\n"
		"The mapping from mime type to D-Bus service is done by looking up the\n"
		"application for the mime type and in the desktop file for that application\n"
		"the X-Osso-Service field is used to get the D-Bus service.\n"},
	{"open_file_list", (PyCFunction)Context_mime_open_file_list, METH_VARARGS | METH_KEYWORDS,
		"osso.Mime.open_file_list(list)\n\n"
		"This method opens a list of files in the application that has\n"
		"registered as the handler for the mime type of the file.\n"
		"\n"
		"It receives as its parameter a list of string representations of\n"
		"the GnomeVFS URI of each file to be opened (UTF-8).\n"
		"\n"
		"These will be sent to the application that handles those MIME-types.\n"
		"If more then one MIME-type is specified, many applications may be launched.\n"
		"\n"
		"The mapping from mime type to D-Bus service is done by looking up the\n"
		"application for the mime type and in the desktop file for that application\n"
		"the X-Osso-Service field is used to get the D-Bus service.\n"
		},
	{"open_file_with_mime_type", (PyCFunction)Context_mime_open_file_with_mime_type, METH_VARARGS | METH_KEYWORDS,
		"osso.Mime.open_file_with_mime_type(uri, mime_type)\n\n"
		"This method opens a file using the application that is\n"
		"registered as the handler for the given mime type.\n"
		"\n"
		"This operates similarly to osso.Mime.open_file() with the exception\n"
		"that a file does not need to be given, and the mime type supplied\n"
		"is used without the need for checking the mime-type of the file itself.\n"},
	{"get_category_for_mime_type", (PyCFunction)Context_mime_get_category_for_mime_type, METH_VARARGS | METH_KEYWORDS,
		"osso.Mime.get_category_for_mime_type(mime_type)\n\n"
		"Return the category the specified mime type is in or None. See\n"
		"osso.Mime.get_mime_types_for_category() for more information.\n"},
	{"get_mime_types_for_category", (PyCFunction)Context_mime_get_mime_types_for_category, METH_VARARGS | METH_KEYWORDS,
		"osso.Mime.get_mime_types_for_category(category)\n\n"
		"Returns a list of mime types that are in the specified category.\n"
		"\n"
		"The mapping between category and mime type is handled through the shared mime\n"
		"info. Add the tag <osso:category name=\"foo\"/> to a mime type to specify that\n"
		"the mime type is in the category \"foo\". Valid category names are:\n"
		"\n"
		"\"audio\", \"bookmarks\", \"contacts\", \"documents\", \"emails\", \"images\" and \"video\"\n"
		"\n"
		"An example:\n"
		"\n"
		"<mime-type type=\"text/plain\">\n"
		"    <osso:category name=\"documents\"/>\n"
		"</mime-type>\n"
		"\n"
		"It receives the category (string) as its parameter and returns either the\n"
		"corresponding list of mime-types (list of strings) or None, if none were\n"
		"found or it's not a valid category.\n"},
	{"get_application_mime_types", (PyCFunction)Context_mime_get_application_mime_types, METH_VARARGS | METH_KEYWORDS,
		"osso.Mime.get_application_mime_types(app_id)\n\n"
		"Returns a list of mime types supported by the application corresponding to\n"
		"the specified appliction id.\n"
		"\n"
		"The list of mime types is specifed in the desktop file for the application\n"
		"with the MimeType field.\n"
		"\n"
		"app_id is the application id, as returned by GnomeVFS.\n"
		"\n"
		"It returns a list of mime types, represented as strings, or None if none were\n"
		"found.\n"},
	{"get_icon_names", (PyCFunction)Context_mime_get_icon_names, METH_VARARGS | METH_KEYWORDS,
		"osso.Mime.get_icon_names(mime_type)\n\n"
		"Returns a list of icon names for the specified mime type. The icon names\n"
		"are GtkIconTheme names. A number of names are returned, ordered by how\n"
		"specific they are. For example, if the mime type \"image/png\" is passed,\n"
		"the first icon name might correspond to a png file icon, the second to\n"
		"an image file icon, and the third to a regular file icon.\n"
		"\n"
		"In order to decide which icon to use, the existance of it in the icon theme\n"
		"should be checked with gtk_icon_theme_has_icon(). If the first icon is not\n"
		"available, try the next etc.\n"},
	/* Default */
	{"close", (PyCFunction)Mime_close, METH_NOARGS, "Close Mime context."},
	{0, 0, 0, 0}
};


static PyTypeObject MimeType = {
	PyObject_HEAD_INIT(NULL)
	0,																/* ob_size */
	"osso.Mime",													/* tp_name */
	sizeof(Context),												/* tp_basicsize */
	0,																/* tp_itemsize */
	(destructor)Mime_dealloc,										/* tp_dealloc */
	0,																/* tp_print */
	0,																/* tp_getattr */
	0,																/* tp_setattr */
	0,																/* tp_compare */
	0,																/* tp_repr */
	0,																/* tp_as_number */
	0,																/* tp_as_sequence */
	0,																/* tp_as_mapping */
	0,																/* tp_hash */
	0,																/* tp_call */
	0,																/* tp_str */
	0,																/* tp_getattro */
	0,																/* tp_setattro */
	0,																/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_CHECKTYPES|Py_TPFLAGS_BASETYPE,	/* tp_flags */
	"OSSO Mime class",												/* tp_doc */
	0,																/* tp_traverse */
	0,																/* tp_clear */
	0,																/* tp_richcompare */
	0,																/* tp_weaklistoffset */
	0,																/* tp_iter */
	0,																/* tp_iternext */
	Mime_methods,													/* tp_methods */
	0,																/* tp_members */
	0,																/* tp_getset */
	0,																/* tp_base */
	0,																/* tp_dict */
	0,																/* tp_descr_get */
	0,																/* tp_descr_set */
	0,																/* tp_dictoffset */
	(initproc)Mime_init,											/* tp_init */
	0,																/* tp_alloc */
	Mime_new,														/* tp_new */
};

static struct PyMethodDef osso_methods[] = {
	{0, 0, 0, 0}
};

PyMODINIT_FUNC
initmime(void)
{
	PyObject *module;

	/* prepare types */
	if (PyType_Ready(&MimeType) < 0) {
		return;
	}

	/* initialize module */
	module = Py_InitModule3("mime", osso_methods,
			"FIXME: put documentation about RPC, Application, Autosave, Statusbar, etc.");

	/* add types */
	Py_INCREF(&MimeType);
	PyModule_AddObject(module, "Mime", (PyObject *)&MimeType);

	_load_exceptions();
	/* add contants */
	/* : */
	/* : */
	/* : */
}

	
static void
_wrap_mime_callback_handler (void *data, int argc, char **argv)
{
	int i;
	PyObject *py_args = NULL;
	PyObject *uris = NULL;
	PyGILState_STATE state;

	state = PyGILState_Ensure();

	if (mime_callback == NULL) {
		return;
	}

	uris = PyTuple_New(argc);
	if (uris == NULL) {
		return;
	}
	for (i = 0; i < argc; i++) {
		PyTuple_SetItem(uris, i, Py_BuildValue("s", argv[i]));
	}

	py_args = Py_BuildValue("(OO)", uris, data);
	PyEval_CallObject(mime_callback, py_args);

	PyGILState_Release(state);
	return;
}


PyObject *
Context_set_mime_callback (Context *self, PyObject *args, PyObject *kwds)
{
	PyObject *py_func = NULL;
	PyObject *py_data = NULL;
	osso_return_t ret;

	static char *kwlist[] = { "callback", "user_data", 0 };

	if (!_check_context(self->context)) return 0;

	if (!PyArg_ParseTupleAndKeywords(args, kwds,
				"O|O:Context.set_mime_callback", kwlist, &py_func,
				&py_data)) {
		return NULL;
	}

	if (py_func != Py_None) {
		if (!PyCallable_Check(py_func)) {
			PyErr_SetString(PyExc_TypeError, "callback parameter must be callable");
			return NULL;
		}
		Py_XINCREF(py_func);
		Py_XDECREF(mime_callback);
		mime_callback = py_func;
	} else {
		Py_XDECREF(mime_callback);
		mime_callback = NULL;
	}

	if (mime_callback != NULL) {
		ret = osso_mime_set_cb(self->context,
				_wrap_mime_callback_handler, py_data);
	} else {
		ret = osso_mime_unset_cb(self->context);
	}

	if (ret != OSSO_OK) {
		_set_exception(ret, NULL);
		return NULL;
	}

	Py_RETURN_NONE;
}


PyObject *
Context_mime_open_file (Context *self, PyObject *args, PyObject *kwds)
{
	const char *file_uri;
	gint result;
	DBusConnection *dbus_conn;
	DBusError error;

	static char *kwlist[] = { "uri", 0 };

	if (!_check_context(self->context)) return 0;

	if (!PyArg_ParseTupleAndKeywords(args, kwds,
				"s:Mime.open_file", kwlist, &file_uri)) {
		return NULL;
	}

	// Get a D-Bus connection
	dbus_error_init (&error);
	dbus_conn = dbus_bus_get (DBUS_BUS_SESSION, &error);
	if (dbus_conn == NULL || dbus_error_is_set (&error)) {
		PyErr_SetString (PyExc_RuntimeError, error.message);
		dbus_error_free (&error);
		return NULL;
	}

	// And finally call the C funtion we're wrapping.
	result = osso_mime_open_file (dbus_conn, file_uri);

	if (result != 1) {
		PyErr_SetString (PyExc_RuntimeError,
			"Failed when trying to open the specified URI.");
		return NULL;
	}

	Py_RETURN_NONE;
}


PyObject *
Context_mime_open_file_list (Context *self, PyObject *args, PyObject *kwds)
{
	PyObject *uri_list = NULL;
	gint result;
	DBusConnection *dbus_conn;
	DBusError error;
	Py_ssize_t list_size;
	Py_ssize_t i;
	GSList *uri_gslist = NULL;
	gboolean ok = TRUE;
	PyObject *string_obj;
	char *uri;

	static char *kwlist[] = { "list", 0 };

	if (!_check_context(self->context)) return 0;

	if (!PyArg_ParseTupleAndKeywords(args, kwds,
				"O!:Mime.open_file_list", kwlist, &PyList_Type, &uri_list)) {
		ok = FALSE;
		goto CLEAN_UP;
	}

	// Create a GLib list from the Python one.
	list_size = PyList_GET_SIZE (uri_list);
	for (i = 0; i < list_size; i++) {
		string_obj = PyList_GetItem (uri_list, i);
		uri = PyString_AsString (string_obj);
		uri_gslist = g_slist_append (uri_gslist, uri);
	}

	// Get a D-Bus connection
	dbus_error_init (&error);
	dbus_conn = dbus_bus_get (DBUS_BUS_SESSION, &error);
	if (dbus_conn == NULL || dbus_error_is_set (&error)) {
		PyErr_SetString (PyExc_RuntimeError, error.message);
		dbus_error_free (&error);
		ok = FALSE;
		goto CLEAN_UP;
	}

	// And finally call the C funtion we're wrapping.
	result = osso_mime_open_file_list (dbus_conn, uri_gslist);

	if (result != 1) {
		PyErr_SetString (PyExc_RuntimeError,
			"Failed when trying to open the list of URIs.");
		ok = FALSE;
		goto CLEAN_UP;
	}

	CLEAN_UP:

	if (uri_gslist != NULL)
		g_slist_free (uri_gslist);

	if (ok == TRUE)
		Py_RETURN_NONE;
	else
		return NULL;
}


PyObject *
Context_mime_open_file_with_mime_type (Context *self, PyObject *args, PyObject *kwds)
{
	const char *file_uri;
	const char *mime_type;
	gint result;
	DBusConnection *dbus_conn;
	DBusError error;

	static char *kwlist[] = { "uri", "mime_type", 0 };

	if (!_check_context(self->context)) return 0;

	if (!PyArg_ParseTupleAndKeywords(args, kwds,
				"ss:Mime.open_file_with_mime_type", kwlist, &file_uri, &mime_type)) {
		return NULL;
	}

	// Get a D-Bus connection
	dbus_error_init (&error);
	dbus_conn = dbus_bus_get (DBUS_BUS_SESSION, &error);
	if (dbus_conn == NULL || dbus_error_is_set (&error)) {
		PyErr_SetString (PyExc_RuntimeError, error.message);
		dbus_error_free (&error);
		return NULL;
	}

	// And finally call the C funtion we're wrapping.
	result = osso_mime_open_file_with_mime_type (dbus_conn, file_uri, mime_type);

	if (result != 1) {
		PyErr_SetString (PyExc_RuntimeError,
			"Failed when trying to open the specified URI.");
		return NULL;
	}

	Py_RETURN_NONE;
}


PyObject *
Context_mime_get_category_for_mime_type (Context *self, PyObject *args, PyObject *kwds)
{
	const char *mime_type;
	OssoMimeCategory category;
	const gchar *category_str;

	static char *kwlist[] = { "mime_type", 0 };

	if (!_check_context(self->context)) return 0;

	if (!PyArg_ParseTupleAndKeywords(args, kwds,
				"s:Mime.get_category_for_mime_type", kwlist, &mime_type)) {
		return NULL;
	}

	// Call the C funtion we're wrapping.
	category = osso_mime_get_category_for_mime_type (mime_type);

	category_str = osso_mime_get_category_name (category);

	if (category_str == NULL)
		Py_RETURN_NONE;
	else
		return PyString_FromString (category_str);
}


PyObject *
Context_mime_get_mime_types_for_category (Context *self, PyObject *args, PyObject *kwds)
{
	const char *category;
	GList *mime_list = NULL;
	static char *kwlist[] = { "category", 0 };
	OssoMimeCategory cat_enum;
	PyObject *ret_val = NULL;
	PyObject *py_string;
	GList *curr_item;
	const char *mime_type;
	Py_ssize_t i;

	if (!_check_context(self->context)) return 0;

	if (!PyArg_ParseTupleAndKeywords(args, kwds,
				"s:Mime.get_mime_types_for_category", kwlist, &category)) {
		return NULL;
	}

	cat_enum = osso_mime_get_category_from_name (category);

	// Call the C funtion we're wrapping.
	mime_list = osso_mime_get_mime_types_for_category (cat_enum);

	if (mime_list == NULL)
		Py_RETURN_NONE;

	// Convert from GList to Python list
	ret_val = PyList_New (g_list_length (mime_list));

	curr_item = mime_list;
	i = 0;
	while (curr_item != NULL) {
		mime_type = (char*) curr_item->data;

		py_string = PyString_FromString (mime_type);
		PyList_SET_ITEM (ret_val, i, py_string);

		curr_item = curr_item->next;
		i++;
	}

	// Clean up
	osso_mime_types_list_free (mime_list);

	return ret_val;
}


PyObject *
Context_mime_get_application_mime_types (Context *self, PyObject *args, PyObject *kwds)
{
	const char* app_id;
	GList *mime_list;
	PyObject *ret_val = NULL;
	PyObject *py_string;
	GList *curr_item;
	const char *mime_type;
	Py_ssize_t i;
	static char *kwlist[] = { "app_id", 0 };

	if (!_check_context(self->context)) return 0;

	if (!PyArg_ParseTupleAndKeywords(args, kwds,
				"s:Mime.get_application_mime_types", kwlist, &app_id)) {
		return NULL;
	}

	// Call the C funtion we're wrapping.
	mime_list = osso_mime_application_get_mime_types (app_id);

	if (mime_list == NULL)
		Py_RETURN_NONE;

	// Convert from GList to Python list
	ret_val = PyList_New (g_list_length (mime_list));

	curr_item = mime_list;
	i = 0;
	while (curr_item != NULL) {
		mime_type = (char*) curr_item->data;

		py_string = PyString_FromString (mime_type);
		PyList_SET_ITEM (ret_val, i, py_string);

		curr_item = curr_item->next;
		i++;
	}

	// Clean up
	osso_mime_application_mime_types_list_free (mime_list);

	return ret_val;
}


PyObject *
Context_mime_get_icon_names(Context *self, PyObject *args, PyObject *kwds)
{
	const char *mime_type;
	gchar **icon_names;
	int i;
	PyObject *ret_val = NULL;
	PyObject *py_string;
	static char *kwlist[] = { "mime_type", 0 };

	if (!_check_context(self->context)) return 0;

	if (!PyArg_ParseTupleAndKeywords(args, kwds,
				"s:Mime.get_icon_names", kwlist, &mime_type)) {
		return NULL;
	}

	// Call the C funtion we're wrapping.
	icon_names = osso_mime_get_icon_names (mime_type, NULL);
	if (icon_names == NULL)
		Py_RETURN_NONE;

	// Convert icon_names to a Python list
	ret_val = PyList_New (0);

	for (i = 0; icon_names[i] != NULL; i++) {
		py_string = PyString_FromString (icon_names[i]);
		PyList_Append (ret_val, py_string);
	}

	// Clean up
	g_strfreev (icon_names);

	return ret_val;
}

/* vim:ts=4:noet:sw=4:sws=4:si:ai:showmatch:foldmethod=indent
 */
