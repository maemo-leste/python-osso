/**
 * osso-locale.c
 * Python bindings for libosso components.
 *
 * Copyright (C) 2005-2007 INdT - Instituto Nokia de Tecnologia
 *
 * Contact: Luciano Miguel Wolf <luciano.wolf@indt.org.br>
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

static PyObject *set_locale_callback = NULL;

/* ----------------------------------------------- */
/* Context type default methods                    */
/* ----------------------------------------------- */

PyObject *
Locale_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	Context *self;

	self = (Context *)type->tp_alloc(type, 0);
	if (self != NULL) {
		self->context = NULL;
	}

	return (PyObject *)self;
}


int
Locale_init(Context *self, PyObject *args, PyObject *kwds)
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
Locale_close(Context *self)
{
	if (!_check_context(self->context)) return 0;
	self->context = NULL;
	Py_RETURN_NONE;
}


void
Locale_dealloc(Context *self)
{
	if (self->context == NULL)
		return;

	self->context = NULL;
	return;
}


static struct PyMethodDef Locale_methods[] = {
	/* Locale */
	{"set_locale_notification_callback", (PyCFunction)Context_set_locale_notification_callback, METH_VARARGS | METH_KEYWORDS,
		"locale.set_locale_notification_callback(callback[, user_data])\n\n"
		"This function registers a callback that is called whenever the locale is\n"
		"changed.\n"
		"Use None in callback parameter to unset this callback. The callback\n"
		"will receive the parameters: new_locale, user_data.\n"},
	{"set_locale", (PyCFunction)Context_set_locale, METH_VARARGS,
		"locale.set_locale(new_locale) \n\n"
		"This function issues a notification over the D-Bus system bus indicating\n"
		"a change of locale. To receive such a notification, install a callback\n"
		"function with set_locale_notification_callback.\n"},
	/* Default */
	{"close", (PyCFunction)Locale_close, METH_NOARGS, "Close Locale context."},
	{0, 0, 0, 0}
};

static PyTypeObject LocaleType = {
	PyObject_HEAD_INIT(NULL)
	0,																/* ob_size */
	"osso.Locale",													/* tp_name */
	sizeof(Context),												/* tp_basicsize */
	0,																/* tp_itemsize */
	(destructor)Locale_dealloc,										/* tp_dealloc */
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
	"OSSO Locale class",											/* tp_doc */
	0,																/* tp_traverse */
	0,																/* tp_clear */
	0,																/* tp_richcompare */
	0,																/* tp_weaklistoffset */
	0,																/* tp_iter */
	0,																/* tp_iternext */
	Locale_methods,													/* tp_methods */
	0,																/* tp_members */
	0,																/* tp_getset */
	0,																/* tp_base */
	0,																/* tp_dict */
	0,																/* tp_descr_get */
	0,																/* tp_descr_set */
	0,																/* tp_dictoffset */
	(initproc)Locale_init,											/* tp_init */
	0,																/* tp_alloc */
	Locale_new,														/* tp_new */
};

static struct PyMethodDef osso_methods[] = {
	{0, 0, 0, 0}
};

PyMODINIT_FUNC
initlocale(void)
{
	PyObject *module;

	/* prepare types */
	LocaleType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&LocaleType) < 0) {
		return;
	}

	/* initialize module */
	module = Py_InitModule3("locale", osso_methods,
			"FIXME: put documentation about Locale module.");

	/* add types */
	Py_INCREF(&LocaleType);
	PyModule_AddObject(module, "Locale", (PyObject *)&LocaleType);

	/* add contants */
	/* : */
	/* : */
	/* : */
}

static void
_wrap_locale_callback_handler(const char *new_locale, void *data)
{
	PyObject *py_args = NULL;
	PyObject *py_ret = NULL;
	PyGILState_STATE state;

	state = PyGILState_Ensure();

	if (set_locale_callback == NULL) {
		return;
	}

	py_args = Py_BuildValue("(sO)", new_locale, data);
	py_ret = PyEval_CallObject(set_locale_callback, py_args);

	if (py_ret == NULL) {
		PyGILState_Release(state);
		return;
	}

	PyGILState_Release(state);
}

PyObject *
Context_set_locale_notification_callback(Context *self, PyObject *args, PyObject *kwds)
{
	PyObject *py_func = NULL;
	PyObject *py_data = NULL;
	osso_return_t ret;

	static char *kwlist[] = { "py_func", "py_data", 0};
	if (!_check_context(self->context)) return 0;

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "O|O:Context.locale_set_notification_callback",
				kwlist, &py_func, &py_data)) {
		return PyInt_FromLong(OSSO_INVALID);
	}

	if (py_data == NULL)
		py_data = Py_None;
	if (py_func != Py_None) {
		if (!PyCallable_Check(py_func)) {
			PyErr_SetString(PyExc_TypeError, "callback parameter must be callable");
			return PyInt_FromLong(OSSO_INVALID);
		}
		Py_XINCREF(py_func);
		Py_XDECREF(set_locale_callback);
		set_locale_callback = py_func;
	} else {
		Py_XDECREF(set_locale_callback);
		set_locale_callback = NULL;
	}

	ret = osso_locale_change_set_notification_cb(self->context, _wrap_locale_callback_handler, py_data);

	if (ret != OSSO_OK) {
		_set_exception(ret, NULL);
		return PyInt_FromLong(ret);
	}

	return PyInt_FromLong(OSSO_OK);
}


PyObject *
Context_set_locale(Context *self, PyObject *args)
{
	char *new_locale = NULL;
	osso_return_t ret;

	if (!_check_context(self->context)) return 0;

	if (!PyArg_ParseTuple(args, "s:Context.locale_set", &new_locale)) {
		return ossoret_to_pyobj(OSSO_INVALID);
	}

	ret = osso_locale_set(self->context, new_locale);
	if (ret != OSSO_OK) {
		_set_exception(ret, NULL);
		return PyInt_FromLong(ret);
	}
	return ossoret_to_pyobj(OSSO_OK);
}

/* vim:ts=4:noet:sw=4:sws=4:si:ai:showmatch:foldmethod=indent
 */
