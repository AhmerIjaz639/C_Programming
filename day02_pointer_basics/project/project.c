#include <stdio.h>
#include <stdlib.h>

int main()
{
    // 1. Stack variable
    int stack_value = 100;

    // 2. Heap variable
    int *heap_value = malloc(sizeof(int));

    if (heap_value == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    *heap_value = 200;

    // 3. Array
    int arr[3] = {10, 20, 30};

    printf("================================\n");
    printf("       MEMORY INSPECTOR\n");
    printf("================================\n\n");

    // Stack information
    printf("Stack variable\n");
    printf("Value   : %d\n", stack_value);
    printf("Address : %p\n\n", (void *)&stack_value);

    // Heap information
    printf("Heap variable\n");
    printf("Value   : %d\n", *heap_value);
    printf("Address : %p\n\n", (void *)heap_value);

    // Array information
    printf("Array\n");

    for (int i = 0; i < 3; i++)
    {
        printf("Element %d : %d | Address: %p\n",
               i,
               *(arr + i),
               (void *)(arr + i));
    }

    printf("\n");

    // Pointer information
    printf("Pointer\n");
    printf("p points to : %p\n", (void *)heap_value);
    printf("*p value    : %d\n", *heap_value);

    printf("\n================================\n");

    // Cleanup
    free(heap_value);
    heap_value = NULL;

    return 0;
}