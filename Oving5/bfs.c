#include <stdlib.h>
#include <stdio.h>

typedef struct kant_st // Kanttypen
{
  struct kant_st *neste_kant; // Referanse til neste gang i kantlista
  struct node_st *til_node;   // Hvilken node kanten går til
} Kant;

typedef struct node_st // Nodetypen som referer til første listelement i nodens kantliste
{
  Kant *kant1; // Første listelementet i nodens kantliste
  void *d;     // Annen data
} Node;

typedef struct graf_st // Inneholder en tabell med alle nodene i grafen
{
  int N, K;
  Node *node;
} Graf;

typedef struct
{
  int dist; // Avstand mellom noden og startnoden
  Node *forgjenger;
} Forgjenger;

Graf *ny_ugraf(FILE *f)
{
  Graf *graf = (Graf *)malloc(sizeof(Graf));
  fscanf(f, "i% i%\n", &graf->N, &graf->K);
}

#define infinite 1000000000

Forgjenger *new_forgjenger()
{
  Forgjenger *forgjenger = calloc(1, sizeof(Forgjenger));
  forgjenger->dist = infinite;
  return forgjenger;
}

void initForgj(Graf *graf, Node *node)
{
  for (int i = graf->N; i--;)
  {
    graf->node[i].d = new_forgjenger();
  }
  ((Forgjenger *)node->d)->dist = 0;
}

void bfs(Graf *graf, Node *node)
{
  initForgj(graf, node);
  while (!){
    
  }
}