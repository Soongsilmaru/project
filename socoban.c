#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termio.h>

/*=================================================================*/

void mapsizecheck();
int mapcheck(int s,int a,int b);
void display_help(void);
void display_map(int);
int getch(); 
void move_player(int);
void key_options(int);

/*=================================================================*/

char maparr[5][30][30];
int mapsize[2][5], MAXSTAGE; //����=0 ����=1
int player_x, player_y;
int sto_x, sto_y;
int delta_x=0, delta_y=0;
int check=1;

/*=================================================================*/
int main()
{
	system("clear");
	int i=0, j=0, stage=10, result, s=0;  //map[����][����][����]
	char v[30], name[10];
			/*�� �и�*/
	FILE *mapfile;
	mapfile = fopen("map","r");
	while (fscanf(mapfile,"%s",&v)!=EOF)
	{
		if (v[0]>='1' && v[0]<='5')
		{
			stage=v[0]-49;
			if (stage>=1 && stage <=4)
			{
			mapsize[0][stage-1]=i; //�� ����ũ�� ����1,2,3,4 ����
			}
			i=0;
			continue;
		}
		else if (v[0]=='e')
		{
			mapsize[0][stage]=i;      //�� ����ũ�� ���� 5����
			break;
		}
		if (i==0)
		{
			for (s=0;s<30;s++)
			{
				if (v[s]=='\0')
				{
					mapsize[1][stage]=s;	//�� ����ũ�� ����
					break;
				}
			}
		}
		strcpy(maparr[stage][i], v);
		i++;
	}
	MAXSTAGE=stage+1;
/*=================================================================*/	
	printf("�� ���� �˻縦 �����մϴ�.\n");
	for (s=0;s<MAXSTAGE;s++)
	{
		result=mapcheck(s,mapsize[0][s],mapsize[1][s]);
	if (result==0)
		{
			printf("stage %d���� �ڽ��� ������ ��������� ������ �����ʽ��ϴ�.\n",s+1);
			return 0;
		}
	else if (result==1)
		printf("stage %d ok\n",s+1);
	}
/*=================================================================*/	
	printf("input name : ");
	scanf("%s", &name);
	system("clear");
	
	stage=1;
	
	while(check){
		
		printf("   Hello %s\n\n", name);
		display_map(stage); //�ӽ÷� ���� 1 ���.  
		key_options(stage);	
		
		}		
/*=================================================================*/	
	system("clear");
	return 0;
	
}
		
/*=================================================================*/
int getch(void){ //getch ����  
    int ch;

    struct termios buf;
    struct termios save;

    tcgetattr(0, &save);
    buf = save;

    buf.c_lflag&=~(ICANON|ECHO);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;

    tcsetattr(0, TCSAFLUSH, &buf);

    ch = getchar();
    tcsetattr(0, TCSAFLUSH, &save);

    return ch;
 }
/*=================================================================*/
int mapcheck(int s, int a,int b) //s=�������� a=����, b=����
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

void key_options(int level){
	int char_input, input_char;
	delta_x=0, delta_y=0;
	char_input = getch();
	system("clear"); //ȭ�� Ŭ���� 
	switch (char_input){
	case 'h':
		delta_x=-1;
		move_player(level);
		return;
	case 'j':
		delta_y=1;
		move_player(level);
		return;
	case 'k':
		delta_y=-1;
		move_player(level);
		return;
	case 'l':
		delta_x=1;
		move_player(level);
		return;
	case 'd':
		display_help();
		input_char = getch();
		if(input_char=='\0') { //���͸� ������ ���� ȭ������ ���ư��� ��.		
			system("clear");
			return;
		}
	case 'e': //e�� ������ ���α׷� ����. ���� ���� ���̺��ϴ� �Լ� �߰� �ʿ���. 
		check=0;
		return;
	}
	
}

/*=================================================================*/

void display_help(void){ //dŰ �Է½� ��� ���� ������. ���߿� main �Լ����� if������ �Լ� ���. 
	printf("h(����), j(�Ʒ�), k(��), l(������)\n"); //�ڵ��� �������� ���� ���پ� ���. 
	printf("u(undo)\n");
	printf("r(replay)\n");
	printf("n(new)\n");
	printf("e(exit)\n");
	printf("s(save)\n");
	printf("f(file load)\n");
	printf("d(display help)\n");
	printf("t(top)\n");
	
	printf("\n(Command) d");
}

/*=================================================================*/

void display_map(int level){
	int i, j;
	for(i=0;i<=mapsize[0][level-1];i++){ //level�� �� ��� 
			for(j=0;j<=mapsize[1][level-1];j++){
				if(maparr[level-1][i][j]=='@'){
					player_x=j;
					player_y=i;
				}
				printf("%c", maparr[level-1][i][j]);
			}
			printf("\n");
		}
}

/*=================================================================*/

void move_player(int level){
	
	if(maparr[level-1][player_y+delta_y][player_x+delta_x]=='#') return;
	if(maparr[level-1][player_y+delta_y][player_x+delta_x]=='$') return;
	
	if(sto_x==player_x&&sto_y==player_y){
		maparr[level-1][player_y][player_x]='O';
	}
	else maparr[level-1][player_y][player_x]='.';
	
	if(maparr[level-1][player_y+delta_y][player_x+delta_x]=='O'){
		sto_x=player_x+delta_x;
		sto_y=player_y+delta_y;
	}
	maparr[level-1][player_y+delta_y][player_x+delta_x]='@';
	
	return;
}

/*=================================================================*/


