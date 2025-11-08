import java.io.BufferedReader;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
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
  private static final Logger LOGGER = Logger.getLogger(Main.class.getName());
  static void readFile(String filePathString) throws IOException {
    Path filePath = Paths.get(filePathString);
    try (BufferedReader bufferedReader =
              Files.newBufferedReader(filePath, StandardCharsets.UTF_8)) {
        String lineText;
        while ((lineText = bufferedReader.readLine()) != null) {
            System.out.println(lineText);
        }
    }
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