#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

int tree[MAX_SIZE]; // Array to hold the binary tree nodes
int values[MAX_SIZE]; // Array to hold the input values
int value_count = 0; // Count of input values

void init_tree() {
    for (int i = 0; i < MAX_SIZE; i++) {
        tree[i] = -1; // Initialize the tree array with -1 to indicate empty
    }
}

void build_tree() {
    init_tree(); // Clear the tree
    for (int i = 0; i < value_count; i++) {
        int value = values[i];
        if (i == 0) {
            tree[0] = value; // First value as root
        } else {
            int parent = (i - 1) / 2; // Calculate parent index
            if (i % 2 == 1) {
                tree[(parent * 2) + 1] = value; // Set left child
            } else {
                tree[(parent * 2) + 2] = value; // Set right child
            }
        }
    }
}

void delete_value(int index) {
    if (index < 0 || index >= value_count) {
        printf("Invalid index for deletion.\n");
        return;
    }
    int key = values[index];
    
    // Shift the elements left to remove the value
    for (int j = index; j < value_count - 1; j++) {
        values[j] = values[j + 1];
    }
    values[value_count - 1] = -1; // Clear last element
    value_count--; // Decrease the count
    printf("Node '%d' deleted from array.\n", key);
    build_tree(); // Rebuild the tree after deletion
}

void print_tree() {
    printf("\nBinary Tree: ");
    for (int i = 0; i < MAX_SIZE; i++) {
        if (tree[i] != -1) {
            printf("%d ", tree[i]); // Print the node
        } else {
            printf("- "); // Print placeholder for empty nodes
        }
    }
    printf("\n");
}

int main() {
    int choice;
    int value;
    int num_values, index_to_delete;

    // Initialize the values array
    for (int i = 0; i < MAX_SIZE; i++) {
        values[i] = -1; // Initialize with -1 to indicate empty
    }

    do {
        printf("\nMenu:\n");
        printf("1. Insert values\n");
        printf("2. Delete a value\n");
        printf("3. Display the tree\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: // Insert values
                printf("How many values to insert: ");
                scanf("%d", &num_values);
                if (num_values + value_count > MAX_SIZE) {
                    printf("Cannot insert more than %d values.\n", MAX_SIZE);
                    break;
                }
                for (int i = 0; i < num_values; i++) {
                    printf("Enter value %d: ", i + 1);
                    scanf("%d", &value);
                    values[value_count++] = value; // Store in array
                }
                build_tree(); // Build the tree after all insertions
                break;
            case 2: // Delete a value
                printf("Enter the index of the value to delete (0 to %d): ", value_count - 1);
                scanf("%d", &index_to_delete);
                delete_value(index_to_delete);
                break;
            case 3: // Display the tree
                print_tree();
                break;
            case 4: // Exit
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);

    return 0;
}
