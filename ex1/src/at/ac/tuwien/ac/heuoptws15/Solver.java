package at.ac.tuwien.ac.heuoptws15;

import java.util.*;

/**
 * Created by Philipp on 25.10.2015.
 */
public class Solver {
    private final KPMPInstance instance;
    private List<Integer> spineOrder;
    private int numberOfEdgeInGraph;
    private HashMap<Integer, List<Edge>> edgePartition;
    private List<Edge> edgeNotInPartition;

    public Solver(KPMPInstance instance) {
        this.instance = instance;
        spineOrder = new LinkedList<>();
        edgePartition = new HashMap<>();
        edgeNotInPartition = new ArrayList<>();
        numberOfEdgeInGraph = instance.NumberOfEdges();
    }

    public List<Integer> getOrdering() {
        return spineOrder;
    }

    public HashMap<Integer, List<Edge>> getEdgePartition() {
        return edgePartition;
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
            int a = lookOrder[i] / n;
            int b = lookOrder[i] % n;
            if (matrix[a][b]) {
                insert(new Edge(getVertexPos(a), getVertexPos(b)), k);
            }
        }
        System.out.println("Crossing number will be at least " + edgeNotInPartition.size());
        // distribute them evenly
        for (Edge e : edgeNotInPartition) {
            int selection = selectCandidate(candidates);
            edgePartition.get(selection).add(e);
        }

    }

    private void insert(Edge edge, int k) {
        List<Integer> candidates = new LinkedList<>();

        for (int i = 0; i < k; i++) {
            if (!wouldCross(edge, i)) {
                candidates.add(i);
            }
        }
        if (candidates.size() == 0) {
            edgeNotInPartition.add(edge);
        } else {
            int selection = candidates.get(0);//selectCandidate(candidates);
            edgePartition.get(selection).add(edge);
        }

    }

    private int selectCandidate(List<Integer> candidates) {
        double[] props = new double[candidates.size()];
        int sum = 0;
        // so we will always choose a candidate + 1
        int expectedNodesPerPage = numberOfEdgeInGraph / instance.getK() + 1;
        for (int i = 0; i < candidates.size(); i++) {
            int difference = expectedNodesPerPage - edgePartition.get(candidates.get(i)).size();
            props[i] = difference;
            sum += props[i];
        }
        double rand = Math.random();
        int selection = candidates.get(0);
        for (int i = 0; i < candidates.size(); i++) {
            props[i] /= sum;
            double probSum = 0;
            for (int j = 0; j <= i; j++) {
                probSum += props[j];
            }
            if (rand < probSum) {
                selection = candidates.get(i);
                break;
            }
        }
        return selection;
    }

    private int[] getIndexArray(int n) {
        int[] array = new int[n];
        for (int i = 0; i < n; i++) {
            array[i] = i;
        }

        // new order
        for (int i = n - 1; i > 0; i--) {
            int j = (int) Math.floor(Math.random() * (i + 1));
            int tmp = array[j];
            array[j] = array[i];
            array[i] = tmp;
        }

        return array;

    }

    private int getVertexPos(int x) {
        return spineOrder.indexOf(x);
    }

    private boolean wouldCross(Edge e, int page) {
        List<Edge> ed = edgePartition.get(page);
        for (Edge edi : ed) {
            if (doEdgesCross(edi, e)) return true;
        }
        return false;
    }

    private boolean doEdgesCross(Edge a, Edge b) {
        Edge small = a;
        Edge large = b;
        if (large.A < small.A) {
            Edge tmp = large;
            large = small;
            small = tmp;
        }
        return (small.A < small.B && large.A < large.B && small.A < large.A && small.B < large.B);
    }

    public int getNumberOfCrossings() {
        int sum = 0;
        for (int i = 0; i < instance.getK(); i++) {
            sum += getNumberOfCrossingsOnPage(i);
        }
        return sum;
    }

    public int getNumberOfCrossingsOnPage(int page) {
        List<Edge> ed = edgePartition.get(page);
        int crossings = 0;
        for (Edge a : ed) {
            for (Edge b : ed) {
                if (a == b) continue;
                if (doEdgesCross(a, b)) crossings++;
            }
        }
        return crossings;
    }

    public class Edge {
        public int A, B;

        public Edge(int a, int b) {
            // check for smamller?
            A = a;
            B = b;
        }
    }
}
