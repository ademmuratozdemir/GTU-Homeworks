import java.util.*;

public class Main {

  public static void main(String[] args) {
    SocialNetworkGraph network = new SocialNetworkGraph();
    Scanner scanner = new Scanner(System.in);
    int choice;

    do {
      System.out.println("\n===== Social Network Analysis Menu =====");
      System.out.println("1. Add person");
      System.out.println("2. Remove person");
      System.out.println("3. Add friendship");
      System.out.println("4. Remove friendship");
      System.out.println("5. Find shortest path");
      System.out.println("6. Suggest friends");
      System.out.println("7. Count clusters");
      System.out.println("8. Exit");
      System.out.print("Please select an option: ");
      choice = scanner.nextInt();
      scanner.nextLine();

      switch (choice) {
        case 1:
          System.out.print("Enter name: ");
          String nameForAdding = scanner.nextLine();
          System.out.print("Enter age: ");
          int age = scanner.nextInt();
          scanner.nextLine();
          System.out.print("Enter hobbies (comma-separated): ");
          String hobbiesInput = scanner.nextLine();
          List<String> hobbies = Arrays.asList(hobbiesInput.split(","));
          network.addPerson(nameForAdding, age, hobbies);
          break;
        case 2:
          System.out.print("Enter name: ");
          String nameForDeleting = scanner.nextLine();
          network.removePerson(nameForDeleting);
          break;
        case 3:
          System.out.print("Enter first person’s name: ");
          String name1_forAddingFriendship = scanner.nextLine();
          System.out.print("Enter first person’s timestamp: ");
          String timestamp1_forAddingFriendship = scanner.nextLine();
          System.out.print("Enter second person’s name: ");
          String name2_forAddingFriendship = scanner.nextLine();
          System.out.print("Enter second person’s timestamp: ");
          String timestamp2_forAddingFriendship = scanner.nextLine();
          network.addFriendship(
            name1_forAddingFriendship,
            timestamp1_forAddingFriendship,
            name2_forAddingFriendship,
            timestamp2_forAddingFriendship
          );
          break;
        case 4:
          System.out.print("Enter first person’s name: ");
          String name1_forDeletingFriendship = scanner.nextLine();
          System.out.print("Enter second person's name: ");
          String name2_forDeletingFriendship = scanner.nextLine();
          network.removeFriendship(
            name1_forDeletingFriendship,
            name2_forDeletingFriendship
          );
          break;
        case 5:
          System.out.print("Enter first person’s name: ");
          String name1_forFindShortestPath = scanner.nextLine();
          System.out.print("Enter first person’s timestamp: ");
          String timestamp1_forFindShortestPath = scanner.nextLine();
          System.out.print("Enter second person’s name: ");
          String name2_forFindShortestPath = scanner.nextLine();
          System.out.print("Enter second person’s timestamp: ");
          String timestamp2_forFindShortestPath = scanner.nextLine();
          network.findShortestPath(
            name1_forFindShortestPath,
            timestamp1_forFindShortestPath,
            name2_forFindShortestPath,
            timestamp2_forFindShortestPath
          );
          break;
        case 6:
          System.out.print("Enter name: ");
          String nameForSuggestingFriends = scanner.nextLine();
          System.out.println("Enter person's timestamp: ");
          String timestampForSuggestingFriends = scanner.nextLine();
          System.out.println("Enter maximum number of friends to suggest: ");
          int maxFriendsToSuggest = scanner.nextInt();
          network.suggestFriends(
            nameForSuggestingFriends,
            timestampForSuggestingFriends,
            maxFriendsToSuggest
          );
          break;
        case 7:
          network.countClusters();
          break;
        case 8:
          System.out.println("Exiting program...");
          System.exit(0);
          break;
        default:
          System.out.println("Invalid option. Please try again.");
          break;
      }
    } while (choice != 8);

    scanner.close();
  }
}
