#include <paroot.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 256

typedef struct Entry {
  string key;
  int value;
  struct Entry *next;
} Entry;

typedef struct {
  Entry **entries;
  int size;
} HashTable;

int hash(string key) {
  unsigned long hash = 5381;

  char *ptr = key;

  while (*ptr != '\0') {
    hash = ((hash << 5) + hash) + *ptr;

    ptr++;
  }

  return hash % CAPACITY;
}

HashTable *hashtable_create(void) {
  HashTable *hashtable = malloc(sizeof(HashTable));

  if (hashtable == NULL) {
    printf("Failed to allocate memory for hash table!\n");

    exit(1);
  }

  hashtable->entries = calloc(CAPACITY, sizeof(Entry *));

  if (hashtable->entries == NULL) {
    printf("Failed to allocate memory for entries!\n");

    free(hashtable);

    exit(1);
  }

  hashtable->size = 0;

  return hashtable;
}
void hashtable_destroy(HashTable *hashtable) {
  for (int i = 0; i < CAPACITY; i++) {
    Entry *entry = hashtable->entries[i];

    while (entry != NULL) {
      Entry *next = entry->next;

      free(entry->key);
      free(entry);

      entry = next;
    }
  }

  free(hashtable->entries);
  free(hashtable);
}
void hashtable_add(HashTable *hashtable, string key, int value) {
  int index = hash(key);

  Entry *entry = hashtable->entries[index];

  while (entry != NULL) {
    if (strcmp(entry->key, key) == 0) {
      entry->value = value;

      return;
    }

    entry = entry->next;
  }

  Entry *new_entry = malloc(sizeof(Entry));
  if (new_entry == NULL) {
    printf("Failed to allocate memory for entry!\n");

    exit(1);
  }

  new_entry->key = malloc(strlen(key) + 1);
  if (new_entry->key == NULL) {
    printf("Failed to allocate memory for key!\n");

    free(new_entry);

    exit(1);
  }

  strcpy(new_entry->key, key);

  new_entry->value = value;
  new_entry->next = hashtable->entries[index];

  hashtable->entries[index] = new_entry;

  hashtable->size++;
}
bool hashtable_exists(HashTable *hashtable, string key) {
  int index = hash(key);

  Entry *entry = hashtable->entries[index];

  while (entry != NULL) {
    if (strcmp(entry->key, key) == 0) {
      return true;
    }

    entry = entry->next;
  }

  return false;
}
int hashtable_get(HashTable *hashtable, string key) {
  int index = hash(key);

  Entry *entry = hashtable->entries[index];

  while (entry != NULL) {
    if (strcmp(entry->key, key) == 0) {
      return entry->value;
    }

    entry = entry->next;
  }

  printf("Key not found: %s\n", key);

  exit(1);
}
void hashtable_remove(HashTable *hashtable, string key) {
  int index = hash(key);

  Entry *entry = hashtable->entries[index];
  Entry *prev = NULL;

  while (entry != NULL) {
    if (strcmp(entry->key, key) == 0) {
      if (prev == NULL) {
        hashtable->entries[index] = entry->next;
      } else {
        prev->next = entry->next;
      }

      free(entry->key);
      free(entry);

      hashtable->size--;

      return;
    }

    prev = entry;
    entry = entry->next;
  }
}
int main(void) {
  HashTable *hashtable = hashtable_create();

  hashtable_add(hashtable, "John Doe", 25);
  hashtable_add(hashtable, "Jane Smith", 30);
  hashtable_add(hashtable, "Jim Beam", 22);

  printf("Before delete:\n");
  printf("John Doe: %d\n", hashtable_get(hashtable, "John Doe"));
  printf("Jane Smith: %d\n", hashtable_get(hashtable, "Jane Smith"));
  printf("Jim Beam: %d\n", hashtable_get(hashtable, "Jim Beam"));

  printf("\nDeleting Jane Smith...\n");
  hashtable_remove(hashtable, "Jane Smith");

  printf("\nAfter delete:\n");
  printf("John Doe exists: %s\n",
         hashtable_exists(hashtable, "John Doe") ? "true" : "false");
  printf("Jane Smith exists: %s\n",
         hashtable_exists(hashtable, "Jane Smith") ? "true" : "false");
  printf("Jim Beam exists: %s\n",
         hashtable_exists(hashtable, "Jim Beam") ? "true" : "false");

  printf("\nRe-adding Jane Smith with new value...\n");
  hashtable_add(hashtable, "Jane Smith", 99);
  printf("Jane Smith: %d\n", hashtable_get(hashtable, "Jane Smith"));

  hashtable_destroy(hashtable);
}