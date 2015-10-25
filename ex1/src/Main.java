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
        for (int i = 0; i < 30; i++) {
            Solver solver = getSolution(inst);
            KPMPSolutionWriter writer = new KPMPSolutionWriter(inst.getK());
            writer.setEdges(solver.getEdgePartition());
            writer.setSpineOrder(solver.getOrdering());
            File f = new File(path);
            writer.write("solution/" + f.getName());
            //writer.print();
        }
    }

    private static Solver getSolution(KPMPInstance instance) {
        Solver solver = new OtherSolver(instance);
        solver.solve();
        System.out.println("Crossings are: " + solver.getNumberOfCrossings());
        return solver;
    }
}
