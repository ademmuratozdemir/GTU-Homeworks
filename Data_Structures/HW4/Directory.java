import java.util.LinkedList;

public class Directory extends FileSystemElement {

  private String type = "directory";
  private LinkedList<FileSystemElement> children;

  public String getType() {
    return type;
  }

  public void setType(String type) {
    this.type = type;
  }

  public Directory(String name, FileSystemElement parent) {
    super(name, parent);
    children = new LinkedList<>();
  }

  public LinkedList<FileSystemElement> getChildren() {
    return children;
  }

  public void setChildren(LinkedList<FileSystemElement> children) {
    this.children = children;
  }

  public void addElement(FileSystemElement element) {
    children.add(element);
  }

  public void removeElement(FileSystemElement element) {
    children.remove(element);
  }

  @Override
  public void displayInfo() {
    System.out.println("Directory: " + this.getName());
  }
}
