package a_star_search;

import java.io.*;

public class Writer {

    public static String outputFile = "src/a_star_search/OutputFile.txt";
    public static String inputFile = "src/a_star_search/Input.txt";
    public static String analysisFile = "src/a_star_search/Analysis.txt";
    
    public static void eraseOutput() {
        try (FileWriter fw = new FileWriter(Writer.outputFile);
                BufferedWriter bw = new BufferedWriter(fw);
                PrintWriter out = new PrintWriter(bw)) {
            out.println("");

        } catch (IOException e) {
        }
    }

    public static void writeOutput(String s) {
        try (FileWriter fw = new FileWriter(Writer.outputFile, true);
                BufferedWriter bw = new BufferedWriter(fw);
                PrintWriter out = new PrintWriter(bw)) {
            out.println(s);

        } catch (IOException e) {
        }
    }

    public static void writeAnalysis(String s) {
        try (FileWriter fw = new FileWriter(Writer.analysisFile, true);
                BufferedWriter bw = new BufferedWriter(fw);
                PrintWriter out = new PrintWriter(bw)) {
            out.println(s + "\n");

        } catch (IOException e) {
        }
    }
    public static void eraseAnalysis() {
        try (FileWriter fw = new FileWriter(Writer.analysisFile);
                BufferedWriter bw = new BufferedWriter(fw);
                PrintWriter out = new PrintWriter(bw)) {
            out.println("");

        } catch (IOException e) {
        }
    }
}
