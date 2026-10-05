#include <stdio.h>
int main(){
        int x=10;
        int *p=&x;
        printf("x=%d\n",x);
        printf("adress of x =%p\n",(void*)&x);
        printf("p=%p\n",(void *)p);
        printf("value through p= %d\n",*p);
        *p=50;
        printf("value at x now : %d\n",x);
        return 0;
}
