## 5.1 Heap
To slags heap: max-heap og min-heap
**Heap:** komplett binærtre med en nøkkel i hver node
- *heapegenskapen:* 
	- Max-heap: Hver node har en nøkkelverdi som er $\geq$ begge barnenodenes
	- Min-heap: Hver node har en nøkkelverdi som er $\leq$ begge barnenodenes
**Anvendelser**
- Sortering og prioritetskø
- **Prioritetskø:** datastruktur hvor man kan legge inn og ta ut elementer som har ulik *prioritet*
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
- Flytter nodene om de ikke følger *heapegenskapen*
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

