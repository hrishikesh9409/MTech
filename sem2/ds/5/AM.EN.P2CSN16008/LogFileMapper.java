import java.io.IOException;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;
import java.util.StringTokenizer;  
import java.util.*;

/**
 * Example input line:
 * 96.7.4.14 - - [24/Apr/2011:04:20:11 -0400] "GET /cat.jpg HTTP/1.1" 200 12433
 *
 */
public class LogFileMapper extends Mapper<LongWritable, Text, Text, IntWritable> {

  private final Text mapKey = new Text();

  @Override
  public void map(LongWritable key, Text value, Context context)
      throws IOException, InterruptedException {

    /*
     * Split the input line into space-delimited fields.
     */
            final String line = value.toString();
            StringTokenizer st = new StringTokenizer(line," "); 
            String[] tok = new String[10]; 
            int i = 0;
            final String[] data = line.trim().split("- -");
            if(st.countTokens() == 10){
              while(st.hasMoreTokens()){
                tok[i] = st.nextToken();
                i++;
            }
            int n = Integer.parseInt(tok[8]);
          
                if (n == 200 && data.length > 1) 
                {
                  final String ipAddress = data[0];
                  mapKey.set(ipAddress);
                  context.write(new Text(mapKey), new IntWritable(1));
                }
          }
    }
}