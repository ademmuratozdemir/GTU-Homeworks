public class Player extends Observers{
    
    private int current=0;


    public void message(){

        System.out.println("New player added.");

    }
    
    // showing list function
    public void show_list()
    {
        for (Entry e : entries) {
            if(e instanceof Playable)
                System.out.println(e.getName());
        }

    }
    // currenctly_playing function
    public Playable currently_playing()
    {
        int index=0;

        for (Entry e : entries) {
            if(e instanceof Playable && index == current){
                return ((Playable) e);
            }
            index++;
        }
        return null;
    }
    // next function
    public void next(String type) throws Exception{

        int j=0;
        if(type == "audio")
        {
            for (j = current+1; j < entries.size(); j++) {
                
                if(entries.get(j) instanceof audio)
                {
                    current = j;
                    break;
                }
            }
        }
        else if(type == "video")
        {
            for (j = current+1; j < entries.size(); j++) {
                
                if(entries.get(j) instanceof video)
                {
                    current = j;
                    break;
                }
            }

        }

        if(j == entries.size())
            throw new Exception("End of the list");
        

    }
    // previous function
    public void previous(String type) throws Exception{

        int j=0;
        boolean check = false;
        if(type == "audio")
        {
            for (j = current-1; j > 0; j--) {
                
                if(entries.get(j) instanceof audio)
                {
                    check = true;
                    current = j;
                    break;
                }
            }
            
        }
        else if(type == "video")
        {
            for (j = current-1; j > 0; j--) {
                
                if(entries.get(j) instanceof video)
                {
                    check = true;
                    current = j;
                    break;
                }
            }
        }
        if(j<0 || !check)
            throw new Exception("No Item");  // if there is no item , throwing an exception.
    }
    // remove function
    public void remove(Playable playable){
        entries.remove((Entry)playable);
    }


}
