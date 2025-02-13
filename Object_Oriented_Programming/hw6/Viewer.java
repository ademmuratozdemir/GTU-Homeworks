
public class Viewer extends Observers{

    int current = 0;



    public void message(){

        System.out.println("New viewer added.");

    }
    // showing list function
    public void show_list()
    {
        for (Entry e : entries) {
            if(e instanceof nonPlayable)
                System.out.println(e.getName());
        }

    }
    // currently_playing function
    public Playable currently_playing()
    {
        int index=0;

        for (Entry e : entries) {
            if(e instanceof nonPlayable && index == current){
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
            for (j = current; j < entries.size(); j++) {
                
                if(entries.get(j) instanceof audio)
                {
                    current = j;
                    break;
                }
            }
        }
        else if(type == "video")
        {
            for (j = current; j < entries.size(); j++) {
                
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
        if(type == "text")
        {
            for (j = current; j > 0; j--) {
                
                if(entries.get(j) instanceof text)
                {
                    current = j;
                    break;
                }
            }
            
        }
        else if(type == "image")
        {
            for (j = current; j > 0; j--) {
                
                if(entries.get(j) instanceof video)
                {
                    current = j;
                    break;
                }
            }
        }
        if(j<0)
            throw new Exception("No Item");
    }
    // remove function
    public void remove(nonPlayable nonplayable){
        entries.remove((Entry) nonplayable);
    }

   







}