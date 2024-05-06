import java.util.Scanner;

public class VotingMachine {
    public VotingMachine(){
    }

    public static void main(String[] args) throws AlreadyNominatedException, MoreThanOnceException, CandidateNotNominatedException {
        ElectionData eData = new ElectionData(new MostFirstVotesStrategy());
        Scanner keyboard = new Scanner(System.in);
        System.out.println("Hello! Current candidates are: " + eData.getCandidates());

        boolean keepGoing = true;

        while (keepGoing) {

            System.out.println("Do you want to [n]ominate someone, [v]ote for someone, change winner [s]trategy, see who [w]on, or [q]uit?");

            if (keyboard.hasNext("q")) {
                System.out.println("You quit. Thanks for voting!");
                keepGoing = false;


            }
            else if (keyboard.hasNext("n")) {
                System.out.println("Who shall you be nominating?");
                keyboard.nextLine();
                String nominatedPerson = keyboard.nextLine();

                if (eData.getCandidates().contains(nominatedPerson)) {
                    System.out.println("This person was already nominated. Try again.");
                    continue;
                }
                eData.nominateCandidate(nominatedPerson);

                System.out.println("Current candidates are: " + eData.getCandidates());
            }


            else if (keyboard.hasNext("v")) {
                System.out.println("Enter your first vote.");
                keyboard.nextLine();
                String firstVote = keyboard.nextLine();
                if (!eData.getCandidates().contains(firstVote)) {
                    System.out.println(new CandidateNotNominatedException(firstVote).getMessage());
                    System.out.println("Would you like to nominate this candidate? Answer [y]es or [n]o.");
                    String yesNo = keyboard.nextLine();
                    if (yesNo.equals("y")) {
                        eData.nominateCandidate(firstVote);
                        continue;
                    } else {
                        continue;
                    }
                }

                System.out.println("Enter your second vote.");
                String secondVote = keyboard.nextLine();
                if (!eData.getCandidates().contains(secondVote)) {
                    System.out.println(new CandidateNotNominatedException(secondVote).getMessage());
                    System.out.println("Would you like to nominate this candidate? Answer [y]es or [n]o.");
                    String yesNo = keyboard.nextLine();
                    if (yesNo.equals("y")) {
                        eData.nominateCandidate(secondVote);
                        continue;
                    } else {
                        continue;
                    }
                }
                if(firstVote.equals(secondVote)){
                    System.out.println("You voted for a candidate more than once!");
                    continue;
                }

                System.out.println("Enter your third vote.");
                String thirdVote = keyboard.nextLine();
                if (!eData.getCandidates().contains(thirdVote)) {
                    System.out.println(new CandidateNotNominatedException(thirdVote).getMessage());
                    System.out.println("Would you like to nominate this candidate? Answer [y]es or [n]o.");
                    String yesNo = keyboard.nextLine();
                    if (yesNo.equals("y")) {
                        eData.nominateCandidate(thirdVote);
                        continue;
                    } else {
                        continue;
                    }
                }
                if(secondVote.equals(thirdVote) || firstVote.equals(thirdVote)){
                    System.out.println("You voted for a candidate more than once!");
                    continue;
                }

                eData.submitVote(firstVote, secondVote, thirdVote);

            }
             else if(keyboard.hasNext("s")){
                 System.out.println("Which strategy would you like to use? Most [f]irst votes or most [a]greeable?");
                 keyboard.nextLine();
                 String strategy = keyboard.nextLine();
                 if(strategy.contains("f")){
                     eData.setStrategy(new MostFirstVotesStrategy());
                 } else if(strategy.contains("a")){
                     eData.setStrategy(new MostAgreeableStrategy());
                 } else {
                     System.out.println("Invalid input. Retry.");
                 }
            }
             else if(keyboard.next().equals("w")){
                 if(eData.calculateWinner().isPresent()){
                     System.out.println("The winner is: " + eData.calculateWinner());
                 } else {
                     System.out.println("No clear winner under the current strategy.");
                 }
            }


        }

    }
    }


