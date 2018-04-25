#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct tree_node   /// Структура двоичного дерева
{
    char key[20];  /// Ключ
    struct tree_node *left;  /// Указатель на левого потомка
    struct tree_node *right;  /// Указатель на правого потомка
} tree;

typedef struct tree_balanced_node
{
        char key[20];
        unsigned char height;  /// Высота
        struct tree_balanced_node* left;
        struct tree_balanced_node* right;
} tree_b;

typedef struct ht_row_open
{
    char word[20];  /// Слово
    struct ht_row_open* next;  /// Указатель на следующий элемент

} ht_row_open;

//ht_row_open **HT_open;

typedef struct ht_row_close
{
    char word[20];
} ht_row_close;

//ht_row_close *HT_close;

int main(void)
{
    printf("\nбинарное   %lu\n", sizeof(tree));

    printf("\nсбалансированное   %lu\n", sizeof(tree_b));

    printf("\nоткрытая   %lu\n", sizeof(ht_row_open));

    printf("\nзакрытая   %lu\n", sizeof(ht_row_close));

    return 0;
}