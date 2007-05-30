# Tests osso.ic.get_configured iaps
# Author: Lauro Moura <lauro.neto@indt.org.br>


import osso.ic
import gobject

def main():
    iaps = osso.ic.get_configured_iaps()
    for i in iaps:
        print i

if __name__ == "__main__":
    main()
