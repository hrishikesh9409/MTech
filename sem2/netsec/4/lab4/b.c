//gcc -ggdb  -fno-stack-protector -z execstack example5.c -o example5
//echo 0 > /proc/sys/kernel/randomize_va_space

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void remove_later() 
{
   printf("You have found my weakness!\n");
}

void foo() {
   
    char buf[15];
    scanf("%s",buf);
}
int main()
{
    foo();

    return 0;
}
