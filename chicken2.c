#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termio.h>
#include <ctype.h>

/*================================함수=============================*/

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
int Namecheck();
int levelupcheck(int);
void load_rank();
void read_rank();
void arrange_rank();
void save_rank();
/*=============================전역변수============================*/

char maparr[5][30][30]; //maparr[라운드][세로][가로]
int mapsize[2][5], MAXSTAGE; //세로=0 가로=1
char name[11];
int player_x, player_y, sto_x[5][30]={0},sto_y[5][30]={0}, stonumber[5]={0};
int old_x, old_y;
int stage;
int delta_x=0, delta_y=0;
int check=1,i=0;
int movecount[5]={0};
int undo_count[5]={0};
int if_box_moved=0;
int undo_boxing=0;
char mapar[5][30][30];
int movecount_max[5]; // 랭킹 저장시 각 맵별 랭킹 개수
int movecounts[5][5]; //순위 저장용 배열
char names[5][5][11]; //순위 저장용 배열
/*=================================================================*/

int main()
{
    map();
    printmapcheck();
    printname();
	printf("\n\n\n S E E  Y O U %s...\n\n\n",name);
    return 0;
}

/*=================================================================*/
int map()
{
    system("clear");
    int i=0, j=0, stage=10, result, s=0;  //map[라운드][세로][가로]
    char v[30];
            /*맵 분리*/
    FILE *mapfile;
    mapfile = fopen("map.txt","r");
    while (fscanf(mapfile,"%s",&v)!=EOF)
    {
        if (v[0]>='1' && v[0]<='5')
          {
            stage=v[0]-49;
            if (stage>=1 && stage <=4)
            {
            mapsize[0][stage-1]=i; //맵 세로크기 측정1,2,3,4 라운드
            }
            i=0;
            continue;
        }
        else if (v[0]=='e')
        {
            mapsize[0][stage]=i;      //맵 세로크기 측정 5라운드
            break;
        }
        if (i==0)
        {
            for (s=0;s<30;s++)
            {
                if (v[s]=='\0')
                {
                    mapsize[1][stage]=s;    //맵 가로크기 측정
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
    int i=0, j=0, stage=10, result, s=0;  //map[라운드][세로][가로]
    char v[30];
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
int printname()
{
    int i=0, j=0, result, s=0, max=0;
    char v[30];
    Namecheck();
    system("clear");

    stage=1;

    while(check){
        printf("   Hello %s\n\n", name);
        display_map(stage);
           key_options(stage);
        stage=stage+levelupcheck(stage);
        }

    system("clear");
    return 0;
}
/*=================================================================*/
int Namecheck()
{
    int max=0,n=0;
    printf("이름");
    scanf("%s",&name);

    for (int i=0;i<11;i++)
    {
        if (name[i]=='\0')
        {
            max=i;
            break;
        }
    }
    if (max==0)
    {
        printf("10자가 넘습니다.\n");
        return n;
    }
    for (int i=0;i<max;i++)
    {
        if (isalpha(name[i])==0)
        {
            printf("알파벳이 아닙니다. 다시 입력해주세요.\n");
            return n;
        }
    }
    n=1;
    return n;
}
/*=================================================================*/
int getch(void){ //getch 정의
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
 int mapcheck(int s, int a,int b) //s=스테이지 a=세로, b=가로
{
    int result,i,j,target=0,box=0;
        for (i=0;i<a;i++)
      {
          for (j=0;j<b;j++)
          {
              if (maparr[s][i][j]=='O'){
                  sto_x[s][target]=j;
                  sto_y[s][target]=i;
                 // printf("sto_x[%d][%d]=%d\n",s,target,j);
                 // printf("sto_y[%d][%d]=%d\n",s,target,i);
                  target++;
              }
              else if (maparr[s][i][j]=='$')
                  box++;
          }
      }
    stonumber[s]=target;
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
    system("clear"); //화면 클리어
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
        if(undo_count[level-1]>=5||undocheck||old_x==0||old_y==0) return;
        undocheck=1;
        undo_count[level-1]++;
        delta_x=old_x-player_x;
        delta_y=old_y-player_y;
        if(if_box_moved) undo_boxing=1;
        move_player(level);
        return;
    case 'r':
     for (int k=0; k<=4; k++)
    {
        movecount[k] = 0;
        undo_count[k] = 0;
        map();
    }
        return;
    case 'd':
        display_help();
        while(1){
            input_char = getch();
                if(input_char=='\n') { //엔터를 누르면 원래 화면으로 돌아가게 함.
                    system("clear");
                    return;
                }
        }
    case 's':
        save(level);
        return;
    case 'f':
        load();
        return;
    case 'e': //e를 누르면 프로그램 종료. 파일 정보 세이브하는 함수 추가 필요함.
        check=0;
        return;

      case 't': //랭킹 출력
        read_rank(0);
        map();
        return;
    }

}
/*=================================================================*/

void display_help(void){ //d키 입력시 명령 내용 보여줌. 나중에 main 함수에서 if문으로 함수 사용.
    printf("h(왼쪽), j(아래), k(위), l(오른쪽)\n"); //코드의 가독성을 위해 한줄씩 띄움.
    printf("u(undo)\n");
    printf("r(replay)\n");
    printf("n(new)\n");
    printf("e(exit)\n");
    printf("s(save)\n");
    printf("f(file load)\n");
    printf("d(display help)\n");
    printf("t(top)\n");
    printf("\nEnter 입력 시 게임으로 되돌아감");
    printf("\n(Command) d");
}

/*=================================================================*/

void display_map(int level){
    int i, j;
    for(i=0;i<=mapsize[0][level-1];i++){ //level번 맵 출력
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
    printf("움직인 횟수: %d     undo 횟수 : %d", movecount[level-1], undo_count[level-1]);
}

/*=================================================================*/

void move_player(int level)
{
    old_x=player_x;
    old_y=player_y;
    int j;
    movecount[level-1]++;

    if (maparr[level-1][player_y+delta_y][player_x+delta_x]=='#')
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

    if(maparr[level-1][player_y+delta_y][player_x+delta_x]=='$')
    {
        if (maparr[level-1][player_y+delta_y*2][player_x+delta_x*2]=='#')
            return;
        if (maparr[level-1][player_y+delta_y*2][player_x+delta_x*2]=='$')
            return;
        maparr[level-1][player_y+delta_y*2][player_x+delta_x*2]='$';
        maparr[level-1][player_y+delta_y][player_x+delta_x]='.';
        if_box_moved=1;
    }else if_box_moved=0;

    for(j=0;j<=stonumber[level-1];j++)
    {
        if (sto_x[level-1][j]==player_x && sto_y[level-1][j]==player_y)
        {
            maparr[level-1][player_y][player_x]='O';
            break;
        }
        else;
        maparr[level-1][player_y][player_x]='.';
    }

    maparr[level-1][player_y+delta_y][player_x+delta_x]='@';
    return;
}

/*=================================================================*/
void save(int level){
    FILE *SAVE;
    SAVE=fopen("sokoban","w");
    int i, j;

    fprintf(SAVE,"name\n%s\n", name);
    fprintf(SAVE,"level\n%d\n", level); //level 저장
    fprintf(SAVE,"Map\n");
    for(i=0;i<mapsize[0][level-1];i++){ //level번 맵 현재상태 저장
            for(j=0;j<=mapsize[1][level-1];j++){
                fprintf(SAVE,"%c", maparr[level-1][i][j]);
            }
            fprintf(SAVE,"\n");
        }
    fprintf(SAVE,"end\n");

    fprintf(SAVE,"movecount\n%d\n", movecount[level-1]); //음직인 횟수 저장
    fprintf(SAVE,"undocount\n%d\n", undo_count[level-1]); //언도 횟수 저장

    fprintf(SAVE,"e");
    fclose(SAVE);
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
int levelupcheck(int level){
    int i=0,x,y;
    for(i=0;i<stonumber[level-1];i++){
        x=sto_x[level-1][i];
        y=sto_y[level-1][i];
        if(maparr[level-1][y][x]!='$') 
			return 0;
    }
	save_rank();
	load_rank();
	//arrange_rank(stonumber);
    if (stage == MAXSTAGE+1)
    {
        system("clear");
        printf("\n\n\n C L E A R! \n\n\n");
        exit(0);
    }
	return 1;
}

/*=================================================================*/

void read_rank(int num) // 랭킹 출력 함수
{
    load_rank(); //파일에서 순위 불러와서 배열에 저장
    system("clear");
    system("clear");
    printf("    Hello %s\n", name);
    if(num==0)
    {   for(i=0; i<5; i++)
        {
            if(movecount_max[i] == 0) //출력할 순위가 아예 없는 맵은 순위 출력하지 않음
            continue;
            printf("map %d\n", i+1);
            for(int j=0; j<movecount_max[j]; j++)
                printf("%s %d\n", names[i][j], movecounts[i][j]);
        }

        printf("\n\n(command) t");
    }
    else
    {   printf("map %d\n", num);
        for(int i=0; i<movecount_max[num-1]; i++)
        {
            printf("%s %d", names[num-1][i], movecounts[num-1][i]);
        }
        printf("\n\n(command) t%d", num);
    printf("\n게임으로 돌아가려면 아무 키나 누르십시오.");
    }

    if(getch())
    {
        return;
    }
}

/*===============================================================*/

void load_rank() //ranking.txt에서 순위를 읽어 배열에 저장
{
    int i, j;
    int tmp=0;
    int tmp_stagenumber;
    FILE *fp = fopen("ranking.txt", "rt");
    for(i=0; i<5; i++)
    {
           fscanf(fp, "map %d %d\n", &tmp_stagenumber, &movecount_max[i]);
        for(j=0; j<movecount_max[i]; j++)
        {
            fscanf(fp, "%s %d\n", names[i][j], movecounts[i][j]);
        }
    }
    fclose(fp);
    return;
}

/*=========================================================*/

void arrange_rank(int ar_rank) //각 맵별로 순위를 추가, 정렬 후 순위 배열에 추가하는 함수
{

    int i, j, view;
    if(movecounts[ar_rank] == 0) //순위 배열에 순위가 저장이 안 되어 있을 때
        {
            strcpy(names[ar_rank][0], name);
            movecounts[ar_rank][0] = view;
            movecount_max[ar_rank]++;
            return;
        }
    if(view>movecounts[ar_rank][movecount_max[ar_rank]-1])//저장할 움직인 횟수가 원래 저장되어 있던 움직인 횟수보다 크다면
        {
    if(movecounts[ar_rank] != 5) //이때 순위 개수 5개 미만이면 가장 뒤에 현재 움직인 횟수와 이름 저장해줌
            {
                movecounts[ar_rank][movecount_max[ar_rank]] = view;
                strcpy(names[ar_rank][movecount_max[ar_rank]], name);
                movecount_max[ar_rank]++;
            }
        }
    else //저장할 움직인 횟수가 원래 저장되어 있던 움직인 횟수보다 작으면
    {
        for(i=0;i<movecount_max[ar_rank]; i++) //반복문을 통해 view보다 큰 움직인 횟수 찾기
        {
            if(movecounts[ar_rank][i]>view)
            {
                break; //이때 view가 저장될 위치가 i로 저장됨
            }
        }
        if(movecount_max[ar_rank]!=5) //맵의 순위 개수가 5개가 아닐 때
        {
            for(j=movecount_max[ar_rank]-1;j>=i; j--) // view보다 큰 순위들을 한칸씩 뒤로 이동
            {
                strcpy(names[ar_rank][j+1], names[ar_rank][j]);
                movecounts[ar_rank][j+1]=movecounts[ar_rank][j];
            }
        strcpy(names[ar_rank][i], name);
        movecounts[ar_rank][i]=view;
        movecount_max[ar_rank]++; // 현재 맵의 순위 개수 1증가
        }
        else if(movecount_max[ar_rank]==5)
        {
            for(j=3;j>=i;j--)
            {
                strcpy(names[ar_rank][j+1], names[ar_rank][j]);
                movecounts[ar_rank][j+1]=movecounts[ar_rank][j];
            }
        strcpy(names[ar_rank][i], name);
        movecounts[ar_rank][i]=view;
        }
    }
              return;

}

/*=========================================================*/

void save_rank() //순위를 파일에 저장
{
    int i, j;
    FILE *fp = fopen("ranking.txt", "w");
    for(i=0; i<5; i++)
    {
        fprintf(fp, "map %d %d\n", i+1, movecount_max[i]);
            for(j=0; j<movecount_max[i]; j++)
            {
            fprintf(fp, "%s %d\n", names[i][j], movecounts[i][j]);
            }
    }

    fclose(fp);
    return;
}

/*========================================================*/
