BEGIN {
        recvdSize = 0
        throughPut = 0
}

{
        event = $1
        time = $2
        node_id = $3
        pkt_size = $8
        level = $4

        if (level == "AGT" && event == "r" ) {
                if (time > stopTime) {
                        stopTime = time
                }
                recvdSize = recvdSize + (pkt_size * 8)
                throughPut = recvdSize / 25;
                printf(" %.2f\t%.2f\n",time, throughPut);
        }
}
END {

}
