#!/usr/bin/env python
#self-learning and forwarding of bridge/switch

try:
	from socket import *
	from time import ctime
	import socket
	import argparse,sys,select
	
except ImportError as e:
	print 'ERROR IN IMPORTING PYTHON LIBRARIES: ',e.value

#Begin parsing command line arguments
parser = argparse.ArgumentParser(description='Processing Command line inputs of self-learning and forwarding of bridge/switch')

parser.add_argument('-s', type=int, default=4321, help='Is the udp port on which a switch will listen to receive msg from connected devices ')
parser.add_argument('-p', type=str, default='1301,1302,1303,1304,1305,1306,1307,1308', help='Ports which implies that devices are connected i.e. those devices having these ethernet addresses ')

args = parser.parse_args()#to parse the programs cmd line

if args.s:
       listen_port=args.s
if args.p:
       switch_udp_port=args.p

#End parsing command line arguments

sys_ipaddr='127.0.0.1'


if listen_port < 1024 or listen_port > 65535:
	sys.exit(2)


try:
	print "The udp port on which a switch will listen to receive msg = ", listen_port
	print '**********************************************************************************'
	print 'Self-learning and forwarding of bridge/switch'
	print '**********************************************************************************'


except IOError, msg:
	parser.error(str(msg))

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)    # Create Datagram Socket (UDP)

addr0 = socket.getaddrinfo(sys_ipaddr, listen_port, AF_INET, SOCK_DGRAM, IPPROTO_UDP, 0)
system_addr=addr0[0][4]

s.bind(system_addr)
switch_udp_port = switch_udp_port.split(",")
n=len(switch_udp_port)
print 'Number of pc:',n	
if n<=8:

	mac=[]
	while 1:
		r,w,e = select.select([sys.stdin,s],[],[])
		for read in r:
			info = s.recv(8192)
			info = info.split(",")
			if info[0]=="Broadcast":
				#print "This is a broadcast reply"
				if int(info[1]) not in mac:
					print 'Mac table updated with broadcast reply'
					mac.append(int(info[1]))
					for i in mac:
			   			print 'port:',i
			   			print	
				continue
			dst_port = int(info[0])	
			src_port=int(info[1])
			message = info[2]
			flag=0
			if message!='':
				print
				print 'Received Message:'
				print 'msg:',info
				print 'dest port',dst_port
				for d in range(0,n):
					dst=int(switch_udp_port[d])
					if dst==dst_port:
						flag=1		
				if flag==1:
					print 'given destination port is valid'
					print
				else:
					print 'given destination port is not valid,so exiting from program...'
					sys.exit(2)				  		
				if dst_port in mac:
					addr1 = socket.getaddrinfo(sys_ipaddr, dst_port, AF_INET, SOCK_DGRAM, IPPROTO_UDP, 0)
					dst_addr1 = addr1[0][4]
					s.sendto('%s %s %s %s %s'%(dst_port,',',src_port,',',message),dst_addr1)
					print
					print 'Unicasting'
					print 'Message Received '
					print 'Dst port:',dst_port 
					print 'Message:',message
					#print 'Time:%s'%ctime()
					print '...............................................'  	
					if src_port in mac:
						print 'Source address available in MAC table. No need to learn'	
					else:
						mac.append(src_port)
						print
						print 'MAC table updated. Current table looks like...'
						print '______________________________'
						for i in mac:
			   				print 'port:',i
			   				print	
						print '______________________________'        
						print
				else:
					print
					print 'packet broadcasting'
					if src_port in mac:
						print 'source port known to switch but destination not known, so broad casting'
						print
					else:
						mac.append(src_port)
						print
						print 'MAC table entry'
						print '_______________________'
						for i in mac:
		   					print 'port:',i
		   					print
						print '_______________________'
						print
					for k in range(0,n):
						switch_udp_port_t=int(switch_udp_port[k])				
						if src_port!=switch_udp_port_t:
				
							addr=socket.getaddrinfo(sys_ipaddr,switch_udp_port_t, AF_INET, SOCK_DGRAM, IPPROTO_UDP, 0)
				
							dst_addr=addr[0][4]
							s.sendto('%s %s %s %s %s'%(dst_port,',',src_port,',',message),dst_addr)	
else:
	print 'switch has only 8 ports to connect pc'			
