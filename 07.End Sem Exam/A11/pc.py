#!/usr/bin/env python
#pc program

try:
	from socket import *
	from time import ctime
	import socket
	import argparse,sys,select
	
except ImportError as e:
        print 'ERROR IN IMPORTING PYTHON LIBRARIES: ',e.value
parser = argparse.ArgumentParser(description='Processing Command line inputs for pc program')
parser.add_argument('-s', type=int, default=1301, help='My source port program will listen for incoming msgs on this UDP port')
parser.add_argument('-d', type=int, default=4321, help='Indicates in which switch udp port this PC is connected. On invoking the program')
parser.add_argument('-m', type=int, default=1024, help='Indicates mac address')
args = parser.parse_args()	#To parse the programs cmd line

if args.s:
        pcsource_port=args.s
if args.d:
        switch_udp_port=args.d
if args.m:
        mac=args.m


#End parsing command line arguments

sys_ipaddr='127.0.0.1'

if pcsource_port < 1024 or switch_udp_port < 1024 or pcsource_port > 65535 or switch_udp_port > 65535:
	print 'Argument value range invalid'
	sys.exit(2)


try:
	print"-----------------------------------------------------------------------------------"
	print "Source port=", pcsource_port
	print 'switch udpport =  ', switch_udp_port
   
	print '**********************************************************************************'
	print 'PC'
	print '**********************************************************************************'
	print

except IOError, msg:
    parser.error(str(msg))

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)    # Create Datagram Socket (UDP)

addr0 = socket.getaddrinfo(sys_ipaddr, pcsource_port, AF_INET, SOCK_DGRAM, IPPROTO_UDP, 0)
system_addr=addr0[0][4] 	#Retrieves the tuple with ip address and port , were as all others are not relevant to the program

addr1 = socket.getaddrinfo(sys_ipaddr, switch_udp_port, AF_INET, SOCK_DGRAM, IPPROTO_UDP, 0)
dst_addr1 = addr1[0][4]

s.bind(system_addr)  


#message

while 1:
	print 'Please provide the destination port and message:'
	print
	r,w,e = select.select([sys.stdin,s],[],[])
	for read in r:
		if read == sys.stdin: 
			msg = sys.stdin.readline()
			m=msg.split(",");
			dst_port=int(m[0])
			info=m[1]
			print
			print 'Me:', msg
			#print 'Time:%s'%ctime()  
			print '................................................'  
			print
			s.sendto('%s %s %s %s %s'%(dst_port,',',pcsource_port,',',info),dst_addr1)
		
		else:

			info = s.recv(8192)
			info = info.split(",")
			dst_port = int(info[0])
			if pcsource_port==dst_port:
				src_port = int(info[1])
				message = info[2]
		
				if message!='':
					print
					print 'Received Message:'
					print 'Dst port ',dst_port
					print 'Src port:',src_port 
					print 'Message:',message
					#print 'MAC: ',mac
					s.sendto('Broadcast,%s'%str(dst_port),dst_addr1)
					#print 'Time:%s'%ctime()
					print '...............................................'  	
					print 
			else:
				print
				print 'Revieved brocadcast packet,But I am not a designated pc to recieve this packet so discarding'					
				print		
