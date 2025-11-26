### Tekstsøk
Enkel naiv algoritme for søk
- Dobbeltløkke for $n-m$ posisjoner, og $m$ tegn i søkeordet 
	- Lengde $n$ på tekst man søker i 
	- Lengde $m$ på søkeord
- $O(n \cdot m)$, $\Theta(n)$ 
#### Boyer moore søk
- Se på siste tegn i søkeord først
- Hvis det ikke passer, flytt søkeordet så langt vi kan (til neste match)
**Regelen om upassende tegn**
- Hvis tegnet ikke fins i søketeksten, kan vi flytte $m$ steg fram, hvis mismatch var på *siste* i søketeksten
	- Med mismatch på *nestsiste* tegn kan vi flytte $m-1$ steg
	- Mismtach på *nestnestsiste* tegn, flytter vi $m-2$ steg osv.
- Har en todimensjonell tabell for hvor mye vi kan flytte
	- En indeks er det upassende tegne
	- Den andre indeksen er posisjonen i søketeksten
	- Verdien i cellen er hvor langt vi kan flytte fremover
- Tabellen lager vi med å *preprosessere* søketeksten
- $\Omega(n/m)$ 
**Regelen om passende endelse**
![[Screenshot 2025-11-15 at 12.42.41.png | 400]]
- 0,1: når siste posisjon intreffer *n*, kan vi bare flytte ett steg
- 2: Feil i første posisjon
	- *Regel om upassende tegn* lar oss bare flytte ett steg
- *Regel om passende endelse* lar oss flytte to steg
- Vi slår opp både *upassende tegn* og *passende endelse* og bruker regelen som gir det lengste hoppet.
- Tabellen for *passende endelse*
	- Index er hvor mange tegn som passet
	- Verdien i cellen er hvor langt vi kan flytte
	- Lages ved å prøve ut om søketeksten overlapper med seg selv
**Galil sin regel**
- Hvis vi søker etter "aaa" i "aaaaaa..." har vi $O(n \cdot m)$ da søkeordet passer overalt, og de samme a-ene sjekkes flere ganger
- Unngå unødvendige sammenligninger:
	- Når vi flytter søkeordet kortere enn den delen av søkeordet vi allerede har sjekket, trenger vi ikke sjekke det overlappende området om igjen
	- Korte flytt skjer fordi søkeordet delvis matcher seg selv. Hvis det ikke hadde matchet, hadde vi flyttet lengre
![[Screenshot 2025-11-15 at 13.18.19.png | 500]]
- Programmet trenger ikke å sjekke den oransje regionen om igjen
- $O(n)$ og $\Omega(n/m)$ for tekstsøk
### Datakompresjon
#### Run-length coding
- En serie repetisjoner erstattes med et antall:
	- $ABIIIIIIIIIIIIBBBCDEFFFGH \to AB12I3BCDE3FGH$
- Ulemper
	- Kan være sifre i det vi komprimerer
	- Ser vanligvis på bytes og ikke tekst
	- Kunne skille mellom data og metadata (header)
- Bruker negative byte for ukomprimerte sekvenser:
	- $ABIIIIIIIIIIIIBBBCDEFFFGH$ $\to  [-2]AB[12]I[3]B[-3]CDE[3]F[-2]GH$
- Hva med $ABABABABAB$
#### LZ (Lempel-Ziv)
- Leser gjennom fila
- Input kopieres til output
- Hvis en lang nok sekvens kommer til igjen: 
	- dropp den og skriv eller en referanse til output
	- format: repeter $X$ tegn, som vi har sett $Y$ tegn tidligere
- Hjelper hvis sekvensen er lenger enn en slik referanse
- Søker bakover i et sirkulært *buffer*
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
- LZ leser input og skriver bakover-referanser og sekvenser med ukomprimerte tegn
- Ukomprimerte tegn telles opp og komprimeres videre med Huffmannkoding
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
- Godt eksempel på *adaptivt komprimering* der algoritmen lærer dataens struktur underveis
**Ulemper**
- Ikke optimalt for allerede komprimerte data eller tilfeldige data
- Ordbok kan bli stor
#### LZW + huffmann
- LZW: leser input, bygger en dictionary, skriver ordnumre
- Noen ord forekommer oftere enn andre. Programmet finner antall frekvenser for ulike ordnumre
	- Skriver huffmannkoder i stedet for ordnumre
	- Ord som forekommer ofte, får kortere koder
#### Burrows-Wheeler transformasjonen (BWT)
- *Hva:* Smart måte å organisere tekst på slik at den blir lettere å komprimere. Reorganiserer rekkefølgen slik at like tegn havner ved siden av hverandre. Tar tekst med repeterende mønstre og gjør dem om til repeterende tegn
- *Eksempel:* "BANANA"
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
#### Move-to-front transformasjonen (MTF)
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
*Tegn 1: B*
- Finn B i tabellen → B er på index 1
- Skriv 1 (index) til output
- Flytt B til front (move to front): `[B, A, C, D]`
```
Input:  B
Output: 1
Tabell: [B, A, C, D]
```
*Tegn 2: A*
- Finn A i tabellen → A er på index 1
- Skriv 1 til output
- Flytt A til front: `[A, B, C, D]`
```
Input:  B A
Output: 1 1
Tabell: [A, B, C, D]
```
*Tegn 3: N*
- Finn N i tabellen → N er på index 13 (i et fullt alfabet)
- Skriv 13 til output
- Flytt N til front: `[N, A, B, C, D, ...]`
```
Input:  B A N
Output: 1 1 13
Tabell: [N, A, B, C, D, ...]
```
*Tegn 4: A*
- Finn A i tabellen → A er på index 1 (fordi N nå er først)
- Skriv 1 til output
- Flytt A til front: `[A, N, B, C, D, ...]`
```
Input:  B A N A
Output: 1 1 13 1
Tabell: [A, N, B, C, D, ...]
```
*Tegn 5: N*
- Finn N i tabellen → N er på index 1 (fordi A nå er først)
- Skriv 1 til output
- Flytt N til front: `[N, A, B, C, D, ...]`
```
Input:  B A N A N
Output: 1 1 13 1 1
Tabell: [N, A, B, C, D, ...]
```
*Tegn 6: A*
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

| Input tegn | Finn index | Output | Ny tabell         |
| ---------- | ---------- | ------ | ----------------- |
| A          | 0          | **0**  | [A, B, C, D, ...] |
| A          | 0          | **0**  | [A, B, C, D, ...] |
| A          | 0          | **0**  | [A, B, C, D, ...] |
| B          | 1          | **1**  | [B, A, C, D, ...] |
| B          | 0          | **0**  | [B, A, C, D, ...] |
| A          | 1          | **1**  | [A, B, C, D, ...] |
- *Resultat:* `0 0 0 1 0 1`
**Hvorfor**
- Repeterte tegn `AAA` blir til `000`
- Mange nuller og små tall gir effektiv huffmannkoding

**Enda et eksempel**
- inn:`caaaaacbbbbbbbaaaabb`
- ut:`21000012000000200010`

| Frekvens før        | Frekvens etter          |
| ------------------- | ----------------------- |
| a                 9 | 0                   14  |
| b                 9 | 1                     3 |
| c                 2 | 2                    3  |
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
