// Элементами стека являются слова.  Распечатайте слова в обратном порядке.


// после пункта 4 или 6 next указывает на адрес 0x200000001 или 0x200000000
// может это потому, что после прохода по списку указатель не возвращается на последний элемент

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# define STACK_OVERFLOW  -100
# define STACK_UNDERFLOW -101
# define STACK_MAX_SIZE 20
# define OUT_OF_MEMORY   -102
# define INCORRECT_INPUT -1

struct Stack {
    char data[STACK_MAX_SIZE][30];
    int size;
} stack;

struct Head {
    char value[100];
    struct Head *next;
    //struct Head *first;
} head;

int size_head(struct Head *head);
int push_head(struct Head **head, char *value);

unsigned long long tick(void)
{
    unsigned long long d;
    
    __asm__ __volatile__ ("rdtsc" : "=A" (d) );
    
    return d;
}

int push_stack(struct Stack *stack, char *value) {
    if (stack->size >= STACK_MAX_SIZE) {
        return STACK_OVERFLOW;
    }
    int len = strlen(value);
    for (int i = 0; i < len; i++)
    {
        if (value[i] != '\n')
            stack->data[stack->size][i] = value[i];
        //printf("%c", value[i]);
    }
    stack->size++;
    
    return 0;
}

int push_head(struct Head **head, char *value) {
    struct Head *tmp = malloc(sizeof(head));
    if (tmp == NULL)
        return STACK_OVERFLOW;
    tmp->next = *head;
    //printf("\ntmp  %p", tmp->next);
    strcpy(tmp->value, value);
    *head = tmp;
    //printf("   tmp  %p", tmp->next);
    return 0;
}

int pop_stack(struct Stack *stack) {
    if (stack->size == 0) {
        return STACK_UNDERFLOW;
    }
    stack->size--;
    return 0;
}

struct Head *pop_head(struct Head **head) {
    struct Head *out;
    if ((*head) == NULL) {
        printf("\nПустой стек.\n");
        return NULL;
    }
    else
    {
    	out = *head;  // узел
    	*head = (*head)->next;
    	printf("\n%p    %s", out->next, out->value);
    	free(out);
    	return 0;
    }
}/////////////////////

void printStackValue(char value[30]) {
    printf("%s   ", value);
}
 
void printStack(struct Stack *stack) {
    int i;
    int len = stack->size - 1;
    for (i = 0; i < len; i++) {
        printStackValue(stack->data[i]);
    }
    if (stack->size != 0) {
        printStackValue(stack->data[i]);
    }
    printf("\n");
}

void reverseHead(struct Head *head, int count) {
    while (head) {
        printf("\n%p    %s", head->next, head->value);
        if (count == 1)
            break;
        head = head->next;
    }
    printf("\n");
}   //////////////

void printHead2(struct Head *head) ////////////
{
    if (!head)
        return;
    printHead2(head->next);
    printf("\n%p    %s", head->next, head->value);
}

/*int test(char *s){
    int len = strlen(s);
    int count = 0; // проверка на пробелы
    for (int i = 0; i < len; i++)
    {
        if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= '0' && s[i] <= '9'))
        {
            count++;
            continue;
        }
        else if (s[i] == ' ')
            continue;
        else if (s[i] == '\n')
        {
            if (count == 0)
        		return INCORRECT_INPUT;
            return 0;
        }
        else
            return INCORRECT_INPUT;
    }
    return 0;
}*/

int test_enter(char *s)
{
    int len = strlen(s);
    int count = 0;
    for (int i = 0; i < len; i++)
    {
        if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= '0' && s[i] <= '9'))
        {
            count++;
            continue;
        }
        else if (s[i] == '\n')
        {
            if (count == 0)
                return INCORRECT_INPUT;
            return 0;
        }
        else
            return INCORRECT_INPUT;
    }
    return 0;
}

/*int parse(struct Stack *stack, struct Head **head, char *s, int *count)
{
    char str[30] = {'\0'};
    //init(str, 30);
    int len = strlen(s), j = 0;
    if (len == 1)
        return INCORRECT_INPUT;
    for (int i = 0;  i < len; i++)
    {
        if ((s[i] == ' ' || s[i] == '\n') && (s[i-1] != ' '))
        {
            if (i != 0)
            {
	            str[j] = '\0';

	            push_stack(stack, str);
    	        push_head(head, str, count);
        	    for(int k = 0; k < 30; k++)
            	    str[k] = '\0';
            	j = 0;
            }
        }
        else if (s[i] != '\n')
        {
            if ((s[i] == ' ' && s[i-1] != ' ') || (s[i] != ' '))
            {
                str[j] = s[i];
                j++;
            }
        }
    }
    return 0;
}*/

int Enter(struct Stack *stack, struct Head **head, int *count)
{
    char str[30] = {'\0'};
    printf("Введите слово:");
    fgets(str, 30, stdin);
    if (test_enter(str) == -1)
        return INCORRECT_INPUT;
    //printf("\n%s", str);
    if (push_stack(stack, str) == -100)
        return STACK_OVERFLOW;
    push_head(head, str);
    *count += 1;
    return 0;
}

void reverse(struct Stack *stack)
{
    int i;
    for (i = stack->size - 1; i > 0; i--)
    {
        printStackValue(stack->data[i]);
    }
    if (stack->size != 0) {
        printStackValue(stack->data[i]);
    }
    printf("\n");
}

void time(double t1, double t2)
{
    int x2 = 100*t1/t2;
    
    printf("\n\nСравнение времени и памяти:\n");
    printf("            Стек статичным массивом      Стек списком\n");
    printf("Время, с    %d%% (%7.6f)               100%% (%7.6f)\n", x2, t1/1000000000, t2/1000000000);
}

int menu(void)
{
    char s[5];
    int a = -1;
    printf("\n\n1. Добавить эл-ты в стек\n2. Извлечь эл-т из стека\n3. Распечатать стек статичным массивом\n");
	printf("4. Распечатать стек списком\n5. Распечатать стек массивом в обратном порядке\n6. Распечатать стек списком");
	printf(" в обратном порядке\n7. Распечатать список свободных адресов\n8. Выход\n");
    do {
        printf("\nВведите номер выбранного пункта: ");
        fgets(s, 5, stdin);
        a = atoi(s);
        printf("\n");
    } while (a < 1 || a > 8);
    return a;
}

int main()
{
    setbuf(stdout, NULL);
    
    struct Stack stack;
    struct Head *head = NULL;
    struct Head *node[STACK_MAX_SIZE];
    
    
    stack.size = 0;
    char str [200];
    int err = 0, choice =-1;
    int count = 0, n = 0;
    int len_node = 0;
    
    /*printf("\nВведите слова через пробел: \n");
    fgets(str, 200, stdin);
    if (test(str) == -1)
    {
        printf("\nНекорректный ввод.\n");
        return -1;
    }
    if (parse(&stack, &head, str, &count) == -1)
    {
        printf("\nНекорректный ввод.\n");
        return 0;
    }*/
    n = count;
    int count1 = 1;
    double time1 = 0, time2 = 0;
    do {
        choice = menu();
        if (choice == 1)
        {
            //printf("  head  %p\n", head->next);
            err = Enter(&stack, &head, &count);
            //printf("  head  %p\n", head->next);
            if (err == -1)
                printf("\nНекорректный ввод.\n");
            else if (err == -100)
                printf("\nПереполнение стека.\n");
            else
            {
                n++;
                count1++;
                //printf("  head  %p\n", head->next);
                //head->first = head->next;
            }
        }
        else if (choice == 2)
        {
            err = pop_stack(&stack);
            if (err == -101)
                printf("\nПустой стек.\n");
            else
            {
                //head->next = head->first;
                if (count1 == 0)
                {
                    printf("\nСтек списком пустой.\n");
                }
                else
                {
        	    node[len_node] = head;
        	    pop_head(&head);
        	    //printf("\ncount = %d\n", count);
                count--;
                //printf("\n+++++ %p   %s", node[len_node]->next, node[len_node]->value);
                //print_node(*node, len_node);
                //head->first = head->next;
                //printf("\n%p   %s\n", head->next, head->value);
                len_node++;
                }
            }
        }
        else if (choice == 3)
        {
            if (stack.size == 0)
                printf("\nПустой стек.\n");
            else
                printStack(&stack);
        }
        else if (choice == 4)
        {
            if (count == 0 || count1 == 0)
                printf("\nПустой стек.\n");
            else
            {
                //head->next = head->first;
                printHead2(head);
                printf("\n");
                //head->next = head->first;
            }
                
        }
        else if (choice == 5)
        {
            if (count == 0)
            {
                printf("\nПустой стек.\n");
            }
            else
            {
            	unsigned long long tb = 0, te = 0;
            	tb = tick();
            	reverse(&stack);
            	te = tick();
            	time1 = te - tb;
            	printf("\nВремя печати стека массивом в обратном порядке: %f\n", time1/1000000000);
            }
        }
        else if (choice == 6)
        {
            if (count == 0 || count1 == 0)
                printf("\nПустой стек.\n");
            else
            {
            	unsigned long long tb =0, te = 0;
            	//head->next = head->first;
            	tb = tick();
            	//reverseHead(head, count);
            	while(head)
            	    pop_head(&head);
            	count1 = 0;
            	te = tick();
            	printf("\n");
            	time2 = te - tb;
            	printf("\nВремя печати стека списком в обратном порядке: %f\n", time2/1000000000);
            	//head->next = head->first;
            }
        }
        else if (choice == 7)
        {
            int k = n;
            printf("\nАдреса освобожденных элементов стека списком: \n");
            for (int i = 0; i <= len_node-1; i++)
                {
                    printf("\nадрес %d  элемента стека: %p", k--, node[i]->next);
                }
        }
    } while (choice != 8);
    
    if (time1 != 0 && time2 != 0)
        time(time1, time2);
    return 0;
}
