#	TCL script by Hrishikesh N
#	AM.EN.P2CSN16008
#	Assignment a) - Analyse traffic between stationary AP and varying number of clients 

# ======================================================================
# Define options
# ======================================================================
set val(chan)           Channel/WirelessChannel    ;# channel type
set val(prop)           Propagation/TwoRayGround   ;# radio-propagation model
set val(netif)          Phy/WirelessPhy            ;# network interface type
set val(mac)            Mac/802_11                 ;# MAC type
set val(ifq)            Queue/DropTail/PriQueue    ;# interface queue type
set val(ll)             LL                         ;# link layer type
set val(ant)            Antenna/OmniAntenna        ;# antenna model
set val(ifqlen)         50                         ;# max packet in ifq
set val(number_of_nodes)             [lindex $argv 0]           ;# number of nodes
set val(rp)             DSDV                       ;# routing protocol
set val(x)              500                        ;# X dimension of topography
set val(y)              500                        ;# Y dimension of topography
# ======================================================================
# Main Program
# ======================================================================
#
# Initialize Global Variables
#
set ns_		[new Simulator]
set tracefd     [open trace.tr w]
$ns_ trace-all $tracefd
set namtrace [open output.nam w]
$ns_ namtrace-all-wireless $namtrace $val(x) $val(y)

set topo [new Topography]
$topo load_flatgrid $val(x) $val(y)
#
# Create God
#
create-god $val(number_of_nodes)
#
#  Create the specified number of mobilenodes [$val(number_of_nodes)] and "attach" them
#  to the channel. 
#  Here two nodes are created : node(0) and node(1)

# configure node

$ns_ node-config -adhocRouting $val(rp) \
-llType $val(ll) \
-macType $val(mac) \
-ifqType $val(ifq) \
-ifqLen $val(ifqlen) \
-antType $val(ant) \
-propType $val(prop) \
-phyType $val(netif) \
-channelType $val(chan) \
-topoInstance $topo \
-agentTrace ON \
-routerTrace ON \
-macTrace ON \
-movementTrace ON			

for {set i 0} {$i < $val(number_of_nodes) } {incr i} {
	set node_($i) [$ns_ node]	
	$node_($i) random-motion 0		;# disable random motion
}
#
# Provide initial (X,Y, for now Z=0) co-ordinates for mobilenodes
#

$node_(0) set X_ 200.0
$node_(0) set Y_ 100.0
$node_(0) set Z_ 0.0

set var [lindex $argv 3]
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
close $tracefd
#exec nam output.nam
exit 0
}

#puts "Starting Simulation..."
$ns_ run