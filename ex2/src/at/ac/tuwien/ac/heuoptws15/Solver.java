package at.ac.tuwien.ac.heuoptws15;

import com.sun.deploy.util.ArrayUtil;

import java.util.*;

/**
 * Created by Philipp on 25.10.2015.
 */
public abstract class Solver {
    protected final KPMPInstance instance;
    protected int[] spineOrder;
    protected int numberOfEdgeInGraph;
    protected HashMap<Integer, List<Edge>> edgePartition;
    protected int[] vertexCrossingsCount;
    protected List<Edge> edgeNotInPartition;
    public int crossings = 0;
    private final boolean[] marked;
    private int count = 0;
    protected TreeMap<Integer, Integer> ordering;
    public Solver(KPMPInstance instance) {
        this.instance = instance;
        spineOrder = new int[instance.getNumVertices()];
        edgePartition = new HashMap<>();
        edgeNotInPartition = new ArrayList<>();
        numberOfEdgeInGraph = instance.NumberOfEdges();
        vertexCrossingsCount = new int[instance.getNumVertices()];
        marked = new boolean[instance.getNumVertices()];
        ordering = new TreeMap<>();
    }

    public List<Integer> getOrdering() {
        LinkedList<Integer> list = new LinkedList<>();
        for(int i = 0; i < spineOrder.length; i++)
        {
            list.add(spineOrder[i]);
        }
        return list;
    }

    public HashMap<Integer, List<Edge>> getEdgePartition() {
        return edgePartition;
    }

    public abstract void solve();


    protected int selectCandidate(List<Integer> candidates) {
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


    protected int getVertexPos(int x) {
        return spineOrder[x];
    }

    protected boolean wouldCross(Edge e, int page) {
        List<Edge> ed = edgePartition.get(page);
        for (Edge edi : ed) {
            if (doEdgesCross(edi, e)) return true;
        }
        return false;
    }

    protected boolean doEdgesCross(Edge a, Edge b) {
        Edge small = a;
        Edge large = b;
        if (getVertexPos(large.A) < getVertexPos(small.A)) {
            Edge tmp = large;
            large = small;
            small = tmp;
        }
        return (getVertexPos(small.A) < getVertexPos(small.B) &&
                getVertexPos(large.A) < getVertexPos(large.B) &&
                getVertexPos(small.A) < getVertexPos(large.A) &&
                getVertexPos(small.B) < getVertexPos(large.B));
    }

    public int getNumberOfCrossings() {
        int sum = 0;
        for (int i = 0; i < instance.getK(); i++) {
            sum += getNumberOfCrossingsOnPage(i);
        }
        return sum;
    }

    public void countVertexCrossings() {
        getNumberOfCrossings();
        Arrays.fill(vertexCrossingsCount, 0);
        for (List<Edge> edge : edgePartition.values()) {
            for (Edge e : edge) {
            }
        }
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
    public int getNumberOfCrossingsOnPage(int page) {
        List<Edge> ed = edgePartition.get(page);
        int crossings = 0;
        for (Edge a : ed) {
            for (Edge b : ed) {
                if (a == b) continue;
                if (doEdgesCross(a, b)) {
                    crossings++;
                }
            }
        }
        return crossings;
    }

    public class Edge {
        public int A, B;

        public Edge(int a, int b) {
            A = a;
            B = b;

        }
    }
}
