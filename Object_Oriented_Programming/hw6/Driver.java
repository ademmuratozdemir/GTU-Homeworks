public class Driver {

    public static void main(String[] args) {
        // We create observable dataset.
        Dataset ds = new Dataset();
        //We create different observers.
        Player p1 = new Player();
        Player p2 = new Player();
        Viewer v1 = new Viewer();
        Viewer v2 = new Viewer();
        //Lets register them to our dataset so that they can reach to data and updates.
        ds.register(p1);
        ds.register(p2);
        ds.register(v1);
        ds.register(v2);
        
        ds.add(new Image("imagename1", "dimension info1", "other info1"));  // Creating image objects.
        ds.add(new Image("imagename2", "dimension info2", "other info2"));
        ds.add(new Image("imagename3", "dimension info3", "other info3"));
        ds.add(new Image("imagename4", "dimension info4", "other info4"));
        ds.add(new Image("imagename5", "dimension info5", "other info5"));
        ds.add(new audio("audioname1", "duration1", "other info1"));   // Creating audio objects.
        ds.add(new audio("audioname2", "duration2", "other info2"));
        ds.add(new audio("audioname3", "duration3", "other info3"));
        ds.add(new video("videoname1", "duration1", "other info1"));   // Creating video objects.
        ds.add(new video("videoname2", "duration2", "other info2"));
        ds.add(new video("videoname3", "duration3", "other info3"));
        ds.add(new text("textname1", "other info1"));   // Creating image objects.
        ds.add(new text("textname2", "other info2"));
        ds.add(new text("textname3", "other info3"));

        System.out.printf("\nPlayers:\n\n");
        p1.show_list();   // Firstly showing the player list.
        System.out.printf("\nViewers:\n\n");
        v1.show_list();   // Showing the viewer list.
        
        System.out.println("\nGoing to first player");
        Playable po = p1.currently_playing();  // Here, showing the first playable object.
        
        po.info();
        System.out.printf("\n");
        
        System.out.println("Going to first video");
        try{
            p1.next("video");     // Here, showing the first video.
        }catch (Exception e){ e.printStackTrace();}

        po = p1.currently_playing();
        
        po.info();

        System.out.println("\nGoing to second video");
        try{
            p1.next("video");  // Here, showing the next video.
        }catch (Exception e){ e.printStackTrace();}

        po = p1.currently_playing();
        
        po.info();

        System.out.printf("\n");
        
        ds.remove(po);   // Here, deleting the currently playing player.
        System.out.printf("\n");

        System.out.println("Showing list\n");
        p1.show_list();  // After deleting, showing the updated player list.
        System.out.printf("\n");


        System.out.println("Going to previous audio");
        try{
            p1.previous("audio");  // Then we go to previous audio after delete the video.
        }catch (Exception e){
            e.printStackTrace();
        }
        po = p1.currently_playing();

        po.info();

        System.out.printf("\n");
        System.out.println("Going to previous audio");  // Here, we showing the previous audio.
        try{
            p1.previous("audio");
        }catch (Exception e){
            e.printStackTrace();
        }
        po = p1.currently_playing();

        po.info();





    }
    
}
