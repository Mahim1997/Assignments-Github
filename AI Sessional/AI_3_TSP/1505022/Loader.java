package offline3;

import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Loader {

    public static String outputFile = "src/offline3/OutputFile.txt";
    public static String inputFile = "src/offline3/Input.txt";
    public static String analysisFile = "src/offline3/Analysis.txt";

    public static void eraseOutput() {
        try (FileWriter fw = new FileWriter(Loader.outputFile);
                BufferedWriter bw = new BufferedWriter(fw);
                PrintWriter out = new PrintWriter(bw)) {
            out.println("");

        } catch (IOException e) {
        }
    }

    public static void writeOutput(String s, boolean flag) {
        try (FileWriter fw = new FileWriter(Loader.outputFile, true);
                BufferedWriter bw = new BufferedWriter(fw);
                PrintWriter out = new PrintWriter(bw)) {
            out.print(s);
            System.out.print(s);
            if (flag == true) {
                out.println();
                System.out.println();
            }

        } catch (IOException e) {
        }
    }

    public static void writeOutput(String s) {
        try (FileWriter fw = new FileWriter(Loader.outputFile, true);
                BufferedWriter bw = new BufferedWriter(fw);
                PrintWriter out = new PrintWriter(bw)) {
            out.println(s);
            System.out.println(s);

        } catch (IOException e) {
        }
    }

    public static List<Point> getInput() {
        File file = new File(inputFile);
        Scanner sc = null;
        if(Main.randomInput == true){
            return Main.generateRandomInput();
        }
        try {
            sc = new Scanner(file);

            return readFromFile(sc);

        } catch (FileNotFoundException ex) {
            sc = new Scanner(System.in);

            return readFromConsole(sc);
        }
    }

    private static List<Point> readFromConsole(Scanner sc) {
        int n = -1;
        if (sc.hasNext()) {
            n = sc.nextInt();
        }
        int i = 0;
        List<Point> list = new ArrayList<>();
        while (sc.hasNext() && (i < n)) {
            double x, y;
            x = sc.nextDouble();
            y = sc.nextDouble();
            Point p = new Point(x, y, i);
            list.add(p);
            i++;
        }
        return list;
    }

    private static List<Point> readFromFile(Scanner sc) {
        List<Point> list = new ArrayList<>();

        int i = 0;
        while (sc.hasNext() == true) {

            double x = sc.nextDouble();
            double y = sc.nextDouble();
            Point point = new Point(x, y, i);
            i++;
            list.add(point);
        }

        return list;
    }

    public static Improvement_Heuristic getImprover(String type, List<Point> tour) {
        if (type.equals(Improvement_Heuristic.twoOPT)) {
            return new Two_Opt(tour);
        } else if (type.equals(Improvement_Heuristic.threeOPT)) {
            return new Three_Opt(tour);
        }
        return null;
    }

    public static IConstructor getConstructor(String type, List<Point> list) {
        if (type.equals("NearestNeighbour")) {
            return new NearestNeighbour(list);
        } else if (type.equals("NearestInsertion")) {
            return new Nearest_Insertion(list);
        } else if (type.equals("CheapestInsertion")) {
            return new Cheapest_Insertion(list);
        }
        return null;
    }

    public static void writeSolution(List<Point> tour) {
        String str = getSolution(tour, Main.allPrint);
        Loader.writeOutput(str);
    }

    public static double getCost(List<Point> tour) {
        double cost = 0;
        for (int i = 0; i < (tour.size() - 1); i++) {
            cost += (Point.getEucladianDistance(tour.get(i), tour.get(i + 1)));
        }
        return cost;
    }

    public static double getCostWithoutRepeat(List<Point> tour) {
        double cost = 0;
        for (int i = 0; i < (tour.size() - 1); i++) {
            cost += (Point.getEucladianDistance(tour.get(i), tour.get(i + 1)));
        }
        cost += (Point.getEucladianDistance(tour.get(tour.size() - 1), tour.get(0)));   //Add last vertex to the initial vertex.
        return cost;
    }

    public static String getSolution(List<Point> tour, boolean flag) {
        String str = "";
        double cost = Loader.getCost(tour);
        str += ("Printing Cost of tour: " + String.valueOf(cost));
        if (flag == true) {
            str += ("\n[Using indices]: ");
//            str += ("\nPrinting the tour [using indices]: ");
            for (int i = 0; i < tour.size(); i++) {
                str += (tour.get(i).getIndex() + " ");
                if (i != tour.size() - 1) {
                    str += ("-> ");
                }
            }
//            str += ("\nPrinting the tour [using points]: \n");
            str += ("\n[Using points]: \n");
        } else {
            str += ("\nPrinting the tour : \n");
        }
        for (int i = 0; i < tour.size(); i++) {
            str += (tour.get(i).getPoint() + " ");
            if (i != tour.size() - 1) {
                str += ("-> ");
            }
//            str += "\n";
        }
        str += "\n";
        return str;
    }

}
