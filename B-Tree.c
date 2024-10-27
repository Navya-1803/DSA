#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MIN_DEGREE 2 

struct BTreeNode {
    int *keys;             
    int t;                  
    struct BTreeNode **children; 
    int n;                 
    bool isLeaf;            
};

// Function prototypes
struct BTreeNode* createNode(int t, bool isLeaf);
void traverse(struct BTreeNode* root);
void splitChild(struct BTreeNode* x, int i, struct BTreeNode* y);
void insertNonFull(struct BTreeNode* x, int key);
void insert(struct BTreeNode** root, int key);
void removeKey(struct BTreeNode* root, int key);
void removeFromNode(struct BTreeNode* root, int idx);
int findKey(struct BTreeNode* root, int key);
int getPred(struct BTreeNode* root, int idx);
int getSucc(struct BTreeNode* root, int idx);
void menu(struct BTreeNode** root);
void initializeTree(struct BTreeNode** root);
void displayTree(struct BTreeNode* root, int level);

struct BTreeNode* createNode(int t, bool isLeaf) {
    struct BTreeNode* newNode = (struct BTreeNode*)malloc(sizeof(struct BTreeNode));
    newNode->t = t;
    newNode->isLeaf = isLeaf;
    newNode->keys = (int*)malloc((2 * t - 1) * sizeof(int));
    newNode->children = (struct BTreeNode**)malloc(2 * t * sizeof(struct BTreeNode*));
    newNode->n = 0;
    return newNode;
}

void traverse(struct BTreeNode* root) {
    int i;
    for (i = 0; i < root->n; i++) {
        if (!root->isLeaf)
            traverse(root->children[i]);
        printf("%d ", root->keys[i]);
    }
    if (!root->isLeaf)
        traverse(root->children[i]);
}

void splitChild(struct BTreeNode* x, int i, struct BTreeNode* y) {
    int t = y->t;
    struct BTreeNode* z = createNode(t, y->isLeaf);
    z->n = t - 1;

    for (int j = 0; j < t - 1; j++)
        z->keys[j] = y->keys[j + t];

    if (!y->isLeaf) {
        for (int j = 0; j < t; j++)
            z->children[j] = y->children[j + t];
    }
    y->n = t - 1;

    for (int j = x->n; j >= i + 1; j--)
        x->children[j + 1] = x->children[j];
    x->children[i + 1] = z;

    for (int j = x->n - 1; j >= i; j--)
        x->keys[j + 1] = x->keys[j];
    x->keys[i] = y->keys[t - 1];
    x->n++;
}

void insertNonFull(struct BTreeNode* x, int key) {
    int i = x->n - 1;
    if (x->isLeaf) {
        while (i >= 0 && x->keys[i] > key) {
            x->keys[i + 1] = x->keys[i];
            i--;
        }
        x->keys[i + 1] = key;
        x->n++;
    } else {
        while (i >= 0 && x->keys[i] > key)
            i--;
        if (x->children[i + 1]->n == 2 * x->t - 1) {
            splitChild(x, i + 1, x->children[i + 1]);
            if (x->keys[i + 1] < key)
                i++;
        }
        insertNonFull(x->children[i + 1], key);
    }
}

void insert(struct BTreeNode** root, int key) {
    if ((*root)->n == 2 * (*root)->t - 1) {
        struct BTreeNode* s = createNode((*root)->t, false);
        s->children[0] = *root;
        splitChild(s, 0, *root);
        int i = 0;
        if (s->keys[0] < key)
            i++;
        insertNonFull(s->children[i], key);
        *root = s;
    } else {
        insertNonFull(*root, key);
    }
}

void removeKey(struct BTreeNode* root, int key) {
    int idx = findKey(root, key);
    if (idx < root->n && root->keys[idx] == key) {
        if (root->isLeaf)
            removeFromNode(root, idx);
        else if (root->children[idx]->n >= root->t) {
            int pred = getPred(root, idx);
            root->keys[idx] = pred;
            removeKey(root->children[idx], pred);
        } else if (root->children[idx + 1]->n >= root->t) {
            int succ = getSucc(root, idx);
            root->keys[idx] = succ;
            removeKey(root->children[idx + 1], succ);
        }
    } else {
        if (root->isLeaf)
            return;
        bool flag = ((idx == root->n) ? true : false);
        if (root->children[idx]->n < root->t)
            return;
        if (flag && idx > root->n)
            removeKey(root->children[idx - 1], key);
        else
            removeKey(root->children[idx], key);
    }
}

void removeFromNode(struct BTreeNode* root, int idx) {
    for (int i = idx + 1; i < root->n; ++i) {
        root->keys[i - 1] = root->keys[i];
    }
    if (!root->isLeaf) {
        for (int i = idx + 1; i <= root->n; ++i) {
            root->children[i - 1] = root->children[i];
        }
    }
    root->n--;
}

int findKey(struct BTreeNode* root, int key) {
    int idx = 0;
    while (idx < root->n && root->keys[idx] < key)
        ++idx;
    return idx;
}

int getPred(struct BTreeNode* root, int idx) {
    struct BTreeNode* current = root->children[idx];
    while (!current->isLeaf)
        current = current->children[current->n];
    return current->keys[current->n - 1];
}

int getSucc(struct BTreeNode* root, int idx) {
    struct BTreeNode* current = root->children[idx + 1];
    while (!current->isLeaf)
        current = current->children[0];
    return current->keys[0];
}

// Function to display the tree structure
void displayTree(struct BTreeNode* root, int level) {
    if (root == NULL)
        return;
    
    printf("Level %d: ", level);
    for (int i = 0; i < root->n; i++)
        printf("%d ", root->keys[i]);
    printf("\n");

    if (!root->isLeaf) {
        for (int i = 0; i <= root->n; i++)
            displayTree(root->children[i], level + 1);
    }
}

// Menu function for interaction
void menu(struct BTreeNode** root) {
    int choice, key;
    while (1) {
        printf("\n--- B-Tree Menu ---\n");
        printf("1. Insert\n2. Delete\n3. Display\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the key to insert: ");
                scanf("%d", &key);
                insert(root, key);
                printf("Key %d inserted.\n", key);
                break;
            case 2:
                printf("Enter the key to delete: ");
                scanf("%d", &key);
                removeKey(*root, key);
                printf("Key %d deleted.\n", key);
                break;
            case 3:
                printf("B-Tree structure:\n");
                displayTree(*root, 0);
                break;
            case 4:
                printf("Exiting...\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

void initializeTree(struct BTreeNode** root) {
    int n, key;
    printf("Enter the number of initial values for the B-Tree: ");
    scanf("%d", &n);
    *root = createNode(MIN_DEGREE, true);
    for (int i = 0; i < n; i++) {
        printf("Enter key %d: ", i + 1);
        scanf("%d", &key);
        insert(root, key);
    }
}

int main() {
    struct BTreeNode* root = NULL;
    initializeTree(&root);
    menu(&root);
    return 0;
}
