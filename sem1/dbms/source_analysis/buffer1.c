/* test buffer program */
#include <unistd.h>
 
void Test()
{
   char buff[4];
   printf("Some input: ");
   gets(buff);
   puts(buff);
}
 
int main(int argc, char *argv[ ])
{
   Test();
   return 0;
}