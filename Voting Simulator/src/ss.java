public class BookCommandLineIO {

    /**
     * addBookFromIO() Handles adding the book from command line input using scanner
     * handles all exceptions here
     */
    public void ..0.. {
        Scanner keyboard = new Scanner(System.in);
        System.out.println("Please enter a book name:");
        String bookName = keyboard...1..;
        System.out.println("Please enter book content or !DONE to finish");
        boolean keepGoing = true;
        String content = "";
        while(keepGoing){
            String line = keyboard...1..;
            if(line.equals("!DONE")){
                keepGoing = false;
            }
            else{
                content += line;
            }
        }