#!/usr/bin/python

import serial, struct, time
import Tkinter
from Tkinter import *
import Tkinter as ttk
import binascii

CMD_MIN = 0x06
CMD_MAX = 0x05
CMD_GETLEVEL = 0xA0

class dali:
	def __init__(self, portname):
		self.portname = portname
		self.open()
		self.buf = ""
		self.queue = []
		
	def __del__(self):
		self.close()

	def open(self):
		self.ser = serial.Serial(self.portname,38400,timeout=0)

	def close(self):
		if self.ser:
			self.ser.close()
			self.ser = False

	def cmd(self, addr, cmd):
		self.ser.write(binascii.hexlify(chr(addr)+chr(cmd)).upper() + "\n")
		time.sleep(0.05)

	def tick(self):
		while self.ser.inWaiting():
			self.buf += self.ser.read(100)	   
			pos = self.buf.find("\n")
			while pos>=0:
				line = self.buf[:pos]
				self.buf = self.buf[pos+1:]
				pos = self.buf.find("\n")
				self.queue.append(line.strip())

	def get_reply(self):
		time.sleep(0.1)
		self.tick()
		ret = False
		if (len(self.queue)):
			if (self.queue[-1]=="FF"):
				ret = True
			self.queue[:] = []
		return ret

	def compare(self, addr):
		#Address high to low, then compare
		self.cmd(0xB1, (addr&0xff0000)>>16)
		self.cmd(0xB3, (addr&0xff00)>>8)
		self.cmd(0xB5, (addr&0xff))
		self.cmd(0xA9, 0x00)
		return self.get_reply()
		
	def search(self):
		addr = 0x800000
		for j in [pow(2,x) for x in range(22,-1,-1)]:
			state = self.compare(addr)
			print "Comparison with address 0x%0.6x = %u" % (addr, state)
			if state:
				addr -= j
			else:
				addr += j
		# Add one back if last operation was true (equal or higher)
		if state:
			addr += 1
		if (self.compare(addr)):
			return addr
		else:
			addr += 1
			if (self.compare(addr)):
				return addr
			print "Final comparison with 0x%0.6x failed" % addr
			return 0x1000000
		

	def set_arc(self, addr, level):
		self.cmd(addr<<1, level)

	def command(self, addr, cmd):
		self.cmd((addr<<1)|1, cmd)

	def set_fade_time(self, addr, fade_time):
		#self.command(addr, 129) # Enable write memory
		#self.command(addr, 129) # Enable write memory
		self.cmd(0xA5, 0)
		self.cmd(0xA5, 0)
		self.cmd(0b10100011, fade_time)   # Write to DTR
		self.cmd((addr<<1)|1, 0b00101110) # Store DTR as fade time
		self.cmd((addr<<1)|1, 0b00101110) # Store DTR as fade time

	def add_to_group(self, addr, group):
		# Configuration commands must be repeated within 100 ms
		self.command(addr, 0b01100000 | group) # Add to group
		self.command(addr, 0b01100000 | group)

	def init(self, address):

		# Initialize, then randomize - ready for special command next 15 minutes
		print "Initializing and randomizing"
		self.cmd(0xA5, 0)
		self.cmd(0xA5, 0)
		time.sleep(0.3)
		self.cmd(0xA7, 0)
		self.cmd(0xA7, 0)

		while 1:
			print "Searching for long address"
			# Search for long address
			long_addr = self.search()
			if long_addr < 0x1000000:			
				print "Found long address 0x%0.6x" % long_addr
			else:
				print "Didn't find new long address, finished"
				return

			# Program short address, then verify
			self.cmd(0xB7, (address<<1)|1)
			self.cmd(0xB7, (address<<1)|1)
			time.sleep(2)
			self.cmd(0xB9, (address<<1)|1)

			if self.get_reply():
				print "Programmed short address 0x%0.2x" % address
			else:
				print "Failed to program short address"

			# Withdraw
			print "Withdrawing"
			self.cmd(0xAB, 0)
			
			address += 1

d = dali("/dev/ttyUSB0")

#d.init(12)

#d.set_fade_time(4, 1)

d.add_to_group(10,2) 
d.add_to_group(11,2) 
d.add_to_group(14,2)
#time.sleep(1)
#d.command(4, 0x00)
#time.sleep(1)
#d.set_arc(4, 250)

while(1):
	for j in range(9,15):
		print "Controlling DALI driver %u" % j
		d.command(66, 0x05) # goto MAX
		time.sleep(2)
		d.command(66, 0) # MIN


#dimmerange 0x95 - 0xFE

