#include <stdio.h>
#include <stdlib.h>

#define infinite 1000000000 // For å markere uoppdaget node

// kant i naboliste
typedef struct edge
{
  struct edge *next_edge; // neste kant fra samme "fra node"
  struct node *to_node;   // peker til "til" noden altså naboen
} Edge;

// en enkel node i grafen
typedef struct node
{
  Edge *edge1; // første kanten i nabolisten med flere kanter fa denne noden
  void *other; //
} Node;

// grafen
typedef struct graph
{
  int N, K;
  Node *node; // pointer til en array med noder
} Graph;

typedef struct predecessor
{
  int distance;      // korteste avstand mellom noden og startnoden
  Node *predecessor; // forrige node på
} Predecessor;

// funksjon som lager forgjengere
Predecessor *new_predecessor()
{
  // Setter alle noder til uoppdaget
  Predecessor *predecessor = calloc(1, sizeof(Predecessor)); // nuller ut
  predecessor->distance = infinite;
  return predecessor;
}

void initPredecessor(Graph *graph, Node *startNode)
{
  int startNodeIdx = (int)(startNode - &graph->node[0]);
  if (startNodeIdx > graph->N - 1)
  {
    printf("Start node is out of bounds. will lead to segfault\n");
    printf("Start node: %d, total nodes: %d\n", startNodeIdx, graph->N);
    exit(1);
  }
  for (int i = graph->N; i--;)
  {
    // Setter other feltet til predecessor
    graph->node[i].other = new_predecessor();
  }
  // Startnode avstand 0 da avstand fra startnode til startnode er 0
  ((Predecessor *)startNode->other)->distance = 0;
  ((Predecessor *)startNode->other)->predecessor = NULL;
}

// Naboliste
Graph *new_graph(FILE *filePointer)
{
  Graph *graph = (Graph *)malloc(sizeof(Graph));
  fscanf(filePointer, "%d %d", &graph->N, &graph->K);
  // array med noder
  graph->node = (Node *)calloc(graph->N, sizeof(Node));

  for (int i = 0; i < graph->K; i++)
  {
    int from, to;
    Edge *edge = (Edge *)malloc(sizeof(Edge));
    fscanf(filePointer, "%d %d", &from, &to);

    // initialiserer edge (kant)
    edge->to_node = &graph->node[to];          // Lagrer [til] noden i pekeren til kanten
    edge->next_edge = graph->node[from].edge1; // neste kant er lik gammel hode kant

    graph->node[from].edge1 = edge; // ny kant til topp kant
  }
  return graph;
}

void bfs(Graph *graph, Node *startNode)
{
  initPredecessor(graph, startNode);
  Node **queue = malloc(graph->N * sizeof(Node *));
  int head = 0, tail = 0;
  // Startnode i køen
  queue[tail++] = startNode;
  while (head < tail)
  {
    Node *node = queue[head++];
    // itererer gjennom nabolisten til node til edge ikek er null, kunne brukt while her
    for (Edge *edge = node->edge1; edge; edge = edge->next_edge)
    {
      // Accesser data (pv) til naboen
      Predecessor *pv = (Predecessor *)edge->to_node->other; // to_node e naboen til node
      // hvis ubesøkt
      if (pv->distance == infinite)
      {
        // Distance til nabo = avstand til nåværende node + 1
        pv->distance = ((Predecessor *)node->other)->distance + 1;
        // lagrer forgjenger
        pv->predecessor = node;
        // legg nabo i kø bakerst
        queue[tail++] = edge->to_node;
      }
    }
  }
  free(queue);
}

// BFS
void del1(FILE *fptr, int startNodeIdx)
{
  if (fptr == NULL)
  {
    printf("Not able to open file\n");
    exit(1);
  }

  Graph *graph = new_graph(fptr);

  Node *startNode = &graph->node[startNodeIdx];
  bfs(graph, startNode);
  printf("Node | Predecessor | Distance\n");
  for (int node = 0; node < graph->N; node++)
  {
    Predecessor *pv = (Predecessor *)graph->node[node].other;
    int predecessor_idx = -1;
    if (pv->predecessor)
    {
      predecessor_idx = (int)((Node *)pv->predecessor - graph->node);
      printf("%d | %d | %d\n", node, predecessor_idx, pv->distance);
    }
    else
    {
      if (predecessor_idx == -1 && pv->distance == 0)
      {
        printf("%d |  | %d\n", node, pv->distance);
      }
      else if (predecessor_idx == -1 && pv->distance == infinite)
      {
        printf("%d |  | not reachable\n", node);
      }
    }
  }
  fclose(fptr);
}

// del 2-------------------------------------------------------
// del 2-------------------------------------------------------
// del 2-------------------------------------------------------
// del 2-------------------------------------------------------

typedef struct topological_list
{
  int found;
  Node *next;
} Topo_lst;

Node *dfsTopo(Node *startNode, Node *nodeLst)
{
  Topo_lst *startNodeInfo = startNode->other;

  if (startNodeInfo->found)
  {
    return nodeLst;
  }

  startNodeInfo->found = 1;

  for (Edge *edge = startNode->edge1; edge; edge = edge->next_edge)
  {
    nodeLst = dfsTopo(edge->to_node, nodeLst);
  }

  startNodeInfo->next = nodeLst;
  return startNode;
}

Node *topologicalSearch(Graph *graph)
{
  Node *nodeLst = 0;

  for (int i = graph->N; i--;)
  {
    graph->node[i].other = calloc(sizeof(Topo_lst), 1);
  }
  for (int i = graph->N; i--;)
  {
    nodeLst = dfsTopo(&graph->node[i], nodeLst);
  }
  return nodeLst;
}

// TOPOLOGISK
void del2(FILE *fptr)
{
  Graph *graph = new_graph(fptr);
  Node *start = topologicalSearch(graph);

  // Linked list iterasjon
  for (Node *node = start; node; node = ((Topo_lst *)node->other)->next)
  {
    int nodeIdx = (int)(node - &graph->node[0]);
    printf("%d ", nodeIdx);
  }
  printf("\n");
  fclose(fptr);
}

int main(int argc, char **argv)
{
  FILE *fptr;
  // fptr = fopen("ø5g1.txt", "r");
  // fptr = fopen("ø5g2.txt", "r");
  // fptr = fopen("ø5g3.txt", "r");
  fptr = fopen("ø5g5.txt", "r");
  // fptr = fopen("ø5g7.txt", "r");

  // fptr = fopen("ø5Skandinavia.txt", "r");

  int startNodeIdx = 0; // For bfs

  int del = (argc >= 2) ? atoi(argv[1]) : 1;
  (del == 1) ? del1(fptr, startNodeIdx) : del2(fptr);

  return 0;
}