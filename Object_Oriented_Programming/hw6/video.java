
//we create audio class inherits from Entry class.And use visual and playable interfaces.

public class video extends Entry implements Visual,Playable{


    public String duration;
    public String otherInfo;

    public video(String name, String duration, String otherInfo) {
        super(name);

        this.duration = duration;
        this.otherInfo = otherInfo;
        
    }

    public String getDuration() {
        
        return duration;
    }
    
    public void info() {
       System.out.printf("Video"+ "  ");
       System.out.printf(super.getName()+ "  ");
       System.out.printf(getDuration()+ "  ");
       System.out.printf(otherInfo+ "\n");
       
    }

    @Override
    public double getDimension() {
        return 600;
    }
    
}
