import at.ac.tuwien.ac.heuoptws15.*;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class Main {

    public static void main(String[] args) throws IOException {
        System.out.println(System.getProperty("user.dir"));
        for (String s : args) {
            solveOne(s);
        }
    }

    public static void solveOne(String path) throws IOException {
        KPMPInstance inst = KPMPInstance.readInstance(path);
        int best = 999999999;
        int numRuns = 5;
        double mean = 0, sd = 0;
        KPMPSolutionWriter writer = null;
        int[] res = new int[numRuns];
        for (int i = 0; i < numRuns; i++) {
            System.out.println("start: ");
            Solver solver = getSolution(inst);
            System.out.println("start count: ");
            res[i] = solver.crossings;
            if (res[i] < best) {
                writer = new KPMPSolutionWriter(inst.getK());
                writer.setEdges(solver.getEdgePartition());
                writer.setSpineOrder(solver.getOrdering());
            }
            best = res[i] < best ? res[i] : best;
            mean += res[i];
            System.out.println(i);
        }
        File f = new File(path);
        writer.write("solution/" + f.getName());
        mean /= numRuns;
        for (int i = 0; i < numRuns; i++) {
            sd += Math.pow((res[i] - mean), 2);
        }
        sd = Math.sqrt(sd / numRuns);

        System.out.println("best for: " + path + " = " + best);
        System.out.println("mean: " + mean);
        System.out.println("sd: " + sd);
    }

    private static Solver getSolution(KPMPInstance instance) {
        Solver solver = new RandomGreedySolver(instance);
        solver.solve();
        return solver;
    }
}
