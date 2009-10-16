
SERVICE_DIR=/usr/share/dbus-1/services

install:
	cp osso_test_receiver.service $(SERVICE_DIR)
	cp osso_test_receiver.py /tmp
	chmod +x /tmp/osso_test_receiver.py
uninstall:
	rm $(SERVICE_DIR)/osso_test_receiver.service 
	rm /tmp/osso_test_receiver.py
