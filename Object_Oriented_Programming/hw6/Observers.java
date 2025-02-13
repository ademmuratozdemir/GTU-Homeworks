import java.util.ArrayList;
import java.util.List;

// a base class for create Player and Viewer classes.

public class Observers {

   protected List<Entry> entries = new ArrayList<>();

   // add function
   public void add(Entry entry){
        entries.add(entry); 
   }
   // remove function
   public void remove(Entry entry)
   {
        System.out.println(entry.getName() + " deleted from " + getClass().getName() + ".");
        entries.remove(entry);
   }

   public void message(){
       System.out.println("Observers base class");
   }

    
}
