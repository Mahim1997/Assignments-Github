package offline3;

import java.util.*;

public class Main {

    public static boolean isTESTING = true; //If true -> fixed indices set, else if false -> random indices for start
    public static int limit = 10;
    public static boolean allPrint = true;
    public static boolean randomInput = true;

    public static void main(String[] args) {

        Runner.runNearestInsertion();

        Runner.runCheapestInsertion();

        Runner.runNearestNeighbor();

        Runner.runImprove_2OPT(IConstructor.nearestNeighbor);

        Runner.runImprove_3OPT(IConstructor.nearestNeighbor);

    }

    static List<Point> generateRandomInput() {
        double n = 50,
                finalLimit = 1000,
                x, y;
        List<Point> list = new ArrayList<>();
        Point p;
        for (int i = 0; i < n; i++) {
            x = (Math.random() * finalLimit);
            y = (Math.random() * finalLimit);
            p = new Point(x, y, i);
            if (list.contains(p) == true) {
                continue;
            }
            list.add(p);
        }

        return list;
    }

}
