import java.util.*;

public class Queens {

    List<List<Integer>> allSolutions = new ArrayList<>();

    /**
     * recurses until it finds the first solution given an n number of queens
     * @param board     the current board
     * @param numQueens the number of queens to solve for
     * @return the first solution
     */
    public List<Integer> solution(List<Integer> board, int numQueens) {
        List<Integer> nextBoard = nextLegalPosition(board, numQueens);
        // solution has been found if it's a legal position and the last index of the board isn't empty
        boolean solutionFound = isLegalPosition(board, numQueens) && board.get(board.size() - 1) != 0;
        if (solutionFound) {
            return board;
        }
        // otherwise recurse
        else {
            return solution(nextBoard, numQueens);
        }
    }

    /**
     * counts the number of solutions for given an n number of queens
     * @param board      the current board
     * @param numQueens  the number of queens to solve for
     * @param currentRow the current row we are placing a queen in
     */
    public void solutionCount(List<Integer> board, int numQueens, int currentRow) {
        // if all queens have been placed, solution has been found
        if (currentRow == numQueens) {
            allSolutions.add(new ArrayList<>(board));
            return;
        }
        // tries placing a queen in each column of the current row
        for (int col = 1; col <= numQueens; col++) {
            board.set(currentRow, col);
            if (isLegalPosition(board, numQueens)) {
                // continue to next row if placement was legal
                solutionCount(board, numQueens, currentRow + 1);
            }
            // clears current position
            board.set(currentRow, 0);
        }
    }

    /**
     * finds the next legal position from a legal, illegal or solution position
     * @param board     the current board
     * @param numQueens the number of queens to solve for
     * @return the next legal position
     */
    public List<Integer> nextLegalPosition(List<Integer> board, int numQueens) {
        List<Integer> nextBoard = nextPosition(new ArrayList<>(board), numQueens);
        // if it's a legal position and the last index of the board is filled (AKA we have a solution)
        if (isLegalPosition(nextBoard, numQueens) && board.get(board.size() - 1) != 0) {
            // sets the row index to the last row of the board
            int lastQueenRowIndex = numQueens - 1;

            // while the row index is valid
            while (lastQueenRowIndex >= 0) {
                // sets the current row to zero and decrements row index so that it looks at previous row
                nextBoard.set(lastQueenRowIndex, 0);
                lastQueenRowIndex--;
                // if the current row is valid and the last queen isn't in the last column of the given row
                if (lastQueenRowIndex >= 0 && nextBoard.get(lastQueenRowIndex) < numQueens) {
                    // tries placing the queen in each subsequent column in that row, up to the last column
                    for (int col = nextBoard.get(lastQueenRowIndex) + 1; col <= numQueens; col++) {
                        nextBoard.set(lastQueenRowIndex, col);
                        // returns that position if it is legal
                        if (isLegalPosition(nextBoard, numQueens)) {
                            return nextBoard;
                        }
                    }
                }
            }
        }
        // checks if there is a legal position (case where we don't have a full solution yet)
        if (isLegalPosition(nextBoard, numQueens)) {
            return nextBoard;
        } else {
            // finds the row index of the queen that was just placed in the new row
            int lastQueenRowIndex = 0;
            for (int i = 0; i < numQueens; i++) {
                if (nextBoard.get(i) != 0) {
                    lastQueenRowIndex = i;
                }
            }
            // checks that rows don't go out of bounds
            while (lastQueenRowIndex != -1) {
                // tries placing the queen in that row in each column
                for (int col = nextBoard.get(lastQueenRowIndex) + 1; col <= numQueens; col++) {
                    nextBoard.set(lastQueenRowIndex, col);
                    // if legal, returns that board
                    if (isLegalPosition(nextBoard, numQueens)) {
                        return nextBoard;
                    }
                }
                // if not, backtracks to previous queen
                nextBoard.set(lastQueenRowIndex, 0);
                lastQueenRowIndex--;
            }
        }
        return nextBoard;
    }

    /**
     * finds the next position, regardless of whether it is legal
     * @param board     the current board
     * @param numQueens the number of queens to solve for
     * @return the next position
     */
    public List<Integer> nextPosition(List<Integer> board, int numQueens) {
        int lastQueenRowIndex = 0;
        // updates the last queen index until it finds an empty value or reaches end of board
        for (int i = 0; i < numQueens; i++) {
            if (board.get(i) != 0) {
                lastQueenRowIndex = i;
            }
        }
        // if first row is empty
        if (board.get(lastQueenRowIndex) == 0) {
            board.set(lastQueenRowIndex, 1);
        }
        // simply adds queen to new row if there are rows left (if first row isn't empty)
        else if (lastQueenRowIndex + 1 < numQueens) {
            board.set(lastQueenRowIndex + 1, 1);
        }
        return board;
    }

    /**
     * determines if a position is legal
     * @param board     the current board
     * @param numQueens the number of queens to solve for
     * @return true if it's legal, false otherwise
     */
    public boolean isLegalPosition(List<Integer> board, int numQueens) {
        // loops for each column
        for (int col = 0; col < numQueens; col++) {
            // keeps track of queens found in each column
            int queensFound = 0;
            // goes through each row in a given column
            for (int row = 0; row < numQueens; row++) {
                // if there is a queen in that column, update the counter
                if (board.get(row) == col + 1) {
                    queensFound++;
                    // if there is more than one queen in the column, position is illegal
                    if (queensFound > 1) {
                        return false;
                    }
                }
            }
        }
        // loops for each element in the row
        for (int i = 0; i < numQueens; i++) {
            // skips empty positions
            if (board.get(i) == 0) {
                continue;
            }
            // loops through each subsequent element in the row
            for (int j = i + 1; j < numQueens; j++) {
                // skips empty positions
                if (board.get(j) == 0) {
                    continue;
                }
                // compares the difference of the row indices with the difference of the column indices
                // if equal, queens are in the same diagonal
                if (Math.abs(i - j) == Math.abs(board.get(i) - board.get(j))) {
                    return false;
                }
            }
        }
        return true;
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        Queens q = new Queens();

        /*List<Integer> initialPosition = new ArrayList<>(List.of(1, 6, 8, 3, 5, 0, 0, 0));
        System.out.println("The initial position is: " + initialPosition);
        boolean answer1 = q.isLegalPosition(initialPosition, 8);
        System.out.println("Is legal: " + answer1);
        List<Integer> nextPosition = q.nextPosition(initialPosition, 8);
        System.out.println("Next general position: " + nextPosition);
        List<Integer> nextLegalPosition = q.nextLegalPosition(initialPosition, 8);
        System.out.println("Next legal position: " + nextLegalPosition);
        System.out.println();

        List<Integer> initialPosition2 = new ArrayList<>(List.of(1, 6, 8, 3, 7, 0, 0, 0));
        System.out.println("The initial position is: " + initialPosition2);
        boolean answer2 = q.isLegalPosition(initialPosition2, 8);
        System.out.println("Is legal: " + answer2);
        List<Integer> nextPosition2 = q.nextPosition(initialPosition2, 8);
        System.out.println("Next general position: " + nextPosition2);
        List<Integer> nextLegalPosition2 = q.nextLegalPosition(initialPosition2, 8);
        System.out.println("Next legal position: " + nextLegalPosition2);
        System.out.println();


        List<Integer> initialPosition3 = new ArrayList<>(List.of(1, 6, 8, 3, 7, 4, 2, 5));
        System.out.println("The initial position is: " + initialPosition3);
        boolean answer3 = q.isLegalPosition(initialPosition3, 8);
        System.out.println("Is legal: " + answer3);
        List<Integer> nextPosition3 = q.nextPosition(initialPosition3, 8);
        System.out.println("Next general position: " + nextPosition3);
        List<Integer> nextLegalPosition3 = q.nextLegalPosition(initialPosition3, 8);
        System.out.println("Next legal position: " + nextLegalPosition3);
        System.out.println();


        List<Integer> initialPosition4 = new ArrayList<>(List.of(1, 3, 0, 0));
        System.out.println("The initial position is: " + initialPosition4);
        boolean answer4 = q.isLegalPosition(initialPosition3, 4);
        System.out.println("Is legal: " + answer4);
        List<Integer> nextPosition4 = q.nextPosition(initialPosition4, 4);
        System.out.println("Next general position: " + nextPosition4);
        List<Integer> nextLegalPosition4 = q.nextLegalPosition(initialPosition4, 4);
        System.out.println("Next legal position: " + nextLegalPosition4);
        System.out.println();*/

        /*List<Integer> test = new ArrayList<>(List.of(0,0,0,0,0,0,0,0));
        List<Integer> result = q.solution(test, 8);
        System.out.println(result);*/

        /*List<Integer> test = new ArrayList<>(List.of(0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
        q.solution(test, 10, 0);
        System.out.println(q.allSolutions.size());
        System.out.println(q.allSolutions.get(0));*/

        System.out.println("Would you like to [a] test the isLegal/nextLegal functions or [b] count and print solutions to N queens?");
        String choice = sc.nextLine();
        if (choice.equals("a")) {
            System.out.println("How many queens do you want to test?");
            int numQueens = sc.nextInt();
            List<Integer> initialPosition = new ArrayList<>();
            System.out.println("Enter a possible solution with " + numQueens + " slots.");
            for (int i = 0; i < numQueens; i++) {
                if (sc.hasNextInt()) {
                    initialPosition.add(sc.nextInt());
                }
            }
            System.out.println("The initial position is: " + initialPosition);
            boolean answer = q.isLegalPosition(initialPosition, numQueens);
            if (answer) {
                System.out.println("This is a legal position.");
            } else {
                System.out.println("This is not a legal position.");
            }

            List<Integer> nextLegalPosition = q.nextLegalPosition(initialPosition, numQueens);
            System.out.println("Next legal position: " + nextLegalPosition);
        }

        if (choice.equals("b")) {
            for (int numQueens = 4; numQueens <= 12; numQueens++) {
                List<Integer> board = new ArrayList<>(Collections.nCopies(numQueens, 0));
                q.allSolutions.clear();
                q.solutionCount(board, numQueens, 0);
                System.out.println("There are " + q.allSolutions.size() + " solutions to the " + numQueens + "-Queens problem");
            }
            for (int numQueens = 4; numQueens <= 14; numQueens++) {
                List<Integer> board = new ArrayList<>(Collections.nCopies(numQueens, 0));
                List<Integer> answer = q.solution(board, numQueens);
                System.out.println("The first solution for the " + numQueens + "-Queens problem is: " + answer);
            }
            System.out.println("EXTRA CREDIT: ");
            List<Integer> fifteenQueens = new ArrayList<>(Collections.nCopies(15, 0));
            q.allSolutions.clear();
            q.solutionCount(fifteenQueens, 15, 0);
            System.out.println("There are " + q.allSolutions.size() + " solutions to the 15-Queens problem");
        }
    }
}

