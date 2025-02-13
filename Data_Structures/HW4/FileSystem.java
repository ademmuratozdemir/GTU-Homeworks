import java.sql.Timestamp;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.List;

public class FileSystem {

  private Directory rootDirectory;

  // Constructor
  public FileSystem() {
    // Initialize root directory
    rootDirectory = new Directory("root", null);
  }

  // Getters and Setters
  public Directory getRootDirectory() {
    return rootDirectory;
  }

  public void setRootDirectory(Directory rootDirectory) {
    this.rootDirectory = rootDirectory;
  }

  // a function that creates a file
  public void createFile(String name, Directory parent) {
    File file = new File(name, parent);
    parent.addElement(file);
    System.out.println("\nFile created: " + name);
  }

  // a function that creates a directory
  public void createDirectory(String name, Directory parent) {
    Directory directory = new Directory(name, parent);
    parent.addElement(directory);
    System.out.println("\nDirectory created: " + name);
  }

  // a function that deletes a file
  public void deleteFile(String name, Directory parent) {
    for (FileSystemElement element : parent.getChildren()) {
      if (element.getName().equals(name)) {
        parent.removeElement(element);
        System.out.println("File deleted: " + name);
        return;
      }
    }
    System.out.println("File not found: " + name);
  }

  // a function that deletes a directory
  public void deleteDirectory(String name, Directory parent) {
    for (FileSystemElement element : parent.getChildren()) {
      if (element.getName().equals(name)) {
        parent.removeElement(element);
        System.out.println("Directory deleted: " + name);
        return;
      }
    }
    System.out.println("Directory not found: " + name);
  }

  // move the file or directory to the new directory
  public void moveElement(
    String name,
    Directory currentDirectory,
    Directory newDirectory
  ) {
    // search for the file or directory in the current directory
    FileSystemElement element;

    for (FileSystemElement child : currentDirectory.getChildren()) {
      if (child.getName().equals(name)) {
        element = child;
        currentDirectory.removeElement(element);
        element.setParent(newDirectory);
        newDirectory.addElement(element);
        System.out.println("Element moved: " + name);
        return;
      }
    }
  }

  // search for a file or directory in the file system
  public void search(String name, FileSystemElement currentElement) {
    // check if the current element matches the search name
    if (currentElement.getName().equals(name)) {
      System.out.println("Found: " + getPath(currentElement));
    }

    // if the current element is a directory, recursively search its children
    if (currentElement instanceof Directory) {
      Directory currentDirectory = (Directory) currentElement;
      for (FileSystemElement child : currentDirectory.getChildren()) {
        search(name, child);
      }
    }
  }

  //helper function to get the path of the current element
  private String getPath(FileSystemElement element) {
    StringBuilder pathBuilder = new StringBuilder();
    FileSystemElement current = element;

    // traverse from the current element up to the root directory
    while (current != null) {
      pathBuilder.insert(0, current.getName());
      if (current.getParent() != null) {
        pathBuilder.insert(0, "/");
      }
      current = current.getParent();
    }

    return pathBuilder.toString();
  }

  // print the directory tree
  public void printDirectoryTree() {
    // initialize the current directory as the root directory
    Directory currentDirectory = rootDirectory;

    // print the path to the current directory from the root
    System.out.println("\nPath to current directory from root:");
    printPathToCurrentDirectory(currentDirectory, "");
  }

  // helper method to print the path to the current directory
  private void printPathToCurrentDirectory(
    Directory currentDirectory,
    String indent
  ) {
    // print the current directory with appropriate indentation
    System.out.println(indent + "* " + currentDirectory.getName() + "/");

    // print each child directory or file
    for (FileSystemElement child : currentDirectory.getChildren()) {
      if (child instanceof Directory) {
        printPathToCurrentDirectory((Directory) child, indent + "  ");
      } else {
        System.out.println(indent + "  " + child.getName());
      }
    }
  }

  // a function that lists the contents of a directory
  public void listContents(Directory directory) {
    for (FileSystemElement element : directory.getChildren()) {
      if (element instanceof File) {
        System.out.println("File: " + element.getName());
      } else if (element instanceof Directory) {
        System.out.println("* " + element.getName() + "/");
      }
    }
  }

  // a function that sorts and prints date of children of a directory by date
  public void sortDirectoryByDate(Directory directory) {
    // get the children of the directory
    List<FileSystemElement> children = directory.getChildren();

    // sort the children by date
    Collections.sort(
      children,
      new Comparator<FileSystemElement>() {
        public int compare(
          FileSystemElement element1,
          FileSystemElement element2
        ) {
          return element1.getDataCreated().compareTo(element2.getDataCreated());
        }
      }
    );

    // display the sorted directory contents by date
    System.out.println("Sorted Directory Contents by Date:");
    for (FileSystemElement child : children) {
      Timestamp timestamp = child.getDataCreated();
      Date date = new Date(timestamp.getTime());
      System.out.println(child.getName() + " - " + date);
    }
  }

  // a function that displays the current path from root to current directory
  public String getCurrentPath(Directory directory) {
    String path = "";
    for (
      Directory dir = directory;
      dir != null;
      dir = (Directory) dir.getParent()
    ) {
      path = dir.getName() + "/" + path;
    }
    return path;
  }

  // a function that changes the current directory based on the given path
  public Directory changeDirectory(String path) {
    String[] directories = path.split("/");
    Directory currentDirectory = rootDirectory;
    for (String dir : directories) {
      if (dir.equals("..")) {
        currentDirectory = (Directory) currentDirectory.getParent();
      } else {
        boolean found = false;
        for (FileSystemElement element : currentDirectory.getChildren()) {
          if (element.getName().equals(dir)) {
            currentDirectory = (Directory) element;
            found = true;
            break;
          }
        }
        if (!found) {
          System.out.println("Directory not found: " + dir);
          return null;
        }
      }
    }
    return currentDirectory;
  }
}
