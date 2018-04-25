/*
	+ Построить двоичное дерево поиска, в вершинах которого находятся слова из текстового файла.
	+ Вывести его на экран в виде дерева.
	+ Определить количество вершин дерева, содержащих слова, начинающиеся на указанную букву.
	- Выделить эти вершины цветом.
    + Сравнить время поиска начинающихся на указанную букву слов в дереве и в файле.
    
    + Ввод слова.
    + Удаление слова.
    + Поиск слова.
    Обход дерева.
*/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

#include <graphviz/gvc.h>

#define METHODS_H
#define OK 0
#define ERR_NO_FILE -1
#define ERR_FILE_EMPTY -1
#define ERR_DELETE -3
# define ERR_ROOT -4

typedef struct node_s
{
	int flag;
    int reps;
    char word[50];
    struct node_s *left, *right, *par;
}node;

typedef struct {
    node *root;
} tree;


unsigned long long tick(void) {
    unsigned long long d;
    
    __asm__ __volatile__ ("rdtsc" : "=A" (d) );
    
    return d;
}

void init_tree(tree *tr) {
    tr->root = NULL;
}

void init_node(node *temp) {
    temp->left = NULL;
    temp->right = NULL;
    temp->par = NULL;
}

void init_root(tree *tr) {
    node *temp = (node*)malloc(sizeof(node));
    init_node(temp);
    tr->root = temp;
}

void add(tree *tr, char *word) {
    node *temp1 = NULL;
    node *temp2 = tr->root;
    while (temp2 != NULL)
    {
        temp1 = temp2;
        if (strcmp(word, temp2->word) > 0)
            temp2 = temp2->left;
        else if(strcmp(word, temp2->word) < 0)
            temp2 = temp2->right;
        else
        {
            temp2->reps++;
            return;
        }
    }
    if (temp1 == NULL)
    {
        init_root(tr);
        strcpy(tr->root->word, word);
        tr->root->reps = 1;
    }
    else if (strcmp(temp1->word, word) >= 0)
    {
        node *temp = (node*)malloc(sizeof(node));
        init_node(temp);
        strcpy(temp->word, word);
        temp->reps = 1;
        temp1->right = temp;
        temp->par = temp1;
    }
    else
    {
        node *temp = (node*)malloc(sizeof(node));
        init_node(temp);
        strcpy(temp->word, word);
        temp->reps = 1;
        temp1->left = temp;
        temp->par = temp1;
    }
}

void print_tree(struct node_s * nod, int level) {
    if(nod)
    {
        print_tree(nod->left, level + 1);
        for(int i = 0;i < level;i++)
            printf("%s", "     ");
        printf("%s\n", nod->word);
        print_tree(nod->right,level + 1);
    }
}

node* search(tree *tr, char *word, int *n) {
    node *temp = tr->root;*n = 0;
    while (temp != NULL)
    {
        int cmp = strcmp(word, temp->word);
        //printf("\n%s", temp->word);
        if (cmp == 0)
            break;
        if (cmp < 0)
            temp = temp->left;
        else
            temp = temp->right;

        (*n)++;
        //printf("%d\n", *n);
    }
    return temp;
}

void search_word_tree(struct node_s * nod, char *word, int *n) {
    if (nod) {
        search_word_tree(nod->left, word, n);
        if (nod->word[0] == word[0])
        {
        	//*n += nod->reps;
        	*n += 1;
        	nod->flag = 1;
        }
        search_word_tree(nod->right, word, n);
    }
    else return;
}

int is_left(node *nod) {
    int res = 0;
    if (nod->par)
        if (nod->par->left)
            if (!strcmp(nod->word, nod->par->left->word))
                res = 1;
    return res;
}

int delete(tree *tr, char *word) {
    node *temp;
    int n = 0;
    temp = search(tr, word, &n);
    if (temp == NULL)
        return ERR_DELETE;
    //if (n == 2)
    //	return ERR_ROOT;
    //printf("\npar   %p   %p\n", temp->par, temp);
    if (temp != tr->root)
    {
        if ((temp->left == NULL)*(temp->right == NULL)) ///////// ******* &&&&&&&& ///////
        {
            if (is_left(temp))
                temp->par->left = NULL;
            else
                temp->par->right = NULL;
        }
        else if ((temp->left == NULL)*(temp->right != NULL))
        {
            if (is_left(temp))
            {
                temp->par->left = temp->right;
                temp->right->par = temp->par;
            }
            else
            {
                temp->par->right = temp->right;
                temp->right->par = temp->par;
            }
        }
        else if ((temp->right == NULL)*(temp->left != NULL))
        {
            if (is_left(temp))
            {
                temp->par->left = temp->left;
                temp->left->par = temp->par;
            }
            else
            {
                temp->par->right = temp->left;
                temp->left->par = temp->par;
            }
        }
        else
        {
            node *temp2 = temp->right;
            while (temp2->left != NULL)
            {
                temp2 = temp2->left;
            }
            if (is_left(temp))
            {
                temp->par->left = temp2;
                temp2->par = temp->par;
            }
            else
            {
                temp->par->right = temp2;
                temp2->par = temp->par;
            }

            temp2->left = temp->left;
            temp->left->par = temp2;
        }
    }
    else
    {
        if ((tr->root->left == NULL)*(tr->root->right == NULL))
            tr->root = NULL;
        else if ((tr->root->left == NULL)*(tr->root->right != NULL))
        {
            tr->root->right->par = NULL;
            tr->root = tr->root->right;
        }
        else if ((tr->root->left != NULL)*(tr->root->right == NULL))
        {
            tr->root->left->par = NULL;
            tr->root = tr->root->left;
        }
        else
        {
            node *temp2 = tr->root->right;
            while (temp2->left != NULL)
            {
                temp2 = temp2->left;
            }
            temp2->left = tr->root->left;
            tr->root->left->par = temp2;
            tr->root = tr->root->right;
            tr->root->par = NULL;
        }
    }
    return OK;
}

void preOrder(const struct node_s * nod) {
    if (nod)
    {
        printf("%s   ", nod->word);
        preOrder(nod->left);
        preOrder(nod->right);
    }
    else return;
}

void inOrder(const struct node_s * nod) {
    if (nod) {
        inOrder(nod->left);
        printf("%s   ", nod->word);
        inOrder(nod->right);
    }
    else return;
}

void postOrder(const struct node_s * nod) {
    if (nod) {
        postOrder(nod->left);
        postOrder(nod->right);
        printf("%s   ", nod->word);
    }
    else return;
}

void time(double t1, double t2) {
    int x2 = 100*t1/t2;
    
    printf("\n\nСравнение времени:\n");
    printf("            Время поиска в дереве      Время поиска в файле\n");
    printf("Время, с    %d%% (%7.6f)               100%% (%7.6f)\n", x2, t1/1000000000, t2/1000000000);
}

int menu(void) {
    char s[5];
    int a = 0;
    printf("\n\n1. Добавление слова в дерево\n2. Извлечение слова из дерева");
    printf("\n3. Поиск слова в дереве\n4. Обход дерева");
    printf("\n5. Определение количества вершин в дереве и в файле, содержащих слова, которые начинаются на указанную букву\n");
	printf("\n0. Выход\n");
    do {
        printf("\nВведите номер выбранного пункта: ");
        //fgets(s, 5, stdin);
        //a = atoi(s);
        scanf("%d", &a);
        printf("\n");
    } while (a < 0 || a > 5);
    return a;
}

void bst_print_dot_null(char *key, int nullcount, FILE* stream)
{
    fprintf(stream, "    null%d [shape=point];\n", nullcount);
    fprintf(stream, "    %s -> null%d;\n", key, nullcount);
}

void bst_print_dot_aux(struct node_s* node, FILE* stream)
{
    static int nullcount = 0;

    if (node->right)
    {
        fprintf(stream, "    %s -> %s;\n", node->word, node->right->word);
        bst_print_dot_aux(node->right, stream);
    }
    else
        bst_print_dot_null(node->word, nullcount++, stream);

    if (node->left)
    {
        fprintf(stream, "    %s -> %s;\n", node->word, node->left->word);
        bst_print_dot_aux(node->left, stream);
    }
    else
        bst_print_dot_null(node->word, nullcount++, stream);
        
    if (node->flag == 1)
    {
    	fprintf(stream, "%s [style=filled, fillcolor=lightblue];\n", node->word);
    }
    	
}

void bst_print_dot(struct node_s* tree, FILE* stream)
{
    fprintf(stream, "digraph BST {\n");
    fprintf(stream, "    node [fontname=\"Arial\"];\n");

    if (!tree)
        fprintf(stream, "\n");
    else if (!tree->right && !tree->left)
        fprintf(stream, "    %s;\n", tree->word);
    else
        bst_print_dot_aux(tree, stream);

    fprintf(stream, "}\n");
}

int main()
{
	setbuf(stdout, NULL);
	
	FILE *f;
    tree *tr = (tree*)malloc(sizeof(tree));
    init_tree(tr);
    node *temp;
    int err = OK, check = 0, choice, file_reps = 0, question = 0;
    double tb1, tb2, te1, te2;
    char word[50];
    f = fopen("input.txt", "r");
    if(f == NULL)
    {
        printf("\nФайл \"input.txt\" не найден.\n");
        err = ERR_NO_FILE;
    }
    else
    {
    	while(fscanf(f, "%s", word) == 1)
        {
            add(tr, word);
            strcpy(word, "");
            check = 1;
        }
        fclose(f);
        if(!check)
        {
            printf("\nПустой файл.\n");
            err = ERR_FILE_EMPTY;
        }
        else
        {
        	int choice = -1;
        	print_tree(tr->root, 0);
        	FILE *f1 = fopen("graph.txt", "w");
        	
        	do {
        		choice = menu();
        		if (choice == 1)
        		{
        			printf("\nВведите слово: ");
        			scanf("%s", word);
        			add(tr, word);
        			print_tree(tr->root, 0);
        		}
        		else if (choice == 2)
        		{
        			printf("\nВведите слово: ");
        			scanf("%s", word);
        			err = delete(tr, word);
        			strcpy(word, "");
        			if (err == ERR_DELETE)
        				printf("\nСлово не найдено.\n");
        			//else if (err == ERR_ROOT)
        			//	printf("\nНельзя удалить корень дерева.\n");
        			print_tree(tr->root, 0);
        			
        		}
        		else if (choice == 3)
        		{
        			int n = 0;
        			printf("\nВведите слово: ");
        			scanf("%s", word);
        			temp = search(tr, word, &n);
        			if (temp == NULL)
        				printf("\nСлово в дереве не найдено.\n");
        			else
        			{
        				printf("\nСлово в дереве найдено.");
        				printf("\nВ процессе поиска сравнений было произведено %d раз(а).", n);
        			}
        		}
        		else if (choice == 4)
        		{
        			int a;
        			printf("\n1. Pre-order\n2. In-order\n3. Post-order\n");
        			printf("\nВыберете способ обхода: ");
        			scanf("%d", &a);
        			if (a == 1)
        				preOrder(tr->root);
        			else if (a == 2)
        				inOrder(tr->root);
        			else if (a == 3)
        				postOrder(tr->root);
        			printf("\n");
        		}
        		else if (choice == 5)
        		{
        			char word_file[50] = {0};
        			int count = 0;
        			printf("\nВведите букву: ");
        			scanf("%s", word);
        			tb1 = tick();
        			search_word_tree(tr->root, word, &count);
        			te1 = tick();
        			if (count == 0)
        				printf("\nСлово в дереве не найдено.\n");
        			else
        			{
	        			printf("\nКоличество слов, которые начинаются на букву %s: %d\n", word, count);
	        			printf("\nВремя поиска в дереве: %f\n", (te1-tb1)/1000000000);
	        		}
	        		//////////////////////////////////////
	        		int count1 = 0;
	        		f = fopen("input.txt", "r");
	        		tb2 = tick();
	        		while (fscanf(f, "%s", word_file) == 1)
	        		{
	        			if (word_file[0] == word[0])
	        				count1++;
	        		}
	        		te2 = tick();
	        		fclose(f);
	        		if (count1 == 0)
	        			printf("\nСлово в файле не найдено.\n");
	        		else
	        		{
	        			printf("\nКоличество слов, которые начинаются на букву %s: %d\n", word, count1);
	        			printf("\nВремя поиска в файле: %f\n", (te2-tb2)/1000000000);
	        		}
	        		if (count && count1)
	        			time((te1-tb1), (te2-tb2));
        		}
        	} while (choice != 0);
        bst_print_dot(tr->root, f1);
        fclose(f1);
        }
    }
    free(tr);
}
