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


static struct PyMethodDef IapEvents_methods[] = {
	/* Default */
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
	IapEvents_methods,												/* tp_methods */
	0,																/* tp_members */
	0,																/* tp_getset */
	0,																/* tp_base */
	0,																/* tp_dict */
	0,																/* tp_descr_get */
	0,																/* tp_descr_set */
	0,																/* tp_dictoffset */
	(initproc)IapEvent_init,										/* tp_init */
	0,																/* tp_alloc */
	IapEvent_new,													/* tp_new */
};

static struct PyMethodDef osso_methods[] = {
	{"set_callback",
		(PyCFunction)IapIc_set_callback,
		METH_VARARGS,
		"Adds a callback for IAP events."},
	{"connect",
		(PyCFunction)IapIc_connect,
		METH_VARARGS,
		"Requests a connection. Answer is sent using the callback function."},
	{0, 0, 0, 0}
};

PyMODINIT_FUNC
initic(void)
{
	PyObject *module;

	/* prepare types */
	if (PyType_Ready(&IapEventType) < 0) {
		return;
	}

	/* initialize module */
	module = Py_InitModule3("ic", osso_methods,
			"FIXME: put documentation about RPC, Application, Autosave, Statusbar, etc.");

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
			"siO:osso.ic.connect", kwlist, &iap, &flags, &user_data))
		return NULL;

	ret = osso_iap_connect(iap, flags, user_data);

	return PyInt_FromLong(ret);
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
			"iii", kwlist, &domain, &type, &protocol))
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
			"i", kwlist, &fd))
		return NULL;

	ret = osso_close(fd);

	return PyInt_FromLong(ret);
}

/* vim:ts=4:noet:sw=4:sws=4:si:ai:showmatch:foldmethod=indent
 */
