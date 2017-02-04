
#Setting up new Simulator for ns
set ns [new Simulator]

#Opening in write mode
set nf [open out.nam w]
set tf [open out.tr w]

#Enable trace
$ns trace-all $tf
$ns namtrace-all $nf

#Setting up output files for write mode
set out1 [open output1.tr w]
set out2 [open output2.tr w]
set out3 [open output3.tr w]

#Initial TCP Sink values
set initial1 0
set initial2 0

#Check for correct number of inputs
if {$argc!=2} {
puts "Error! Improper number of arguments!"
exit 0
}
 

#Record{} procedure to calculate throughput
proc record {} {

#Global variable declaration
global ns out1 out2 out3 tcpsink1 tcpsink2 initial1 initial2

#Cumilative operation of TCPSink values
set bw0 [$tcpsink1 set bytes_]
set bw1 [$tcpsink2 set bytes_]

#Setting up time
set time 20

#Current time value
set now [$ns now]

if {$now == 100} {

set initial1 $bw0
set initial2 $bw1
}

#Obtain throughput values
if {$now > 100 } {

set throughput1 [expr $bw0/($now)]
set throughput2 [expr $bw1/($now)]

set ratio [expr $throughput1/$throughput2]

#Push values into output file
puts $out1 "$now $throughput1"
puts $out2 "$now $throughput2"
puts $out3 "$ratio"
puts "Throughput1*10000: $throughput1"
puts "Throughput2*10000: $throughput2"
}

#Recursion call
$ns at [expr $now + $time] "record"
}

#Set source, receiver and routers
set R1 [$ns node]
set R2 [$ns node]
set src1 [$ns node]
set src2 [$ns node]
set rcv1 [$ns node]
set rcv2 [$ns node]

#DropTail Duplex between Routers constant
$ns duplex-link $R1 $R2 1Mb 5ms DropTail

#Setting up duplex links according to the case argv[1]
if {[lindex $argv 1] eq "1"} {
$rcv1 color red
$rcv2 color red
$ns duplex-link $src1 $R1 10Mb 5ms DropTail
$ns duplex-link $R2 $rcv1 10Mb 5ms DropTail
$ns duplex-link $src2 $R1 10Mb 12.5ms DropTail
$ns duplex-link $R2 $rcv2 10Mb 12.5ms DropTail
puts "Case 1 is being implemented. Please wait."
}

if {[lindex $argv 1] eq "2"} {
$rcv1 color green
$rcv2 color green
$ns duplex-link $src1 $R1 10Mb 5ms DropTail
$ns duplex-link $R2 $rcv1 10Mb 5ms DropTail
$ns duplex-link $src2 $R1 10Mb 20ms DropTail
$ns duplex-link $R2 $rcv2 10Mb 20ms DropTail
puts "Case 2 is being implemented. Please wait."
}

if {[lindex $argv 1] eq "3"} {
$rcv1 color blue
$rcv2 color blue
$ns duplex-link $src1 $R1 10Mb 5ms DropTail
$ns duplex-link $R2 $rcv1 10Mb 5ms DropTail
$ns duplex-link $src2 $R1 10Mb 27.5ms DropTail
$ns duplex-link $R2 $rcv2 10Mb 27.5ms DropTail
puts "Case 3 is being implemented. Please wait."
}

#Setting up TCP Agents according to TCP SACK or VEGAS argv[0]
if {[lindex $argv 0] eq "SACK"} {
set tcp1 [new Agent/TCP/Sack1]
set tcp2 [new Agent/TCP/Sack1]
puts "SACK is being implemented. Please wait."

}

if {[lindex $argv 0] eq "VEGAS"} {
set tcp1 [new Agent/TCP/Vegas]
set tcp2 [new Agent/TCP/Vegas]
puts "VEGAS is being implemented. Please wait."

}

#Set up the TCP Sinks
set tcpsink1 [new Agent/TCPSink]
set tcpsink2 [new Agent/TCPSink]

#Perform attach
$ns attach-agent $src1 $tcp1
$ns attach-agent $src2 $tcp2
$ns attach-agent $rcv1 $tcpsink1
$ns attach-agent $rcv2 $tcpsink2

#Perform connect
$ns connect $tcp1 $tcpsink1
$ns connect $tcp2 $tcpsink2

#Setting up FTP Agents and attaching over TCP
set ftp1 [new Application/FTP]
set ftp2 [new Application/FTP]
$ftp1 attach-agent $tcp1
$ftp2 attach-agent $tcp2

#Making Custom Layout

$R1 shape box
$R1 color red
$R2 color red
$R2 shape box

$ns duplex-link-op $R1 $R2 color "green"
$ns duplex-link-op $R1 $R2 label "backbone link"


$ns duplex-link-op $R1 $R2 orient right
$ns duplex-link-op $src1 $R1 orient 300deg
$ns duplex-link-op $rcv1 $R2 orient 240deg
$ns duplex-link-op $src2 $R1 orient 60deg
$ns duplex-link-op $rcv2 $R2 orient 120deg


#Finish{} Procedure
proc finish {} {
global ns nf tf
$ns flush-trace

close $nf
close $tf

exec xgraph output1.tr output2.tr -geometry 800x400 &
exec nam out.nam &
puts "You should see NAM and XGRAPH now. If a message appears as 'Problems with input data', please open the output1.tr and output2.tr files from gedit, or use the xgrpah command to open it."
exit 0
}

#Simulations
$ns at 0.5 "$ftp1 start"
$ns at 0.5 "$ftp2 start"

#Call Record
$ns at 100 "record"

$ns at 400 "$ftp1 stop"
$ns at 400 "$ftp2 stop"


#Call finish
$ns at 405 "finish"

$ns run



