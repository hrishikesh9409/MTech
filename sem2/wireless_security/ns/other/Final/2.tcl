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
set val(number_of_AP)                [lindex $argv 0]           ;# number of AP
set val(number_of_nodes)             [lindex $argv 1]           ;# number of nodes
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
set k [expr $val(number_of_AP) * $val(number_of_nodes) + $val(number_of_AP)]

create-god $k


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

set client $val(number_of_nodes)

for {set i 0} {$i < $k } {incr i} {
	set node_($i) [$ns_ node]	
	$node_($i) random-motion 0		;# disable random motion
}

set a 150
set b 50
set var [lindex $argv 4]
# puts "The second arg is [lindex $argv 1]";
$defaultRNG seed [lindex $argv 4]
set arr_RNG [new RNG]
set arr_ [new RandomVariable/Exponential]
$arr_ set avg_ 9
$arr_ use-rng $arr_RNG
#puts "Inter-arrival time Packet size"
for {set i 0} {$i < $val(number_of_AP)} {incr i} {
    set xx_ [$arr_ value]
    $node_($i) set X_ [expr $a + $xx_]
    $node_($i) set Y_ [expr $b + $xx_ * log($xx_)]
    $node_($i) set Z_ 0.0 
    set a [expr $a + 50]
}
for {set j [expr $val(number_of_AP)]} {$j < $k} {incr j} {
	set xx_ [$arr_ value]
	$node_($j) set X_ [expr 15 * $xx_]
	$node_($j) set Y_ [expr 10 * log($xx_)]
	$node_($j) set Z_ 0.0
}

for {set i 0} {$i < $k} {incr i} {
$ns_ initial_node_pos $node_($i) 30
}

Application/Traffic/CBR set packetSize_ 1023
Application/Traffic/CBR set rate_ 256Kb

set p $val(number_of_AP)
for {set i 0} {$i < $val(number_of_AP)} {incr i} {
    for {set j $p} {$j < $k} {incr j} {
        set udp($j) [new Agent/UDP]
        $ns_ attach-agent $node_($j) $udp($j)
        set cbr($j) [new Application/Traffic/CBR]
        $cbr($j) attach-agent $udp($j)
        set null_($j) [new Agent/Null]
        $ns_ attach-agent $node_($i) $null_($j)
        $ns_ connect $udp($j) $null_($j)
        $ns_ at $j  "$cbr($j) start"
        if {$j > $val(number_of_nodes)} {
            set p $j
            continue
        } 
    }
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