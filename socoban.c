#include <stdio.h>
#include <string.h>

/*=================================================================*/

#define ESC 27
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

/*=================================================================*/
//함수 정의 할 자리
/*=================================================================*/

int main()
{
	int i=0,j=0,l=0,stage;     //map[세로][가로][페이지]
	char v[30],maparr[5][30][30];
			/*맵 분리*/
	FILE *mapfile;
	mapfile = fopen("map","r");
	while (fscanf(mapfile,"%s",&v)!=EOF)
	{
		if (v[0]>='1' && v[0]<='5'){
				stage=v[0]-49;
				j=0;
				continue;
			}
		else if (v[0]=='e')
			break;
		strcpy(maparr[stage][j], v);
		j++;
	}
	for (i=0; i<5; i++){
		for (j=0; j<=30; j++)
		{
    		for (l=0; l<=30; l++)
			{
				if (maparr[i][j][l]!='\0')
					printf("%c",maparr[i][j][l]);
			}
			printf("\n");
		}
	}
	return 0;
}
