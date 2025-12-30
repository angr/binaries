#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Test case 1: Loop counter naming (nested loops)
// Expected: outer loop counter -> i, inner loop counter -> j
int sum_matrix(int n)
{
  int sum = 0;
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      sum += i * j;
    }
  }
  return sum;
}

// Test case 2: Triple nested loops
// Expected: i, j, k for the three nesting levels
int triple_nested_loops(int n)
{
  int sum = 0;
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      for (int k = 0; k < n; k++)
      {
        sum += i * j * k;
      }
    }
  }
  return sum;
}

// Test case 3: Pointer patterns
// Expected: pointer variables named as ptr, cur, etc.
void pointer_test(int* data, int count)
{
  int* ptr = data;
  int* end = data + count;

  while (ptr < end)
  {
    *ptr = *ptr * 2;
    ptr++;
  }
}

// Test case 4: Linked list traversal pattern
// Expected: cur/iter/node naming for iterator
struct Node {
  int value;
  struct Node* next;
};

int sum_linked_list(struct Node* head)
{
  int sum = 0;
  struct Node* cur = head;

  while (cur != NULL)
  {
    sum += cur->value;
    cur = cur->next;
  }

  return sum;
}

// Test case 5: Array indexing patterns
// Expected: index variables named as idx, index, etc.
void array_scale(int* arr, int size, int scale)
{
  for (int idx = 0; idx < size; idx++)
  {
    arr[idx] = arr[idx] * scale;
  }
}

// Test case 6: Call result naming
// Expected: ptr for malloc result, len for strlen result
char* duplicate_string(const char* input)
{
  int len = strlen(input);
  char* ptr = malloc(len + 1);

  if (ptr != NULL)
  {
    strcpy(ptr, input);
  }

  return ptr;
}

// Test case 7: Size parameter naming
// Expected: size/n/count for size parameters
void copy_data(void* dst, const void* src, size_t size)
{
  memcpy(dst, src, size);
}

// Test case 8: Boolean flag patterns
// Expected: flag/found/result naming for boolean variables
int find_value(int* arr, int size, int target)
{
  int found = 0;

  for (int i = 0; i < size; i++)
  {
    if (arr[i] == target)
    {
      found = 1;
      break;
    }
  }

  return found;
}

// Test case 9: Multiple boolean flags
// Expected: Multiple flag variables
int validate_input(const char* str, int min_len, int max_len)
{
  int valid = 1;
  int has_digits = 0;
  int has_letters = 0;

  int len = strlen(str);

  if (len < min_len || len > max_len)
  {
    valid = 0;
  }
  else
  {
    for (int i = 0; i < len; i++)
    {
      if (str[i] >= '0' && str[i] <= '9')
      {
        has_digits = 1;
      }
      else if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z'))
      {
        has_letters = 1;
      }
    }

    if (!has_digits || !has_letters)
    {
      valid = 0;
    }
  }

  return valid;
}

// Test case 10: File I/O with call results
// Expected: fp for fopen result, count for fread result
int read_file_size(const char* filename)
{
  FILE* fp = fopen(filename, "rb");

  if (fp == NULL)
  {
    return -1;
  }

  // Seek to end
  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  fclose(fp);

  return (int)size;
}

// Test case 11: Memory allocation with size parameters
// Expected: ptr for malloc, size for parameter
void* allocate_buffer(size_t count, size_t element_size)
{
  size_t total_size = count * element_size;
  void* ptr = malloc(total_size);

  if (ptr != NULL)
  {
    memset(ptr, 0, total_size);
  }

  return ptr;
}

// Test case 12: String processing with multiple patterns
// Expected: Multiple naming patterns
int count_words(const char* str)
{
  int count = 0;
  int in_word = 0;

  const char* ptr = str;

  while (*ptr != '\0')
  {
    if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n')
    {
      in_word = 0;
    }
    else if (!in_word)
    {
      in_word = 1;
      count++;
    }
    ptr++;
  }

  return count;
}

int main(int argc, char** argv)
{
  if (argc < 2) {
    return 1;
  }

  if (!strcmp(argv[1], "--sum_matrix")) {
    int result = sum_matrix(atoi(argv[2]));
    printf("Sum of matrix elements: %d\n", result);
  }
  else if (!strcmp(argv[1], "--triple_nested")) {
    int result = triple_nested_loops(atoi(argv[2]));
    printf("Triple nested result: %d\n", result);
  }
  else if (!strcmp(argv[1], "--pointer_test")) {
    int data[] = {1, 2, 3, 4, 5};
    pointer_test(data, 5);
    printf("Pointer test done\n");
  }
  else if (!strcmp(argv[1], "--linked_list")) {
    struct Node n3 = {30, NULL};
    struct Node n2 = {20, &n3};
    struct Node n1 = {10, &n2};
    int result = sum_linked_list(&n1);
    printf("Linked list sum: %d\n", result);
  }
  else if (!strcmp(argv[1], "--array_scale")) {
    int arr[] = {1, 2, 3, 4, 5};
    array_scale(arr, 5, 2);
    printf("Array scaled\n");
  }
  else if (!strcmp(argv[1], "--duplicate")) {
    char* dup = duplicate_string(argv[2]);
    if (dup) {
      printf("Duplicated: %s\n", dup);
      free(dup);
    }
  }
  else if (!strcmp(argv[1], "--find_value")) {
    int arr[] = {1, 2, 3, 4, 5};
    int result = find_value(arr, 5, atoi(argv[2]));
    printf("Found: %d\n", result);
  }
  else if (!strcmp(argv[1], "--validate")) {
    int result = validate_input(argv[2], 4, 20);
    printf("Valid: %d\n", result);
  }
  else if (!strcmp(argv[1], "--file_size")) {
    int result = read_file_size(argv[2]);
    printf("File size: %d\n", result);
  }
  else if (!strcmp(argv[1], "--count_words")) {
    int result = count_words(argv[2]);
    printf("Word count: %d\n", result);
  }

  return 0;
}
