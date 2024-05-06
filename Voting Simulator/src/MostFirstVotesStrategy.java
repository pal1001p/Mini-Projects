import java.util.HashMap;
import java.util.Map;
import java.util.Optional;

/**
 * strategy for evaluating candidates based on whether they have the most first votes in terms of number and percentage
 */
public class MostFirstVotesStrategy implements I3VoteStrategy{



    /**
     * a method for calculating the winner
     * @param votes the hashmap of candidates and the votes for them
     * @return Optional.of(candidate) for the candidate that has both:
     * the most first votes
     * the first votes that are strictly greater (> not >=) than 50% the total # of first votes
     * E.g. a clear majority. having 1/3rd the total first votes is not good enough
     * produces Optional.empty() otherwise
     */
    @Override
    public Optional<String> calculateWinner(HashMap<String, Votes> votes) {

        int mostFirst = 0;
        int totalFirst = 0;
        String candidateWithMost = "";

        //gets total first votes
        for(Votes vote: votes.values()){
            totalFirst += vote.getFirstVotes();
        }

        //loops over each kv pair in hash map
        for(Map.Entry<String, Votes> entry : votes.entrySet()) {

            int firstVotes = entry.getValue().getFirstVotes();

            //if the first votes of some entry is greater, update it
            if (firstVotes > mostFirst) {
                mostFirst = firstVotes;

                //if the highest first votes is greater than 50 percent, update that entry's key
                if (mostFirst > totalFirst / 2) {
                    candidateWithMost = entry.getKey();

                }
            }
        }
        if (!candidateWithMost.isEmpty()) {
            return Optional.of(candidateWithMost);
        } else {
            return Optional.empty();
        }
    }
}

