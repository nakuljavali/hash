# hash

#include <stdio.h>
#include <stdlib.h>

#define SIZE_TABLE 1021

typedef struct _list_t_ {
  int key;
  int value;
  struct _list_t_ *next;
} list_t;

typedef struct _hash_table_t_ {
  int size;
  list_t **table;
} hash_table_t;

hash_table_t *create_hash_table(int size)
{
  hash_table_t *new_table;
  int i;
  
  if (size < 1) return NULL;
  new_table = malloc(sizeof(hash_table_t));

  if (new_table == NULL) {
    printf("Memory Error\n");
    exit(1);
  }
  
  if ((new_table->table = malloc(sizeof(list_t *) * size)) == NULL) {
    return NULL;
  }

  for(i=0; i<size; i++) {
    new_table->table[i] = NULL;
  }

  new_table->size = size;

  return new_table;  
}

unsigned int hash(unsigned int x) 
{
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = ((x >> 16) ^ x);
  return (x % SIZE_TABLE);
}


list_t *lookup_key(hash_table_t *hashtable, unsigned int key)
{
  list_t *list;
  unsigned int hashval = hash(key);

   for(list = hashtable->table[hashval]; list != NULL; list = list->next) {
    if (key == list->key) return list;
  }
  return NULL;
}

int insert(hash_table_t *hashtable, int key, int value)
{
  list_t *new_list;
  list_t *current_list;
  unsigned int hashval = hash(key);

  current_list = lookup_key(hashtable, key);

  if (current_list != NULL) {
    current_list->value = value;
    return 2;
  }

  if ((new_list = malloc(sizeof(list_t))) == NULL) return 1;

  new_list->key = key;
  new_list->value = value;
  new_list->next = hashtable->table[hashval];
  hashtable->table[hashval] = new_list; // basically prepending the list

  return 0;
}

void free_table(hash_table_t *hashtable)
{
  int i;
  list_t *list, *temp;

  if (hashtable==NULL) return;

  for(i=0; i<hashtable->size; i++) {
    list = hashtable->table[i];
    while(list!=NULL) {
      temp = list;
      list = list->next;
      free(temp);
    }
  }

  free(hashtable->table);
  free(hashtable);
}

int main()
{
  hash_table_t *my_hash_table;
  list_t *list;

  my_hash_table = create_hash_table(SIZE_TABLE);

  insert(my_hash_table, 10, 100);
  printf("Inserted key = %d, value = %d \n", 10, 100 );
  list = lookup_key(my_hash_table, 10);
  printf("looked up value: %d \n", list->value);

  insert(my_hash_table, 10, 200);
  printf("Inserted key = %d, value = %d \n", 10, 200 );
  list = lookup_key(my_hash_table, 10);
  printf("looked up value: %d \n", list->value);
  
  free_table(my_hash_table);
}
