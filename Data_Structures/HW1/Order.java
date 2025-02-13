public class Order {

  private String product_name;
  private int count;
  private int total_price;
  private int status;
  private int customer_ID;

  // Getters and Setters
  public int getTotal_price() {
    return total_price;
  }

  public void setTotal_price(int total_price) {
    this.total_price = total_price;
  }

  public String getProduct_name() {
    return product_name;
  }

  public void setProduct_name(String product_name) {
    this.product_name = product_name;
  }

  public int getCount() {
    return count;
  }

  public void setCount(int count) {
    this.count = count;
  }

  public int getStatus() {
    return status;
  }

  public void setStatus(int status) {
    this.status = status;
  }

  public int getCustomer_ID() {
    return customer_ID;
  }

  public void setCustomer_ID(int customer_ID) {
    this.customer_ID = customer_ID;
  }

  //printing the order
  public void print_order() {
    System.out.print("Product Name: " + getProduct_name());
    System.out.print(" - Count: " + getCount());
    System.out.print(" - Total Price: " + getTotal_price());
    System.out.print(" - Status: " + statusPrinter(getStatus()));
  }

  // a status printer method for printing the status of the order
  public String statusPrinter(int status) {
    if (status == 0) {
      return "Initialized";
    } else if (status == 1) {
      return "Processing";
    } else if (status == 2) { // Add condition here
      return "Completed";
    } else if (status == 3) {
      return "Cancelled";
    }

    // Add a default return statement
    return "";
  }
}
