
// we create audio class inherits from Entry class.And use visual and nonplayable interfaces.

public class Image extends Entry implements Visual,nonPlayable{ 

    public String dimension;
    public String otherInfo;
    

    public Image(String name, String dimension, String otherInfo) {
        super(name);
        this.dimension = dimension;
        this.otherInfo = otherInfo;
    }

    public double getDimension() {
        return 600;
    }

    @Override
    public void info() {
       System.out.println("Image");
       System.out.println(super.getName());
       System.out.println(getDimension());
        
    }
    
}
