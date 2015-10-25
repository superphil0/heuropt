import at.ac.tuwien.ac.heuoptws15.KPMPInstance;
import at.ac.tuwien.ac.heuoptws15.KPMPSolutionWriter;
import at.ac.tuwien.ac.heuoptws15.Solver;

import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;

public class Main {

    public static void main(String[] args) throws FileNotFoundException {
        System.out.println(System.getProperty("user.dir"));
        KPMPInstance inst = KPMPInstance.readInstance(args[0]);
        KPMPSolutionWriter writer = new KPMPSolutionWriter(inst.getK());
        initInst(inst, writer);

        for (int a = 0; a < inst.getNumVertices(); a++) {
            for (int b : inst.getAdjacencyList().get(a)) {
                if (b >= a) continue;

                writer.addEdgeOnPage(a, b, 0);
            }
        }

        //writer.print();
    }

    private static void initInst(KPMPInstance instance, KPMPSolutionWriter writer) {
        Solver solver = new Solver(instance);
        solver.solve();
        System.out.println("Crossings are: " + solver.getNumberOfCrossings());
        List<Integer> spineOrder = new ArrayList<>();

        writer.setSpineOrder(spineOrder);
    }
}
