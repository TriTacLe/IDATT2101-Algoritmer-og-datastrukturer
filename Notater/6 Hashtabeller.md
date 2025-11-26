 ### 6.1 Hashtabell
- Rask gjenfinning av lagrede verdier, basert på nøkler
- Rask innsetting av nye verdier (nøkler)
- Nøkkelen brukes ikke som tabell indeks
**Virkemåte**
- Hashtabell bruker en *hashfunksjon* som kan konvertere nøkler i streng eller interger til heltall i området $\text{[0,m-1]}$ som kan brukes til å slå opp i en tabell med $m$ plasser. 
- Brukes ved *write og read* 
- Når vi *write* "Tri", lager hashfunksjonen tallet ut fra navnet, og andre opplysninger om "Tri" legges på den plass. Når vi senere *reader* "Tri" vil hashfunksjonen gi oss tallet 21345 hvor vi finner opplysningene lenket med "Tri"
- Funksjonen skal kun beregnes i $\Theta(1)$
- Tidsforbruk avhenger *ikke* av antall data
#### 6.1.1 Lastfaktoren
- **Definisjon:** For en hashtabell med størrelse $m$ og $n$ elementer, er *lastfaktoren* gitt ved: 
$$\alpha = \frac{n}{m}$$
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
- Ganger vi nøkkelen $k$ med et tall $A$, der $0<A<1$. Produktet består av en heltallsdel og en desimaldel. Ganger desimaldelen på størrelsen på hashtabellen $m$for å få indeksen
$$h(k)=[m(kA-[kA)]$$
- Funker for alle verdier av $m$.
- Bruker desimaltall, og beregninger med desimaltall er tregere enn heltallsoperasjoner.
### 6.3 Kollisjonshåndtering
#### 6.3.1 Lenkede lister
- Hver post kan ha ei lenke til neste post, og hashtabellen blir en tabell med listehoder.
![[Screenshot 2025-09-22 at 15.24.26.png]]
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
	- *Kan* løses med å sette inn et markørelement for å markere slettede elementer, men dette har en annen ulempe: etter mye innsetting og sletting vil alle ledige posisjoner inneholde slette-markører og søk etter elementer som ikke fins i tabellen $O(m)$, fordi søket ikke kan gi opp før den har gått igjennom hele tabellen.
- Brukes derfor mest når vi skal legge inn elementer uten å slette annet enn når vi kvitter oss med hele tabellen. 
- Tre måter å implementere åpen addressering: lineær probing, kvadratisk probing og dobbel hasing. Det som skiller seg ut er probesekvensen
##### Lineær probing
- Enkleste form for åpen addressing
- Hvis et nytt element hasher til en opptatt plass, prøver vi neste posisjon, er den opptatt går vi ett steg videre, og gir oss ikke før vi har prøvd alle plassene.
- Sikrer at vi alltid får inn nye elementer når tabellen ikke er full
$$probe(h,i,m)=(h+i) \text{ mod } m$$
![[Screenshot 2025-09-22 at 16.17.26.png|600]]
- Går fra start til slutt for å få plassert tallet
- Problem:
	- Hvis mange elementer kolliderer på samme sted får vi en lang sammenhengende kjede med "feilplasserrte" elementer som må hoppes over hvert eneste oppslag

##### Kvadratisk probing
- Forbedring ift lineær probing
- Tall som kolliderer i ett forsøk vil ikke være nødt til å fortsette å kollidere så fremt de har ulik hashverdi 
$$probe(h,i,m)=(h+k_{1}i+k_{2}i^2) \text{ mod } m$$
![[Screenshot 2025-09-22 at 16.30.32.png]]
- Valg av $k_{1} \text{, } k_{2} \text{ og }m$
	- Hvis alle tre har en felles faktor, vil ikke probsekvensen komme innom alle tabellposisjonene og vi risikerer å ikke kunne plassere elementer selv om det er ledige plasser.

##### Dobbel hasing
- Kvadratisk probing har et problem: elementer som hasher til samme posisjon - hvis den er opptatt, vil begge prøve på nytt i samme sted.
- Bruker to hasfunksjoner i probsekvensen:
	1. En som angir den første posisjonen som prøves
	2. Angir hvor langt vi skal hoppe hvis det blir kollisjon.
$$probe(h_{1}, h_{2}, i, m)=(h_{1}+ih_{2}) \text{ mod }m)$$
![[Screenshot 2025-09-22 at 17.43.18.png]]
- Tall som hasher til samme posisjon, kolliderer ikke med hverandre på andre forsøk, fordi $h_{2}(k)$ lager ulike verdier for de forskjellige verdier av $k$ hvor $h_{1}(k)$ gir samme verdi
- Krav til $h_{2}(k)$
	1. Funksjonen $h_{2}(k)$ må ikke få verdi 0
	2. $h_{2}(k)$ og tabellstørrelsen $m$ må være relativt prime: ikke ha felles faktorer for noen verdi av $k$. Hvis de har en felles faktor vil ikke probesekvensen gå innom alle posisjonene. 
	3. For å minimere antall kollisjoner bør $h_{2}(k) \text{ mod }m$ i største mulig graf gi ulike tall der $h_{1}(k)$ gir like tall.
- Krav om ingen felles faktor kan oppnås:
	- Hvis $m$ er et primtall, kan vi la $h_{2}(k)$ produsere tall i intervallet $[1,m]$. Ex: $h_{2}(k)=k \text{ mod }(m-1)+1$. Dette oppfylles krav 3
	- Hvis $m$ er en toerpotens, kan vi ha $h_{2}(k)$ som bare produserer oddetall. Ex: $h_{2}(k)=(2|k|+1) \text{ mod }m$

**Analyse av åpen adressering**
