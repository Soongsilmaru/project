#include <stdio.h>
#include <string.h>

/*=================================================================*/

void mapsizecheck();
int mapcheck(int s,int a,int b);

/*=================================================================*/

char maparr[5][30][30];
int mapsize[2][5], MAXSTAGE; //세로=0 가로=1

/*=================================================================*/
int main()
{
	int i=0,stage=10,result,s=0;  //map[라운드][세로][가로]
	char v[30];
			/*맵 분리*/
	FILE *mapfile;
	mapfile = fopen("map","r");
	while (fscanf(mapfile,"%s",&v)!=EOF)
	{
		if (v[0]>='1' && v[0]<='5')
		{
			stage=v[0]-49;
			if (stage>=1 && stage <=4)
			{
			mapsize[0][stage-1]=i; //맵 가로크기 측정1,2,3,4 라운드
			}
			i=0;
			continue;
		}
		else if (v[0]=='e')
		{
			mapsize[0][stage]=i;      //맵 가로크기 측정 5라운드
			break;
		}
		if (i==0)
		{
			for (s=0;s<30;s++)
			{
				if (v[s]=='\0')
				{
					mapsize[1][stage]=s;	//맵 세로크기 측정
					break;
				}
			}
		}
		strcpy(maparr[stage][i], v);
		i++;
	}
	MAXSTAGE=stage+1;
/*=================================================================*/	
	printf("맵 파일 검사를 시작합니다.\n");
	for (s=0;s<MAXSTAGE;s++)
	{
		result=mapcheck(s,mapsize[0][s],mapsize[1][s]);
	if (result==0)
		{
			printf("stage %d에서 박스의 개수와 보관장소의 개수가 같지않습니다.\n",s+1);
			return 0;
		}
	else if (result==1)
		printf("stage %d ok\n",s+1);
	}
}
		
/*=================================================================*/
int mapcheck(int s,int a,int b) //s=스테이지 a=세로, b=가로
{
	int result,i,j,target=0,box=0;
	    for (i=0;i<a;i++)
      {
          for (j=0;j<b;j++)
          {
              if (maparr[s][i][j]=='O')
                  target++;
              else if (maparr[s][i][j]=='$')
                  box++;
		  }
	  }
	if (target==box)
		result=1;
	else
		result=0;
	target=box=0;
	return result;
}	
/*=================================================================*/
