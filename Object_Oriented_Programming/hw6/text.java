
//we create audio class inherits from Entry class.And use nonvisual and nonplayable interfaces.

public class text extends Entry implements nonVisual,nonPlayable{

    public String otherInfo;

    public text(String name, String otherInfo) {
        super(name);
        this.otherInfo = otherInfo;
    }

    public void info(){
       System.out.println("Text");
       System.out.println(super.getName());
    }
    
}
