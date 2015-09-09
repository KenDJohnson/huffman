#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct
{
	char node_val;
	int weight;
	struct btree_node *parent;
	struct btree_node *left;
	struct btree_node *right;
} btree_node;

typedef struct
{
	char node_val;
	int weight;
	struct pri_q_node *forward;
	struct pri_q_node *back;	
} pri_q_node;

void insert_pq(pri_q_node *root, pri_q_node *new)
{
    pri_q_node *current = root, *new_fw;
    printf("adding %c : %d\n", new->node_val, new->weight);
	if(new->weight == 0) return;
	printf("added\n");
	while(new->weight < current->weight && current->forward != NULL) current = (pri_q_node *) current->forward;
	if(current->forward != NULL)
	{
		new->forward = current->forward;
		new_fw = (pri_q_node *)new->forward;
		new_fw->back = (struct pri_q_node *) new;
	}
	current->forward = (struct pri_q_node *) new;
	new->back = (struct pri_q_node *) current;
}

void print_tree(btree_node *root)
{
    printf("printing\n");
    if(root->left != NULL) print_tree((btree_node *) root->left);
    if(root->right != NULL) print_tree((btree_node *) root->right);
    printf("%c : %d\n", root->node_val, root->weight);
}


btree_node* encode(pri_q_node *root)
{
    pri_q_node *current = (pri_q_node *) root, *first, *second, *parent;
    btree_node *bt_first, *bt_second, *bt_parent;
    while(current->forward != NULL && (first = (pri_q_node *) current->forward)->forward != NULL)
    {
        first = current; 
        second = (pri_q_node *) current->forward;
        parent = (pri_q_node *) malloc(sizeof(pri_q_node));
        bt_first = (btree_node *) malloc(sizeof(btree_node));
        bt_second = (btree_node *) malloc(sizeof(btree_node));
        bt_parent = (btree_node *) malloc(sizeof(btree_node));

        bt_first->node_val = first->node_val;
        bt_first->weight = first->weight;
        bt_first->parent = (struct btree_node *) bt_parent;
        bt_first->left = NULL;
        bt_first->right = NULL;

        bt_second->node_val = second->node_val;
        bt_second->weight = second->weight;
        bt_second->parent = (struct btree_node *) bt_parent;
        bt_second->left = NULL;
        bt_second->right = NULL;

        bt_parent->left = (struct btree_node *) bt_first;
        bt_parent->right = (struct btree_node *) bt_second;
        bt_parent->weight = bt_first->weight + bt_second->weight;

        parent->weight = bt_parent->weight;
        parent->node_val = 0;
        parent->forward = NULL;
        parent->back = NULL;
        insert_pq(root, parent);

        current = (pri_q_node *) second->forward;
    }
    return bt_parent;
}
int main(int argc, char **argv)
{
	int chars[128], *weights, i, nxt_char = 0, alphabet_size = 0, k = 0, *alphabet, first_char;
	FILE *fp;
    btree_node *btree_root;
    pri_q_node *root, *new_node, *current;
	fp = fopen("text", "r");
	if(fp < 0 ) return -1;
    for(i = 0; i < 128; i++)
    {
        chars[i] = 0;
    }
	while((nxt_char = fgetc(fp)) != EOF)
	{
		if(nxt_char < 0 || nxt_char > 127) continue;
		chars[nxt_char]++;
	}
	for(i = 0; i < 128; i++)
	{
/*		i > 31 || i < 127 ? printf("%c ", i) : printf("%d ", i);
		printf("%d\n", chars[i]);*/
		if(chars[i] > 0) alphabet_size++;
	}
	alphabet = (int *) malloc(sizeof(int) * alphabet_size);
	weights = (int *) malloc(sizeof(int) * alphabet_size);
	memset(weights, 0, alphabet_size);
	memset(alphabet, 0, alphabet_size);
	for(i = 0; i < 127; i++)
	{
		if(chars[i] > 0)
		{
			alphabet[k] =  i;
			weights[k++] = chars[i];
		}
	}
    root = (pri_q_node *) malloc(sizeof(*root));
    first_char = alphabet[0] ? 0 : 1;
    root->node_val = alphabet[first_char];
    root->weight = weights[first_char];
    root->back = NULL;
    root->forward = NULL;
    
	for(i = first_char+1; i <= alphabet_size; i++)
    {
        new_node = (pri_q_node *) malloc(sizeof(*new_node));
        new_node->node_val = alphabet[i];
        new_node->weight = weights[i];
        new_node->back = NULL;
        new_node->forward = NULL;
        insert_pq(root, new_node);
    }
    current = root;
    while(current->forward != NULL)
    {
        printf("%c: %d\n", current->node_val, current->weight);
        current = (pri_q_node *) current->forward;
    }
    
    printf("here\n");
    btree_root = encode(root);
    printf("here2\n");
    print_tree(btree_root);

    free(alphabet);
    free(weights);
    current = root;
    while(current->forward != NULL)
    {
        new_node = (pri_q_node *)current->forward;
        free(current);
        current = new_node;
    }
}


