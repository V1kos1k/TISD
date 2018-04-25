
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <math.h>


#define INFINIT 40075

typedef struct matrix_t
{
    unsigned int n;         // The number of columns
    float **data;
} matr;


#define panic(MESSAGE)          \
do                          \
{                           \
    fprintf(stderr, MESSAGE);       \
    putc('\n',stderr);      \
}                           \
while(0)

#define assert_eq(A, B) assert((A) == (B))

#define memory_check( ELEM, RET, MESS)                              \
do                                                          \
{                                                           \
    if (ELEM == NULL)                                          \
    {                                                       \
        panic(MESS);                                        \
        return RET;                                             \
    }                                                       \
} while (0)

int menu_value(void);
unsigned long long int tick(void);

void free_matrix_rows(float **data, int n);
float **allocate_matrix_rows(int n);
matr *create_matr(unsigned int n);
void free_matr(matr *matrix);
matr *read_matr(FILE *f);
void print_matr(FILE *f, const matr *m);
matr *floyd_warshall(const matr *a);
void show_matr_gv(const matr *a);
void show_best_gv(const matr *a, const matr *best);


void free_matrix_rows(float **data, int n)
{
    for (int i = 0; i < n; i++)
       // free ����� �������� NULL
       free(data[i]);

    free(data);
}

float **allocate_matrix_rows(int n)
{
    int m = n;
    float **data = calloc(n, sizeof(float*));
    if (!data)
    {
        //panic("Memory allocation failure in allocate_matrix_rows #1.");
        return NULL;
    }

    for (int i = 0; i < n; i++)
    {
        data[i] = calloc(m, sizeof(float));
    if (!data[i])
        {
            free_matrix_rows(data, n);
            //panic("Memory allocation failure in allocate_matrix_rows #2.");
            return NULL;
        }
    }

    return data;
}

matr *create_matr(unsigned int n)
{
    matr *res = malloc(sizeof(matr));
    if (!res)
    {
        //panic("Memory allocation failure in create_matr #1.");
        return NULL;
    }
    res->data = allocate_matrix_rows(n);
    if (!res->data)
    {
        //panic("Memory allocation failure in create_matr #2.");
        free(res);
        return NULL;
    }
    res->n = n;
    return res;
}

matr *read_matr(FILE *f)
{
    rewind(f);
    unsigned int n;
    char str;
    if (fscanf(f, "%c", &str) != 1)
    {
    	printf("\nПустой файл.\n");
    	return NULL;
    }
    rewind(f);
    if (fscanf(f, "%u", &n) != 1)
    {
        panic("\nНекорректный размер матрицы.\n");
        return NULL;
    }
    matr *res = create_matr(n);
    float tmp = 1;
    int err = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (fscanf(f, "%f", &tmp) != 1)
            {
                panic("\nНекорректные данные.\n");
                return NULL;
            }
            res->data[i][j] = tmp;
        }
    }
    return res;
}

void print_matr(FILE *f, const matr *m)
{
    if (m != NULL)
    {
        for (int i = 0; i < m->n; i++)
        {
            for (int j = 0; j < m->n; j++)
            {
                fprintf(f, "%5.2f ", m->data[i][j]);
            }
            putc('\n', f);
        }
    }
    else
        fprintf(f, "Пустой файл\n");
}

void free_matr(matr *matrix)
{
    if (matrix != NULL)
    {
        free_matrix_rows(matrix->data, matrix->n);
        free(matrix);
    }
}

matr *copy_matr(const matr *A)
{
    if (A != NULL)
    {
        matr *C = create_matr(A->n);
        memory_check(C, NULL, "\nОшибка памяти\n");
        memory_check(C->data, NULL, "\nОшибка памяти\n");

        C->n = A->n;
        C->n = A->n;
        for (int i = 0; i < A->n; ++i)
            for (int j = 0; j < A->n; ++j)
                C->data[i][j] = A->data[i][j];
        return C;
    }
    else
    {
        return NULL;
    }
}

matr *floyd_warshall(const matr *A)
{
    matr *res = copy_matr(A);
    memory_check(res, NULL, "");
    for (int k = 0; k < A->n; k++)
    {
        for (int i = 0; i < A->n; i++)
        {
            for (int j = 0; j < A->n; j++)
            if (i != j)
            {
                //printf("\n%f   [%d, %d]\n", res->data[i][j], i, j);
                //printf("\n%f   [%d, %d]\n", res->data[i][k]+res->data[k][j], i, j);
                res->data[i][j] = fmin(res->data[i][j], res->data[i][k]+res->data[k][j]);
                //printf("\n%f   [%d, %d]\n\n", res->data[i][j], i, j);
            }
            else
            {
                res->data[i][j] = 0.0;
                //printf("\n %d   %d\n", i, j);
            }
        }
    }
    return res;
}

void show_matr_gv(const matr *a)
{
    if (a == NULL)
        return;
    FILE *f = fopen("graph.txt", "w");
    //memory_check(f, , "File open failure!");
    fprintf(f, "digraph My_graph {\n");
    for (int i = 0; i < a->n; i++)
        for (int j = 0; j < a->n; j++)
        {
            if (a->data[i][j] < INFINIT && a->data[i][j] != 0)
                fprintf(f, "%d -> %d [label=\"%.2f км\"];\n", i+1, j+1, a->data[i][j]);
        }
    fprintf(f, "}\n");
    fclose(f);
    //system("D:/temp/graph/output.gv");
}

void show_best_gv(const matr *a, const matr *not_best)
{
    if (a == NULL)
        return;
    FILE *f = fopen("graph1.txt", "w");
    //memory_check(f, , "File open failure!");
    fprintf(f, "digraph My_graph {\n");
    for (int i = 0; i < a->n; i++)
        for (int j = 0; j < a->n; j++)
        {
            if (a->data[i][j] < INFINIT && a->data[i][j] != 0 && a->data[i][j] >= not_best->data[i][j])
                fprintf(f, "%d -> %d [label=\"%.2f км\"];\n", i+1, j+1, a->data[i][j]);
        }
    fprintf(f, "}\n");
    fclose(f);
    //system("D:/temp/graph/output.gv");
}

int menu_value(void) // Zotkin made
{
    int value = 0;
    int flag = 0;
    do
    {
        flag = 0;
        printf("Выбор: ");
        flag = scanf("%d", &value);
        // read all chars
        char ch;
        while( scanf("%c", &ch) && ch != '\n' )
            flag = 0;
    }
    while( flag == 0 );
    return value;
}

unsigned long long int tick(void)
{
    unsigned long long int time = 0;
    __asm__ __volatile__ ("rdtsc" : "=A" (time));
    return time;
}

int test(const matr *a) {
	int count = 0;
	for (int i = 0; i < a->n; i++)
		for (int j = 0; j < a->n; j++)
			if ((a->data[i][j] != 0) && (i != j))
				count++;
	return count;
}

int main(void)
{
    FILE *fin = fopen("input.txt", "r");
    //memory_check(fin, EXIT_FAILURE, "Это фиаско");
    unsigned long long t1;

    int menu = 0;
	int flag = 1;

	matr *graph = read_matr(fin);
	memory_check(graph, EXIT_FAILURE, "");
	if (graph->n < 2)
	{
		printf("\nНекорректный размер матрицы.\n");
		return -1;
	}
	int err = test(graph);
	if (err != (graph->n*graph->n - graph->n))
	{
		printf("\nНекорректный ввод. Должна быть система двухсторонних дорог.\n");
		return -1;
	}
    matr *best = NULL;
	do
	{
		puts("\nМЕНЮ\n==============================");
		puts(" 1. Вывести граф");
		puts(" 2. Найти кратчайший путь");
		//puts(" 3. Вывести матрицу графа");
		//puts(" 4. Вывести матрицу кратчайших путей");
		puts("\n 0. Выход");
		menu = menu_value();
		switch(menu)
		{
			case 1:
                show_matr_gv(graph);
                printf("\nМатрица графа:\n==============================\n");
                print_matr(stdout, graph);
                break;
            case 2:
                free_matr(best);
                t1 = tick();
                best = floyd_warshall(graph);
                printf("Время поиска кратчаешего пути: %llu\n", tick()-t1);
                printf("\nМатрица кратчайших путей графа:\n==============================\n");
                print_matr(stdout, best);
                show_best_gv(best, graph);
                break;
            /*case 3:
                print_matr(stdout, graph);
                break;
            case 4:
                print_matr(stdout, best);
                show_best_gv(best, graph);
                break;*/
            case 0:
                flag = 0;
                break;
            default:
                puts("\nНекорректеый ввод.");
                break;
		}
	} while (flag);

	free_matr(graph);
	free_matr(best);
    fclose(fin);
	return EXIT_SUCCESS;
}