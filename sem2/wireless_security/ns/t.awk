BEGIN {
recv=0;
gotime = 1;
time = 0;
time_interval=5;
}
#body
{
       event = $1
       time = $2
       node_id = $3
       level = $4
       pktType = $7
       pkt_size = $8

 if(time>gotime) {

  print gotime, (pkt_size * recv * 8.0)/1000; 
  gotime+= time_interval;
  recv=0;
  }

#============= CALCULATE throughput=================

if (( event == "r") && ( pktType == "cbr" ) && ( level=="AGT" ))
{
 recv++;
}

} #body


END {
;
}