#!/usr/bin/python2.5

import osso
import gobject


def main():
    
    osso_c = osso.Context("osso_test_note", "0.0.1", False)

    note = osso.SystemNote(osso_c)

    result = note.system_note_dialog("System Note test application", type='notice')

    print result
    loop = gobject.MainLoop()

    loop.run()

if __name__ == "__main__":
    main()
