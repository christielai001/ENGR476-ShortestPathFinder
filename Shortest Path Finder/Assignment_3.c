#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 8
#define INFINITY 999999

typedef struct
{
    char label[3];
    int value;
} Node;

FILE *file;
char file_name[100];
Node matrix[SIZE][SIZE];
Node source_node, destination_node, current_node, node1, node2;
Node visited_node[SIZE], unvisited_node[SIZE]; // store all the visited and unvisited nodes
int visited_count = 0;
int unvisited_count = 0;
int node_count = 0;
int shorter_distance = INFINITY;
int distance[SIZE];
int distance_count = 0;
int min_distance = 0;
Node cost[20];
int cost_count = 0;

Node first_node[20];
int first_node_count = 0;

int count[26] = {0};

Node neighbors[20];
int neighbors_count = 0;
Node neighbor_node[SIZE];

void matrix_node(Node matrix[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            // Make the matrix value of all 0 into nodes
            if (matrix[i][j].value == 0)
            {
                unvisited_node[unvisited_count] = matrix[i][j];
                unvisited_count++;
            }
        }
    }
}

// Removes the unvisited node that is already in visited node
void removesUnvisitedNode()
{
    for (int i = 0; i < visited_count; i++)
    {
        for (int j = 0; j < unvisited_count; j++)
        {
            if (strcmp(visited_node[i].label, unvisited_node[j].label) == 0)
            {
                // Remove the common node from unvisited_node by shifting elements
                for (int k = j; k < (unvisited_count)-1; k++)
                {
                    unvisited_node[k] = unvisited_node[k + 1];
                }
                (unvisited_count)--;
                break;
            }
        }
    }
}

// Compare nodes with labels that are the same but reversed
void compare_Node_labels(Node matrix[SIZE][SIZE], Node first_node[20])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            int length = strlen(matrix[i][j].label);
            if (length == 2)
            {
                char reversed_label[3];
                strcpy(reversed_label, matrix[i][j].label);
                strrev(reversed_label); // reverse the string from AB to BA

                for (int x = i + 1; x < SIZE; x++)
                {
                    for (int y = 0; y < SIZE; y++)
                    {
                        if (strlen(matrix[x][y].label) == length && strcmp(matrix[x][y].label, reversed_label) == 0)
                        {
                            strcpy(first_node[first_node_count].label, matrix[i][j].label);
                            first_node[first_node_count].value = matrix[i][j].value;
                            first_node_count++;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void Shortest_Path()
{
    // Find the cost nodes
    for (int i = 0; i < first_node_count; i++)
    {
        if (first_node[i].value != INFINITY)
        {
            strcpy(cost[cost_count].label, first_node[i].label);
            cost[cost_count].value = first_node[i].value;
            cost_count++;
        }
    }

    // Find the neighboring nodes
    int smallest_value = INFINITY;
    int smallest_index = -1;

        for (int i = 0; i < cost_count; i++)
        {
            if (cost[i].label[0] == current_node.label[0])
            {

                neighbor_node[i].label[0] = cost[i].label[1]; // Set the second character as the neighbor's label
                neighbor_node[i].label[1] = cost[i].label[0]; // Null-terminate the label string
                neighbor_node[i].value = cost[i].value;

                neighbors[neighbors_count] = neighbor_node[i];
                neighbors_count++;

                if (cost[i].value < smallest_value)
                {
                    smallest_value = cost[i].value;
                    smallest_index = i;
                }

                if (smallest_index != -1)
                {
                    current_node.label[0] = cost[smallest_index].label[1];
                    current_node.value = cost[smallest_index].value;
                }
            }

            else if (cost[i].label[1] == current_node.label[0])
            {
                neighbor_node[i].label[0] = cost[i].label[0]; // Set the second character as the neighbor's label
                neighbor_node[i].label[1] = cost[i].label[1]; // Null-terminate the label string
                neighbor_node[i].value = cost[i].value;

                neighbors[neighbors_count] = neighbor_node[i];
                neighbors_count++;

                if (cost[i].value < smallest_value)
                {
                    smallest_value = cost[i].value;
                    smallest_index = i;
                }
            }
        }
    // Set the neighboring node with the smallest value as the new current node
    if (smallest_index != -1)
    {
        current_node.label[0] = cost[smallest_index].label[0];
        current_node.value = cost[smallest_index].value;
    }
}


int main()
{
    // Prompt the user to enter a file to read and output the content of the file
    printf("Please enter input file name: ");
    gets(file_name);

    file = fopen(file_name, "r"); // opens the file and read the content data

    if (file == NULL)
    {                                        // check to see if the file can open
        printf("Error opening the file.\n"); // if file does not open it terminates the file
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

    fclose(file); // close and saves the written data into the file */

    matrix_node(matrix);

    // Prints statements
    printf("\nMatrix Labels with Characters: \n");
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            printf("%s\t", matrix[i][j].label);
        }
        printf("\n");
    }

    // Print the matrix_labels
    printf("\nMatrix labels with Integers: \n");
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            printf("%d\t", matrix[i][j].value);
        }
        printf("\n");
    }

    // Prompt user to enter source and destination node
    printf("\nPlease enter a source node: ");
    scanf(" %s", &source_node.label);

    printf("Please enter a destination node: ");
    scanf(" %s", &destination_node.label);

    // Make the source node a visited node
    strcpy(current_node.label, source_node.label);
    strcpy(visited_node[visited_count].label, current_node.label);
    visited_node[visited_count].value = 0;
    visited_count++;

    removesUnvisitedNode();

    printf("\nCurrent Node: %s %d\n", current_node.label, current_node.value);

    printf("\nPrint out visited nodes: \n");
    for (int i = 0; i < visited_count; i++)
    {
        printf("%s %d\n", visited_node[i].label, visited_node[i].value);
    }

    printf("\nPrint out unvisited nodes: \n");
    for (int i = 0; i < unvisited_count; i++)
    {
        printf("%s %d\n", unvisited_node[i].label, unvisited_node[i].value);
    }

    compare_Node_labels(matrix, first_node);
    Shortest_Path();

    // Print the first_node
    printf("\nFirst nodes with labels that are the same but reversed:\n");
    for (int i = 0; i < first_node_count; i++)
    {
        printf("%s %d\n", first_node[i].label, first_node[i].value);
    }

    // Print the cost nodes
    printf("\nCost Nodes:\n");
    for (int i = 0; i < cost_count; i++)
    {
        printf("%s %d (Cost)\n", cost[i].label, cost[i].value);
    }

    for (int i = 0; i < 26; i++)
    {
        if (count[i] > 0)
        {
            printf("\n%c: %d", 'A' + i, count[i]);
        }
    }

    // Print the Neighbor nodes
    printf("\nNeighbors nodes: ");
    for (int i = 0; i < neighbors_count; i++)
    {
        printf("\n%c %d\n", neighbors[i]);
    }

    printf("\nCurrent Node: %s %d\n", current_node.label, current_node.value);

    return 0;
}
