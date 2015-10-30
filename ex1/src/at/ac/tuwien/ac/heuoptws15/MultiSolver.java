package at.ac.tuwien.ac.heuoptws15;

import java.util.*;

/**
 * Created by Philipp on 25.10.2015.
 */
public class MultiSolver extends Solver {


    private final boolean[] marked;
    private int count = 0;
    private TreeMap<Integer, Integer> ordering;

    public MultiSolver(KPMPInstance instance) {
        super(instance);
        ordering = new TreeMap<>();
        marked = new boolean[instance.getNumVertices()];
    }

    public void solve() {
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
            //System.out.println("at edge "+ i + " from " + n*n);
            int a = lookOrder[i] / n;
            int b = lookOrder[i] % n;
            if (matrix[a][b]) {
                insert(new Edge(a, b), k);
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
        optimizeNodes();
    }

    private void optimizeNodes() {
        getOrderedVertices();
    }

    private void depthFirstSearch(int v) {
        marked[v] = true;
        ordering.put(v, count);
        count++;
        for (int w : instance.getAdjacencyList().get(v)) {
            if (!marked[w]) {
                depthFirstSearch(w);
            }
        }
    }


    public void getOrderedVertices() {
        ArrayList<Integer> vertices = new ArrayList<>();
        for (int v = 0; v < instance.getNumVertices(); v++) {
            vertices.add(v);
        }
        Collections.shuffle(vertices);

        for (int v : vertices) {
            if (!marked[v]) {
                depthFirstSearch(v);
            }
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
            candidates.add(i);
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
