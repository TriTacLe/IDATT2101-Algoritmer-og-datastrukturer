## A*-raskere korteste vei
### Litt Dijkstras
**Varianter og forbedringer for Dijkstras algoritme**
- Stopp i tide
- Finne nærmeste ressurs
- Smart bruk av prioritetskø
- Fibonacci-heap
**Stopp i tide**
- Trenger ikke å behandle alle noder i grafen
- Når målnoden hentes ut av prioritetskøen, har vi beste vei dit og kan bryte av
- Ikke *asymptotisk* bedre
- Vinner likevel mye når:
	- vi prøver å finne veien til flyplassen
	- vi har hele verdenskartet i grafen
- NB! Ikke la algoritmen stoppe når den finner målnoden
	- Første vei trenger ikke å være den beste
	- Stopp når målnoden plukkes ut av køen
**Finner nærmeste ressurs**
- Noder kan ha ressurs som: butikk, kafé
- Ønsker "de 10 nærmeste butikkene"
	- Flere mål
	- Vet ikke hvilke noder dette er
- Hvordan?
	- Gjør et normalt Dijkstra søk, start der vi er
	- Sjekk nodene når de plukkes ut av prioritetskøen
	- Hvis noden "er butikk", legg den i resultattabellen
	- Stopp når vi får 10 slike
	- Presenter resultatet (10 butikk med avstand og info)
**Smart bruk av prioritetskøen**
- Boka legger alle nodene inn i prioritetskøen med én gang
- Vi trenger bare legge inn de som blir "funnet"
	- først bare startnoden
	- så hver node som vi finner en vei til.
- Mindre prioritetskø $\to$ mindre arbeid å finne minste (nærmest)
- En prioritetskø med færre noder i, kan påvirke kjøretiden
	- Dijkstras med usortert tabell går fra $O(N^2)$ til $O(N\sqrt{N})$
	- Dijkstras med heap går fra $O((N+K)log N)$ til $O(\frac{1}{2}(N+K)log \text{ }N)$ 
- Ikke *asymptotisk* bedre, men en halvering
**Fibonacci heap**
- Funnet opp for å forbedre Dijkstras algoritme fordi den har lavere kjøretid på noen operasjoner
- Vanlig heap trenger $O(log\text{ }N)$ tid for å
	- sette inn elementer
	- endre prioritet
- Fibonacci-heap klarer dette på $O(1)$ tid i gjennomsnitt. 
- Dijkstra med heap: $O((N+K)log\text{ }N)$ 
- Dijkstra med fibonacci-heap: $O(N log \text{ }N+K)$ 
- Brukes lite da strukturen er tungdrevet
### A*
- A* er en forbedring på Dijkstras algoritme
- Forskjell mellom A* og Dijkstra er *prioriteringen*
	- Dijkstras prioriterer noder utfra deres avstand til startnoden. Dette fører til at søket sprer seg som en ring fra startstedet. Noden nær startnoden undersøkes først, i alle retninger
	- A* bruker i tillegg et *estimat* for avstanden mellom noden og målet. Nodens prioritet blir avstand fra startnoden og, pluss estimatet for veien videre til målet. Når A* plukker neste node, vil det være noden hvor: avstand til startnoden + estimert avstand til målet blir til sammen *minst mulig*. Områder nærmere målnoden blir mer interessante. Der øker avstand fra startnoden, men estimert avstand minker til målet. Summen av startavstand og målavstand endrer seg lite langs vei mot målet, og øker i alle andre retninger.
- Mulig for noen grafer, ikke alle
- Veikart:
	- Noder har koordinater
	- Avstand: $\sqrt{(x_{1}-x_{2})^2+(y_{1}-y_{2}^2)}$ (rettlinja avstand mellom to punkter)
- Problemet med Dijkstra:
	- Søket ekspanderer i en sirkel rundt start
	- leter for mye i feil retning.
- **Valg av estimat**
	- Lavt estimat går bra
	- Dijkstra tilsvarer A* med estimat $0$
	- Estimat kan godt være lavere enn virkelig avstand, men aldri høyere.
	- Må aldri estimere for høyt, (miste korteste vei)
	- Jo høyere estimat, jo bedre (smalere ellipse)
- Noder nærmere målnoden havner tidligere i køen
- Noder i feil retning undertrykkes
- Søket blir ellipseformet, mindre areal enn sirkelen.
## ALT (optimalisert A*)
**Behovet**
- Problemet med Dijkstra: sirkulært søk, tar tid
- Problemet med geometrisk A*:
	- tyngre regning
	- hjelper bare litt
	- elliptisk søk, men: høyeste fartsgrense brukes lite, vi får "runde ellipser"
- ALT gir bedre estimater

**Alt:** er A* + *landemerke-heuristikk*
Metoden går ut på å gjøre noen få noder til *landemerker*. Fra disse landemerkene beregner vi avstander *til og fra alle andre noder* på kartet ved å kjøre *Dijkstra* på hvert av landemerkene (*preprosessering*). Bygger opp tabeller med avstand mellom hver node og landemerkenodene.
- Landemerker: noder hvor vi beregner
		- avstand til alle andre noder (Dijkstra)
		- avstand fra alle andre norder (Dijkstra på omvendt graf)
	- beregningene gjøres som en *preprossessering* av kart
	- bør ligge spredt rundt kanten på kartet
- Noders avstander til landemerker, gir mye bedre avstandsestimater for A*
- 3-8 landemerker gir raske søk, for mange krever mye minne
- Landemerkene brukes til å lage en **heuristisk funksjon** for A* søk ved hjelp av triangelulikheten

**Landemerket bak startpunktet**
- **Triangelulikheten: $\delta(n, m) \geq \delta(L, m) - \delta(L, n)$**
	- $\delta$, distanse mellom to noder
	- $n$, en node
	- $L$, landemerket 
	- $m$, mål
- **Ulikheten må gjelde:**
	- Hvis $\delta(n,m)$ er *mindre* enn differansen, må det finnes en kortere vei fra $L$ til $m$ via $n$
	- Men $\delta(L,m)$ og $\delta(L,n)$ er korteste distanser, funnet med Dijkstras algoritme
	- Så kortere vei fins ikke og ulikheten er korrekt
- **A* kan bruke $\delta(L, m)- \delta(L,n)$ som estimat for $\delta(n,m)$** 
	- Det blir aldri for mye
	- Lett å beregne, når vi har avstandene *fra* landemerket
	- ligger tett opptil virkelig avstand
		- hvis $n$ ligger omtrent på veien fra $L$ til $m$
- Vi har flere landemerker
	- for hver node $n$, bruk det landemerket som gir høyest estimat. Hvis estimatene blir negative kan vi bruke 0 som estimat.
	- kan ikke bli for stort
	- jo større, jo bedre
- Bedre resultat enn geometrisk A*
	- Prioriterer økende avstand til landemerker
- Svakhet
	- Fortsatt avstikkere inn på sideveier
	- Dette kan unngås

**Landemerket etter målet**
- **Triangelulikheten: $\delta(n, m) \geq \delta(n, L) - \delta(m, L)$**
- **Ulikheten må gjelde (som sist):**
	- Hvis $\delta(n,m)$ er mindre enn differansen, må det finnes en kortere vei fra $L$ til $m$ via $n$
	- Men $\delta(L,m)$ og $\delta(L,n)$ er korteste distanser, funnet med Dijkstras algoritme
	- Så kortere vei fins ikke og ulikheten er korrekt
- **A* kan bruke $\delta(n, L)- \delta(m,L)$ som estimat for $\delta(n,m)$** 
	- lett å beregne, når vi har avstand *til* landemerket
	- ligger tett opptil virkelig avstand
		- hvis $m$ ligger omtrent på veien fra $n$ til $L$
- **Styrke**
	- A* prioriterer kortere avstand til landemerker
	- Ingen blindveier, for da øker avstand til landemerket
- **Svakhet**
	- Kan ta avstikkere som *har* vei til landemerket.

**ALT, kombinert algoritme**
- Vi bruker både landemerker *bak* startspunktet, og *etter* målet
- For alle landemerker $L_{x}$ beregner vi
	- $\delta(L_{x},m)-\delta(L_{x},n)$ og
	- $\delta(n,L_{x})-\delta(m,L_{x})$
- A* bruker den *største av alle differansene som estimat* for $\delta(n,m)$
- Hvis den største differansen er negative, bruker vi $0$
**Kostnad og fordeler**
- Antall prosesserte noder minkes
- Kartet må forhåndsprosesseres
- For hver eneste node må vi lagre: avstand fra og til hvert landemerket
- Europakart med 22 mill. noder og 6 landemerker
	- avstander som int (4 byte)
	- $22 000 000\cdot 6\cdot2\cdot4 ≈ 1\text{GB}$
**Velge landemerker manuelt**
- Velger landemerkene ut i fra hvordan kartet brukes. Bør være et landemerke bak start og et etter mål
- Landemerker på ytterkantene av kartet, der hovedveier slutter
**Velge landemerker automatisk**
- Dijkstra-søk fra en tilfeldig node. Landemerke $L_{1}$ blir den som havnet lengst unna
- Søk fra $L_{1}$, $L_{2}$ blir noden lengst unna $L_{1}$
- Søk fra $L_{2}$, $L_3$ blir den hvis sum av avstander til $L_1$ og $L_2$ blir størst
- Søk fra $L_n$, $L_{n+1}$ blir noden hvis sum av avstander til tidligere landemerker er størst.
- Må sjekkes manuelt. Risiko for å at samme landemerke velges om igjen
**Tabeller og cache**
- Kjøretid: antall noder slått opp ganger antall landemerker
	- Flere landemerker gir færre noder slått opp
- Hva er best for cache?
