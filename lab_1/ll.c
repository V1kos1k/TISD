#include <stdio.h>
#include <stdlib.h>

#define TRUE               1
#define LENGTH            31
#define POWER_LENGTH       5

#define OK                 1
#define ERROR_WRONG_SIGN  -1
#define ERROR_WRONG_NUMB  -2
#define ERROR_TOO_LONG    -3
#define ERROR_LONG_POWER  -4
#define ERROR_WRONG_POW   -5
#define ERROR_WRONG_PSIGN -7
#define ERROR_NO_POINT    -8
#define ERROR_ZERO_DIV    -9
#define ERROR_EMPTY      -10
#define ERROR_OVERFLOW   -11

struct longinteger
{
    char mantiss[LENGTH+2];
    int  length;
};

struct longreal
{
    char mantiss[LENGTH+3+POWER_LENGTH];
    int  length;
};


// Проверка на нулевой массив
int isZero(char *pa, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (*(pa+i) != '0')
            return 0;
    }
    return 1;
}

// Форматирует данные в удобный для сложения/вычитания вид
int clear (char *pa, int len_a, int full_length)
{
    for(int i = len_a; i>0; i--)
        *(pa+full_length+i-len_a-1) = *(pa+i-1);
    for(int i = 0; i < full_length-len_a; i++)
        *(pa+i) = '0';
    return OK;
}

// Функция, высчитывающая сумму посимвольно
int summ (char *pa, char *pb, int len_a, int len_b, int full_length)
{
    clear(pa,len_a,full_length);
    clear(pb,len_b,full_length);
    int new_rank = 0;
    for (int i = full_length-1; i>=0; i--)
    {
        if ((*(pa+i) + *(pb+i) - '0' + new_rank) <= '9')
        {
            *(pa+i) += *(pb+i) - '0' + new_rank;
            new_rank = 0;
        }
        else
        {
            *(pa+i) += *(pb+i) - '0' + new_rank - 10;
            new_rank = 1;
        }
    }
    return OK;
}

// Функция, высчитывающая разность посимвольно (указатель pa указывает на большее число)
int substr (char *pa, char *pb, int len_a, int len_b, int full_length)
{
    clear(pa,len_a,full_length);
    clear(pb,len_b,full_length);
    int new_rank = 0;
    int tmp = *(pa-1);
    for (int i = full_length-1; i>=-1 ; i--)
    {
        if ( i == -1 && (*(pa-1) == '+' || *(pa-1) == '-' ) && new_rank == 1)
        {
            *pa = '0';
            char universal[6] = {'1','0','0','0','0','0'};
            for (int i = 0; i<6; i++)
            {
                tmp = universal[i];
                universal[i] = *(pa+i);
                *(pa+i) = tmp;
            }
            tmp = *(pa-1);
            substr(pa,&universal[0],6,6,6);
            if (tmp == '-')
                *(pa-1) = '+';
            else
                *(pa-1) = '-';
            return OK;
        }
        if (i == -1)
            break;
        if ((*(pa+i) - *(pb+i) + '0' - new_rank) >= '0')
        {
            *(pa+i) -= (*(pb+i) - '0' + new_rank);
            new_rank = 0;
        }
        else
        {
            *(pa+i) -= (*(pb+i) - '0' + new_rank - 10);
            new_rank = 1;
        }
    }
    return OK;
}

// Функция, переводящая число в массив чаров
int translate (char *pa, int *len, int number)
{
    *len = 0;
    while (number > 0)
    {
        for (int i = *len; i>=0; i--)
            *(pa+i+1) = *(pa+i);
        *(pa) = '0' + number % 10;
        number = number / 10;
        *len += 1;
    }
    return OK;
}

// Функция сдвига массива
int arr_shift (char *pa, int*len, int shift)
{
    for (int i = 0; i<*len-shift; i++)
        *(pa+i) = *(pa+i+shift-1);
    for (int i = *len-shift; i < *len; i++)
    {
        *(pa+i) = '0';
        *len = *len - 1;
    }
    return OK;
}

// Функця нормализации числа
int normilize (char *pa, int *len, int number)
{
    char power_addition[POWER_LENGTH+1];
    int power_addition_length = 0;
    translate(&power_addition[0], &power_addition_length, abs(number));
    if ((number > 0 && *(pa-1) == '+') ||
        (number < 0 && *(pa-1) == '-'))
        summ(pa, &power_addition[0], *len, power_addition_length, POWER_LENGTH+1);
    else
        substr(pa, &power_addition[0], *len, power_addition_length, POWER_LENGTH+1);
    return OK;
}



// Функция считывания числа
// *pa - указатель на первый элемент
// type - тип переменной 1 - целый, 0 - вещественный
int read_number(char *pa, int type, int *new_length)
{
    //Инициализация переменных
    char current;
    int real_point_pos = -1;
    int real_power_length = -1;
	
    //определение знака целого числа (или знака мантиссы вещественного)
    scanf("%c",&current);
    if (current == '+' || current == '-')
        *pa = current;
    else
    {
        if (current == '\n')
            return ERROR_EMPTY;
        return ERROR_WRONG_PSIGN;
    }
	
    for (int i = *new_length; i<LENGTH+2; i++)
    {
        scanf("%c",&current);
        //Для целого числа
        if (type)
        {
            if (current >= '0' && current <= '9')
            {
                if (i>=31)
                    return ERROR_TOO_LONG;
                *(pa+i) = current;
                *new_length += 1;
            }
            else
                if (current != '\n')
					return ERROR_WRONG_NUMB;
				else
					break;
        }
        //Для вещественного числа
        else
        {
            if (current >= '0' && current <= '9')
            {
                if (i>=31)
                    return ERROR_TOO_LONG;
                *(pa+i) = current;
                *new_length += 1;
            }
            else
                if (current == '.' && real_point_pos == -1)
                {
                    real_point_pos = i;
                    i--;
                }
                else
                {
                    if ((current == 'E' || current == 'e') && real_power_length == -1)
                    {
                        real_power_length = 0;
                        i--;
                        break;
                    }
                    else
                        return ERROR_WRONG_NUMB;
                }
        }
    }

    //проверка наличия точки
    if (!type && real_point_pos == -1)
        return ERROR_NO_POINT;
    
    if (!type)
    {
        //определение знака порядка
        scanf("%c",&current);
        if (current == '+' || current == '-')
            *(pa+LENGTH+1) = current;
        else
            return ERROR_WRONG_PSIGN;

        //Ввод порядка
        for (int i = LENGTH+2; i<LENGTH+3+POWER_LENGTH; i++)
        {
            scanf("%c",&current);
            if (current >= '0' && current <= '9')
            {
                if (i>=LENGTH+2+POWER_LENGTH)
                    return ERROR_LONG_POWER;
                *(pa+i) = current;
                real_power_length++;
            }
            else
                if (current != '\n')
                    return ERROR_WRONG_POW;
                else
                    break;
            
        }
        
        int num_first_pos = 0;
        for (int i = 1; *(pa+i) == '0'; i++)
        {
            num_first_pos++;
            real_point_pos--;
        }
        if (num_first_pos > 0)
            arr_shift (pa+1, new_length, num_first_pos+1);

        for (int i = *new_length - 1; *(pa+i) == '0'; i--)
            *new_length -= 1;
        if (isZero(pa+1,*new_length))
            return ERROR_ZERO_DIV;
        normilize(pa+LENGTH+2,&real_power_length,real_point_pos-*new_length);
    }
    
    else
    {
        int num_first_pos = 0;
        for (int i = 1; *(pa+i) == '0'; i++)
        {
            num_first_pos++;
            real_point_pos--;
        }
        if (num_first_pos > 0)
        {
            arr_shift (pa+1, new_length, num_first_pos+1);
            *new_length -= 1;
        }
    }
    
    return OK;
}


// Функция сравнения двух массивов
int compare (char *pa, char *pb, int len)
{
    for (int i = 1; i<len; i++)
    {
        if (*(pa+i) > *(pb+i))
            return 1;
        if (*(pa+i) < *(pb+i))
            return -1;
    }
    return 0;
}

// Функция деления//делимое///делитель////результат
int division (char *pdend, char *pder, char *pres, int *dend_len, int *der_len, int *res_len)
{
    int power_diff = 1;
    int current_num = 0;
    int gap = 0;
    // Определение знака деления
    if (*pdend == *pder)
        *pres = '+';
    else
        *pres = '-';
    // Очистка полей
    for (int i = *dend_len; i <= LENGTH; i++)
    {
        //printf("-%c ", *(pdend+i));
        *(pdend+i) = '0';
        power_diff -= 1;
        //printf("+%c ", *(pdend+i));
    }
    for (int i = *der_len; i <= LENGTH; i++)
    {
        *(pder+i) = '0';
        power_diff += 1;
    }
    //for (int i = 0; i <= LENGTH; i++)
    //    printf("%c ", *(pdend+1));
    if (compare(pdend,pder,*der_len) == -1)
        power_diff -= 1;
    
    for (int j = 0; j < LENGTH+1; j++)
    {
        gap = 0;
        current_num = 0;
        //Получение нужного разряда
        for (int k = 0; compare(pdend,pder,LENGTH) == -1; k++)
        {
            if (isZero(pdend+1,LENGTH))
                break;
            gap += 1;
            if (*(pder+LENGTH) > '5')
                *(pder+LENGTH) += 1;
            
            for (int i = LENGTH-1; i > 0; i--)
            	*(pder+i+1) = *(pder+i);
            *(pder+1) = '0';
            *(dend_len) -= 1;
            if (k > 0)
            {
                *(pres+j+1) = '0';
                j+= 1;
            }
        }
        while (compare(pdend,pder,LENGTH+1) >= 0)
        {
            substr(pdend+1,pder+1,LENGTH,LENGTH,LENGTH);
            current_num += 1;
        }
            
        *(pres+j+1) = '0'+current_num;
        //printf("\n+%c", *(pres+j+1));
        *(res_len) += 1;
        
        for (int k = 0; k < LENGTH - gap; k++)
        {
            *(pdend+k+1) = *(pdend+k+1+gap);
            *(pder+k+1) = *(pder+k+1+gap);
        }
    }

    //  Округление, если требуется
    if (*(pres+LENGTH) >= '5' && *(pres+LENGTH-1) != '9')
        *(pres+LENGTH-1) += 1;
    else
    {
        if (*(pres+LENGTH) >= '5' && *(pres+LENGTH-1) == '9')
        {
            for (int i = LENGTH-1; i>=0; i--)
            {
                if (*(pres+i) == '9')
                    *(pres+i) = '0';
                else
                    if (*(pres+i) >= '0' && *(pres+i) < '9')
                    {
                        *(pres+i) += 1;
                        break;
                    }
                    else
                    {
                        *(pres+1) = '1';
                        power_diff += 1;
                    }
            }
    }
    }
    
    // Новый порядок
    if (*(pder+LENGTH+1) == '+')
        *(pder+LENGTH+1) = '-';
    else
        *(pder+LENGTH+1) = '+';


    for (int i = 0; i<POWER_LENGTH+2; i++)
        *(pres+i+LENGTH+1) = *(pder+i+LENGTH+1);
    int tmp = 0;
    
    for (int i = LENGTH+2; *(pder+i) == '0'; i++)
        tmp += 1;
    
    if (tmp > 0)
    {
        for (int i = tmp; i < POWER_LENGTH+2; i++)
        {
            *(pres+LENGTH+2+i-tmp) = *(pres+LENGTH+2+i);
            *(pres+LENGTH+2+i) = '0';
        }
    }
    
    
    tmp = POWER_LENGTH+1-tmp;

    normilize(pres+LENGTH+2, &tmp, power_diff);
    
    if (*(pres+LENGTH+2) > '0')
        return ERROR_OVERFLOW;
    
    return OK;
}

// Вывод результата
int output(char *pres)
{
   
    printf("\n             0    5    10   15   20   25   30");
    printf("\n             |----|----|----|----|----|----|");
    printf("\nРезультат: %c0.",*pres);
    
    for (int i = 1; i<LENGTH; i++)
        printf("%c",(*(pres+i)));
    printf("E");
    
    printf("%c",*(pres+LENGTH+1));
    for (int i = LENGTH+3; i<POWER_LENGTH+LENGTH+3; i++)
        printf("%c",(*(pres+i)));
    
    printf("\n");
    
    return OK;
}



int main()
{
    int error;                              // Переменная, отвечаюшая за код ошибки
    struct longinteger dividend;            // Массив, хранящий информацию о делимом
    struct longreal divider;                // Массив, хранящий информацию о делителе
    struct longreal result;                 // Массив, хранящий информацию о частном
    
    dividend.length = 1;
    divider.length = 1;
    result.length = 1;
    
    // Ввод делимого
    printf("\nВведите целое делимое без пробелов (вначале введите знак,\nзатем число длинной не более 30 символов): \n");
    printf("0    5    10   15   20   25   30\n");
    printf("|----|----|----|----|----|----|\n");
    error = read_number(&dividend.mantiss[0], 1, &dividend.length);
    
    // Проверка ошибок ввода делимого
    if (error == ERROR_WRONG_SIGN)
    {
        printf("Ошибка: Неправильный знак! Первый символ должен быть знак + или - \n");
        return OK;
    }
    if (error == ERROR_WRONG_NUMB)
    {
        printf("Ошибка: Неправильное число! Число должно состоять только из десятичных цифр и быть целым.\n");
        return OK;
    }
    if (error == ERROR_TOO_LONG)
    {
        printf("Ошибка: Слишком длинное число! Количество знаков не должно привышать 31 (1 знак и 30 цифр)!\n");
        return OK;
    }
    if (error == ERROR_EMPTY)
    {
        printf("Ошибка: Ожидался ввод числа!\n");
        return OK;
    }
    
    // Ввод делителя
    printf("\nВведите вещественный делитель без пробелов (вначале введите знак,\nзатем число в виде m.nE+/-k, где суммарная длина\nмантисы (m+n) не превышет 30 знаков, а длина порядка\nk не превышает 5 знаков): \n");
    printf("0    5    10   15   20   25   30\n");
    printf("|----|----|----|----|----|----|\n");
    error = read_number(&divider.mantiss[0], 0, &divider.length);
    
    // Проверка ошибок ввода делителя
    if (error == ERROR_WRONG_SIGN)
    {
        printf("Ошибка: Неправильный знак! Первый символ должен быть знак + или - \n");
        return OK;
    }
    if (error == ERROR_WRONG_NUMB)
    {
        printf("Ошибка: Неправильно задана мантисса числа! Она должна состоять из десятичных цифр, иметь одну точку, один знак + или - и заканчиваться символом E или e.\n");
        return OK;
    }
    if (error == ERROR_TOO_LONG)
    {
        printf("Ошибка: Слишком длинная мантисса! Количество знаков не должно привышать 32 (1 знак, 1  точка и 30 цифр, мантисса оканчивается символом E или e)!\n");
        return OK;
    }
    if (error == ERROR_LONG_POWER)
    {
        printf("Ошибка: Слишком длинный порядок! Количество знаков порядка не должно привышать 6 (1 знак и 5 цифр порядка, порядок начинается после символа E и e)\n");
        return OK;
    }
    if (error == ERROR_WRONG_POW)
    {
        printf("Ошибка: Неправильно задан порядок! Она должна состоять из десятичных цифр и знака + или -. Порядок начинается сразу после символа E или e. \n");
        return OK;
    }
    if (error == ERROR_WRONG_PSIGN)
    {
        printf("Ошибка: Неправильный знак порядка! Первый символ порядка должен быть знак + или - (пробел не допускается)\n");
        return OK;
    }
    if (error == ERROR_NO_POINT)
    {
        printf("Ошибка: Не задана точка мантиссы! Мантисса должна содержать точку согласно формату +/-m.nE+/-k, где m.n - мантисса числа\n");
        return OK;
    }
    if (error == ERROR_ZERO_DIV)
    {
        printf("Ошибка: Нельзя делить на ноль!\n");
        return OK;
    }
    if (error == ERROR_EMPTY)
    {
        printf("Ошибка: Ожидался ввод числа!\n");
        return OK;
    }
    
    
    //  Высчитывание частного
    error = division(&dividend.mantiss[0],&divider.mantiss[0],&result.mantiss[0],&dividend.length,&divider.length,&result.length);
    
    if (error == ERROR_OVERFLOW)
    {
        printf("Ошибка: В ходе вычисления произошло переполнение порядка!\n");
        return OK;
    }
    
    // Вывод результата
    output(&result.mantiss[0]);
}
