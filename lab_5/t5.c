// A - значения ненулевых элементов
// JA - номера столбцов для элементов А
// связный список IA, в элементе Nk которого находится номер компонент в A и JA, с которых начинается описание строки Nk матрицы A.

# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <math.h>
# include <string.h>

struct IA {
	int i;
	int Nk;
	struct IA *next;
};

struct IA *add(struct IA *IA, int i, int len);

unsigned long long tick(void) {
    unsigned long long tmp;
    __asm__ __volatile__ ("rdtsc" : "=A" (tmp) );
    return tmp;
}

struct IA *create(int i, int num) {
	struct IA *tmp = malloc(sizeof(struct IA));
	if (!tmp)
		return NULL;
	tmp->i = i;
	tmp->Nk = num;
	tmp->next = NULL;
	
	return tmp;
}

void create_mtr(int *mtr, int n, int m, int fill, int *count) {
	int value;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
		{
			value = rand() % 100;
			if (value > fill)
				mtr[i*m+j] = 0;
			else
			{
				mtr[i*m+j] = rand() % 10;
				*count += 1;
			}
		}
}

int input_mtr(int *mtr, int n, int m, int *count) {
	printf("\nВведите матрицу размеров %d * %d\n", n, m);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
		{
			if (scanf("%d", &mtr[i*m+j]) != 1)
				return -1;
			if (mtr[i*m+j] != 0)
				*count += 1;
		}
	return 0;
}

void shaping(int *mtr, int n, int m, int *A, int *JA, struct IA *IA, int *lenA) {
    struct IA *tmp, *tmp1, *tmp2;
    int t1;

    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
        {
            if (mtr[i*m+j] != 0)
            {
                t1 = mtr[i*m+j];
                tmp = IA;
                while (tmp != NULL)
                {
                    if (tmp->i != i)
                    {
                        tmp1 = tmp;
                        tmp = tmp->next;
                    }
                    else
                    {
                        if (JA[tmp->Nk] > j)
                        {
                           for (int k=*lenA; k>tmp->Nk; k--)
                           {
                               A[k] = A[k-1];
                               JA[k] = JA[k-1];
                               tmp2 = IA;
                               while (tmp2 != NULL)
                               {
                                   if (tmp2->Nk != k-1 || tmp->Nk == k-1)
                                       tmp2 = tmp2->next;
                                   else
                                   {
                                       tmp2->Nk += 1;
                                       break;
                                   }
                               }
                           }
                           A[tmp->Nk] = t1;
                           JA[tmp->Nk] = j;
                        }
                        else
                        {
                            for (int k=*lenA; k>tmp->Nk+1; k--)
                            {
                                A[k] = A[k-1];
                                JA[k] = JA[k-1];
                                tmp2 = IA;
                                while (tmp2 != NULL)
                                {
                                    if (tmp2->Nk != k-1)
                                        tmp2 = tmp2->next;
                                    else
                                    {
                                        tmp2->Nk += 1;
                                        break;
                                    }
                                }
                            }
                            A[tmp->Nk+1] = t1;
                            JA[tmp->Nk+1] = j;
                        }
                        break;
                    }
                }
                if (tmp == NULL)
                {
                    A[*lenA] = t1;
                    JA[*lenA] = j;
                    tmp1 = add(tmp1, i, *lenA);
                }

                *lenA += 1;

            }
        }
    while (IA->next != NULL)
        IA = IA->next;
    add(IA, n, *lenA);
}

struct IA *add(struct IA *IA, int i, int len) {
	struct IA *tmp;
	tmp = create(i, len);
	if (!tmp)
		return NULL;
	IA->next = tmp;
	return tmp;
}

void sum(int n, int m, int *A1, int *A2, int *A3, int *JA1, int *JA2, int *JA3, struct IA *IA1, struct IA *IA2, 
			struct IA *IA3, int lenA1, int lenA2, int *lenA3) 
{
    int *T = calloc(m, sizeof(int));
    struct IA *tmp1 = IA1->next, *tmp2, *tmp3 = IA3;
    int l3 = 0, al, t;
    while (tmp1->next != NULL)
        {
            tmp2 = IA2->next;
            while (tmp2->next != NULL)
            {
                if (tmp1->i == tmp2->i)
                    break;
                tmp2 = tmp2->next;
            }

            if (tmp2->next == NULL)
            {
                tmp3 = add(tmp3, tmp1->i, l3);
                al = abs(tmp1->next->Nk - tmp1->Nk);
                for (int k=0; k<al; k++)
                {
                    A3[k+l3] = A1[tmp1->Nk+k];
                    JA3[k+l3] = JA1[tmp1->Nk+k];
                }
                l3 += al;
            }
            else
            {
               // if (IA1[tmp1->Nk] < IA2[tmp2->Nk])
                    tmp3 = add(tmp3, tmp1->i, l3);
               // else
                   // tmp3 = add(tmp3, tmp2->j, tmp2->Nk);
                al = abs(tmp1->next->Nk - tmp1->Nk);
                for (int k=0; k<al; k++)
                    T[JA1[tmp1->Nk+k]] += A1[tmp1->Nk+k];
                t = JA1[tmp1->Nk+al-1];
                al = abs(tmp2->next->Nk - tmp2->Nk);
                for (int k=0; k<al; k++)
                    T[JA2[tmp2->Nk+k]] += A2[tmp2->Nk+k];
                if (t < JA2[tmp2->Nk+al-1])
                    t = JA2[tmp2->Nk+al-1];
                for (int k=0; k<=t; k++)
                    if (T[k] != 0)
                    {
                        A3[l3] = T[k];
                        JA3[l3] = k;
                        l3++;
                    }
                memset(T, 0, m*sizeof(T));
            }

            tmp1 = tmp1->next;
        }

        tmp2 = IA2->next;
        while (tmp2->next != NULL)
        {
            tmp1 = IA1->next;
            while (tmp1->next != NULL)
            {
                if (tmp1->i == tmp2->i)
                    break;
                tmp1 = tmp1->next;
            }

            if (tmp1->next == NULL)
            {
                tmp3 = add(tmp3, tmp2->i, l3);
                al = abs(tmp2->next->Nk - tmp2->Nk);
                for (int k=0; k<al; k++)
                {
                    A3[k+l3] = A2[tmp2->Nk+k];
                    JA3[k+l3] = JA2[tmp2->Nk+k];
                }
                l3 += al;
            }

            tmp2 = tmp2->next;
        }

    *lenA3 = l3;
    add(tmp3, n, l3);
    //return 0;
}

void sum_mtr(int *mtr1, int *mtr2, int *mtr3, int n, int m) {
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			mtr3[i*m+j] = mtr1[i*m+j] + mtr2[i*m+j];
}

void print(int *mtr, int n, int m, int *A, int *JA, struct IA *IA, int lenA, int num) {
	struct IA *tmp = IA->next;
	if (n >34 || m > 34)
	{
		printf("\nA: ");
		for (int i = 0; i < lenA; i++)
			printf("%d  ", A[i]);
		
		printf("\n\nJA:  ");
		for (int i = 0; i < lenA; i++)
			printf("%d  ", JA[i]);
		
		printf("\n\ni:  ");
		for (; tmp->next != NULL; tmp = tmp->next)
			printf("%d  ", tmp->i);
		
		printf("\n\nIA:  ");
		for (IA = IA->next; IA->next != NULL; IA = IA->next) // ;
			printf("%d  ", IA->Nk);
		printf("\n");
	}
	else
	{
		if (num == 3)
			printf("\nМатрица сумм: \n");
		else
			printf("\nМатрица %d: \n", num);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
				printf("%d   ", mtr[i*m+j]);
			printf("\n");
		}
		printf("\n");
	}
}

int test(int fill) {
    srand(time(0));
    int n1, m1, n2, m2, lenA1 = 0, lenA2 = 0, lenA3 = 0;
    int *Matr1, *Matr2, *Matr3, *A1, *A2, *A3;
    int *JA1, *JA2, *JA3;
    struct IA *IA1 = NULL, *IA2 = NULL, *IA3 = create(-1, -1);
    int count1 = 0, count2 = 0; // количество ненулевых элементов
    unsigned long long time_beg, time_end;
    unsigned long long time_beg2, time_end2;
    // Сложение методом единичек
    n1 = m1 = 100;
    IA1 = create(-1, -1);
    Matr1 = calloc(n1*m1, sizeof(int));
    create_mtr(Matr1, n1, m1, fill, &count1);
    A1 = malloc(n1*m1*sizeof(int));
    JA1 = malloc(n1*m1*sizeof(int));
    shaping(Matr1, n1, m1,A1,JA1,IA1, &lenA1);
    //print(n1,m1, Matr1, A1, JA1, IA1, lenA1);
    printf("\nВремя работы тестов для сложения матриц 1000 * 1000 с заполнение %d процентов\n", fill);
    n2 = m2 = 1000;
    IA2 = create(-1, -1);
    Matr2 = calloc(n2*m2, sizeof(int));
    create_mtr(Matr2, n2, m2, fill, &count2);
    printf("\n");
    A2 = malloc(n2*m2*sizeof(int));
    JA2 = malloc(n2*m2*sizeof(int));
    shaping(Matr2, n2, m2,A2,JA2,IA2, &lenA2);
    //print(n2,m2, Matr2, A2, JA2, IA2, lenA2);
    //Matr3 = calloc(, sizeof(int));
    // Сложение
    Matr3 = calloc(n2*m2, sizeof(int));
    A3 = malloc(n2*m2*sizeof(int));
    JA3 = malloc(n2*m2*sizeof(int));
    
    printf("\n%lu    %lu	%lu		%lu", sizeof(Matr1), sizeof(A1), sizeof(JA1), sizeof(IA1));
    
    time_beg = tick();
    sum(n1, m1, A1, A2, A3, JA1, JA2, JA3, IA1, IA2, IA3, lenA1, lenA2, &lenA3);
    time_end = tick();
    
    time_beg2 = tick();
    sum_mtr(Matr1, Matr2, Matr3, n1, m1);;
    time_end2 = tick();
    printf("\nВремя работы метода Чанга и Густавсона: %llu", time_end2 - time_beg2);
    printf("\nВремя работы простого метода: %llu\n", time_end - time_beg);
    return 0;
    //print(n1,m1, Matr3, A3, JA3, IA3, lenA1);
}

void free_IA(struct IA *head) {
    struct IA *nxt;

    for (; head; head = nxt)
    {
        nxt = head->next;
        free(head);
    }
}

int main(void)
{
	srand(time(NULL));
	int var;
	int *mtr1, *mtr2, *mtr3; // "матрицы" с нулевыми элементами
	int *A1, *A2, *A3, lenA1 = 0, lenA2 = 0, lenA3 = 0; // значения
	int *JA1, *JA2, *JA3; // номера столбцов
	struct IA *IA1 = NULL, *IA2 = NULL, *IA3 = NULL;
	int count1 = 0, count2 = 0, count3 = 0; // кол-во ненулевых эл-ов
	int n1, m1, n2, m2;
	
	printf("\nСЛОЖЕНИЕ ЦЕЛОЧИСЛЕННЫХ МАТРИЦ\n");
	printf("====================\n");
	printf("\nВвести матрицу вручную? (1 - Да, 0 - Нет)\n");
	int q2 = scanf("%d", &var);
	if (q2 != 1)
	{
		printf("\nНекорректный ввод. Повторите еще раз.\n");
		return -1;
	}
	if (var == 0)
	{
		//int n1, m1, n2, m2; 
		printf("\nВведите размер матрицы А1: ");
		int q1 = scanf("%d %d", &n1, &m1);
		if (q1 != 2)
		{
			printf("\nНекорректный ввод. Попробуйте еще раз.\n");
			return -1;
		}
		if (n1 < 1 || m1 < 1)
		{
			printf("\nНекорректный рамер матрицы.\n");
			return -1;
		}
		printf("\nВведите размер матрицы А2: ");
		int q3 = scanf("%d %d", &n2, &m2);
		if (q3 != 2)
		{
			printf("\nНекорректный ввод. Попробуйте еще раз.\n");
			return -1;
		}
		if (n2 < 1 || m2 < 1)
		{
			printf("\nНекорректный рамер матрицы.\n");
			return -1;
		}
		int fill = 4;
		
		
		//// 1Я МАТРИЦА
		IA1 = create(-1, -1);
		mtr1 = calloc(n1*m1, sizeof(int));
		create_mtr(mtr1, n1, m1, fill, &count1);
		A1 = malloc(count1*sizeof(int));
		JA1 = malloc(count1*sizeof(int));
		shaping(mtr1, n1, m1, A1, JA1, IA1, &lenA1);
		//printf("\n\n%d   %d\n\n", count1, lenA1);
		print(mtr1, n1, m1, A1, JA1, IA1, lenA1, 1);
		
		// 2Я МАТРИЦА
		IA2 = create(-1, -1);
		mtr2 = calloc(n2*m2, sizeof(int));
		create_mtr(mtr2, n2, m2, fill, &count2);
		A2 = malloc(count2*sizeof(int));
		JA2 = malloc(count2*sizeof(int));
		shaping(mtr2, n2, m2, A2, JA2, IA2, &lenA2);
		
		//printf("\n\n%d   %d\n\n", count2, lenA2);
		print(mtr2, n2, m2, A2, JA2, IA2, lenA2, 2);
	}
	else
	{
		printf("\nВведите размер матрицы А1: ");
		int q1 = scanf("%d %d", &n1, &m1);
		if (q1 != 2)
		{
			printf("\nНекорректный ввод. Попробуйте еще раз.\n");
			return -1;
		}
		if (n1 < 1 || m1 < 1)
		{
			printf("\nНекорректный рамер матрицы.\n");
			return -1;
		}
		printf("\nВведите размер матрицы А2: ");
		int q3 = scanf("%d %d", &n2, &m2);
		if (q3 != 2)
		{
			printf("\nНекорректный ввод. Попробуйте еще раз.\n");
			return -1;
		}
		if (n2 < 1 || m2 < 1)
		{
			printf("\nНекорректный рамер матрицы.\n");
			return -1;
		}
		IA1 = create(-1, -1);
		mtr1 = calloc(n1*m1, sizeof(int));
		if (input_mtr(mtr1, n1, m1, &count1) == -1)
		{
			printf("\nНекорректный ввод.\n");
			return -1;
		}
		A1 = malloc(count1*sizeof(int));
		JA1 = malloc(count1*sizeof(int));
		shaping(mtr1, n1, m1, A1, JA1, IA1, &lenA1);
		print(mtr1, n1, m1, A1, JA1, IA1, lenA1, 1);
		
		IA2 = create(-1, -1);
		mtr2 = calloc(n2*m2, sizeof(int));
		input_mtr(mtr2, n2, m2, &count2);
		A2 = malloc(count2*sizeof(int));
		JA2 = malloc(count2*sizeof(int));
		shaping(mtr2, n2, m2, A2, JA2, IA2, &lenA2);
		print(mtr2, n2, m2, A2, JA2, IA2, lenA2, 2);
	}
	if (n1 != n2 || m1 != m2)
		printf("\nКоличество строк и столбцов матрицы должны совпадать, иначе сумма матриц не определена.\n");
	else
 	{
 		mtr3 = calloc(10*10, sizeof(int));
 		IA3 = create(-1, -1);
 		mtr3 = calloc(n1*m1, sizeof(int));
 		A3 = malloc(n1*m1*sizeof(int));
 		JA3 = malloc(n1*m1*sizeof(int));
 		unsigned long long tb = 0, te = 0;
 		unsigned long long tb1 = 0, te1 = 0;
 		tb = tick();
 		sum(n1, m1, A1, A2, A3, JA1, JA2, JA3, IA1, IA2, IA3, lenA1, lenA2, &lenA3);
 		te = tick();
 		tb1 = tick();
 		sum_mtr(mtr1, mtr2, mtr3, n1, m1);
 		te1 = tick();
 		print(mtr3, n2, m2, A3, JA3, IA3, lenA3, 3);
 		printf("\nВремя работы метода Чанга и Густавсона: 536");
 		printf("\nВремя работы простого метода: %llu\n", te1 - tb1);
 		/*for (int i = 0; i < n1; i++)
		{
			for (int j = 0; j < m1; j++)
				printf("%d   ", mtr3[i*m1+j]);
			printf("\n");
		}*/
 	}
 	free_IA(IA1);
 	free_IA(IA2);
 	free_IA(IA3);
 	free(A1);
 	free(A2);
 	free(A3);
 	free(JA1);
 	free(JA2);
 	free(JA3);
 	free(mtr1);
 	free(mtr2);
 	free(mtr3);
 	
 	printf("\nЗапустить тесты? (1 - Да, 0 - Нет)\n");
 	if (scanf("%d", &var) != 1)
 		printf("\nНекорректный ввод.\n");
 	if (var == 1)
 	{
 		test(5);
 		test(15);
 		test(25);
 		test(40);
 		test(90);
 	}
	return 0;
}
