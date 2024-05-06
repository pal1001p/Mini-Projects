/**
 * class that extends Exception to throw exception when a candidate is redundantly nominated
 */
public class AlreadyNominatedException extends Exception{


    /**
     * Constructor for exception that gets thrown when a candidate is redundantly nominated
     * @param person the candidate name that was already nominated
     */
    public AlreadyNominatedException(String person){
         super(person + " was already nominated!");
    }


}
