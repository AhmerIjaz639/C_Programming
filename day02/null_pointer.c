#include <stdio.h>
int main (){
int *p=NULL;
printf("*p=%p\n",(void *)p);
if(p==NULL){
printf("p is null cannot dereference safely.\n");
}

printf("%d\n",*p);




return 0;
}
