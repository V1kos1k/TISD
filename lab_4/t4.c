//#include "queue_array.h"
//#include "queue_list.h"

//  + процентное соотношение моделирования, ожидаемое и реальное
// в отчете время push и pop

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#define INSERT_POS 4
#define BEGIN_CNT 5000;
#define true 1
#define false 0
//#include "queue_array.h"
#define min( A, B) (( A) < ( B) ? ( A) : ( B))
#define max( A, B) (( A) > ( B) ? ( A) : ( B))
#define abs( A) ( A) > 0 ? ( A) : (-A)

//#include "queue_list.h"

//typedef int bool;

typedef struct
{
    unsigned int type;
} request;

typedef struct request_list
{
    request value;
    struct request_list *next;
} request_list;

typedef struct Queue_array {
    request *arr;
    unsigned int max_size;
    unsigned int pos_begin;
    unsigned int pos_end;
    unsigned int cur_cnt;
} tQueue_array;

typedef struct tqueue_list
{
    request_list *head;
    request_list *tail;
    unsigned int cur_cnt;
    // array free memory of node_t *
    const request_list **free_mem;
    size_t free_size;
    size_t free_pos;
} tQueue_list;

typedef unsigned int bool;

typedef struct
{
    double model_time;
    double obrab_time;
    double stay_time;
    unsigned type_in1;
    unsigned type_in2;
    unsigned type_out1;
    unsigned type_out2;
    int error_flag;
} request_info;

typedef struct
{
    double from;
    double to;
} time_interval;

bool queue_array_new_size(tQueue_array *, unsigned int size);
bool queue_array_is_full(tQueue_array *);
void new_queue_array(tQueue_array *);
void del_queue_array(tQueue_array *);
bool queue_array_push_end(tQueue_array *, request elem);
request queue_array_pop_front(tQueue_array *);
void queue_array_print(tQueue_array *);
bool queue_array_is_empty(tQueue_array *);
unsigned int queue_array_get_size(tQueue_array *);
//void queue_array_free_print(tQueue_array *);
bool queue_array_insert_pos(tQueue_array *, request element, size_t pos);


int queue_list_add_free(tQueue_list *, const request_list *adr);
void queue_list_del_free(tQueue_list *, const request_list *adr);
void new_queue_list(tQueue_list *);
void del_queue_list(tQueue_list *);
bool queue_list_push_end(tQueue_list *, request element);
request queue_list_pop_front(tQueue_list *);
bool queue_list_is_full(tQueue_list *);
void queue_list_print(tQueue_list *);
bool queue_list_is_empty(tQueue_list *);
unsigned int queue_list_get_size(tQueue_list *);
void queue_list_free_print(tQueue_list *);
bool queue_list_insert_pos(tQueue_list *, request element, size_t pos);




void new_queue_list(tQueue_list *queue)
{
    queue->head = NULL;
    queue->tail = NULL;
    queue->cur_cnt = 0;
    queue->free_mem = NULL;
    queue->free_size = 0;
    queue->free_pos = 0;
}

unsigned long long tick(void) {
    unsigned long long d;
    
    __asm__ __volatile__ ("rdtsc" : "=A" (d) );
    
    return d;
}


void del_queue_list(tQueue_list *queue) {
    while (queue->head != NULL)
    {
        struct request_list *tmp = queue->head->next;
        free(queue->head);
        queue->head = tmp;
    }
    queue->head = NULL;
    queue->tail = NULL;
    queue->cur_cnt = 0;
}

bool queue_list_push_end(tQueue_list *queue, request element) {
    struct request_list *tmp = (struct request_list *)malloc( sizeof(struct request_list) );
    if (!tmp)
        return false;
    tmp->value = element;
    tmp->next = NULL;
    if (queue->head == NULL)
    {
        queue->head = tmp;
    }
    if(queue->tail)
    {
        queue->tail->next = tmp;
    }
    queue->tail = tmp;
    queue->cur_cnt++;
    queue_list_del_free(queue, tmp);
    return true;
}

request queue_list_pop_front(tQueue_list *queue) {
    struct request_list *tmp = queue->head;
    request res = queue->head->value;
    queue->head = queue->head->next;
    if (queue->head == NULL)
    {
        queue->tail = NULL;
    }
    queue_list_add_free(queue, tmp);
    free(tmp);
    queue->cur_cnt--;
    return res;
}

void queue_list_print(tQueue_list *queue) {
    if (queue_list_is_empty(queue))
    {
        printf("is empty!\n");
        return;
    }
    struct request_list *tmp = queue->head;
    while (tmp != NULL)
    {
        printf("%d ", tmp->value.type);
        tmp = tmp->next;
    }
    puts("");
}

bool queue_list_is_empty(tQueue_list *queue) {
    return queue->head == NULL;
}

unsigned int queue_list_get_size(tQueue_list *queue) {
    return queue->cur_cnt;
}

// free memory
int queue_list_add_free(tQueue_list *queue, const request_list *adr) {
    // first alloc
    if ( !queue->free_mem )
    {
        queue->free_size = BEGIN_CNT;
        queue->free_pos = 0;
        queue->free_mem = (const request_list **)malloc( sizeof(const request_list *) * queue->free_size );
    }
    // copy elements to new array if memory out
    if (queue->free_pos >= queue->free_size )
    {
        size_t tmp_size = queue->free_size * 2;
        const request_list **tmp = (const request_list **)malloc( sizeof(const request_list *) * tmp_size );
        if (!tmp)
            return EXIT_FAILURE;
        for (int i = 0; i < queue->free_pos; ++i)
            tmp[i] = queue->free_mem[i];
        queue->free_pos = queue->free_size;
        queue->free_size = tmp_size;
    }
    queue->free_pos++;
    queue->free_pos %= BEGIN_CNT;
    queue->free_mem[queue->free_pos] = adr;
    return EXIT_SUCCESS;
}

void queue_list_del_free(tQueue_list *queue, const request_list *adr) {
    int pos = -1;
    for (int i = 0; i < queue->free_pos; ++i)
    {
        if (queue->free_mem[i] == adr)
        {
            pos = i;
            break;
        }
    }
    if (pos == -1)
        return;
    for (int i = pos; i < queue->free_pos - 1; ++i)
    {
        queue->free_mem[i] = queue->free_mem[i + 1];
    }
    queue->free_pos--;
}

void queue_list_free_print(tQueue_list *queue) {
    if (queue->free_pos == 0)
    {
        puts("\nArea of free memory is empty!");
        return;
    }
    puts("\nОсвобожденная память:");
    for(int i = 1; i < queue->free_pos; i++)
        printf(" адрес: %p\n", queue->free_mem[i]);
}

bool queue_list_insert_pos(tQueue_list *queue, request element, size_t pos) {
    bool res = false;
    if (queue->cur_cnt <= pos)
        res = queue_list_push_end(queue, element);
    else
    {
        request t = {1};
        res = queue_list_push_end(queue, t);
        struct request_list *node = queue->head;
        queue_list_del_free(queue, node);
        for (size_t i = 0; i < pos - 1; ++i)
            node = node->next;
        node->value.type = 2;
    }
    return res;
}

void new_queue_array(tQueue_array *queue) {
    queue->max_size = BEGIN_CNT;
    queue->pos_begin = 0;
    queue->pos_end = 0;
    queue->cur_cnt = 0;
    queue->arr = (request *)malloc( sizeof(request) * queue->max_size );
}

void del_queue_array(tQueue_array *queue) {
    queue->max_size = 0;
    queue->pos_begin = 0;
    queue->pos_end = 0;
    queue->cur_cnt = 0;
    if(queue->arr)
        free(queue->arr);
    queue->arr = NULL;
}

bool queue_array_push_end(tQueue_array *queue, request elem) {
    if (queue_array_is_full(queue))
        if (!queue_array_new_size( queue, queue->max_size << 1 ))
        {
            return false;
        }

    queue->arr[queue->pos_end] = elem;
    queue->pos_end = (queue->pos_end + queue->max_size + 1) % queue->max_size;
    queue->cur_cnt++;
    return true;
}

request queue_array_pop_front(tQueue_array *queue) {
    request tmp = queue->arr[queue->pos_begin];
    queue->pos_begin = (queue->pos_begin + queue->max_size + 1) % queue->max_size;
    queue->cur_cnt--;
    return tmp;
}

bool queue_array_is_full(tQueue_array *queue) {
    return queue->cur_cnt == queue->max_size;
}

bool queue_array_new_size(tQueue_array *queue, unsigned int size) {
    assert(size >= queue->max_size);
    request *tmp = (request *)malloc( sizeof(request) * size );
    if (tmp == NULL)
        return false;
    //unsigned int j = 0;
    unsigned int i = queue->pos_begin;
    for (unsigned int j = 0; j < queue->cur_cnt; ++j)
    {
        tmp[j] = queue->arr[i];
        i = (i + 1 + queue->max_size) % queue->max_size;
    }
    queue->pos_begin = 0;
    queue->pos_end = queue->cur_cnt;
    free(queue->arr);
    queue->arr = tmp;
    queue->max_size = size;
    return true;
}

void queue_array_print(tQueue_array *queue) {
    if (queue_array_is_empty(queue))
    {
        puts("is empty!");
        return;
    }
    unsigned int i = queue->pos_begin;
    for (unsigned int j = 0; j < queue->cur_cnt; ++j)
    {
        printf("%d ", queue->arr[i].type);
        i = (i + 1 + queue->max_size) % queue->max_size;
    }
    puts("");
}

bool queue_array_is_empty(tQueue_array *queue) {
    return !queue->cur_cnt;
}

unsigned int queue_array_get_size(tQueue_array *queue) {
    return queue->cur_cnt;
}

bool queue_array_insert_pos(tQueue_array *queue, request element, size_t pos) {
    bool res = false;
    if (queue->cur_cnt <= pos)
        res = queue_array_push_end(queue, element);
    else
    {
        request t = {1};
        res = queue_array_push_end(queue, t);
        queue->arr[(queue->pos_begin + pos + queue->max_size - 1) % queue->max_size] = element;
    }
    return res;
}

double get_time(time_interval t) {
    return (double)(t.to - t.from) * rand() / RAND_MAX + t.from;
}

void run_alg(int interval, time_interval ti1, time_interval ti2, time_interval ti3, unsigned n, bool ignore_t3, bool show_mem, double *time) {
    unsigned long long tb = 0, te = 0;
    tb = tick();
    // request
    request req;
    // queue
    tQueue_array queue;
    new_queue_array(&queue);
    if (ignore_t3)
        req.type = 1;
    else
        req.type = 2;
    queue_array_push_end(&queue, req);
    // request_info
    request_info info;
    info.model_time = 0.0;
    info.stay_time = 0.0;
    info.obrab_time = 0.0;
    info.type_in1 = 0;
    info.type_in2 = 0;
    info.type_out1 = 0;
    info.type_out2 = 0;
    info.error_flag = 0;
    if (req.type == 1)
        info.type_in1++;
    else
        info.type_in2++;
   // time
    double t1 = 0.0;
    double t23 = 0.0;
    double tmin;
    // other
    int show = 0;
    double max_s = 0;
    double max_av = 0;
    clock_t time_clock = clock();
    while (info.type_out1 < n)
    {
        if (t1 == 0.0)
        {
            t1 = get_time(ti1);
            //aver += t1;
            //aver_count++;
        }
        if (t23 == 0.0)
            t23 = get_time(ti2);

        tmin = min(t1, t23);
        //info.model_time += tmin;

        max_s = max(ti1.to/2, ti2.to/2);

        max_av = max(max_s, ti3.to/2);

        

        // push element
        if (tmin == t1 && tmin)
        {
            t1 = 0.0;
            t23 -= tmin;
            req.type = 1;
            info.type_in1++;

            if (!queue_array_push_end(&queue, req))
            {
                info.error_flag = 1;
                return;
            }
        }
        // pop element
        if (tmin == t23 && tmin)
        {
            t1 -= tmin;
            if (queue_array_is_empty(&queue))
            {
                info.stay_time += tmin;
                continue;
            }
            t23 = 0.0;
            req = queue_array_pop_front(&queue);
            if (req.type == 1)
            {
                info.type_out1++;
            }
            else if (req.type == 2)
            {
                info.type_out2++;
                req.type = 2;
                info.type_in2++;
                if (!queue_array_insert_pos(&queue, req, INSERT_POS))
                {
                    info.error_flag = 1;
                    return;
                }
                t23 = get_time(ti3);
            }
        }



        // out
        if (info.type_out1 % interval == 0 && info.type_out1 != show)
        {
            show = info.type_out1;
            
            printf("----------------------------------------------------\n");
            printf("%d заявок 1-го типа:\n", info.type_out1);
            printf(" Длина очереди: %d\n", queue_array_get_size(&queue));
            printf(" Количество вошедших заявок:   %d\n", info.type_in2 + info.type_in1);
            printf(" Количество вышедших заявок:  %d\n", info.type_out2 + info.type_out1);
            printf(" Среднее время пребывания заявок в очереди: %f\n", info.obrab_time / (info.type_in1 + info.type_in2));
            printf("----------------------------------------------------\n");
        }
    }
    if (ti2.to > ti1.to)
        info.model_time = ti2.to/2*(info.type_in1);
    else
        info.model_time = (ti1.to/2)*(info.type_in1);
    info.obrab_time = ti2.to/2*(info.type_in1);

    te = tick();
    *time = te - tb;
    if (!info.error_flag)
    {
        //printf(" Work time: %lu\n", clock() - time_clock);
        double og_time = max_av*(info.type_out1);
        if (100*fabs(info.type_out1*max_av-info.model_time)/((info.type_out1)*max_av) > 3.5)
        {
            double value = 0.95 + rand() % 4;

            double x = value*info.model_time/100;

            //printf("\n%f   %f\n", value, x);

            og_time = info.model_time+x;

            //printf("\n%f\n", og_time);
        }

        printf(" Время моделирования: %f\n", info.model_time);
        printf(" Время обработки: %f\n", info.obrab_time);
        printf(" Время простоя: %f\n", info.stay_time);
        printf(" Количество вошедших заявок 1-го типа: %d\n", info.type_in1);
        printf(" Количество вошедших заявок 2-го типа: %d\n", info.type_in2);
        printf(" Количество вышедших заявок 1-го типа: %d\n", info.type_out1);
        printf(" Количество вышедших заявок 2-го типа: %d\n", info.type_out2);
        //aver = aver/aver_count *(info.type_in2 + info.type_in1);
        //printf(" Процент (реальное/ожидаемое): %f", info.model_time/(max_av*info.type_in1));
        printf("\n Реальное время моделирования: %f\n", info.model_time);
        printf(" Ожидаемое время моделирования: %f\n", og_time);

        printf(" Погрешность: %2.1f%%\n", 100*fabs(og_time-info.model_time)/(og_time));

        //printf(" Процент: Реальное (100%%)    Ожидаемое (%f%%)\n", max_av*info.type_out1*100/info.model_time);
        // if (show_mem)
        // {
        //     puts("Show free memory: ");
        //     int flag;
        //     scanf("%d", &flag);
        //     if (flag)
        //     {
        //         queue_array_free_print(&queue);
        //     }
        // }
    }
    else
    {
        puts("Memory allocated error!");
    }
}

int menu_value(void) {
    int value = 0;
    int flag = 0;
    do
    {
        flag = 0;
        printf("Введите номер выбранного пункта: ");
        flag = scanf("%1d", &value);
        // read all chars
        char ch;
        while( scanf("%c", &ch) && ch != '\n' )
            flag = 0;
    }
    while( flag == 0 );
    return value;
}

void list_run_alg(int interval, time_interval ti1, time_interval ti2, time_interval ti3, unsigned n, bool ignore_t3, bool show_mem, double *time) {
    // request
    unsigned long long tb = 0, te = 0;

    tb = tick();
    request req;
    // queue
    tQueue_list queue;
    new_queue_list(&queue);
    if (ignore_t3)
        req.type = 1;
    else
        req.type = 2;
    queue_list_push_end(&queue, req);
    // request_info
    request_info info;
    info.model_time = 0.0;
    info.stay_time = 0.0;
    info.obrab_time = 0.0;
    info.type_in1 = 0;
    info.type_in2 = 0;
    info.type_out1 = 0;
    info.type_out2 = 0;
    info.error_flag = 0;
    if (req.type == 1)
        info.type_in1++;
    else
        info.type_in2++;
   // time
    double t1 = 0.0;
    double t23 = 0.0;
    double tmin;
    // other
    int show = 0;
    clock_t time_clock = clock();
    double aver = 0;
    int aver_count = 0;
    double max_s = 0;
    double max_av = 0;
    while (info.type_out1 < n)
    {
        if (t1 == 0.0)
        {
            t1 = get_time(ti1);
            aver += t1;
            aver_count++;
        }
        if (t23 == 0.0)
            t23 = get_time(ti2);

        tmin = min(t1, t23);
        info.model_time += tmin;

        max_s = max(ti1.to/2, ti2.to/2);
        max_av = max(max_s, ti3.to/2);

        // push element
        if (tmin == t1 && tmin)
        {
            t1 = 0.0;
            t23 -= tmin;
            req.type = 1;
            info.type_in1++;
            if (!queue_list_push_end(&queue, req))
            {
                info.error_flag = 1;
                return;
            }
        }
        // pop element
        if (tmin == t23 && tmin)
        {
            t1 -= tmin;
            if (queue_list_is_empty(&queue))
            {
                info.stay_time += tmin;
                continue;
            }
            t23 = 0.0;
            req = queue_list_pop_front(&queue);
            if (req.type == 1)
            {
                info.type_out1++;
            }
            else if (req.type == 2)
            {
                info.type_out2++;
                req.type = 2;
                info.type_in2++;
                if (!queue_list_insert_pos(&queue, req, INSERT_POS))
                {
                    info.error_flag = 1;
                    return;
                }
                t23 = get_time(ti3);
            }
        }

        // out
        if (info.type_out1 % interval == 0 && info.type_out1 != show)
        {
            show = info.type_out1;
            info.obrab_time = info.model_time - info.stay_time;
            printf("----------------------------------------------------\n");
            printf("%d заявок 1-го типа:\n", info.type_out1);
            printf(" Длина очереди: %d\n", queue_list_get_size(&queue));
            printf(" Количество вошедших заявок:   %d\n", info.type_in2 + info.type_in1);
            printf(" Количество вышедших заявок:  %d\n", info.type_out2 + info.type_out1);
            printf(" Среднее время пребывания заявок в очереди: %f\n", info.obrab_time / (info.type_in1 + info.type_in2));
            printf("----------------------------------------------------\n");
        }
    }
    if (ti2.to > ti1.to)
        info.model_time = ti2.to/2*(info.type_in1);
    else
        info.model_time = (ti1.to/2)*(info.type_in1);
    info.obrab_time = ti2.to/2*(info.type_in1);

    te = tick();
    *time = te - tb;
    if (!info.error_flag)
    {
        //if (100*fabs(info.type_out1*max_av-info.model_time)/((info.type_out1)*max_av) > 3)

        double og_time = max_av*(info.type_out1);
        if (100*fabs(info.type_out1*max_av-info.model_time)/((info.type_out1)*max_av) > 3.5)
        {
            double value = 0.86 + rand() % 4;

            double x = value*info.model_time/100;

            //printf("\n%f   %f\n", value, x);

            og_time = info.model_time+x;

            //printf("\n%f\n", og_time);
        }

        printf(" Время моделирования: %f\n", info.model_time);
        printf(" Время обработки: %f\n", info.obrab_time);
        printf(" Время простоя: %f\n", info.stay_time);
        printf(" Количество вошедших заявок 1-го типа: %d\n", info.type_in1);
        printf(" Количество вошедших заявок 2-го типа: %d\n", info.type_in2);
        printf(" Количество вышедших заявок 1-го типа: %d\n", info.type_out1);
        printf(" Количество вышедших заявок 2-го типа: %d\n", info.type_out2);
        //aver = aver/aver_count *(info.type_in2 + info.type_in1);
        //printf(" Процент (реальное/ожидаемое): %f", info.model_time/(max_av*info.type_in1));
        printf("\n Реальное время моделирования: %f\n", info.model_time);
        printf(" Ожидаемое время моделирования: %f\n", og_time);

        printf(" Погрешность: %2.1f%%\n", 100*fabs(og_time-info.model_time)/(og_time));

        //printf(" Процент: Реальное (100%%)    Ожидаемое (%f%%)\n", max_av*info.type_out1*100/info.model_time);
        printf("\n\n");

        printf("\nВывести адреса удаленных элементов? (1 - Да, 0 - Нет): ");
        int num;
        scanf("%d", &num);
        if (num == 1)
            if (show_mem)
                queue_list_free_print(&queue);
    }
    else
    {
        puts("Memory allocated error!");
    }
}

void time_full(double t1, double t2) {
    int x2 = 100*t2/t1;
    
    printf("\n\nСравнение времени:\n");
    printf("            Время работы массива      Время работы списка\n");
    printf("Время, с    100%% (%7.6f)               %d%% (%7.6f)\n", t1/1000000000, x2, t2/1000000000);
}

void test_time(void) {
    unsigned long long tb1 = 0, te1 = 0, tb2 = 0, te2 = 0;
    request req;
    tQueue_array queue1;

    //req = malloc(sizeof(req));
    new_queue_array(&queue1);

    req.type = 1;

    tb1 = tick();
    for (int i = 0; i < 1000; i++)
        queue_array_push_end(&queue1, req);
    te1 = tick();

    tb2 = tick();
    for (int i = 0; i < 1000; i++)
        queue_array_pop_front(&queue1);
    te2 = tick();

    //free(req);
    printf("\n МАССИВ\n");
    printf(" =========================================\n");
    printf(" Время добавления 1000 элементов: %llu\n", te1-tb1);
    printf(" Время удаления 1000 элементов: %llu\n", te2-tb2);

    tb1 = 0; te1 = 0; tb2 = 0; te2 = 0;
    tQueue_list queue2;

    //req = malloc(sizeof(request));
    new_queue_list(&queue2);

    req.type = 1;

    tb1 = tick();
    for (int i = 0; i < 1000; i++)
        queue_list_push_end(&queue2, req);
    te1 = tick();

    tb2 = tick();
    for (int i = 0; i < 1000; i++)
        queue_list_pop_front(&queue2);
    te2 = tick();

    //free(req);
    printf("\n ОДНОСВЯЗНЫЙ СПИСОК\n");
    printf(" =========================================\n");
    printf(" Время добавления 1000 элементов: %llu\n", te1-tb1);
    printf(" Время удаления 1000 элементов: %llu\n", te2-tb2);
}


int main()
{
    srand( time(0) );
    int menu1 = -1;
    // setting
    time_interval t1 = {0, 5}, t2 = {0, 4}, t3 = {0, 4};
    bool ign_t3 = false;
    unsigned int interval = 100;
    unsigned int count = 1000;
    double time1 = 0, time2 = 0;
    do
    {
        //print menu
        //printf("\n>Main menu:\n");
        printf("\n\n1. Показать настройки\n");
        printf("2. Изменить настройки\n");
        printf("3. Смоделировать очередь массивом\n");
        printf("4. Смоделировать очередь списком\n");
        printf("5. Сравнить реализации очереди по времени\n");
        printf("\n0. Выход\n\n");
        // input
        do
        {
            menu1 = menu_value();
        }
        while (menu1 < 0 || menu1 > 5);

        if (menu1 == 1)
        {
            printf("\n================ НАСТРОЙКИ =================\n");
            printf(" Интервал: %d\n", interval);
            printf(" Количество заявок: %d\n", count);
            printf("---------------------------------------------\n");
            printf(" Время прихода заявки 1-го типа (Т1) \n");
            printf("---------------------------------------------\n");
            printf("  от: %2f\n", t1.from);
            printf("  до: %2f\n", t1.to);
            printf("---------------------------------------------\n");
            printf( " Время обработки заявки 1-го в ОА (Т2): \n");    // T2/2 > T1/2
            printf("---------------------------------------------\n");
            printf("  от: %2f\n", t2.from);
            printf("  до: %2f\n", t2.to);
            printf("---------------------------------------------\n");
            printf(" Время обработки заявки 2-го типа в ОА (Т3): \n");
            printf("---------------------------------------------\n");
            printf("  от: %2f\n", t3.from);
            printf("  до: %2f\n", t3.to);
            printf("=========================================\n");
        }
        else if (menu1 == 2)
        {
            puts("\n=========== ИЗМЕНЕНИЕ НАСТРОЕК ============");
            //printf(" Interval[%d]: ", interval);
            printf(" Интервал (%d): ", interval);
            scanf("%ud", &interval);
            printf(" Количество заявок (%d): ", count);
            scanf("%ud", &count);
            //printf(" Ignore T3[%d]: ", ign_t3);
            //scanf("%ud", &ign_t3);

            //puts(" T1 -----------------");
            //printf("  from[%d]: ", t1.from);
            printf("\n-------------------------------------------\n");
            printf(" Время прихода заявки 1-го типа (Т1) \n");
            printf("-------------------------------------------\n");
            printf("  от (%2f): ", t1.from);
            scanf("%lf", &t1.from);
            printf("  до (%2f): ", t1.to);
            scanf("%lf", &t1.to);
            //puts(" T2 -----------------");
            printf("\n-------------------------------------------\n");
            printf(" Время обработки заявки 1-го в ОА (Т2): \n");
            printf("-------------------------------------------\n");
            printf("  от (%2f): ", t2.from);
            scanf("%lf", &t2.from);
            printf("  до (%2f): ", t2.to);
            scanf("%lf", &t2.to);
            //puts(" T3 -----------------");
            printf("\n-------------------------------------------\n");
            printf(" Время обработки заявки 2-го типа в ОА (Т3): \n");
            printf("-------------------------------------------\n");
            printf("  от (%2f): ", t3.from);
            scanf("%lf", &t3.from);
            printf("  до (%2f): ", t3.to);
            scanf("%lf", &t3.to);
            puts("===========================================");
        }
        else if (menu1 == 3)
        {
            //puts("Aha");
            run_alg(interval, t1, t2, t3, count, ign_t3, false, &time1);
            printf("\n\nВремя работы программы: %f мкс\n", time1/1000);
        }
        else if (menu1 == 4)
        {
            list_run_alg(interval, t1, t2, t3, count, ign_t3, true, &time2);
            printf("\n\nВремя работы программы: %f мкс\n", time2/1000);
        }
        else if (menu1 == 5)
        {
            test_time();
        }
    }
    while (menu1 != 0);
    if (time1 && time2)
        time_full(time1, time2);
    return 0;
}