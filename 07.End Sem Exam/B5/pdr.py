#!/usr/bin/env python

import os
import sys
import argparse
import re

def psize(name):
	ps = re.split('_|\.' , name)
	return int(ps[len(ps)-2])

def throughput(filenames):
	data = []
	for filename in filenames:
		name = re.split('_|\.', filename)
		packetsize = int(name[len(name)-2])
		data.append((packetsize, throughput_helper(filename , packetsize)))
		pdrop_helper(filename, packetsize)
		
	return data

def throughput_helper(filename , packetsize):
	trace = open(filename ,  'r')
	count = 0
	start = 0.0
	end = 0.0
	prev = 0.0
	for line in trace:
		if line.startswith('+') and 'tcp' in line and str(packetsize + 40) in line:
			words = line.split(' ')
			prev = float(words[1])	
			if count == 0:
				start = prev		
			count += 1
	end = prev
	throughput = (count*8.0*(packetsize + 40))/((end-start)*1000000)
	print('throughput ' + str(throughput))
	trace.close()
	return throughput
	
def pdrop_helper(filename , packetsize):
	trace = open(filename ,  'r')
	countd = 0
	countr = 0
	
	for line in trace:
	        if line.startswith('d'):
	                countd = countd + 1
	        if line.startswith('r'):
	                countr = countr + 1

    # if countr == 0:
    #     print('pdr = INF ')
    # else:
        
    #     print('pdr ' + str(ratio))
    
	ratio = countd/countr
	print('ratio ' + str(ratio))
	print('packet drop ' + str(ratio))
	print('packet receive ' + str(ratio))	

def create_file(data):
	datfile = open('plotvalues.dat', 'w')
	datfile.write('#X\tY\n')
	for (k , v) in data:
		datfile.write(str(k) + '\t' + str(v) + '\n')
	datfile.close()

def plot(data):
	gp = open('throughputplot.gp' , 'w')
	gp.write('set grid\n')
	gp.write('set title \"Performance of Stop-n-wait\"\n')
	gp.write('set xlabel \"Packet Size(bytes)\"\n')
	gp.write('set ylabel \"Throughput(mbps)\"\n')
	gp.write('set xrange [64:5000]\n')
	l = []
	for (key, value) in data:
		l.append(str(key))
	s = " , ".join(l)
	gp.write('set xtics (' + s + ')\n')
	gp.write('plot \"plotvalues.dat\" using 1:2:(sprintf(\"(%d,%f)\", $1 , $2)) with labels center offset 3.4,0.5 notitle, \'\' with linespoints pointtype 7\n')
	gp.write('pause -1')

def main():
	parser = argparse.ArgumentParser(description = 'Process the tracefiles and print the throughput')
	parser.add_argument('filenames' , metavar='filename', type=str , nargs='+',
						help='ns2 tracefiles for which you want the throughputs in mbps. The throughputs will be used to generate the graph')
	args = parser.parse_args()
	data = throughput(sorted(args.filenames, key=psize))
	create_file(data)
	plot(data)

if __name__ == '__main__':
	main()
