#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct
{
	char node_val;
	int weight;
	struct pri_q_node *forward;
	struct pri_q_node *back;	
	/* So they can also be btree nodes */
	struct pri_q_node *left;
	struct pri_q_node *right;
} pri_q_node;

pri_q_node* insert_pq(pri_q_node *root, pri_q_node *new)
{
    pri_q_node *current = root, *back;
	int added = 0;
	if(new->weight == 0) return root;
	while(new->weight > current->weight) 
	{
		if(current->forward == NULL)
		{
			current->forward = (struct pri_q_node *) new;
			new->back = (struct pri_q_node *) current;
			added++;
			break;
		}
		else
		{
			current = (pri_q_node *) current->forward;
		}
	}
	if(!added)
	{
		if(current->back != NULL)
		{
			back = (pri_q_node *) current->back;
			new->back = (struct pri_q_node *) back;
			back->forward = (struct pri_q_node *) new;
		}
		else if(current == root)
		{
			root = new;
		}
			new->forward = (struct pri_q_node *) current;
			current->back = (struct pri_q_node *) new;
	}
	return root;
}
pri_q_node* deque_pq(pri_q_node *root)
{
	pri_q_node *new_root = (pri_q_node *) root->forward;
	new_root->back = NULL;
	root = new_root;
	return new_root;
}

void free_tree(pri_q_node *root)
{
	if(root->left != NULL) free_tree((pri_q_node *) root->left);
	if(root->right != NULL) free_tree((pri_q_node *) root->right);
	free(root);
}

void padding ( char *ch, int n )
{
  int i;

  for ( i = 0; i < n; i++  )
	printf("%s", ch);

}

void structure( pri_q_node *root, int level  )
{
  int i;

  if ( root == NULL  ) {
    padding ( "\t\t", level  );
    puts ( "~"  );
  
  }
  else {
    structure ((pri_q_node *) root->right, level + 1  );
    padding ( "\t\t", level  );
    printf ( "%d:%c\n", root->weight, root->node_val  );
    structure ( (pri_q_node *) root->left, level + 1  );
  
  }

}

pri_q_node* encode(pri_q_node *root)
{
    pri_q_node *current = (pri_q_node *) root, *first, *second, *parent;
    while(current->forward != NULL && (first = (pri_q_node *) current->forward)->forward != NULL)
    {
        first = current; 
		current = deque_pq(current);
        second = current;
		current = deque_pq(current);
        parent = (pri_q_node *) malloc(sizeof(pri_q_node));
		parent->left = (struct pri_q_node *) first;
		parent->right = (struct pri_q_node *) second;
		parent->weight = first->weight + second->weight;
        current = insert_pq(current, parent);
    }
        first = current; 
		current = deque_pq(current);
        second = current;
        parent = (pri_q_node *) malloc(sizeof(pri_q_node));
		parent->left = (struct pri_q_node *) first;
		parent->right = (struct pri_q_node *) second;
		parent->weight = first->weight + second->weight;
        current = insert_pq(current, parent);
    return parent;
}


void compress(char *fin, char *fout)
{
	int chars[128], *weights, i, nxt_char = 0, alphabet_size = 0, k = 0, *alphabet, first_char, nodecount;
	FILE *fp;
    pri_q_node *root, *new_node, *current;
	fp = fopen(fin, "r");
	if(fp < 0 ) { printf("Unable to open file: %s\n", fin); exit(-1);}
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
		if(chars[i] > 0) 
		{
			alphabet_size++;
		}
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
        root = insert_pq(root, new_node);
    }
    current = root;
	nodecount = 1;
    while(current->forward != NULL)
    {
        printf("%c: %d\n", current->node_val, current->weight);
        current = (pri_q_node *) current->forward;
		nodecount++;
    }
    printf("%c: %d\n", current->node_val, current->weight);
    root = encode(root);
	structure(root, 0);
    free(alphabet);
    free(weights);
	free_tree(root);
}
int main(int argc, char **argv)
{
	if(argc < 4) { printf("usage: %s [-c|-x] filein fileout\n", argv[0]); return -1; }
	compress(argv[1], argv[2]);
}


