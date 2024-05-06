/**
 * class that extends Exception to throw exception when a candidate is voted for but has not been nominated
 */
public class CandidateNotNominatedException extends Exception{

    /**
     * field to store candidate that was voted for but not nominated
     */
    private String candidate;

    /**
     * Constructor for exception that gets thrown when a candidate is voted for but has not been nominated
     * Stores the candidate name in a private field (candidate)
     * @param name the candidate name that was voted for but not nominated
     */
    public CandidateNotNominatedException(String name){
        super(name + " was not nominated!");
        candidate = name;
    }

    /**
     * getter for the candidate name that was voted for but not nominated
     * @return the name of the candidate
     */
    public String getCandidate(){
        return candidate;
    }


}
