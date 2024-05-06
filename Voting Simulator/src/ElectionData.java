import java.util.*;


/**
 * public class ElectionData, which implements methods to run the program
 */
public class ElectionData {
    /**
     * stores a hashmap of key-value pairs of the candidate and the votes for that candidate
     */
    private HashMap<String, Votes> votes;
    /**
     * stores the winner calculation strategy being used
     */
    private I3VoteStrategy strategyUsed;

    /**
     * constructor for ElectionData; initializes all fields
     * @param strategy the strategy being used
     */
    public ElectionData(I3VoteStrategy strategy){
        this.votes = new HashMap<>();
        this.strategyUsed = new MostFirstVotesStrategy();
    }

    /**
     * method for setting the strategy of ElectionData
     * @param strategy the strategy to be set
     */
    public void setStrategy(I3VoteStrategy strategy){
        this.strategyUsed = strategy;
    }

    /**
     * gets the candidates in the running
     * @returns the list of candidates using HashMap's keySet() method
     */
    public Set<String> getCandidates(){

        return new HashSet<>(votes.keySet());
    }


    /**
     * adds a candidate to the ballot by putting them in the hashmap with 0 votes if the exception isn't triggered
     * @param person the person being nominated
     * @throws AlreadyNominatedException if the person is already on the ballot
     * since redundantly adding them could throw away the current data
     */
    public void nominateCandidate(String person) throws AlreadyNominatedException{
        if(votes.containsKey(person)){
            throw new AlreadyNominatedException(person);
        }
        else{
        votes.put(person, new Votes(0,0,0));
         }
    }


    /**
     * increases the candidates votes, respectively if none of the exceptions are triggered
     * @param first the first candidate's name
     * @param second the second candidate's name
     * @param third the third candidate's name
     * @throws CandidateNotNominatedException if any of the choices are not on the ballot
     * @throws MoreThanOnceException  if any of the choices are duplicates
     */
    public void submitVote(String first, String second, String third) throws CandidateNotNominatedException, MoreThanOnceException{
//not sure
        for(Map.Entry<String, Votes> entry : votes.entrySet()) {
            if (entry.getKey().equals(first)) {
                entry.getValue().voteFirst();
            }

            if (entry.getKey().equals(second)) {
                entry.getValue().voteSecond();
            }
            if (entry.getKey().equals(third)) {
                entry.getValue().voteThird();
            }

        }

        if(!votes.containsKey(first)){
            throw new CandidateNotNominatedException(first);
        }
        if(!votes.containsKey(second)){
            throw new CandidateNotNominatedException(second);
        }
        if(!votes.containsKey(third)){
            throw new CandidateNotNominatedException(third);
        }
        if(first.equals(second)){
            throw new MoreThanOnceException(first);
        }
        if(first.equals(third)){
            throw new MoreThanOnceException(first);
        }
        if(third.equals(second)){
            throw new MoreThanOnceException(third);
        }

    }

    /**
     * calls the strategy's calculateWinner(...) method and returns whatever the strategy returns.
     * makes a deep copy of the HashMap to pass to the strategy
     * @return the name of the Optional.of(candidate) if a clear winner was chosen;
     * produces Optional.empty() if there is no clear winner based on the current strategy.
     */

    public Optional<String> calculateWinner(){

        HashMap<String, Votes> copy = new HashMap<>();
        for (HashMap.Entry<String, Votes> entry : votes.entrySet()) {
            String name = entry.getKey();
            Votes vote = entry.getValue();
            copy.put(name, new Votes(entry.getValue()));
        }
        return this.strategyUsed.calculateWinner(copy);
    }
}


