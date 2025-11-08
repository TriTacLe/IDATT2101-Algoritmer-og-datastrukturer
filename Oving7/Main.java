
import java.io.BufferedReader;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.PriorityQueue;
import java.util.Set;
import java.util.logging.Level;
import java.util.logging.Logger;

enum InteressePunkter {
    STEDSNAVN(1),
    BENSINSTASJON(2),
    LADESTASJON(4),
    SPISESTED(8),
    DRIKKESTED(16),
    OVERNATTINGSSTED(32);

    private final int kode;

    InteressePunkter(int kode) {
        this.kode = kode;
    }

    public int getKode() {
        return kode;
    }
}

class Node {

    int nummer;
    double breddegrad;
    double lengdegrad;
    List<Kant> kanter;
    String navn;
    Set<InteressePunkter> interesseTyper;

    int avstand;
    Node forgjenger;
    boolean besøkt;

    public Node(int nummer, double breddegrad, double lengdegrad) {
        this.nummer = nummer;
        this.breddegrad = breddegrad;
        this.lengdegrad = lengdegrad;
        this.kanter = new ArrayList<>();
        this.interesseTyper = new HashSet<>();
        this.avstand = Integer.MAX_VALUE;
        this.forgjenger = null;
        this.besøkt = false;
    }

    public void resetDijkstra() {
        this.avstand = Integer.MAX_VALUE;
        this.forgjenger = null;
        this.besøkt = false;
    }
}

class Kant {

    Node fraNode;
    Node tilNode;
    int kjøretid;
    int lengde;
    int fartsgrense;

    public Kant(Node fraNode, Node tilNode, int kjøretid, int lengde, int fartsgrense) {
        this.fraNode = fraNode;
        this.tilNode = tilNode;
        this.kjøretid = kjøretid;
        this.lengde = lengde;
        this.fartsgrense = fartsgrense;
    }
}

class Dijkstra {

    private final Map<Integer, Node> noder;
    private PriorityQueue<Node> køPrioritet;

    public Dijkstra(Map<Integer, Node> noder) {
        this.noder = noder;
    }

    private void resetNoder() {
        for (Node node : noder.values()) {
            node.resetDijkstra();
        }
    }

    public void kjørDijkstra(int startNodeNr, boolean brukKjøretid) {
        resetNoder();

        Node startNode = noder.get(startNodeNr);

        køPrioritet = new PriorityQueue<>(Comparator.comparingInt(n -> n.avstand));

        startNode.avstand = 0;
        køPrioritet.add(startNode);

        while (!køPrioritet.isEmpty()) {
            Node current = køPrioritet.poll();

            if (current.besøkt) {
                continue;
            }
            current.besøkt = true;

            for (Kant kant : current.kanter) {
                Node nabo = kant.tilNode;

                if (nabo.besøkt) {
                    continue;
                }

                int vekt = brukKjøretid ? kant.kjøretid : kant.lengde;
                int nyAvstand = current.avstand + vekt;

                if (nyAvstand < nabo.avstand) {
                    nabo.avstand = nyAvstand;
                    nabo.forgjenger = current;
                    køPrioritet.add(nabo);
                }
            }
        }
    }

    public List<Node> finnKortesteVei(int fraNodeNr, int tilNodeNr, boolean brukKjøretid) {
        kjørDijkstra(fraNodeNr, brukKjøretid);

        Node tilNode = noder.get(tilNodeNr);
        if (tilNode == null || tilNode.avstand == Integer.MAX_VALUE) {
            return null;
        }

        List<Node> vei = new ArrayList<>();
        Node current = tilNode;
        while (current != null) {
            vei.add(0, current);
            current = current.forgjenger;
        }

        return vei;
    }

    public Node finnNærmesteInteressePunkt(int fraNodeNr, InteressePunkter type, boolean brukKjøretid) {
        kjørDijkstra(fraNodeNr, brukKjøretid);

        Node nærmeste = null;
        int minAvstand = Integer.MAX_VALUE;

        for (Node node : noder.values()) {
            if (node.nummer != fraNodeNr
                    && node.interesseTyper.contains(type)
                    && node.avstand < minAvstand) {
                nærmeste = node;
                minAvstand = node.avstand;
            }
        }

        return nærmeste;
    }

    public int getAvstand(int nodeNr) {
        Node node = noder.get(nodeNr);
        return (node != null) ? node.avstand : Integer.MAX_VALUE;
    }
}

public class Main {

    private static final Logger LOGGER = Logger.getLogger(Main.class.getName());
    private static final Map<Integer, Node> noder = new HashMap<>();
    private static int antallNoder = 0;
    private static int antallKanter = 0;
    private static int antallInteressePunkter = 0;

    static void lesNoder(String filePathString) throws IOException {
        Path filePath = Paths.get(filePathString);
        try (BufferedReader bufferedReader = Files.newBufferedReader(filePath, StandardCharsets.UTF_8)) {

            // første linje antall noder
            String førsteLinje = bufferedReader.readLine();
            antallNoder = Integer.parseInt(førsteLinje.trim());
            System.out.println("\nAntall noder: " + antallNoder);

            String lineText;
            int teller = 0;
            // Leser resten
            while ((lineText = bufferedReader.readLine()) != null) {
                String[] deler = lineText.trim().split("\\s+");
                if (deler.length >= 3) {
                    int nodeNr = Integer.parseInt(deler[0]);
                    double breddegrad = Double.parseDouble(deler[1]);
                    double lengdegrad = Double.parseDouble(deler[2]);

                    Node node = new Node(nodeNr, breddegrad, lengdegrad);
                    noder.put(nodeNr, node);
                    teller++;
                }
            }
            System.out.println("\nLest " + teller + " noder");
        }
    }

    static void lesKanter(String filePathString) throws IOException {
        Path filePath = Paths.get(filePathString);
        try (BufferedReader bufferedReader = Files.newBufferedReader(filePath, StandardCharsets.UTF_8)) {

            // Les første linje antall kanter
            String førsteLinje = bufferedReader.readLine();
            antallKanter = Integer.parseInt(førsteLinje.trim());
            System.out.println("Forventer " + antallKanter + " kanter");

            String lineText;
            int counter = 0;

            while ((lineText = bufferedReader.readLine()) != null) {
                String[] deler = lineText.trim().split("\\s+");
                if (deler.length >= 5) {
                    int fraNodeNr = Integer.parseInt(deler[0]);
                    int tilNodeNr = Integer.parseInt(deler[1]);
                    int kjøretid = Integer.parseInt(deler[2]);
                    int lengde = Integer.parseInt(deler[3]);
                    int fartsgrense = Integer.parseInt(deler[4]);

                    Node fraNode = noder.get(fraNodeNr);
                    Node tilNode = noder.get(tilNodeNr);

                    // init
                    if (fraNode != null && tilNode != null) {
                        Kant kant = new Kant(fraNode, tilNode, kjøretid, lengde, fartsgrense);
                        fraNode.kanter.add(kant);
                        counter++;
                    }
                }
            }
            System.out.println("Lest " + counter + " kanter");
        }
    }

    static void lesInteressePunkter(String filePathString) throws IOException {
        Path filePath = Paths.get(filePathString);
        try (BufferedReader bufferedReader = Files.newBufferedReader(filePath, StandardCharsets.UTF_8)) {

            // Les første linje med antall interessepunkter
            String førsteLinje = bufferedReader.readLine();
            antallInteressePunkter = Integer.parseInt(førsteLinje.trim());
            System.out.println("Forventer " + antallInteressePunkter + " interessepunkter");

            String lineText;
            int teller = 0;
            while ((lineText = bufferedReader.readLine()) != null) {

                // Parse linje (nodenr kode "Navn på stedet")
                String[] deler = lineText.trim().split("\\s+", 3);
                if (deler.length >= 3) {
                    int nodeNr = Integer.parseInt(deler[0]);
                    int kode = Integer.parseInt(deler[1]);
                    String navn = deler[2].replaceAll("\"", ""); // Fjern anførselstegn

                    Node node = noder.get(nodeNr);
                    if (node != null) {
                        node.navn = navn;
                        node.interesseTyper = getInteressepunkter(kode);
                        teller++;
                    }
                }
            }
            System.out.println("Lest " + teller + " interessepunkter");
        }
    }

    private static Set<InteressePunkter> getInteressepunkter(int code) {
        Set<InteressePunkter> typer = new HashSet<>();

        // Sjekk hver bit for seg
        if ((code & 1) == 1) {
            typer.add(InteressePunkter.STEDSNAVN);
        }
        if ((code & 2) == 2) {
            typer.add(InteressePunkter.BENSINSTASJON);
        }
        if ((code & 4) == 4) {
            typer.add(InteressePunkter.LADESTASJON);
        }
        if ((code & 8) == 8) {
            typer.add(InteressePunkter.SPISESTED);
        }
        if ((code & 16) == 16) {
            typer.add(InteressePunkter.DRIKKESTED);
        }
        if ((code & 32) == 32) {
            typer.add(InteressePunkter.OVERNATTINGSSTED);
        }

        return typer;
    }

    private static void skrivUtVei(List<Node> vei, boolean visDetaljer) {
        if (vei == null || vei.isEmpty()) {
            System.out.println("Ingen vei funnet!");
            return;
        }

        System.out.println("Vei med " + vei.size() + " noder:");
        if (visDetaljer) {
            for (int i = 0; i < vei.size(); i++) {
                Node node = vei.get(i);
                System.out.print("  Node " + node.nummer);
                if (node.navn != null) {
                    System.out.print(" (" + node.navn + ")");
                }
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
            System.out.println("  Fra node " + start.nummer
                    + (start.navn != null ? " (" + start.navn + ")" : "")
                    + " til node " + slutt.nummer
                    + (slutt.navn != null ? " (" + slutt.navn + ")" : ""));
        }
    }

    public static void main(String[] args) {
        try {
            lesNoder("noder.txt");
            lesKanter("kanter.txt");
            lesInteressePunkter("interessepkt.txt");

            System.out.println("Totalt antall noder: " + noder.size());

            int totaltAntallKanter = 0;
            for (Node node : noder.values()) {
                totaltAntallKanter += node.kanter.size();
            }
            System.out.println("Totalt antall kanter: " + totaltAntallKanter);

            Map<InteressePunkter, Integer> interesseTelling = new HashMap<>();
            for (Node node : noder.values()) {
                for (InteressePunkter type : node.interesseTyper) {
                    interesseTelling.merge(type, 1, Integer::sum);
                }
            }
            System.out.println("\nInteressepunkter per type:");
            for (Map.Entry<InteressePunkter, Integer> entry : interesseTelling.entrySet()) {
                System.out.println("  " + entry.getKey() + ": " + entry.getValue());
            }

            Dijkstra dijkstra = new Dijkstra(noder);

            // Bare eksempel
            int fraNode = 1;
            int tilNode = 1000;

            System.out.println("\nKorteste vei basert på kjøretid fra node " + fraNode + " til " + tilNode + ":");
            List<Node> veiTid = dijkstra.finnKortesteVei(fraNode, tilNode, true);
            if (veiTid != null) {
                int avstandTid = dijkstra.getAvstand(tilNode);
                System.out.println("Total kjøretid: " + avstandTid + "(1/100 sekunder) = "
                        + String.format("%.2f", avstandTid / 100.0) + " sekunder");
                skrivUtVei(veiTid, false);
            } else {
                System.out.println("Ingen vei funnet");
            }

            System.out.println("\nKorteste vei basert på lengde fra node " + fraNode + " til " + tilNode + ":");
            List<Node> veiLengde = dijkstra.finnKortesteVei(fraNode, tilNode, false);
            if (veiLengde != null) {
                int avstandLengde = dijkstra.getAvstand(tilNode);
                System.out.println("Total lengde: " + avstandLengde + " meter = "
                        + String.format("%.2f", avstandLengde / 1000.0) + " km");
                skrivUtVei(veiLengde, false);
            } else {
                System.out.println("Ingen vei funnet");
            }

            // Finn nærmeste interessepunkt
            System.out.println("\nNærmeste interessepunkter");
            int søkFraNode = 500;

            // bensinstasjon
            Node nærmesteBensinstasjon = dijkstra.finnNærmesteInteressePunkt(
                    søkFraNode, InteressePunkter.BENSINSTASJON, false);
            if (nærmesteBensinstasjon != null) {
                System.out.println("Nærmeste bensinstasjon fra node " + søkFraNode + ":");
                System.out.println("Navn besinstasjon:  " + nærmesteBensinstasjon.navn
                        + " (node " + nærmesteBensinstasjon.nummer + ")");
                System.out.println("  Avstand: " + nærmesteBensinstasjon.avstand + " meter");
            }

            // ladestasjon
            Node nærmesteLadestasjon = dijkstra.finnNærmesteInteressePunkt(
                    søkFraNode, InteressePunkter.LADESTASJON, false);
            if (nærmesteLadestasjon != null) {
                System.out.println("\nNærmeste ladestasjon fra node " + søkFraNode + ":");
                System.out.println("Navn ladestasjon  " + nærmesteLadestasjon.navn
                        + " (node " + nærmesteLadestasjon.nummer + ")");
                System.out.println("  Avstand: " + nærmesteLadestasjon.avstand + " meter");
            }
        } catch (IOException e) {
            LOGGER.log(Level.SEVERE, "Error ved lesing av fil: ", e);
        } catch (Exception e) {
            LOGGER.log(Level.SEVERE, "Error: ", e);
            e.printStackTrace();
        }
    }
}
