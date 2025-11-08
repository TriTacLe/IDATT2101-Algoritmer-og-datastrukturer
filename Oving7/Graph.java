
/**
 * Kompakt adjacency-list med "head/next" (CSR-lignende). adjHead[u] peker til
 * første edgeIndex for u, neste kant finnes i next[edgeIndex], osv. Vi bygger
 * både forward (adj) og reverse (radj).
 */
public final class Graph {

    public final int n;                 // antall noder
    public final Node[] nodes;          // id==index
    public final int[] adjHead;         // størrelse n, init -1
    public final int[] adjTo;           // størrelse m
    public final int[] adjTime;         // størrelse m
    public final int[] adjNext;         // størrelse m

    public final int[] radjHead;
    public final int[] radjTo;
    public final int[] radjTime;
    public final int[] radjNext;

    Graph(int n, Node[] nodes,
            int[] adjHead, int[] adjTo, int[] adjTime, int[] adjNext,
            int[] radjHead, int[] radjTo, int[] radjTime, int[] radjNext) {
        this.n = n;
        this.nodes = nodes;
        this.adjHead = adjHead;
        this.adjTo = adjTo;
        this.adjTime = adjTime;
        this.adjNext = adjNext;
        this.radjHead = radjHead;
        this.radjTo = radjTo;
        this.radjTime = radjTime;
        this.radjNext = radjNext;
    }

    public Iterable<Edge> neighbors(int u) {
        return () -> new java.util.Iterator<>() {
            int e = adjHead[u];

            @Override
            public boolean hasNext() {
                return e != -1;
            }

            @Override
            public Edge next() {
                Edge ret = new Edge(adjTo[e], adjTime[e]);
                e = adjNext[e];
                return ret;
            }
        };
    }

    public Iterable<Edge> rneighbors(int u) {
        return () -> new java.util.Iterator<>() {
            int e = radjHead[u];

            @Override
            public boolean hasNext() {
                return e != -1;
            }

            @Override
            public Edge next() {
                Edge ret = new Edge(radjTo[e], radjTime[e]);
                e = radjNext[e];
                return ret;
            }
        };
    }
}
