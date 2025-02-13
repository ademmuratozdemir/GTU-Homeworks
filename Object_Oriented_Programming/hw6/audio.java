
// For we create audio class inherits from Entry class.And use nonvisual and playable interfaces.

public class audio extends Entry implements nonVisual, Playable{

    public String duration;
    public String otherInfo;


    public audio(String name, String duration, String otherInfo) {
        super(name);
        this.duration = duration;
        this.otherInfo = otherInfo;

    }

    
    public String getDuration() {
        
        return duration;
    }

    public void info() {
       System.out.printf("Audio  ");
       System.out.printf(super.getName()+ "  ");
       System.out.printf(getDuration()+ "  ");
       System.out.printf(otherInfo + "\n");

    }
    
}
