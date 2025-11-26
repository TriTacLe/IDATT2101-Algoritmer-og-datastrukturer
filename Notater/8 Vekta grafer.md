### 8.1 Vektede grafer
- I en vektet graf er det forskjell på kantene
- Hver kant har en *vekt* $v$:
	- et tall, vanligvis
	- betydningen avhenger av hva vi bruker grafen til
- Kanter kan ha negativ vekt i noen situasjoner
**Datastruktur**
- Legger til et felt `vekt`på datastrukturen til uvektet graf

**Aktuelle problemer som brukes i vektede grafer**
- Korteste-vei problemet
- Minimale spenntrær
- Maksimum flyt
### 8.2 Korteste-vei problemet
Korteste vei varianter
1. *Korteste vei fra én startnode til alle de andre.* Å finne kortest vei til bare én annen node har ikke lavere kompleksitet da veien i verste fall kan gå igjennom alle andre noder
2. *Korteste vei fra alle andre noder til én målnode.* Beregne omvendte grafen $G^T$, og bruke en vanlig kortest vei algoritme med målnoden som startnode.
3. *Kortest vei fra hver node til hver av de andre.* Kan bruke "en til alle" algoritme for hver node, men det finnes flere effektive måter.

**Rundtur:** begynner og slutter i samme node. 
- Korteste vei fra en node til en annen kan ikke inneholde rundtur

**Alle til alle korteste vei (Floyd Warshall)**
- Kjøretid‚ $\Theta (N^3)$, og trenger‚ $\Theta(N^2)$plass for resultatet
```Java
for (int i=0; i<N; ++i) dist[i][i] = 0;
for (alle kanter) dist[fra][til] = kantvekt;
for (int k=0; k<N; ++k) {
	for (int i=0; i<N; ++i) {
		for (int j=0; j<N; ++j) {
			if (dist[i][j] > dist[i][k] + dist[k][j])
			dist[i][j] = dist[i][k] + dist[k][j];
		}
	}
}
```

#### 8.2.1 Dijkstras algoritme
- Den mest effektive algoritmen for korteste-vei problemet.
- En node til alle andre
- Ingen negativ vekt
- Bruker *prioritetskø* for å holde orden på hvilken node som har lavest distanseestimat blant de som en ikke er ferdig med
- Samme «forgjengersystem» som i BFS
- Algoritmen garanterer alltid korteste vei fordi den behandler noder i rekkefølge etter avstand - når en node først er ferdigbehandlet, har vi funnet korteste vei dit.

**Implementasjon**
1. *Start:* Startnoden har avstand 0 og alle andre til uendelig. Legg alle noder i en prioritetskø
2. *Velg node:* Ta ut noden med lavest avstand fra køen (første gang: startnoden)
3. *Oppdater nabo:* For hver nabo til valgt node:
	- Beregn ny avstand = nodens avstand + kantvekt
	- Hvis ny avstand er kortere enn naboens nåværende avstand, oppdater den
4. *Gjenta:* Fortsett til alle noder er behandlet - til prioritetskøen er tom

**Navigasjon og brukt i praksis**
- Legg merke til at «forgjengerne» går mot kjøreretningen
- I navigasjonsprogrammer bruker vi ofte den omvendte grafen, og starter søket i målnoden. Slik finner vi alle veier inn til målet, heller enn alle veier ut fra start. Nyttig hvis man kjører feil et sted. I Dijkstra kan ta litt tid på «verdenskartet». Vi kan bryte av tidlig når algoritmen plukker startnoden ut av prioritetskøen.
	- NB! Ikke når startnoden blir funnet for første gang!
	- Den første veien trenger ikke være den beste
	- En bedre vei kan gå gjennom flere noder (men lavere kantvekter)

**Kompleksitet**
- Kjøretid avhengig av hvordan prioritetskøen implementeres.
- Originale dijkstra
	- Brukte en tabell og fant minimumsnoden med lineær søk
	- $\Theta(N^2)$
- Optimalisert dijkstra
	- Bruke min-heap som prioritetskø (minste verdi øverst)
	- $\Theta((N+K)log \text{ }N)$  
- Felles: Vi ser på heap, samt sortert og usortert tabell
	- Initialisere nodene: $\Theta(N)$
	- Lage prioritetskø: $\Theta(1)$, siden alle utenom én node starter med samme avstand.
	- Hente minste (gjøres N ganger)
		- Heap: $O(\log N)$, sort.tab: $\Theta(1)$, usort.tab: $O(N)$
	- Endre prioritet (max K ganger)
		- Heap: $O(\log N)$, sort.tab: $O(N)$, usort.tab: $O(1)$
	- Totalt
		- Heap: $O((K+N)\log N)$, best hvis $K$ er proposjonal med $N$
		- Sortert tabell: $O(KN)$ 
		- Usortert tabell: $O(N^2)$, best hvis $K$ er proposjonal med $N^2$

#### 8.2.2 Bellman-Ford algoritmen
- Tåler kanter med negativ vekt, så lenge det ikke er negative rundturer
**Virkemåte**
- Bruk samme system med forgjengere
- Startnode har distanse $0$, andre $∞$
- Gjenta $N - 1$ ganger:
	- Gå gjennom alle de $K$ kantene og *for hver kant*: Hvis `dist[u]+vekt<dist[v]`, oppdater `dist[v]`
- Nå er de korteste veiene funnet.
- Sjekk negative sykler/rundturer: hvis noen avstander fortsatt oppdateres, finnes en negativ sykel

| Bellman-Ford                   | Dijkstra                          |
| ------------------------------ | --------------------------------- |
| Håndterer negative kantvekter  | Kun positive vekter               |
| O(V·E) - tregere               | O((V+E)log V) - raskere           |
| Sjekker alle kanter V-1 ganger | Besøker hver node én gang         |
| Kan detektere negative sykler  | Kan ikke håndtere negative vekter |
**Når bruke Bellman-Ford?**
- Når grafen har negative kantvekter
- Når du må detektere negative sykler
- Ellers bruk Dijkstra (raskere)

**Kompleksitet**
- Å sjekke alle kantene tar $O(K)$ tid.
- Det gjør vi $N-1$ ganger
- Kjøretiden blir $\Theta(NK)$
	- Som når Dijkstra bruker sortert tabell
### 8.3 Minimale spenntrær
- **Spenntre:** tre som kobler sammen alle nodene ved hjelp av $N-1$ kanter valgt ut blant de $K$ kanter i grafen. Ingen rundturer/sykler
- **Kostnaden** for et spenntre er *summen* av vektene på alle kantene som er med i spenntreet
- **Minimalt spenntre:** er et deltre av en graf som:
	1. Forbinder alle noder
	2. Har minimal konstnad
	3. Ingen sykler
	- To algoritmer for å finne minimale spenntrær i en graf: kruskals og prim
#### 8.3.1 Kruskals algoritme
**Formål:** Finner minimal spenntre ved å alltid velge billigste kanten som ikke skaper sykel
1. Sorter alle kanter etter vekt
2. Velg billigste kant som ikke lager sykel
3. Gjenta til vi har $N-1$ kanter for $N$ noder
4. Starter med å se på alle nodene som hvert sitt separate tre
5. Bruker den billigste kanten vi kan for å koble sammen to trær til ett.
	- Må unngå rundturer/sykler, kan ikke alltid bruke aller billigste kant hvis den kobler to noder i samme tre.
6. Fortsetter til alt henger sammen i et spenntre
7. Minimalt, fordi vi alltid velger billigste brukbare kant
8. Må holde orden på mengder av trær underveis, for å kunne sjekke hvorvidt nodene i hver sin ende av kanten tilhører separate trær. Hvis begge ender av kant er i samme gruppe $\to$ sykel
	- Når trær slås sammen, må vi oppdatere så alle nodene tilhører samme tre før vi går videre

**Kompleksitet** 
- I en graf med spenntre er $K\geq N$
- Å sortere kantene tar $O(K \log K)$ tid
- De beste implementasjonene klarer dette på $O(K \log K)$tid
#### 8.3.2 Prims algoritme
Finner minimalt spenntre ved å gradvis utvide ett tre fra en startnode
**Virkemåte**
1. Start fra en vilkårlig node
2. Se på alle kanter som går ut fra treet til noder utenfor
3. Velg billigste kant til en ny node
4. Gjenta til alle noder er med i treet.
**Implementasjon**
- Ligner på Dijkstra, bruker forgjengere og prioritetskø
- Velger en tilfeldig startnode
	- Oppdaterer naboenes prioriteter utfra vekt på kantene
- Kobler på nærmeste nabo, og oppdaterer dennes naboer
	- Forskjellen fra Dijkstra: Noder har ikke «egen distanse», de nulles ut når vi tar dem ut av køen. Det er bare avstand fra treet som betyr noe nå.
- Finner nærmeste nabo til dette treet, kobler den på.
	- Må se på alle kanter ut fra treet, ikke bare fra én bestemt node
- Fortsetter å la treet vokse, til alle noder er med
- Minimalt, fordi vi alltid bruker den billigste kanten som bringer en ny node inn i treet.
**Kompleksitet**
- Samme som for Dijkstra, nesten identisk program
- Grafer som har spenntre, henger alltid sammen. $K \geq N$
	- sortert tabell $O(KN)$ er dermed ikke aktuelt
	- usortert tabell $O(N^2)$
	- heap $O((K + N) \log N$ blir $O(K \log N)$

| Prim                              | Kruskal                      |
| --------------------------------- | ---------------------------- |
| Vokser **ett tre** fra startnode  | Slår sammen **flere trær**   |
| Ser på kanter fra tre til utenfor | Ser på alle kanter globalt   |
| Trenger ikke sortere alle kanter  | Må sortere alle kanter først |
| Bedre for tette grafer            | Bedre for spredte grafer     |
### 8.4 Maksimum flyt
#### Flytnettverk
- Kantene er transportkapasitet
- Vi ønsker å transportere mest mulig fra en Kilde (K) til et Sluk (S), to spesielle noder i grafen
- Andre noder kan bare sende videre, ikke lagre noe. Sum av flyt *inn* er lik sum av flyt *ut* for vanlige noder
- For hver kant vises flyt og kapasitet på formen $\text{flyt}/\text{kapasitet}$
**Maksimal flyt-problemet:** å finne den maksimale flyten vi kan ha gjennom et flytnettverk nettverk, fra kilde til sluk, uten å gå over kapasitetgrense noen steder.

**Flere kilder og sluk**
- Bare én kilde og ett sluk ser ut som en begrensnin
	- Mange fabrikker, mange kjøpesentre…
- Vi kan ha flere, men da kan vi transformere til det enklere problemet:
	- Kildene og slukene gjøres om til vanlige noder, og innfører en superkilde og supersluk, med uendelig kapasitet på tilkoblingene.
![[Screenshot 2025-10-08 at 01.19.25.png]]

**Flytnettverk:** rettet graf hvor hver kant$(n,m)$ har en kapasitet $k(n,m)$. Flyten gjennom kanten kalles $f(n,m)$. Antar at alle noder ligger mellom kilden og sluket
- All transport må gå hele veien fra K til S, ingen mellomlagring
- Flyt i en kant kan maksimalt være lik kapasiteten, men kan være mindre
- Positiv flyt i en retning, kan sees på som negativ flyt i motsatt retning
- *Regler:*
	- **Kapasitetregelen:** $f(n,m) \leq k(n,m)$. Flyten må være mindre eller lik kapasiteten
	- **Symmetriregelen:** $f(n,m) =-f(m,n)$. Flyt i én retning sees på som negativ flyt i motsatt retning
	- **Flytkonversering:** Summen av flyt inn og ut fra noder utenom kilden og sluk må være null. Nodene har ingen lagringskapasitet.
- **Total flyt i grafen kalles $|f|$ og er lik:**
	- Summen av alt som flyter ut av kilden
	- Summen av alt som flyter inn i sluket
	- Summen av alt som flyter på tvers av et hvilket som helst snitt gjennom grafen, der kilde og sluk havner på hver sin side
**Definisjoner**
- *Restkapasitet:* Angir hvor mye mer flyt som kan sendes gjennom en kant: $k_r(n,m)=k(n,m)-f(n,m)$ 
- *Flytøkende vei*: en vei fra kilde til sluk der kapasiteten ikke er fullt utnyttet. Vi kan øke flyt langs veien tilsvarende den kanten som har minst *restkapasitet*. 
- *Restnett:* En graf med oversikt over *restkapasitet* og muligheter for forandring i flyten i hele grafen
- *Flytkansellering:* I blant kan vi øke flyten ved å «snu» flyten gjennom en kant som har uhensiktsmessig flyt. 
- *Flaskehals:* kanten med minst restkapasitet langs en *flytøkende vei*
- Når det går en flyt $f$ fra $a$ til $b$, minker *restkapasiteten* fra $a$ til $b$ med $f$. Men *restkapasiteten* fra $b$ til $a$ øker med $f$. 
	- Når vi har flyt i motsatt retning, kan restkapasiteten være større enn totalkapasiteten for den aktuelle kanten. 
#### Ford Fulkerson metoden
- Finn en hvilken som helst flytøkende vei (vei fra kilde til sluk hvor det finnes kapasitet i riktige retning langs veien)
- Finn kanten med minst restkapasitet i denne veien
- Øk flyten langs hele veien tilsvarende denne restkapasiteten
- Gjenta til det ikke fins fler flytøkende veier
- Metoden har en svakhet; den kan ta lang tid!
**Virkemåte**
1. Initialiser flyten til $0$ for alle kanter
2. Finne en ledig flytøkende vei
3. Send så mye flyt som mulig
4. Gjenta til ingen veier fins Flyten økes så mye som mulig, altså like mye som kapasiteten i den kanten langs veien som har lavest kapasitet.
#### Edmond Karp metoden
- Som Ford Fulkerson, men:
- Bruk BFS på *restnettet* for å finne en flytøkende vei.
	- Start BFS i kildenoden og avsutter når man finner sluket
	- Finner (og bruker opp) de flytøkende veiene som har færrest kanter først.
	- Unngår dermed «fella» som Ford Fulkerson kan gå i
- I praksis på ark
	1. Bruk BFS for å finne kortest vei fra K til S i restnettet
	2. Send flyt langs veien
	3. Oppdater restnettet
	4. Gjenta
- Kompleksitet $O(NK^2)$

| Ford-Fulkerson                          | Edmonds-Karp                           |
| --------------------------------------- | -------------------------------------- |
| Bruker DFS eller annen søkemetode       | Alltid BFS                             |
| O(K·max_flow) - avhenger av tallverdier | O(NK²) - avhenger kun av grafstørrelse |
| Kan være treg på spesielle grafer       | Garantert polynomial tid               |
| Enklere konseptuelt                     | Mer forutsigbar ytelse                 |
