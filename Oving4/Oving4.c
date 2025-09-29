#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_NAME 256 //

// Linked list
typedef struct Node
{
  char *key;
  int value; // CSV row
  struct Node *next;
} Node;

typedef struct HashTable
{
  size_t buckets; // table size
  size_t entries; // total elements
  Node **array;
} HashTable;

size_t hashFunction(const struct HashTable *hash_table, const char *name) // Name is the key
{
  int hash_value = 0;
  for (size_t i = 0; name[i] != '\0'; i++)
  {
    hash_value = (31 * hash_value + name[i]) % hash_table->buckets;
  }
  return hash_value;
}

// insert a key (whole name) -value (row position in the .txt file)
void insertHashTable(HashTable *hash_table, const char *key, int value, int *total_collisions)
{
  size_t index = hashFunction(hash_table, key);
  Node *head = hash_table->array[index];

  size_t bucket_length = 0;
  for (Node *p = head; p; p = p->next)
    bucket_length++;
  if (bucket_length)
  {
    printf("Collision on %zu insert %s with:", index, key);
    for (Node *p = head; p; p = p->next)
      printf(" %s", p->key);
    printf("\n");
  }
  *total_collisions += bucket_length;

  Node *new_node = (Node *)malloc(sizeof(Node));
  new_node->key = strdup(key);
  new_node->value = value;
  new_node->next = head;
  hash_table->array[index] = new_node;
  hash_table->entries++;
}

// returns pointer to the value in key-value
int *searchHashTable(HashTable *hash_table, const char *key, int *search_collision)
{
  size_t index = hashFunction(hash_table, key);
  Node *p = hash_table->array[index];
  while (p)
  {
    if (strcmp(p->key, key) == 0)
    {
      return &p->value;
    }
    (*search_collision)++;
    printf("Search collision on %zu, collide with %s, searching for %s\n", index, p->key, key);
    p = p->next;
  }
  return NULL;
}

// total elements / table size
double calculateLoadfactor(size_t entries, size_t buckets)
{
  return buckets == 0 ? 0 : (double)entries / (double)buckets;
}

int isPrime(int numb)
{
  if (numb % 2 == 0 || numb < 2)
    return 0;
  if (numb == 2)
    return 1;
  for (int i = 3; i <= numb / i; i += 2)
  {
    if (numb % i == 0)
    {
      return 0;
    }
  }
  return 1;
}

int find_table_size(double numb)
{
  while (!isPrime((int)numb))
    numb++;
  return numb;
}

int main(int argc, char **argv)
{
  FILE *fp;
  fp = fopen("navn.txt", "r"); // Open a file in readmode hence "r"
  if (fp == NULL)
  {
    printf("Not able to open file\n");
    return 1;
  }

  char name_file[MAX_NAME];
  int total_elements = 0;

  while ((fgets(name_file, sizeof(name_file), fp)))
    total_elements++;
  rewind(fp);

  // 1.3 times larger than the total elements and it should be a prime number.
  int table_size = find_table_size(1.3 * total_elements);

  HashTable hash_table;

  hash_table.buckets = table_size;
  hash_table.entries = 0;
  hash_table.array = (Node **)calloc(hash_table.buckets, sizeof(Node *));

  int insert_collision = 0;
  int line_count = 0;

  while ((fgets(name_file, sizeof(name_file), fp)))
  {
    // Trim for white space
    size_t n = strlen(name_file);
    if (n && name_file[n - 1] == '\n')
      name_file[n - 1] = '\0';
    line_count++;
    insertHashTable(&hash_table, name_file, line_count, &insert_collision);

    // int hash_value = hashFunction(&hash_table, name_file);
    // printf("%s => %d\n", name_file, hash_value);
  }
  fclose(fp);

  double load_factor = calculateLoadfactor(hash_table.entries, hash_table.buckets);
  double avg_collision_per_person = (double)insert_collision / (double)hash_table.entries;
  printf("Total names: %zu\n", hash_table.entries);
  printf("Table size: %zu\n", hash_table.buckets);
  printf("Total collisions at insert: %d\n", insert_collision);
  printf("Load factor: %.6f\n", load_factor);
  printf("Average collision per person: %.6f\n", avg_collision_per_person);

  int search_collision = 0;
  char *name = "Tri Tac,Le";
  int *pindex = searchHashTable(&hash_table, name, &search_collision);
  pindex ? printf("Searching for: %s at row in name.txt: %d, search collision: %d\n", name, *pindex, search_collision) : printf("Lookup name: %s failed, search collision: %d \n", name, search_collision);

  return 0;
}