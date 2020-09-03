package muncala;

import java.util.Scanner;
import static muncala.interfaces.Util.*;

public class Test {

    static void mainTestFunction() {
        testConsole();
//        testRepeat();
    }

    private static void testConsole() {
        Scanner sc = new Scanner(System.in);
        int idx = -1;
        GameState state = GameState.getInitialState();
        state.setTurn(PLAYER_1);
//        Strategies max = new MiniMax(PLAYER_1);
//        Strategies min = new MiniMax(PLAYER_2);
        state.printBoard();
        while (true) {
            System.out.println("Turn of " + state.getWhoseTurn() + " , enter idx to move: ");
            idx = sc.nextInt();
            state.move_By_Changing_State_with_idx(idx);
            state.printBoard();
            if (state.isGameOver()) {
                break;
            }
        }

    }

    private static void testRepeat() {
        int[]gameBoard = {4, 4, 0, 5, 5, 5, 1, 4, 4, 4, 4, 4, 4, 0};
        GameState state = new GameState(gameBoard);
        state.setTurn(PLAYER_1);
        state.printBoard();
        state.move_By_Changing_State_with_idx(5);
        state.printBoard();
    }

}
