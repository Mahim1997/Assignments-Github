package offline3;

import java.util.List;

class Runner {

    public static void runNearestNeighbor() {
        Loader.eraseOutput();
        List<Point> list = Loader.getInput();

        IConstructor constructor = null;
        constructor = Loader.getConstructor(IConstructor.nearestNeighbor, list);
        Loader.writeOutput("\n--------------------------------------------------------\n\n");
        Loader.writeOutput("Using Nearest Neighbor Construction Heuristic\n");
        for (int i = 0; i < Main.limit; i++) {
            constructor.formConstructor(list);
            constructor.constructTour();
            Loader.writeSolution(constructor.getTour());
        }
    }

    public static void runNearestInsertion() {
        Loader.eraseOutput();
        List<Point> list = Loader.getInput();

        IConstructor constructor = null;
        constructor = Loader.getConstructor(IConstructor.nearestInsertion, list);
        Loader.writeOutput("\n--------------------------------------------------------\n\n");
        Loader.writeOutput("Using Nearest Insertion Construction Heuristic\n");
        for (int i = 0; i < Main.limit; i++) {
            constructor.formConstructor(list);
            constructor.setIndex(i);
            constructor.constructTour();
            Loader.writeSolution(constructor.getTour());
        }
    }

    public static void runCheapestInsertion() {
        Loader.eraseOutput();
        List<Point> list = Loader.getInput();

        IConstructor constructor = null;
        constructor = Loader.getConstructor(IConstructor.cheapestInsertion, list);
        Loader.writeOutput("\n--------------------------------------------------------\n\n");
        Loader.writeOutput("Using Cheapest Insertion Construction Heuristic\n");
        for (int i = 0; i < Main.limit; i++) {
            constructor.formConstructor(list);
            constructor.setIndex(i);
            constructor.constructTour();
            Loader.writeSolution(constructor.getTour());
        }
    }

    public static void runImprove_2OPT(String type) {
        Loader.eraseOutput();
        List<Point> list = Loader.getInput();

        IConstructor constructor = null;
        constructor = Loader.getConstructor(type, list);

        Improvement_Heuristic improver = null;
        Loader.writeOutput("\n--------------------------------------------------------\n\n");
        Loader.writeOutput("Using 2 Opt Improvement Heuristic\n");
        for (int i = 0; i < Main.limit; i++) {
            constructor.formConstructor(list);
//            constructor.setIndex(5);    //TEST
            constructor.setIndex(i);
            constructor.constructTour();
//            Loader.writeSolution(constructor.getTour());
//            Loader.writeOutput("\n\nNOW USING 2 OPT HEURISTIC\n\n");
            improver = Loader.getImprover(Improvement_Heuristic.twoOPT, constructor.getTour());
            improver.improveTour();
            Loader.writeSolution(improver.getTour());
        }
    }

    public static void runImprove_3OPT(String type) {
        Loader.eraseOutput();
        List<Point> list = Loader.getInput();

        IConstructor constructor = null;
        constructor = Loader.getConstructor(type, list);

        Improvement_Heuristic improver = null;
        Loader.writeOutput("\n--------------------------------------------------------\n\n");
        Loader.writeOutput("Using 3 Opt Improvement Heuristic\n");
        for (int i = 0; i < Main.limit; i++) {
            constructor.formConstructor(list);
//            constructor.setIndex(5);    //TEST
            constructor.setIndex(i);
            constructor.constructTour();
//            Loader.writeSolution(constructor.getTour());
//            Loader.writeOutput("\n\nNOW USING 2 OPT HEURISTIC\n\n");
            improver = Loader.getImprover(Improvement_Heuristic.threeOPT, constructor.getTour());
            improver.improveTour();
            Loader.writeSolution(improver.getTour());
        }
    }
}
/*
1 2
5 3
4 5
2 7
3 9
8 3
1 9
5 8
6 7
9 2
*/
