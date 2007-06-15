#!/usr/bin/python2.5

import osso
import gtk
import hildon

# Creates a program with a text entry and a hscale and keep the state
# during a session, even if the program is closed.
# Author: Lauro Moura <lauro.neto@indt.org.br>

class TestStateSaving(hildon.Program):
    def __init__(self):
        hildon.Program.__init__(self)
        self.window = hildon.Window()
        self.window.connect("destroy", self.quit)
        self.add_window(self.window)

        vbox = gtk.VBox()
        self.window.add(vbox)

        self.entry = gtk.Entry()
        vbox.add(self.entry)

        self.hscale = gtk.HScale(gtk.Adjustment(upper=10, step_incr=1))
        vbox.add(self.hscale)

        self.context = osso.Context("osso_test_statesaving", "0.0.1", False)
        self.stateSaving = osso.StateSaving(self.context)
        self.connect("notify::is-topmost", self.topmost_change)

        self.state_read()

        self.window.show_all()

    def run(self):
        gtk.main()

    def quit(self, evt):
        self.save_state()
        gtk.main_quit()

    # Read the data from the state file and set the widgets values
    # if it exists.
    def state_read(self):
        try:
            data = self.stateSaving.state_read()
            print "State read = ", data
            self.entry.set_text(data[0])
            self.hscale.get_adjustment().set_value(data[1])
        except osso.OssoNoStateException, message:
            print "Error loading state: %s" % message

    # Builds the data from the widget and saves to the state file
    def save_state(self):
        print "Saving state"
        text = self.entry.get_text()
        nums = self.hscale.get_adjustment().get_value()

        self.stateSaving.state_write((text, nums))

    # Called whenever the application is sent to background or
    # get to foreground. If it goes to background, saves the state.
    def topmost_change(self, arg, user_data):
        if self.get_is_topmost():
            self.set_can_hibernate(False)
        else:
            self.save_state()
            self.set_can_hibernate(True)

def main():
    prog = TestStateSaving()
    prog.run()

if __name__ == "__main__":
    main()
