### 7.1 Hva er en graf
**Graf:** En datastruktur som består av *noder*, og en rekke forbindelser mellom disse. Forbindelsene kalles *kanter*.
![[Screenshot 2025-10-01 at 11.12.28.png | 300]]
**Rettet graf:** kantene har en retning
**Urettet graf:** kantene ikke har en retning
**Nabo:** $\text{kant(n,m)}$ sier vi at $m$ er *nabo* til $n$ og skrives $n \to m$.
**Selvløkke:** kant som begynner og slutter i samme node
![[Screenshot 2025-10-01 at 11.19.07.png | 350]]
### 7.2 Implementasjon
- Lagre informasjon om hvilke *noder* og *kanter* som finnes. 
- Nodene legges vanligvis i en tabell. 
- Holde orden på kantene med *naboliste* eller *nabotabell* 
**Graf-fil**
- Første raden angir antall noder $N$ og kanter $K$ i grafen
- Nodene nummereres fra $0$ til $N-1$
**Naboliste**
![[Screenshot 2025-11-12 at 19.47.42.png | 400]]
- Tabell med noder
- Hver node er et *listehode*, der hver node har vi liste over kantene
- Hver kant er et *listeelement* og inneholder
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
- `kant[i][j]`forteller om det er forbindelse fra node `j` til `i`. Den *første* indeksen forteller hvilke node kanten går fra og den *andre* forteller hvilke node kanten går til. 
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
- En nodes *forgjenger* leder oss til en annen node. Derfra kan vi finne den nodens forgjenger osv. 
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
- Opererer *rekursivt*: hvis en node ikke er besøkt, marker den og kjør DFS på naboer
Naboliste
- For hver node må vi sjekke alle dens kanter
- $O(N+K)$
Nabotabell
- For hver node må vi finne kantene, ved å sjekke $N$ mulige kanter
- $O(N^2)$
### 7.5 Topologisk sortering
- **Formål:** ordne noder i en rettet asyklisk graf slik at hver rettet kant fra node A $\to$ node B kommer A før B i sortering
- Noen må være ferdige før andre begynner.
- En rettet graf med en *node* per aktivitet. Når en aktivitet $i$ må være ferdig før $j$ begynner legger vi inn en *kant* fra $i$ til $j$.
- Ta en slik graf og sette nodene i en mulig *rekkefølge*.
- Kan være mange riktige rekkefølger
- Graf som sorteres topologisk må være *asyklisk*.
- Resultatet av topologisk sortering er ei lenket liste med alle nodene i en *logisk* rekkefølge
- $\Theta(N+K)$ 

**Implementasjon (enten BFS eller DFS basert)**
- Start med DFS en tilfeldig node.
- Hver gang DFS er ferdig med å behandle en node, lenkes den inn først i resultatlista, 
- Arbeid bakover og bruk DFS til å behandle en node, lenk den til resultatlista osv.
### 7.6 Sterkt sammenhengende komponenter
En urettet graf kan deles inn i *sammenhengende deler*; finnes en vei fra hver node til enhver annen, og hele grafen sies å henge sammen hvis det er bare en slik del
![[Screenshot 2025-11-12 at 21.01.39.png]]
- **Sterk sammenhengende deler:** En del av en *rettet graf* som er slik at ALLE nodene har en *vei* til alle andre noder. Gjelder begge veier
- For å finne *sterkt sammenhengende deler* trenger vi den omvendte grafen. Den har akkurat de samme *sterkt sammenhengende delene* som originalen
Algoritme for å finne sterkt sammenhengende deler
1. Kjør DFS på alle nodene i grafen, så får man ett eller flere dybde-først-trær. DFS beregner ferdig-tider for alle nodene
2. Sorter nodene i grafen å synkende ferdig-tid
3. Lag den omvendte grafen, $G^T$
4. Kjør DFS på alle nodene i den omvendte grafen. Start med den som fikk høyest ferdig-tid og fortsett nedover.
5. Hvert av dybde først-trærne etter punkt 4 vil være sterkt sammenhengende del av grafen.
- $\Theta(N+K)$, evt $\Theta(N^2)$ 
