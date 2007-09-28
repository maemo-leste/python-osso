/**
 * osso-helplib.c
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

static PyTypeObject *_PyGtkDialog_Type;
#define PyGtkDialog_Type (*_PyGtkDialog_Type)

/* ----------------------------------------------- */
/* Help type default methods                       */
/* ----------------------------------------------- */

PyObject *
Help_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	Context *self;

	self = (Context *)type->tp_alloc(type, 0);
	if (self != NULL) {
		self->context = NULL;
	}

	return (PyObject *)self;
}


int
Help_init(Context *self, PyObject *args, PyObject *kwds)
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
Help_close(Context *self)
{
	if (!_check_context(self->context)) return 0;
	self->context = NULL;
	Py_RETURN_NONE;
}


void
Help_dealloc(Context *self)
{
	if (self->context == NULL)
		return;

	self->context = NULL;
	return;
}


static struct PyMethodDef Help_methods[] = {
	/* Help */
	{"show", (PyCFunction)Context_help_show, METH_VARARGS | METH_KEYWORDS,
		"Fill in later!\n"},
	{"dialog_help_enable", (PyCFunction)Context_help_dialog_help_enable, METH_VARARGS | METH_KEYWORDS,
		"Fill in later!\n"},
	/* Default */
	{"close", (PyCFunction)Help_close, METH_NOARGS, "Close help context."},
	{0, 0, 0, 0}
};

static PyTypeObject HelpType = {
	PyObject_HEAD_INIT(NULL)
	0,																/* ob_size */
	"osso.Help",													/* tp_name */
	sizeof(Context),												/* tp_basicsize */
	0,																/* tp_itemsize */
	(destructor)Help_dealloc,										/* tp_dealloc */
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
	"OSSO Help class",												/* tp_doc */
	0,																/* tp_traverse */
	0,																/* tp_clear */
	0,																/* tp_richcompare */
	0,																/* tp_weaklistoffset */
	0,																/* tp_iter */
	0,																/* tp_iternext */
	Help_methods,													/* tp_methods */
	0,																/* tp_members */
	0,																/* tp_getset */
	0,																/* tp_base */
	0,																/* tp_dict */
	0,																/* tp_descr_get */
	0,																/* tp_descr_set */
	0,																/* tp_dictoffset */
	(initproc)Help_init,											/* tp_init */
	0,																/* tp_alloc */
	Help_new,														/* tp_new */
};

static struct PyMethodDef osso_methods[] = {
	{0, 0, 0, 0}
};

PyMODINIT_FUNC
inithelp(void)
{
	PyObject *module;

	/* prepare types */
	HelpType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&HelpType) < 0) {
		return;
	}

	/* initialize module */
	module = Py_InitModule3("help", osso_methods,
			"FIXME: put documentation about Help module.");

	/* add types */
	Py_INCREF(&HelpType);
	PyModule_AddObject(module, "Help", (PyObject *)&HelpType);

	/* add contants */
	/* : */
	/* : */
	/* : */
}

PyObject *
Context_help_show(Context *self, PyObject *args, PyObject *kwds){
	char *topic_id;
	int flags;
	osso_return_t ret;

	static char *kwlist[] = { "topic_id", "flags", 0 };

	if (!_check_context(self->context)) return 0;

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "sI:Context.help_show",
				kwlist, &topic_id, &flags))
	{
		return NULL;
	}

	ret = ossohelp_show(self->context, topic_id, flags);
	if (ret != OSSO_OK) {
		_set_exception(ret, NULL);
		return NULL;
	}

	/* redo this!*/
	Py_RETURN_NONE;
}

PyObject *
Context_help_dialog_help_enable(Context *self, PyObject *args, PyObject *kwds){
	PyGObject *dialog;
	PyObject *module;
	char *topic_id = NULL;
	osso_return_t ret;

	static char *kwlist[] = { "dialog", "topic_id", 0 };

	if (!_check_context(self->context)) return 0;

    if ((module = PyImport_ImportModule("gtk")) != NULL) {
		_PyGtkDialog_Type = (PyTypeObject *)PyObject_GetAttrString(module, "Dialog");
		if (_PyGtkDialog_Type == NULL) {
			PyErr_SetString(PyExc_ImportError, "cannot import name Dialog from gtk");
			return NULL;
		}
    } else {
        PyErr_SetString(PyExc_ImportError, "could not import gtk");
        return NULL;
    }
					
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!|s:Context.help_dialog_help_enable",
				kwlist, &PyGtkDialog_Type, &dialog, &topic_id))
	{
		return NULL;
	}

	ret = ossohelp_dialog_help_enable(GTK_DIALOG(dialog->obj), topic_id, self->context);
	if (ret==FALSE) {
		_set_exception(ret, NULL);
		return NULL;
	}
	/* redo this! */
	Py_RETURN_NONE;
}

/* vim:ts=4:noet:sw=4:sws=4:si:ai:showmatch:foldmethod=indent
 */
