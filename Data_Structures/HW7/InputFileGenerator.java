import java.io.FileWriter;
import java.io.IOException;
import java.util.Random;
import java.util.Scanner;

public class InputFileGenerator {

  private static final String[] SYMBOLS = {
    "AAPL",
    "GOOGL",
    "MSFT",
    "AMZN",
    "TSLA",
    "FB",
    "NFLX",
    "NVDA",
    "BABA",
    "V",
  };
  private static final Random RANDOM = new Random();
  private static final int TOTAL_LINES = 1000;

  public static void main(String[] args) {
    Scanner scanner = new Scanner(System.in);
    System.out.print("Enter the name of file that will be created: ");
    String filename = scanner.nextLine();
    generateInputFile(TOTAL_LINES, filename);
  }

  private static void generateInputFile(int totalLines, String filename) {
    try (FileWriter writer = new FileWriter(filename)) {
      for (int i = 0; i < totalLines; i++) {
        String command = getRandomCommand();
        writer.write(command + "\n");
      }
      System.out.println("Input file generated successfully: " + filename);
    } catch (IOException e) {
      System.err.println("Error writing to file: " + e.getMessage());
    }
  }

  private static String getRandomCommand() {
    int commandType = RANDOM.nextInt(4); // 0: ADD, 1: REMOVE, 2: SEARCH, 3: UPDATE
    String symbol = getRandomSymbol();
    switch (commandType) {
      case 0: // ADD
        double price = getRandomPrice();
        long volume = getRandomVolume();
        long marketCap = getRandomMarketCap();
        return String.format(
          "ADD %s %.2f %d %d",
          symbol,
          price,
          volume,
          marketCap
        );
      case 1: // REMOVE
        return String.format("REMOVE %s", symbol);
      case 2: // SEARCH
        return String.format("SEARCH %s", symbol);
      case 3: // UPDATE
        String newSymbol = getRandomSymbol();
        double newPrice = getRandomPrice();
        long newVolume = getRandomVolume();
        long newMarketCap = getRandomMarketCap();
        return String.format(
          "UPDATE %s %s %.2f %d %d",
          symbol,
          newSymbol,
          newPrice,
          newVolume,
          newMarketCap
        );
      default:
        throw new IllegalStateException("Unexpected value: " + commandType);
    }
  }

  private static String getRandomSymbol() {
    return SYMBOLS[RANDOM.nextInt(SYMBOLS.length)];
  }

  private static double getRandomPrice() {
    return 10 + (1000 - 10) * RANDOM.nextDouble();
  }

  private static long getRandomVolume() {
    return 100000 + RANDOM.nextInt(900000);
  }

  private static long getRandomMarketCap() {
    return 500000000 + RANDOM.nextInt(500000000);
  }
}
