#include <paroot.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN_CAPACITY 16

typedef struct {
  int *data;
  int size;
  int capacity;
} Vector;

int next_power_of_two(int n) {
  if (n <= MIN_CAPACITY) {
    return MIN_CAPACITY;
  }

  int power = MIN_CAPACITY;

  while (power < n) {
    power *= 2;
  }

  return power;
}

Vector *vector_create(int initial_capacity) {
  Vector *vector = malloc(sizeof(Vector));

  if (vector == NULL) {
    printf("Failed to allocate memory for vector!\n");

    exit(1);
  }

  vector->capacity = next_power_of_two(initial_capacity);
  vector->size = 0;
  vector->data = malloc(sizeof(int) * vector->capacity);

  if (vector->data == NULL) {
    printf("Failed to allocate memory for vector data!\n");

    free(vector);

    exit(1);
  }

  return vector;
}

void vector_destroy(Vector *vector) {
  free(vector->data);
  free(vector);
}

int vector_size(Vector *vector) { return vector->size; }

int vector_capacity(Vector *vector) { return vector->capacity; }

bool vector_is_empty(Vector *vector) { return vector->size == 0; }

int vector_at(Vector *vector, int index) {
  if (index < 0 || index >= vector->size) {
    printf("Index out of bounds!\n");

    exit(1);
  }

  int *ptr = vector->data + index;

  return *ptr;
}

void resize(Vector *vector, int new_capacity) {
  if (new_capacity < MIN_CAPACITY) {
    new_capacity = MIN_CAPACITY;
  }

  int *new_data = malloc(sizeof(int) * new_capacity);

  if (new_data == NULL) {
    printf("Failed to allocate memory during resize!\n");

    exit(1);
  }

  int *src = vector->data;
  int *dst = new_data;

  for (int i = 0; i < vector->size; i++) {
    *dst = *src;

    src++;
    dst++;
  }

  free(vector->data);

  vector->data = new_data;
  vector->capacity = new_capacity;
}

void vector_push(Vector *vector, int item) {
  if (vector->size == vector->capacity) {
    resize(vector, vector->capacity * 2);
  }

  int *ptr = vector->data + vector->size;

  *ptr = item;

  vector->size++;
}

void vector_insert(Vector *vector, int index, int item) {
  if (index < 0 || index > vector->size) {
    printf("Index out of bounds!\n");

    exit(1);
  }

  if (vector->size == vector->capacity) {
    resize(vector, vector->capacity * 2);
  }

  int *src = vector->data + vector->size - 1;
  int *dst = vector->data + vector->size;

  for (int i = vector->size; i > index; i--) {
    *dst = *src;

    src--;
    dst--;
  }

  int *insert_ptr = vector->data + index;

  *insert_ptr = item;

  vector->size++;
}

void vector_prepend(Vector *vector, int item) {
  vector_insert(vector, 0, item);
}

int vector_pop(Vector *vector) {
  if (vector->size == 0) {
    printf("Cannot pop from empty vector!\n");

    exit(1);
  }

  int *ptr = vector->data + vector->size - 1;

  int value = *ptr;

  vector->size--;

  if (vector->size > 0 && vector->size <= vector->capacity / 4) {
    resize(vector, vector->capacity / 2);
  }

  return value;
}

void vector_delete(Vector *vector, int index) {
  if (index < 0 || index >= vector->size) {
    printf("Index out of bounds!\n");

    exit(1);
  }

  int *dst = vector->data + index;
  int *src = vector->data + index + 1;

  for (int i = index; i < vector->size - 1; i++) {
    *dst = *src;

    src++;
    dst++;
  }

  vector->size--;

  if (vector->size > 0 && vector->size <= vector->capacity / 4) {
    resize(vector, vector->capacity / 2);
  }
}

void vector_remove(Vector *vector, int item) {
  int i = 0;

  while (i < vector->size) {
    int *ptr = vector->data + i;

    if (*ptr == item) {
      vector_delete(vector, i);
    } else {
      i++;
    }
  }
}

int vector_find(Vector *vector, int item) {
  int *ptr = vector->data;

  for (int i = 0; i < vector->size; i++) {
    if (*ptr == item) {
      return i;
    }
    ptr++;
  }
  return -1;
}

int main(void) {
  Vector *vector = vector_create(0);

  while (true) {
    int input = get_int("Enter a number (enter 0 to end): ");

    if (input == 0) {
      break;
    }

    vector_push(vector, input);
  }

  print_array_int(vector->data, vector->size);

  int index = get_int("Enter an index : ");

  printf("Item at index %d is %d\n", index, vector_at(vector, index));

  vector_remove(vector, get_int("Enter an item to remove: "));

  print_array_int(vector->data, vector->size);

  vector_destroy(vector);
}