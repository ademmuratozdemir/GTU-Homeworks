import java.util.Scanner;

public class Driver {

  public static void main(String[] args) {
    FileSystem fs = new FileSystem();
    Scanner scanner = new Scanner(System.in);
    Directory currentDirectory = fs.getRootDirectory();

    while (true) {
      System.out.println(
        "\nCurrent Directory: " + fs.getCurrentPath(currentDirectory)
      );
      System.out.println("\n===== File System Management Menu =====");
      System.out.println("1. Change directory");
      System.out.println("2. List directory contents");
      System.out.println("3. Create file");
      System.out.println("4. Create directory");
      System.out.println("5. Delete file");
      System.out.println("6. Delete directory");
      System.out.println("7. Move file/directory");
      System.out.println("8. Search file/directory");
      System.out.println("9. Print directory tree");
      System.out.println("10. Sort contents by date");
      System.out.println("11. Exit");
      System.out.print("Please select an option: ");

      int choice = scanner.nextInt();
      switch (choice) {
        case 1:
          System.out.println(
            "Current Directory: " + fs.getCurrentPath(currentDirectory)
          );
          System.out.println("Enter the path to change directory");
          String path = scanner.next();
          if (fs.changeDirectory(path) != null) currentDirectory =
            fs.changeDirectory(path); else System.out.println("Invalid path");

          break;
        case 2:
          System.out.println(
            "\nListing directory contents of " +
            fs.getCurrentPath(currentDirectory)
          );
          fs.listContents(currentDirectory);
          break;
        case 3:
          System.out.println(
            "\nCurrent Directory: " + fs.getCurrentPath(currentDirectory)
          );
          System.out.print("\nEnter the file name: ");
          String createFileName = scanner.next();
          fs.createFile(createFileName, currentDirectory);
          break;
        case 4:
          System.out.println(
            "\nCurrent Directory: " + fs.getCurrentPath(currentDirectory)
          );
          System.out.print("\nEnter the directory name: ");
          String createDirectoryName = scanner.next();
          fs.createDirectory(createDirectoryName, currentDirectory);
          break;
        case 5:
          System.out.println(
            "\nCurrent Directory: " + fs.getCurrentPath(currentDirectory)
          );
          System.out.print("\nEnter the file name: ");
          String deleteFileName = scanner.next();
          fs.deleteFile(deleteFileName, currentDirectory);
          break;
        case 6:
          System.out.println(
            "\nCurrent Directory: " + fs.getCurrentPath(currentDirectory)
          );
          System.out.print("\nEnter the directory name: ");
          String deleteDirectoryName = scanner.next();
          fs.deleteDirectory(deleteDirectoryName, currentDirectory);
          break;
        case 7:
          System.out.println(
            "\nCurrent Directory: " + fs.getCurrentPath(currentDirectory)
          );
          System.out.print("\nEnter the file/directory name to move: ");
          String name = scanner.next();
          System.out.print("Enter the new directory path: ");
          String dest = scanner.next();
          Directory destination = (Directory) findElementAtPathEnd(dest, fs);

          fs.moveElement(name, currentDirectory, destination);
          break;
        case 8:
          System.out.print("\nEnter the file/directory name: ");
          String searchName = scanner.next();
          System.out.println("Search query :" + searchName);
          System.out.println("Searching from root");
          fs.search(searchName, fs.getRootDirectory());
          break;
        case 9:
          fs.printDirectoryTree();
          break;
        case 10:
          fs.sortDirectoryByDate(currentDirectory);
          break;
        case 11:
          System.out.println("Exiting...");
          scanner.close();
          System.exit(0);
      }
    }
  }

  public static FileSystemElement findElementAtPathEnd(
    String path,
    FileSystem fs
  ) {
    // parsing array of directories
    String[] directories = path.split("/");

    // getting the last element of the path
    String elementName = directories[directories.length - 1];

    // if the path is absolute
    for (FileSystemElement childElement : fs.getRootDirectory().getChildren()) {
      // if the child is a directory
      if (!(childElement instanceof Directory)) {
        continue;
      }

      // getting the name of the child
      String childName = childElement.getName();

      // if the child name is equal to the element name
      if (childName.equals(elementName)) {
        return childElement;
      }

      // recursively calling the method to find the element
      FileSystemElement foundElement = findChildElement(
        (Directory) childElement,
        elementName
      );
      // if the element is found in the child directory
      if (foundElement != null) {
        return foundElement;
      }
    }

    return null;
  }

  // helper method to find the element in the child directory
  private static FileSystemElement findChildElement(
    Directory directory,
    String elementName
  ) {
    // iterating over the children of the directory
    for (FileSystemElement childElement : directory.getChildren()) {
      // if the child is not a directory, continue
      if (!(childElement instanceof Directory)) {
        continue;
      }

      // getting the name of the child
      String childName = childElement.getName();

      // if the child name is equal to the element name
      if (childName.equals(elementName)) {
        return childElement;
      }

      // recursively calling the method to find the element
      FileSystemElement foundElement = findChildElement(
        (Directory) childElement,
        elementName
      );
      // if the element is found in the child directory
      if (foundElement != null) {
        return foundElement;
      }
    }

    // if the element is not found in the directory
    return null;
  }
}
