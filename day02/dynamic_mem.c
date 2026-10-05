#include <stdio.h>
#include<stdlib.h>
int main(){
int *p = malloc(5*sizeof(int));
if (p==NULL) return 1;
for(int i=0;i<5;i++){
	p[i]=(i+1)*10;
	printf("p= %d\n",p[i]);
}
free(p);
p=NULL;
return 0;
}
