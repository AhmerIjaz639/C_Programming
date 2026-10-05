#include <stdio.h>
int main(){
	int arr[5] = {10,20,25,35,14};
	int *p=arr;
	for (int i=0; i<5;i++){
		printf("p= %p, *p %d\n",(void *)p,*p);
		p++;


}	return 0;


}
