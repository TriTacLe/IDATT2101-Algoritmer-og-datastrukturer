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
- Når man når *kapasitetgrensa* får vi et problem. For å få plass til flere elementer må lage en ny og større tabell, og kopiere over alle elementene i forrige tabell til den nye tabellen, før vi setter inn det nye elementet. Hvis vi har $n$ elementer er dette en $O(n)$ operasjon. Kan unngå dette ved å sørge for at man starter med stor nok kapasitet, men det kan hende vi bruker unødvendig mye plass.
- Hvis vi vil sette inn et nytt element på en annen plass enn bakerst må vi flytte på elementer. Hvis vi ønsker at det nye elementet skal komme først må alle elementer flyttes ett hakk, noe som gir $O(n)$. 
	- Hvis vi antar at vi skal sette inn element $n+1$ på posisjon $i$, vil antall elementer vi må flytte på være $(n-i)$. Hvis alle posisjoner er like sannsynlig, vil vi i gjennomsnittet flytte på $\frac{1}{2}(n+1)$ elementer
	- Viser at innsetting er $O(n)$
- Hvis vi skal fjerne et element fra en usortert liste, kan vi flytte siste element inn på plassen til det elementet som skal fjernes, og minske lengden med $1$.  $O(1)$
- Hvis rekkefølgen til alle elementer skal være uforandret, må vi i stedet flytte alle de etterfølgende elementene ett hakk fremover, $O(n)$ 
**Sekvensiell gjennomløping av lista**
- **Traverse datastrukturen:** gå igjennom en datastruktur slik at vi er innom alle elementene.
- Trenger da bare å lage en løkke der man starter i den ene eller andre enden der elementnumret øker/minker for hver runde. Siden vi skal behandle alle $n$ elementene, må tidskompleksiteten være $n$ ganger tidskompleksiteten for det vi skal gjøre med hvert element.
### 4.1.2 Linked list
**Node:** datastruktur som består av et element samt én eller flere lenker til én eller flere andre noder.
**Lenket liste:** samling av noder der hver node leder videre til neste node i lista.
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
- Denne implementasjonen av en *node* består av felt som inneholder dataene vi ønsker å lagre (double-tall) og ei lenke til neste node. 
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
**Kø:** lineær datastruktur der innsetting skjer bakerst og uttak skjer fremst. FIFO
To måter å implementere en kø på: tabell eller linked list
- Operasjoner som trengs: sett inn, ta ut, finn verdi av første, finne ut om køen er tom. I tabell trenger vi å finne ut om det er plass i køen
- For tabell
	- Å fjerne element er ikke så enkelt. Bruker en sirkel
## 4.3 Stakk
**Stakk:** lineær datastruktur der innsetting og uttak skjer fremst. LIFO
To måter å implementere en kø på: tabell eller linked list
- Linked list: trenger bare å bruke hodelenka
## 4.4 Trær
### 4.4.1 Grafer og trær
- Ulinært datastruktur
- Har ei rot
- Har noder, hver node kan lenke til mer enn en annen barnenode
- Alle utenom rota har en foreldrenode
- Noder uten barn kalles ytre noder/løvnoder
- Noder med barn kalles indre noder
### 4.4.2 Binærtre
**Binærtrær:** Posisjonstre der hver node har maksimalt to barn
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
- **Dybde:** Antall lenker mellom *noden* og *rota*. Rota har dybde 0
- **Høyde:** Antall lenker mellom *noden* og *den noden som er lengs unna i et av nodens subtrær*. Treet høyde er definert som rotas høyde
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
- Start med rotnoden og sjekke om den er null. Hvis ikke, sjekk om nodens nøkkelverdi er mindre enn nøkkelverdien til den vi sammenligner med, skal vi gå til venstre, hvis den er stører, går vi til høyre. Fortsetter helt til vi finner en node som mangler det barnet vi skulle ha gått videre til og setter noden inn her
**Søk**
- Lik innsetting, men vi må også teste på lik nøkkelverdier. Hvis nøkkelverdien er lik, returnerer vi noden. Hvis vi kommer til node uten barnet vi skulle gått videre til returnerer vi NULL (nøkkelverdien finnes ikke).
**Sletting**
- Tre tilfelles, avhengig av hvor mange barn noden som skal slettes har
	1. Ingen barn. Lenka fra foreldrenoden som lenker til noden settes lik 0
	2. Ett barn. Lenka fra foreldrenoden som lenker til noden, settes til å lenke til nodens barn.
	3. To barn. Erstatter nodens element med elementet i noden som kommer etter i sorteringsrekkefølgen, som er den minste i høyre subtre.
**Tidskompleksitet for operasjoner på binærtre**
Dybde og høyde
- Høyden være mellom O(log n) og O(n), vanligvis mye nærmere O(log n)
- Antar at antall noder i venstre subtre er omtrent lik antall noder i høyre subtre.
- O(log n) for å finne dypden av en node, da vi må gå fra noden opp til rota.
- Å finne høyden må vi sjekke alle muligheter, og derfor innom alle noder. $\theta(n)$ 
Innsetting, søking og sletting
- Hvis treet har høyde O(log n) vil søking og innsetting få kompleksitet O(log n)
- Tilfelle 1 og 2 får vi ingen løkke og dermed O(1)
- I tilfelle 3 har vi ei løkke O(h) og et rekursivt kall O(1). Sletting vil da O(log n) hvis h er det
- I verste fall blir disse operasjonene O(n)
### B-trær
- Ikke-binære: kan ha hundrevis av barn
- *m-veis tre* er et tre som kan ha maksimalt *m* barn
- Alle løvnoder er på samme nivå