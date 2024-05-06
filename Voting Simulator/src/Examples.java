import org.junit.Test;

import java.util.*;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

public class Examples {
 ElectionData eData;

 public Examples(){
     eData = new ElectionData(new MostFirstVotesStrategy());
    }

// probably fails bc idk how to implement optional.empty
    @Test
    public void testSetStrategy(){
    eData.setStrategy(new MostAgreeableStrategy());

        try {
            this.eData.nominateCandidate("gompei");
            this.eData.nominateCandidate("husky");
            this.eData.nominateCandidate("bristaco");
            this.eData.submitVote("gompei", "husky", "bristaco");
            this.eData.submitVote("bristaco", "husky", "gompei");

        }
        catch(Exception e){
            fail(e.getMessage());
        }

        //try catch says that test will immediately fail if any of the functions in the try part are bad
        assertEquals(Optional.of("husky"), this.eData.calculateWinner());
    }

@Test
    public void testSetStrategy2(){
        eData.setStrategy(new MostAgreeableStrategy());

        try {
            this.eData.nominateCandidate("gompei");
            this.eData.nominateCandidate("husky");
            this.eData.nominateCandidate("bristaco");
            this.eData.submitVote("bristaco", "gompei", "husky");
            this.eData.submitVote("bristaco", "husky", "gompei");
            this.eData.submitVote("bristaco", "husky", "gompei");
            this.eData.submitVote("bristaco", "husky", "gompei");


        }
        catch(Exception e){
            fail(e.getMessage());
        }

        //try catch says that test will immediately fail if any of the functions in the try part are bad
        assertEquals(Optional.of("bristaco"), this.eData.calculateWinner());
    }

        @Test
        public void testOneVote(){
            try {
                this.eData.nominateCandidate("gompei");
                this.eData.nominateCandidate("brin");
                this.eData.nominateCandidate("hu");
                this.eData.submitVote("gompei", "brin", "hu");
            }
            catch(Exception e){
                fail(e.getMessage());
            }
            //try catch says that test will immediately fail if any of the functions in the try part are bad
            assertEquals(Optional.of("gompei"), this.eData.calculateWinner());
        }


        @Test(expected=AlreadyNominatedException.class)
          public void testCandidateAlreadyNominated() throws AlreadyNominatedException{
            this.eData.nominateCandidate("gompei");
             this.eData.nominateCandidate("gompei");

             fail("did not throw exception properly");
        }

        @Test (expected = CandidateNotNominatedException.class)
        public void testCandidatesNotNominated() throws CandidateNotNominatedException,AlreadyNominatedException,MoreThanOnceException{
            this.eData.nominateCandidate("gompei");
            this.eData.nominateCandidate("husky");
            this.eData.nominateCandidate("bristaco");
            this.eData.submitVote("gompei1", "mark", "bristaco");

             fail("did not throw exception properly");
        }

    @Test (expected = MoreThanOnceException.class)
    public void testCandidatesNominatedMoreThanOnce() throws CandidateNotNominatedException,AlreadyNominatedException,MoreThanOnceException{
        this.eData.nominateCandidate("gompei");
        this.eData.nominateCandidate("husky");
        this.eData.nominateCandidate("bristaco");
        this.eData.submitVote("gompei", "gompei", "bristaco");

        fail("did not throw exception properly");
    }

// testing a simply majority
    @Test
    public void testSimpleMajority(){
        try {
            this.eData.nominateCandidate("gompei");
            this.eData.nominateCandidate("husky");
            this.eData.nominateCandidate("bristaco");
            this.eData.nominateCandidate("bro");
            this.eData.nominateCandidate("brooo");

            this.eData.submitVote("husky", "bro", "bristaco");
            this.eData.submitVote("husky", "bro", "bristaco");
            this.eData.submitVote("gompei", "husky", "bristaco");
            this.eData.submitVote("gompei", "husky", "bristaco");
            this.eData.submitVote("gompei", "bro", "brooo");
            this.eData.submitVote("bro", "brooo", "bristaco");
            this.eData.submitVote("bro", "brooo", "bristaco");

        }
        catch(Exception e){
            fail(e.getMessage());
        }
        //try catch says that test will immediately fail if any of the functions in the try part are bad
        assertEquals(Optional.empty(), this.eData.calculateWinner());
    }

    @Test
    public void testSimpleMajority2(){
        try {
            this.eData.nominateCandidate("gompei");
            this.eData.nominateCandidate("husky");
            this.eData.nominateCandidate("bristaco");

            this.eData.submitVote("husky", "gompei", "bristaco");
            this.eData.submitVote("gompei", "husky", "bristaco");

        }
        catch(Exception e){
            fail(e.getMessage());
        }
        //try catch says that test will immediately fail if any of the functions in the try part are bad
        assertEquals(Optional.empty(), this.eData.calculateWinner());
    }



    @Test
    public void testGetCandidate(){
        try {
            this.eData.nominateCandidate("gompei");
            this.eData.nominateCandidate("husky");
            this.eData.nominateCandidate("bristaco");


            this.eData.submitVote("husky", "gompei", "bristaco");
            this.eData.submitVote("husky", "gompei", "bristaco");
            this.eData.submitVote("gompei", "husky", "bristaco");
        }
        catch(Exception e){
            fail(e.getMessage());
        }

        //try catch says that test will immediately fail if any of the functions in the try part are bad
        assertEquals(Set.of("bristaco", "husky", "gompei"), this.eData.getCandidates());
    }



    //testing making deep copy of hash map
    @Test
    public void testGetCandidate2(){

        try {
            this.eData.nominateCandidate("gompei");
            this.eData.nominateCandidate("husky");
            this.eData.nominateCandidate("bristaco");

        }
        catch(Exception e){
            fail(e.getMessage());
        }
        eData.getCandidates().removeAll(eData.getCandidates());

        //try catch says that test will immediately fail if any of the functions in the try part are bad
        assertEquals(Set.of("bristaco", "husky", "gompei"), this.eData.getCandidates());
    }


    }
