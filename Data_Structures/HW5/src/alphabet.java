import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.Map;
import java.util.Set;

public class alphabet {

  private Set<Character> english_alphabet = new LinkedHashSet<>();
  private Map<Character, Map<Character, Character>> map = new HashMap<>();

  public alphabet() {
    // do not edit this method
    fill_english_alphabet();
    fill_map();
  }

  private void fill_english_alphabet() {
    // do not edit this method
    for (char c : "ABCDEFGHIJKLMNOPQRSTUVWXYZ".toCharArray()) {
      english_alphabet.add(c);
    }
  }

  // Fill the map with the Viegenere cipher table
  private void fill_map() {
    for (char rowIndicator : english_alphabet) {
      // Create a new row map for the current letter
      Map<Character, Character> rowMap = new HashMap<>();
      // Start with the current letter
      char currentChar = rowIndicator;
      // Iterate over the English alphabet
      for (char columnIndicator : english_alphabet) {
        // Put the mapping of each letter to the shifted letter in the row map
        rowMap.put(columnIndicator, currentChar);
        // Shift the current letter to the right by one position
        currentChar = (char) ('A' + (currentChar - 'A' + 1) % 26);
      }
      // Put the row map into the main map with the current letter as the key
      map.put(rowIndicator, rowMap);
    }
  }

  public void print_map() {
    // do not edit this method
    System.out.println("*** Viegenere Cipher ***\n\n");
    System.out.println("    " + english_alphabet);
    System.out.print(
      "    ------------------------------------------------------------------------------"
    );
    for (Character k : map.keySet()) {
      System.out.print("\n" + k + " | ");
      System.out.print(map.get(k).values());
    }
    System.out.println("\n");
  }

  // Return the map
  public Map<Character, Map<Character, Character>> get_map() {
    return map;
  }
}
