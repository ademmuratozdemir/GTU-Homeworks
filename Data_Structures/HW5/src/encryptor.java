import java.util.Map;

public class encryptor {

  private Map<Character, Map<Character, Character>> map;
  private String key;
  private String keystream = "";
  private String plain_test;
  private String cipher_text = "";

  // Constructor
  public encryptor(
    Map<Character, Map<Character, Character>> _map,
    String _key,
    String text
  ) {
    this.map = _map;
    this.key = _key;
    this.plain_test = text.toUpperCase();
  }

  public void encrypt() {
    // do not edit this method
    generate_keystream();
    generate_cipher_text();
  }

  // Generate the keystream based on the plaintext and key
  private void generate_keystream() {
    // Determine the length of the plaintext and the key
    int textLength = plain_test.length();
    int keyLength = key.length();

    // Generate the keystream based on the relationship between plaintext length and key length
    if (textLength < keyLength) {
      // If plaintext length is shorter than key length, repeat key to match plaintext length
      keystream = key.substring(0, textLength);
    } else if (textLength > keyLength) {
      // If plaintext length is longer than key length, repeat key until it matches plaintext length
      while (keystream.length() < textLength) {
        keystream += key;
      }
      // Truncate excess characters to match plaintext length
      keystream = keystream.substring(0, textLength);
    } else {
      // If plaintext length matches key length, use the key itself as the keystream
      keystream = key;
    }
  }

  // Generate the ciphertext based on the plaintext and keystream
  private void generate_cipher_text() {
    // Iterate over each character in the plaintext
    for (int i = 0; i < plain_test.length(); i++) {
      // Retrieve the current plaintext character
      char plain_testChar = plain_test.charAt(i);
      // Retrieve the current keystream character
      char keystreamChar = keystream.charAt(i);
      // Retrieve the cipher character based on the current plaintext and keystream characters
      char cipherChar = map.get(plain_testChar).get(keystreamChar);
      // Append the cipher character to the ciphertext
      cipher_text += cipherChar;
    }
  }

  // Return the keystream
  public String get_keystream() {
    return keystream;
  }

  // Return the ciphertext
  public String get_cipher_text() {
    return cipher_text;
  }
}
