#Make a NS simulator 
set ns [new Simulator]        
set nf [open out.nam w] 
$ns namtrace-all $nf 
set nr [open packet_size_64.tr w]
$ns trace-all $nr

# Define a 'finish' procedure
# Define a 'finish' procedure 

proc finish {} {

global ns nf

$ns flush-trace

#Close the NAM trace file

close $nf

#Execute NAM on the trace file

exec nam out.nam &


exit 0

}

# Create the nodes:
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
set n6 [$ns node]

$ns duplex-link $n1 $n5 10Mb 1ms DropTail
$ns duplex-link $n2 $n5 10Mb 1ms DropTail
$ns duplex-link $n3 $n6 10Mb 1ms DropTail
$ns duplex-link $n4 $n6 10Mb 1ms DropTail
$ns duplex-link $n5 $n6 1.5Mb 1ms DropTail


#Give node position (for NAM)

$ns duplex-link-op $n1 $n5 orient right-down 
$ns duplex-link-op $n2 $n5 orient right-up
$ns duplex-link-op $n5 $n6 orient right
$ns duplex-link-op $n6 $n3 orient right-up
$ns duplex-link-op $n6 $n4 orient right-down


# ########################################################
# Set Queue Size of link (n2-n3) to 10 (default is 50 ?)
#  $ns queue-limit $n2 $n3 10


# Add a TCP sending module to node n1
set tcp1 [new Agent/TCP/Reno]
$ns attach-agent $n1 $tcp1

# ########################################################
$tcp1 set window_ 8000
$tcp1 set packetSize_ 552


# Add a TCP receiving module to node n4
set sink1 [new Agent/TCPSink]
$ns attach-agent $n4 $sink1

# Direct traffic from "tcp1" to "sink1"
$ns connect $tcp1 $sink1

# Setup a FTP traffic generator on "tcp1"
set ftp1 [new Application/FTP]
$ftp1 attach-agent $tcp1
$ftp1 set type_ FTP               

# Schedule start/stop times
$ns at 0.1   "$ftp1 start"
$ns at 10.0 "$ftp1 stop"

# Set simulation end time
$ns at 15.0 "finish"            




# Run simulation !!!!
$ns run
