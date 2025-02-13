import java.sql.Timestamp;

public abstract class FileSystemElement {

  protected String name;
  protected FileSystemElement parent;
  protected Timestamp dataCreated;

  public FileSystemElement(String name, FileSystemElement parent) {
    this.name = name;
    this.parent = parent;
    this.dataCreated = new Timestamp(System.currentTimeMillis());
  }

  public String getName() {
    return name;
  }

  public void setName(String name) {
    this.name = name;
  }

  public Timestamp getDataCreated() {
    return dataCreated;
  }

  public void setDataCreated(Timestamp dataCreated) {
    this.dataCreated = dataCreated;
  }

  public FileSystemElement getParent() {
    return parent;
  }

  public void setParent(FileSystemElement parent) {
    this.parent = parent;
  }

  public abstract void displayInfo();
}
