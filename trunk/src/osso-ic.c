/**
 * osso-ic.c
 * Python bindings for libosso components.
 *
 * Copyright (C) 2007 INdT - Instituto Nokia de Tecnologia
 *
 * Authors:
 * Lauro Moura <lauro.neto@indt.org.br>
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

#include <osso-ic.h>

static PyObject *ic_callback = NULL;

PyObject *
IapEvent_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	IapEvent *self;

	self = (IapEvent *)type->tp_alloc(type, 0);
	if (self != NULL) {
		self->event = NULL;
	}

	return (PyObject *)self;
}


int
IapEvent_init(IapEvent *self, PyObject *args, PyObject *kwds)
{
	PyObject *event = NULL;
	IapEvent *fullevent = NULL;
	
	if (!PyArg_ParseTuple(args, "O", &event))
		return -1;

	fullevent = (IapEvent *)event;
	self->event = fullevent->event;
	
	if (self->event == NULL) {
		PyErr_SetString(OssoException, "IAP Event not initialize yet.");
		return -1;
	}

	return 0;
}


void
IapEvent_dealloc(IapEvent *self)
{
	if (self->event == NULL)
		return;

	self->event = NULL;
	return;
}


static struct PyMethodDef IapEvent_methods[] = {
	/* Default */
	{0, 0, 0, 0}
};


static PyObject *
IapEvent_get_type(IapEvent *self, void *closure)
{
	int ret;

	if (self->event != NULL)
		ret = self->event->type;
	else
		ret = -1;

	return PyInt_FromLong(ret);
}

static PyObject *
IapEvent_get_iap_name(IapEvent *self, void *closure)
{
	char *name;

	if (self->event != NULL)
		name = (char *)self->event->iap_name;
	else {
		Py_INCREF(Py_None);
		return Py_None;
	}

	return PyString_FromString(name);
}

static PyObject *
IapEvent_get_extra(IapEvent *self, void *closure)
{
	int ret = 0;
	int type = 0;

	if (closure == NULL || self == NULL || self->event == NULL){
		Py_INCREF(Py_None);
		return Py_None;
	}

	type = self->event->type;

	if (!strncmp(closure, "code", 4)) {

		if (type == OSSO_IAP_ERROR)
			ret = self->event->u.error_code;
		else
			ret = -1;

	} else if (!strncmp(closure, "time", 4)) {

		if (type == OSSO_IAP_STATISTICS)
			ret = self->event->u.statistics.time_active;
		else
			ret = -1;

	} else if (!strncmp(closure, "signal", 5)) {

		if (type == OSSO_IAP_STATISTICS)
			ret = self->event->u.statistics.signal_strength;
		else
			ret = -1;

	} else if (!strncmp(closure, "rx_packets", 10)) {

		if (type == OSSO_IAP_STATISTICS)
			ret = self->event->u.statistics.rx_packets;
		else
			ret = -1;

	} else if (!strncmp(closure, "tx_packets", 10)) {

		if (type == OSSO_IAP_STATISTICS)
			ret = self->event->u.statistics.tx_packets;
		else
			ret = -1;

	} else if (!strncmp(closure, "rx_bytes", 10)) {

		if (type == OSSO_IAP_STATISTICS)
			ret = self->event->u.statistics.rx_bytes;
		else
			ret = -1;

	} else if (!strncmp(closure, "tx_types", 10)) {

		if (type == OSSO_IAP_STATISTICS)
			ret = self->event->u.statistics.tx_bytes;
		else
			ret = -1;

	} else {
		Py_INCREF(Py_None);
		return Py_None;
	}

	return PyInt_FromLong(ret);
}

static struct PyGetSetDef IapEvent_getset[] = {
	{"type",
		(getter)IapEvent_get_type, 0, 0},
	{"iap",
		(getter)IapEvent_get_iap_name, 0, 0},
	{"error_code",
		(getter)IapEvent_get_extra, 0, "code"},
	{"time_active",
		(getter)IapEvent_get_extra, 0, "time"},
	{"signal_strength",
		(getter)IapEvent_get_extra, 0, "signal"},
	{"rx_packets",
		(getter)IapEvent_get_extra, 0, "rx_packets"},
	{"tx_packets",
		(getter)IapEvent_get_extra, 0, "tx_packets"},
	{"rx_bytes",
		(getter)IapEvent_get_extra, 0, "rx_bytes"},
	{"tx_bytes",
		(getter)IapEvent_get_extra, 0, "tx_bytes"},

	{0, 0, 0, 0}
};

static PyTypeObject IapEventType = {
	PyObject_HEAD_INIT(NULL)
	0,																/* ob_size */
	"osso.Mime",													/* tp_name */
	sizeof(IapEvent),												/* tp_basicsize */
	0,																/* tp_itemsize */
	(destructor)IapEvent_dealloc,									/* tp_dealloc */
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
	IapEvent_methods,												/* tp_methods */
	0,																/* tp_members */
	IapEvent_getset,												/* tp_getset */
	0,																/* tp_base */
	0,																/* tp_dict */
	0,																/* tp_descr_get */
	0,																/* tp_descr_set */
	0,																/* tp_dictoffset */
	(initproc)IapEvent_init,										/* tp_init */
	0,																/* tp_alloc */
	IapEvent_new,													/* tp_new */
};

/**************************************************************/

static struct PyMethodDef osso_ic_methods[] = {
	{"set_callback",
		(PyCFunction)IapIc_set_callback,
		METH_VARARGS | METH_KEYWORDS,
		"Adds a callback for IAP events."},
	{"connect",
		(PyCFunction)IapIc_connect,
		METH_VARARGS | METH_KEYWORDS,
		"Requests a connection. Answer is sent using the callback function."},
	{"disconnect",
		(PyCFunction)IapIc_disconnect,
		METH_VARARGS | METH_KEYWORDS,
		"Requests to disconnect the device."},
	{"get_statistics",
		(PyCFunction)IapIc_get_statistics,
		METH_VARARGS | METH_KEYWORDS,
		"Requests statistics about current connection."},
	{"get_configured_iaps",
		(PyCFunction)IapIc_get_configured_iaps,
		METH_VARARGS | METH_KEYWORDS,
		"Gets a list of all configured IAP's."},
	{"socket",
		(PyCFunction)IapIc_socket,
		METH_VARARGS | METH_KEYWORDS,
		"Connects to a IAP, blocking until connection is established,"
		"returning a new socket."},
	{"socket_close",
		(PyCFunction)IapIc_disconnect,
		METH_VARARGS | METH_KEYWORDS,
		"Closes the socket and disconnects the IAP."},
	{0, 0, 0, 0}
};

void
add_constants(void)
{
	/* PyModule_AddStringConstanti(PyObject *module,
	 * 							   const char *name,
	 * 							   const char *value); */
	return;
}

PyMODINIT_FUNC
initic(void)
{
	PyObject *module;

	/* prepare types */
	if (PyType_Ready(&IapEventType) < 0) {
		return;
	}

	/* initialize module */
	module = Py_InitModule3("ic", osso_ic_methods, NULL);
			

	/* add types */
	Py_INCREF(&IapEventType);
	PyModule_AddObject(module, "IapEvent", (PyObject *)&IapEventType);

	/* add contants */
	/* : */
	/* : */
	/* : */
}

	
static void
_wrap_ic_callback_handler (struct iap_event_t *event, void *arg)
{
	PyObject *py_args = NULL;
	PyObject *uris = NULL;
	PyGILState_STATE state;

	state = PyGILState_Ensure();

	if (ic_callback == NULL) {
		return;
	}

	uris = PyTuple_New(1);
	py_args = Py_BuildValue("(OO)", uris, arg);
	PyEval_CallObject(ic_callback, py_args);

	PyGILState_Release(state);
	return;
}

/*****************************************************************/

PyObject *
IapIc_set_callback (PyObject *self, PyObject *args)
{
	PyObject *py_func = NULL;
	osso_return_t ret;

	if (!PyArg_ParseTuple(args, "O:osso.ic.set_callback", &py_func)) 
		return NULL;

	if (py_func != Py_None) {
		if (!PyCallable_Check(py_func)) {
			PyErr_SetString(PyExc_TypeError, "callback parameter must be callable");
			return NULL;
		}
		Py_XINCREF(py_func);
		Py_XDECREF(ic_callback);
		ic_callback = py_func;
	} else {
		Py_XDECREF(ic_callback);
		ic_callback = NULL;
	}

	if (ic_callback != NULL) {
		ret = osso_iap_cb(_wrap_ic_callback_handler);
	}
	
	Py_RETURN_NONE;
}

PyObject *
IapIc_connect(PyObject *self, PyObject *args, PyObject *kwargs)
{
	char *iap;
	dbus_uint32_t flags;
	PyObject *user_data;
	int ret;

	static char *kwlist[] = {"iap", "flags", "data", 0};

	if (!PyArg_ParseTupleAndKeywords(args, kwargs,
			"si!O:osso.ic.connect", kwlist, &iap, &flags, &user_data))
		return NULL;

	if (user_data == NULL)
		user_data = Py_None;

	ret = osso_iap_connect(iap, flags, user_data);

	return PyInt_FromLong(ret);
}

PyObject *
IapIc_disconnect(PyObject *self, PyObject *args, PyObject *kwargs)
{
	char *iap;
	PyObject *user_data;
	int ret;

	static char *kwlist[] = {"iap", "data", 0};

	if (!PyArg_ParseTupleAndKeywords(args, kwargs,
			"sO:osso.ic.disconnect", kwlist, &iap, &user_data))
		return NULL;

	ret = osso_iap_disconnect(iap, user_data);

	return PyInt_FromLong(ret);
}

PyObject *
IapIc_get_statistics(PyObject *self, PyObject *args, PyObject *kwargs)
{
	char *iap;
	PyObject *user_data;
	int ret;

	static char *kwlist[] = {"iap", "data", 0};

	if (!PyArg_ParseTupleAndKeywords(args, kwargs,
			"sO:osso.ic.get_statistics", kwlist, &iap, &user_data))
		return NULL;

	ret = osso_iap_get_statistics(iap, user_data);

	return PyInt_FromLong(ret);
}

/* WARNING: Uses gconf (call g_types_init)... */
PyObject *
IapIc_get_configured_iaps(PyObject *self)
{
	GSList *iaps_c;
	GSList *node;
	PyObject *list = NULL;
	int len;
	int i = 0;

	iaps_c = osso_iap_get_configured_iaps();

	if (iaps_c == NULL){
		Py_INCREF(Py_None);
		return Py_None;
	}

	len = g_slist_length(iaps_c);

	list = PyList_New(len);

	node = iaps_c;
	for (i = 0; i < len; i++) {
		char *data;
		PyObject *py_data;

		data = node->data;
		py_data = PyString_FromString(data);
		
		PyList_SetItem(list, i, py_data);

		free(data);
	}

	g_slist_free(iaps_c);

	return list;
}

/***** Socket Interface ************/
PyObject *
IapIc_socket(PyObject *self, PyObject *args, PyObject *kwargs)
{
	int domain;
	int type;
	int protocol;
	int fd;

	static char *kwlist[] = {"domain", "type", "protocol", 0};

	if ( !PyArg_ParseTupleAndKeywords(args, kwargs,
			"iii:osso.ic.socket", kwlist, &domain, &type, &protocol))
		return NULL;

	fd = osso_socket(domain, type, protocol);

	return PyInt_FromLong(fd);
}

PyObject *
IapIc_close(PyObject *self, PyObject *args, PyObject *kwargs)
{
	int fd;
	int ret;

	static char *kwlist[] = {"fd", 0};

	if ( !PyArg_ParseTupleAndKeywords(args, kwargs,
			"i:osso.ic.socket_close", kwlist, &fd))
		return NULL;

	ret = osso_close(fd);

	return PyInt_FromLong(ret);
}

/* vim:ts=4:noet:sw=4:sws=4:si:ai:showmatch:foldmethod=indent
 */
