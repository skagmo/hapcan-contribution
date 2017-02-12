from array import array

HOST = "0.0.0.0"
PORT = 10001
RECV_BUFFER = 65536
SKT_TIMEOUT = 0

def verify(data):
	if (len(data)!=15) and (len(data)!=5) and (len(data)!=13):
		return 0
	if (data[-1] != '\xa5'):
		return 0
	chk = sum(array("B", data[1:-2])) % 256
	if (chk != ord(data[-2])):
		return 0
	return 1

# Convert from CAN frame ID and byte array to HAPCAN format
def encode_frame(can_id, can_data):
	frame = [0xaa]
	frame.append((can_id & 0x1fe00000) >> 21)
	frame.append(((can_id & 0x1e0000) >> 13) | ((can_id & 0x10000) >> 16))
	frame.append((can_id & 0xff00) >> 8)
	frame.append(can_id & 0xff)
	for d in can_data:
		frame.append(d)
	frame.append(sum(frame[1:]) % 256)
	frame.append(0xa5)
	return ''.join(map(chr, frame))

def decode_frame(frame):
	can_id = ord(frame[1]) << 21
	can_id += (ord(frame[2])&0xf0) << 13
	can_id += (ord(frame[2])&0x1) << 16
	can_id += ord(frame[3]) << 8
	can_id += ord(frame[4])
	can_data = frame[5:13]
	return [can_id, can_data]

class parser:
	def __init__(self):
		self.START=0
		self.DATA=1
		self.state = self.START
		
	def parse(self, c):
		if (self.state == self.START) and (c == '\xaa'):
			self.data = c
			self.state = self.DATA
		elif (self.state == self.DATA):
			self.data += c
			if (verify(self.data)):
				self.state = self.START
				return 1
			if (len(self.data) > 15):
				#print "Oversize"
				self.state = self.START
		return 0

import socket, select

class socket_server:
	def __init__(self):
		self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		self.server_socket.bind((HOST, PORT))
		self.server_socket.listen(10)
		self.connections = []
		self.connections.append(self.server_socket)
		self.parser = parser()

	def close(self):
		self.server_socket.close()

	def __del__(self):
		self.close()

	def poll(self):
		# Get the list sockets which are ready to be read through select
		read_sockets,write_sockets,error_sockets = select.select(self.connections,[],[],SKT_TIMEOUT)

		for sock in read_sockets:
			# New connection
			if sock == self.server_socket:
				sockfd, addr = self.server_socket.accept()
				self.connections.append(sockfd)
				print "Client (%s, %s) connected" % addr

			# Data from client
			else:
				try:
					ethdata = sock.recv(RECV_BUFFER)
					if ethdata:
						pkts = []
						for c in ethdata:
							if (self.parser.parse(c)):
								pkts.append(self.parser.data)	
						if (len(pkts)):
							return pkts
					# If null packet, disconnect client
					else:
						print "Client disconnected"
						sock.close()
						self.connections.remove(sock)
				except Exception as e:
					print e
		return 0

	def send(self, data):
		for k in self.connections:
			if not (k == self.server_socket):
				try:
					k.send(data)
				except Exception as e:
					print e
					try:
						print "Client disconnected"
						k.close()
						self.connections.remove(k)
					except Exception as e:
						print e

