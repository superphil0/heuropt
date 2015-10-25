package at.ac.tuwien.ac.heuoptws15;

import java.util.LinkedList;
import java.util.List;

/**
 * Created by Philipp on 25.10.2015.
 */
public class DeterministicSolver extends Solver {
    public DeterministicSolver(KPMPInstance instance) {
        super(instance);
    }

    public void solve() {
        for (int i = 0; i < instance.getNumVertices(); i++) {
            spineOrder.add(i);
        }
        int k = instance.getK();
        List<Integer> candidates = new LinkedList<>();
        for (int i = 0; i < k; i++) {
            candidates.add(i);
            edgePartition.put(i, new LinkedList<>());
        }

        boolean[][] matrix = instance.getAdjacencyMatrix();
        int n = instance.getNumVertices();
        int[] lookOrder = getIndexArray(n * n);
        for (int i = 0; i < n * n; i++) {
            //System.out.println("at edge "+ i + " from " + n*n);
            int a = lookOrder[i] / n;
            int b = lookOrder[i] % n;
            if (matrix[a][b]) {
                insert(new Edge(getVertexPos(a), getVertexPos(b)), k);
            }
        }
        // distribute them evenly
        int i = 0;
        for (Edge e : edgeNotInPartition) {
            i++;
            //System.out.println("at edge " + i + " from " + edgeNotInPartition.size());
            int selection = selector(candidates); // selectCandidate(candidates);
            edgePartition.get(selection).add(e);
        }
    }

    private int selector(List<Integer> candidates) {
        int best = 99999999;
        int argmin = 0;
        for (int i : candidates) {
            if (edgePartition.get(i).size() < best) {
                best = edgePartition.get(i).size();
                argmin = i;
            }
        }
        return argmin;
    }

    protected void insert(Edge edge, int k) {
        List<Integer> candidates = new LinkedList<>();

        for (int i = 0; i < k; i++) {
            if (!wouldCross(edge, i)) {
                candidates.add(i);
            }
        }
        if (candidates.size() == 0) {
            edgeNotInPartition.add(edge);
        } else {
            int selection = selector(candidates);
            edgePartition.get(selection).add(edge);
        }

    }

    protected int[] getIndexArray(int n) {
        int[] array = new int[n];
        for (int i = 0; i < n; i++) {
            array[i] = i;
        }
        return array;
    }
}
