import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import javax.swing.SwingUtilities;

public class Main {

  public static void main(String[] args) {
    if (args.length != 1) {
      System.out.println("Usage: java Main <input_file>");
      return;
    }

    String inputFile = args[0];
    performFirstPartAnalysis(inputFile);
    performSecondPartAnalysis(
      new int[] {
        500,
        1000,
        2000,
        3000,
        4000,
        5000,
        6000,
        7000,
        8000,
        9000,
        10000,
      }
    );
  }

  // Perform first part analysis
  private static void performFirstPartAnalysis(String inputFile) {
    StockDataManager manager = new StockDataManager();
    List<Integer> addTimes = new ArrayList<>();
    List<Integer> searchTimes = new ArrayList<>();
    List<Integer> removeTimes = new ArrayList<>();
    List<Integer> updateTimes = new ArrayList<>();

    try (BufferedReader br = new BufferedReader(new FileReader(inputFile))) {
      String line;
      while ((line = br.readLine()) != null) {
        String[] tokens = line.split(" ");
        String command = tokens[0];
        long startTime, endTime;

        switch (command) {
          case "ADD":
            startTime = System.nanoTime();
            manager.addOrUpdateStock(
              tokens[1],
              Double.parseDouble(tokens[2]),
              Long.parseLong(tokens[3]),
              Long.parseLong(tokens[4])
            );
            endTime = System.nanoTime();
            addTimes.add((int) (endTime - startTime));
            break;
          case "REMOVE":
            startTime = System.nanoTime();
            manager.removeStock(tokens[1]);
            endTime = System.nanoTime();
            removeTimes.add((int) (endTime - startTime));
            break;
          case "SEARCH":
            startTime = System.nanoTime();
            manager.searchStock(tokens[1]);
            endTime = System.nanoTime();
            searchTimes.add((int) (endTime - startTime));
            break;
          case "UPDATE":
            startTime = System.nanoTime();
            manager.addOrUpdateStock(
              tokens[1],
              Double.parseDouble(tokens[3]),
              Long.parseLong(tokens[4]),
              Long.parseLong(tokens[5])
            );
            endTime = System.nanoTime();
            updateTimes.add((int) (endTime - startTime));
            break;
        }
      }
    } catch (IOException e) {
      e.printStackTrace();
    }

    // Print average times
    System.out.println("\nFirst Part Analysis:");
    System.out.println(
      "Average ADD time: " + calculateAverage(addTimes) + " ns"
    );
    System.out.println(
      "Average SEARCH time: " + calculateAverage(searchTimes) + " ns"
    );
    System.out.println(
      "Average REMOVE time: " + calculateAverage(removeTimes) + " ns"
    );
    System.out.println(
      "Average UPDATE time: " + calculateAverage(updateTimes) + " ns"
    );
  }

  // Perform second part analysis
  private static void performSecondPartAnalysis(int[] sizes) {
    List<Integer> sizeList = new ArrayList<>();
    List<Integer> avgAddTimes = new ArrayList<>();
    List<Integer> avgSearchTimes = new ArrayList<>();
    List<Integer> avgRemoveTimes = new ArrayList<>();
    List<Integer> avgUpdateTimes = new ArrayList<>();

    for (int size : sizes) {
      StockDataManager manager = new StockDataManager();

      // Fill the manager with 'size' entries
      for (int i = 0; i < size; i++) {
        manager.addOrUpdateStock(
          "SYM" + i,
          Math.random() * 100,
          (long) (Math.random() * 1000000),
          (long) (Math.random() * 1000000000)
        );
      }

      // Perform 100 operations and measure times
      measureAndPlotTimes(
        manager,
        size,
        sizeList,
        avgAddTimes,
        avgSearchTimes,
        avgRemoveTimes,
        avgUpdateTimes
      );
    }

    // Plot graphs for ADD, SEARCH, REMOVE
    plotGraph(sizeList, avgAddTimes, "ADD");
    plotGraph(sizeList, avgSearchTimes, "SEARCH");
    plotGraph(sizeList, avgRemoveTimes, "REMOVE");
    plotGraph(sizeList, avgUpdateTimes, "UPDATE");
  }

  private static void measureAndPlotTimes(
    StockDataManager manager,
    int size,
    List<Integer> sizeList,
    List<Integer> avgAddTimes,
    List<Integer> avgSearchTimes,
    List<Integer> avgRemoveTimes,
    List<Integer> avgUpdateTimes
  ) {
    List<Integer> addTimes = new ArrayList<>();
    List<Integer> searchTimes = new ArrayList<>();
    List<Integer> removeTimes = new ArrayList<>();
    List<Integer> updateTimes = new ArrayList<>();

    long startTime, endTime;

    // Measure time for ADD operations
    for (int j = 0; j < 100; j++) {
      String symbol = "NEW" + size + "_" + j;
      startTime = System.nanoTime();
      manager.addOrUpdateStock(
        symbol,
        Math.random() * 100,
        (long) (Math.random() * 1000000),
        (long) (Math.random() * 1000000000)
      );
      endTime = System.nanoTime();
      addTimes.add((int) (endTime - startTime));
    }

    // Measure time for SEARCH operations
    for (int j = 0; j < 100; j++) {
      String symbol = "NEW" + size + "_" + j;
      startTime = System.nanoTime();
      manager.searchStock(symbol);
      endTime = System.nanoTime();
      searchTimes.add((int) (endTime - startTime));
    }

    // Measure time for REMOVE operations
    for (int j = 0; j < 100; j++) {
      String symbol = "NEW" + size + "_" + j;
      startTime = System.nanoTime();
      manager.removeStock(symbol);
      endTime = System.nanoTime();
      removeTimes.add((int) (endTime - startTime));
    }

    // Measure time for UPDATE operations
    for (int j = 0; j < 100; j++) {
      String symbol = "NEW" + size + "_" + j;
      startTime = System.nanoTime();
      manager.addOrUpdateStock(
        symbol,
        Math.random() * 100,
        (long) (Math.random() * 1000000),
        (long) (Math.random() * 1000000000)
      );
      endTime = System.nanoTime();
      updateTimes.add((int) (endTime - startTime));
    }

    sizeList.add(size);
    avgAddTimes.add(calculateAverage(addTimes));
    avgSearchTimes.add(calculateAverage(searchTimes));
    avgRemoveTimes.add(calculateAverage(removeTimes));
    avgUpdateTimes.add(calculateAverage(updateTimes));

    // Print average times
    System.out.println("\nSecond Part Analysis for size " + size + ":");
    System.out.println(
      "Average ADD time: " + calculateAverage(addTimes) + " ns"
    );
    System.out.println(
      "Average SEARCH time: " + calculateAverage(searchTimes) + " ns"
    );
    System.out.println(
      "Average REMOVE time: " + calculateAverage(removeTimes) + " ns"
    );
    System.out.println(
      "Average UPDATE time: " + calculateAverage(updateTimes) + " ns"
    );
  }

  private static int calculateAverage(List<Integer> data) {
    if (data.isEmpty()) {
      return 0;
    }
    int sum = 0;
    for (int time : data) {
      sum += time;
    }
    return sum / data.size();
  }

  private static void plotGraph(
    List<Integer> xData,
    List<Integer> yData,
    String operation
  ) {
    SwingUtilities.invokeLater(() -> {
      String plotType = "scatter"; // Change to "scatter" for scatter plot

      GUIVisualization frame = new GUIVisualization(plotType);
      frame.setDataPointsX(xData);
      frame.setDataPointsY(yData);
      frame.setTitle("Average " + operation + " Operation Time (ns)");
      frame.setVisible(true);
    });
  }
}
