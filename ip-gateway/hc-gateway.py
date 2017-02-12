#!/usr/bin/python
# -*- coding: utf-8 -*-

# HAPCAN IP gateway, skagmo.com

import time
import sys
import hapcan

CAN_PORT = 0
SER_PORT = 0

if (len(sys.argv) == 2):
	if "can" in sys.argv[1]:
		CAN_PORT = sys.argv[1]
		print "Using SocketCAN"
	else:
		SER_PORT = sys.argv[1]
		SER_BAUD = 115200
		print "Using serial port"
else:
	print "Missing port"
	sys.exit(0)

if __name__ == "__main__":
	server = hapcan.socket_server()

	if (SER_PORT):
		import serial
		ser = serial.Serial(SER_PORT, SER_BAUD, timeout=0)
		serialparser = hapcan.parser()

	else:
		import can
		bus = can.interface.Bus(channel=CAN_PORT, bustype='socketcan_ctypes')
		can_reader = can.BufferedReader()
		notifier = can.Notifier(bus, [can_reader])
	
	try:
		while 1:
			pkts = server.poll()
			while pkts:
				for pkt in pkts:
					print "[eth]" + ":".join("{:02x}".format(ord(c)) for c in pkt)
					if (SER_PORT):
						ser.write(pkt)
						#time.sleep(0.02) # HAPCAN serial interface needs delay between packets!
					else:
						# Frame targeted to bus
						if (len(pkt)==15):
							[can_id, can_data] = hapcan.decode_frame(pkt)
							msg = can.Message(arbitration_id=can_id, data=can_data, extended_id=True)
							bus.send(msg)

						# Request directly to interface. Pretends to be an RS-232 interface.
						# Todo: Provide actual correct response
						elif (len(pkt)==5) and (pkt[1] == '\x10'):
							if pkt[2] == "\x40":
								print "Hardware type request to node"
								server.send("\xaa\x10\x41\x30\x00\x03\xff\x00\x00\x07\xa0\x2a\xa5")
							elif pkt[2] == "\x60":
								print "Firmware type request to node"
								server.send("\xaa\x10\x61\x30\x00\x03\x65\x00\x00\x03\x04\x10\xa5")
							elif pkt[2] == "\xe0":
								print "Description request to node"
								server.send("\xaa\x10\xe1\x52\x53\x32\x33\x32\x43\x20\x49\xd9\xa5")
								server.send("\xaa\x10\xe1\x6e\x74\x65\x72\x66\x61\x63\x65\x39\xa5")
							elif pkt[2] == "\xc0":
								print "Supply voltage request to node"
								server.send("\xaa\x10\xc1\xc5\x40\xa7\x70\xff\xff\xff\xff\xe9\xa5")
						else:
							print "HAPCAN packet not known (%u B)" % len(pkt)
				pkts = server.poll()

			if (SER_PORT):
				# Check for new serial data
				for c in ser.read(1000):
					if (serialparser.parse(c)):
						print "[tty]" + ":".join("{:02x}".format(ord(c)) for c in serialparser.data)
						server.send(serialparser.data)
			else:
				# Check for new CAN message
				msg = can_reader.get_message(timeout=0)
				while(msg):
					frame = hapcan.encode_frame(msg.arbitration_id, msg.data)
					print "[can]" + ":".join("{:02x}".format(ord(c)) for c in frame)
					server.send(frame)
					msg = can_reader.get_message(timeout=0)

			time.sleep(0.025)

	finally:
		server.close()
		if (SER_PORT):
			ser.close()
