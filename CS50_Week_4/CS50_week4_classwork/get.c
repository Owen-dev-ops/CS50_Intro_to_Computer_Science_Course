#include <cs50.h>
#include <stdio.h>

int main(void)
{
    char *s = malloc(4);
    printf("s: ");
    scanf("%s", s);
    printf("s: %s\n", s);
    free(s);
}
