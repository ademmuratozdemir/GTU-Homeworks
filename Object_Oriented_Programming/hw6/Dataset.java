import java.util.ArrayList;
import java.util.List;
public class Dataset {
    
    private List<Observers> observers = new ArrayList<>();  // create and arraylist for storage.


    public void register(Observers newObserver){    // we register observers to our dataset.
        observers.add(newObserver);
    }

    public void add(Entry newEntry){

        // here adding the objects to Player
        if(newEntry instanceof video || newEntry instanceof audio)
        {
            for (Observers obs : observers) {
    
                if(obs instanceof Player)
                {
                    obs.add(newEntry);
                    obs.message();

                }
            }
        }

        // here adding the objects to Viewer
        if(newEntry instanceof text || newEntry instanceof Image)
        {
            for (Observers obs : observers) {
    
                if(obs instanceof Viewer)
                {
                    obs.add(newEntry);
                    obs.message();

                }

            }
        }
    }

    // removing objects that are playable
    public void remove(Playable playable)
    {

        for (Observers obs : observers) {
                if(obs instanceof Player)
                    obs.remove((Entry) playable);
        }      
    }
    // removing objects that are nonplayable
    public void remove(nonPlayable playable)
    {
            for (Observers obs : observers) {
                if(obs instanceof Viewer)
                    obs.remove((Entry) playable);
            }

    }
}