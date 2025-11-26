### 9.1 Ulike optimaliseringsmetoder
**Optimaliseringsproblem**: problem der vi ønsker å finne den beste løsningen blant mange mulige. 
- Ex: minimal spenntre og kortest-vei
**Brute force-algoritmer**
- Beregne *alle* muligheter og velge den beste. 
- Bruker mye datamaskinkraft
- Bør unngås hvis de resulterer i eksponentiell kompleksitet, f.eks. $O(2^n)$ eller $O(n!)$
- Kan brukes hvis de resulterer i en polynomisk kompleksitet, f.eks. $O(n^2)$ eller $O(n^5)$

**Splitt og hersk-algoritmer**
- Deler problemet i mindre subproblemer, løser disse og setter samme løsningene til en løsning på det opprinnelige problemet.
- Tre ulike teknikker: Vanlig splitt og hersk med *rekursjon*, og to andre teknikker *uten rekursjon*
- Dynamisk programmering og grådige algoritmer
## 9.2 Dynamisk programmering
- Lik splitt og hersk mtp dele problemer i mindre løsninger og sette dem sammen.
- Egner seg når de samme delproblemene dukker opp mange ganger. Når løsningene på disse delproblemene er beregnet og lagret, kan disse løsningene brukes på nytt neste gang samme delproblem dukker opp. 
- Løsningene er lagret i en tabell
- Dynamisk programmering starter nederst og *lagrer* løsninger som settes sammen til stadige nye løsninger på veien til toppen.
- En del *rå kraft* algoritmer får lavere kompleksitet når vi bruker dynamisk programmering.

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
		1. Ta *ikke* med vare *i*: Da er verdien $T[j][i-1]$ (samme vekt, uten vare *i*)
		2. Ta *med* vare *i* (hvis $v_i \leq j$): Da er verdien $p_i + T[j-v_1][i-1]$ 
		- Vi velger det beste: $T[j][i]=\text{max}(T[j][i-1],p_i+T[j-v_i][i-1])$
- **Dynamisk programmering perspektiv**
	- Verdiene lagres i tabellen $T[i][j]$. Når vi beregner en ny celle bruker vi tidligere lagrede verdier fra tabellen
	- For hver celle $T[i][j]$ sammenligner vi
		1. Ta *ikke* med vare *i*: Da er verdien $T[j][i-1]$ (cellen rett over)
		2. Ta *med* vare *i*: Da er verdien $p_i + T[j-v_i][i-1]$ (pris av vare $i$ + en celle lengre tilbake)
	- $p_i + T[j-v_1][i-1]$ er ikke nødvendigvis forrige celle, men cellen som representerer *hva er maks verdi med den vekten vi har igjen etter å ta vare $i$* 
- $\Theta(V \cdot n)$ 

**Redigeringsavstand (tror ikke det er pensum)**
- Avstanden mellom to strenger: minste antall lovlige endringer vi kan gjøre med det ene ordet for å omforme det til det andre
- Lovlige endringer:
	1. Ta bort et char
	2. Sette inn et char
	3. Endre et char

## 9.3 Grådige algoritmer
- **Definisjon:** Algoritme som alltid gjør det som synes best i øyeblikket uten tanke på helheten. 
	- Går aldri *bakover* på valg og tenker ikke *fremover*
- Raske og effektive, *hvis* problemet lar seg løse slikt
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
- For hvert tegn lager vi et *binærtre* som består av ei rot og lagrer både tegnet og frekvensen i rota. Disse trærne lagrer vi i en *prioritetskø* som er ordnet slikt at tegn med lavest frekvens kommer først ut av køen.
- Henter to og to trær ut fra prioritetskøen, og lar de to trærne være venstre og høyre subtre i et nytt tre og lar rota få en frekvens som er summen av frekvensene i de to subtrærne. Dette treet setter vi inn i prioritetskøen igjen. Deretter kombinerer vi de to neste trærne og forsetter helt til treet er laget. (vi tar da ut først trærne med lavest frekvens - disse havner dermed nederst)
- Når treet er laget setter vi $0$ på høyreside og $1$ på venstreside langs grenene. Da kan vi lese kodene for de ulike tegnene.
- For å **finne skillet mellom tegnene** følger vi treet nedover til vi kommer til en løvnode og slikt har vi fått ett tegn. Deretter starter vi på toppen og finner ett nytt tegn
- Prefixfrie koder: Ingen av huffmann-kodene er prefix

**Hvordan er huffmann en grådig algoritme?**
- Slår sammen de trærne med lavest frekvens fordi det gir minst økning i vektet veilengde - vi gjør det som synes best i øyeblikket.

**Dekoding**
- Huffmankode er prefiksfri. (En kort tegnkode kan aldri være starten på en annen lengre kode
- Dekoding av bitstreng: start i rota, bølg bitmønsteret til vi finner et tegn. Start igjen i rota og fortsett helt til vi har dekodet meldingen
