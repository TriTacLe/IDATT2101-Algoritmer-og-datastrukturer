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
- Kjøretiden kan aldri bli verre enn det som angis med $O$-notasjon ![[Screenshot 2025-08-21 at 11.54.45.png]]Hvis dette er tilfelle, skriver vi $f(n) \in O(g(n))$ og $g(n)$ er asymptotisk øvre grense for $f(n)$
**$\Omega$ - en nedre grense for kjøretid**
- Denne notasjonen brukes for å angi best mulig kjøretid.![[Screenshot 2025-08-21 at 12.47.54.png]]Hvis dette er tilfelle, skriver vi $f(n) \in \Omega (g(n))$ og $g(n)$ er en asymptotisk nedre grense for $f(n)$
**$\Theta$ - en øvre og nedre grense**
- Notasjonen angir både øvre og nedre grense for kjøretid![[Screenshot 2025-08-21 at 12.55.58.png]]Sier at $g(n)$ begrenser $f(n)$ asymptotisk

### Praktisk betydning
- Ulike kjøretid gir ulik ganger kjøretid:
	- Lineær, $n$
	- Kvadratisk, $n^2$
	- Kubisk, $n^3$
	- Konstant tid, $1$: tiden varierer ikke med $n$
