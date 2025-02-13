public class Customer extends Person {

  private Order[] orders = new Order[100];
  private int operator_ID;
  public int orders_size = 0;

  // Getters and Setters
  public Order[] getOrders() {
    return orders;
  }

  public void setOrders(Order[] orders) {
    this.orders = orders;
  }

  public int getOperator_ID() {
    return operator_ID;
  }

  public void setOperator_ID(int operator_ID) {
    this.operator_ID = operator_ID;
  }

  //printing the customer
  public void print_customer() {
    if (this instanceof Retail_customer) {
      System.out.println("Name & Surname : " + getName() + " " + getSurname());
      System.out.println("Address: " + getAddress());
      System.out.println("Phone: " + getPhone());
      System.out.println("ID: " + getID());
      System.out.println("Operator ID: " + getOperator_ID());
      print_orders();
    } else {
      System.out.println("Name & Surname : " + getName() + " " + getSurname());
      System.out.println("Address: " + getAddress());
      System.out.println("Phone: " + getPhone());
      System.out.println("ID: " + getID());
      System.out.println("Operator ID: " + getOperator_ID());
      System.out.println(
        "Company Name: " + ((Corporate_customer) this).getCompany_name()
      );
      print_orders();
    }
  }

  //printing the orders of the customer
  public void print_orders() {
    for (int i = 0; i < orders_size; i++) {
      System.out.print("\nOrder #" + (i + 1) + " => ");
      orders[i].print_order();
    }
    System.out.println("\n");
    System.out.println("----------------------");
  }

  //defining the orders of the customer
  public void define_orders(Order[] orders) {
    for (int i = 0; i < orders.length; i++) {
      if (orders[i].getCustomer_ID() == getID()) {
        this.orders[orders_size] = orders[i];
        this.orders_size++;
      }
    }
  }
}
