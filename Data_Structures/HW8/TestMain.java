import java.util.*;

public class TestMain {

  public static void main(String[] args) {
    SocialNetworkGraph network = new SocialNetworkGraph();
    network.addPerson("Alice", 25, Arrays.asList("Reading", "Hiking"));
    network.addPerson("Bob", 30, Arrays.asList("Gaming", "TraVeling"));
    network.addPerson("Charlie", 35, Arrays.asList("Cooking", "Painting"));
    network.addPerson("Adem", 34, Arrays.asList("Swimming", "Cycling"));
    network.addPerson("Murat", 23, Arrays.asList("Reading", "Hiking"));
    network.addPerson("Mehmet", 30, Arrays.asList("Gaming", "TraVeling"));
    network.addPerson("Ahmet", 35, Arrays.asList("Cooking", "Painting"));
    network.addPerson("Ali", 34, Arrays.asList("Swimming", "Cycling"));
    network.addPerson("Veli", 23, Arrays.asList("Reading", "Hiking"));
    network.addPerson("Hasan", 30, Arrays.asList("Gaming", "TraVeling"));
    network.addPerson("Ayse", 12, Arrays.asList("Gaming", "TraVeling"));
    network.addPerson("Fatma", 45, Arrays.asList("Gaming", "TraVeling"));
    network.addPerson("Zeynep", 23, Arrays.asList("Gaming", "TraVeling"));

    network.addFriendship("Alice", "Charlie");
    network.addFriendship("Alice", "Bob");
    network.addFriendship("Alice", "Adem");
    network.addFriendship("Bob", "Charlie");
    network.addFriendship("Bob", "Adem");
    network.addFriendship("Murat", "Mehmet");
    network.addFriendship("Murat", "Hasan");
    network.addFriendship("Murat", "Ahmet");
    network.addFriendship("Veli", "Hasan");
    network.addFriendship("Veli", "Ali");
    network.addFriendship("Murat", "Veli");
    network.addFriendship("Hasan", "Ali");
    network.addFriendship("Bob", "Mehmet");
    network.addFriendship("Charlie", "Mehmet");
    network.addFriendship("Charlie", "Adem");
    network.addFriendship("Adem", "Murat");
    network.addFriendship("Murat", "Ali");
    network.addFriendship("Alice", "Hasan");
    network.addFriendship("Alice", "Veli");
    network.addFriendship("Charlie", "Ali");
    network.addFriendship("Ali", "Bob");
    network.addFriendship("Ayse", "Fatma");
    network.addFriendship("Ayse", "Zeynep");
    network.addFriendship("Fatma", "Zeynep");

    network.findShortestPath("Alice", "Veli");
    network.findShortestPath("Alice", "Hasan");
    network.findShortestPath("Adem", "Ahmet");
    network.findShortestPath("Charlie", "Ali");

    network.suggestFriends("Alice", 4);
    network.suggestFriends("Bob", 2);
    network.suggestFriends("Ali", 3);
    network.suggestFriends("Hasan", 1);
    network.suggestFriends("Ayse", 2);

    network.countClusters();

    network.removeFriendship("Bob", "Mehmet");
    network.removeFriendship("Charlie", "Adem");
    network.removeFriendship("Alice", "Hasan");
    network.removeFriendship("Alice", "Veli");
    network.removeFriendship("Charlie", "Ali");
    network.removeFriendship("Ali", "Bob");
    network.removeFriendship("Ayse", "Fatma");

    network.removePerson("Alice");
    network.removePerson("Bob");
    network.removePerson("Charlie");

    network.findShortestPath("Adem", "Ali");
    network.findShortestPath("Murat", "Ayse");
    network.findShortestPath("Alice", "Ahmet");
  }
}
