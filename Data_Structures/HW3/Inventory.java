import java.io.File;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Scanner;

public class Inventory {

  // data field
  private LinkedList<ArrayList<Device>> inventoryList = new LinkedList<>();

  // Getter
  public LinkedList<ArrayList<Device>> getInventoryList() {
    return inventoryList;
  }

  // Setter
  public void setInventoryList(LinkedList<ArrayList<Device>> inventoryList) {
    this.inventoryList = inventoryList;
  }

  // a function to add a new device. Time complexity of this function is O(1)
  public void addDevice() {
    Scanner scanner = new Scanner(System.in);
    System.out.println("Enter the device category:");
    System.out.println(
      "Select one of them :  Computer, SmartPhone, TV, VacuumCleaner, Headphones"
    );
    String category = scanner.nextLine();
    System.out.println("Enter the device name:");
    String name = scanner.nextLine();
    System.out.println("Enter the device price:");
    double price = scanner.nextDouble();
    System.out.println("Enter the device quantity:");
    int quantity = scanner.nextInt();
    if (category.equals("Computer")) {
      this.getInventoryList().get(0).add(new Computer(name, price, quantity));
      System.out.println(
        "Computer , " +
        name +
        " , " +
        price +
        "$ , " +
        quantity +
        " amount added successfully\n"
      );
    } else if (category.equals("SmartPhone")) {
      this.getInventoryList().get(1).add(new SmartPhone(name, price, quantity));
      System.out.println(
        "Smartphone , " +
        name +
        " , " +
        price +
        "$ , " +
        quantity +
        " amount added successfully\n"
      );
    } else if (category.equals("TV")) {
      this.getInventoryList().get(2).add(new TV(name, price, quantity));
      System.out.println(
        "TV , " +
        name +
        " , " +
        price +
        "$ , " +
        quantity +
        " amount added successfully"
      );
    } else if (category.equals("VacuumCleaner")) {
      this.getInventoryList()
        .get(3)
        .add(new VacuumCleaner(name, price, quantity));
      System.out.println(
        "VacuumCleaner , " +
        name +
        " , " +
        price +
        "$ , " +
        quantity +
        " amount added successfully\n"
      );
    } else if (category.equals("Headphones")) {
      this.getInventoryList().get(4).add(new Headphones(name, price, quantity));
      System.out.println(
        "Headphones , " +
        name +
        " , " +
        price +
        "$ , " +
        quantity +
        " amount added successfully\n"
      );
    } else {
      System.out.println("Invalid category\n");
    }
  }

  // a function to remove a device. Time complexity of this function is O(n)
  public void removeDevice() {
    Scanner scanner = new Scanner(System.in);
    System.out.println("Enter the device category:");
    String category = scanner.nextLine();
    System.out.println("Enter the device name:");
    String name = scanner.nextLine();
    if (category.equals("Computer")) {
      for (int i = 0; i < this.getInventoryList().get(0).size(); i++) {
        if (this.getInventoryList().get(0).get(i).getName().equals(name)) {
          this.getInventoryList().get(0).remove(i);
          System.out.println("Device removed successfully\n");
          break; // Add break statement to exit the loop after removing the device
        }
      }
    } else if (category.equals("SmartPhone")) {
      for (int i = 0; i < this.getInventoryList().get(1).size(); i++) {
        if (this.getInventoryList().get(1).get(i).getName().equals(name)) {
          this.getInventoryList().get(1).remove(i);
          System.out.println("Device removed successfully\n");
        }
      }
    } else if (category.equals("TV")) {
      for (int i = 0; i < this.getInventoryList().get(2).size(); i++) {
        if (this.getInventoryList().get(2).get(i).getName().equals(name)) {
          this.getInventoryList().get(2).remove(i);
          System.out.println("Device removed successfully\n");
        }
      }
    } else if (category.equals("VacuumCleaner")) {
      for (int i = 0; i < this.getInventoryList().get(3).size(); i++) {
        if (this.getInventoryList().get(3).get(i).getName().equals(name)) {
          this.getInventoryList().get(3).remove(i);
          System.out.println("Device removed successfully\n");
        }
      }
    } else if (category.equals("Headphones")) {
      for (int i = 0; i < this.getInventoryList().get(4).size(); i++) {
        if (this.getInventoryList().get(4).get(i).getName().equals(name)) {
          this.getInventoryList().get(4).remove(i);
          System.out.println("Device removed successfully\n");
        }
      }
    } else {
      System.out.println("Invalid category");
    }
  }

  // a function to update device details. Time complexity of this function is O(n*m) where n is the number of categories and m is the number of devices in each category
  public void updateDeviceDetails() {
    int found = 0;
    Scanner scanner = new Scanner(System.in);
    System.out.println("Enter the device name:");
    String name = scanner.nextLine();
    for (int i = 0; i < this.getInventoryList().size(); i++) {
      for (int j = 0; j < this.getInventoryList().get(i).size(); j++) {
        if (this.getInventoryList().get(i).get(j).getName().equals(name)) {
          System.out.println("Enter a new name");
          String newName = scanner.nextLine();
          this.getInventoryList().get(i).get(j).setName(newName);
          System.out.println("Enter the new device price:");
          double newPrice = scanner.nextDouble();
          this.getInventoryList().get(i).get(j).setPrice(newPrice);
          System.out.println("Enter the new device quantity:");
          int newQuantity = scanner.nextInt();
          this.getInventoryList().get(i).get(j).setQuantity(newQuantity);
          System.out.println(
            newName +
            " details updated: Price -  " +
            newPrice +
            "$ , Quantity -  " +
            newQuantity +
            " updated successfully\n"
          );
          found = 1;
          break;
        }
      }
    }
    if (found == 0) {
      System.out.println("Device not found\n");
    }
  }

  // a function to display all devices. Time complexity of this function is O(n*m) where n is the number of categories and m is the number of devices in each category
  public void displayAllDevices() {
    int counter = 1;
    System.out.println("\nDevice List:");
    for (int i = 0; i < this.getInventoryList().size(); i++) {
      for (int j = 0; j < this.getInventoryList().get(i).size(); j++) {
        System.out.println(
          counter +
          ". " +
          "Category: " +
          this.getInventoryList().get(i).get(j).getCategory() +
          " , " +
          "Name: " +
          this.getInventoryList().get(i).get(j).getName() +
          " , " +
          "Price: " +
          this.getInventoryList().get(i).get(j).getPrice() +
          "$ , " +
          "Quantity: " +
          this.getInventoryList().get(i).get(j).getQuantity()
        );
        counter++;
      }
    }
  }

  // a function to find the cheapest device . Time complexity of this function is O(n*m) where n is the number of categories and m is the number of devices in each category
  public void findCheapestDevice() {
    double minPrice = Double.MAX_VALUE;
    Device cheapestDevice = null;
    for (int i = 0; i < this.getInventoryList().size(); i++) {
      for (int j = 0; j < this.getInventoryList().get(i).size(); j++) {
        if (this.getInventoryList().get(i).get(j).getPrice() < minPrice) {
          minPrice = this.getInventoryList().get(i).get(j).getPrice();
          cheapestDevice = this.getInventoryList().get(i).get(j);
        }
      }
    }
    System.out.println(
      "Cheapest device is: " +
      "Category: " +
      cheapestDevice.getCategory() +
      " , " +
      "Name: " +
      cheapestDevice.getName() +
      " , " +
      "Price: " +
      cheapestDevice.getPrice() +
      "$ , " +
      "Quantity: " +
      cheapestDevice.getQuantity()
    );
  }

  // a function to sort devices by price . Time complexity of this function is O(n logn) where n is the total number of devices in the inventory list.
  public void sortingDevicesByPrice() {
    LinkedList<Device> templist = new LinkedList<>();
    System.out.println("\nDevices sorted by price:");
    for (int i = 0; i < this.getInventoryList().size(); i++) {
      for (int j = 0; j < this.getInventoryList().get(i).size(); j++) {
        templist.add(this.getInventoryList().get(i).get(j));
      }
    }

    templist.sort((Device d1, Device d2) -> {
      if (d1.getPrice() > d2.getPrice()) {
        return 1;
      } else if (d1.getPrice() < d2.getPrice()) {
        return -1;
      } else {
        return 0;
      }
    });

    for (int i = 0; i < templist.size(); i++) {
      System.out.println(
        (i + 1) +
        ". " +
        "Category: " +
        templist.get(i).getCategory() +
        " , " +
        "Name: " +
        templist.get(i).getName() +
        " , " +
        "Price: " +
        templist.get(i).getPrice() +
        "$ , " +
        "Quantity: " +
        templist.get(i).getQuantity()
      );
    }
  }

  // a function to calculate total inventory value . Time complexity of this function is O(n*m) where n is the number of categories and m is the number of devices in each category
  public void calculateTotalValue() {
    double totalValue = 0;
    for (int i = 0; i < this.getInventoryList().size(); i++) {
      for (int j = 0; j < this.getInventoryList().get(i).size(); j++) {
        totalValue +=
          this.getInventoryList().get(i).get(j).getPrice() *
          this.getInventoryList().get(i).get(j).getQuantity();
      }
    }
    System.out.println("\nTotal inventory value: " + totalValue + "$");
  }

  // a function to restock a device . Time complexity of this function is O(n*m) where n is the number of categories and m is the number of devices in each category
  public void restockingDevices() {
    Scanner scanner = new Scanner(System.in);
    System.out.println("Enter the device name:");
    String name = scanner.nextLine();
    int found = 0;
    for (int i = 0; i < this.getInventoryList().size(); i++) {
      for (int j = 0; j < this.getInventoryList().get(i).size(); j++) {
        if (this.getInventoryList().get(i).get(j).getName().equals(name)) {
          System.out.println(
            "Do you want to add or remove stock? (Add/Remove): "
          );
          String userChoice = scanner.nextLine();
          if (userChoice.equals("Add")) {
            System.out.println("Enter the quantity to add: ");
            int quantity = scanner.nextInt();
            this.getInventoryList()
              .get(i)
              .get(j)
              .setQuantity(
                this.getInventoryList().get(i).get(j).getQuantity() + quantity
              );
            System.out.println(
              this.getInventoryList().get(i).get(j).getName() +
              " restocked.New quantity is: " +
              this.getInventoryList().get(i).get(j).getQuantity()
            );
          } else if (userChoice.equals("Remove")) {
            System.out.println("Enter the quantity to remove: ");
            int quantity = scanner.nextInt();
            this.getInventoryList()
              .get(i)
              .get(j)
              .setQuantity(
                this.getInventoryList().get(i).get(j).getQuantity() - quantity
              );
            System.out.println(
              this.getInventoryList().get(i).get(j).getName() +
              " restocked.New quantity is: " +
              this.getInventoryList().get(i).get(j).getQuantity()
            );
          } else {
            System.out.println("Invalid choice");
          }
          found = 1;
          break;
        }
      }
    }
    if (found == 0) {
      System.out.println("Device not found");
    }
  }

  // a function to export inventory report . Time complexity of this function is O(n*m) where n is the number of categories and m is the number of devices in each category
  public void exportingInventoryReport() {
    String report = "Electronics Shop Inventory Report\n";
    report += "Generated on: 26th March 2024\n\n";
    report += "Device List:\n";
    int counter = 1;
    for (int i = 0; i < this.getInventoryList().size(); i++) {
      for (int j = 0; j < this.getInventoryList().get(i).size(); j++) {
        report +=
          counter +
          ". " +
          "Category: " +
          this.getInventoryList().get(i).get(j).getCategory() +
          " , " +
          "Name: " +
          this.getInventoryList().get(i).get(j).getName() +
          " , " +
          "Price: " +
          this.getInventoryList().get(i).get(j).getPrice() +
          "$ , " +
          "Quantity: " +
          this.getInventoryList().get(i).get(j).getQuantity() +
          "\n";
        counter++;
      }
    }
    File file = new File("InventoryReport.txt");
    try {
      file.createNewFile();
      java.io.PrintWriter output = new java.io.PrintWriter(file);
      output.println(report);
      output.println("\nSummary: ");
      output.println("Total number of devices: " + totalNumberofDevices());
      double totalValue = 0;
      for (int i = 0; i < this.getInventoryList().size(); i++) {
        for (int j = 0; j < this.getInventoryList().get(i).size(); j++) {
          totalValue +=
            this.getInventoryList().get(i).get(j).getPrice() *
            this.getInventoryList().get(i).get(j).getQuantity();
        }
      }
      output.println("Total inventory value: " + totalValue + "$");
      output.println("\nEnd of report");
      output.close();
      System.out.println("Inventory report exported successfully\n");
    } catch (java.io.IOException e) {
      System.out.println("An error occurred");
      e.printStackTrace();
    }
  }

  //helper function to calculate total number of devices
  public int totalNumberofDevices() {
    int totalDevices = 0;
    for (int i = 0; i < this.getInventoryList().size(); i++) {
      totalDevices += this.getInventoryList().get(i).size();
    }
    return totalDevices;
  }
}
