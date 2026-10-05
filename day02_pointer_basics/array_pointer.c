#include <stdio.h>
int main(){
	int arr[5] = {10,20,25,35,14};
	int *p=arr;
	printf("arr =%p\n",(void*)&arr);
	printf("p=%p\n",(void *)p);
	printf("*p= %d\n",*p);
	return 0;


}


