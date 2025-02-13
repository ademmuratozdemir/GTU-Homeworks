import java.text.SimpleDateFormat;
import java.util.*;

public class SocialNetworkGraph {

  Map<String, Person> people = new HashMap<>();
  Map<Person, List<Person>> friendships = new HashMap<>();

  /**
   * Adds a new person to the social network graph.
   *
   * @param name    the name of the person
   * @param age     the age of the person
   * @param hobbies the list of hobbies of the person
   */
  public void addPerson(String name, int age, List<String> hobbies) {
    Person person = new Person(name, age, hobbies);
    people.put(name, person);
    friendships.put(person, new ArrayList<>());
    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    System.out.println(
      "\nPerson added: " +
      person.name +
      " (Timestamp: " +
      sdf.format(person.timestamp) +
      ")"
    );
  }

  /**
   * Removes a person from the social network graph.
   * If the person exists in the network, it removes the person and all their friendships.
   * If the person does not exist in the network, it displays a message indicating that the person was not found.
   *
   * @param name the name of the person to be removed
   */
  public void removePerson(String name) {
    Person person = people.get(name);
    if (person != null) {
      // Remove the person from the network
      people.remove(name);
      // Remove the person's friendships
      List<Person> friends = friendships.remove(person);
      for (Person friend : friends) {
        friendships.get(friend).remove(person);
      }
      System.out.println("\nPerson removed: " + person);
    } else {
      System.out.println("\nPerson not found in the network.");
    }
  }

  /**
   * Adds a friendship between two persons in the social network graph.Created for Main.java
   *
   * @param name1      the name of the first person
   * @param timestamp1 the timestamp of the first person's activity
   * @param name2      the name of the second person
   * @param timestamp2 the timestamp of the second person's activity
   */
  public void addFriendship(
    String name1,
    String timestamp1,
    String name2,
    String timestamp2
  ) {
    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    try {
      Date ts1 = sdf.parse(timestamp1);
      Date ts2 = sdf.parse(timestamp2);
      Person person1 = findPersonByNameAndTimestamp(name1, ts1);
      Person person2 = findPersonByNameAndTimestamp(name2, ts2);

      if (person1 != null && person2 != null) {
        friendships.get(person1).add(person2);
        friendships.get(person2).add(person1);
        System.out.println(
          "\nFriendship added between " + person1.name + " and " + person2.name
        );
      } else {
        System.out.println("\nOne or both persons not found in the network.");
      }
    } catch (Exception e) {
      System.out.println("\nInvalid timestamp format.");
    }
  }

  /**
   * Adds a friendship between two people in the social network.Created for TestMain.java
   *
   * @param name1 the name of the first person
   * @param name2 the name of the second person
   */
  public void addFriendship(String name1, String name2) {
    Person person1 = people.get(name1);
    Person person2 = people.get(name2);
    if (person1 != null && person2 != null) {
      friendships.get(person1).add(person2);
      friendships.get(person2).add(person1);
      System.out.println(
        "\nFriendship added between " + person1.name + " and " + person2.name
      );
    } else {
      System.out.println("\nOne or both persons not found in the network.");
    }
  }

  /**
   * Removes a friendship between two people in the social network.
   *
   * @param name1 the name of the first person
   * @param name2 the name of the second person
   */
  public void removeFriendship(String name1, String name2) {
    Person person1 = people.get(name1);
    Person person2 = people.get(name2);
    if (person1 != null && person2 != null) {
      friendships.get(person1).remove(person2);
      friendships.get(person2).remove(person1);
      System.out.println(
        "\nFriendship removed between " + person1.name + " and " + person2.name
      );
    } else {
      System.out.println("\nOne or both persons not found in the network.");
    }
  }

  /**
   * Finds a person in a social network.
   */
  private Person findPersonByNameAndTimestamp(String name, Date timestamp) {
    for (Person person : people.values()) {
      // Due to the precision of Date objects, we need to use a tolerance value
      long tolerance = 1000; // 1 second tolerance
      if (
        person.name.equals(name) &&
        Math.abs(person.timestamp.getTime() - timestamp.getTime()) < tolerance
      ) {
        return person;
      }
    }
    return null;
  }

  /**
   * Finds the shortest path between two persons in the social network graph based on their names and timestamps. Created for Main.java
   *
   * @param name1      the name of the first person
   * @param timestamp1 the timestamp associated with the first person
   * @param name2      the name of the second person
   * @param timestamp2 the timestamp associated with the second person
   */
  public void findShortestPath(
    String name1,
    String timestamp1,
    String name2,
    String timestamp2
  ) {
    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    try {
      Date ts1 = sdf.parse(timestamp1);
      Date ts2 = sdf.parse(timestamp2);
      Person start = findPersonByNameAndTimestamp(name1, ts1);
      Person end = findPersonByNameAndTimestamp(name2, ts2);

      if (start == null || end == null) {
        System.out.println("\nOne or both persons not found in the network.");
        return;
      }

      Map<Person, Person> prev = bfsFindShortestPath(start, end);
      if (prev != null) {
        printPath(start, end, prev);
      } else {
        System.out.println(
          "\nNo path found between " + name1 + " and " + name2
        );
      }
    } catch (Exception e) {
      System.out.println("\nInvalid timestamp format.");
    }
  }

  /**
   * Finds the shortest path between two persons in the social network graph.Created for TestMain.java
   *
   * @param name1 the name of the first person
   * @param name2 the name of the second person
   */
  public void findShortestPath(String name1, String name2) {
    Person start = people.get(name1);
    Person end = people.get(name2);

    if (start == null || end == null) {
      System.out.println("\nOne or both persons not found in the network.");
      return;
    }

    Map<Person, Person> prev = bfsFindShortestPath(start, end);
    if (prev != null) {
      printPath(start, end, prev);
    } else {
      System.out.println("\nNo path found between " + name1 + " and " + name2);
    }
  }

  /**
   * Performs a breadth-first search to find the shortest path between two persons in the social network graph.
   *
   * @param start the starting person
   * @param end the ending person
   * @return a map containing the previous person for each person in the shortest path, or null if no path is found
   */
  private Map<Person, Person> bfsFindShortestPath(Person start, Person end) {
    Queue<Person> queue = new LinkedList<>();
    Map<Person, Person> prev = new HashMap<>();
    Set<Person> visited = new HashSet<>();

    queue.add(start);
    visited.add(start);
    prev.put(start, null);

    while (!queue.isEmpty()) {
      Person current = queue.poll();

      if (current.equals(end)) {
        return prev;
      }

      for (Person neighbor : friendships.get(current)) {
        if (!visited.contains(neighbor)) {
          queue.add(neighbor);
          visited.add(neighbor);
          prev.put(neighbor, current);
        }
      }
    }
    return null;
  }

  /**
   * Prints the shortest path between two persons in the social network graph.
   *
   * @param start the starting person
   * @param end the ending person
   * @param prev a map containing the previous person for each person in the path
   */
  private void printPath(Person start, Person end, Map<Person, Person> prev) {
    List<Person> path = new ArrayList<>();
    for (Person at = end; at != null; at = prev.get(at)) {
      path.add(at);
    }
    Collections.reverse(path);
    System.out.println("\nShortest path: " + path);
  }

  /**
   * Counts the clusters in the social network graph.
   * A cluster is a group of connected people in the graph.
   * Uses breadth-first search (BFS) algorithm to find clusters.
   * Prints the number of clusters found and the people in each cluster.
   */
  public void countClusters() {
    Set<Person> visited = new HashSet<>();
    int clusterCount = 0;
    List<List<Person>> clusters = new ArrayList<>();

    System.out.println("\nCounting clusters in the social network...");

    for (Person person : people.values()) {
      if (!visited.contains(person)) {
        List<Person> cluster = new ArrayList<>();
        bfsCluster(person, visited, cluster);
        clusters.add(cluster);
        clusterCount++;
      }
    }

    System.out.println("\nNumber of clusters found: " + clusterCount);

    for (int i = 0; i < clusters.size(); i++) {
      System.out.println("\nCluster " + (i + 1) + ":");
      for (Person person : clusters.get(i)) {
        System.out.println(person.name);
      }
    }
  }

  /**
   * Performs a breadth-first search starting from the specified person and adds all visited persons to the cluster.
   *
   * @param start    the starting person for the breadth-first search
   * @param visited  a set of visited persons to keep track of visited nodes
   * @param cluster  a list to store the persons in the cluster
   */
  private void bfsCluster(
    Person start,
    Set<Person> visited,
    List<Person> cluster
  ) {
    Queue<Person> queue = new LinkedList<>();
    queue.add(start);
    visited.add(start);

    while (!queue.isEmpty()) {
      Person current = queue.poll();
      cluster.add(current);

      for (Person neighbor : friendships.get(current)) {
        if (!visited.contains(neighbor)) {
          queue.add(neighbor);
          visited.add(neighbor);
        }
      }
    }
  }

  /**
   * Suggests friends for a given person based on mutual friends and common hobbies.Created for Main.java
   *
   * @param name           the name of the person
   * @param timestamp      the timestamp in the format "yyyy-MM-dd HH:mm:ss"
   * @param maxSuggestions the maximum number of friend suggestions to display
   */
  public void suggestFriends(
    String name,
    String timestamp,
    int maxSuggestions
  ) {
    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    try {
      Date ts = sdf.parse(timestamp);
      Person person = findPersonByNameAndTimestamp(name, ts);

      if (person == null) {
        System.out.println("\nPerson not found in the network.");
        return;
      }

      Map<Person, Double> scores = new HashMap<>();
      List<Person> friends = friendships.get(person);

      for (Person p : people.values()) {
        if (!p.equals(person) && !friends.contains(p)) {
          int mutualFriends = countMutualFriends(person, p);
          int commonHobbies = countCommonHobbies(person, p);
          double score = mutualFriends + (commonHobbies * 0.5);
          if (score > 0) {
            scores.put(p, score);
          }
        }
      }

      List<Map.Entry<Person, Double>> sortedScores = new ArrayList<>(
        scores.entrySet()
      );
      sortedScores.sort((a, b) -> Double.compare(b.getValue(), a.getValue()));

      System.out.println("\nSuggested friends for " + person.name + ":");
      for (int i = 0; i < Math.min(maxSuggestions, sortedScores.size()); i++) {
        Map.Entry<Person, Double> entry = sortedScores.get(i);
        Person suggestedFriend = entry.getKey();
        double score = entry.getValue();
        int mutualFriends = countMutualFriends(person, suggestedFriend);
        int commonHobbies = countCommonHobbies(person, suggestedFriend);
        System.out.println(
          suggestedFriend.name +
          " (Score: " +
          score +
          ", " +
          mutualFriends +
          " mutual friends, " +
          commonHobbies +
          " common hobbies)"
        );
      }
    } catch (Exception e) {
      System.out.println("\nInvalid timestamp format.");
    }
  }

  /**
   * Suggests friends for a given person based on mutual friends and common hobbies.Created for TestMain.java
   *
   * @param name The name of the person for whom to suggest friends.
   * @param maxSuggestions The maximum number of friend suggestions to display.
   */
  public void suggestFriends(String name, int maxSuggestions) {
    Person person = people.get(name);

    if (person == null) {
      System.out.println("\nPerson not found in the network.");
      return;
    }

    Map<Person, Double> scores = new HashMap<>();
    List<Person> friends = friendships.get(person);

    for (Person p : people.values()) {
      if (!p.equals(person) && !friends.contains(p)) {
        int mutualFriends = countMutualFriends(person, p);
        int commonHobbies = countCommonHobbies(person, p);
        double score = mutualFriends + (commonHobbies * 0.5);
        if (score > 0) {
          scores.put(p, score);
        }
      }
    }

    List<Map.Entry<Person, Double>> sortedScores = new ArrayList<>(
      scores.entrySet()
    );
    sortedScores.sort((a, b) -> Double.compare(b.getValue(), a.getValue()));

    System.out.println("\nSuggested friends for " + person.name + ":");
    for (int i = 0; i < Math.min(maxSuggestions, sortedScores.size()); i++) {
      Map.Entry<Person, Double> entry = sortedScores.get(i);
      Person suggestedFriend = entry.getKey();
      double score = entry.getValue();
      int mutualFriends = countMutualFriends(person, suggestedFriend);
      int commonHobbies = countCommonHobbies(person, suggestedFriend);
      System.out.println(
        suggestedFriend.name +
        " (Score: " +
        score +
        ", " +
        mutualFriends +
        " mutual friends, " +
        commonHobbies +
        " common hobbies)"
      );
    }
  }

  /**
   * Counts the number of mutual friends between two persons.
   *
   * @param person1 the first person
   * @param person2 the second person
   * @return the number of mutual friends between the two persons
   */
  private int countMutualFriends(Person person1, Person person2) {
    List<Person> friends1 = friendships.get(person1);
    List<Person> friends2 = friendships.get(person2);
    int mutualFriends = 0;

    for (Person friend : friends1) {
      if (friends2.contains(friend)) {
        mutualFriends++;
      }
    }

    return mutualFriends;
  }

  /**
   * Counts the number of common hobbies between two persons.
   *
   * @param person1 the first person
   * @param person2 the second person
   * @return the number of common hobbies between the two persons
   */
  private int countCommonHobbies(Person person1, Person person2) {
    List<String> hobbies1 = person1.hobbies;
    List<String> hobbies2 = person2.hobbies;
    int commonHobbies = 0;

    for (String hobby : hobbies1) {
      if (hobbies2.contains(hobby)) {
        commonHobbies++;
      }
    }

    return commonHobbies;
  }
}
