#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 8
#define INFINITY 999999

typedef struct
{
    char label[4];
    int value;
} Node;

Node matrix[SIZE][SIZE];
Node node[SIZE], cost[SIZE], neighbors[SIZE], neighbors_node[SIZE];
Node source, destination, current_node;
int node_count = 0, cost_count = 0, neighbors_count = 0;

// Function to find the index of a node in the cost array
int findNodeIndex(char* label)
{
    for (int i = 0; i < cost_count; i++)
    {
        if (strcmp(cost[i].label, label) == 0)
            return i;
    }
    return -1;
}

void matrix_node(Node matrix[SIZE][SIZE], Node cost[SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            // Define the nodes
            if (matrix[i][j].value == 0 && matrix[i][j].value != INFINITY)
            {
                strcpy(node[node_count].label, matrix[i][j].label);
                node[node_count].value = matrix[i][j].value;
                node_count++;
            }

            // Define the cost nodes
            if (matrix[i][j].value > 0 && matrix[i][j].value != INFINITY)
            {
                strcpy(cost[cost_count].label, matrix[i][j].label);
                cost[cost_count].value = matrix[i][j].value;
                cost_count++;
            }
        }
    }
}

void neighbor_node()
{
    int smallest = INFINITY; // Initialize smallest with a large value
    int smallest_index = -1; // Initialize with an invalid index
    Node larger[SIZE];
    int larger_count = 0;

    for (int i = 0; i < cost_count; i++)
    {
        if (cost[i].label[0] == current_node.label[0])
        {
            neighbors_node[i].label[0] = cost[i].label[1];
            neighbors_node[i].label[1] = cost[i].label[0];
            neighbors_node[i].value = cost[i].value;

            // Add current_node value to the cost neighbor node
            neighbors_node[i].value = neighbors_node[i].value + current_node.value;

            neighbors[neighbors_count] = neighbors_node[i];
            neighbors_count++;

            // Update smallest if a smaller value is found
            if (neighbors_node[i].value < smallest)
            {
                smallest = neighbors_node[i].value;
                smallest_index = i;
                neighbors_count--;
            }
        }
    }

    // Update current_node.label if smallest_index is valid
    if (smallest_index != -1)
    {
        current_node.label[0] = neighbors_node[smallest_index].label[0];
        current_node.value = neighbors_node[smallest_index].value;
    }
}

int main()
{
    FILE *file;
    char file_name[100];

    // Prompt the user to enter a file to read and output the content of the file
    printf("Please enter input file name: ");
    gets(file_name);

    file = fopen(file_name, "r"); // opens the file and read the content data

    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return 1;
    }

    // Reads the matrix from the file
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (fscanf(file, "%s %d", matrix[i][j].label, &matrix[i][j].value) == EOF)
            {
                printf("Failed to read the matrix values.\n");
                return 1;
            }
        }
    }

    fclose(file);

    printf("\nEnter the source node: ");
    scanf("%s", source.label);

    printf("\nEnter the destination node: ");
    scanf("%s", destination.label);

    matrix_node(matrix, cost);

    int sourceIndex = findNodeIndex(source.label);
    int destIndex = findNodeIndex(destination.label);

    if (sourceIndex == -1 || destIndex == -1)
    {
        printf("Invalid source or destination node.\n");
        return 1;
    }

    strcpy(current_node.label, source.label);
    current_node.value = 0;

    while (strcmp(current_node.label, destination.label) != 0)
    {
        neighbor_node();
    }

    // Find the shortest path by backtracking from the destination
    char shortest_path[SIZE][4];
    int shortest_path_length = 0;
    strcpy(shortest_path[shortest_path_length++], destination.label);

    while (strcmp(current_node.label, source.label) != 0)
    {
        int index = findNodeIndex(current_node.label);
        strcpy(shortest_path[shortest_path_length++], cost[index].label);
        strcpy(current_node.label, cost[index].label);
    }

    printf("\nShortest Path from %s to %s:\n", source.label, destination.label);
    for (int i = shortest_path_length - 1; i >= 0; i--)
    {
        printf("%s ", shortest_path[i]);
    }
    printf("\n");

    return 0;
}
