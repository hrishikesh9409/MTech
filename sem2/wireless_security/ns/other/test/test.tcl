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
set val(nn)             10                                    ;# number of mobilenodes
set val(rp)             DumbAgent                             ;# routing protocol
set val(x)              600                                   ;# X dimension of topography
set val(y)              600                                   ;# Y dimension of topography  
# =========================================================================================
# Main Program
# =========================================================================================


Mac/802_11 set dataRate_ 11Mb

# Global Variables
# Create an instance of Simulator
set ns_ [new Simulator]

#create the trace file and nam file

set tracefd [open project1.tr w]
$ns_ trace-all $tracefd

set namtrace [open project1.nam w]
$ns_ namtrace-all-wireless $namtrace $val(x) $val(y)

# set up topography object
set topo [new Topography]
$topo load_flatgrid $val(x) $val(y)

# Create God
create-god $val(nn)

# Create channel
set chan_1_ [new $val(chan)]

#configure the nodes

$ns_ node-config -adhocRouting $val(rp) \
                 -llType $val(ll) \
                 -macType $val(mac) \
                 -ifqType $val(ifq) \
                 -ifqLen $val(ifqlen) \
                 -antType $val(ant) \
                 -propType $val(prop) \
                 -phyType $val(netif) \
                 -topoInstance $topo \
                 -agentTrace OFF \
                 -routerTrace OFF \
                 -macTrace ON \
                 -movementTrace ON \
                 -channel $chan_1_

# Node Creation
for {set i 0} {$i < [expr $val(nn)]} {incr i} {
    set node_($i) [$ns_ node]
    $node_($i) random-motion 0;
    set mac_($i) [$node_($i) getMac 0]
    $mac_($i) set RTSThreshold_ 3000
}

# Provide initial location of mobile nodes
$node_(0) set X_ 200.0
$node_(0) set Y_ 360.0
$node_(0) set Z_ 0.0

$node_(1) set X_ 103.196
$node_(1) set Y_ 243.027
$node_(1) set Z_ 0.0

$node_(2) set X_ 131.392
$node_(2) set Y_ 495.291
$node_(2) set Z_ 0.0

$node_(3) set X_ 209.685
$node_(3) set Y_ 208.717
$node_(3) set Z_ 0.0

$node_(4) set X_ 57.6513
$node_(4) set Y_ 415.884
$node_(4) set Z_ 0.0

$node_(5) set X_ 52.4213
$node_(5) set Y_ 320.267
$node_(5) set Z_ 0.0

$node_(6) set X_ 304.964
$node_(6) set Y_ 248.475
$node_(6) set Z_ 0.0

$node_(7) set X_ 351.768
$node_(7) set Y_ 353.56
$node_(7) set Z_ 0.0

$node_(8) set X_ 325.835
$node_(8) set Y_ 448.596
$node_(8) set Z_ 0.0

$node_(9) set X_ 236.392
$node_(9) set Y_ 507.022
$node_(9) set Z_ 0.0


$ns_ at 0.0 "$node_(0) label AP"
$ns_ at 0.0 "$node_(1) label N1"
$ns_ at 0.0 "$node_(2) label N2"
$ns_ at 0.0 "$node_(3) label N3"
$ns_ at 0.0 "$node_(4) label N4"
$ns_ at 0.0 "$node_(5) label N5"
$ns_ at 0.0 "$node_(6) label N6"
$ns_ at 0.0 "$node_(7) label N7"
$ns_ at 0.0 "$node_(8) label N8"
$ns_ at 0.0 "$node_(9) label N9"

$ns_ at 0.0 "$node_(0) add-mark m1 green circle"
$ns_ at 0.0 "$node_(1) add-mark m1 red circle"
$ns_ at 0.0 "$node_(2) add-mark m1 yellow circle"
$ns_ at 0.0 "$node_(3) add-mark m1 blue circle"
$ns_ at 0.0 "$node_(4) add-mark m1 purple circle"
$ns_ at 0.0 "$node_(5) add-mark m1 pink circle"
$ns_ at 0.0 "$node_(6) add-mark m1 red circle"
$ns_ at 0.0 "$node_(7) add-mark m1 plum circle"
$ns_ at 0.0 "$node_(8) add-mark m1 yellow circle"
$ns_ at 0.0 "$node_(9) add-mark m1 grey circle"

# set AP_ADDR1 [$mac_(0) id]
# $mac_(0) ap $AP_ADDR1

# $mac_(0) ScanType ACTIVE

Application/Traffic/CBR set packetSize_ 1023
Application/Traffic/CBR set rate_ 256Kb

for {set i 1} {$i < [expr $val(nn)]} {incr i} {
        set udp1($i) [new Agent/UDP]
        $ns_ attach-agent $node_($i) $udp1($i)
    set cbr1($i) [new Application/Traffic/CBR]
    $cbr1($i) attach-agent $udp1($i)
}


set null0 [new Agent/Null]
$ns_ attach-agent $node_(0) $null0
$ns_ connect $udp1(1) $null0

set null1 [new Agent/Null]
$ns_ attach-agent $node_(0) $null1
$ns_ connect $udp1(2) $null1

set null2 [new Agent/Null]
$ns_ attach-agent $node_(0) $null2
$ns_ connect $udp1(3) $null2

set null3 [new Agent/Null]
$ns_ attach-agent $node_(0) $null3
$ns_ connect $udp1(4) $null3

set null4 [new Agent/Null]
$ns_ attach-agent $node_(0) $null4
    $ns_ connect $udp1(5) $null4

set null5 [new Agent/Null]
$ns_ attach-agent $node_(0) $null5
$ns_ connect $udp1(6) $null5

set null6 [new Agent/Null]
$ns_ attach-agent $node_(0) $null6
$ns_ connect $udp1(7) $null6

set null7 [new Agent/Null]
$ns_ attach-agent $node_(0) $null7
$ns_ connect $udp1(8) $null7

set null8 [new Agent/Null]
$ns_ attach-agent $node_(0) $null8
$ns_ connect $udp1(9) $null8

for {set i 0} {$i < [expr $val(nn)]} {incr i} {
$ns_ initial_node_pos $node_($i) 30
}

$ns_ at 2.0  "$cbr1(1) start"
$ns_ at 3.0  "$cbr1(2) start"
$ns_ at 4.0  "$cbr1(3) start"
$ns_ at 5.0  "$cbr1(4) start"
$ns_ at 6.0  "$cbr1(5) start"
$ns_ at 7.0  "$cbr1(6) start"
$ns_ at 8.0  "$cbr1(7) start"
$ns_ at 9.0  "$cbr1(8) start"
$ns_ at 10.0 "$cbr1(9) start"


$ns_ at 20.0 "puts \"NS EXITING...\" ; $ns_ halt"

proc stop {} {
global ns_ tracefd
$ns_ flush-trace
close $tracefd
exec nam project1.nam
exit 0
}
puts "Starting Simulation..."
$ns_ run