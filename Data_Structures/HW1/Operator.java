public class Operator extends Person {

  private int wage;
  private Customer[] customers = new Customer[100];
  public int customers_size = 0;

  // Getters and Setters
  public Customer[] getCustomers() {
    return customers;
  }

  public void setCustomers(Customer[] customers) {
    this.customers = customers;
  }

  public int getWage() {
    return wage;
  }

  public void setWage(int wage) {
    this.wage = wage;
  }

  //printing the operator
  public void print_operator() {
    System.out.println("Name & Surname : " + getName() + " " + getSurname());
    System.out.println("Address: " + getAddress());
    System.out.println("Phone: " + getPhone());
    System.out.println("ID: " + getID());
    System.out.println("Wage: " + getWage());
    System.out.println("----------------------");
  }

  //printing the customers of the operator
  public void print_customers() {
    if (customers_size == 0) {
      System.out.println("This operator doesn't have any customer.");
    }
    for (int i = 0; i < customers_size; i++) {
      System.out.print("Customer #" + (i + 1));
      if (customers[i] instanceof Retail_customer) {
        System.out.println(" (a retail customer)");
      } else {
        System.out.println(" (a corporate customer)");
      }
      customers[i].print_customer();
    }
  }

  //defining the customers of the operator
  public void define_customers(Customer[] customers) {
    for (int i = 0; i < customers.length; i++) {
      if (getID() == customers[i].getOperator_ID()) {
        this.customers[customers_size] = customers[i];
        this.customers_size++;
      }
    }
  }
}
