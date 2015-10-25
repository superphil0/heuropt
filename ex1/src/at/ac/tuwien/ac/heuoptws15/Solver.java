package at.ac.tuwien.ac.heuoptws15;

import java.util.*;

/**
 * Created by Philipp on 25.10.2015.
 */
public abstract class Solver {
    protected final KPMPInstance instance;
    protected List<Integer> spineOrder;
    protected int numberOfEdgeInGraph;
    protected HashMap<Integer, List<Edge>> edgePartition;
    protected List<Edge> edgeNotInPartition;

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
        return spineOrder.indexOf(x);
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
