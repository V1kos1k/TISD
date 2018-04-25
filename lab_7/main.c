//#include "hash.h"
//#include "binary.h"
#include <time.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct tree_node   /// Структура двоичного дерева
{
    char key[20];  /// Ключ
    struct tree_node *left;  /// Указатель на левого потомка
    struct tree_node *right;  /// Указатель на правого потомка
};

struct tree_balanced_node
{
        char key[20];
        unsigned char height;  /// Высота
        struct tree_balanced_node* left;
        struct tree_balanced_node* right;
};

typedef struct ht_row_open
{
    char word[20];  /// Слово
    struct ht_row_open* next;  /// Указатель на следующий элемент

} ht_row_open;

ht_row_open **HT_open;

typedef struct ht_row_close
{
    char word[20];
} ht_row_close;

ht_row_close *HT_close;




struct tree_node* create_node(const char *name)
{
    struct tree_node *node;

    node = malloc(sizeof(struct tree_node));

    if (node)
    {
        strcpy(node->key, name);
        node->left = NULL;
        node->right = NULL;
    }

    return node;
}

struct tree_node* insert(struct tree_node *tree, struct tree_node *node)
{
    int cmp;

    if (tree == NULL)
        return node;

    cmp = strcmp(node->key, tree->key);

    if (cmp < 0)
        tree->left = insert(tree->left, node);
    if (cmp > 0)
        tree->right = insert(tree->right, node);

    return tree;
}

struct tree_node* search_node(struct tree_node *tree, const char *x, int *count)
{
    while (tree != NULL)
    {
        (*count)++;
        if (strcmp(x, tree->key) == 0)
        {
            return tree;
        }
        else if (strcmp(x, tree->key) < 0)
            tree = tree->left;
        else
            tree = tree->right;
    }

    return NULL;
}



unsigned long long tick(void) {
    unsigned long long d;
    
    __asm__ __volatile__ ("rdtsc" : "=A" (d) );
    
    return d;
}



void apply_pre(struct tree_node *tree, void (*f)(struct tree_node*, void*), void *arg)
{
    if (tree == NULL)
        return;
    f(tree, arg);
    apply_pre(tree->left, f, arg);
    apply_pre(tree->right, f, arg);
}

void apply_post(struct tree_node *tree, void (*f)(struct tree_node*, void*), void *arg)
{
    if (tree == NULL)
        return;

    apply_post(tree->left, f, arg);
    apply_post(tree->right, f, arg);
    f(tree, arg);
}


void free_node(struct tree_node *node, void *param)
{
    free(node);
}

void to_dot(struct tree_node *tree, void *param)
{
    FILE *f = param;

    if (tree->left)
        fprintf(f, "%s -> %s;\n", tree->key, tree->left->key);

    if (tree->right)
        fprintf(f, "%s -> %s;\n", tree->key, tree->right->key);

    if (!(tree->left || tree->right) && tree)
        fprintf(f, "%s;\n", tree->key);
}

void export_to_dot(FILE *f, const char *tree_name, struct tree_node *tree)
{
    fprintf(f, "digraph %s {\n", tree_name);
    apply_pre(tree, to_dot, f);
    fprintf(f, "}\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

struct tree_balanced_node* create_node_b(const char *name)
{
    struct tree_balanced_node *node;
    node = malloc(sizeof(struct tree_balanced_node));
    if (node)
    {
        strcpy(node->key, name);
        node->left = NULL;
        node->right = NULL;
        node->height = 1;
    }
    return node;
}


unsigned char height(struct tree_balanced_node* p)
{
    return p ? p->height : 0;
}

int bfactor(struct tree_balanced_node* p)
{
    return height(p->right) - height(p->left);
}

void fixheight(struct tree_balanced_node* p)
{
    unsigned char hl = height(p->left);
    unsigned char hr = height(p->right);
    p->height = (hl>hr ? hl : hr) + 1;
}

struct tree_balanced_node* rotateright(struct tree_balanced_node* p) {
    struct tree_balanced_node* q = p->left;
    p->left = q->right;
    q->right = p;
    fixheight(p);
    fixheight(q);
    return q;
}

struct tree_balanced_node* rotateleft(struct tree_balanced_node* q) // ëåâûé ïîâîðîò âîêðóã q
{
    struct tree_balanced_node* p = q->right;
    q->right = p->left;
    p->left = q;
    fixheight(q);
    fixheight(p);
    return p;
}

struct tree_balanced_node* balance(struct tree_balanced_node* p) // áàëàíñèðîâêà óçëà p
{
    fixheight(p);
    if (bfactor(p) == 2)
    {
        if (bfactor(p->right) < 0)
            p->right = rotateright(p->right);
        return rotateleft(p);
    }
    if (bfactor(p) == -2)
    {
        if (bfactor(p->left) > 0)
            p->left = rotateleft(p->left);
        return rotateright(p);
    }
    return p; // áàëàíñèðîâêà íå íóæíà
}

struct tree_balanced_node* insert_b(struct tree_balanced_node* p, struct tree_balanced_node* node) // âñòàâêà êëþ÷à k â äåðåâî ñ êîðíåì p
{
    int cmp;

    if (p == NULL)
        return node;

    cmp = strcmp(node->key, p->key);

    if (cmp < 0)
        p->left = insert_b(p->left, node);
    if (cmp > 0)
        p->right = insert_b(p->right, node);
    return balance(p);
}

struct tree_balanced_node* search_node_b(struct tree_balanced_node *tree, const char *x, int *count)
{
    while (tree != NULL)
    {
        (*count)++;
        if (strcmp(x, tree->key) == 0)
        {
            return tree;
        }
        else if (strcmp(x, tree->key) < 0)
            tree = tree->left;
        else
            tree = tree->right;
    }

    return NULL;
}

void apply_pre_b(struct tree_balanced_node *p, void (*f)(struct tree_balanced_node*, void*), void *arg)
{
    if (p == NULL)
        return;
    f(p, arg);
    apply_pre_b(p->left, f, arg);
    apply_pre_b(p->right, f, arg);
}

void free_node_b(struct tree_balanced_node *node, void *param)
{
    free(node);
}

void to_dot_b(struct tree_balanced_node *p, void *param)
{
    FILE *f = param;

    if (p->left)
        fprintf(f, "%s -> %s;\n", p->key, p->left->key);

    if (p->right)
        fprintf(f, "%s -> %s;\n", p->key, p->right->key);

    if (!(p->left || p->right) && p)
        fprintf(f, "%s;\n", p->key);
}

void export_to_dot_b(FILE *f, const char *tree_name, struct tree_balanced_node *tree)
{
    fprintf(f, "digraph %s {\n", tree_name);
    apply_pre_b(tree, to_dot_b, f);
    fprintf(f, "}\n");
}

void apply_post_b(struct tree_balanced_node *tree, void (*f)(struct tree_balanced_node*, void*), void *arg)
{
    if (tree == NULL)
        return;

    apply_post_b(tree->left, f, arg);
    apply_post_b(tree->right, f, arg);
    f(tree, arg);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int hash_func(char *str, int M)
{
    int sum = 0;
    for (int i = 0; i < strlen(str); i++)
        sum += str[i];
    return (sum % M);
}

ht_row_open *insertWord_open(char *str, int M)
{
    ht_row_open *p, *p0;
    int ind;

    ind = hash_func(str, M);
    if ((p = malloc(sizeof(ht_row_open))) == 0)
    {
        exit(1);
    }
    p0 = HT_open[ind];
    p->next = p0;

    strcpy(p->word, str);
    HT_open[ind] = p;
    return p;
}

ht_row_open *findWord_open(char *str, int M)
{
    int count = 0;
    double tb = 0, te = 0;
    int ind = hash_func(str, M);
    ht_row_open *head = HT_open[ind];

    tb = tick();
    for ( ; head; head = head->next)
    {
        count++;
        if (strcmp(head->word, str) == 0)
        {
            te = tick();
            printf("Слово %s найдено.\n", str);
            printf("Количество сравнений: %d\n", count);
            printf("\nВремя поиска: %f\n", (te-tb));
            return head;
        }
    }
    te = tick();
    printf("Слово не найдено.\n\n");
    printf("\nВремя поиска: %f\n", (te-tb));
    return NULL;
}

ht_row_open *FW(char *str, int M)
{
    int count = 0;
    int ind = hash_func(str, M);
    ht_row_open *head = HT_open[ind];

    for ( ; head; head = head->next)
    {
        count++;
        if (strcmp(head->word, str) == 0)
        {
            return head;
        }
    }
    return NULL;
}

void print_row(ht_row_open *head)
{
    for ( ; head; head = head->next)
    {
        printf("%s  ", head->word);
    }

   printf("\n---------------------------------------------------------------------------\n");
}

int insertWord_close(char *str, int M_close, int M)
{
    int ind = hash_func(str, M);

    while (HT_close[ind].word[0] != '\0')
    {
        ind++;
        if (ind == M_close)
        {
            ht_row_close *tmp = HT_close;
            HT_close = realloc(tmp, M_close*2*sizeof(ht_row_close));
            for (int i = M_close; i < M_close*2; i++)
                 HT_close[i].word[0] = '\0';
            M_close *= 2;
        }
    }
    strcpy(HT_close[ind].word, str);

    return M_close;
}

int findWord_close(char *str, int M_close, int M)
{
    int count = 0;
    double tb = 0, te = 0;
    int ind = hash_func(str, M);
    tb = tick();
    for (int i = ind; i < M_close; i++)
    {
        count ++;
        if (strcmp(HT_close[i].word, str) == 0)
        {
            te = tick();
            printf("Слово %s найдено.\n", str);
            printf("Количество сравнений: %d\n", count);
            printf("\nВремя поиска: %f\n", (te-tb));
            return count;
        }
    }
    te = tick();
    printf("Слово не найдено.\n\n");
    printf("\nВремя поиска: %f\n", (te-tb));
    return -1;
}

int FC(char *str, int M_close, int M)
{
    int count = 0;
    int ind = hash_func(str, M);
    for (int i = ind; i < M_close; i++)
    {
        count ++;
        if (strcmp(HT_close[i].word, str) == 0)
        {
            return count;
        }
    }
    return -1;
}






int main()
{
    setbuf(stdout, NULL);
    //system ("chcp 1251");
    int M = 31;
    int M_close = M;
    char str[30];
    char filename[20];
    FILE *f;
    if ((HT_open = malloc(M * sizeof(ht_row_open *))) == 0) exit(1);
    if ((HT_close = malloc(M_close * sizeof(ht_row_close))) == 0) exit(1);

    for (int i = 0; i < M; i++)
        HT_open[i] = NULL;

    for (int i = 0; i < M_close; i++)
        HT_close[i].word[0] = '\0';

    struct tree_node *node, *root = NULL;
    struct tree_balanced_node *bal_node, *bal_root = NULL;
    int choose = -1;
    int i;
    double t1, t2;
    double te1, te2;

    f = fopen("input.txt", "r");
    if (f == NULL)
    {
        printf("Невозможно открыть файл!");
        return -1;
    }
    while (!feof(f))
    {
        fscanf(f, "%s", str);
        insertWord_open(str, M);
        M_close = insertWord_close(str, M_close, M);
        node = create_node(str);
        root = insert(root, node);
        bal_node = create_node_b(str);
        bal_root = insert_b(bal_root, bal_node);
    }
    fclose(f);

    while (choose != 0)
    {
        printf("\n\n1. Вывести двоичное дерево.\n"
               "2. Вывести сбалансированное дерево.\n"
               "3. Вывести хеш-таблицу (открытая).\n"
               "4. Вывести хеш-таблицу (закрытая).\n"
               "5. Поиск слова.\n"
               "\n0. Выход.\n\n");
        printf("Команда: ");
        scanf ("%d", &choose);
        printf("\n\n");
        if (((choose > 1 && choose < 7) ||choose == 8) && root == NULL)
            printf ("\nСтруктуры не  заполнены!\n\n");
        else
        {
            switch (choose) {
            case 1:
                f = fopen("tree.gv", "w");
                export_to_dot(f, "tree", root);
                fclose(f);
                break;
            case 2:
                f = fopen("treebalance.gv", "w");
                export_to_dot_b(f, "treebalance", bal_root);
                fclose(f);
                break;
            case 3:
                printf("\n---------------------------------------------------------------------------\n");
                for (int i = 0; i < M; i++)
                {
                    printf("%3d | ", i+1);
                    print_row(HT_open[i]);
                }
                printf("\n");
                break;
            case 4:
                for (i = M_close-1; i>= 0; i--)
                {
                    if (HT_close[i].word[0] != '\0')
                        break;
                }

                printf("\n---------------------------\n");
                for (int j = 0; j <= i; j++)
                {
                    printf("%3d | ", j + 1);
                    printf("%s", HT_close[j].word);
                    printf("\n---------------------------\n");
                }
                printf("\n");
                break;
            case 5:
                printf("\nВведите слово для поиска: ");
                scanf("%s", str);
                printf("\n");
                int count = 0;

                t1 = tick();
                node = search_node(root, str, &count);
                te1 = tick();
                printf("\n\nДВОИЧНОЕ ДЕРЕВО\n");
                printf("==========================\n");

                if (node)
                {
                    printf("Слово %s найдено.\n", str);
                    printf("Количество сравнений: %d\n\n", count);
                }
                else printf("Слово не найдено.\n\n");
                printf("Время поиска: %f\n", (te1-t1));


                count = 0;
                t2 = tick();
                bal_node = search_node_b(bal_root, str, &count);
                te2 = tick();
                printf("\n\nСБАЛАНСИРОВАННОЕ ДЕРЕВО\n");
                printf("==========================\n");
                if (bal_node)
                {
                    printf("Слово %s найдено.\n", str);
                    printf("Количество сравнений: %d\n\n", count);
                }
                else printf("Слово не найдено!\n\n");
                printf("Время поиска: %f\n", (te2-t2));


                printf("\n\nХЕШ-ТАБЛИЦА (открытая).\n");
                printf("==========================\n");
                findWord_open(str, M);

                printf("\n\nХЕШ-ТАБЛИЦА (закрытая).\n");
                printf("==========================\n");
                findWord_close(str, M_close, M);

                break;
            case 0:
                return 0;
            default:
                printf("Ошибка ввода\n");
                break;
        }
        }
        //system("pause");
    }


    return 0;
}
