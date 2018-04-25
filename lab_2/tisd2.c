# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>

# define MIN 40
# define MAX 100

# define INCORRECT_INPUT -1
# define INCORRECT_TYPE_OF_TOURISM -2
# define INCORRECT_TYPE_OF_EXCURSION -3
# define INCORRECT_TYPE_OF_SEASON -4
# define INCORRECT_TYPE_OF_SPORT -5
# define LIST_IS_OVERCROWDED -100
# define MIN_NUMB_OF_LIST_ITEMS -101
# define NOT_FOUND -6

struct country {
    int ID;
    char country[30];
    double population;
    char capital[30];
    char mainland[30];
    char tourism[30];
    double price;
    union {                //////////
        struct {
            int numb_obj;
            char form[15];
        } excursions;
        struct {
            char season[10];
            double temp_air;
            double temp_water;
            double time;
        } beach;
        struct {
            char form[15];
        } sport;
    } item;
} info[MAX];

struct keys {
    int ID;
    char country[30];
}key[MAX];

int test_str(char *s);
int test_beach_season(char *s);
int test_ex_form(char *s);
int test_tourism(char *s);
int test_sport(char *s);
void Print(struct country *info, int *a, int len);


unsigned long long tick(void)
{
    unsigned long long d;
    
    __asm__ __volatile__ ("rdtsc" : "=A" (d) );
    
    return d;
}

void init_country(struct country  *info)
{
    for (int i = 0; i < 30; i++)
    {
    	info->country[i] = '\0';
    	info->capital[i] = '\0';
    	info->mainland[i] = '\0';
    	info->tourism[i] = '\0';
    }
    info->ID = 0;
    info->population = 0;
    info->price = 0;
}

void Read(FILE *f)
{
    char str[500]; 
	fscanf(f, "%s", str);
	printf("\n%s", str);
	for (int i = 0; i < 100; i++)
	    printf("\n%c", str[i]);
}

int Read_file(FILE *f, struct country *info, int *ps)    // как сделать чтобы когда был пробел, функция не переставала считывать строку
{
    int i = 0, b = 0;
    char str[200];
    
    //if(fscanf(f, "%s", str))
    if (fgets(str, 200, f))
    {
        
        char j = str[0];
        char word[30];
        int k = 0, count = 0, q = 0;
        while (j != '\0' && j != '\n')
        {
            if (j != '|')
            {
                word[k] = j;
                if (count == 1)
                    info->country[b] = j;
                if (count == 3)
                    info->capital[b] = j;
                if (count == 4)
                   info->mainland[b] = j;
                if (count == 5)
                    info->tourism[b] = j;
                if (count == 6)
                {
                    if (*ps == 1)
                        info->item.beach.season[b] = j;
                    if (*ps == 3)
                        info->item.sport.form[b] = j;
                }
                if (count == 7)
                {
                    if (*ps == 2)
                        info->item.excursions.form[b] = j;
                }
                b++;
            }
            else
            {
                word[k] = '\0';
                b = 0;
                k = -1;
                if (count == 0)
                    info->ID = atoi(word);
                if (count == 2)
                    info->population = atof(word);
                if (count == 5)
                {
                    if (strcmp(word, "beach") == 0)
                    {
                        for (int i = 0; i < 10; i++)
                            info->item.beach.season[i] = '\0';
                            
                        info->item.beach.temp_air = 0;
                        info->item.beach.temp_water = 0;
                        info->item.beach.time = 0;
                        *ps = 1;
                    }
                    if (strcmp(word, "excursions") == 0)
                    {
                        for (int i = 0; i < 15; i++)
                            info->item.excursions.form[i] = '\0';
                        info->item.excursions.numb_obj = 0;
                        *ps = 2;
                    }
                    if (strcmp(word, "sport") == 0)
                    {
                        for (int i = 0; i < 15; i++)
                            info->item.sport.form[i] = '\0';
                        *ps = 3;
                    }
                }                    	
                if (count == 6)
                {
                    if (*ps == 2)
            	        info->item.excursions.numb_obj = atoi(word);
            	}
            	if (count == 7)
            	{
                	if (*ps == 1)
                        info->item.beach.temp_air = atof(word);
                }
            	if (count == 8)
            	{
                	if (*ps == 1)
                        info->item.beach.temp_water = atof(word);
                }
            	if (count == 9)
            	{
                	if (*ps == 1)
                        info->item.beach.time = atof(word);
                }
            	if (count == 10  && *ps == 1)
                    info->price = atof(word);
                if (count == 8 && *ps == 2)
                    info->price = atof(word);
                if (count == 7 && *ps == 3)
                    info->price = atof(word);
                count++;
                //i++;
            }
        k++;
        q++;
        j = str[q];
        }
    }
    return 0;
}

void Len_Array(FILE *f, int *len)
{
    rewind(f);
    char num[128];
  	while(!feof(f))
  	{
        if(fscanf(f, "%s", num) == 1)
            *len += 1;
    }

}

void Print(struct country *info, int *a, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (info[i].country[0])
        {
            *a += 1;
		    printf("| %-2d |", *a);
		    printf(" %-2d |", info[i].ID);
		    printf("\t%-15s |", info[i].country);
		    printf("      %-6.1f     |", info[i].population);
		    printf("\t%-13s |", info[i].capital);
		    printf("\t%-13s |", info[i].mainland);
		    printf("  %-10s |", info[i].tourism);
		    if (strcmp(info[i].tourism, "beach") == 0)
            {
			    printf(" %-7s ;", info[i].item.beach.season);
			    printf(" %-3.1f ;", info[i].item.beach. temp_air);
			    printf(" %-3.1f ;", info[i].item.beach.temp_water);
		        printf(" %-4.1f |", info[i].item.beach.time);
 			}
 		    if (strcmp(info[i].tourism, "excursions") == 0)
		    {
	 		    //printf("\t     |");
			    //printf("      |      |      |");
			    printf("     %-3d ;", info[i].item.excursions.numb_obj);
			    printf("  %-15s   |", info[i].item.excursions.form);
		    }
		    if (strcmp(info[i].tourism, "sport") == 0)
 		    {
			    //printf("\t     |      |      |      |    |");
                printf("\t\t %-15s   |", info[i].item.sport.form);
 		    }
 		    printf("   %-5.1f |\n", info[i].price);
            }
    }
}

void Print_Key(struct country *info, int *a, int len, struct keys *key)
{
    int i = 0;
    for (int j = 0; j < len; j++)
    {
        for (int i = 0; i < len; i++)
        {
        	if (info[i].ID == key[j].ID)
        	{
            *a += 1;
		    printf("| %-2d |", *a);
		    printf(" %-2d |", info[i].ID);
		    printf("\t%-15s |", info[i].country);
		    printf("      %-6.1f     |", info[i].population);
		    printf("\t%-13s |", info[i].capital);
		    printf("\t%-13s |", info[i].mainland);
		    printf("  %-10s |", info[i].tourism);
		    if (strcmp(info[i].tourism, "beach") == 0)
            {
			    printf(" %-7s ;", info[i].item.beach.season);
			    printf(" %-3.1f ;", info[i].item.beach. temp_air);
			    printf(" %-3.1f ;", info[i].item.beach.temp_water);
		        printf(" %-4.1f |", info[i].item.beach.time);
 			}
 		    if (strcmp(info[i].tourism, "excursions") == 0)
		    {
	 		    //printf("\t     |");
			    //printf("      |      |      |");
			    printf("     %-3d ;", info[i].item.excursions.numb_obj);
			    printf("  %-15s   |", info[i].item.excursions.form);
		    }
		    if (strcmp(info[i].tourism, "sport") == 0)
 		    {
			    //printf("\t     |      |      |      |    |");
                printf("\t\t %-15s   |", info[i].item.sport.form);
 		    }
 		    printf("   %-5.1f |\n", info[i].price);
            }
        }
    }
}

void Key_filling(struct keys *key, struct country *info, int len)
{
    for (int i = 0; i < len; i++)
    {
        key[i].ID = info[i].ID;
        strcpy(key[i].country, info[i].country);
    }
}

int Sort(struct country *info, int len)
{
    struct country tmp;
    for (int i = 0; i < len; i++)
    {
        for (int j = i; j < len; j++)
        {
           if (strcmp(info[i].country, info[j].country) > 0)
           {
               struct country tmp = info[i];
               info[i] = info[j];
               info[j] = tmp;
           }
        }
    }
    return 0;
}

int Key_bubble(struct keys *key, int len)
{
    for (int i = 0; i < len; i++)
    {
        for (int j = i+1; j < len; j++)
        {
            if (strcmp(key[i].country, key[j].country) > 0)
            {
                struct keys tmp;
                
                tmp = key[i];
                key[i] = key[j];
                key[j] = tmp;
            }
        }
    }
    return 0;
}

void Sort_vkl(struct country *info, int len)
{
    for (int i = 1; i < len; i++)
    {
        struct country temp = info[i];
        int ind = i;
        while (ind > 0 && strcmp(info[ind-1].country, temp.country) > 0)
        {
            info[ind] = info[ind-1];
            ind--;
        }
    info[ind] = temp;
    }
}

void Key_vkl(struct keys *key, int len)
{
    for (int i = 1; i < len; i++)
    {
        struct keys tmp = key[i];
        int ind = i;
        while(ind > 0 && strcmp(key[ind-1].country, tmp.country) > 0)
        {
            key[ind] = key[ind-1];
            ind--;
        }
    key[ind] = tmp;
    }
}

int Enter(struct country *info, int *len, int *len_ID)
{
    int pass, numb = 0;
    char tmp[30];
    int err = 0;
    FILE *f;
    int pr = 0; // 1-excursions   2-beach   3-sport
    
    f = fopen("list.txt", "a");
    if (*len <= MAX)
    {
        *len_ID += 1;
        info[*len].ID = *len_ID;  
              
        printf("Input the country without spaces: ");
        scanf("%s", info[*len].country);
        err = test_str(info[*len].country);  
        printf("\n");
        if (err == -1)
            return INCORRECT_INPUT;
                   
        printf("Input population (10^6): ");
        numb = scanf("%lf", &info[*len].population);
        if (numb != 1)
            return INCORRECT_INPUT;
        printf("\n");        
        printf("Input the capital without spaces: ");
        scanf("%s", info[*len].capital);
        err = test_str(info[*len].capital);
        printf("\n");
        if (err == -1)
            return INCORRECT_INPUT;
            
        printf("Input the mainland without spaces: ");
        scanf("%s", info[*len].mainland);
        err = test_str(info[*len].mainland);
        printf("\n");
        if (err == -1)
            return INCORRECT_INPUT;
                    
        printf("Input the type of tourism (beach/excursions/sport): ");
        scanf("%s", info[*len].tourism);
        err = test_tourism(info[*len].tourism);
        printf("\n");
        if (err == -1)
            return INCORRECT_INPUT;
        else if (err == -2)
            return INCORRECT_TYPE_OF_TOURISM;       
        if (strcmp(info[*len].tourism, "excursions") == 0)
        {
            pr = 1;
            printf("Input the number of objects: ");
            numb = scanf("%d", &info[*len].item.excursions.numb_obj);
            if (numb != 1)
                return INCORRECT_INPUT;
            printf("\n");           
            printf("Input the main view (nature/history/art)");
            scanf("%s", info[*len].item.excursions.form);
            err = test_ex_form(info[*len].item.excursions.form);
            printf("\n");
            if (err == -1)
                return INCORRECT_INPUT;
            else if (err == -3)
                return INCORRECT_TYPE_OF_EXCURSION;
        }
        else if (strcmp(info[*len].tourism, "beach") == 0)
        {
            pr = 2;
            printf("Input the main season (summer/autumn/winter/spring): ");
            scanf("%s", info[*len].item.beach.season);
            err = test_beach_season(info[*len].item.beach.season);
            printf("\n");
            if (err == -1)
                return INCORRECT_INPUT;
            else if (err == -4)
                return INCORRECT_TYPE_OF_SEASON;            
            printf("Input air temperature: ");
            numb = scanf("%lf", &info[*len].item.beach.temp_air);
            if (numb != 1)
                return INCORRECT_INPUT;
            printf("\n");
            printf("Input water temperature: ");
            numb = scanf("%lf", &info[*len].item.beach.temp_water);
            if (numb != 1)
                return INCORRECT_INPUT;
            printf("\n");
            printf("Input the flight time to the country: ");
            numb = scanf("%lf", &info[*len].item.beach.time);
            if (numb != 1)
                return INCORRECT_INPUT;
            printf("\n");
        }
        else if (strcmp(info[*len].tourism, "sport") == 0)
        {
            pr = 3;
            printf("Input the sport (skiing/ascent/surfing): ");
            scanf("%s", info[*len].item.sport.form);
            err = test_sport(info[*len].item.sport.form);
            printf("\n");
            if (err == -1)
                return INCORRECT_INPUT;
            else if (err == -5)
                return INCORRECT_TYPE_OF_SPORT;
        }
        printf("Input the minimum cost of rest (10^3): ");
        numb = scanf("%lf", &info[*len].price);
        if (numb != 1)
                return INCORRECT_INPUT;
        
        fprintf(f, "%d|", info[*len].ID);
        fprintf(f, "%s|", info[*len].country);  
        fprintf(f, "%lf|", info[*len].population);
        fprintf(f, "%s|", info[*len].capital);
        fprintf(f, "%s|", info[*len].mainland);
        fprintf(f, "%s|", info[*len].tourism);
        if (pr == 1)
        {
            fprintf(f, "%d|", info[*len].item.excursions.numb_obj);
            fprintf(f, "%s|", info[*len].item.excursions.form);
        }
        else if (pr == 2)
        {
            fprintf(f, "%s|", info[*len].item.beach.season);
            fprintf(f, "%3.1lf|", info[*len].item.beach.temp_air);
            fprintf(f, "%3.1lf|", info[*len].item.beach.temp_water);
            fprintf(f, "%3.1lf|", info[*len].item.beach.time);
        }
        else if (pr == 3)
            fprintf(f, "%s|", info[*len].item.sport.form);
        fprintf(f, "%3.1lf|\n", info[*len].price);
        fclose(f);
        
        *len += 1;
        return 0;
    }
    else
        return LIST_IS_OVERCROWDED;
}

int test_sport(char *s)
{
    int i = 0;
    while (s[i] != '\0')
    {
        if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'z') || (s[i] == '_'))
            i++;
        else
            return INCORRECT_INPUT;
    }
    if (strcmp(s, "skiing") != 0 && strcmp(s, "ascent") != 0 && strcmp(s, "surfing") != 0)
        return INCORRECT_TYPE_OF_SPORT;
    else
        return 0;
}

int test_beach_season(char *s)
{
    int i = 0;
    while (s[i] != '\0')
    {
        if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'z') || (s[i] == '_'))
            i++;
        else
            return INCORRECT_INPUT;
    }
    if (strcmp(s, "summer") != 0 && strcmp(s, "autumn") != 0 && strcmp(s, "winter") != 0 && strcmp(s, "spring"))
        return INCORRECT_TYPE_OF_SEASON;
    else
        return 0;
}

int test_ex_form(char *s)
{
    int i = 0;
    while (s[i] != '\0')
    {
        if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'z') || (s[i] == '_'))
            i++;
        else
            return INCORRECT_INPUT;
    }
    if (strcmp(s, "nature") != 0 && strcmp(s, "history") != 0 && strcmp(s, "art") != 0)
        return INCORRECT_TYPE_OF_EXCURSION;
    else
        return 0;
}

int test_tourism(char *s)
{
    int i = 0;
    while (s[i] != '\0')
    {
        if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'z') || (s[i] == '_'))
            i++;
        else
            return INCORRECT_INPUT;
    }
    if (strcmp(s, "beach") != 0 && strcmp(s, "excursions") != 0 && strcmp(s, "sport") != 0)
        return INCORRECT_TYPE_OF_TOURISM;
    else
        return 0;
}

int test_str(char *s)
{
    int i = 0;
    while (s[i] != '\0')
    {
        if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'z') || (s[i] == '_') || (s[i] == '-'))
            i++;
        else
            return INCORRECT_INPUT;
    }
    return 0;
}

int Delete(struct country *info, int len, int numb)
{
    FILE *f, *f1;
    f = fopen("list.txt", "r");
    f1 = fopen("list1.txt", "w");
    char str[200], s[10];
    int count = 0;
    
    
    for (int i = 0; i < len; i++)
    {
        
        if(fscanf(f, "%s", str) == 1)
        {
            s[0] = str[0];
            
            if (str[1] != '|')
            {
                s[1] = str[1];
                if (str[2] != '|')
                   s[2] = str[2];
            }
            
            count = atoi(s);
            if (count != numb)
                fprintf(f1, "%s\n", str);
            
        }
    }
    remove("list.txt");
    rename("list1.txt", "list.txt");
    fclose(f1);
    int n = 1;
    if (len > 40)
    {   
        if (numb > 0 && numb <= len)
        {
            for (int i = 0; i < len;i++)
            {
                if (info[i].ID == numb)
                {
                    n = 1;
                    for (int j = i; j < len; j++)
                    {
                        info[j] = info[j+1];
                    }
                }
            }
            if (n == 0)
                return NOT_FOUND;
            else
                return 0;
        }
        else
            return INCORRECT_INPUT;
    }
    else
        return MIN_NUMB_OF_LIST_ITEMS;
}

void Skiing(struct country *info, int len)
{
    printf("  №    ID  \tCountry \t Population, 10^6        Capital           Mainland         Tourism   |--------------------------|  Price, 10^3");
    printf("\n|--------------------------------------------------------------------------------------------------------------------------------------------|\n");
    int a = 0;
    for (int i = 0; i < len; i++)
    {
        if (strcmp(info[i].tourism, "sport") == 0 && strcmp(info[i].item.sport.form, "skiing") == 0)
        {
            a += 1;
		    printf("| %-2d |", a);
		    printf(" %-2d |", info[i].ID);
		    printf("\t%-15s |", info[i].country);
		    printf("      %-6.1f     |", info[i].population);
		    printf("\t%-13s |", info[i].capital);
		    printf("\t%-13s |", info[i].mainland);
		    printf("  %-10s |", info[i].tourism);
		    if (strcmp(info[i].tourism, "beach") == 0)
            {
			    printf(" %-7s ;", info[i].item.beach.season);
			    printf(" %-3.1f ;", info[i].item.beach. temp_air);
			    printf(" %-3.1f ;", info[i].item.beach.temp_water);
		        printf(" %-4.1f |", info[i].item.beach.time);
 			}
 		    if (strcmp(info[i].tourism, "excursions") == 0)
		    {
	 		    //printf("\t     |");
			    //printf("      |      |      |");
			    printf("     %-3d ;", info[i].item.excursions.numb_obj);
			    printf("  %-15s   |", info[i].item.excursions.form);
		    }
		    if (strcmp(info[i].tourism, "sport") == 0)
 		    {
                printf("\t\t %-15s   |", info[i].item.sport.form);
 		    }
 		    printf("   %-5.1f |\n", info[i].price);
            }
        }
}

int Test(struct country *info, int len)
{
    int err = 0;
    for (int i = 0; i < len; i++)
    {
        if (test_str(info[i].country) == -1)
            return INCORRECT_INPUT;
        if (test_str(info[i].capital) == -1)
            return INCORRECT_INPUT;
        if (test_str(info[i].mainland) == -1)
            return INCORRECT_INPUT;
        err = test_tourism(info[i].tourism);
        if (err == -1)
            return INCORRECT_INPUT;
        else if (err == -2)
            return INCORRECT_TYPE_OF_TOURISM;
        if (strcmp(info[i].tourism, "excursions") == 0)
        {
            err = test_ex_form(info[i].item.excursions.form);
            if (err == -1)
                return INCORRECT_INPUT;
            else if (err == -3)
                return INCORRECT_TYPE_OF_EXCURSION;
        }
        if (strcmp(info[i].tourism, "beach") == 0)
        {
            err = test_beach_season(info[i].item.beach.season);
            if (err == -1)
                return INCORRECT_INPUT;
            else if (err == -4)
                return INCORRECT_TYPE_OF_SEASON;
        }
        if (strcmp(info[i].tourism, "sport") == 0)
        {
            err = test_sport(info[i].item.sport.form);
            if (err == -1)
                return INCORRECT_INPUT;
            else if (err == -5)
                return INCORRECT_TYPE_OF_SPORT;
        }
        
    }
    return 0;
}

int menu(void)
{   
    char s[10];
    int answer;
    
    printf("\n1. Add a country\n2. Delete a country\n3. Output the original table\n4. Sort the source table\n5. Sort the key table\n6. Countries where you can go skiing\n\n0. Exit\n");
    do{
        printf("\nInput the number of the selected item: ");
        scanf("%s", s);
        printf("\n");
        answer = atoi(s);
    } while (answer < 0 || answer > 6);
    return answer;
}

int main()
{
    setbuf(stdout, NULL);

    struct country info[MAX];
    struct country info_key[MAX];
    struct country info_vkl[MAX];
    struct country info_bubble[MAX];
    struct keys key[MAX];
    struct keys key_vkl[MAX];
    
    char choice = -1;
    FILE *f;
    int len = 0, pr[MAX];
    int err = 0;
    
    f = fopen("list.txt", "r");
    if (!f)
    {
        printf("Could not open list.txt because of %s\n", strerror(errno));
        return -1;
    }
    Len_Array(f, &len);
    
    if (len == 0)
    {
        printf("\nEmpty file.\n");
        return 0;
    }
    else if (len < MIN)
    {
        printf("\nMinimum number of list items 40.\n");
        return 0;
    }
    else if (len > MAX)
    {
        printf("\nThe list is overcrowded. Maximun number of list items 100.\n");
    }
    
    rewind(f);
    
    for (int i = 0; i < len; i++)
    {
        init_country(&info[i]);
        Read_file(f, &info[i], &pr[i]);
    }
    fclose(f);
    
    err = Test(info, len);
    if (err == -1 || err == -2 || err == -3 || err == -4 || err == -5)
    {
        printf("\nIncorrect data.\n");
        return -1;
    }
    
    int len_ID = len;
    
    for (int i = 0; i < len; i++)
    {
        info_bubble[i] = info[i];  // копирование таблицы
        info_key[i] = info[i];
    }
    
    double time1 = 0, time2 = 0;
    double time3 = 0, time4 = 0;
    do {
        choice = menu();
    
        if (choice == 1)
        {
            int err = 0;
            err = Enter(info_bubble, &len, &len_ID);
            info[len] = info_bubble[len];
            if (err == -1)
                printf("\nIncorrect input.\n");
            else if (err == -2)
                printf("\nIncorrect type of tourism.\n");
            else if (err == -3)
                printf("\nIncorrect type of excursions.\n");
            else if (err == -4)
                printf("\nIncorrect type of season.\n");
            else if (err == -5)
                printf("\nIncorrect type of sport.\n");
            else if (err == -100)
                printf("\nThe list is overcrowded.\n");
            else if (err == 0)
            {
                for (int i = 0; i < len; i++)
                    info_key[i] = info_bubble[i];
            }
        }
        
        else if (choice == 2)
        {
            
            int err = 0;
            
            int numb = 0;
            printf("Input the record number: ");
            scanf("%d", &numb);
                
            err = Delete(info_bubble, len, numb);
            err = Delete(info_key, len, numb);
            err = Delete(info, len, numb);
            if (err == -1)
                printf("\nIncorrect number.\n");
            else if (err == -101)
                printf("\nMinimum number of list items 40.\n");
            else if (err == -6)
                printf("\nNot found.\n");
            else
                len--;
        }
        else if (choice == 3)
        {
            int a = 0;
            printf("  №    ID  \tCountry \t Population, 10^6        Capital           Mainland         Tourism   |--------------------------|  Price, 10^3");
            printf("\n|--------------------------------------------------------------------------------------------------------------------------------------------|\n");
            Print(info_bubble, &a, len);
        }
        else if (choice == 4)
        {
            unsigned long long tb =0, te = 0;
            for(int i = 0; i < len; i++)
                info_vkl[i] = info_bubble[i];  // копирование таблицы
            tb = tick();
            Sort(info_bubble, len);
            te = tick();
            int a = 0;
            printf("  №    ID  \tCountry \t Population, 10^6        Capital           Mainland         Tourism   |--------------------------|  Price, 10^3");
            printf("\n|--------------------------------------------------------------------------------------------------------------------------------------------|\n");
            Print(info_bubble, &a, len);
            time1 = (te - tb);
            printf("\nTime of sort (bubble): %f\n", time1/1000000000);
            
            a = 0;
            tb = tick();
            Sort_vkl(info_vkl, len);
            te = tick();
            time2 = (te - tb);
            printf("\nTime of sort (insertions): %f\n", time2/1000000000);\
        }
        else if (choice == 5)
        {
            unsigned long long tb = 0, te = 0;
            
            Key_filling(key, info_key, len);
            Key_filling(key_vkl, info_key, len);
            
            int q = 0;
            
            tb = tick();
            Key_bubble(key, len);
            te = tick();
            printf("\n\n");
            int a = 0, b = 0;
            printf("  №    ID  \tCountry \t Population, 10^6        Capital           Mainland         Tourism   |--------------------------|  Price, 10^3");
            printf("\n|--------------------------------------------------------------------------------------------------------------------------------------------|\n");
            
            Print_Key(info_key, &a, len, key);
            
            
            time3 = te - tb;
            printf("\nTime of sort keys(bubble): %f\n", time3/1000000000);
            
            tb = tick();
            Key_vkl(key_vkl, len);
            te = tick();
            
            time4 = te - tb;
            printf("\nTime of sort keys(insertions): %f\n", time4/1000000000);
        }
        else if (choice == 6)
            Skiing(info, len);
        else if (choice == 0)
            break;
    } while(choice != 0);
    if (time1 != 0 && time3 != 0)
    {
        int x2 = 0, x3 = 0, x4 = 0;
        x2 = 100*time2/time1;
        x3 = 100*time3/time1;
        x4 = 100*time4/time1;
        printf("\nTime comparison:\n");
        printf("                 Bubble, c            Insertions, c\n");
        printf("Source table     100%% (%7.6f)        %d%% (%7.6f)\n", time1/1000000000, x2, time2/1000000000);
        printf("Key table        %d%% (%7.6f)         %d%% (%7.6f)\n\n", x3, time3/1000000000, x4, time4/1000000000);
    }
    return 0;
}
