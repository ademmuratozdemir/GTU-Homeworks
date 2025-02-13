import java.io.File;
import java.util.Scanner;

public class Driver {

  public static void main(String[] args) {
    // create an array of orders, retail customers, corporate customers and operators
    Order orders[] = new Order[0];
    Retail_customer[] retail_customers = new Retail_customer[0];
    Corporate_customer[] corporate_customers = new Corporate_customer[0];
    Operator[] operators = new Operator[0];
    // create variables to keep track of the size of the arrays
    int order_size = 0;
    int retail_customer_size = 0;
    int corporate_customer_size = 0;
    int operator_size = 0;

    File file = new File("content.txt");

    try {
      // Create a Scanner object to read from the file
      Scanner scanner = new Scanner(file);

      // Read the file line by line
      while (scanner.hasNextLine()) {
        String line = scanner.nextLine(); // Read the next line
        String[] parsedLine = line.split(";");
        int checkEmptyString = 0;
        //check if line has an empty string
        for (int i = 0; i < parsedLine.length; i++) {
          if (parsedLine[i].equals("")) {
            checkEmptyString = 1;
            break;
          }
        }
        if (checkEmptyString == 1) {
          continue;
        }
        //creating order objects
        if (parsedLine[0].equals("order")) {
          if (parsedLine.length != 6) {
            continue;
          } else {
            Order order = new Order();
            order.setProduct_name(parsedLine[1]);
            order.setCount(Integer.parseInt(parsedLine[2]));
            //check if the count is less than 1
            if (
              order.getCount() <= 1 || order.getCount() >= Integer.MAX_VALUE
            ) {
              continue;
            }
            order.setTotal_price(Integer.parseInt(parsedLine[3]));
            order.setStatus(Integer.parseInt(parsedLine[4]));
            order.setCustomer_ID(Integer.parseInt(parsedLine[5]));
            Order tmporders[] = new Order[order_size + 1];
            orders = append_Array(orders, order_size, order, tmporders);
            order_size++;
          }
          //creating retail_customer objects
        } else if (parsedLine[0].equals("retail_customer")) {
          if (parsedLine.length != 7) {
            continue;
          } else {
            //check if the ID is already used
            int ID_check = checkSameID(
              retail_customers,
              corporate_customers,
              operators,
              Integer.parseInt(parsedLine[5])
            );
            if (ID_check == 1) {
              continue;
            }
            ID_check = 0;
            Retail_customer retail_customer = new Retail_customer();
            retail_customer.setName(parsedLine[1]);
            retail_customer.setSurname(parsedLine[2]);
            retail_customer.setAddress(parsedLine[3]);
            retail_customer.setPhone(parsedLine[4]);
            retail_customer.setID(Integer.parseInt(parsedLine[5]));
            retail_customer.setOperator_ID(Integer.parseInt(parsedLine[6]));
            Retail_customer tmpretail_customers[] = new Retail_customer[retail_customer_size +
            1];
            retail_customers =
              append_Array(
                retail_customers,
                retail_customer_size,
                retail_customer,
                tmpretail_customers
              );
            retail_customer_size++;
          }
          //creating corporate_customer objects
        } else if (parsedLine[0].equals("corporate_customer")) {
          if (parsedLine.length != 8) {
            continue;
          } else {
            int ID_check = checkSameID(
              retail_customers,
              corporate_customers,
              operators,
              Integer.parseInt(parsedLine[5])
            );
            if (ID_check == 1) {
              continue;
            }
            ID_check = 0;
            Corporate_customer corporate_customer = new Corporate_customer();
            corporate_customer.setName(parsedLine[1]);
            corporate_customer.setSurname(parsedLine[2]);
            corporate_customer.setAddress(parsedLine[3]);
            corporate_customer.setPhone(parsedLine[4]);
            corporate_customer.setID(Integer.parseInt(parsedLine[5]));
            corporate_customer.setOperator_ID(Integer.parseInt(parsedLine[6]));
            corporate_customer.setCompany_name(parsedLine[7]);
            Corporate_customer tmpcorporate_customers[] = new Corporate_customer[corporate_customer_size +
            1];
            corporate_customers =
              append_Array(
                corporate_customers,
                corporate_customer_size,
                corporate_customer,
                tmpcorporate_customers
              );
            corporate_customer_size++;
          }
          //creating operator objects
        } else if (parsedLine[0].equals("operator")) {
          if (parsedLine.length != 7) {
            continue;
          } else {
            int ID_check = checkSameID(
              retail_customers,
              corporate_customers,
              operators,
              Integer.parseInt(parsedLine[5])
            );
            if (ID_check == 1) {
              continue;
            }
            ID_check = 0;
            Operator operator = new Operator();
            operator.setName(parsedLine[1]);
            operator.setSurname(parsedLine[2]);
            operator.setAddress(parsedLine[3]);
            operator.setPhone(parsedLine[4]);
            operator.setID(Integer.parseInt(parsedLine[5]));
            operator.setWage(Integer.parseInt(parsedLine[6]));
            Operator tmpoperators[] = new Operator[operator_size + 1];
            operators =
              append_Array(operators, operator_size, operator, tmpoperators);
            operator_size++;
          }
        }
      } // Close the Scanner
      scanner.close();
    } catch (Exception e) {
      System.out.println(
        "An error occurred while reading the file: " + e.getMessage()
      );
      e.printStackTrace();
    }

    //defining orders for retail customers
    for (int i = 0; i < retail_customer_size; i++) {
      retail_customers[i].define_orders(orders);
    }

    //defining orders for corporate customers
    for (int i = 0; i < corporate_customer_size; i++) {
      corporate_customers[i].define_orders(orders);
    }
    //defining customers for operators
    for (Operator operator : operators) {
      operator.define_customers(retail_customers);
      operator.define_customers(corporate_customers);
    }

    //taking user input
    System.out.println("Please enter your ID...");
    Scanner scanner2 = new Scanner(System.in);
    String userInput = scanner2.nextLine();
    // Utility method to check if a string is an integer
    if (isInteger(userInput) == 0) {
      System.out.println("Please enter a valid ID");
      return;
    }
    int ID = Integer.parseInt(userInput);

    int found = 0;

    //finding and printing the operator
    for (Operator operator : operators) {
      if (ID == operator.getID()) {
        System.out.println("***Operator Screen*** ");
        System.out.println("----------------------");
        operator.print_operator();
        operator.print_customers();
        found = 1;
      }
    }
    //finding and printing the retail customer
    for (Retail_customer retail_customer : retail_customers) {
      if (ID == retail_customer.getID()) {
        System.out.println("***Customer Screen*** ");
        System.out.println("----------------------");
        retail_customer.print_customer();
        found = 1;
      }
    }
    //finding and printing the corporate customer
    for (Corporate_customer corporate_customer : corporate_customers) {
      if (ID == corporate_customer.getID()) {
        System.out.println("***Customer Screen*** ");
        System.out.println("----------------------");
        corporate_customer.print_customer();
        found = 1;
      }
    }

    if (found == 0) {
      System.out.println(
        "No operator/customer found with ID " +
        userInput +
        ". Please try again."
      );
    }

    scanner2.close();
  }

  //appending an array
  private static <T> T[] append_Array(
    T[] items,
    int size,
    T item,
    T[] tmpItems
  ) {
    for (int i = 0; i < size; i++) {
      tmpItems[i] = items[i];
    }
    items = tmpItems;
    items[size] = item;
    return items;
  }

  //check if the ID is already used
  private static int checkSameID(
    Retail_customer retail_customers[],
    Corporate_customer corporate_customers[],
    Operator operators[],
    int ID
  ) {
    for (int i = 0; i < retail_customers.length; i++) {
      if (retail_customers[i].getID() == ID) {
        return 1;
      }
    }

    for (Corporate_customer corporate_customer : corporate_customers) {
      if (corporate_customer.getID() == ID) {
        return 1;
      }
    }

    for (Operator operator : operators) {
      if (operator.getID() == ID) {
        return 1;
      }
    }

    return 0;
  }

  //check if a string is an integer
  private static int isInteger(String str) {
    try {
      Integer.parseInt(str);
      return 1;
    } catch (NumberFormatException e) {
      return 0;
    }
  }
}
