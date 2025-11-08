
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.util.*;

public class AltBuild {

    static void lesNoderInn(String filsti, Map<Integer, Node> noderPerId) throws IOException {
        try (BufferedReader bufferedReader = Files.newBufferedReader(Paths.get(filsti), StandardCharsets.UTF_8)) {
            String forsteLinje = bufferedReader.readLine();
            if (forsteLinje == null) {
                throw new IOException("Tom noder-fil");
            }

            String linje;
            while ((linje = bufferedReader.readLine()) != null) {
                String[] deler = linje.trim().split("\\s+");
                if (deler.length >= 3) {
                    int nodeId = Integer.parseInt(deler[0]);
                    double breddegrad = Double.parseDouble(deler[1]);
                    double lengdegrad = Double.parseDouble(deler[2]);
                    noderPerId.put(nodeId, new Node(nodeId, breddegrad, lengdegrad));
                }
            }
        }
    }

    static void lesKanterInn(String filsti, Map<Integer, Node> noderPerId) throws IOException {
        try (BufferedReader bufferedReader = Files.newBufferedReader(Paths.get(filsti), StandardCharsets.UTF_8)) {
            String forsteLinje = bufferedReader.readLine();
            if (forsteLinje == null) {
                throw new IOException("Tom kanter-fil");
            }

            String linje;
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
                        fraNode.utKanter.add(new Kant(fraNode, tilNode, kjoerertidCentiSekund, lengdeMeter, fartsgrenseKmt));
                    }
                }
            }
        }
    }

    static void lesInteressePunkterInn(String filsti, Map<Integer, Node> noderPerId) throws IOException {
        try (BufferedReader bufferedReader = Files.newBufferedReader(Paths.get(filsti), StandardCharsets.UTF_8)) {
            String forsteLinje = bufferedReader.readLine();
            if (forsteLinje == null) {
                throw new IOException("Tom interessepkt-fil");
            }

            String linje;
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
                    }
                }
            }
        }
    }

    static Set<InteressePunkter> dekoderInteressepunkter(int kode) {
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

    static int[] velgLandemerkerHeuristisk(Map<Integer, Node> graf, int antallLandemerker) {
        int minsteId = Integer.MAX_VALUE;
        int stoersteId = Integer.MIN_VALUE;

        for (int id : graf.keySet()) {
            minsteId = Math.min(minsteId, id);
            stoersteId = Math.max(stoersteId, id);
        }

        if (antallLandemerker <= 1) {
            return new int[]{minsteId};
        }
        if (antallLandemerker == 2) {
            return new int[]{minsteId, stoersteId};
        }

        int startId = minsteId;
        int sluttId = stoersteId;
        int spenn = Math.max(1, (sluttId - startId));

        int midt1 = startId + spenn / 3;
        int midt2 = startId + 2 * spenn / 3;

        int[] kandidater = new int[]{startId, midt1, midt2, sluttId};
        for (int i = 0; i < kandidater.length; i++) {
            if (!graf.containsKey(kandidater[i])) {
                kandidater[i] = naermesteEksisterendeId(graf, kandidater[i]);
            }
        }
        return Arrays.stream(kandidater).distinct().limit(antallLandemerker).toArray();
    }

    static int naermesteEksisterendeId(Map<Integer, Node> graf, int targetId) {
        int besteId = -1;
        int besteDifferanse = Integer.MAX_VALUE;

        for (int id : graf.keySet()) {
            int differanse = Math.abs(id - targetId);
            if (differanse < besteDifferanse) {
                besteId = id;
                besteDifferanse = differanse;
            }
        }
        return besteId;
    }

    public static void main(String[] kommandolinjeArgumenter) {
        try {
            String noderFil = (kommandolinjeArgumenter.length >= 1) ? kommandolinjeArgumenter[0] : "noder.txt";
            String kanterFil = (kommandolinjeArgumenter.length >= 2) ? kommandolinjeArgumenter[1] : "kanter.txt";
            String interessepktFil = (kommandolinjeArgumenter.length >= 3) ? kommandolinjeArgumenter[2] : "interessepkt.txt";
            String utFil = (kommandolinjeArgumenter.length >= 4) ? kommandolinjeArgumenter[3] : "alt-time.bin";
            int antallLandemerker = (kommandolinjeArgumenter.length >= 5) ? Integer.parseInt(kommandolinjeArgumenter[4]) : 4;

            // Bygg graf
            Map<Integer, Node> noderPerId = new HashMap<>();
            lesNoderInn(noderFil, noderPerId);
            lesKanterInn(kanterFil, noderPerId);
            lesInteressePunkterInn(interessepktFil, noderPerId);

            int totaltAntallKanter = noderPerId.values().stream().mapToInt(v -> v.utKanter.size()).sum();
            System.out.println("ALT bygg: noder=" + noderPerId.size() + ", kanter=" + totaltAntallKanter);

            // Velg landemerker
            int[] landemerker = velgLandemerkerHeuristisk(noderPerId, antallLandemerker);

            DijkstraAdapter adapter = new DijkstraAdapter(noderPerId);
            int antallNoder = adapter.hentAntallNoder();
            AltData altData = AltPreprocess.bygg(antallNoder, landemerker, adapter);

            AltPreprocess.lagre(altData, Paths.get(utFil));
            System.out.println("ALT skrevet til: " + utFil + "  (L=" + landemerker.length + ", N=" + antallNoder + ")");
        } catch (Exception e) {
            e.printStackTrace();
            System.err.println("Feil under ALT-forbehandling. Avbrutt.");
            System.exit(1);
        }
    }
}
