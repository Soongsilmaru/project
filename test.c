#include<stdio.h>

int main(){

	int i=1, j=1, temp_i, temp_j;
	char a[5][30][30]={};

	FILE* fp;
	fp = fopen("maptest", "r");
	while(fscanf(fp, "%c", &a[1][i][j])!=EOF){
		if(a[1][i][j]=='\n'){
			i++;
			temp_j=j;
			j=1;
			continue;
		}
		j++;
	}

	temp_i=i;
	printf("%d\n\n", i);

	for(i=1;i<=temp_i;i++){
		for(j=1;j<=temp_j;j++){
			printf("%c", a[1][i][j]);
		}	
	}
	fclose(fp);

	return 0;
}
