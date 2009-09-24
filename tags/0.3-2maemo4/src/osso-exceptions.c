/**
 * osso.c
 * Python bindings for libosso components.
 *
 * Copyright (C) 2005-2006 INdT - Instituto Nokia de Tecnologia
 *
 * Contact: Osvaldo Santana Neto <osvaldo.santana@indt.org.br>
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

/* ----------------------------------------------- */
/* Context type default methods                    */
/* ----------------------------------------------- */

static struct PyMethodDef osso_methods[] = {
	{0, 0, 0, 0}
};

PyMODINIT_FUNC
initexceptions(void)
{
	PyObject *module;

	/* initialize module */
	module = Py_InitModule3("exceptions", osso_methods,
			"FIXME: put documentation about Exceptions module.");

	/* add exceptions */
	OssoException = PyErr_NewException("osso.OssoException", 0, 0);
	OssoRPCException = PyErr_NewException("osso.OssoRPCException", OssoException, 0);
	OssoInvalidException = PyErr_NewException("osso.OssoInvalidException", OssoException, 0);
	OssoNameException = PyErr_NewException("osso.OssoNameException", OssoException, 0);
	OssoNoStateException = PyErr_NewException("osso.OssoNoStateException", OssoException, 0);
	OssoStateSizeException = PyErr_NewException("osso.OssoStateSizeException", OssoException, 0);

	Py_INCREF(OssoException);
	Py_INCREF(OssoRPCException);
	Py_INCREF(OssoInvalidException);
	Py_INCREF(OssoNameException);
	Py_INCREF(OssoNoStateException);
	Py_INCREF(OssoStateSizeException);

	PyModule_AddObject(module, "OssoException", OssoException);
	PyModule_AddObject(module, "OssoRPCException", OssoRPCException);
	PyModule_AddObject(module, "OssoInvalidException", OssoInvalidException);
	PyModule_AddObject(module, "OssoNameException", OssoNameException);
	PyModule_AddObject(module, "OssoNoStateException", OssoNoStateException);
	PyModule_AddObject(module, "OssoStateSizeException", OssoStateSizeException);

	/* add contants */
	/* : */
	/* : */
	/* : */
}


/* vim:ts=4:noet:sw=4:sws=4:si:ai:showmatch:foldmethod=indent
 */
