public class preprocessor {

  private String initial_string;
  private String preprocessed_string;

  // Constructor
  public preprocessor(String str) {
    this.initial_string = str;
    this.preprocessed_string = "";
  }

  public void preprocess() {
    // do not edit this method
    capitalize();
    clean();
  }

  // Capitalize the string
  private void capitalize() {
    initial_string = initial_string.replace("ı", "").toUpperCase();
  }

  // Clean the string
  private void clean() {
    // Iterate over each character in the initial string
    for (char c : initial_string.toCharArray()) {
      // Check if the character is an uppercase English alphabet letter
      if ((c >= 'A' && c <= 'Z')) {
        // Append the uppercase letter to the preprocessed string
        preprocessed_string += c;
      }
    }

    // Check if the preprocessed string is empty
    if (preprocessed_string.equals("")) {
      // Print an error message
      System.out.println("Given input is not proper. Please try again.");
      // Exit the program
      System.exit(0);
    }
  }

  // Return the preprocessed string
  public String get_preprocessed_string() {
    return preprocessed_string;
  }
}
