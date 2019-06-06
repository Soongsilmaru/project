#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termio.h>

/*================================�Լ�=============================*/

void mapsizecheck();
int mapcheck(int s,int a,int b);
void display_help(void);
void display_map(int);
int getch(); 
void move_player(int);
void key_options(int);
int map();
int printmapcheck();
int printname();
void save(int);
void load();

/*=============================��������============================*/

char maparr[5][30][30]; //maparr[����][����][����]
int mapsize[2][5], MAXSTAGE; //����=0 ����=1
char name[10];
int player_x, player_y, sto_x[30]={0},sto_y[30]={0};
int old_x, old_y;
int delta_x=0, delta_y=0;
int check=1,i=0;
int movecount=0;
int undo_count=0;
int if_box_moved=0;
int undo_boxing=0;
char mapar[5][30][30];

/*=================================================================*/

int main()
{
	map();
	printmapcheck();
	printname();
	return 0;
}

/*=================================================================*/
int map()
{
	system("clear");
	int i=0, j=0, stage=10, result, s=0;  //map[����][����][����]
	char v[30];
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
}

/*=================================================================*/
int printmapcheck()
{
	int i=0, j=0, stage=10, result, s=0;  //map[����][����][����]
	char v[30];
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
}

/*=================================================================*/
int printname()
{
	int i=0, j=0, stage=10, result, s=0;
	char v[30];
	printf("input name : ");
	scanf("%s", &name);
	system("clear");
	
	stage=1;
	
	while(check){
		printf("   Hello %s\n\n", name);
		display_map(stage); //�ӽ÷� ���� 1 ���.  
		key_options(stage);	
		}		
	
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
	static int undocheck=0;
	undo_boxing=0;
	int char_input, input_char;
	delta_x=0, delta_y=0;
	char_input = getch();
	system("clear"); //ȭ�� Ŭ���� 
	if(char_input!='u') undocheck=0;
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
	case 'u':
		if(undo_count>=5||undocheck||old_x==0||old_y==0) return;
		undocheck=1;
		undo_count++;
		delta_x=old_x-player_x;
		delta_y=old_y-player_y;
		if(if_box_moved) undo_boxing=1;
		move_player(level);
		return;
	case 'd':
		display_help();
		while(1){
			input_char = getch();
				if(input_char=='\n') { //���͸� ������ ���� ȭ������ ���ư��� ��.		
					system("clear");
					return;
				}
		}
	case 's':
		save(level);
		check=0;
		return;
	case 'f':
		load();
		return;
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
				if(maparr[level-1][i][j]=='@')
				{
					player_x=j;
					player_y=i;
				}
				printf("%c", maparr[level-1][i][j]);
			}
			printf("\n");
		}
	printf("������ Ƚ��: %d     undo Ƚ�� : %d", movecount, undo_count);
}

/*=================================================================*/

void move_player(int level)
{	
	old_x=player_x;
	old_y=player_y;
	int j;
	
	if (maparr[level-1][player_y+delta_y][player_x+delta_x]=='#')
		return;
		
	if(undo_boxing){
		maparr[level-1][player_y][player_x]='$';
			for(j=0;j<=i;j++){	
				if (sto_x[j]==player_x-delta_x && sto_y[j]==player_y-delta_y){
					maparr[level-1][player_y-delta_y][player_x-delta_x]='O';
					break;
				}
				else
				maparr[level-1][player_y-delta_y][player_x-delta_x]='.';
			}
		maparr[level-1][player_y+delta_y][player_x+delta_x]='@';
		movecount++;
		return;
	}

	if(maparr[level-1][player_y+delta_y][player_x+delta_x]=='$') 
	{
		if (maparr[level-1][player_y+delta_y*2][player_x+delta_x*2]=='#')
			return;
		if (maparr[level-1][player_y+delta_y*2][player_x+delta_x*2]=='$')
			return;
		if (maparr[level-1][player_y+delta_y*2][player_x+delta_x*2]=='.')
		{
			maparr[level-1][player_y+delta_y*2][player_x+delta_x*2]='$';
			maparr[level-1][player_y+delta_y][player_x+delta_x]='.';
		}
		if (maparr[level-1][player_y+delta_y*2][player_x+delta_x*2]=='O')
		{
			maparr[level-1][player_y+delta_y*2][player_x+delta_x*2]='$';
			maparr[level-1][player_y+delta_y][player_x+delta_x]='.';
				sto_x[i]=player_x+delta_x*2;
				sto_y[i]=player_y+delta_y*2;
				i++;
		}
		if_box_moved=1;
	}else if_box_moved=0;
	
	for(j=0;j<=i;j++)
	{	
		if (sto_x[j]==player_x && sto_y[j]==player_y)
		{
			maparr[level-1][player_y][player_x]='O';
			break;
		}
		else
		maparr[level-1][player_y][player_x]='.';
	}
	
	if(maparr[level-1][player_y+delta_y][player_x+delta_x]=='O')
	{
		sto_x[i]=player_x+delta_x;
		sto_y[i]=player_y+delta_y;
		i++;
	}

	maparr[level-1][player_y+delta_y][player_x+delta_x]='@';
	movecount++;
	return;
}

/*=================================================================*/

void save(int level){
	FILE *SAVE;
	SAVE=fopen("sokoban","w");
	int i, j;
	
	fprintf(SAVE,"name\n%s\n", name);
	fprintf(SAVE,"level\n%d\n", level); //level ���� 
	fprintf(SAVE,"Map\n");
	for(i=0;i<mapsize[0][level-1];i++){ //level�� �� ������� ���� 
			for(j=0;j<=mapsize[1][level-1];j++){
				fprintf(SAVE,"%c", maparr[level-1][i][j]);
			}
			fprintf(SAVE,"\n");
		}
	fprintf(SAVE,"end\n");

	fprintf(SAVE,"movecount\n%d\n", movecount); //������ Ƚ�� ����
	fprintf(SAVE,"undocount\n%d\n", undo_count); //�� Ƚ�� ���� 
	
	fprintf(SAVE,"e");
	return;
}

/*=================================================================*/

void load(){
	char v[30];
	int i=0, level;
	FILE *LOAD;
	LOAD=fopen("sokoban","r");
	while(fscanf(LOAD,"%s",&v)!=EOF){
		if(v[0]=='n'){
			fscanf(LOAD,"%s",&v);
			strcpy(name, v);
		}
		if(v[0]=='l'){
			fscanf(LOAD,"%s",&v);
			level=v[0]-'1';
		}
		if(v[0]=='M'){
			while(1){
				fscanf(LOAD,"%s",&v);
				if(v[0]=='e') break;
				strcpy(maparr[level][i], v);
				i++;
			} 
		}
		if(v[0]=='m'){
			fscanf(LOAD,"%d",&v);
			movecount=v[0];
		}
		if(v[0]=='u'){
			fscanf(LOAD,"%d",&v);
			undo_count=v[0];
		}
	}
}
