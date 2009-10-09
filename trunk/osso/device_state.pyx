include "helper.pyx"

from libosso cimport osso_return_t, osso_context_t, osso_hw_state_t, osso_hw_cb_f, osso_display_event_cb_f, OSSO_OK, osso_display_state_on, osso_display_blanking_pause, osso_hw_set_event_cb, osso_hw_unset_event_cb, osso_hw_set_display_event_cb, osso_display_state_t, OSSO_DEVMODE_NORMAL, OSSO_DEVMODE_FLIGHT, OSSO_DEVMODE_OFFLINE, OSSO_DEVMODE_INVALID
from context cimport Context
from exceptions import OssoException
cimport libosso

cdef void _hw_callback_handler(osso_hw_state_t *state, void *data) with gil:
    device_state = <DeviceState>data
    device_state.cb_data[0](device_state.cb_data[1])

cdef void _display_callback_handler(osso_display_state_t state, void *data) with gil:
    device_state = <DeviceState>data
    device_state.display_cb_data[0](<int>state, device_state.display_cb_data[1])

cdef class DeviceState:
    def __cinit__(self, Context context not None):
        self.ctx = context.ctx

    def display_state_on(self):
        cdef osso_return_t ret
        ret = osso_display_state_on(self.ctx)
        if ret != OSSO_OK:
            _set_exception(ret, NULL)
	    
    def display_blanking_pause(self):
        cdef osso_return_t ret
        ret = osso_display_blanking_pause(self.ctx)
        if ret != OSSO_OK:
            _set_exception(ret, NULL)

    def set_device_state_callback(self, callback, bint shutdown=False, bint save_data=False,
        bint memory_low=False, bint system_inactivity=False, mode="normal", user_data=None):
        cdef osso_return_t ret
        cdef osso_hw_state_t state

        state.shutdown_ind = shutdown
        state.save_unsaved_data_ind = save_data
        state.memory_low_ind = memory_low
        state.system_inactivity_ind = system_inactivity

        if mode == "normal":
            state.sig_device_mode_ind = OSSO_DEVMODE_NORMAL
        elif mode == "flight":
            state.sig_device_mode_ind = OSSO_DEVMODE_FLIGHT
        elif mode == "offline":
            state.sig_device_mode_ind = OSSO_DEVMODE_OFFLINE
        elif mode == "invalid":
            state.sig_device_mode_ind = OSSO_DEVMODE_INVALID
        else:
            raise OssoException, "Invalid device mode. Use 'normal', 'flight', 'offline' or 'invalid' instead."

        if callback:
            if not callable(callback):
                raise TypeError, "callback parameter must be callable"

            self.cb_data = (callback, user_data)
            ret = osso_hw_set_event_cb(self.ctx, &state, _hw_callback_handler, <void*>self)
        else:
            ret = osso_hw_unset_event_cb(self.ctx, &state)
            self.cb_data = None

        if ret != OSSO_OK:
            _set_exception(ret, NULL)

    def set_display_event_cb(self, callback, user_data=None):
        cdef osso_return_t ret

        if callback:
            if not callable(callback):
                raise TypeError, "callback parameter must be callable"

            self.display_cb_data = (callback, user_data)
            ret = osso_hw_set_display_event_cb(self.ctx, _display_callback_handler, <void*>self)

            if ret != OSSO_OK:
                _set_exception(ret, NULL)

OSSO_DISPLAY_ON = libosso.OSSO_DISPLAY_ON
OSSO_DISPLAY_OFF = libosso.OSSO_DISPLAY_OFF
OSSO_DISPLAY_DIMMED = libosso.OSSO_DISPLAY_DIMMED
