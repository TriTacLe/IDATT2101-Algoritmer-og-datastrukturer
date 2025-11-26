## 1. Kompleksitetsanalyse
Finne ut tid og plass som går med for å kjøre en algoritme
- Avhengig av antall data, $n$
- Kan avhenge av hvordan dataene "ser ut"
- Kjøretiden $T$ for en algoritme er proposjonal med $f(n)$, der $n$ er størrelsen på datamengden
### Tidskompleksitet
- Finne ut antall operasjoner (kjøretid) som utføres når algoritmen kjøres
- Operasjoner som:
    - Summere to tall, $a+b$
    - Tilordning, $a=b$
    - Enkle tester, `if(a>b)`
### Asymptotisk analyse
- Hva som skjer når $n \rightarrow ∞$
- Teller bare løkker og metodekall
**$O$ - en øvre grense for kjøretid**
- Kjøretiden kan aldri bli verre enn det som angis med $O$-notasjon ![[Screenshot 2025-08-21 at 11.54.45.png]] Hvis dette er tilfelle, skriver vi $f(n) \in O(g(n))$ og $g(n)$ er asymptotisk øvre grense for $f(n)$
**$\Omega$ - en nedre grense for kjøretid**
- Denne notasjonen brukes for å angi best mulig kjøretid. ![[Screenshot 2025-08-21 at 12.47.54.png]] Hvis dette er tilfelle, skriver vi $f(n) \in \Omega (g(n))$ og $g(n)$ er en asymptotisk nedre grense for $f(n)$
**$\Theta$ - en øvre og nedre grense**
- Notasjonen angir både øvre og nedre grense for kjøretid ![[Screenshot 2025-08-21 at 12.55.58.png]] Sier at $g(n)$ begrenser $f(n)$ asymptotisk
### Praktisk betydning
- Ulike kjøretid gir ulik ganger kjøretid:
    - Lineær, $n$
    - Kvadratisk, $n^2$
    - Kubisk, $n^3$
    - Konstant tid, $1$: tiden varierer ikke med $n$
---
## 2. Rekursjon
### 2.1 Rekursjon
- _Rekursiv algoritme:_ en algoritme som kaller seg selv
- Indirekte rekursjon: algoritme kaller en annen algoritme, og den andre kaller den første igjen.
- Rekursiv datastruktur: datatype der variablene inneholder en peker eller referanse til en annen variabel av samme type.
#### Basis og rekursjonstrinn
- For at rekursjon skal kunne brukes må to krav være oppfylt:
    - Ha et lett tilfelle (en basis) hvor problemet kan løses direkte uten bruk av rekursjon. ex: test om påstanden gjelder for basisen $n=1$
    - og kunne dele problemet i nye problemer som er nærmere den lette løsningen. ex: anta at påstanden gjelder for $n-1$
- Er det enkelt å lage algoritmen ikke-rekursiv, gjør det.
- Er det enkelt å lage algoritmen rekursiv, gjør det.
- Vil få uendelig rekursjon hvis man ikke passer på at man har en basis og at man for hvert rekursjonsnivå kommer nærmere basisen.
#### Rekursjon og induksjon
Induksjonsbevis kan brukes til å bevise at en rekursiv algoritme er riktig og at den har en bestemt tidskompleksitet
### 2.2 Splitt og hersk
Dele problemet på midten. Ex: Binærsøkealgoritmen
### Mestermetoden
![[Screenshot 2025-08-27 at 13.22.06.png | 500]]

---
## 3. Sortering
### 3.1 Sorteringsproblemet
- Inndata: $n$ tall: $a_{1}, a_{2},...,a_{n}$
- Utdata: Tallene ordnet i stigende orden, slik at $a_{1} \leq a_{2} \leq...\leq a_{n}$
- Tar altså en uordnet mengde tall og plasserer dem i stigende rekkefølge
- Faktorer som påvirker effektiviteten på sorteringsalgoritme:
    - Størrelsen på datamengden
    - Hvilken grad den er sortert
- Noen ganger kopieres tall over i en ny tabell hvor de er blir satt i rekkefølge. Dette krever dobbelt så mye minne som det er data til
### 3.2 Standardmetode for å bytte om tall
```C
void switch (int *i, int *j)
{
	int k = *j;
	*j = *i;
	*i = k;
}
```
### 3.3 Innsettingssortering
```Java
public static void innsettingssort(int [] tall){
	for(int i = 1; i < tall.length; ++i){
		int verdiSomSettesInn = tall[i]
		int vIdx = i - 1;
		
		while (vIdx >= 0 && tall[vIdx]>verdiSomSettesInn){
			tall[vIdx + 1] = tall[vIdx];
			vIdx--;
		}
		tall[vIdx + 1] = verdiSomSettesInn
	}	
}
```
- Effektiv for små datamengder, typisk under 10 elementer ($O$-notasjon er en grenseverdi som gjelder store datamengder, ikke små)
- Setter ett og ett element på plass blant de sorterte
- $\Omega(n)$ om flyttingen ikke skjer i det heletatt
- $O(n^2)$ om betingensel slår til hver eneste gang
### 3.4 Boblesortering

```Java
public static void boblesort(int[] tall){
	for(int i = 0; i<tall.length - 1; i>0; i--){
		for(int j = 0; j<i;i++){
			if(tall[j]>tall[j+1]){
				int temp = tall[j]
				tall[j] = tall[j+1]
				tall[j+1] = temp
			}
		}
	}
}
```

- Går gjennom tabellen $n-1$ ganger.
- Hver iterasjon sjekker alle tall som står etter hverandre, og bytter plass hvis dette tallet er større enn neste. Det største tallet "synker" til sisteplass i tabellen i første iterasjon, mens små tall "bobler" opp. I neste iterasjon er det ikke nødvendig å ta med det siste tallet, så hver iterasjon blir ett tall kortere
- $\theta(n^2)$
### 3.5 Velgesortering

```Java
public static void velgesort(int[] tall){
	for (int i = 0; i < tall.length - 1; i--){
		int maxIdx = 0;
		for (int j=1; j <= i; j++){
			if (tall[j] > tall[maxIdx]){
				maxIdx = j
			}
		}
		if (maxIdx!=i){
			int temp = tall[i]
			tall[i] = tall[maxIdx]
			tall[maxIdx] = temp
		}
	}
}
```

- Finn største verdi, bytt den inn på plass $n-1$
- Bytt nest største inn på plass $n-2$
- Fortsett slik til vi har byttet nest minste inn på plass $1$
- Velgesorteringen gjør like mange sammenlikninger som boblesortering, men færre ombyttinger. Velgesortering bytter om tall maksimalt én gang for hvert iterasjon av den ytre løkka.
- $\Theta(n^2)$

### 3.6 Om kvadratiske sorteringsalgoritmer - sortere ved å bytte naboer
- Sorteringsalgoritmer som bare bytter nabotall er $\Omega(n^2)$
    - Invertering: to tall som står feil ift hverandre: `t[i]>t[j]`, mens `i < j`
    - En baklengstabell har $\frac{n(n-1)}{2}$ inverteringer
    - En tilfeldig uordnet tabell har $\frac{n(n-1)}{4}$ inverteringer
    - Å bytte to naboer retter en invertering

### 3.7 Shellsort
- Sammenlikner og bytter om nødvendige tall som står langt fra hverandre i tabellen
- Når $s=1$ utføres en vanlig innsettingssortering
### 3.8 Flettesortering
- Splitt og hersk for sortering
- Består av to metoder:
    - Rekursive flettesort, som gjør oppdelingen
    - Metode som slår sammen sorterte lister
- Deler tabellen inn i to halvparter (to del-tabeller)
- Nå kan vi sortere del-tabellene hver for seg rekursivt med flettesortering
- Deretter deler vi opp i mindre og mindre usorterte tabeller helt til vi får minitabeller med bare ett tall hver - rekursjonen stopper når del-tabell har størrelse 1
- Så "flettes" de sorterte deltabellene sammen til en sortert tabell.
- $T(N)=2T(\frac{n}{2})+n \to \Theta(n~log~n)$
### 3.9 Quicksort
- Raskeste sorteringsalgoritmen for generelle data.
- Splitt og hersk
- Hvordan den funker:
    - Tabellen deles i to deler, hvor alle tall i den laveste delen har lavere verdi enn alle tall i den høyeste delen.
    - Delingen foregår ved at vi velger ett tall fra tabellen som delingstall (_pivot_) og flytter mindre tall mot venstre og større tall mot høyre. Delingsposisjonen blir stedet hvor _pivot_ havner
    - De to deltabellene sorteres hver for seg med rekursiv bruk av quicksort.
    - Ender oppdelingen når vi får deltabeller med bare ett tall i. Ferdig sorter

```Java
public static void quicksort(int[] tall, int venstreEnd, int hoyreEnd){
	if (hoyreEnd - vesntreEnd>2){
		int pivot = splitt(tall, venstreEnd, hoyreEnd)
		quicksort(tall, venstreEnd, pivot - 1)
		quicksort(tall, pivot + 1, hoyreEnd)
	} else {
		median3sort(tall, venstreEnd, hoyreEnd)
	}
}
```

- Quicksort er ikke så effektiv for små datamengder. Derfor burde man bryte rekursjonen før man kommer ned til små tall per tabell. Tabellene blir da sortert av en mer effektiv metode for små tabeller.
#### 3.9.1 Finne en metode som finner _pivot_ effektivt
- Å finne medianen er for tidskrevende
- Velge tilfeldig tall? Uheldig hvis man velger det minste eller største tallet som pivot og får $\theta(n^2)$.
- Finne medianen av første, siste og midterste tallet i tabellen og bruker det som pivot. Øker sjansen for et godt pivot.
```java
private static int median3sort(int[] tall, int venstreEnd, int hoyreEnd){
	int median = (venstreEnd + hoyreEnd)/2;
	if(tall[venstreEnd]>tall[median]){
		int temp1 = tall[median];
		tall[median] = tall[venstreEnd];
		tall[venstreEnd] = temp1;
	}
	if(tall[median]>tall[hoyreEnd]){
		int temp2 = tall[hoyreEnd];
		tall[hoyreEnd] = tall[median];
		tall[median] = temp2;
	}
	if (tall[venstreEnd] > tall[median]){
		int temp3 = tall[median];
		tall[median] = tall[venstreEmd];
		tall[venstreEnd] = temp3;
	}
	return median
}
```

- **Analyse:** Metoden inneholder ingen løkker, bare if-tester og noen sammlikninger. Begge er $\Theta(1)$, så kjøretiden for metoden er $\Theta(1)$
#### 3.9.2 Metoden splitt
```Java
private static int splitt(int[] tall, int venstreEnd, int hoyreEnd){
	int median = median3sort(tall, venstreEnd, hoyreEnd);
	int pivot = tall[median]
	
	bytt(tall, midtIndeks, hoyreIndeks - 1); // flytt pivot ut av veien
	
	int endpunktV = venstreEnd;
	int endpunktH = hoyreEnd - 1;
	
	for(;;) {
		while(tall[++emdpunktV]<pivot){}
		while(tall[--endPunktH]>pivot){}
		if (endpunktV >= endPunktH) break;
		bytt(tall, endpunktV, endpunktH);
	}
	bytt(tall, endpunktV, hoyreEnd - 1); // Pivot på endelig plass
	return endpunktV
}
```

- Partisjonering: velger en _pivot_, flytter tall _mindre_ enn pivot til _venstre_, og tall _større_ enn pivot til _høyre_, setter pivot på sin endelig plass, og returnerer pivot-indeksen
#### 3.9.4 Forbedringer og farer
- Forbedringer
    - Legge litt arbeid i å finne en bra delingsverdi
    - Små deltabeller kan sorteres med andre metoder som innsettingssortering
- Feller
    - Skjevdeling med 0 elementer, heng
    - For dyp rekursjon, kræsj
    - Skjevdeling når nøklene er like, $n^2$
    - Skjevdeling hvis tabellene er sortert fra før av $n^2$
    - Tidskrevende tester
**Hvordan unngå farene?**
- Skjevdeling med 0 elementer
    - Fikse med programmering: Velge pivot mellom de to delintervallene - sorterer ikke pivotelementet om igjen
    - Pivot velges som median av 3 (eller 5,7,9)
- For dyp rekursjon
    - Kombinere iterasjon og rekursjon
    - Bytte til heapsort $O(n \text{ }log \text{ n})$
- Skjevdeling når nøklene er like
    - Fikse med programmering: Intervallene bør bli like store
- Skjevdeling hvis tabellene er sortert fra før av
    - Unngås ved å ha midterste tall som pivot
- Tidskrevende tester
#### Quicksort med to delingstall
- Velger to delingstall, $p_1, p_2$ der $p_1 \leq p_2$
- Fordel tallene i tre grupper istedenfor to
    1. De som er mindre enn $p_1$
    2. De som er $\geq p_1$ og $\leq p_2$
    3. De som er større enn $p_2$
- De tre delene sorteres rekursivt
    - Hvis $p_1 = p_2$ trenger vi ikke å sortere midterste intervallet (alle er like).
- Litt raskere
    - færre sammenligninger, mindre rekursjon
#### Quicksort med tre delingstall
- Tre delingstall, deler tabellen i fire intervaller
- Nesten som to rekursjonstrinn av vanlig quicksort
- Enda flere pivots er mulig, men
    - fryktelig komplisert.
    - stadig mer arbeid, vanskeligere å vinne noe
### 3.10 Nedre grense for kompleksitet og lineær sortering
Alle algoritmer der man må sammenligne tallene med hverandre å bruke $\Omega(n\cdot log \text{ }n)$ tid.
**Sortering i lineær tid**
- Kan sortere bedre enn $\Omega(n\cdot log \text{ }n)$
- Men man må gjøre det på annet vis enn ved å sammenligne elementene.
### 3.11 Tellesortering
Bruker tre tabeller
- **Inn:** inneholder tallene som sorteres
- **Ut:** som får tallene i sortert orden
- **ht:** hjelpetabell som holder orden på hvor mange ganger hvert av tallene i intervallet $0...k$ finnes i $inn$ tabellen.
- Sammenligner ikke tallene og kan ikke sortere alle datatyper
- For hvert element i _inn_ tabellen finner vi hvor mange elementer vi har som er mindre eller lik det. Med denne informasjonen kan vi plassere elementet på rett plass i _ut_ tabellen. Ex: hvis det fins 17 tall mindre enn $x$, setter vi $x$ på 18 plass. Hjelpetabellen fikser dette. Om det er flere like tall må vi ikke plassere dem på samme plass.
- Bruker tallene i hjelpetabellen som indeks
- $\Theta(n+k)$
- Egner seg når tallene ligger tett og er heltall
- Hvis tallene er spredt er det kanskje mer gunstig å bruke $O(n \text{ }log \text{ n})$ algoritmene.
- Slår $O(n \text{ }log \text{ n})$ hvis $k < n \text{ }log \text{ n}$
### 3.12 Radixsortering
- ikke-sammenlignende sorteringsalgoritme
- Deler nøklene opp i "sifre"
    - Sorterer på minst signifikante siffer først
    - Deretter nest minst osv
    - Bruker en stabil hjelpealgoritme
- Ofte tellesortering som hjelpealgoritme
- Kjøretiden er et produkt av antall sifre og kjøretiden for den stabile algoritmen
### 3.13 Introsort
- Kombinasjon av flere sorteringsmetoder
- Bruker i utgangspunkt quicksort, dele opp og sortere rekursivt
- Forsiktig med rekursjonsdybden, max $2 \text{ }log_2\text{ }n$
    - Normalt får vi ikke så dyp rekursjon
    - Hvis det skjer, sorter gjenværende intervall med _heapsort_
    - _heapsort_ er $\Theta(n \text{ }log \text{ }n)$ i alle tilfeller.
- Når et restintervall er lite nok (typisk 16–50)
    - Sorter det med innsettingssortering, i stedet for quicksort
    - Fordi insettingssort er raskere enn quicksort på små del-tabeller
    - raskere enn å bare bruke quicksort
- Introsort er dermed rask (og trygg) i alle tilfeller
    - godt egnet som standard sorteringsalgoritme
---
## 4. Liste, kø, stakk og trær

## 4.1 Lister
**Abstrakt datatype:** begrep som vi knytter bestemte operasjoner til, men vi sier ingenting om hvordan den er implementert.
**Liste:** samling av data av samme datatype
- Kan være sortert
- Lineær datastruktur, der hvert element har et element foran og bak seg bortsett fra det første og siste
- Hvert objekt kaller vi et element i lista, og vi må kunne:
    - opprette lista
    - finne ut hvor mange elementer det er i lista
    - sette inn elementer i lista
    - fjerne elementer fra lista
    - tømme lista
    - oppslag i lista: finne et element i lista når vi vet hvor det står
    - søking i lista: finne et element i lista når vi vet hvilke verdi det har
    - gå gjennom alle elementene i lista sekvensielt og gjøre noe med hvert enkelt
    - finne største og minste
    - evt sortere listeelementene mhp et kriterium
### 4.1.1 Liste implementert ved hjelp av en tabell
**Opprette lista, finne lengden og tømme den**
- Angir bestemt kapasitet (makslengde) før man tar i bruk tabellen `int tab[10]`
- Finne kapasitet og lengde er $O(1)$ operasjoner
- Hvis man ønsker å tømme lista, setter man lengden til $0$. Også en $O(1)$ operasjon
**Sette inn/fjerne elementer fra lista**
- Må sette inn elementet og øke lengden til lista, $O(1)$
- Når man når _kapasitetgrensa_ får vi et problem. For å få plass til flere elementer må lage en ny og større tabell, og kopiere over alle elementene i forrige tabell til den nye tabellen, før vi setter inn det nye elementet. Hvis vi har $n$ elementer er dette en $O(n)$ operasjon. Kan unngå dette ved å sørge for at man starter med stor nok kapasitet, men det kan hende vi bruker unødvendig mye plass.
- Hvis vi vil sette inn et nytt element på en annen plass enn bakerst må vi flytte på elementer. Hvis vi ønsker at det nye elementet skal komme først må alle elementer flyttes ett hakk, noe som gir $O(n)$.
    - Hvis vi antar at vi skal sette inn element $n+1$ på posisjon $i$, vil antall elementer vi må flytte på være $(n-i)$. Hvis alle posisjoner er like sannsynlig, vil vi i gjennomsnittet flytte på $\frac{1}{2}(n+1)$ elementer
    - Viser at innsetting er $O(n)$
- Hvis vi skal fjerne et element fra en usortert liste, kan vi flytte siste element inn på plassen til det elementet som skal fjernes, og minske lengden med $1$. $O(1)$
- Hvis rekkefølgen til alle elementer skal være uforandret, må vi i stedet flytte alle de etterfølgende elementene ett hakk fremover, $O(n)$
**Sekvensiell gjennomløping av lista**
- **Traverse datastrukturen:** gå igjennom en datastruktur slik at vi er innom alle elementene.
- Trenger da bare å lage en løkke der man starter i den ene eller andre enden der elementnumret øker/minker for hver runde. Siden vi skal behandle alle $n$ elementene, må tidskompleksiteten være $n$ ganger tidskompleksiteten for det vi skal gjøre med hvert element.
### 4.1.2 Linked list
**Node:** datastruktur som består av et element samt én eller flere lenker til én eller flere andre noder. **Lenket liste:** samling av noder der hver node leder videre til neste node i lista.
```C
typedef struct Node{
	double element;
	struct Node *next
} Node;

Node *newNode(double element, Node *nextNode){
	Node *node = (*Node)malloc(sizeof(Node));
	node->element = element;
	node->next = nextNode;
	return node;
}
```
- Denne implementasjonen av en _node_ består av felt som inneholder dataene vi ønsker å lagre (double) og ei lenke til neste node.
- Rekursiv datastruktur
```C
typedef struct {
	Node *head
	int totalElement
} SimpleLinkedList;
```
- Dette er implementasjon av selve lista og består av felt som inneholder en node som er den første noden. Denne noden har da et referanse til neste node og den neste noden har da referanse til noden der igjen osv. Den inneholder også antall elementer som holder styr på antall noder
**Dobbelt lenket liste**
- Traversere lista begge veier ved å la hver node har to lenker, en som viser til neste node og en som viser til forrige.
```C
typedef struct Node{
	double element;
	struct Node *next
	struct Node *prev
} Node;

Node *newNode(double element, Node *next, Node *prev){
	Node *node = (*Node)malloc(sizeof(Node));
	node->element = element;
	node->next = next;
	node->prev = prev;
	return node;
}

typedef struct {
	double totalElements
	Node *head;
	Node *tail;
} DobbelLenke
```
## 4.2 Kø
**Kø:** lineær datastruktur der innsetting skjer bakerst og uttak skjer fremst. FIFO To måter å implementere en kø på: tabell eller linked list
- Operasjoner som trengs: sett inn, ta ut, finn verdi av første, finne ut om køen er tom. I tabell trenger vi å finne ut om det er plass i køen
- For tabell
    - Å fjerne element er ikke så enkelt. Bruker en sirkel
## 4.3 Stakk
**Stakk:** lineær datastruktur der innsetting og uttak skjer fremst. LIFO To måter å implementere en kø på: tabell eller linked list
- Linked list: trenger bare å bruke hodelenka
## 4.4 Trær
### 4.4.1 Grafer og trær
- Ulineært datastruktur
- Har ei rot
- Har noder, hver node kan lenke til mer enn en annen barnenode
- Alle utenom rota har en foreldrenode
- Noder uten barn kalles ytre noder/løvnoder
- Noder med barn kalles indre noder
### 4.4.2 Binærtre
**Binærtrær:** Posisjonstrær der hver node har maksimalt to barn
- Er ordnet og barnas rekkefølge har dermed en betydning
```C
typedef struct Binærtre {
	void *element;
	struct Binærtre *left
	struct Binærtre *right
	struct Binærtre *parent
} Binærtre;
typedef struct {
	Binærtre *rot;
} Tre;
```
**Høyde og dybde**
- **Dybde:** Antall lenker mellom _noden_ og _rota_. Rota har dybde 0
- **Høyde:** Antall lenker mellom _noden_ og _den noden som er lengs unna i et av nodens subtrær_. Treet høyde er definert som rotas høyde
**Finne nodens dypbde**
- Hvis nodene har foreldrelenke
- Hvis nodene ikke har foreldrelenke
**Finne nodens høyde**
- Høyden til en node er 1 mer enn høyden til det høyeste subtreet
### 4.4.3 Traversering
- **Traversere:** Gå gjennom og evt gjøre med alle nodene i et tre
- Fire måter: preorden, inorden og postorden, og nivåorden
- **Preordentravasering**
    - Rekkefølge: Først noden vi har funnet. Så dens venstre subtre, til slutt dens høyre subtre.
- **Innordentravasering**
    - Rekkefølge: Først venstre subtre, så noden, så høyre subtre
- **Postordentravasering**
    - Rekkefølge: Først venstre subtre, så høyre subtre, til slutt noden selv
    - Finne høyden, siden vi må finne høyden til de to subtrærne før vi kan finne nodens høyde.
- **Nivåordnettravasering**
    - Rekkefølge: Behandler nodene i ett nivå før alle nodene på neste nivå. Må bruke en kø. Legger først rota i køen og tar den ut og behandler den, så legges rotas barn i køen, så disse nodenes barn, osv
### 4.4.4 Binært søketre
- **Binært søketre:** enten et tomt tre eller et binærtre der nodene har nøkler. Nodens nøkkel er større enn nøklene til alle noder i venstre subtre, og mindre enn alle nodene i høyre subtre.
- Anvendelse: databaseindex med søk på «større, mindre, lik»
- Operasjoner: Sette inn, søke, slette
**Innsetting**
- Start med rotnoden og sjekke om den er null. Hvis ikke, sjekk om nodens nøkkelverdi er mindre enn nøkkelverdien til den vi sammenligner med, skal vi gå til venstre, hvis den er størrer, går vi til høyre. Fortsetter helt til vi finner en node som mangler det barnet vi skulle ha gått videre til og setter noden inn her
**Søk**
- Lik innsetting, men vi må også teste på lik nøkkelverdier. Hvis nøkkelverdien er lik, returnerer vi noden. Hvis vi kommer til node uten barnet vi skulle gått videre til returnerer vi NULL (nøkkelverdien finnes ikke).
**Sletting**
- Tre tilfelles, avhengig av hvor mange barn noden som skal slettes har
    1. Ingen barn. Lenka fra foreldrenoden som lenker til noden settes lik 0
    2. Ett barn. Lenka fra foreldrenoden som lenker til noden, settes til å lenke til nodens barn.
    3. To barn. Erstatter nodens element med elementet i noden som kommer etter i sorteringsrekkefølgen, som er den minste i høyre subtre.
**Tidskompleksitet for operasjoner på binærtre** Dybde og høyde
- Høyden være mellom O(log n) og O(n), vanligvis mye nærmere O(log n)
- Antar at antall noder i venstre subtre er omtrent lik antall noder i høyre subtre.
- O(log n) for å finne dypden av en node, da vi må gå fra noden opp til rota.
- Å finne høyden må vi sjekke alle muligheter, og derfor innom alle noder. $\theta(n)$
**Innsetting, søking og sletting**
- Hvis treet har høyde O(log n) vil søking og innsetting få kompleksitet O(log n)
- Tilfelle 1 og 2 får vi ingen løkke og dermed O(1)
- I tilfelle 3 har vi ei løkke O(h) og et rekursivt kall O(1). Sletting vil da O(log n) hvis h er det
- I verste fall blir disse operasjonene O(n)
### B-trær
- Ikke-binære: kan ha hundrevis av barn
- _m-veis tre_ er et tre som kan ha maksimalt _m_ barn
- Alle løvnoder er på samme nivå
---
## 5. Heap og prioritetskø
## 5.1 Heap
To slags heap: max-heap og min-heap
**Heap:** komplett binærtre med en nøkkel i hver node
- _heapegenskapen:_
    - Max-heap: Hver node har en nøkkelverdi som er $\geq$ begge barnenodenes
    - Min-heap: Hver node har en nøkkelverdi som er $\leq$ begge barnenodenes
**Anvendelser**
- Sortering og prioritetskø
- **Prioritetskø:** datastruktur hvor man kan legge inn og ta ut elementer som har ulik _prioritet_
- Bruker max-heap for å implementere prioritetskø
## 5.2 Implementasjon

### 5.2.1 Heap som tabell
- Heap er et komplett binærtre og kan implementeres effektivt som tabell
- Rotnoden i indeks 0, nodene på første nivå på indeks 1 og 2 osv.
- Trenger ikke referanse til foreldre- og barnenoder. For node i posisjon $i_{node}$ kan vi finne indeks til foreldre- og barnenoder slikt:
    - $i_{foreldre}= (i_{node}-1)/2$
    - $i_{venstre barn}=2i_{node}+1$
    - $i_{høyre barn}=2i_{node}+2$

```Java
class Heap{
	int len; //
	int node[]; //
	
	// Beregne høyre, venstre barnenode og foreldrenode
	int over(int i) { return (i-1) >> 1;}
	int venstre(int i) { return (i-1)  1;}
	int hoyre(int i) { return (i-1) 1;}
}
```
Rask beregning av indekser
- Høyreskift
- Venstreskift
- Flytter bitmønstere til siden, i totallsystemet blir det å gange og dele med 2
### 5.2.2 Metoder
- Fiks heap: Hjelpemetode. Brukes til å implementere alle andre metodene.
- Lag heap: lage en heap av uordnet mengde tall
- Hent maks: fjerner max-elementet (rota). Lett å finne det, men da er det ikke et gyldig tre lengre. Omorganiser resten så det blir en heap igjen
- Prioritetsendring:
- Innsetting
- Heapsort
- Høyden på heap er alltid $log_{2}n$ der $n$ er antall noder
**Fiks heap**
- Forutsetning: rotas bar er korrekte heaper.
- Rekursiv metode
- Finn det største barnet til rota
- Hvis det har større nøkkel enn rota, bytt plass
- Gjenta denne operasjonen med den byttede noden som rot. Den noden som flyttes ned ikke er nok å flytte ett hakk ned, men kanskje den må flyttes videre
- Stoppes når vi ikke lengre trenger å bytte, eller når en løvnode
- I verste fall må vi flytte fra topp til bunn
- Flytter nodene om de ikke følger _heapegenskapen_
    - Parameter er indeks til noden som skal flyttes
    - Antall flytt og metodekall (rekursiv) bestemmes av nodens høyde, $h$. Verste kjøretid blir $O(log \text{ }n)$ hvis noden må flyttes helt til løvnodene. Beste kjøretid er $\Omega(1)$ hvis noden står på rett plass fra før av da ingen rekursive kall nås.
**Lag heap:** lage en heap av uordnet mengde tall
- Kaller på fiks heap
**Hent maks**
- Alltid rotnoden som er størst. $\Theta(1)$
- Etter at rotnoden tas bort, må den siste noden flyttes til rot, og kort ned heap-størrelsen.
- Ofte passer den siste noden dårlig og man bruker fiks heap og lar den synke ned på rett plass
**Prioritetsendring**
- UNIX gir prioritet til prosesser som settes igang etter I/O og nedprioriterer prosesser som bruker regnekraft
- Justere prioritet og flytte node opp/ned for å beholde heapegenskapen
**Innsetting**
- Utvider plass i tabellen, setter noden inn der, og flytt noden til riktig plass iht heapegenskapen
**Heapsort:** sorteringsalgoritme som nytter heapstrukturen
- Trenger ikke heap på forhånd da algoritmen gjør hele tabellen som til max-heap.
- Start med kall til `lag_heap` som lager max-heap (hvis man vil sortere stigende rekkefølge) av usorterte data
- Plukker ut det største tallet og legges bakerst i tabellen, reduser heapstørrelsen med én.
- Kjører en løkke med `hent_max` til heapen er tom
- Etter på er tabellen sortert, med største element siste, nest største nest siste, opp til det minste som er først
- $O(\text{n log n})$ og $\Omega(n)$
---
## 6. Hashtabeller
### 6.1 Hashtabell
- Rask gjenfinning av lagrede verdier, basert på nøkler
- Rask innsetting av nye verdier (nøkler)
- Nøkkelen brukes ikke som tabell indeks
**Virkemåte**
- Hashtabell bruker en _hashfunksjon_ som kan konvertere nøkler i streng eller interger til heltall i området $\text{[0,m-1]}$ som kan brukes til å slå opp i en tabell med $m$ plasser.
- Brukes ved _write og read_
- Når vi _write_ "Tri", lager hashfunksjonen tallet ut fra navnet, og andre opplysninger om "Tri" legges på den plass. Når vi senere _reader_ "Tri" vil hashfunksjonen gi oss tallet 21345 hvor vi finner opplysningene lenket med "Tri"
- Funksjonen skal kun beregnes i $\Theta(1)$
- Tidsforbruk avhenger _ikke_ av antall data
#### 6.1.1 Lastfaktoren
- **Definisjon:** For en hashtabell med størrelse $m$ og $n$ elementer, er _lastfaktoren_ gitt ved: $$\alpha = \frac{n}{m}$$
- Full tabell: $\alpha = 1$
- Dårlig utnyttet: $\alpha \text{ nesten } 0$
- Overfylt: $\alpha > 1$
#### 6.1.2 Kollisjon
- **Definisjon:** To samme poster med ulik nøkkel kan få samme verdi av hashfunksjonen, men en tabellposisjon kan bare lagre én post
- Kollisjoner er nesten umulig å unngå da hashfunksjonen konverterer nøkler med mer enn $m$ mulige nøkkelverdier til heltall innenfor $[0,m-1]$
### 6.2 Hashfunksjoner
- Hashfunksjon tar nøkkel som parameter, og beregner en posisjon i hashtabellen.
- Beregningen skal skje $\Theta(1)$ og skal gi god spredning slik at postene fordeles jevnt utover tabellen og gir få kollisjoner
- Kan bruke ascii eller unicodeverdier for hvert enkelt tegn i strengen.
#### 6.2.1 Hashfunksjon basert på restdivisjon

```C
int restdivhash(int k, int m){
	return k % m;
}
```

- Omformer en nøkkel $k$ til en av $m$ posisjoner i hashtabellen ved hjelp av formelen: 
$$h(k)= k \text{ \% } m$$
- $m$: tabellstørrelse, $k$: nøkkel, $h$: hashfunksjon
- Virker best hvis $m$ er et primtall.
- Restdivisjon med heltall går fort.
- Dårlig hvis m er en toerpotens. For $m = 16$ vil $h$ bare avhenge av de fire siste bitene i $k$.
- Dårlig hvis $m$ er $10, 100, 1000, … h$ avhenger bare av de siste sifrene i nøkkelen
#### 6.2.2 Hashfunksjon basert på multiplikasjon
- Ganger vi nøkkelen $k$ med et tall $A$, der $0<A<1$. Produktet består av en heltallsdel og en desimaldel. Ganger desimaldelen på størrelsen på hashtabellen $m$ for å få indeksen $$h(k)=[m(kA-[kA)]$$
- Funker for alle verdier av $m$.
- Bruker desimaltall, og beregninger med desimaltall er tregere enn heltallsoperasjoner.
### 6.3 Kollisjonshåndtering
#### 6.3.1 Lenkede lister
- Hver post kan ha ei lenke til neste post, og hashtabellen blir en tabell med listehoder. ![[Screenshot 2025-09-22 at 15.24.26.png]]
- Bildet viser en hashtabell med 7 plasser (0-6) der tallene er satt inn vha restdivisjon, hvor de som har kollidert er lenket til hverandre.
- Verste situasjon er om alle elementene hashet til samme posisjon i hashtabellen
- Søketiden i ei lenket liste er proporsjonal med lengden av lista, $\frac{n}{m}$, lik lastfaktoren $\alpha$
#### 6.3.2 Åpen adressering
- Alle elementer lagres i selve hashtabellen med ingen lister eller eksterne strukturer.
- I en slik hashtabell kommer lastfaktoren $\alpha$ aldri over $1$
- Ved en kollisjon, finner vi systematisk en anne ledig plass i tabellen og legger elementet der.
- Beregner bare hvor i tabellen den neste mulige posisjonen er.
- Hva skjer om vi søker på en tom posisjon?
    - Søken sluttes når den støter på den tomme posisjonen
    - _Kan_ løses med å sette inn et markørelement for å markere slettede elementer, men dette har en annen ulempe: etter mye innsetting og sletting vil alle ledige posisjoner inneholde slette-markører og søk etter elementer som ikke fins i tabellen $O(m)$, fordi søket ikke kan gi opp før den har gått igjennom hele tabellen.
- Brukes derfor mest når vi skal legge inn elementer uten å slette annet enn når vi kvitter oss med hele tabellen.
- Tre måter å implementere åpen addressering: lineær probing, kvadratisk probing og dobbel hasing. Det som skiller seg ut er probesekvensen
##### Lineær probing
- Enkleste form for åpen addressing
- Hvis et nytt element hasher til en opptatt plass, prøver vi neste posisjon, er den opptatt går vi ett steg videre, og gir oss ikke før vi har prøvd alle plassene.
- Sikrer at vi alltid får inn nye elementer når tabellen ikke er full $$probe(h,i,m)=(h+i) \text{ mod } m$$ ![[Screenshot 2025-09-22 at 16.17.26.png|600]]
- Går fra start til slutt for å få plassert tallet
- Problem:
    - Hvis mange elementer kolliderer på samme sted får vi en lang sammenhengende kjede med "feilplasserrte" elementer som må hoppes over hvert eneste oppslag
##### Kvadratisk probing
- Forbedring ift lineær probing
- Tall som kolliderer i ett forsøk vil ikke være nødt til å fortsette å kollidere så fremt de har ulik hashverdi $$probe(h,i,m)=(h+k_{1}i+k_{2}i^2) \text{ mod } m$$ ![[Screenshot 2025-09-22 at 16.30.32.png]]
- Valg av $k_{1} \text{, } k_{2} \text{ og }m$
    - Hvis alle tre har en felles faktor, vil ikke probsekvensen komme innom alle tabellposisjonene og vi risikerer å ikke kunne plassere elementer selv om det er ledige plasser.
##### Dobbel hashing
- Kvadratisk probing har et problem: elementer som hasher til samme posisjon - hvis den er opptatt, vil begge prøve på nytt i samme sted.
- Bruker to hasfunksjoner i probsekvensen:
    1. En som angir den første posisjonen som prøves
    2. Angir hvor langt vi skal hoppe hvis det blir kollisjon. $$probe(h_{1}, h_{2}, i, m)=(h_{1}+ih_{2}) \text{ mod }m)$$ ![[Screenshot 2025-09-22 at 17.43.18.png]]
- Tall som hasher til samme posisjon, kolliderer ikke med hverandre på andre forsøk, fordi $h_{2}(k)$ lager ulike verdier for de forskjellige verdier av $k$ hvor $h_{1}(k)$ gir samme verdi
- Krav til $h_{2}(k)$
    1. Funksjonen $h_{2}(k)$ må ikke få verdi 0
    2. $h_{2}(k)$ og tabellstørrelsen $m$ må være relativt prime: ikke ha felles faktorer for noen verdi av $k$. Hvis de har en felles faktor vil ikke probesekvensen gå innom alle posisjonene.
    3. For å minimere antall kollisjoner bør $h_{2}(k) \text{ mod }m$ i største mulig grad gi ulike tall der $h_{1}(k)$ gir like tall.
- Krav om ingen felles faktor kan oppnås:
    - Hvis $m$ er et primtall, kan vi la $h_{2}(k)$ produsere tall i intervallet $[1,m]$. Ex: $h_{2}(k)=k \text{ mod }(m-1)+1$. Dette oppfylles krav 3
    - Hvis $m$ er en toerpotens, kan vi ha $h_{2}(k)$ som bare produserer oddetall. Ex: $h_{2}(k)=(2|k|+1) \text{ mod }m$
**Analyse av åpen adressering**
---
## 7. Uvekta grafer

### 7.1 Hva er en graf

**Graf:** En datastruktur som består av _noder_, og en rekke forbindelser mellom disse. Forbindelsene kalles _kanter_.
![[Screenshot 2025-10-01 at 11.12.28.png | 300]] 
**Rettet graf:** kantene har en retning **Urettet graf:** kantene ikke har en retning **Nabo:** $\text{kant(n,m)}$ sier vi at $m$ er _nabo_ til $n$ og skrives $n \to m$. **Selvløkke:** kant som begynner og slutter i samme node 
![[Screenshot 2025-10-01 at 11.19.07.png | 350]]
### 7.2 Implementasjon
- Lagre informasjon om hvilke _noder_ og _kanter_ som finnes.
- Nodene legges vanligvis i en tabell.
- Holde orden på kantene med _naboliste_ eller _nabotabell_
**Graf-fil**
- Første raden angir antall noder $N$ og kanter $K$ i grafen
- Nodene nummereres fra $0$ til $N-1$
**Naboliste** 
![[Screenshot 2025-11-12 at 19.47.42.png | 400]]
- Tabell med noder
- Hver node er et _listehode_, der hver node har vi liste over kantene
- Hver kant er et _listeelement_ og inneholder
    - Referanse til neste kant
    - Referanse til hvilken node kanten går til
Tre datatyper:
1. Kanttypen som listelement. Felt for node kanten går til, og referanse til neste kan i kantlista
2. Nodetypen, som referer til første listeelement i nodens kantliste.
3. Graftypen, som inneholder en tabell med alle nodene
- $O(n)$ plass
- Ofte lavere kompleksitet for grafalgoritmer
**Nabotabell**
![[Screenshot 2025-11-12 at 19.48.03.png | 300]]
- Todimensjonal tabell `kant[N][N]`hvor begge indekser er nodenumre
- `kant[i][j]`forteller om det er forbindelse fra node `j` til `i`. Den _første_ indeksen forteller hvilke node kanten går fra og den _andre_ forteller hvilke node kanten går til.
- $O(n^2)$ plass, greit om vi har en komplett graf
- Lett å finne om to noder er naboer.
- Tar tid å finne alle kanter ut fra en node.
**Grafalgoritmer**
- BFS
- DFS
- Topologisk sortering
- Sterk sammenhengende komponenter
### 7.3 Bredde-først søk (BFS)
Starter i en startnode $s$, og finner alle noder som kan nås fra den.
- Finner noden med avstand $x$ fra $s$ før den finner noen med avstand $x+1$.
- Kortest vei fra en startnode $s$ til andre noder
- En nodes _forgjenger_ leder oss til en annen node. Derfra kan vi finne den nodens forgjenger osv.
- Uendelig avstand brukes på noder som ikke kan nås fra startnoden.
Naboliste
- For hver node må vi sjekke alle dens kanter
- $O(N+K)$
Nabotabell
- For hver node må vi finne kantene, ved å sjekke $N$ mulige kanter
- $O(N^2)$
### 7.4 Dybde-først søk
- Prøver å følge en vei så langt som mulig
- Hvis det ikke går, gå tilbake til forrige node og prøv andre veier
- Marker besøkte noder
- Opererer _rekursivt_: hvis en node ikke er besøkt, marker den og kjør DFS på naboer
Naboliste
- For hver node må vi sjekke alle dens kanter
- $O(N+K)$
Nabotabell
- For hver node må vi finne kantene, ved å sjekke $N$ mulige kanter
- $O(N^2)$
### 7.5 Topologisk sortering
- **Formål:** ordne noder i en rettet asyklisk graf slik at hver rettet kant fra node A $\to$ node B kommer A før B i sortering
- Noen må være ferdige før andre begynner.
- En rettet graf med en _node_ per aktivitet. Når en aktivitet $i$ må være ferdig før $j$ begynner legger vi inn en _kant_ fra $i$ til $j$.
- Ta en slik graf og sette nodene i en mulig _rekkefølge_.
- Kan være mange riktige rekkefølger
- Graf som sorteres topologisk må være _asyklisk_.
- Resultatet av topologisk sortering er ei lenket liste med alle nodene i en _logisk_ rekkefølge
- $\Theta(N+K)$
**Implementasjon (enten BFS eller DFS basert)**
- Start med DFS en tilfeldig node.
- Hver gang DFS er ferdig med å behandle en node, lenkes den inn først i resultatlista,
- Arbeid bakover og bruk DFS til å behandle en node, lenk den til resultatlista osv.
- Reverser deretter resultatlista
### 7.6 Sterkt sammenhengende komponenter
En *urettet graf* kan deles inn i _sammenhengende deler_; finnes en vei fra hver node til enhver annen, og hele grafen sies å henge sammen hvis det er bare en slik del ![[Screenshot 2025-11-12 at 21.01.39.png]]
- **Sterk sammenhengende deler:** En del av en _rettet graf_ som er slik at ALLE nodene har en _vei_ til alle andre noder. Gjelder begge veier
- For å finne _sterkt sammenhengende deler_ trenger vi den omvendte grafen. Den har akkurat de samme _sterkt sammenhengende delene_ som originalen
Algoritme for å finne sterkt sammenhengende deler
1. Kjør DFS på alle nodene i grafen, så får man ett eller flere dybde-først-trær. DFS beregner ferdig-tider for alle nodene
2. Sorter nodene i grafen å synkende ferdig-tid
3. Lag den omvendte grafen, $G^T$
4. Kjør DFS på alle nodene i den omvendte grafen. Start med den som fikk høyest ferdig-tid og fortsett nedover.
5. Hvert av dybde først-trærne etter punkt 4 vil være sterkt sammenhengende del av grafen.
- $\Theta(N+K)$, evt $\Theta(N^2)$
---
## 8. Vekta grafer

### 8.1 Vektede grafer
- I en vektet graf er det forskjell på kantene
- Hver kant har en _vekt_ $v$:
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
1. _Korteste vei fra én startnode til alle de andre._ Å finne kortest vei til bare én annen node har ikke lavere kompleksitet da veien i verste fall kan gå igjennom alle andre noder
2. _Korteste vei fra alle andre noder til én målnode._ Beregne omvendte grafen $G^T$, og bruke en vanlig kortest vei algoritme med målnoden som startnode.
3. _Kortest vei fra hver node til hver av de andre._ Kan bruke "en til alle" algoritme for hver node, men det finnes flere effektive måter.
**Rundtur:** begynner og slutter i samme node.
- Korteste vei fra en node til en annen kan ikke inneholde rundtur
**Alle til alle korteste vei (Floyd Warshall)**
- Kjøretid‚ $\Theta (N^3)$, og trenger‚ $\Theta(N^2)$plass for resultatet
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
- Bruker _prioritetskø_ for å holde orden på hvilken node som har lavest distanseestimat blant de som en ikke er ferdig med
- Samme «forgjengersystem» som i BFS
- Algoritmen garanterer alltid korteste vei fordi den behandler noder i rekkefølge etter avstand - når en node først er ferdigbehandlet, har vi funnet korteste vei dit.
**Implementasjon**
1. _Start:_ Startnoden har avstand 0 og alle andre til uendelig. Legg alle noder i en prioritetskø
2. _Velg node:_ Ta ut noden med lavest avstand fra køen (første gang: startnoden)
3. _Oppdater nabo:_ For hver nabo til valgt node:
    - Beregn ny avstand = nodens avstand + kantvekt
    - Hvis ny avstand er kortere enn naboens nåværende avstand, oppdater den
4. _Gjenta:_ Fortsett til alle noder er behandlet - til prioritetskøen er tom
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
    - Gå gjennom alle de $K$ kantene og _for hver kant_: Hvis `dist[u]+vekt<dist[v]`, oppdater `dist[v]`
- Nå er de korteste veiene funnet.
- Sjekk negative sykler/rundturer: hvis noen avstander fortsatt oppdateres, finnes en negativ sykel

| Egenskap        | Bellman-Ford                   | Dijkstra                          |
| --------------- | ------------------------------ | --------------------------------- |
| Kantvekter      | Håndterer negative kantvekter  | Kun positive vekter               |
| Kompleksitet    | O(V·E) - tregere               | O((V+E)log V) - raskere           |
| Fremgangsmåte   | Sjekker alle kanter V-1 ganger | Besøker hver node én gang         |
| Negative sykler | Kan detektere negative sykler  | Kan ikke håndtere negative vekter |
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
- **Kostnaden** for et spenntre er _summen_ av vektene på alle kantene som er med i spenntreet
- **Minimalt spenntre:** er et deltre av en graf som:
    1. Forbinder alle noder
    2. Har minimal konstnad
    3. Ingen sykler
    - To algoritmer for å finne minimale spenntrær i en graf: kruskals og prim

**Bruksområder**
- Planlegge graving, rør, strømledninger. Mange trasèer er mulige, kan ha ulik pris både pga. lengde og vanskeligheter i terrenget
- kretskort/chip
    - Mange punkter skal kobles til jord/strømforsyning.
    - Kortere forbindelser er billigere
- switching
    - switcher kobles sammen i nettverk. Kan ikke ha rundturer pga. broadcast-trafikk. Kobler ut redundante forbindelser til vi sitter igjen med et spenntre. (STP)
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

|Egenskap|Prim|Kruskal|
|---|---|---|
|Strategi|Vokser **ett tre** fra startnode|Slår sammen **flere trær**|
|Kantutvelgelse|Ser på kanter fra tre til utenfor|Ser på alle kanter globalt|
|Sortering|Trenger ikke sortere alle kanter|Må sortere alle kanter først|
|Best for|Bedre for tette grafer|Bedre for spredte grafer|
### 8.4 Maksimum flyt
#### Flytnettverk
- Kantene er transportkapasitet
- Vi ønsker å transportere mest mulig fra en Kilde (K) til et Sluk (S), to spesielle noder i grafen
- Andre noder kan bare sende videre, ikke lagre noe. Sum av flyt _inn_ er lik sum av flyt _ut_ for vanlige noder
- For hver kant vises flyt og kapasitet på formen $\text{flyt}/\text{kapasitet}$

**Maksimal flyt-problemet:** å finne den maksimale flyten vi kan ha gjennom et flytnettverk nettverk, fra kilde til sluk, uten å gå over kapasitetgrense noen steder.

**Flere kilder og sluk**
- Bare én kilde og ett sluk ser ut som en begrensning
    - Mange fabrikker, mange kjøpesenter…
- Vi kan ha flere, men da kan vi transformere til det enklere problemet:
    - Kildene og slukene gjøres om til vanlige noder, og innfører en superkilde og supersluk, med uendelig kapasitet på tilkoblingene. ![[Screenshot 2025-10-08 at 01.19.25.png]]

**Flytnettverk:** rettet graf hvor hver kant$(n,m)$ har en kapasitet $k(n,m)$. Flyten gjennom kanten kalles $f(n,m)$. Antar at alle noder ligger mellom kilden og sluket
- All transport må gå hele veien fra K til S, ingen mellomlagring
- Flyt i en kant kan maksimalt være lik kapasiteten, men kan være mindre
- Positiv flyt i en retning, kan sees på som negativ flyt i motsatt retning
- _Regler:_
    - **Kapasitetregelen:** $f(n,m) \leq k(n,m)$. Flyten må være mindre eller lik kapasiteten
    - **Symmetriregelen:** $f(n,m) =-f(m,n)$. Flyt i én retning sees på som negativ flyt i motsatt retning
    - **Flytkonversering:** Summen av flyt inn og ut fra noder utenom kilden og sluk må være null. Nodene har ingen lagringskapasitet.
- **Total flyt i grafen kalles $|f|$ og er lik:**
    - Summen av alt som flyter ut av kilden
    - Summen av alt som flyter inn i sluket
    - Summen av alt som flyter på tvers av et hvilket som helst snitt gjennom grafen, der kilde og sluk havner på hver sin side
**Definisjoner**
- _Restkapasitet:_ Angir hvor mye mer flyt som kan sendes gjennom en kant: $k_r(n,m)=k(n,m)-f(n,m)$
- _Flytøkende vei_: en vei fra kilde til sluk der kapasiteten ikke er fullt utnyttet. Vi kan øke flyt langs veien tilsvarende den kanten som har minst _restkapasitet_.
- _Restnett:_ En graf med oversikt over _restkapasitet_ og muligheter for forandring i flyten i hele grafen
- _Flytkansellering:_ I blant kan vi øke flyten ved å «snu» flyten gjennom en kant som har uhensiktsmessig flyt.
- _Flaskehals:_ kanten med minst restkapasitet langs en _flytøkende vei_
- Når det går en flyt $f$ fra $a$ til $b$, minker _restkapasiteten_ fra $a$ til $b$ med $f$. Men _restkapasiteten_ fra $b$ til $a$ øker med $f$.
    - Når vi har flyt i motsatt retning, kan restkapasiteten være større enn virkelig avstand.
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
4. Gjenta til ingen veier fins
Flyten økes så mye som mulig, altså like mye som kapasiteten i den kanten langs veien som har lavest kapasitet.
#### Edmonds Karp metoden
- Som Ford Fulkerson, men:
- Bruk BFS på _restnettet_ for å finne en flytøkende vei.
    - Start BFS i kildenoden og avsutter når man finner sluket
    - Finner (og bruker opp) de flytøkende veiene som har færrest kanter først.
    - Unngår dermed «fella» som Ford Fulkerson kan gå i
- I praksis på ark
    1. Bruk BFS for å finne kortest vei fra K til S i restnettet
    2. Send flyt langs veien
    3. Oppdater restnettet
    4. Gjenta
- Kompleksitet $O(NK^2)$

| Egenskap       | Ford-Fulkerson                          | Edmonds-Karp                           |
| -------------- | --------------------------------------- | -------------------------------------- |
| Søkemetode     | Bruker DFS eller annen søkemetode       | Alltid BFS                             |
| Kompleksitet   | O(K·max_flow) - avhenger av tallverdier | O(NK²) - avhenger kun av grafstørrelse |
| Ytelse         | Kan være treg på spesielle grafer       | Garantert polynomial tid               |
| Implementering | Enklere konseptuelt                     | Mer forutsigbar ytelse                 |

---
## 9. Dyn. programmering, grådige alg.
### 9.1 Ulike optimaliseringsmetoder
**Optimaliseringsproblem**: problem der vi ønsker å finne den beste løsningen blant mange mulige.
- Ex: minimal spenntre og kortest-vei
**Brute force-algoritmer**
- Beregne _alle_ muligheter og velge den beste.
- Bruker mye datamaskinkraft
- Bør unngås hvis de resulterer i eksponentiell kompleksitet, f.eks. $O(2^n)$ eller $O(n!)$
- Kan brukes hvis de resulterer i en polynomisk kompleksitet, f.eks. $O(n^2)$ eller $O(n^5)$
**Splitt og hersk-algoritmer**
- Deler problemet i mindre subproblemer, løser disse og setter samme løsningene til en løsning på det opprinnelige problemet.
- Tre ulike teknikker: Vanlig splitt og hersk med _rekursjon_, og to andre teknikker _uten rekursjon_
- Dynamisk programmering og grådige algoritmer
## 9.2 Dynamisk programmering
- Lik splitt og hersk mtp dele problemer i mindre løsninger og sette dem sammen.
- Egner seg når de samme delproblemene dukker opp mange ganger. Når løsningene på disse delproblemene er beregnet og lagret, kan disse løsningene brukes på nytt neste gang samme delproblem dukker opp.
- Løsningene er lagret i en tabell
- Dynamisk programmering starter nederst og _lagrer_ løsninger som settes sammen til stadige nye løsninger på veien til toppen.
- Bottom-up: bygg fra minste til største. 
- Top-down: rekursjon med memoisering.
- En del _rå kraft_ algoritmer får lavere kompleksitet når vi bruker dynamisk programmering.
### 9.2.1 To eksempler på dynamisk programmering
#### Ryggsekkproblemet
- **Diskrete ryggsekkproblemet:** Vi har $n$ varer, der hver vare har pris $p_{i}$ og vekt $v_{i}$, $i=1...n$. Maksimal vekt er $V$ og vi skal få med oss størst verdi i én omgang. I praksis må man prøve alle kombinasjoner som ikke veier for mye
- $p_i$ = prisen av vare nummer $i$
- $v_i$ = vekten av vare nummer $i$
- Løses:
    - Start med å lage en todimensjonal tabell der radene er **varene** og kolonnene representerer **vektkapasitet**: $T[j][i]$ er **maksimal verdi man kan bære** der **j er vekt til rådighet** og kan velge mellom **i varer**.
    - **Initialisering**
        - $T[j][0]=0 \text{ for alle } j$ (ingen varer $= 0$ verdi)
        - $T[0][i]=0 \text{ for alle } i$ (ingen kapasitet $= 0$ verdi)
    - Start med minste vektene og beregner nye resultater basert på tidligere.
    - Rekursjonsformel: for hver celle $T[j][i]$ har vi to valg
        1. Ta _ikke_ med vare _i_: Da er verdien $T[j][i-1]$ (samme vekt, uten vare _i_)
        2. Ta _med_ vare _i_ (hvis $v_i \leq j$): Da er verdien $p_i + T[j-v_1][i-1]$
        - Vi velger det beste: $T[j][i]=\text{max}(T[j][i-1],p_i+T[j-v_i][i-1])$
- **Dynamisk programmering perspektiv**
    - Verdiene lagres i tabellen $T[i][j]$. Når vi beregner en ny celle bruker vi tidligere lagrede verdier fra tabellen
    - For hver celle $T[i][j]$ sammenligner vi
        1. Ta _ikke_ med vare _i_: Da er verdien $T[j][i-1]$ (cellen rett over)
        2. Ta _med_ vare _i_: Da er verdien $p_i + T[j-v_i][i-1]$ (pris av vare $i$ + en celle lengre tilbake)
    - $p_i + T[j-v_1][i-1]$ er ikke nødvendigvis forrige celle, men cellen som representerer _hva er maks verdi med den vekten vi har igjen etter å ta vare $i$_
- $\Theta(V \cdot n)$
**Redigeringsavstand (tror ikke det er pensum)**
- Avstanden mellom to strenger: minste antall lovlige endringer vi kan gjøre med det ene ordet for å omforme det til det andre
- Lovlige endringer:
    1. Ta bort et char
    2. Sette inn et char
    3. Endre et char
## 9.3 Grådige algoritmer
- **Definisjon:** Algoritme som alltid gjør det som synes best i øyeblikket uten tanke på helheten.
    - Går aldri _bakover_ på valg og tenker ikke _fremover_
- Raske og effektive, _hvis_ problemet lar seg løse slikt
    - Må bevise at grådige algoritmer er korrekte
- Eksempler
    - Prims og Kruskals
    - Dijkstras
    - Huffmann
#### Huffman-koding
- Når tekst lagre på fil, brukes standard metoder å representere de ulike tegnene på (ascii eller unicode)
- **Ide:** La vanlige tegn ha korte bitsekvenser mens uvanlig tegn få lengre bitsekvenser.
    - Bruker 8bit (1 byte) per tegn
- For å finne **Huffmann-kodene** til tegnene teller vi hvor mange ganger hvert tegn forekommer.
- For hvert tegn lager vi et _binærtre_ som består av ei rot og lagrer både tegnet og frekvensen i rota. Disse trærne lagrer vi i en _prioritetskø_ som er ordnet slikt at tegn med lavest frekvens kommer først ut av køen.
- Henter to og to trær ut fra prioritetskøen, og lar de to trærne være venstre og høyre subtre i et nytt tre og lar rota få en frekvens som er summen av frekvensene i de to subtrærne. Dette treet setter vi inn i prioritetskøen igjen. Deretter kombinerer vi de to neste trærne og forsetter helt til treet er laget. (vi tar da ut først trærne med lavest frekvens - disse havner dermed nederst)
- Deretter generer man koder: setter 0 på venstre kant og 1 på høyre kant.. Da kan vi lese kodene for de ulike tegnene.
- For å **finne skillet mellom tegnene** følger vi treet nedover til vi kommer til en løvnode og slikt har vi fått ett tegn. Deretter starter vi på toppen og finner ett nytt tegn
- Prefixfrie koder: Ingen av huffmann-kodene er prefix
**Hvordan er huffmann en grådig algoritme?**
- Slår sammen de trærne med lavest frekvens fordi det gir minst økning i vektet veilengde - vi gjør det som synes best i øyeblikket.
**Dekoding**
- Huffmankode er prefiksfri. (En kort tegnkode kan aldri være starten på en annen lengre kode
- Dekoding av bitstreng: start i rota, bølg bitmønsteret til vi finner et tegn. Start igjen i rota og fortsett helt til vi har dekodet meldingen

---
## 10. Tekstsøk, datakompresjon
### Tekstsøk
Enkel naiv algoritme for søk
- Dobbeltløkke for $n-m$ posisjoner, og $m$ tegn i søkeordet
    - Lengde $n$ på tekst man søker i
    - Lengde $m$ på søkeord
- $O(n \cdot m)$, $\Theta(n)$
#### Boyer-Moore søk
- Se på siste tegn i søkeord først
- Hvis det ikke passer, flytt søkeordet så langt vi kan (til neste match)
**Regelen om upassende tegn**
- Hvis tegnet ikke fins i søketeksten, kan vi flytte $m$ steg fram, hvis mismatch var på _siste_ i søketeksten
    - Med mismatch på _nestsiste_ tegn kan vi flytte $m-1$ steg
    - Mismtach på _nestnestsiste_ tegn, flytter vi $m-2$ steg osv.
- Har en todimensjonell tabell for hvor mye vi kan flytte
    - En indeks er det upassende tegne
    - Den andre indeksen er posisjonen i søketeksten
    - Verdien i cellen er hvor langt vi kan flytte fremover
- Tabellen lager vi med å _preprosessere_ søketeksten
- $\Omega(n/m)$

**Regelen om passende endelse**
![[Screenshot 2025-11-15 at 12.42.41.png | 400]]
- 0,1: når siste posisjon intreffer _n_, kan vi bare flytte ett steg
- 2: Feil i første posisjon
    - _Regel om upassende tegn_ lar oss bare flytte ett steg
- _Regel om passende endelse_ lar oss flytte to steg
- Vi slår opp både _upassende tegn_ og _passende endelse_ og bruker regelen som gir det lengste hoppet.
- Tabellen for _passende endelse_
    - Index er hvor mange tegn som passet
    - Verdien i cellen er hvor langt vi kan flytte
    - Lages ved å prøve ut om søketeksten overlapper med seg selv
**Galil sin regel**
- Hvis vi søker etter "aaa" i "aaaaaa..." har vi $O(n \cdot m)$ da søkeordet passer overalt, og de samme a-ene sjekkes flere ganger
- Unngå unødvendige sammenligninger:
    - Når vi flytter søkeordet kortere enn den delen av søkeordet vi allerede har sjekket, trenger vi ikke sjekke det overlappende området om igjen
    - Korte flytt skjer fordi søkeordet delvis matcher seg selv. Hvis det ikke hadde matchet, hadde vi flyttet lengre ![[Screenshot 2025-11-15 at 13.18.19.png | 500]]
- Programmet trenger ikke å sjekke den oransje regionen om igjen
- $O(n)$ og $\Omega(n/m)$ for tekstsøk
### Datakompresjon
#### Run-length encoding (RLE)
- En serie repetisjoner erstattes med et antall:
    - $ABIIIIIIIIIIIIBBBCDEFFFGH \to AB12I3BCDE3FGH$
- Ulemper
    - Kan være sifre i det vi komprimerer
    - Ser vanligvis på bytes og ikke tekst
    - Kunne skille mellom data og metadata (header)
- Bruker negative byte for ukomprimerte sekvenser:
    - $ABIIIIIIIIIIIIBBBCDEFFFGH$ $\to [-2]AB[12]I[3]B[-3]CDE[3]F[-2]GH$
- Hva med $ABABABABAB$
#### LZ (Lempel-Ziv)
- Leser gjennom fila
- Input kopieres til output
- Hvis en lang nok sekvens kommer til igjen:
    - dropp den og skriv eller en referanse til output
    - format: repeter $X$ tegn, som vi har sett $Y$ tegn tidligere
- Hjelper hvis sekvensen er lenger enn en slik referanse
- Søker bakover i et sirkulært _buffer_
**Bakoverreferanser**
- Å se langt bakover i datastrømmen, gir større sjanser for å finne repetisjoner, men påvirker kjøretid:
    - 1 byte kan peke 255 tegn bakover
    - 2 byte kan peke 65 536 tegn bakover
    - 3 byte kan peke 16 777 215 tegn bakover
- I blant kan vi ikke komprimere og må derfor ha en måte å si "her kommer X bytes ukomprimerte data"
**Hva kan komprimeres?**
- Det vi komprimerer må være lengre enn samlet lengde for: en bakover-referanse og header for en ukomprimert blokk
- Komprimerer ikke korte strenger (dette hjelper ikke)
**Kjøretid**
- For hver tegnposisjon i input må vi søke etter lengste match i bufferet
- Fil med $n$ tegn og buffer med størrelse $m$
- Hvis man tester alle posisjoner er $O(nm^2)$, men bedre i praksis
- Kan bruke Boyer-Moore tekstsøk for bedre kjøretid
#### Deflate-algoritmen (zip), LZ+Huffman
- LZ77 produserer literals (ukomprimerte tegn "?!" ovenfor) og length/distance koder. Disse komprimeres med Huffmann (to separate huffmann-trær). 
#### Lempel-Ziv-Welsh (LZW)
- Ligner LZ
**Komprimering**
1. Start med en ordbok (dictionary) som inneholder alle enkeltbyte-verdier 0-255 (de 256 mulige verdiene en enkelt byte kan ha $\to 2^8=256$)
2. Les inputdata sekvensielt
3. Finn lengste sekvensen som allerede finnes i ordboken
4. Send ut koden for denne sekvensielt
5. Legg til sekvensen + neste tegn som ny oppføring i ordboken
6. Gjenta til all data er prosessert
- Kompresjon hvis ordene blir lengre enn numrene
**Eksempel:** Komprimere "ABABABA"
- Start: ordbok har A = 65, B = 66
- Les "A" $\to$ send 65, legg til "AB" = 256
- Les "B" $\to$ send 66, legg til "BA" = 257
- Les "AB" (finnes nå) $\to$ send 256 legg til "ABA" = 258
- Les "ABA" $\to$ send 258
- Komprimert: $[65,66,256,258$], mye kortere enn originalen for repeterende data
**Fordeler**
- Effektiv for data med repeterende mønstre
- Trenger ikke å sende ordboken med da den bygges dynamisk underveis i komprimeringen
- Raskt og enkelt å implementere
- Godt eksempel på _adaptivt komprimering_ der algoritmen lærer dataens struktur underveis
**Ulemper**
- Ikke optimalt for allerede komprimerte data eller tilfeldige data
- Ordbok kan bli stor
#### LZW + huffmann
- LZW: leser input, bygger en dictionary, skriver ordnumre
- Noen ord forekommer oftere enn andre. Programmet finner antall frekvenser for ulike ordnumre
    - Skriver huffmannkoder i stedet for ordnumre
    - Ord som forekommer ofte, får kortere koder
#### Burrows-Wheeler transformasjonen (BWT)
- _Hva:_ Smart måte å organisere tekst på slik at den blir lettere å komprimere. Reorganiserer rekkefølgen slik at like tegn havner ved siden av hverandre. Tar tekst med repeterende mønstre og gjør dem om til repeterende tegn
- _Eksempel:_ "BANANA"
- Steg:
    1. Legg til spesielt tegn på slutten "BANANA$"
    2. Lag alle mulige rotasjoner av teksten (flytter tegn fra venstre til høyre):
    ```plaintext
    BANANA$
    $BANANA
    A$BANAN
    NA$BANA
    ANA$BAN
    NANA$BA
    ANANA$B
    ```
    3. Sorter radene alfabetisk:
    ```Plaintext
    $BANANA
    A$BANAN
    ANA$BAN
    ANANA$B
    BANANA$
    NA$BANA
    NANA$BA
    ```
    4. Resultatet av BWT er siste kolonnen
#### Move-to-front transformasjon (MTF)
- Kode om data slik at repeterte tegn blir til nuller, og nesten repeterte mønstre blir til små tall. Små tall og mange nuller er lettere å komprimere videre.
##### Virkemåte
**Steg 1: Initialiser tabellen**
- Vi starter med en tabell som inneholder alle mulige tegn i alfabetisk rekkefølge:
```
Tabell: [A, B, C, D]
Index:   0  1  2  3
```
**Steg 2: Les tegn for tegn og transformer**
- La oss si vi har inputstrengen: **"BANANA"**
_Tegn 1: B_
- Finn B i tabellen → B er på index 1
- Skriv 1 (index) til output
- Flytt B til front (move to front): `[B, A, C, D]`
```
Input:  B
Output: 1
Tabell: [B, A, C, D]
```
_Tegn 2: A_
- Finn A i tabellen → A er på index 1
- Skriv 1 til output
- Flytt A til front: `[A, B, C, D]`
```
Input:  B A
Output: 1 1
Tabell: [A, B, C, D]
```
_Tegn 3: N_
- Finn N i tabellen → N er på index 13 (i et fullt alfabet)
- Skriv 13 til output
- Flytt N til front: `[N, A, B, C, D, ...]`
```
Input:  B A N
Output: 1 1 13
Tabell: [N, A, B, C, D, ...]
```
_Tegn 4: A_
- Finn A i tabellen → A er på index 1 (fordi N nå er først)
- Skriv 1 til output
- Flytt A til front: `[A, N, B, C, D, ...]`
```
Input:  B A N A
Output: 1 1 13 1
Tabell: [A, N, B, C, D, ...]
```
_Tegn 5: N_
- Finn N i tabellen → N er på index 1 (fordi A nå er først)
- Skriv 1 til output
- Flytt N til front: `[N, A, B, C, D, ...]`
```
Input:  B A N A N
Output: 1 1 13 1 1
Tabell: [N, A, B, C, D, ...]
```
_Tegn 6: A_
- Finn A i tabellen → A er på index 1
- Skriv 1 til output
- Flytt A til front: `[A, N, B, C, D, ...]`
```
Input:  B A N A N A
Output: 1 1 13 1 1 1
Tabell: [A, N, B, C, D, ...]
```
**Enda bedre eksempel med repetisjon: "AAABBA"**
- Startabell: `[A, B, C, D, ...]`

| Input tegn | Index | Output | Ny tabell         |
| ---------- | ----- | ------ | ----------------- |
| A          | 0     | 0      | [A, B, C, D, ...] |
| A          | 0     | 0      | [A, B, C, D, ...] |
| A          | 0     | 0      | [A, B, C, D, ...] |
| B          | 1     | 1      | [B, A, C, D, ...] |
| B          | 0     | 0      | [B, A, C, D, ...] |
| A          | 1     | 1      | [A, B, C, D, ...] |
- _Resultat:_ `0 0 0 1 0 1`
**Hvorfor**
- Repeterte tegn `AAA` blir til `000`
- Mange nuller og små tall gir effektiv huffmannkoding
**Enda et eksempel**
- inn:`caaaaacbbbbbbbaaaabb`
- ut:`21000012000000200010`

| Tegn | Frekvens før | Frekvens etter |
| ---- | ------------ | -------------- |
| a    | 9            | (som 0): 14    |
| b    | 9            | (som 1): 3     |
| c    | 2            | (som 2): 3     |
- Før: like mange «a» som «b»
- Etter: overvekt av «0», som kan få kortere kode.
#### BZIP2-algoritmen:
1. Run-length coding
2. Burrows-Wheeler transformasjon (hoveddel som sorterer så vi får mange repetisjoner: "AAABBBCCC"
3. Move-To-Front transformasjon (MFT) som gjør ulike repetisjoner om til nuller: "000100200"
4. Run-length coding igjen
5. Huffmannkoding
#### Aritmetisk komprimering
**Aritmetisk komprimering** er spesifikk komprimeringsmetode som
- Representerer hele meldingen som ett enkelt tall i intervallet $[0,1)$
- Deler intervallet opp basert på sannsynligheten til hvert symbol
- Jo mer sannsynlig et symbol er, desto større del av intervallet får det
- Lik, men bedre enn Huffmann-koding fordi den kan bruke brøkdeler av bits per symbol: Hvis "A" har sannsynlighet 0.6 får den 60% av intervallet
- Huffmann bruker hele bits per symbol
#### Adaptiv komprimering
**Adaptiv komprimering** strategi for komprimering:
- Der kompresjonsalgoritmen lærer og tilpasser seg underveis basert på dataene
- Oppdaterer sannsynlighetsmodellen dynamisk underveis i komprimeringen
- Kan brukes med mange algoritmer
#### Drøfte AI og kompresjon:
- Noen mener at datakompresjon og AI er samme problem: AI er korteste programmet som oppfører seg intelligent
- Å oppdage repeterte mønstre kan nyttes for kompresjon og krever intelligens
- Mer intelligens $\to$ bedre kompresjon
---
## 11. Kompleksitetsklasser, haltingproblemet
### Kompleksitetsklasser
**P:** Mengden av problemer som kan _løses_ i polynomisk tid
- $O(1),O(n), O(n^2), O(n^3),...,O(n^{100})$
- Ex: sortering, korteste vei, max flyt
- _Praktisk:_ løs dem effektivt
**NP:** Mengden av problemer hvis _løsningen_ kan _sjekkes_ i polynomisk tid.
- **P** $\in$ **NP**, alle **P**-problemer er også **NP**-problemer
- Eksempel: kan noen tall av $n$ heltall summeres til $0$?
**Det største spørsmålet innen datavitenskap**
- Hvis **P** = **NP**? $\to$ Alle problemer hvis løsningen kan sjekkes raskt, kan også løses raskt
- Hvis er **P** ≠ **NP** $\to$ Noen problemer er vanskeligere å løse enn å sjekke
**NP-komplette problemer (NPC):** en gruppe _vanskeligste_ problemene i **NP**
- Kjennetegn
    - NPC $\in$ NP
    - _Kompletthet_: hvert problem i **NP** kan reduseres til et hvilket som helst problem i **NPC** i polynomisk tid
    - Beste kjente løsninger $O(2^n)$
- Betydning av kompletthet
    - Finner vi én polynomisk løsning på _ett_ NPC-problem, virker den for alle og vi har løst **P** = **NP**
- **Praktisk betydning**
    - Hvis programmet ikke kjører på rimelig tid:
        - sjekk om problemet er **NP**-komplett
        - isåfall kan du gi opp å finne en eksakt løsning som er kjappere.
    - Se evt etter en tilnærmet løsning (_heuristikker_)
        - Mange av de kjente **NP**-komplette problemene har gode tilnærmede løsninger, som lar seg beregne fort nok
### Eksempler på NPC
**Traveling Salesman**
- _Problem:_
    - Har $n$ byer, og kjente kostnader for å reise mellom dem
    - Fins det en reiserute som besøker alle byene nøyaktig én gang med totalkostnad under $x$
- Kompleksitet
    - Kan prøve alle muligheter ($n!$, eventuelt $\frac{n!}{2n}$)
    - Løsningen (en rekkefølge) kan sjekkes på $O(n)$ tid
**Isomorfi**
- _Problem:_ Er $G_1$ isomorf med en subgraf av $G_2$?
- NP men ikke NPC
**Ryggsekkproblemet (gått igjennom i dynamisk programmering)**
- _Problem:_
    - Gitt $n$ varer, hver med pris og vekt
    - Gitt en bærekapasitet
    - Kan vi få med varer for verdien $V$?
- Hvis vekten er desimaltall, går ikke dynamisk programmering
- Vanskelig å finne optimale kombinasjoner, men lett å sjekke et forslag
**Hamiltonsyklus i graf**
- _Problem:_
    - Gitt en graf med $n$ noder
    - Fins en vei som er innom alle n noder én gang og returnerer til startpunktet?
- Alle mulige rekkefølger, $O(n!)$
- Løsningsforslag kan sjekkes i $O(n)$ tid
**Delsum**
- _Problem:_ Gitt $n$ heltall, fins det en delmengde av disse som summerer til $0$
- Kan sjekke alle $2^n$ kombinasjoner
- Forslag kan sjekkes i $O(n)$
**Komplett subgraf**
- _Problem:_
    - Gitt en graf
    - Fins en _komplett_ subgraf med $x$ noder?
- Løsning sjekkes ved å se etter om subgrafen er komplett
**3SAT, 3-way satisfiability**
- _Problem:_
    - Gitt et _boolsk uttrykk_ på formen: $(x_1|x_2|x_3)$ & $(!x_1|!x_4|x_3)$ & $(!x_2|x_4|!x_3)$ & $...$
    - $3$ variabler i hver del, $n$ variabler totalt
    - Fins det en tilordning $(x_1=1,x_2=0,...)$ som gjør hele uttrykket sant?
- $2^n$ mulige kombinasjoner
- Forslag kan sjekkes i $O(n)$ tid
- _Alle_ NP-komplette problemer (NPC) kan reduseres til 3SAT
### NP-harde problemer
- **Definisjon**: Et problem er **NP-hardt** hvis og bare hvis:
    - ethvert problem i _NP_ kan reduseres til dette problemet i polynomisk tid
    - Men: svar trenger ikke kunne _sjekkes_ i polynomisk tid
- Sammenheng
    - Alle _NPC_ er også _NP-harde_
    - Men _NP-hard_ problemer trenger ikke å være i _NP_
    - Delvis uløselige problemer som _halting-problemet_
- Optimaliseringsvarianter:
    1. **Traveling Salesman - Optimalisering:**
        - Finn den _billigste_ av alle mulige reiseruter (ikke bare "under kostnad x")
    2. **Ryggsekkproblemet - Optimalisering:**
        - Få med _maksimal_ verdi (ikke bare "verdi minst V")
    3. **Komplett subgraf - Optimalisering:**
        - Finn den _største_ komplette subgrafen
- _Praktisk:_ finn "gode nok" løsninger
### Halting-problemet
**Problemformulering**
- Gitt et program $P$ og inndata $D$ _Spørsmål:_ Vil $P$ noen gang avslutte når det kjører med $D$, eller vil det gå i uendelig løkke?
- NP-hardt fordi ja/nei svaret ikke kan sjekkes i polynomisk tid.
- **Hvorfor er haltingsproblemet så viktig**
	- Hvis vi kunne løse haltingproblemet kunne vi løst _alle_ problemer med ja/nei svar
**Eksempel - Goldbachs formodning:** Stemmer det at alle partall $> 2$ er summen av to primtall? Fremgangsmåte:
1. Lag et program som tester alle tall fra $4$ til uendelig
2. Programmet avbryter hvis det finner et moteksempel
3. Programmet stopper aldri hvis moteksempelet ikke fins
4. **Hvis vi hadde løsning på haltingproblemet:**
    - Sjekk om programmet stopper $\to$ det fins et moteksempel $\to$ formodningen er usann
    - Sjekk om programmet ikke stopper $\to$ ingen moteksempel $\to$ formodningen er sann
5. Vi trenger aldri å kjøre programmet!

**Bevis for at haltingsproblemet er uløsbart**
1. Anta at vi har et program $H$ som løser halting-problemet
    - $H$(program,input) returnerer "stopper" eller "løper evig"
2. Lag et program $P$ som tar vilkårlig program $Q$ som input
    - $P$ bruker $H$ for å sjekke om $Q$ vil stoppe, når $Q$ får sin egen kildekode som input
    - Hvis _ja_ kjører $P$ en uendelig løkke
    - Hvis _nei_ stopper $P$
```psuedocode
P(Q): // Q er et vilkårlig program
	hvis H(Q,Q) sier "stopper":
		løp i en uendelig løkke
	hvis H(Q,Q) sier "løper evig":
		stopp umiddelbart
```
3. Kjør deretter $P$ med $P$ som input: $P(P)$
4. Analyse:
    - Hvis $H(P,P)$ sier _ja, $P$ stopper_ $\to$ vil $P$ gå i uendelig løkke $\to$ $P$ stopper ikke
    - Hvis $H(P,P)$ sier _nei, $P$ looper_, vil $P$ stoppe $\to$ $P$ løper ikke evig
5. Konklusjon: $H$ gir feil svar uansett $\to$ $H$ kan ikke eksistere
    - Altså programmet $H$ er logisk umulig å løse generelt
**Det store spørsmålet igjen**
- Er **P** = **NP**
- Hvis ja, kan alle problemer løses i polynomisk tid
    - Aldri nødvendig med brute force (rå kraft) i problemløsning, fordi _antall muligheter_ er større enn polynomisk, ofte $2^n$, $n!$ eller $n^n$
    - Kjente **NP**-komplette problemene får enkle og kjappe løsninger
- Hvis nei, er det greit å vite
**Hierarki av vanskelighet**
```
P ⊆ NP
    ↓
   NPC (vanskeligste i NP)
    ↓
 NP-hard (kan være enda vanskeligere)
    ↓
Uløselige problemer (som haltingproblemet)
```
**Praktisk betydning:**
- **P-problemer:** Løs dem effektivt
- **NP-komplette:** Bruk tilnærmingsalgoritmer eller heuristikker
- **NP-harde optimaliseringsproblemer:** Finn "gode nok" løsninger
- **Uløselige problemer:** Kan bare løses for spesialtilfeller
---
## 12. A*- og ALT-algoritmene

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
- Ikke _asymptotisk_ bedre
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
- Ikke _asymptotisk_ bedre, men en halvering
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
- Forskjell mellom A* og Dijkstra er _prioriteringen_
    - Dijkstras prioriterer noder utfra deres avstand til startnoden. Dette fører til at søket sprer seg som en ring fra startstedet. Noden nær startnoden undersøkes først, i alle retninger
    - A* bruker i tillegg et _estimat_ for avstanden mellom noden og målet. Nodens prioritet blir avstand fra startnoden og, pluss estimatet for veien videre til målet. Når A* plukker neste node, vil det være noden hvor: avstand til startnoden + estimert avstand til målet blir til sammen _minst mulig_. Områder nærmere målnoden blir mer interessante. Der øker avstand fra startnoden, men estimert avstand minker til målet. Summen av startavstand og målavstand endrer seg lite langs vei mot målet, og øker i alle andre retninger.
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
**Alt:** er A* + _landemerke-heuristikk_ Metoden går ut på å gjøre noen få noder til _landemerker_. Fra disse landemerkene beregner vi avstander _til og fra alle andre noder_ på kartet ved å kjøre _Dijkstra_ på hvert av landemerkene (_preprosessering_). Bygger opp tabeller med avstand mellom hver node og landemerkenodene.
- Landemerker: noder hvor vi beregner avstand til alle andre noder (Dijkstra) - avstand fra alle andre norder (Dijkstra på omvendt graf)
    - beregningene gjøres som en _preprossessering_ av kart
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
    - Hvis $\delta(n,m)$ er _mindre_ enn differansen, må det finnes en kortere vei fra $L$ til $m$ via $n$
    - Men $\delta(L,m)$ og $\delta(L,n)$ er korteste distanser, funnet med Dijkstras algoritme
    - Så kortere vei fins ikke og ulikheten er korrekt
- _A* kan bruke $\delta(L, m)- \delta(L,n)$ som estimat for $\delta(n,m)$_
    - Det blir aldri for mye
    - Lett å beregne, når vi har avstandene _fra_ landemerket
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
- **A* kan bruke $\delta(n, L)- \delta(m,L)$ som estimat for $\delta(n,m)$_***
    - lett å beregne, når vi har avstand _til_ landemerket
    - ligger tett opptil virkelig avstand
        - hvis $m$ ligger omtrent på veien fra $n$ til $L$
- **Styrke**
    - A* prioriterer kortere avstand til landemerker
    - Ingen blindveier, for da øker avstand til landemerket
- **Svakhet**
    - Kan ta avstikkere som _har_ vei til landemerket.
**ALT, kombinert algoritme**
- Vi bruker både landemerker _bak_ startspunktet, og _etter_ målet
- For alle landemerker $L_{x}$ beregner vi
    - $\delta(L_{x},m)-\delta(L_{x},n)$ og
    - $\delta(n,L_{x})-\delta(m,L_{x})$
- A* bruker den _største av alle differansene som estimat_ for $\delta(n,m)$
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