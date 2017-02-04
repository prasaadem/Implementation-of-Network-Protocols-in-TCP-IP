
#Setting up new Simulator for ns
set ns [new Simulator]

#Opening in write mode
set nf [open out.nam w]
set tf [open out.tr w]

#Enable trace
$ns trace-all $tf
$ns namtrace-all $nf

#Setting up output files for write mode
set out1 [open outputfile1.tr w]
set out2 [open outputfile2.tr w]
set out3 [open outputfile3.tr w]

#Initial TCP Sink values
set throughput1 0
set throughput2 0
set throughput3 0

#Check for correct number of inputs
if {$argc!=2} {
puts "Error! Improper number of arguments!"
exit 0
}
 

#Record{} procedure to calculate throughput
proc record1 {} {

#Global variable declaration
global ns out1 out2 tcpsink1 tcpsink2 throughput1 throughput2

#Cumilative operation of TCPSink values
set bw0 [$tcpsink1 set bytes_]
set bw1 [$tcpsink2 set bytes_]


#Setting up time
set time 1.0

#Current time value
set now [$ns now]

#Obtain throughput values
puts $out1 "$now [expr $bw0/$time*8/1000]"
puts $out2 "$now [expr $bw1/$time*8/1000]"

if {$now >= 30 && $now <= 180 }	{
set throughput1 [expr $throughput1+$bw0/$time*8/1000]
set throughput2 [expr $throughput2+$bw1/$time*8/1000]	
puts "Throughput1*100000: $throughput1"
puts "Throughput2*100000: $throughput2"
}    

if {$now == 180} {
puts "Average Throughput in Kbits/sec is : [expr $throughput1/151]"	
puts "Average Throughput in Kbits/sec is : [expr $throughput2/151]"
}

$tcpsink1 set bytes_ 0
$tcpsink2 set bytes_ 0

#Recursion call
$ns at [expr $now + $time] "record1"
}


#Record{} procedure to calculate throughput
proc record2 {} {

#Global variable declaration
global ns out1 out2 out3 tcpsink1 tcpsink2 null1 throughput1 throughput2 throughput3

#Cumilative operation of TCPSink values
set bw0 [$tcpsink1 set bytes_]
set bw1 [$tcpsink2 set bytes_]
set bw2 [$null1 set bytes_]


#Setting up time
set time 1.0

#Current time value
set now [$ns now]

puts $out1 "$now [expr $bw0/$time*8/1000]"
puts $out2 "$now [expr $bw1/$time*8/1000]"
puts $out3 "$now [expr $bw2/$time*8/1000]"

if {$now >= 30 && $now <= 180 }	{
set throughput1 [expr $throughput1+$bw0/$time*8/1000]
set throughput2 [expr $throughput2+$bw1/$time*8/1000]	
set throughput3 [expr $throughput3+$bw2/$time*8/1000]
puts "Throughput1*100000: $throughput1"
puts "Throughput2*100000: $throughput2"
puts "Throughput3*100000: $throughput3"


}    

if {$now == 180} {
puts "Average Throughput in Kbits/sec is : [expr $throughput1/151]"	
puts "Average Throughput in Kbits/sec is : [expr $throughput2/151]"
puts "Average Throughput in Kbits/sec is : [expr $throughput3/151]"

}

$tcpsink1 set bytes_ 0
$tcpsink2 set bytes_ 0
$null1 set bytes_ 0


#Recursion call
$ns at [expr $now + $time] "record2"
}


#Set source, receiver and routers
set R1 [$ns node]
set R2 [$ns node]
set H1 [$ns node]
set H2 [$ns node]
set H3 [$ns node]
set H4 [$ns node]


#Setting up duplex links according to the case argv[1]
if {[lindex $argv 1] eq "1"} {

$H3 color red
$H4 color red

$H3 shape box
$H4 shape box



if {[lindex $argv 0] eq "DROPTAIL"} {
#Duplex between Routers constant

$ns duplex-link $R1 $R2 1Mb 10ms DropTail
$ns queue-limit $R1 $R2 20;
$ns duplex-link $H1 $R1 10Mb 1ms DropTail
$ns duplex-link $H2 $R1 10Mb 1ms DropTail
$ns duplex-link $R2 $H3 10Mb 1ms DropTail
$ns duplex-link $R2 $H4 10Mb 1ms DropTail
puts "Case 1 and DropTail is being implemented. Please wait."
}

if {[lindex $argv 0] eq "RED"} {
#Duplex between Routers constant
Queue/RED set thresh_ 10
Queue/RED set maxthresh_ 15
Queue/RED set linterm_ 50


$ns duplex-link $R1 $R2 1Mb 10ms RED
$ns queue-limit $R1 $R2 20;
$ns duplex-link $H1 $R1 10Mb 1ms RED
$ns duplex-link $H2 $R1 10Mb 1ms RED
$ns duplex-link $R2 $H3 10Mb 1ms RED
$ns duplex-link $R2 $H4 10Mb 1ms RED
puts "Case 1 and RED is being implemented. Please wait."
}

#Setting up TCP Agents
set tcp1 [new Agent/TCP/Reno]
set tcp2 [new Agent/TCP/Reno]

#Set up the TCP Sinks
set tcpsink1 [new Agent/TCPSink]
set tcpsink2 [new Agent/TCPSink]

#Perform attach
$ns attach-agent $H1 $tcp1
$ns attach-agent $H2 $tcp2
$ns attach-agent $H3 $tcpsink1
$ns attach-agent $H4 $tcpsink2

#Perform connect
$ns connect $tcp1 $tcpsink1
$ns connect $tcp2 $tcpsink2

#Setting up FTP Agents and attaching over TCP
set ftp1 [new Application/FTP]
set ftp2 [new Application/FTP]
$ftp1 attach-agent $tcp1
$ftp2 attach-agent $tcp2


$ns duplex-link-op $R1 $R2 orient right
$ns duplex-link-op $H1 $R1 orient 300deg
$ns duplex-link-op $H3 $R2 orient 240deg
$ns duplex-link-op $H2 $R1 orient 60deg
$ns duplex-link-op $H4 $R2 orient 120deg


}

if {[lindex $argv 1] eq "2"} {

set H5 [$ns node]
set H6 [$ns node]

$H3 shape box
$H3 color red

$H4 color green
$H5 color green
$H6 color green

$H4 shape box
$H5 shape box
$H6 shape box


if {[lindex $argv 0] eq "DROPTAIL"} {
#Duplex between Routers constant

$ns duplex-link $R1 $R2 1Mb 10ms DropTail
$ns queue-limit $R1 $R2 20;
$ns duplex-link $H1 $R1 10Mb 1ms DropTail
$ns duplex-link $H2 $R1 10Mb 1ms DropTail
$ns duplex-link $H3 $R1 10Mb 1ms DropTail
$ns duplex-link $R2 $H4 10Mb 1ms DropTail
$ns duplex-link $R2 $H5 10Mb 1ms DropTail
$ns duplex-link $R2 $H6 10Mb 1ms DropTail
puts "Case 2 and DropTail is being implemented. Please wait."
}

if {[lindex $argv 0] eq "RED"} {
#Duplex between Routers constant
Queue/RED set thresh_ 10
Queue/RED set maxthresh_ 15
Queue/RED set linterm_ 50


$ns duplex-link $R1 $R2 1Mb 10ms RED
$ns queue-limit $R1 $R2 20;
$ns duplex-link $H1 $R1 10Mb 1ms RED
$ns duplex-link $H2 $R1 10Mb 1ms RED
$ns duplex-link $H3 $R1 10Mb 1ms RED
$ns duplex-link $R2 $H4 10Mb 1ms RED
$ns duplex-link $R2 $H5 10Mb 1ms RED
$ns duplex-link $R2 $H6 10Mb 1ms RED
puts "Case 2 and RED is being implemented. Please wait."
}

#Setting up TCP Agents
set tcp1 [new Agent/TCP/Reno]
set tcp2 [new Agent/TCP/Reno]
set udp1 [new Agent/UDP]

#Set up the TCP Sinks
set tcpsink1 [new Agent/TCPSink]
set tcpsink2 [new Agent/TCPSink]
set null1 [new Agent/LossMonitor]

#Perform attach
$ns attach-agent $H1 $tcp1
$ns attach-agent $H2 $tcp2
$ns attach-agent $H3 $udp1
$ns attach-agent $H6 $tcpsink1
$ns attach-agent $H4 $tcpsink2
$ns attach-agent $H5 $null1

#Perform connect
$ns connect $tcp1 $tcpsink1
$ns connect $tcp2 $tcpsink2
$ns connect $udp1 $null1

#Setting up FTP Agents and attaching over TCP
set ftp1 [new Application/FTP]
set ftp2 [new Application/FTP]
set cbr1 [new Application/Traffic/CBR]

$cbr1 set packetSize_ 100
$cbr1 set rate_ 1Mb
$cbr1 set random_ false

$ftp1 attach-agent $tcp1
$ftp2 attach-agent $tcp2
$cbr1 attach-agent $udp1

$ns duplex-link-op $R1 $R2 orient right
$ns duplex-link-op $H1 $R1 orient 300deg
$ns duplex-link-op $H6 $R2 orient 240deg
$ns duplex-link-op $H2 $R1 orient 60deg
$ns duplex-link-op $H4 $R2 orient 120deg


$ns duplex-link-op $H3 $R1 orient right
$ns duplex-link-op $H5 $R2 orient 180deg

}

#Making Custom Layout

$H1 shape box
$H1 color red
$H2 color red
$H2 shape box

$ns duplex-link-op $R1 $R2 color "blue"
$ns duplex-link-op $R1 $R2 label "backbone link"



#Finish{} Procedure
proc finish1 {} {
global ns nf tf
$ns flush-trace

close $nf
close $tf

exec xgraph outputfile1.tr outputfile2.tr -geometry 800x400 &

exec nam out.nam &
puts "You should see NAM and XGRAPH now. If a message appears as 'Problems with input data', please open the output1.tr and output2.tr files from gedit, or use the xgrpah command to open it."
exit 0
}

#Finish{} Procedure
proc finish2 {} {
global ns nf tf
$ns flush-trace

close $nf
close $tf

exec xgraph outputfile1.tr outputfile2.tr -geometry 800x400 &
exec xgraph outputfile3.tr -geometry 800x400 &


exec nam out.nam &
puts "You should see NAM and XGRAPH now. If a message appears as 'Problems with input data', please open the output1.tr and output2.tr files from gedit, or use the xgrpah command to open it."
exit 0
}


#Simulations

if {[lindex $argv 1] eq "1"} {
$ns at 0.5 "$ftp1 start"
$ns at 0.5 "$ftp2 start"
set choice 1

#Call Record
$ns at 30 "record1"

$ns at 180 "$ftp1 stop"
$ns at 180 "$ftp2 stop"


#Call finish
$ns at 185 "finish1"

$ns run

}

if {[lindex $argv 1] eq "2"} {
$ns at 0.5 "$ftp1 start"
$ns at 0.5 "$ftp2 start"
$ns at 0.5 "$cbr1 start"

#Call Record
$ns at 30 "record2"

$ns at 180 "$ftp1 stop"
$ns at 180 "$ftp2 stop"
$ns at 180 "$cbr1 stop"


#Call finish
$ns at 185 "finish2"

$ns run

}
