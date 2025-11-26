### Kompleksitetsklasser
**P:** Mengden av problemer som kan *løses* i polynomisk tid
- $O(1),O(n), O(n^2), O(n^3),...,O(n^{100})$
- Ex: sortering, korteste vei, max flyt
- *Praktisk:* løs dem effektivt
**NP:** Mengden av problemer hvis *løsningen* kan *sjekkes* i polynomisk tid.
- **P** $\in$ **NP**, alle **P**-problemer er også **NP**-problemer
- Eksempel: kan noen tall av $n$ heltall summeres til $0$?
**Det største spørsmålet innen datavitenskap**
- Hvis **P** ≠ **NP**? 
	- Alle problemer hvis løsningen kan sjekkes raskt, kan også løses raskt
- Hvis er **P** = **NP**
	- Noen problemer er vanskeligere å løse enn å sjekke
**NP-komplette problemer (NPC):** en gruppe *vanskeligste* problemene i **NP**
- Kjennetegn
	- NPC $\in$ NP
	- *Kompletthet*: hvert problem i **NP** kan reduseres til et hvilket som helst problem i **NPC** i polynomisk tid
	- Beste kjente løsninger $O(2^n)$
- Betydning av kompletthet
	- Finner vi én polynomisk løsning på *ett* NPC-problem, virker den for alle og vi har løst **P** = **NP**
- **Praktisk betydning**
	- Hvis programmet ikke kjører på rimelig tid:
		- sjekk om problemet er **NP**-komplett
		- isåfall kan du gi opp å finne en eksakt løsning som er kjappere.
	- Se evt etter en tilnærmet løsning (*heuristikker*)
		- Mange av de kjente **NP**-komplette problemene har gode tilnærmede løsninger, som lar seg beregne fort nok
### Eksempler på NPC
**Traveling Salesman**
- *Problem:*
	- Har $n$ byer, og kjente kostnader for å reise mellom dem
	- Fins det en reiserute som besøker alle byene nøyaktig én gang med totalkostnad under $x$
- Kompleksitet
	- Kan prøve alle muligheter ($n!$, eventuelt $\frac{n!}{2n}$)
	- Løsningen (en rekkefølge) kan sjekkes på $O(n)$ tid
**Isomorfi**
- *Problem:* Er $G_1$ isomorf med en subgraf av $G_2$?
- NP men ikke NPC
**Ryggsekkproblemet (gått igjennom i dynamisk programmering)**
- *Problem:*
	- Gitt $n$ varer, hver med pris og vekt
	- Gitt en bærekapasitet
	- Kan vi få med varer for verdien $V$?
- Hvis vekten er desimaltall, går ikke dynamisk programmering
- Vanskelig å finne optimale kombinasjoner, men lett å sjekke et forslag
**Hamiltonsyklus i graf**
- *Problem:*
	- Gitt en graf med $n$ noder
	- Fins en vei som er innom alle n noder én gang og returnerer til startpunktet?
- Alle mulige rekkefølger, $O(n!)$
- Løsningsforslag kan sjekkes i $O(n)$ tid
**Delsum**
- *Problem:* Gitt $n$ heltall, fins det en delmengde av disse som summerer til $0$
- Kan sjekke alle $2^n$ kombinasjoner
- Forslag kan sjekkes i $O(n)$
**Komplett subgraf**
- *Problem:*
	- Gitt en graf
	- Fins en *komplett* subgraf med $x$ noder?
- Løsning sjekkes ved å se etter om subgrafen er komplett
**3SAT, 3-way satisfiability**
- *Problem:*
	- Gitt et *boolsk uttrykk* på formen: $(x_1|x_2|x_3)$ & $(!x_1|!x_4|x_3)$ & $(!x_2|x_4|!x_3)$ & $...$
	- $3$ variabler i hver del, $n$ variabler totalt
	- Fins det en tilordning $(x_1=1,x_2=0,...)$ som gjør hele uttrykket sant?
- $2^n$ mulige kombinasjoner
- Forslag kan sjekkes i $O(n)$ tid
- *Alle* NP-komplette problemer (NPC) kan reduseres til 3SAT
### NP-harde problemer
- **Definisjon**: Et problem er **NP-hardt** hvis og bare hvis: 
	- ethvert problem i *NP* kan reduseres til dette problemet i polynomisk tid
	- Men: svar trenger ikke kunne *sjekkes* i polynomisk tid
- Sammenheng
	- Alle *NPC* er også *NP-harde*
	- Men *NP-hard* problemer trenger ikke å være i *NP*
	- Delvis uløselige problemer som *halting-problemet*
- Optimaliseringsvarianter: 
	1. **Traveling Salesman - Optimalisering:**  
		- Finn den _billigste_ av alle mulige reiseruter (ikke bare "under kostnad x")
	2. **Ryggsekkproblemet - Optimalisering:**  
		- Få med _maksimal_ verdi (ikke bare "verdi minst V")
	3. **Komplett subgraf - Optimalisering:**  
		- Finn den _største_ komplette subgrafen
- *Praktisk:* finn "gode nok" løsninger
### Halting-problemet
**Problemformulering**
- Gitt et program $P$ og inndata $D$
*Spørsmål:* Vil $P$ noen gang avslutte når det kjører med $D$, eller vil det gå i uendelig løkke?
- NP-hardt fordi ja/nei svaret ikke kan sjekkes i polynomisk tid.
**Hvorfor er haltingsproblemet så viktig**
- Hvis vi kunne løse haltingproblemet kunne vi løst *alle* problemer med ja/nei svar
**Eksempel - Goldbachs formodning:** Stemmer det at alle partall $> 2$ er summen av to primtall?
Fremgangsmåte:
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
	- Hvis *ja* kjører $P$ en uendelig løkke
	- Hvis *nei* stopper $P$
```psuedocode
P(Q): // Q er et vilkårlig program
	hvis H(Q,Q) sier "stopper":
		løp i en uendelig løkke
	hvis H(Q,Q) sier "løper evig":
		stopp umiddelbart
```
3. Kjør deretter $P$ med $P$ som input: $P(P)$
4. Analyse:
	- Hvis $H(P,P)$ sier *ja, $P$ stopper* $\to$ vil $P$ gå i uendelig løkke $\to$ $P$ stopper ikke
	- Hvis $H(P,P)$ sier *nei, $P$ looper*, vil $P$ stoppe $\to$ $P$ løper ikke evig
5. Konklusjon: $H$ gir feil svar uansett $\to$ $H$ kan ikke eksistere
	- Altså programmet $H$ er logisk umulig å løse generelt
**Det store spørsmålet igjen**
- Er **P** = **NP**
- Hvis ja, kan alle problemer løses i polynomisk tid
	- Aldri nødvendig med brute force (rå kraft) i problemløsning, fordi *antall muligheter* er større enn polynomisk, ofte $2^n$, $n!$ eller $n^n$
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