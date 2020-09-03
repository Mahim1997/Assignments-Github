package problem1;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

public class Writer {
    public static void eraseOutput(String s) {
        try (FileWriter fw = new FileWriter(Initializer.outputFile);
                BufferedWriter bw = new BufferedWriter(fw);
                PrintWriter out = new PrintWriter(bw)) {
            out.println(s);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public static void writeOutput(String s) {
        try (FileWriter fw = new FileWriter(Initializer.outputFile, true);
                BufferedWriter bw = new BufferedWriter(fw);
                PrintWriter out = new PrintWriter(bw)) {
            out.println(s);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void writeComparison(String s) {
        try (FileWriter fw = new FileWriter(Initializer.compareFile, true);
                BufferedWriter bw = new BufferedWriter(fw);
                PrintWriter out = new PrintWriter(bw)) {
            out.println(s);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void eraseComparison() {
        try (FileWriter fw = new FileWriter(Initializer.compareFile);
                BufferedWriter bw = new BufferedWriter(fw);
                PrintWriter out = new PrintWriter(bw)) {
            out.print("");
            String s = "Comparing for DFS and BFS\n\n";
            out.println(s);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void writeToBFS(String s) {
        try (FileWriter fw = new FileWriter(Initializer.bfsFileName, true);
                BufferedWriter bw = new BufferedWriter(fw);
                PrintWriter out = new PrintWriter(bw)) {
            out.println(s);
            System.out.println(s);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void eraseBFSFile() {
        try (FileWriter fw = new FileWriter(Initializer.bfsFileName);
                BufferedWriter bw = new BufferedWriter(fw);
                PrintWriter out = new PrintWriter(bw)) {
            out.print("");
            String s = "Our format is <numberCannibalsLeft, numberMissionariesLeft, BoatDirection, numMissionariesRight, numCannibalsRight>\n";
            s += "==========================<<<<This solution is for BFS>>>>>============================\n\n\n";
            out.println(s);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void eraseDFSFile() {
        try (FileWriter fw = new FileWriter(Initializer.dfsFileName);
                BufferedWriter bw = new BufferedWriter(fw);
                PrintWriter out = new PrintWriter(bw)) {
            out.print("");
            String s = "Our format is <numberCannibalsLeft, numberMissionariesLeft, BoatDirection, numMissionariesRight, numCannibalsRight>\n";
            s += "==========================<<<<This solution is for DFS>>>>>============================\n\n\n";
            out.println(s);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void writeToDFS(String s) {
        try (FileWriter fw = new FileWriter(Initializer.dfsFileName, true);
                BufferedWriter bw = new BufferedWriter(fw);
                PrintWriter out = new PrintWriter(bw)) {
            out.println(s);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

 
}
