#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
	int new_val = new->weight;
	pri_q_node *current = root, *new_fw;
	if(!root) return;
	
	while(new_val < current->weight && current->forward != NULL) current = current->forward;
	if(new->forward != NULL)
	{
		new->forward = current->forward;
		new_fw = (pri_q_node *)new->forward;
		new_fw->back = new;
	}
	current->forward = new;
	new->back = current;
}

void print_tree(btree_node *root)
{
	
}

int partition(int *a, int *b, int l, int r);
void quicksort(int *a, int *b, int l, int r)
{
	int j;
	if(l < r)
	{
		j = partition(a, b, l, r);
		quicksort(a, b, l, j-1);
		quicksort(a, b, j+1, r);
	}
}

int partition(int *a, int *b, int l, int r)
{
	int pivot, i, j, t, t2;
	pivot = a[l];
	i = l; j = r + 1;
	while(1)
	{
		do ++i; while(a[i] <= pivot && i <= r);
		do --j; while(a[j] > pivot);
		if( i >= j ) break;
		t = a[i]; a[i] = a[j]; a[j] = t;
		t2 = b[i]; b[i] = b[j]; b[j] = t2;
	}
	t = a[l]; a[l] = a[j]; a[j] = t;
	t2 = b[l]; b[l] = b[j]; b[j] = t2;
	return j;
}

int encode(char *alphabet, int * weights, int *codewords)
{
	
}
int main(int argc, char **argv)
{
	int chars[128], *weights, i, nxt_char = 0, alphabet_size = 0, k = 0, *alphabet;
	FILE *fp;
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
	quicksort(weights, alphabet, 0, alphabet_size);
	for(i = 0; i <= alphabet_size; i++) printf("%c: %d\n", alphabet[i], weights[i]);

	

}


