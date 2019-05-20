#include <stdio.h>

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
	int i=0,j,l,map[30][30][5];      //map[세로][가로][페이지]

			/*맵 분리*/
	FILE *mapfile;
	mapfile = fopen("map","r");
	for ( ;i<=5; ){	
		for (l=0; l<=30; l++){
			for (j=0; j<=30; j++){
				fscanf(mapfile,"%c",&map[j][l][i]);
				if (map[j][l][i]>=48 && map[j][l][i]<=57)
					i++;}}}
	for (l=0; l<=30; l++){
                for (j=0; j<=30; j++){
		 	printf("%c",map[j][l][1]);}}
	printf("\n");
	for (l=0; l<=30; l++){
                for (j=0; j<=30; j++){
                        printf("%c",map[j][l][2]);}}
        printf("\n");
	for (l=0; l<=30; l++){
                for (j=0; j<=30; j++){
                        printf("%c",map[j][l][3]);}}
        printf("\n");

	return 0;
}
