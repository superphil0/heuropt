package at.ac.tuwien.ac.heuoptws15;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;

/**
 * Created by Philipp on 25.10.2015.
 */
public class OtherSolver extends Solver {

    public OtherSolver(KPMPInstance instance) {
        super(instance);
    }

    @Override
    public void solve() {
        // init with normal order
        // init things
        int k = instance.getK();
        List<Integer> candidates = new LinkedList<>();
        for (int i = 0; i < k; i++) {
            candidates.add(i);
            edgePartition.put(i, new LinkedList<>());
        }
        // start inserting edges
        boolean[][] matrix = instance.getAdjacencyMatrix();
        int n = instance.getNumVertices();
        int[] lookOrder = getIndexArray(n * n);
        int best = 0;
        HashMap<Integer, List<Edge>> bestSol = null;
        List<Edge> bestNotSet = null;
        for (int j = 0; j < 300; j++) {
            spineOrder = new LinkedList<>();
            int[] array = getIndexArray(instance.getNumVertices());
            for (int i = 0; i < instance.getNumVertices(); i++) {
                spineOrder.add(array[i]);
            }
            for (int i = 0; i < n * n; i++) {
                int a = lookOrder[i] / n;
                int b = lookOrder[i] % n;
                if (matrix[a][b]) {
                    if (!insert(new Edge(getVertexPos(a), getVertexPos(b)), k) && best < i) {
                        best = i;
                    }
                }
            }
            bestSol = edgePartition;
            bestNotSet = edgeNotInPartition;
            edgePartition = new HashMap<>();
            edgeNotInPartition = new LinkedList<>();
            for (int c = 0; c < k; c++) {
                edgePartition.put(c, new LinkedList<>());
            }
        }
        edgePartition = bestSol;
        edgeNotInPartition = bestNotSet;
        System.out.println("Crossing number will be at least " + edgeNotInPartition.size());
        for (Edge e : edgeNotInPartition) {
            int selection = selectCandidate(candidates);
            edgePartition.get(selection).add(e);
        }
    }

    protected boolean insert(Edge edge, int k) {
        List<Integer> candidates = new LinkedList<>();

        for (int i = 0; i < k; i++) {
            if (!wouldCross(edge, i)) {
                candidates.add(i);
            }
        }
        if (candidates.size() == 0) {
            // change some nodes
            edgeNotInPartition.add(edge);
            return false;
        } else {
            int selection = selectCandidate(candidates);//
            edgePartition.get(selection).add(edge);
            return true;
        }

    }

    protected int[] getIndexArray(int n) {
        int[] array = new int[n];
        for (int i = 0; i < n; i++) {
            array[i] = i;
        }

        for (int i = n - 1; i > 0; i--) {
            int j = (int) Math.floor(Math.random() * (i + 1));
            int tmp = array[j];
            array[j] = array[i];
            array[i] = tmp;
        }
        return array;
    }
}
