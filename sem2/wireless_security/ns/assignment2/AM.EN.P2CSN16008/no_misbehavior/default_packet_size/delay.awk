BEGIN {
        highest_packet_id = 0;
        counter = 150;
}
{
        action = $1;
        time = $2;
        from = $3;
        to = $4;
        type = $5;
        pktsize = $6;
        src = $9;
        dst = $10;
        packet_id = $18;


        if( $7 == "cbr" ){

                sub(/\[/, "", packet_id)
                sub(/\]/, "", packet_id)
                packet_id = int(packet_id)

                if ( packet_id > highest_packet_id )
                        highest_packet_id = packet_id;

                if ( start_time[packet_id] == 0 ){
                        start_time[packet_id] = time;
                }

                if ( action != "d" ) {
                        if ( action == "r" ) {
                                end_time[packet_id] = time;
                        }
                }

                delay = end_time[packet_id] - start_time[packet_id]
                if(delay > 0){
                        if(counter == 0){
                                printf("%f\t%f\n",time, delay)
                                counter = 150;
                        }
                        counter = counter -1
                }
        }

}


END {
}
