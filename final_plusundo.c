#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termio.h>
#include <ctype.h>

/*================================�Լ�=============================*/

int mapcheck(int s, int a, int b);
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
int Namecheck();
int levelupcheck(int);

/*=============================��������============================*/

char maparr[5][30][30]; //maparr[����][����][����]
int mapsize[2][5], MAXSTAGE; //����=0 ����=1
char name[11]; //�̸��� �����ϴ� �Լ�
int player_x, player_y, sto_x[5][30]={0},sto_y[5][30]={0}, stonumber[5]={0}; //�÷��̾��� ��ǥ, 'O'�� x��ǥ�� y��ǥ, O�� ������ �������ִ� �迭
int old_x[5], old_y[5]; //������ �÷��̾��� ��ǥ�� �������ִ� ����
int stage; //�������� ��ȣ
int delta_x=0, delta_y=0; //���� �÷��̾��� ��ǥ �������ִ� ����
int check=1,i=0;
int movecount[5]={0}; //������ Ƚ��
int undo_count[5]={0}; //undo Ƚ��
int if_box_moved[5]={0}; //�ڽ� �������� �� �߰����ִ� ����
int undo_boxing=0; // �ڽ� ������ ����
char mapar[5][30][30]; //�� ���
int undocheck=0; //������ undo�� �����ߴ��� �Ǵ��ϱ� ���� ���� 

/*=================================================================*/

int main() // ���� �Լ�, �� ��� ����
{
    map();
    printmapcheck();
    printname();
    return 0;
}

/*=================================================================*/
int map() //�� ������ִ� �Լ�
{
    system("clear");
    int i=0, j=0, stage=10, result, s=0;  //map[����][����][����]
    char v[30];
            /*�� �и�*/
    FILE *mapfile;
    mapfile = fopen("map","r"); //�� ������ �ҷ�����
    while (fscanf(mapfile,"%s",&v)!=EOF) //�������� ������ ���� ������
    {
        if (v[0]>='1' && v[0]<='5') //1~5�ܰ�
   {
            stage=v[0]-49; //1�� �ƽ�Ű�ڵ� ���� 49���� �����
            if (stage>=1 && stage <=4) // ���� ũ�� ����
            {
            mapsize[0][stage-1]=i; //�� ����ũ�� ����1,2,3,4 ����
            }
            i=0;
            continue;
        }
        else if (v[0]=='e') //���� ������ �κ��� �о��� ��
        {
            mapsize[0][stage]=i;      //�� ����ũ�� ���� 5����
            break;
        }
        if (i==0)
        {
            for (s=0;s<30;s++) //���� �ִ� ũ�� 30
            {
                if (v[s]=='\0')
                {
                    mapsize[1][stage]=s;    //�� ����ũ�� ����
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
int printmapcheck() // �� ���� �˻縦 ���� �Լ�
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
    else if (result==1) // ���� ����
        printf("stage %d ok\n",s+1);
    }
}

/*=================================================================*/
int printname() //�̸� ����� �ִ� �Լ�
{
    int i=0, j=0, result, s=0, max=0;
    char v[30];
    Namecheck(); // �̸� ��� �� ���ǿ� �´��� Ȯ�����ִ� �Լ�
    system("clear");

    stage=1;

    while(check){
        printf("   Hello %s\n\n", name);
        display_map(stage); //�� ���
        key_options(stage); //����ڰ� Ű�� �Է����� �� ����Ǵ� �Լ�
        stage=stage+levelupcheck(stage); //�������� ���� �� stage�� +1��
        }

    system("clear");
    return 0;
}
/*=================================================================*/
int Namecheck() //�̸��� ���ǿ� �´��� Ȯ���� �ִ� �Լ�
{
    int max=0,n=0;
    printf("�̸�");
    scanf("%s",&name); //�̸� �Է� ��

    for (int i=0;i<11;i++)
    {
        if (name[i]=='\0')
        {
            max=i;
            break;
        }
    }
    if (max==0) // 10���� �ʰ��� �ٽ� �Է� ����
    {
        printf("10�ڰ� �ѽ��ϴ�.\n");
        return n;
    }
    for (int i=0;i<max;i++)
    {
        if (isalpha(name[i])==0)
        {
            printf("���ĺ��� �ƴմϴ�. �ٽ� �Է����ּ���.\n");
            return n;
        }
    }
    n=1;
    return n;
}
/*=================================================================*/
int getch(void){ //getch ����, Ű�� �Է¹��� �� ȭ�鿡 ������ �ʰ� �ٷ� �Է� ���� �� ����
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
int mapcheck(int s, int a,int b) //s=�������� a=����, b=����, ĸ üũ���ִ� �Լ�
{
    int result,i,j,target=0,box=0;
        for (i=0;i<a;i++)
      {
          for (j=0;j<b;j++)
          {
              if (maparr[s][i][j]=='O'){
                  sto_x[s][target]=j; //O�� ���� ����
                  sto_y[s][target]=i;
                  target++;
              }
              else if (maparr[s][i][j]=='$')
                  box++; //�ڽ��� ���� �߰�����
          }
      }
    stonumber[s]=target;
    if (target==box) //Ÿ�ٰ� �ڽ��� ������ ���� ���
        result=1;
    else
        result=0;
    target=box=0;
    return result;
}
/*=================================================================*/

void key_options(int level){ //����ڿ��� �Է¹��� Ű�� �������ִ� �Լ�
    undo_boxing=0;
    int char_input, input_char;
    delta_x=0, delta_y=0; // �÷��̾��� ���� ��ġ
    char_input = getch(); // Ű �Է�����
    system("clear"); //ȭ�� Ŭ����
    if(char_input!='u') undocheck=0;
    switch (char_input){
    case 'h': //����
        delta_x=-1; //���� ��ġ�� -1�� ����
        move_player(level); //�÷��̾� �����̵��� �Լ� ȣ��
        return;
    case 'j': //�Ʒ�
        delta_y=1;
        move_player(level);
        return;
    case 'k':// ��
        delta_y=-1;
        move_player(level);
        return;
    case 'l': //������
        delta_x=1;
        move_player(level);
        return;
    case 'u': //undo ����
        if(undo_count[level-1]>=5||old_x==0||old_y==0) return;//�ִ� 5�������� ����
		undocheck++;
		undo_count[level-1]++;
		delta_x=old_x[undocheck-1]-player_x;//���� ��ġ���� �÷��̾� ��ġ�� ����. (��ġ �ʱ�ȭ
		delta_y=old_y[undocheck-1]-player_y;
		if(if_box_moved[undocheck-1]) undo_boxing=1;
		move_player(level);
		return;
    case 'r': //���� �ٽ� ����
        map(); //�� ���
        return;
    case 'n': // ù ��° �ʺ��� �ٽ� ����
        for (int k=0; k<=4; k++)
        {
            movecount[k]=0; //������ Ƚ�� �ʱ�ȭ
            undo_count[k]=0; //undo Ƚ�� �ʱ�ȭ
        }
        stage=0;
        map();
        save(level);
        return;
    case 'd': //��ɾ� ������ �����ִ� �Լ�
        display_help();
        while(1){
            input_char = getch();
                if(input_char=='\n') { //���͸� ������ ���� ȭ������ ���ư��� ��.
                    system("clear");
                    return;
                }
        }
    case 's': //���� �����ӱ��� ����
        save(level);
        return;
    case 'f': //sokoban ���Ͽ��� ����� ������ �о� save �������� �̾ �����ϰ� ����
    	undocheck=0;
        load();
        return;
    case 'e': //e�� ������ ���α׷� ����.
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
    printf("\nEnter �Է� �� �������� �ǵ��ư�");
    printf("\n(Command) d");
}

/*=================================================================*/

void display_map(int level){ //���� ����� �ִ� �Լ�
    int i, j;
    for(i=0;i<=mapsize[0][level-1];i++){ //level�� �� ���
            for(j=0;j<=mapsize[1][level-1];j++){
                if(maparr[level-1][i][j]=='@') //�÷��̾��� ��ġ ã��
                {
                    player_x=j;
                    player_y=i;
                }
                printf("%c", maparr[level-1][i][j]);
            }
            printf("\n");
        }
    printf("������ Ƚ��: %d     undo Ƚ�� : %d", movecount[level-1], undo_count[level-1]); //ȭ�鿡 ������ Ƚ���� undo Ƚ�� ���(���� �ٲ� ������ ��� �������)
}

/*=================================================================*/

void move_player(int level) //�÷��̾� �����̴� �Լ�(�ڽ� ������)
{
    int k, j;
    
    if(undocheck==0){
		for(k=(movecount[level-1]<5?movecount[level-1]:5);k>0;k--){
			if_box_moved[k]=if_box_moved[k-1];
			old_x[k]=old_x[k-1];
			old_y[k]=old_y[k-1];
		}
	}
	
	old_x[0]=player_x;
	old_y[0]=player_y;
	
	movecount[level-1]++;

    if (maparr[level-1][player_y+delta_y][player_x+delta_x]=='#') //�÷��̾ �̵��Ϸ��� ��ǥ�� ���� ��� �̵��� �� ���� 
		 return;

    if(undo_boxing){
        maparr[level-1][player_y][player_x]='$';
            for(j=0;j<=i;j++){
                if (sto_x[level-1][j]==player_x-delta_x && sto_y[level-1][j]==player_y-delta_y){
                    maparr[level-1][player_y-delta_y][player_x-delta_x]='O';
                    break;
                }
                else
                maparr[level-1][player_y-delta_y][player_x-delta_x]='.';
            }
        maparr[level-1][player_y+delta_y][player_x+delta_x]='@';
        return;
    }
  if(maparr[level-1][player_y+delta_y][player_x+delta_x]=='$') //�÷��̾ �̵��Ϸ��� ��ǥ�� �ڽ��� ���� ��
    {
        if (maparr[level-1][player_y+delta_y*2][player_x+delta_x*2]=='#') //������ �̵����� ��� �������� ����
            return;
        if (maparr[level-1][player_y+delta_y*2][player_x+delta_x*2]=='$') //�ڽ��� ���޾� ���� ��� �������� ����
            return;
        maparr[level-1][player_y+delta_y*2][player_x+delta_x*2]='$';
        maparr[level-1][player_y+delta_y][player_x+delta_x]='.';
        if_box_moved[0]=1;
    }else if_box_moved[0]=0;

    for(j=0;j<=stonumber[level-1];j++)
    {
        if (sto_x[level-1][j]==player_x && sto_y[level-1][j]==player_y)
        {
            maparr[level-1][player_y][player_x]='O';
            break;
        }
        else
        maparr[level-1][player_y][player_x]='.'; //�������� ä����
    }

    maparr[level-1][player_y+delta_y][player_x+delta_x]='@';
    return;
}


/*================================================================================*/

void save(int level){ //save ���ִ� �Լ�, sokoan ���Ͽ� ������
    FILE *SAVE;
    SAVE=fopen("sokoban","w");
    int i, j;

    fprintf(SAVE,"name\n%s\n", name); //�̸� ����
    fprintf(SAVE,"level\n%d\n", level); //level ����
    fprintf(SAVE,"Map\n");
    for(i=0;i<mapsize[0][level-1];i++){ //level�� �� ������� ����
            for(j=0;j<=mapsize[1][level-1];j++){
                fprintf(SAVE,"%c", maparr[level-1][i][j]);
            }
            fprintf(SAVE,"\n");
        }
    fprintf(SAVE,"end\n");

    fprintf(SAVE,"movecount\n%d\n", movecount[level-1]); //������ Ƚ�� ����
    fprintf(SAVE,"undocount\n%d\n", undo_count[level-1]); //�� Ƚ�� ����

    fprintf(SAVE,"e");
    fclose(SAVE);
    return;
}

/*=================================================================*/

void load(){ // f�ɼ� ���� �� sokoban.txt ���� ������ ���� ���� ��Ȳ �����Ŵ
    char v[30];
    int i=0, level;
    FILE *LOAD;
 LOAD=fopen("sokoban","r"); //sokoban ���� �ҷ�����
    while(fscanf(LOAD,"%s",&v)!=EOF){
        if(v[0]=='n'){
            fscanf(LOAD,"%s",&v);
            strcpy(name, v); //�̸� ����
        }
        if(v[0]=='l'){
            fscanf(LOAD,"%s",&v);
            level=v[0]-'1';
        }
        if(v[0]=='M'){
            while(1){
                fscanf(LOAD,"%s",&v);
                if(v[0]=='e') break;
                strcpy(maparr[level][i], v); //���� ����
                i++;
            }
        }
        if(v[0]=='m'){
            fscanf(LOAD,"%d",&movecount[level]);
        }
        if(v[0]=='u'){
            fscanf(LOAD,"%d",&undo_count[level]);
        }
    }
    stage=level+1;
    fclose(LOAD);
}

/*=================================================================*/

int levelupcheck(int level){ //���ڰ� �־��� ���� �� �־����� �� ���� ������ �Ѿ�� �Լ�
    int i=0,x,y;
    for(i=0;i<stonumber[level-1];i++){
        x=sto_x[level-1][i];
        y=sto_y[level-1][i];
        if(maparr[level-1][y][x]!='$') return 0;
    }
    return 1;
}

/*=================================================================*/
