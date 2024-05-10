
#include <stdio.h>
#include <stdlib.h>

void change_str(char *str)
{
    str = "World";
}

void change_str2(char **str)
{
    *str = "World";
}

void change_char(char c)
{
    c = 'C';
}

void change_str2_malloc(char **str)
{
   free(*str);
   free(str);
}

int main(void)
{
    char *str1 = "Hello, ";
    char c = 'W';
    char **tab_str = (char **)malloc(sizeof(char *) * 2);
    tab_str[0] = "Hello, ";
    tab_str[1] = NULL;
    printf("Before change_str: %s\n", str1);
    printf("Before change_char: %c\n", c);
    printf("Before change_str2: %s\n", tab_str[0]);
    change_str(str1);
    change_char(c);
    change_str2(tab_str);
    printf("After change_str: %s\n", str1);
    printf("After change_char: %c\n", c);
    printf("After change_str2: %s\n", tab_str[0]);



}