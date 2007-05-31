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
#include <osso-ic-dbus.h>
#include <osso-ic-gconf.h>
#include <osso-ic-ui-dbus.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-lowlevel.h>

void
add_constants(PyObject *module)
{
	/* Dbus Constants */
	PyModule_AddStringConstant(module, "ICD_DBUS_SERVICE",
										ICD_DBUS_SERVICE);
	PyModule_AddStringConstant(module, "ICD_DBUS_INTERFACE",
										ICD_DBUS_INTERFACE);
	PyModule_AddStringConstant(module, "ICD_DBUS_PATH",
										ICD_DBUS_PATH);

	PyModule_AddStringConstant(module, "ICD_CONNECT_REQ",
										ICD_CONNECT_REQ);
	PyModule_AddStringConstant(module, "ICD_DISCONNECT_REQ",
										ICD_DISCONNECT_REQ);
	PyModule_AddStringConstant(module, "ICD_ACTIVATE_REQ",
										ICD_ACTIVATE_REQ);
	PyModule_AddStringConstant(module, "ICD_SHUTDOWN_REQ",
										ICD_SHUTDOWN_REQ);
	PyModule_AddStringConstant(module, "ICD_GET_STATE_REQ",
										ICD_GET_STATE_REQ);
	PyModule_AddStringConstant(module, "ICD_GET_STATISTICS_REQ",
										ICD_GET_STATISTICS_REQ);
	PyModule_AddStringConstant(module, "ICD_GET_IPINFO_REQ",
										ICD_GET_IPINFO_REQ);
	PyModule_AddStringConstant(module, "ICD_STATUS_CHANGED_SIG",
										ICD_STATUS_CHANGED_SIG);

	PyModule_AddStringConstant(module, "ICD_DBUS_AUTOCONF_INTERFACE",
										ICD_DBUS_AUTOCONF_INTERFACE);
	PyModule_AddStringConstant(module, "ICD_DBUS_AUTOCONF_PATH",
										ICD_DBUS_AUTOCONF_PATH);
	PyModule_AddStringConstant(module, "ICD_AUTOCONF_CHANGED_SIG",
										ICD_AUTOCONF_CHANGED_SIG);

	PyModule_AddStringConstant(module, "ICD_DBUS_ERROR_SYSTEM_ERROR",
										ICD_DBUS_ERROR_SYSTEM_ERROR);
	PyModule_AddStringConstant(module, "ICD_DBUS_ERROR_GATEWAY_ERROR",
										ICD_DBUS_ERROR_GATEWAY_ERROR);
	PyModule_AddStringConstant(module, "ICD_DBUS_ERROR_AUTH_FAILED",
										ICD_DBUS_ERROR_AUTH_FAILED);
	PyModule_AddStringConstant(module, "ICD_DBUS_ERROR_SERVER_ERROR",
										ICD_DBUS_ERROR_SERVER_ERROR);
	PyModule_AddStringConstant(module, "ICD_DBUS_ERROR_NETWORK_ERROR",
										ICD_DBUS_ERROR_NETWORK_ERROR);
	PyModule_AddStringConstant(module, "ICD_DBUS_ERROR_WPA2_NOT_SUPPORTED",
										ICD_DBUS_ERROR_WPA2_NOT_SUPPORTED);
	PyModule_AddStringConstant(module, "ICD_DBUS_ERROR_SAP_NOT_SUPPORTED",
										ICD_DBUS_ERROR_SAP_NOT_SUPPORTED);
	PyModule_AddStringConstant(module, "ICD_DBUS_ERROR_SAP_CONNECT_FAILED",
										ICD_DBUS_ERROR_SAP_CONNECT_FAILED);
	PyModule_AddStringConstant(module, "ICD_DBUS_ERROR_SAP_NO_PHONE",
										ICD_DBUS_ERROR_SAP_NO_PHONE);
	PyModule_AddStringConstant(module, "ICD_DBUS_ERROR_FLIGHT_MODE",
										ICD_DBUS_ERROR_FLIGHT_MODE);
	PyModule_AddStringConstant(module, "ICD_DBUS_ERROR_INVALID_IAP",
										ICD_DBUS_ERROR_INVALID_IAP);
	PyModule_AddStringConstant(module, "ICD_DBUS_ERROR_IAP_IN_USE",
										ICD_DBUS_ERROR_IAP_IN_USE);
	PyModule_AddStringConstant(module, "ICD_DBUS_ERROR_IAP_NOT_AVAILABLE",
										ICD_DBUS_ERROR_IAP_NOT_AVAILABLE);

	PyModule_AddStringConstant(module, "GWHWD_DBUS_SERVICE",
										GWHWD_DBUS_SERVICE);
	PyModule_AddStringConstant(module, "GWHWD_DBUS_INTERFACE",
										GWHWD_DBUS_INTERFACE);
	PyModule_AddStringConstant(module, "GWHWD_DBUS_PATH",
										GWHWD_DBUS_PATH);
	PyModule_AddStringConstant(module, "GWHWD_DETECT_REQ",
										GWHWD_DETECT_REQ);
	PyModule_AddStringConstant(module, "GWHWD_DBUS_ERROR",
										GWHWD_DBUS_ERROR);

	
	/* GConf constants */
	PyModule_AddStringConstant(module, "ICD_GCONF_PATH",
										ICD_GCONF_PATH);
	PyModule_AddStringConstant(module, "ICD_GCONF_CURRENT",
										ICD_GCONF_CURRENT);
	PyModule_AddStringConstant(module, "ICD_GCONF_AUTO_CONNECT",
										ICD_GCONF_AUTO_CONNECT);
	PyModule_AddStringConstant(module, "ICD_GCONF_DISCONNECT_ON_COVER",
										ICD_GCONF_DISCONNECT_ON_COVER);
	PyModule_AddStringConstant(module, "ICD_GCONF_FLIGHTMODE_WLAN",
										ICD_GCONF_FLIGHTMODE_WLAN);
	PyModule_AddStringConstant(module, "ICD_GCONF_WLAN_TX_POWER",
										ICD_GCONF_WLAN_TX_POWER);
	PyModule_AddStringConstant(module, "ICD_GCONF_TIMEOUT_WLAN",
										ICD_GCONF_TIMEOUT_WLAN);
	PyModule_AddStringConstant(module, "ICD_GCONF_TIMEOUT_DUN_CS",
										ICD_GCONF_TIMEOUT_DUN_CS);
	PyModule_AddStringConstant(module, "ICD_GCONF_TIMEOUT_DUN_PS",
										ICD_GCONF_TIMEOUT_DUN_PS);

	
	/* DBUS UI constants */
	PyModule_AddStringConstant(module, "ICD_UI_DBUS_SERVICE",
										ICD_UI_DBUS_SERVICE);
	PyModule_AddStringConstant(module, "ICD_UI_DBUS_INTERFACE",
										ICD_UI_DBUS_INTERFACE);
	PyModule_AddStringConstant(module, "ICD_UI_DBUS_PATH",
										ICD_UI_DBUS_PATH);
	PyModule_AddStringConstant(module, "ICD_UI_SHOW_CONNDLG_REQ",
										ICD_UI_SHOW_CONNDLG_REQ);
	PyModule_AddStringConstant(module, "ICD_UI_DBUS_ERROR_FLIGHT_MODE",
										ICD_UI_DBUS_ERROR_FLIGHT_MODE);
	PyModule_AddStringConstant(module, "ICD_UI_SHOW_DISCONNDLG_REQ",
										ICD_UI_SHOW_DISCONNDLG_REQ);
	PyModule_AddStringConstant(module, "ICD_UI_SHOW_RETRY_REQ",
										ICD_UI_SHOW_RETRY_REQ);
	PyModule_AddStringConstant(module, "ICD_UI_SHOW_CHANGE_REQ",
										ICD_UI_SHOW_CHANGE_REQ);
	PyModule_AddStringConstant(module, "ICD_UI_SHOW_PASSWD_REQ",
										ICD_UI_SHOW_PASSWD_REQ);
	PyModule_AddStringConstant(module, "ICD_UI_SHOW_GTC_REQ",
										ICD_UI_SHOW_GTC_REQ);
	PyModule_AddStringConstant(module, "ICD_UI_MSCHAP_CHANGE_REQ",
										ICD_UI_MSCHAP_CHANGE_REQ);
	PyModule_AddStringConstant(module, "ICD_UI_SHOW_PRIVATE_KEY_PASSWD_REQ",
										ICD_UI_SHOW_PRIVATE_KEY_PASSWD_REQ);
	PyModule_AddStringConstant(module, "ICD_UI_SHOW_SERVER_CERT_REQ",
										ICD_UI_SHOW_SERVER_CERT_REQ);
	PyModule_AddStringConstant(module, "ICD_UI_STRONG_BT_REQ",
										ICD_UI_STRONG_BT_REQ);
	PyModule_AddStringConstant(module, "ICD_UI_SHOW_SIM_PIN_REQ",
										ICD_UI_SHOW_SIM_PIN_REQ);
	PyModule_AddStringConstant(module, "ICD_UI_DISCONNECT_SIG",
										ICD_UI_DISCONNECT_SIG);
	PyModule_AddStringConstant(module, "ICD_UI_RETRY_SIG",
										ICD_UI_RETRY_SIG);
	PyModule_AddStringConstant(module, "ICD_UI_CHANGE_SIG",
										ICD_UI_CHANGE_SIG);
	PyModule_AddStringConstant(module, "ICD_UI_PASSWD_SIG",
										ICD_UI_PASSWD_SIG);
	PyModule_AddStringConstant(module, "ICD_UI_GTC_RESPONSE_SIG",
										ICD_UI_GTC_RESPONSE_SIG);
	PyModule_AddStringConstant(module, "ICD_UI_MSCHAP_CHANGE_SIG",
										ICD_UI_MSCHAP_CHANGE_SIG);
	PyModule_AddStringConstant(module, "ICD_UI_PRIVATE_KEY_PASSWD_SIG",
										ICD_UI_PRIVATE_KEY_PASSWD_SIG);
	PyModule_AddStringConstant(module, "ICD_UI_SERVER_CERT_SIG",
										ICD_UI_SERVER_CERT_SIG);
	PyModule_AddStringConstant(module, "ICD_UI_STRONG_BT_SIG",
										ICD_UI_STRONG_BT_SIG);
	PyModule_AddStringConstant(module, "ICD_UI_SIM_PIN_SIG",
										ICD_UI_SIM_PIN_SIG);

	return;
}

static struct PyMethodDef osso_ic_const_methods[] = {
	{0, 0, 0, 0}
};

PyMODINIT_FUNC
initconstants(void)
{
	PyObject *module;

	/* initialize module */
	module = Py_InitModule3("constants", osso_ic_const_methods, NULL);

	/* add constants */
	add_constants(module);
}

/* vim:ts=4:noet:sw=4:sws=4:si:ai:showmatch:foldmethod=indent
 */
