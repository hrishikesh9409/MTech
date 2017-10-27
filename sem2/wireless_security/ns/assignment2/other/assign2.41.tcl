# With all nodes Misbehaving - Contention Window With Packet size 512


# =========================================================================================
# Define options
# =========================================================================================
set val(chan)           Channel/WirelessChannel               ;# channel type
set val(prop)           Propagation/TwoRayGround              ;# radio-propagation model
set val(netif)          Phy/WirelessPhy                       ;# network interface type
set val(mac)            Mac/802_11                            ;# MAC type
set val(ifq)            Queue/DropTail/PriQueue               ;# interface queue type
set val(ll)             LL                                    ;# link layer type
set val(ant)            Antenna/OmniAntenna                   ;# antenna model
set val(ifqlen)         50                                    ;# max packet in ifq
set val(nn)             50				                      ;# number of mobilenodes
set val(rp)             AODV                                  ;# routing protocol
set val(x)              600                                   ;# X dimension of topography
set val(y)              600                                   ;# Y dimension of topography  
set val(stop)			100.0 								  ;# time of simulation end
# =========================================================================================
# Main Program
# =========================================================================================

# Simulator Instance Creation
set ns [new Simulator]

#create the trace file and nam file

set tracefd [open project5.tr w]
$ns trace-all $tracefd

set namtrace [open project5.nam w]
$ns namtrace-all-wireless $namtrace $val(x) $val(y)


# set up topography object
set topo [new Topography]
$topo load_flatgrid $val(x) $val(y)

# general operational descriptor- storing the hop details in the network
create-god $val(nn)

# configure the nodes
$ns node-config -adhocRouting $val(rp) \
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
				-macTrace OFF \
				-movementTrace ON

$val(mac) set CWMin_ 31
$val(mac) set CWMax_ 2047

# Node Creation
for {set i 0} {$i < [expr $val(nn)]} {incr i} {
	set node_($i) [$ns node]
	$node_($i) color black
}

for {set i 0} {$i < [expr $val(nn)]} {incr i} {
	$node_($i) set X_ [expr rand()*$val(x)]
	$node_($i) set Y_ [expr rand()*$val(y)]
	$node_($i) set Z_ 0
}

# Define node initial position in nam
for {set i 0} {$i < [expr $val(nn)]} {incr i} {
	$ns initial_node_pos $node_($i) 30
}

#******************************Defining Communication Between node0 and all nodes ****************************
Application/Traffic/CBR set packetSize_ 512

for {set i 0} {$i < [expr $val(nn)]} {incr i} {
	for {set j [expr $i+1]} {$j < [expr $val(nn)]} {incr j} {
		set udp1($j) [new Agent/UDP]
        $ns attach-agent $node_($j) $udp1($j)
        set cbr1($j) [new Application/Traffic/CBR]
        $cbr1($j) attach-agent $udp1($j)
        set null_($j$i) [new Agent/Null]
        $ns attach-agent $node_($i) $null_($j$i)
        $ns connect $udp1($j) $null_($j$i)	    
	}
}

for {set j 1} {$j < [expr $val(nn)]} {incr j} {
    $ns at 2.0  "$cbr1($j) start"
}

$ns at 100.0 "stop"
$ns at 100.0 "puts \"NS EXITING...\" ; $ns halt"

proc stop {} {
global ns tracefd
$ns flush-trace
close $tracefd
# exec nam project1.nam
exit 0
}
puts "Starting Simulation..."
$ns run