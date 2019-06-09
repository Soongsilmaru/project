#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termio.h>
#include <ctype.h>

/*================================함수=============================*/

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

/*=============================전역변수============================*/

char maparr[5][30][30]; //maparr[라운드][세로][가로]
int mapsize[2][5], MAXSTAGE; //세로=0 가로=1
char name[11]; //이름을 저장하는 함수
int player_x, player_y, sto_x[5][30]={0},sto_y[5][30]={0}, stonumber[5]={0}; //플레이어의 좌표, 'O'의 x좌표와 y좌표, O의 개수를 저장해주는 배열
int old_x[5], old_y[5]; //이전의 플레이어의 좌표를 저장해주는 변수
int stage; //스테이지 번호
int delta_x=0, delta_y=0; //현재 플레이어의 좌표 지정해주는 변수
int check=1,i=0;
int movecount[5]={0}; //움직인 횟수
int undo_count[5]={0}; //undo 횟수
int if_box_moved[5]={0}; //박스 움직였을 때 추가해주는 변수
int undo_boxing=0; // 박스 움직임 저장
char mapar[5][30][30]; //맵 출력
int undocheck=0; //이전에 undo를 실행했는지 판단하기 위한 변수 

/*=================================================================*/

int main() // 메인 함수, 맵 출력 시작
{
    map();
    printmapcheck();
    printname();
    return 0;
}

/*=================================================================*/
int map() //맵 출력해주는 함수
{
    system("clear");
    int i=0, j=0, stage=10, result, s=0;  //map[라운드][세로][가로]
    char v[30];
            /*맵 분리*/
    FILE *mapfile;
    mapfile = fopen("map","r"); //맵 파일을 불러와줌
    while (fscanf(mapfile,"%s",&v)!=EOF) //맵파일을 끝까지 읽을 때까지
    {
        if (v[0]>='1' && v[0]<='5') //1~5단계
   {
            stage=v[0]-49; //1의 아스키코드 값이 49임을 고려함
            if (stage>=1 && stage <=4) // 맵의 크기 측정
            {
            mapsize[0][stage-1]=i; //맵 세로크기 측정1,2,3,4 라운드
            }
            i=0;
            continue;
        }
        else if (v[0]=='e') //맵의 마지막 부분을 읽었을 때
        {
            mapsize[0][stage]=i;      //맵 세로크기 측정 5라운드
            break;
        }
        if (i==0)
        {
            for (s=0;s<30;s++) //맵의 최대 크기 30
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
int printmapcheck() // 맵 파일 검사를 위한 함수
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
    else if (result==1) // 조건 만족
        printf("stage %d ok\n",s+1);
    }
}

/*=================================================================*/
int printname() //이름 출력해 주는 함수
{
    int i=0, j=0, result, s=0, max=0;
    char v[30];
    Namecheck(); // 이름 출력 전 조건에 맞는지 확인해주는 함수
    system("clear");

    stage=1;

    while(check){
        printf("   Hello %s\n\n", name);
        display_map(stage); //맵 출력
        key_options(stage); //사용자가 키를 입력했을 때 실행되는 함수
        stage=stage+levelupcheck(stage); //스테이지 깼을 때 stage에 +1해
        }

    system("clear");
    return 0;
}
/*=================================================================*/
int Namecheck() //이름이 조건에 맞는지 확인해 주는 함수
{
    int max=0,n=0;
    printf("이름");
    scanf("%s",&name); //이름 입력 받

    for (int i=0;i<11;i++)
    {
        if (name[i]=='\0')
        {
            max=i;
            break;
        }
    }
    if (max==0) // 10글자 초과시 다시 입력 받음
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
int getch(void){ //getch 정의, 키를 입력받을 때 화면에 보이지 않고 바로 입력 받을 수 있음
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
int mapcheck(int s, int a,int b) //s=스테이지 a=세로, b=가로, 캡 체크해주는 함수
{
    int result,i,j,target=0,box=0;
        for (i=0;i<a;i++)
      {
          for (j=0;j<b;j++)
          {
              if (maparr[s][i][j]=='O'){
                  sto_x[s][target]=j; //O의 개수 저장
                  sto_y[s][target]=i;
                  target++;
              }
              else if (maparr[s][i][j]=='$')
                  box++; //박스의 개수 추가해줌
          }
      }
    stonumber[s]=target;
    if (target==box) //타겟과 박스의 개수가 같을 경우
        result=1;
    else
        result=0;
    target=box=0;
    return result;
}
/*=================================================================*/

void key_options(int level){ //사용자에게 입력받은 키를 정의해주는 함수
    undo_boxing=0;
    int char_input, input_char;
    delta_x=0, delta_y=0; // 플레이어의 현재 위치
    char_input = getch(); // 키 입려받음
    system("clear"); //화면 클리어
    if(char_input!='u') undocheck=0;
    switch (char_input){
    case 'h': //왼쪽
        delta_x=-1; //현재 위치에 -1을 해줌
        move_player(level); //플레이어 움직이도록 함수 호출
        return;
    case 'j': //아래
        delta_y=1;
        move_player(level);
        return;
    case 'k':// 위
        delta_y=-1;
        move_player(level);
        return;
    case 'l': //오른쪽
        delta_x=1;
        move_player(level);
        return;
    case 'u': //undo 실행
        if(undo_count[level-1]>=5||old_x==0||old_y==0) return;//최대 5번까지만 가능
		undocheck++;
		undo_count[level-1]++;
		delta_x=old_x[undocheck-1]-player_x;//이전 위치에서 플레이어 위치를 빼줌. (위치 초기화
		delta_y=old_y[undocheck-1]-player_y;
		if(if_box_moved[undocheck-1]) undo_boxing=1;
		move_player(level);
		return;
    case 'r': //게임 다시 시작
        map(); //맵 출력
        return;
    case 'n': // 첫 번째 맵부터 다시 시작
        for (int k=0; k<=4; k++)
        {
            movecount[k]=0; //움직인 횟수 초기화
            undo_count[k]=0; //undo 횟수 초기화
        }
        stage=0;
        map();
        save(level);
        return;
    case 'd': //명령어 내용을 보여주는 함수
        display_help();
        while(1){
            input_char = getch();
                if(input_char=='\n') { //엔터를 누르면 원래 화면으로 돌아가게 함.
                    system("clear");
                    return;
                }
        }
    case 's': //현재 움직임까지 저장
        save(level);
        return;
    case 'f': //sokoban 파일에서 저장된 내용을 읽어 save 시점부터 이어서 게임하게 해줌
    	undocheck=0;
        load();
        return;
    case 'e': //e를 누르면 프로그램 종료.
        check=0;
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

void display_map(int level){ //맵을 출력해 주는 함수
    int i, j;
    for(i=0;i<=mapsize[0][level-1];i++){ //level번 맵 출력
            for(j=0;j<=mapsize[1][level-1];j++){
                if(maparr[level-1][i][j]=='@') //플레이어의 위치 찾기
                {
                    player_x=j;
                    player_y=i;
                }
                printf("%c", maparr[level-1][i][j]);
            }
            printf("\n");
        }
    printf("움직인 횟수: %d     undo 횟수 : %d", movecount[level-1], undo_count[level-1]); //화면에 움직인 횟수와 undo 횟수 출력(수가 바뀔 때마다 계속 출력해줌)
}

/*=================================================================*/

void move_player(int level) //플레이어 움직이는 함수(박스 움직임)
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

    if (maparr[level-1][player_y+delta_y][player_x+delta_x]=='#') //플레이어가 이동하려는 좌표가 벽일 경우 이동할 수 없음 
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
  if(maparr[level-1][player_y+delta_y][player_x+delta_x]=='$') //플레이어가 이동하려는 좌표에 박스가 있을 때
    {
        if (maparr[level-1][player_y+delta_y*2][player_x+delta_x*2]=='#') //벽으로 이동했을 경우 움직이지 않음
            return;
        if (maparr[level-1][player_y+delta_y*2][player_x+delta_x*2]=='$') //박스가 연달아 있을 경우 움직이지 않음
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
        maparr[level-1][player_y][player_x]='.'; //공백으로 채워줌
    }

    maparr[level-1][player_y+delta_y][player_x+delta_x]='@';
    return;
}


/*================================================================================*/

void save(int level){ //save 해주는 함수, sokoan 파일에 저장함
    FILE *SAVE;
    SAVE=fopen("sokoban","w");
    int i, j;

    fprintf(SAVE,"name\n%s\n", name); //이름 저장
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

void load(){ // f옵션 실행 시 sokoban.txt 파일 내용을 현재 게임 상황 적용시킴
    char v[30];
    int i=0, level;
    FILE *LOAD;
 LOAD=fopen("sokoban","r"); //sokoban 파일 불러오기
    while(fscanf(LOAD,"%s",&v)!=EOF){
        if(v[0]=='n'){
            fscanf(LOAD,"%s",&v);
            strcpy(name, v); //이름 복사
        }
        if(v[0]=='l'){
            fscanf(LOAD,"%s",&v);
            level=v[0]-'1';
        }
        if(v[0]=='M'){
            while(1){
                fscanf(LOAD,"%s",&v);
                if(v[0]=='e') break;
                strcpy(maparr[level][i], v); //레벨 복사
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

int levelupcheck(int level){ //상자가 주어진 곳에 다 넣어졌을 때 다음 레벨로 넘어가는 함수
    int i=0,x,y;
    for(i=0;i<stonumber[level-1];i++){
        x=sto_x[level-1][i];
        y=sto_y[level-1][i];
        if(maparr[level-1][y][x]!='$') return 0;
    }
    return 1;
}

/*=================================================================*/
