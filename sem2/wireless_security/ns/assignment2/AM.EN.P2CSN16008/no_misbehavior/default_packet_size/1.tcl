#expects seed and packet size

# ======================================================================
# Define options
# ======================================================================
set val(chan)           Channel/WirelessChannel    ;#Channel Type
set val(prop)           Propagation/TwoRayGround   ;# radio-propagation model
set val(netif)          Phy/WirelessPhy            ;# network interface type
set val(mac)            Mac/802_11                 ;# MAC type
set val(ifq)            Queue/DropTail/PriQueue             ;# interface queue type
set val(ll)             LL                         ;# link layer type
set val(ant)            Antenna/OmniAntenna        ;# antenna model
set val(ifqlen)         50                        ;# max packet in ifq
set val(nn)             50          ;# number of mobilenodes
set val(seed)           [lindex $argv 0]          ;#seed value for position
set val(rp)             AODV               ;# routing protocol
set val(x)              500
set val(y)              500

#  Global Variables
set ns_         [new Simulator]
set tracefd     [open 1.tr w]
$ns_ trace-all $tracefd

set namtrace [open 1.nam w]
$ns_ namtrace-all-wireless $namtrace $val(x) $val(y)

# set up topography object
set topo       [new Topography]
$topo load_flatgrid $val(x) $val(y)

# Create God
create-god $val(nn)

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
                -macTrace OFF \
                -movementTrace OFF \
                -channel $chan_1_


for {set i 1} {$i <= 2} {incr i} {
        for {set j 1 } {$j <= 25} {incr j} {
                set node_($i$j) [$ns_ node]
                #$node_($i$j) random-motion 0
                #set postion
                #take seed from command line -- argv
                $node_($i$j) set X_ [expr int ( rand() * $val(seed) ) % 500 ]
                $node_($i$j) set Y_ [expr int ( rand() * $val(seed) ) % 500 ]
                $node_($i$j) set Z_ 0.0
        }
}

Application/Traffic/CBR set rate_ 256Kb

set k 1
set l 2

for {set i 1 } {$i <= 25} {incr i} {
        set udp($i) [new Agent/UDP]
        $ns_ attach-agent $node_($k$i) $udp($i)
        set cbr($i) [new Application/Traffic/CBR]
        $cbr($i) attach-agent $udp($i)

        set null($i) [new Agent/Null]
        $ns_ attach-agent $node_($l$i) $null($i)

        $ns_ connect $udp($i) $null($i)

        $ns_ at 2.0 "$cbr($i) start"
}

for {set i 1} {$i <= 2} {incr i} {
        for {set j 1} {$j <= 25} {incr j} {
                $ns_ initial_node_pos $node_($i$j) 30
        }
}


$ns_ at 20.0 "stop"
$ns_ at 20.0 "puts \"NS EXITING...\" ; $ns_ halt"

proc stop {} {
    global ns_ tracefd
    $ns_ flush-trace
    close $tracefd
    exec nam a.nam
    exit 0
}

puts "Starting Simulation..."
$ns_ run
