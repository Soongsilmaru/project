#include<stdio.h>

void move(){

}


int main(){

	int i=1, j=1, level=1, stage, a[5][30][30]={}, maxX[5]={0}, maxY[5]={0};

	int p_x[5], p_y[5];

	FILE* fp;
	fp = fopen("map", "r");
	while(fscanf(fp, "%c", &a[level][i][j])!=EOF){
		if(a[level][i][j]=='@'){ //�÷��̾� ��ġ 
			p_x[level]=j;
			p_y[level]=i;
		}	
		if(a[level][i][j]=='e'){
			maxY[level]=i-1;
			break;
		}
		if(('1'<=a[level][i][j]&&a[level][i][j]<='9')){
			stage=a[level][i][j]-'1'+1;
			maxY[level]=i-1; //�ִ� Y��(=Y���� ����) 
			level=stage;
			
			i=1;
			fscanf(fp, "%c", &a[level][i][j]);
			if(a[level][i][j]==EOF) break;
			continue;
		}
		if(a[level][i][j]=='\n'){ //����(�ٹٲ�)�� ���� ������ y�� 1�� ����. 
			i++;
			maxX[level]=j-1; //�ִ� X��(=X���� ����)  
			j=1;

			continue;
		}
		
		j++;
	}


	for(int k=1; k<=5;k++) printf("Map X : %d, Y : %d // Player X : %d, Y : %d\n", maxX[k], maxY[k], p_x[k], p_y[k]);



	fclose(fp);
	return 0;
}
