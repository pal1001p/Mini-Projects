/**
 * class that extends Exception to throw exception when  a candidate is voted for more than once by the same person (in the same vote)
 */
public class MoreThanOnceException extends Exception{

    /**
     * Constructor with exception that gets thrown when a candidate is voted for more than once by the same person
     * (in the same vote)
     * @param name the candidate name that was voted for multiple times
     */
    public MoreThanOnceException(String name){
        super(name + " was voted for more than once in a single vote!");
    }
}
