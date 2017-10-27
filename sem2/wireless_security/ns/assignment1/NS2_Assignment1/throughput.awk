BEGIN {
recv=0;
GO_time = 1;
time = 0;
pktSize = 50;
timeIntv=2;
finalValue = 0;
}

{
        event = $1
        time = $2
        node_id = $3
        level = $4
        pktType = $7

        if(time>GO_time) {
                finalValue+= (pktSize * recv * 8.0)/1000; #main algorithm
                GO_time+= timeIntv;
                recv=0;
        }

        if (( event == "r") && ( pktType == "cbr" ) && ( level=="AGT" ))
        {
                recv++;
        }

}


END {
        print (finalValue / GO_time) + 10
}
