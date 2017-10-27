
# ======================================================================
# Define options
# ======================================================================
set val(chan)           Channel/WirelessChannel    ;#Channel Type
set val(prop)           Propagation/TwoRayGround   ;# radio-propagation model
set val(netif)          Phy/WirelessPhy            ;# network interface type
set val(mac)            Mac/802_11                 ;# MAC type
set val(ifq)            Queue/DropTail/PriQueue    ;# interface queue type
set val(ll)             LL                         ;# link layer type
set val(ant)            Antenna/OmniAntenna        ;# antenna model
set val(ifqlen)         50                         ;# max packet in ifq
set val(number_of_nodes)             [lindex $argv 0]           ;# number of nodes
set val(malicious)		[lindex $argv 1]	       ;#Number of Malicious nodes
set val(rp)             AODV                       ;# routing protocol
set val(x)              750                        ;# X dimension of topography
set val(y)              750                        ;# Y dimension of topography  


#  Global Variables
set ns_         [new Simulator]
set tracefd     [open trace.tr w]
set namtrace [open output.nam w]
$ns_ trace-all $tracefd

set namtrace [open project.nam w]
$ns_ namtrace-all-wireless $namtrace $val(x) $val(y)

# set up topography object
set topo       [new Topography]
$topo load_flatgrid $val(x) $val(y)

# Create God
create-god $val(number_of_nodes)

# Create channel
set chan_1_ [new $val(chan)]

$ns_ node-config -adhocRouting $val(rp) \
                -llType $val(ll) \
                -macType $val(mac) \
                -ifqType $val(ifq) \
                -ifqLen $val(ifqlen) \
                -antType $val(ant) \
                -propType $val(prop) \
                -phyType $val(netif) \
                -topoInstance $topo \
                -agentTrace ON \
                -routerTrace ON \
                -macTrace ON\
                -movementTrace ON \
                -channel $chan_1_


for {set i 0} {$i < $val(number_of_nodes) } {incr i} {
	set node_($i) [$ns_ node]	
	$node_($i) random-motion 0		;# disable random motion
}

$node_(0) set X_ 200.0
$node_(0) set Y_ 100.0
$node_(0) set Z_ 0.0


set var [lindex $argv 2]
# puts "The second arg is [lindex $argv 1]";
$defaultRNG seed [lindex $argv 3]
set arr_RNG [new RNG]
set arr_ [new RandomVariable/Exponential]
$arr_ set avg_ 9
$arr_ use-rng $arr_RNG
#puts "Inter-arrival time Packet size"
for {set j 1} {$j < $val(number_of_nodes)} {incr j} {
	set xx_ [$arr_ value]
	$node_($j) set X_ [expr 15 * $xx_]
	$node_($j) set Y_ [expr 10 * log($xx_)]
	$node_($j) set Z_ 0.0
}

for {set i 0} {$i < [expr $val(number_of_nodes)]} {incr i} {
$ns_ initial_node_pos $node_($i) 30
}

Application/Traffic/CBR set packetSize_ 1023
Application/Traffic/CBR set rate_ 256Kb

for {set i 0} {$i < $val(malicious)} {incr i} {
	$ns_ at 0.0 "$node_($i) set ragent_ malicious"  
}


for {set i 1} {$i < [expr $val(number_of_nodes)]} {incr i} {
    set udp($i) [new Agent/UDP]
    $ns_ attach-agent $node_($i) $udp($i)
    set cbr($i) [new Application/Traffic/CBR]
    $cbr($i) attach-agent $udp($i)
    set null_($i) [new Agent/Null]
    $ns_ attach-agent $node_(0) $null_($i)
    $ns_ connect $udp($i) $null_($i)
    $ns_ at $i  "$cbr($i) start"
}

$ns_ at 20.0 "stop"
$ns_ at 20.0 "puts \"NS EXITING...\" ; $ns_ halt"

proc stop {} {
    global ns_ tracefd
    $ns_ flush-trace
    exec nam output.nam
    close $tracefd
    exit 0
}

puts "Starting Simulation..."
$ns_ run
