import java.util.Iterator;
import java.util.Map;

public class decryptor {

  private Map<Character, Map<Character, Character>> map;
  private String key;
  private String keystream = "";
  private String plain_text = "";
  private String cipher_text;

  // Constructor
  public decryptor(
    Map<Character, Map<Character, Character>> _map,
    String _key,
    String text
  ) {
    this.map = _map;
    this.key = _key;
    this.cipher_text = text.toUpperCase();
  }

  public void decrypt() {
    // do not edit this method
    generate_keystream();
    generate_plain_text();
  }

  // Generate the keystream
  private void generate_keystream() {
    // Determine the length of the ciphertext and the key
    int textLength = cipher_text.length();
    int keyLength = key.length();

    // Generate the keystream based on the relationship between ciphertext length and key length
    if (textLength < keyLength) {
      // If ciphertext length is shorter than key length, truncate key to match ciphertext length
      keystream = key.substring(0, textLength);
    } else if (textLength > keyLength) {
      // If ciphertext length is longer than key length, repeat key until it matches ciphertext length
      while (keystream.length() < textLength) {
        keystream += key;
      }
      // Truncate excess characters to match ciphertext length
      keystream = keystream.substring(0, textLength);
    } else {
      // If ciphertext length matches key length, use the key itself as the keystream
      keystream = key;
    }
  }

  // Generate the plaintext
  private void generate_plain_text() {
    // Create iterators for the keystream and ciphertext
    Iterator<Character> keystreamIterator = keystream
      .chars()
      .mapToObj(c -> (char) c)
      .iterator();
    Iterator<Character> cipher_textIterator = cipher_text
      .chars()
      .mapToObj(c -> (char) c)
      .iterator();

    // Iterate over each character in the keystream and ciphertext
    while (keystreamIterator.hasNext() && cipher_textIterator.hasNext()) {
      // Retrieve the current keystream and ciphertext characters
      char keystreamChar = keystreamIterator.next();
      char cipher_textChar = cipher_textIterator.next();

      // Iterate over each row in the map
      Iterator<Character> rowIterator = map.keySet().iterator();
      while (rowIterator.hasNext()) {
        char rowIndicator = rowIterator.next();
        Map<Character, Character> rowMap = map.get(rowIndicator);
        // Check if the current row contains the ciphertext character
        if (rowMap.containsValue(cipher_textChar)) {
          // Iterate over each column in the row
          Iterator<Character> columnIterator = rowMap.keySet().iterator();
          while (columnIterator.hasNext()) {
            char columnIndicator = columnIterator.next();
            // Find the column indicator corresponding to the ciphertext character
            if (rowMap.get(columnIndicator) == cipher_textChar) {
              // Apply the inverse Caesar cipher transformation to find the original character
              char plainChar = columnIndicator;
              char keyChar = keystreamChar;
              char originalChar = map
                .get('A')
                .get((char) (((plainChar - keyChar + 26) % 26) + 'A'));
              // Append the original character to the plaintext
              plain_text += originalChar;
              // Exit the inner loop
              break;
            }
          }
          // Exit the outer loop
          break;
        }
      }
    }
  }

  // Return the keystream
  public String get_keystream() {
    return keystream;
  }

  // Return the plaintext
  public String get_plain_text() {
    return plain_text;
  }
}
