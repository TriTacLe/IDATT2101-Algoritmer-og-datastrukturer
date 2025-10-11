#include <stdio.h>
#include <stdlib.h>

#define infinite 1000000000

typedef struct edge
{
  struct edge *next_edge;
  struct node *to_node;
} Edge;

typedef struct node
{
  Edge *edge1;
  void *other;
} Node;

typedef struct graph
{
  int N, K; // Antall noder, antall kanter
  Node *node;
} Graph;

typedef struct predecessor
{
  int distance; // mellom noden og startnoden
  Node *predecessor;
} Predecessor;

Predecessor *new_predecessor()
{
  Predecessor *predecessor = calloc(1, sizeof(Predecessor));
  predecessor->distance = infinite;
  return predecessor;
}

void initPredecessor(Graph *graph, Node *startNode)
{
  for (int i = graph->N; i--;)
  {
    graph->node[i].other = new_predecessor();
  }
  ((Predecessor *)startNode->other)->distance = 0;
}

Graph *new_graph(FILE *filePointer)
{
  Graph *graph = (Graph *)malloc(sizeof(Graph));
  fscanf(filePointer, "%i %i\n", &graph->N, &graph->K);
  graph->node = (Node *)calloc(graph->N, sizeof(Node));
  for (int i = 0; i < graph->K; i++)
  {
    int from, to;
    Edge *edge = (Edge *)malloc(sizeof(Edge));
    fscanf(filePointer, "%i %i\n", &from, &to);
    edge->to_node = &graph->node[to];
    edge->next_edge = graph->node[from].edge1;
    graph->node[from].edge1 = edge;
  }
  return graph;
}

void bfs(Graph *graph, Node *startNode)
{
  initPredecessor(graph, startNode);
  Node **queue = malloc(graph->N * sizeof(Node *));
  int head = 0, tail = 0;
  queue[tail++] = startNode;
  while (head < tail)
  {
    Node *node = queue[head++];
    for (Edge *edge = node->edge1; edge; edge = edge->next_edge)
    {
      Predecessor *predecessor = (Predecessor *)edge->to_node->other;
      if (predecessor->distance == infinite)
      {
        predecessor->distance = ((Predecessor *)node->other)->distance + 1;
        predecessor->predecessor = node;
        queue[tail++] = edge->to_node;
      }
    }
  }
}

int main()
{
  FILE *fptr;
  fptr = fopen("ø5g1.txt", "r");

  if (fptr == NULL)
  {
    printf("Not able to open file");
    exit(1);
  }

  Graph *graph = new_graph(fptr);

  int start = 3;
  Node *startNode = &graph->node[start];
  bfs(graph, startNode);
  printf("Node Predecessor Distance\n");
  for (int i = 0; i < graph->N; i++)
  {
    Predecessor *predecessor = (Predecessor *)graph->node[i].other;
    int predecessor_idx = 0;
    if (predecessor->predecessor)
      predecessor_idx = (int)((Node *)predecessor->predecessor - graph->node);
    printf("%d %d %d\n", i, predecessor_idx, predecessor->distance);
  }

  fclose(fptr);

  return 0;
}