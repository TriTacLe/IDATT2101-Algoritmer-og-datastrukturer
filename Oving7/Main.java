import java.io.BufferedReader;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

enum InteressePunkter{
  STEDSNAVN,
  BENSINSTASJON,
  LADESTASJON,
  SPISESTED,
  DRIKKESTED,
  OVERNATTINGSSTED
}

public class Main{
  public static final class Node{
    public final int nodenr;
    public final double latitude;
    public final double longitude;
    public Node(int nodenr, double latitute, double longitude){
      this.nodenr = nodenr;
      this.latitude = latitute;
      this.longitude=longitude;
    }
  }

  public static final class Edge{
    public final int toNodeNr;
    public final int fromNodeNr;
    public final int travelTime; // Kjøretid
    // public final int length;
    // public final int speedLimit;
    public Edge(int toNodeNr, int fromNodeNr, int travelTime){
      this.toNodeNr = toNodeNr;
      this.fromNodeNr = fromNodeNr;
      this.travelTime = travelTime;
    }
  }

  public static final class Graph{
    
  }

  public static final class InteressePunkt{
    public final int nodenr;
    public final int kode;
    public final String stedsnavn;
    
    public InteressePunkt(int nodenr, int kode, String stedsnavn){
      this.nodenr = nodenr;
      this.kode = kode;
      this.stedsnavn = stedsnavn;
    }
  }
  
  private static final Logger LOGGER = Logger.getLogger(Main.class.getName());
  static void readFile(String filePathString) throws IOException {
    Path filePath = Paths.get(filePathString);
    try (BufferedReader bufferedReader =
              Files.newBufferedReader(filePath, StandardCharsets.UTF_8)) {
        
    }
  }

  static int[] dijktra(int totalNodes, List<Edge> edges, Node srcNode){
    return 
  }

  private static InteressePunkter getInteressepunkter(int code){
    if((code &  1)== 1){
      return InteressePunkter.STEDSNAVN;
    }
    else if((code & 2) == 2){
      return InteressePunkter.BENSINSTASJON;
    }
    else if((code & 4) == 4){
      return InteressePunkter.LADESTASJON;
    }
    else if((code & 8) == 8){
      return InteressePunkter.SPISESTED;
    }
    else if((code & 16) == 16){
      return InteressePunkter.DRIKKESTED;
    }
    else{
      return InteressePunkter.OVERNATTINGSSTED;
    }
  }

  public static void main(String[] args){
    String noder = "noder.txt";
    try {
      readFile(noder);  
    } catch (IOException ioException) {
      LOGGER.log(Level.SEVERE, "Failed to read file: {0}", ioException);
    }
  }
}