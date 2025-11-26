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
-  $\Omega(n)$  om flyttingen ikke skjer i det heletatt
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
- $\theta(n^2)$ 
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
- $T(N)=2T(\frac{n}{2})+n$
### 3.9 Quicksort
- Raskeste sorteringsalgoritmen for generelle data.
- Splitt og hersk
- Hvordan den funker:
	- Tabellen deles i to deler, hvor alle tall i den laveste delen har lavere verdi enn alle tall i den høyeste delen. 
	- Delingen foregår ved at vi velger ett tall fra tabellen som delingstall (*pivot*) og flytter mindre tall mot venstre og større tall mot høyre. Delingsposisjonen blir stedet hvor *pivot* havner
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
#### 3.9.1 Finne en metode som finner *pivot* effektivt
- Å finne medianen er for tidskrevende
- Velge tilfeldig tall? Uheldig hvis man velger det minste eller største tallet som pivot og får $\theta(n^2)$.
- Finne medianen av første, siste og midterste tallet i tabellen og bruker det som pivot. Øker sjansen for et godt pivot. 
``` java
private static int median3sort(int[] tall, int venstreEnd, int hoyreEnd){
	int median = (venstreEnd + hoyreEnd)/2;
	if(tall[venstreEnd]>tall[median]){
		int temp1 = tall[median];
		tall[median] = tall[venstreEnd];
		tall[venstreEnd] = temp1;
	}
	if(tall[median]>tall[hoyre]){
		int temp2 = tall[hoyre];
		tall[hoyre] = tall[median];
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
	
	bytt(tall, midtIndeks, hoyreIndeks - 1);           // flytt pivot ut av veien
	
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
- Partisjonering: velger en *pivot*, flytter tall *mindre* enn pivot til *venstre*, og tall *større* enn pivot til *høyre*, setter pivot på sin endelig plass, og returnerer pivot-indeksen
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
	2. De som er $\geq p_1$ og  $\leq p_2$ 
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
- For hvert element i *inn* tabellen finner vi hvor mange elementer vi har som er mindre eller lik det. Med denne informasjonen kan vi plassere elementet på rett plass i *ut* tabellen. Ex: hvis det fins 17 tall mindre enn $x$, setter vi $x$ på 18 plass. Hjelpetabellen fikser dette. Om det er flere like tall må vi ikke plassere dem på samme plass.
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
	- Hvis det skjer, sorter gjenværende intervall med *heapsort*
	- *heapsort* er $\Theta(n \text{ }log \text{ }n)$ i alle tilfeller. 
- Når et restintervall er lite nok (typisk 16–50)
	- Sorter det med innsettingssortering, i stedet for quicksort
	- Fordi insettingssort er raskere enn quicksort på små del-tabeller
	- raskere enn å bare bruke quicksort
- Introsort er dermed rask (og trygg) i alle tilfeller
	- godt egnet som standard sorteringsalgoritme 
