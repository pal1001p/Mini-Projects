import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;
import java.util.Optional;

/**
 * strategy based on getting winner based on whether they got the most votes in one category
 */
public class MostAgreeableStrategy implements I3VoteStrategy{
    /**
     *
     * @param votes the hashmap of candidates and the votes for them
     * @return Optional.of(candidate) for the candidate
     * that has the most votes in a single category (first, second, or third, respectively).
     * E.g. if bristaco has 2 first votes, but gompei has 3 second votes, gompei beats bristaco
     * produces Optional.empty() otherwise
     */
    @Override
    public Optional<String> calculateWinner(HashMap<String, Votes> votes) {
        int mostVotesInACategory = 0;
        String candidateWithMostVotes = "";

        //loops hashmap
        for (Map.Entry<String, Votes> entry : votes.entrySet()) {

            //looks up each pair's number of votes in all categories
            int firstVotes = entry.getValue().getFirstVotes();
            int secondVotes = entry.getValue().getSecondVotes();
            int thirdVotes = entry.getValue().getThirdVotes();

            //updates count if they have more votes in that category
            //iterates until it stops on the category of the candidate with most votes in that category
            if (firstVotes > mostVotesInACategory) {
                mostVotesInACategory = firstVotes;
                candidateWithMostVotes = entry.getKey();
            }
            if (secondVotes > mostVotesInACategory) {
                mostVotesInACategory = secondVotes;
                candidateWithMostVotes = entry.getKey();
            }
            if (thirdVotes > mostVotesInACategory) {
                mostVotesInACategory = thirdVotes;
                candidateWithMostVotes = entry.getKey();
            }
            }


        if (!candidateWithMostVotes.isEmpty()) {
            return Optional.of(candidateWithMostVotes);
        } else {
            return Optional.empty();
        }
}
}
