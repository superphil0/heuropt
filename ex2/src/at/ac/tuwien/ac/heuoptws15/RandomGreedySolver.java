package at.ac.tuwien.ac.heuoptws15;

import java.util.LinkedList;
import java.util.List;

/**
 * Created by Philipp on 25.10.2015.
 */
public class RandomGreedySolver extends Solver {
    public RandomGreedySolver(KPMPInstance instance) {
        super(instance);
    }

    public void solve() {
        getOrderedVertices();
        for (int i = 0; i < instance.getNumVertices(); i++) {
            spineOrder[i] = ordering.get(i);
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
            int a = lookOrder[i] / n;
            int b = lookOrder[i] % n;
            if (matrix[a][b]) {
                insert(new Edge(getVertexPos(a), getVertexPos(b)), k);
            }
        }
        //System.out.println("Crossing number will be at least " + edgeNotInPartition.size());
        // distribute them evenly
        for (Edge e : edgeNotInPartition) {
            /*int argmin = 0;
            int min = Integer.MAX_VALUE;
            for (int i : candidates) {
                int heur = WouldIncreaseBy(e, i);
                if (heur < min) {
                    min = heur;
                    argmin = i;
                }
            }*/
            int selection = selectCandidate(candidates);
            edgePartition.get(selection).add(e);
            for (Edge b : edgePartition.get(selection)) {
                if (doEdgesCross(e, b)) {
                    crossings++;
                }
            }
        }
    }

    public int WouldIncreaseBy(Edge e, int page) {
        int before = getNumberOfCrossingsOnPage(page);
        edgePartition.get(page).add(e);
        int after = getNumberOfCrossingsOnPage(page);
        edgePartition.get(page).remove(e);
        return after - before;
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
            int selection = selectCandidate(candidates); //candidates.get(0);//
            edgePartition.get(selection).add(edge);
        }

    }

    protected int[] getIndexArray(int n) {
        int[] array = new int[n];
        for (int i = 0; i < n; i++) {
            array[i] = i;
        }

        // new order
        /*for (int i = n - 1; i > 0; i--) {
            int j = (int) Math.floor(Math.random() * (i + 1));
            int tmp = array[j];
            array[j] = array[i];
            array[i] = tmp;
        }*/
        return array;
    }

}
