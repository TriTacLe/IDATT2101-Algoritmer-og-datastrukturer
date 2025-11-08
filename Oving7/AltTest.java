
import java.io.*;
import java.util.*;

// ========== MAIN (eneste public) ==========
public class AltTest {

    public static void main(String[] args) throws Exception {
        // Stub: her kan dere etter hvert
        // 1) lese inn noder/kanter
        // 2) bygge Graph
        // 3) kjøre Dijkstra/ALT

        System.out.println("ALT skeleton loaded. Klar for integrasjon.");
    }
}

// ========== ALT PREPROCESS ==========
final class AltPreprocess {

    // Plug-in for SSSP (bruk kompisen sin Dijkstra når den er klar)
    interface Sssp {

        // returnerer distanser til alle noder, fra start; reversed=true => bruk reverse graph
        int[] run(int start, boolean reversed);
    }

    static AltData build(int n, int[] landmarks, Sssp sssp) {
        AltData D = new AltData(n, landmarks.length);
        for (int i = 0; i < landmarks.length; i++) {
            int lm = landmarks[i];
            int[] from = sssp.run(lm, false); // dist(ℓ -> *)
            int[] to = sssp.run(lm, true);  // dist(* -> ℓ) via reverse graph
            for (int v = 0; v < n; v++) {
                D.distFromLM[i * n + v] = from[v];
                D.distToLM[i * n + v] = to[v];
            }
            System.out.println("Landemerke " + lm + " ferdig.");
        }
        return D;
    }
}

// ========== ALT PATHFINDER (A*) ==========
final class AltPathfinder {

    static final class Result {

        final int[] prev;
        final int nodesPopped;
        final int goalDist;

        Result(int[] prev, int nodesPopped, int goalDist) {
            this.prev = prev;
            this.nodesPopped = nodesPopped;
            this.goalDist = goalDist;
        }
    }

    static AltPathfinder.Result runALT(Graph G, int s, int t, AltData D) {
        final int n = G.n;
        final int INF = Integer.MAX_VALUE;

        int[] dist = new int[n];
        int[] prev = new int[n];
        Arrays.fill(dist, INF);
        Arrays.fill(prev, -1);

        // (f, node) – f = g + h
        PriorityQueue<int[]> pq = new PriorityQueue<>(Comparator.comparingInt(a -> a[0]));
        int h0 = AltHeuristic.estimate(s, t, D);
        dist[s] = 0;
        pq.add(new int[]{safeAdd(dist[s], h0), s});
        int popped = 0;

        while (!pq.isEmpty()) {
            int[] cur = pq.poll();
            int f = cur[0], u = cur[1];
            popped++;

            if (u == t) {
                return new AltPathfinder.Result(prev, popped, dist[t]);
            }
            // Lazy-delete
            int hu = AltHeuristic.estimate(u, t, D);
            if (f != safeAdd(dist[u], hu)) {
                continue;
            }

            for (Edge e : G.neighbors(u)) {
                if (e.timeCentis < 0) {
                    continue;
                }
                int v = e.to;
                int nd = safeAdd(dist[u], e.timeCentis);
                if (nd < dist[v]) {
                    dist[v] = nd;
                    prev[v] = u;
                    int hv = AltHeuristic.estimate(v, t, D);
                    pq.add(new int[]{safeAdd(nd, hv), v});
                }
            }
        }
        return new AltPathfinder.Result(prev, popped, INF);
    }

    static int safeAdd(int a, int b) {
        if (a == Integer.MAX_VALUE || b == Integer.MAX_VALUE) {
            return Integer.MAX_VALUE;
        }
        long s = (long) a + (long) b;
        return s >= Integer.MAX_VALUE ? Integer.MAX_VALUE : (int) s;
    }
}

// ========== ALT HEURISTIKK ==========
final class AltHeuristic {

    static int estimate(int u, int t, AltData D) {
        int best = 0;
        int L = D.L;
        for (int lm = 0; lm < L; lm++) {
            int a = D.fromLM(t, lm); // dist(ℓ -> t)
            int b = D.fromLM(u, lm); // dist(ℓ -> u)
            int c = D.toLM(u, lm);   // dist(u -> ℓ)
            int d = D.toLM(t, lm);   // dist(t -> ℓ)

            int e1 = (a == Integer.MAX_VALUE || b == Integer.MAX_VALUE) ? 0 : Math.max(0, a - b);
            int e2 = (c == Integer.MAX_VALUE || d == Integer.MAX_VALUE) ? 0 : Math.max(0, c - d);
            int e = Math.max(e1, e2);
            if (e > best) {
                best = e;
            }
        }
        return best;
    }
}

// ========== ALT DATA (I/O) ==========
final class AltData {

    final int n;
    final int L;
    final int[] distFromLM; // length n*L
    final int[] distToLM;   // length n*L

    AltData(int n, int L) {
        this.n = n;
        this.L = L;
        this.distFromLM = new int[n * L];
        this.distToLM = new int[n * L];
        Arrays.fill(distFromLM, Integer.MAX_VALUE);
        Arrays.fill(distToLM, Integer.MAX_VALUE);
    }

    int fromLM(int node, int lm) {
        return distFromLM[lm * n + node];
    }

    int toLM(int node, int lm) {
        return distToLM[lm * n + node];
    }

    static void save(File file, AltData d) throws IOException {
        try (var out = new DataOutputStream(new BufferedOutputStream(new FileOutputStream(file)))) {
            out.writeBytes("ALT1");
            out.writeInt(d.n);
            out.writeInt(d.L);
            for (int v : d.distFromLM) {
                out.writeInt(v);
            }
            for (int v : d.distToLM) {
                out.writeInt(v);
            }
        }
    }

    static AltData load(File file) throws IOException {
        try (var in = new DataInputStream(new BufferedInputStream(new FileInputStream(file)))) {
            byte[] magic = new byte[4];
            in.readFully(magic);
            if (!(magic[0] == 'A' && magic[1] == 'L' && magic[2] == 'T' && magic[3] == '1')) {
                throw new IOException("Bad ALT header");
            }
            int n = in.readInt();
            int L = in.readInt();
            AltData d = new AltData(n, L);
            for (int i = 0; i < n * L; i++) {
                d.distFromLM[i] = in.readInt();
            }
            for (int i = 0; i < n * L; i++) {
                d.distToLM[i] = in.readInt();
            }
            return d;
        }
    }
}

// ========== EDGE ==========
final class Edge {

    final int to;
    final int timeCentis; // kantvekt i hundredels sekund

    Edge(int to, int timeCentis) {
        this.to = to;
        this.timeCentis = timeCentis;
    }
}

// ========== GRAPH (forward + reverse) ==========
final class Graph {

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

    public Graph(int n, Node[] nodes,
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
        return () -> new Iterator<>() {
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
        return () -> new Iterator<>() {
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

// ========== INTEREST POINT ==========
final class InterestPoint {

    public final int nodeId;
    public final int code;     // bitmask
    public final String name;

    public InterestPoint(int nodeId, int code, String name) {
        this.nodeId = nodeId;
        this.code = code;
        this.name = name;
    }

    public boolean isType(int mask) {
        return (code & mask) == mask;
    }

    // Koder iht. oppgaven:
    public static final int PLACE = 1;
    public static final int GAS = 2;
    public static final int EV = 4;
    public static final int EAT = 8;
    public static final int DRINK = 16;
    public static final int STAY = 32;
}

// ========== LINE SCAN (rask parser) ==========
final class LineScan {

    static int nextInt(String s, int[] idxRef) {
        int i = skipWs(s, idxRef[0]);
        int sign = 1;
        if (i < s.length() && s.charAt(i) == '-') {
            sign = -1;
            i++;
        }
        long v = 0;
        while (i < s.length()) {
            char c = s.charAt(i);
            if (c <= ' ') {
                break;
            }
            v = v * 10 + (c - '0');
            i++;
        }
        idxRef[0] = i;
        return (int) (sign * v);
    }

    static float nextFloat(String s, int[] idxRef) {
        int i = skipWs(s, idxRef[0]);
        int start = i;
        while (i < s.length() && s.charAt(i) > ' ') {
            i++;
        }
        float v = Float.parseFloat(s.substring(start, i));
        idxRef[0] = i;
        return v;
    }

    static int skipWs(String s, int i) {
        int n = s.length();
        while (i < n && s.charAt(i) <= ' ') {
            i++;
        }
        return i;
    }

    static String nextQuoted(String s, int[] idxRef) {
        int i = skipWs(s, idxRef[0]);
        if (i >= s.length() || s.charAt(i) != '"') {
            int start = i;
            while (i < s.length() && s.charAt(i) > ' ') {
                i++;
            }
            idxRef[0] = i;
            return s.substring(start, i);
        }
        i++; // hopp over "
        int start = i;
        while (i < s.length() && s.charAt(i) != '"') {
            i++;
        }
        String out = s.substring(start, Math.min(i, s.length()));
        idxRef[0] = Math.min(i + 1, s.length());
        return out;
    }
}

// ========== NODE ==========
final class Node {

    public final int id;
    public final float lat;  // holder i massevis
    public final float lon;

    public Node(int id, float lat, float lon) {
        this.id = id;
        this.lat = lat;
        this.lon = lon;
    }
}
