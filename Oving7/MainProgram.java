
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.util.*;
import java.util.logging.Level;
import java.util.logging.Logger;

enum InteressePunkter {
    STEDSNAVN(1), BENSINSTASJON(2), LADESTASJON(4),
    SPISESTED(8), DRIKKESTED(16), OVERNATTINGSSTED(32);

    private final int kode;

    InteressePunkter(int kode) {
        this.kode = kode;
    }

    public int getKode() {
        return kode;
    }
}

final class Node {

    final int nodeNummer;
    final double breddegrad;
    final double lengdegrad;

    final List<Kant> utKanter = new ArrayList<>();
    final List<Kant> innKanter = new ArrayList<>();

    String navn;
    Set<InteressePunkter> interesseTyper = new HashSet<>();

    // Dijkstra
    int samletKostnad = Integer.MAX_VALUE;
    Node forgjengerNode = null;
    boolean besoekt = false;

    // A*
    int gKostnad = Integer.MAX_VALUE;
    int hHeuristikk = 0;
    int fPrioritet = Integer.MAX_VALUE;
    boolean iKo = false;

    Node(int nodeNummer, double breddegrad, double lengdegrad) {
        this.nodeNummer = nodeNummer;
        this.breddegrad = breddegrad;
        this.lengdegrad = lengdegrad;
    }

    void resetDijkstra() {
        samletKostnad = Integer.MAX_VALUE;
        forgjengerNode = null;
        besoekt = false;
    }

    void resetAStar() {
        gKostnad = Integer.MAX_VALUE;
        hHeuristikk = 0;
        fPrioritet = Integer.MAX_VALUE;
        forgjengerNode = null;
        besoekt = false;
        iKo = false;
    }
}

final class Kant {

    final Node fraNode;
    final Node tilNode;
    final int kjoerertidCentiSekund;
    final int lengdeMeter;
    final int fartsgrenseKmt;

    Kant(Node fraNode, Node tilNode, int kjoerertidCentiSekund, int lengdeMeter, int fartsgrenseKmt) {
        this.fraNode = fraNode;
        this.tilNode = tilNode;
        this.kjoerertidCentiSekund = kjoerertidCentiSekund;
        this.lengdeMeter = lengdeMeter;
        this.fartsgrenseKmt = fartsgrenseKmt;
    }
}

final class Heuristics {

    private static final double MAKS_HASTIGHET_MPS = 130_000.0 / 3600.0; // 130 km/t

    static double geodesiskDistanseIMeter(double breddegrad1, double lengdegrad1, double breddegrad2, double lengdegrad2) {
        final double jordradiusMeter = 6_371_000.0;
        double deltaBredde = Math.toRadians(breddegrad2 - breddegrad1);
        double deltaLengde = Math.toRadians(lengdegrad2 - lengdegrad1);

        double a = Math.sin(deltaBredde / 2) * Math.sin(deltaBredde / 2)
                + Math.cos(Math.toRadians(breddegrad1)) * Math.cos(Math.toRadians(breddegrad2))
                * Math.sin(deltaLengde / 2) * Math.sin(deltaLengde / 2);

        double c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
        return jordradiusMeter * c;
    }

    static int meterTilTidCentiSekund(double meter) {
        double sekunder = meter / MAKS_HASTIGHET_MPS;
        return (int) Math.floor(sekunder * 100.0);
    }
}

final class Dijkstra {

    private final Map<Integer, Node> noderPerId;
    private PriorityQueue<Node> prioritetKo;
    int antallNoderPoppet = 0;

    Dijkstra(Map<Integer, Node> noderPerId) {
        this.noderPerId = noderPerId;
    }

    private void reset() {
        for (Node node : noderPerId.values()) {
            node.resetDijkstra();
        }
        antallNoderPoppet = 0;
    }

    public void kjørDijkstra(int startNodeNummer, boolean brukKjoerertid) {
        reset();

        Node startNode = noderPerId.get(startNodeNummer);
        if (startNode == null) {
            return;
        }

        prioritetKo = new PriorityQueue<>(Comparator.comparingInt(n -> n.samletKostnad));
        startNode.samletKostnad = 0;
        prioritetKo.add(startNode);

        while (!prioritetKo.isEmpty()) {
            Node aktivNode = prioritetKo.poll();
            if (aktivNode.besoekt) {
                continue;
            }

            aktivNode.besoekt = true;
            antallNoderPoppet++;

            for (Kant kant : aktivNode.utKanter) {
                Node naboNode = kant.tilNode;
                if (naboNode.besoekt) {
                    continue;
                }

                int vekt = brukKjoerertid ? kant.kjoerertidCentiSekund : kant.lengdeMeter;
                int nyKostnad = (aktivNode.samletKostnad == Integer.MAX_VALUE)
                        ? Integer.MAX_VALUE : aktivNode.samletKostnad + vekt;

                if (nyKostnad < naboNode.samletKostnad) {
                    naboNode.samletKostnad = nyKostnad;
                    naboNode.forgjengerNode = aktivNode;
                    prioritetKo.add(naboNode);
                }
            }
        }
    }

    public List<Node> finnKortesteVei(int fraNodeNummer, int tilNodeNummer, boolean brukKjoerertid) {
        kjørDijkstra(fraNodeNummer, brukKjoerertid);

        Node maalNode = noderPerId.get(tilNodeNummer);
        if (maalNode == null || maalNode.samletKostnad == Integer.MAX_VALUE) {
            return null;
        }

        ArrayList<Node> sti = new ArrayList<>();
        for (Node peker = maalNode; peker != null; peker = peker.forgjengerNode) {
            sti.add(0, peker);
        }
        return sti;
    }

    public int hentSamletKostnad(int nodeNummer) {
        Node node = noderPerId.get(nodeNummer);
        return (node != null) ? node.samletKostnad : Integer.MAX_VALUE;
    }
}

final class AltData {

    final int[][] distanseFraLandemerke;
    final int[][] distanseTilLandemerke;
    final int[] landemerker;

    AltData(int[] landemerker, int antallNoder) {
        this.landemerker = landemerker;
        this.distanseFraLandemerke = new int[landemerker.length][antallNoder];
        this.distanseTilLandemerke = new int[landemerker.length][antallNoder];
    }
}

final class AltPreprocess {

    interface EnkeltKildeKortesteSti {

        int[] run(int startNodeNummer, boolean reversert);
    }

    public static AltData bygg(int antallNoder, int[] landemerker, EnkeltKildeKortesteSti sssp) {
        AltData altData = new AltData(landemerker, antallNoder);
        for (int i = 0; i < landemerker.length; i++) {
            int landemerke = landemerker[i];
            altData.distanseFraLandemerke[i] = sssp.run(landemerke, false);
            altData.distanseTilLandemerke[i] = sssp.run(landemerke, true);
        }
        return altData;
    }

    public static void lagre(AltData altData, Path filsti) throws IOException {
        try (var dataUt = new DataOutputStream(Files.newOutputStream(filsti))) {
            dataUt.writeInt(altData.landemerker.length);
            dataUt.writeInt(altData.distanseFraLandemerke[0].length);

            for (int landemerke : altData.landemerker) {
                dataUt.writeInt(landemerke);
            }
            for (int i = 0; i < altData.landemerker.length; i++) {
                for (int d : altData.distanseFraLandemerke[i]) {
                    dataUt.writeInt(d);
                }
            }
            for (int i = 0; i < altData.landemerker.length; i++) {
                for (int d : altData.distanseTilLandemerke[i]) {
                    dataUt.writeInt(d);
                }
            }
        }
    }

    public static AltData last(Path filsti) throws IOException {
        try (var dataInn = new DataInputStream(Files.newInputStream(filsti))) {
            int antallLandemerker = dataInn.readInt();
            int antallNoder = dataInn.readInt();

            int[] landemerker = new int[antallLandemerker];
            for (int i = 0; i < antallLandemerker; i++) {
                landemerker[i] = dataInn.readInt();
            }

            AltData altData = new AltData(landemerker, antallNoder);

            for (int i = 0; i < antallLandemerker; i++) {
                for (int n = 0; n < antallNoder; n++) {
                    altData.distanseFraLandemerke[i][n] = dataInn.readInt();
                }
            }

            for (int i = 0; i < antallLandemerker; i++) {
                for (int n = 0; n < antallNoder; n++) {
                    altData.distanseTilLandemerke[i][n] = dataInn.readInt();
                }
            }

            return altData;
        }
    }
}

final class GraphViews {

    static Map<Integer, Node> byggReversert(Map<Integer, Node> originalGraf) {
        Map<Integer, Node> reversertGraf = new HashMap<>();

        for (Node node : originalGraf.values()) {
            Node kopi = new Node(node.nodeNummer, node.breddegrad, node.lengdegrad);
            kopi.navn = node.navn;
            kopi.interesseTyper = node.interesseTyper;
            reversertGraf.put(node.nodeNummer, kopi);
        }

        for (Node fra : originalGraf.values()) {
            for (Kant kant : fra.utKanter) {
                Node fraRevers = reversertGraf.get(kant.tilNode.nodeNummer);
                Node tilRevers = reversertGraf.get(kant.fraNode.nodeNummer);
                fraRevers.utKanter.add(new Kant(fraRevers, tilRevers,
                        kant.kjoerertidCentiSekund, kant.lengdeMeter, kant.fartsgrenseKmt));
            }
        }
        return reversertGraf;
    }

    static int maksNodeId(Map<Integer, Node> graf) {
        int maks = 0;
        for (int id : graf.keySet()) {
            maks = Math.max(maks, id);
        }
        return maks;
    }
}

final class DijkstraAdapter implements AltPreprocess.EnkeltKildeKortesteSti {

    private final Map<Integer, Node> graf;
    private final int antallNoder;

    DijkstraAdapter(Map<Integer, Node> normalGraf) {
        this.graf = normalGraf;
        this.antallNoder = GraphViews.maksNodeId(normalGraf) + 1;
    }

    @Override
    public int[] run(int startNodeNummer, boolean reversert) {
        for (Node node : graf.values()) {
            node.samletKostnad = Integer.MAX_VALUE;
            node.forgjengerNode = null;
            node.besoekt = false;
        }

        Node startNode = graf.get(startNodeNummer);
        if (startNode == null) {
            return new int[antallNoder];
        }

        PriorityQueue<Node> prioritetKo = new PriorityQueue<>(Comparator.comparingInt(n -> n.samletKostnad));
        startNode.samletKostnad = 0;
        prioritetKo.add(startNode);

        while (!prioritetKo.isEmpty()) {
            Node aktivNode = prioritetKo.poll();
            if (aktivNode.besoekt) {
                continue;
            }
            aktivNode.besoekt = true;

            List<Kant> adjacency = reversert ? aktivNode.innKanter : aktivNode.utKanter;

            for (Kant kant : adjacency) {
                Node naboNode = kant.tilNode;
                if (naboNode.besoekt) {
                    continue;
                }

                int vekt = kant.kjoerertidCentiSekund;
                int nyKostnad = (aktivNode.samletKostnad == Integer.MAX_VALUE)
                        ? Integer.MAX_VALUE : aktivNode.samletKostnad + vekt;

                if (nyKostnad < naboNode.samletKostnad) {
                    naboNode.samletKostnad = nyKostnad;
                    naboNode.forgjengerNode = aktivNode;
                    prioritetKo.add(naboNode);
                }
            }
        }

        int[] distanse = new int[antallNoder];
        Arrays.fill(distanse, Integer.MAX_VALUE);
        for (Node node : graf.values()) {
            distanse[node.nodeNummer] = node.samletKostnad;
        }
        return distanse;
    }

    int hentAntallNoder() {
        return antallNoder;
    }
}

final class AStar {

    private final Map<Integer, Node> noderPerId;
    private final PriorityQueue<Node> prioritetKo;
    int antallNoderPoppet = 0;

    AStar(Map<Integer, Node> noderPerId) {
        this.noderPerId = noderPerId;
        this.prioritetKo = new PriorityQueue<>(Comparator.comparingInt(n -> n.fPrioritet));
    }

    private void reset() {
        for (Node node : noderPerId.values()) {
            node.resetAStar();
        }
    }

    private int altEstimertRest(Node node, Node maal, AltData altData) {
        int beste = 0;
        int vId = node.nodeNummer;
        int tId = maal.nodeNummer;

        for (int i = 0; i < altData.landemerker.length; i++) {
            int L_t = altData.distanseFraLandemerke[i][tId];
            int L_v = altData.distanseFraLandemerke[i][vId];
            if (L_t != Integer.MAX_VALUE && L_v != Integer.MAX_VALUE) {
                beste = Math.max(beste, Math.max(0, L_t - L_v));
            }

            int v_L = altData.distanseTilLandemerke[i][vId];
            int t_L = altData.distanseTilLandemerke[i][tId];
            if (v_L != Integer.MAX_VALUE && t_L != Integer.MAX_VALUE) {
                beste = Math.max(beste, Math.max(0, v_L - t_L));
            }
        }
        return beste;
    }

    private int geoEstimertRest(Node node, Node maal, boolean brukKjoerertid) {
        double meter = Heuristics.geodesiskDistanseIMeter(
                node.breddegrad, node.lengdegrad, maal.breddegrad, maal.lengdegrad);
        return brukKjoerertid ? Heuristics.meterTilTidCentiSekund(meter) : (int) Math.floor(meter);
    }

    List<Node> finnRuteAStar(int startNodeNummer, int maalNodeNummer, boolean brukKjoerertid, AltData altData) {
        reset();

        Node startNode = noderPerId.get(startNodeNummer);
        Node maalNode = noderPerId.get(maalNodeNummer);
        if (startNode == null || maalNode == null) {
            return null;
        }

        startNode.gKostnad = 0;
        startNode.hHeuristikk = (altData != null)
                ? altEstimertRest(startNode, maalNode, altData)
                : geoEstimertRest(startNode, maalNode, brukKjoerertid);
        startNode.fPrioritet = startNode.gKostnad + startNode.hHeuristikk;
        prioritetKo.add(startNode);
        startNode.iKo = true;
        antallNoderPoppet = 0;

        while (!prioritetKo.isEmpty()) {
            Node aktivNode = prioritetKo.poll();
            if (aktivNode.besoekt) {
                continue;
            }

            aktivNode.besoekt = true;
            antallNoderPoppet++;

            if (aktivNode == maalNode) {
                break;
            }
            for (Kant kant : aktivNode.utKanter) {
                Node naboNode = kant.tilNode;
                if (naboNode.besoekt) {
                    continue;
                }

                int vekt = brukKjoerertid ? kant.kjoerertidCentiSekund : kant.lengdeMeter;
                int tentativG = (aktivNode.gKostnad == Integer.MAX_VALUE)
                        ? Integer.MAX_VALUE : aktivNode.gKostnad + vekt;

                if (tentativG < naboNode.gKostnad) {
                    naboNode.gKostnad = tentativG;
                    naboNode.forgjengerNode = aktivNode;

                    if (naboNode.hHeuristikk == 0 && naboNode != startNode) {
                        naboNode.hHeuristikk = (altData != null)
                                ? altEstimertRest(naboNode, maalNode, altData)
                                : geoEstimertRest(naboNode, maalNode, brukKjoerertid);
                    }

                    naboNode.fPrioritet = (naboNode.gKostnad == Integer.MAX_VALUE)
                            ? Integer.MAX_VALUE : naboNode.gKostnad + naboNode.hHeuristikk;

                    prioritetKo.add(naboNode);
                    naboNode.iKo = true;
                }
            }
        }

        if (!maalNode.besoekt || (maalNode.forgjengerNode == null && startNode != maalNode)) {
            return null;
        }

        ArrayList<Node> rute = new ArrayList<>();
        for (Node peker = maalNode; peker != null; peker = peker.forgjengerNode) {
            rute.add(0, peker);
        }
        return rute;
    }
}

public class MainProgram {

    private static final Logger LOGGER = Logger.getLogger(MainProgram.class.getName());
    private static final Map<Integer, Node> noderPerId = new HashMap<>();

    static void lesNoder(String filsti) throws IOException {
        try (BufferedReader bufferedReader = Files.newBufferedReader(Paths.get(filsti), StandardCharsets.UTF_8)) {
            String forsteLinje = bufferedReader.readLine();
            if (forsteLinje == null) {
                throw new IOException("Tom noder-fil");
            }

            // int forventetAntallNoder = Integer.parseInt(forsteLinje.trim());
            String linje;
            // int faktiskAntall = 0;

            while ((linje = bufferedReader.readLine()) != null) {
                String[] deler = linje.trim().split("\\s+");
                if (deler.length >= 3) {
                    int nodeId = Integer.parseInt(deler[0]);
                    double breddegrad = Double.parseDouble(deler[1]);
                    double lengdegrad = Double.parseDouble(deler[2]);
                    noderPerId.put(nodeId, new Node(nodeId, breddegrad, lengdegrad));
                    // faktiskAntall++;
                }
            }
            // System.out.println("Lest " + faktiskAntall + " noder (forventet " + forventetAntallNoder + ")");
        }
    }

    private static Set<InteressePunkter> parseTypeUnion(String spesifikasjon) {
        Set<InteressePunkter> typer = new HashSet<>();
        for (String rå : spesifikasjon.toLowerCase().split("\\|")) {
            String token = rå.trim();
            switch (token) {
                case "spisested" ->
                    typer.add(InteressePunkter.SPISESTED);
                case "drikkested" ->
                    typer.add(InteressePunkter.DRIKKESTED);
                case "bensinstasjon" ->
                    typer.add(InteressePunkter.BENSINSTASJON);
                case "ladestasjon" ->
                    typer.add(InteressePunkter.LADESTASJON);
                case "overnatting" ->
                    typer.add(InteressePunkter.OVERNATTINGSSTED);
                case "stedsnavn" ->
                    typer.add(InteressePunkter.STEDSNAVN);
                default -> {
                }
            }
        }
        return typer;
    }

    static void lesKanter(String filsti) throws IOException {
        try (BufferedReader bufferedReader = Files.newBufferedReader(Paths.get(filsti), StandardCharsets.UTF_8)) {
            String forsteLinje = bufferedReader.readLine();
            if (forsteLinje == null) {
                throw new IOException("Tom kanter-fil");
            }

            // int forventetAntallKanter = Integer.parseInt(forsteLinje.trim());
            String linje;
            // int faktiskAntall = 0;

            while ((linje = bufferedReader.readLine()) != null) {
                String[] deler = linje.trim().split("\\s+");
                if (deler.length >= 5) {
                    int fraNodeId = Integer.parseInt(deler[0]);
                    int tilNodeId = Integer.parseInt(deler[1]);
                    int kjoerertidCentiSekund = Integer.parseInt(deler[2]);
                    int lengdeMeter = Integer.parseInt(deler[3]);
                    int fartsgrenseKmt = Integer.parseInt(deler[4]);

                    Node fraNode = noderPerId.get(fraNodeId);
                    Node tilNode = noderPerId.get(tilNodeId);

                    if (fraNode != null && tilNode != null) {
                        Kant fremKant = new Kant(fraNode, tilNode, kjoerertidCentiSekund, lengdeMeter, fartsgrenseKmt);
                        Kant innKant = new Kant(tilNode, fraNode, kjoerertidCentiSekund, lengdeMeter, fartsgrenseKmt);
                        fraNode.utKanter.add(fremKant);
                        tilNode.innKanter.add(innKant);
                        // faktiskAntall++;
                    }
                }
            }
            // System.out.println("Lest " + faktiskAntall + " kanter (forventet " + forventetAntallKanter + ")");
        }
    }

    static void lesInteressePunkter(String filsti) throws IOException {
        try (BufferedReader bufferedReader = Files.newBufferedReader(Paths.get(filsti), StandardCharsets.UTF_8)) {
            String forsteLinje = bufferedReader.readLine();
            if (forsteLinje == null) {
                throw new IOException("Tom interessepkt-fil");
            }

            int forventetAntall = Integer.parseInt(forsteLinje.trim());
            String linje;
            // int faktiskAntall = 0;

            while ((linje = bufferedReader.readLine()) != null) {
                String[] deler = linje.trim().split("\\s+", 3);
                if (deler.length >= 3) {
                    int nodeId = Integer.parseInt(deler[0]);
                    int kode = Integer.parseInt(deler[1]);
                    String navn = deler[2].replace("\"", "");

                    Node node = noderPerId.get(nodeId);
                    if (node != null) {
                        node.navn = navn;
                        node.interesseTyper = dekoderInteressepunkter(kode);
                        // faktiskAntall++;
                    }
                }
            }
            // System.out.println("Lest " + faktiskAntall + " interessepunkter (forventet " + forventetAntall + ")");
        }
    }

    private static Set<InteressePunkter> dekoderInteressepunkter(int kode) {
        Set<InteressePunkter> typer = new HashSet<>();
        if ((kode & 1) != 0) {
            typer.add(InteressePunkter.STEDSNAVN);
        }
        if ((kode & 2) != 0) {
            typer.add(InteressePunkter.BENSINSTASJON);
        }
        if ((kode & 4) != 0) {
            typer.add(InteressePunkter.LADESTASJON);
        }
        if ((kode & 8) != 0) {
            typer.add(InteressePunkter.SPISESTED);
        }
        if ((kode & 16) != 0) {
            typer.add(InteressePunkter.DRIKKESTED);
        }
        if ((kode & 32) != 0) {
            typer.add(InteressePunkter.OVERNATTINGSSTED);
        }
        return typer;
    }

    static void skrivUtVei(List<Node> vei, boolean medDetaljer) {
        if (vei == null || vei.isEmpty()) {
            System.out.println("Ingen vei funnet.");
            return;
        }
        System.out.println("Vei med " + vei.size() + " noder:");
        if (medDetaljer) {
            for (int i = 0; i < vei.size(); i++) {
                Node node = vei.get(i);
                System.out.print("  " + node.nodeNummer + (node.navn != null ? (" (" + node.navn + ")") : ""));
                if (i < vei.size() - 1) {
                    System.out.print(" -> ");
                }
                if ((i + 1) % 5 == 0) {
                    System.out.println();
                }
            }
            System.out.println();
        } else {
            Node start = vei.get(0);
            Node slutt = vei.get(vei.size() - 1);
            System.out.println("  Fra " + start.nodeNummer + (start.navn != null ? (" (" + start.navn + ")") : "")
                    + " til " + slutt.nodeNummer + (slutt.navn != null ? (" (" + slutt.navn + ")") : ""));
        }
    }

    static List<Node> finnKNærmeste(Dijkstra dijkstra, int fraNodeNummer, InteressePunkter type, boolean brukKjoerertid, int k) {
        dijkstra.kjørDijkstra(fraNodeNummer, brukKjoerertid);

        PriorityQueue<Node> beste = new PriorityQueue<>(Comparator.comparingInt(n -> -n.samletKostnad)); // max-heap
        for (Node node : noderPerId.values()) {
            if (node.nodeNummer == fraNodeNummer) {
                continue;
            }
            if (!node.interesseTyper.contains(type)) {
                continue;
            }
            if (node.samletKostnad == Integer.MAX_VALUE) {
                continue;
            }

            if (beste.size() < k) {
                beste.add(node);
            } else if (node.samletKostnad < beste.peek().samletKostnad) {
                beste.poll();
                beste.add(node);
            }
        }
        ArrayList<Node> resultat = new ArrayList<>(beste);
        resultat.sort(Comparator.comparingInt(n -> n.samletKostnad));
        return resultat;
    }

    static List<Node> finnKNærmesteUnion(Dijkstra dijkstra, int fraNodeNummer, Set<InteressePunkter> typer, boolean brukKjoerertid, int k) {
        dijkstra.kjørDijkstra(fraNodeNummer, brukKjoerertid);

        PriorityQueue<Node> beste = new PriorityQueue<>(Comparator.comparingInt(n -> -n.samletKostnad));
        for (Node node : noderPerId.values()) {
            if (node.nodeNummer == fraNodeNummer) {
                continue;
            }
            if (Collections.disjoint(node.interesseTyper, typer)) {
                continue;
            }
            if (node.samletKostnad == Integer.MAX_VALUE) {
                continue;
            }

            if (beste.size() < k) {
                beste.add(node);
            } else if (node.samletKostnad < beste.peek().samletKostnad) {
                beste.poll();
                beste.add(node);
            }
        }
        ArrayList<Node> resultat = new ArrayList<>(beste);
        resultat.sort(Comparator.comparingInt(n -> n.samletKostnad));
        return resultat;
    }

    static String formatTidCentiSekund(int centiSekund) {
        if (centiSekund == Integer.MAX_VALUE) {
            return "∞";
        }
        int sekunder = centiSekund / 100;
        int timer = sekunder / 3600;
        int minutter = (sekunder % 3600) / 60;
        int sek = sekunder % 60;
        return String.format("%02d:%02d:%02d", timer, minutter, sek);
    }

    static void skrivRuteSomCsv(List<Node> rute, String filnavn) throws IOException {
        try (var writer = Files.newBufferedWriter(Paths.get(filnavn), StandardCharsets.UTF_8)) {
            writer.write("lat,lon\n");
            if (rute != null) {
                for (Node node : rute) {
                    writer.write(node.breddegrad + "," + node.lengdegrad + "\n");
                }
            }
        }
        System.out.println("Skrev rute til " + filnavn + " (lat,lon per linje)");
    }
    private static final long SOFT_LIMIT_MS = 15_000;
    private static final long HARD_LIMIT_MS = 300_000;

    private static void sjekkTid(String navn, long ms) {
        if (ms > HARD_LIMIT_MS) {
            System.err.printf(
                    "%s: %.3f s — over 5 minutter tyder på lite effektiv prioritetskø.\n",
                    navn, ms / 1000.0
            );
        } else if (ms > SOFT_LIMIT_MS) {
            System.out.printf(
                    "%s: %.3f s — over 15 sekunder. På PC bør dette være raskere.\n",
                    navn, ms / 1000.0
            );
        }
    }

    public static void main(String[] kommandolinjeArgumenter) {

        try {
            lesNoder("noder.txt");
            lesKanter("kanter.txt");
            lesInteressePunkter("interessepkt.txt");

            // System.out.println("Totalt antall noder: " + noderPerId.size());
            // int totaltAntallKanter = noderPerId.values().stream().mapToInt(n -> n.utKanter.size()).sum();
            // System.out.println("Totalt antall kanter: " + totaltAntallKanter);
            // --- SUBKOMMANDO: nearest <startNode> <types> [metric=length|time] [k=5] [outfile] ---
            if (kommandolinjeArgumenter.length >= 1 && kommandolinjeArgumenter[0].equalsIgnoreCase("nearest")) {
                if (kommandolinjeArgumenter.length < 3) {
                    System.out.println("Bruk: nearest <startNode> <types> [metric=length|time] [k=5] [outfile]");
                    System.out.println("Eksempel: nearest 2374446 food|drink length 5 nearest_food_drink_nrk_tyholt.csv");
                    return;
                }

                int startNodeNummer = Integer.parseInt(kommandolinjeArgumenter[1]);
                String typeSpesifikasjon = kommandolinjeArgumenter[2];
                String metrikkArgument = (kommandolinjeArgumenter.length >= 4) ? kommandolinjeArgumenter[3].toLowerCase() : "length";
                boolean brukKjoerertid = metrikkArgument.equals("time");
                int antall = (kommandolinjeArgumenter.length >= 5) ? Integer.parseInt(kommandolinjeArgumenter[4]) : 5;
                String utCsv = (kommandolinjeArgumenter.length >= 6)
                        ? kommandolinjeArgumenter[5]
                        : ("nearest_" + typeSpesifikasjon.replace('|', '_') + "_" + startNodeNummer + ".csv");

                Dijkstra dijkstra = new Dijkstra(noderPerId);
                Set<InteressePunkter> unionTyper = parseTypeUnion(typeSpesifikasjon);

                long t0 = System.nanoTime();
                List<Node> resultat;
                if (unionTyper.size() <= 1) {
                    InteressePunkter type = unionTyper.isEmpty() ? InteressePunkter.SPISESTED : unionTyper.iterator().next();
                    resultat = finnKNærmeste(dijkstra, startNodeNummer, type, brukKjoerertid, antall);
                } else {
                    resultat = finnKNærmesteUnion(dijkstra, startNodeNummer, unionTyper, brukKjoerertid, antall);
                }
                long t1 = System.nanoTime();

                System.out.printf(
                        "Nearest/Dijkstra %s: %.3f ms, =%d\n",
                        brukKjoerertid ? "(time)" : "(length)",
                        (t1 - t0) / 1e6,
                        dijkstra.antallNoderPoppet
                );

                System.out.printf("\n%d nærmeste (%s) fra node %d (%s):\n",
                        resultat.size(), typeSpesifikasjon, startNodeNummer, brukKjoerertid ? "tid" : "meter");
                for (Node node : resultat) {
                    System.out.println("  " + node.nodeNummer + (node.navn != null ? (" (" + node.navn + ")") : "")
                            + "  distanse=" + node.samletKostnad + (brukKjoerertid ? " cs" : " m"));
                }

                skrivRuteSomCsv(resultat, utCsv);
                return;
            }

            int fraNodeNummer = (kommandolinjeArgumenter.length >= 1) ? Integer.parseInt(kommandolinjeArgumenter[0]) : 1;
            int tilNodeNummer = (kommandolinjeArgumenter.length >= 2) ? Integer.parseInt(kommandolinjeArgumenter[1]) : 1000;
            String metrikk = (kommandolinjeArgumenter.length >= 3) ? kommandolinjeArgumenter[2].toLowerCase() : "time";
            boolean brukKjoerertid = !metrikk.equals("length");
            boolean brukALT = (kommandolinjeArgumenter.length >= 4) ? Boolean.parseBoolean(kommandolinjeArgumenter[3]) : true;

            // preporsessering
            Path altFil = Paths.get("alt-time.bin");
            AltData altData = null;
            if (brukALT) {
                if (Files.exists(altFil)) {
                    altData = AltPreprocess.last(altFil);
                    System.out.println("ALT: lastet fra " + altFil);
                } else {
                    System.out.println("Mangler prepro-fil " + altFil
                            + ". Kjør først:  java AltBuild [noder.txt] [kanter.txt] [interessepkt.txt] [alt-time.bin] [Lcount]");
                    System.exit(2);
                }
            }

            // Dijkstra
            Dijkstra dijkstra = new Dijkstra(noderPerId);
            long dijkstraStartNs = System.nanoTime();

            @SuppressWarnings("unused")
            List<Node> dijkstraRute = dijkstra.finnKortesteVei(fraNodeNummer, tilNodeNummer, brukKjoerertid);
            long dijkstraSluttNs = System.nanoTime();
            int dijkstraKostnad = dijkstra.hentSamletKostnad(tilNodeNummer);
            System.out.printf("Dijkstra %s: %.3f ms, kostnad=%d %s, popped=%d\n",
                    brukKjoerertid ? "(time)" : "(length)",
                    (dijkstraSluttNs - dijkstraStartNs) / 1e6,
                    dijkstraKostnad,
                    brukKjoerertid ? ("(" + formatTidCentiSekund(dijkstraKostnad) + ")") : "m",
                    dijkstra.antallNoderPoppet);

            // A*
            AStar aStar = new AStar(noderPerId);
            long aStarStartNs = System.nanoTime();
            List<Node> aStarRute = aStar.finnRuteAStar(fraNodeNummer, tilNodeNummer, brukKjoerertid, altData);
            long aStarSluttNs = System.nanoTime();
            int aStarKostnad = (aStarRute == null) ? Integer.MAX_VALUE : aStarRute.get(aStarRute.size() - 1).gKostnad;
            System.out.printf("A* %s%s: %.3f ms, kostnad=%d %s, popped=%d\n",
                    brukKjoerertid ? "(time)" : "(length)",
                    (brukALT ? " + ALT" : ""),
                    (aStarSluttNs - aStarStartNs) / 1e6,
                    aStarKostnad,
                    brukKjoerertid ? ("(" + formatTidCentiSekund(aStarKostnad) + ")") : "m",
                    aStar.antallNoderPoppet);

            if (dijkstraKostnad != aStarKostnad) {
                System.out.println("Error: Dijkstra og A* kostnad er ulik.");
            }

            if (aStarRute != null) {
                System.out.println("Kort rute-print:");
                skrivUtVei(aStarRute, false);
            }

            String utFilnavn = String.format("route_%d_%d_%s_%s.csv",
                    fraNodeNummer, tilNodeNummer, brukKjoerertid ? "time" : "length", brukALT ? "alt" : "noalt");
            skrivRuteSomCsv(aStarRute, utFilnavn);

        } catch (Exception e) {
            LOGGER.log(Level.SEVERE, "Error:", e);
            e.printStackTrace();
        }
    }
}
