public class File extends FileSystemElement {

  private String type = "file";

  public File(String name, FileSystemElement parent) {
    super(name, parent);
  }

  public String getType() {
    return type;
  }

  public void setType(String type) {
    this.type = type;
  }

  @Override
  public void displayInfo() {
    System.out.println("File: " + this.getName());
  }
}
