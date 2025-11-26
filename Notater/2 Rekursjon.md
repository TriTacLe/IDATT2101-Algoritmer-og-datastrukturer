### 2.1 Rekursjon
- *Rekursiv algoritme:* en algoritme som kaller seg selv
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
Dele problemet på midten
Binærsøkealgoritmen
### Mestermetoden
![[Screenshot 2025-08-27 at 13.22.06.png]]
