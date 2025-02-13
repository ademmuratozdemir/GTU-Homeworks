import java.util.ArrayList;
import java.util.Scanner;

public class Driver {

  public static void main(String[] args) {
    Inventory inventory = new Inventory();
    ArrayList<Device> computerList = new ArrayList<>();
    ArrayList<Device> smartPhoneList = new ArrayList<>();
    ArrayList<Device> tvList = new ArrayList<>();
    ArrayList<Device> vacuumCleanerList = new ArrayList<>();
    ArrayList<Device> headphonesList = new ArrayList<>();
    inventory.getInventoryList().add(computerList);
    inventory.getInventoryList().add(smartPhoneList);
    inventory.getInventoryList().add(tvList);
    inventory.getInventoryList().add(vacuumCleanerList);
    inventory.getInventoryList().add(headphonesList);

    int userChoice;

    System.out.println(
      "Welcome to the Electronics Inventory Management System!"
    );

    Scanner scanner = new Scanner(System.in);

    while (true) {
      System.out.println(
        "\nPlease select an option:\n" + //
        "1. Add a new device\n" + //
        "2. Remove a device\n" + //
        "3. Update device details\n" + //
        "4. List all devices\n" + //
        "5. Find the cheapest device\n" + //
        "6. Sort devices by price\n" + //
        "7. Calculate total inventory value\n" + //
        "8. Restock a device\n" + //
        "9. Export inventory report\n" + //
        "0. Exit\n"
      );

      userChoice = scanner.nextInt();

      switch (userChoice) {
        case 1:
          inventory.addDevice();
          break;
        case 2:
          inventory.removeDevice();
          break;
        case 3:
          inventory.updateDeviceDetails();
          break;
        case 4:
          inventory.displayAllDevices();
          break;
        case 5:
          inventory.findCheapestDevice();
          break;
        case 6:
          inventory.sortingDevicesByPrice();
          break;
        case 7:
          inventory.calculateTotalValue();
          break;
        case 8:
          inventory.restockingDevices();
          break;
        case 9:
          inventory.exportingInventoryReport();
          break;
        case 0:
          System.out.println("Exiting the program...");
          scanner.close();
          System.exit(0);
          break;
        default:
          System.out.println("Invalid input. Please try again.");
          break;
      }
    }
  }
}
