//1505020 && 1505022
#include "iGraphics.h"
#include <windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

struct h
{
    char name[30];
    int sc ;
} ;
typedef struct h hScore ; // To transfer score in binary file
static int scores_for_files[10] = {}; //10 because high scores 1 to 10 are needed
static char names_for_files[10][30] = {'\0'};
int flag = 1 ; //For special apple
int r =5, movex=10, movey=0;
int game_slower=50; // determines how slow the game will run , faster after each level
int applex=400, appley=400, appler=r;
int width=1000, height=600;
int score=0, level=1 ;
char a[100], b[100], c[100]; //Stores score lives and level
int snake[100][2]={{100,300}}, snake_len=10, gameover_var=1,game_pause=0; //snake 2d array x and y co-ordinates [0][x] and [0][y] head
int start_var=1, lives=5, current_key=1, prev_key=0;
int mode = 1 , len = 0;
char str[30] , str2[30]; //str for graphical output while typing and str2 saves name
int file_var=1;
int score_var=0;
int player = 0;
int level_up=0; //for when to add blocks

void structureScreen(); //Screen to enter name
void binary_filing();
void score_screen() //Display score board
{
    int x1 = 170 , x2 = 750 ;
    int dy = 50 , i = 0 ;
    char temp[30] ;

    iSetColor(150,150,150);
    iFilledRectangle(0,0,width,height);
    iSetColor(250,250,250);
    iFilledRectangle(0,height-50,width,50);
    iSetColor(0,150,0);
    iFilledRectangle(x1+190,height-50,273,45);
    iSetColor(50,50,50);
    iText(x1+210,height-40,"BACK (Or PRESS 'H')",GLUT_BITMAP_TIMES_ROMAN_24);

    iSetColor(250,0,0);
    iText(x1,height-40,"NAME",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(x2,height-40,"SCORE",GLUT_BITMAP_TIMES_ROMAN_24);
    for(i=1; i<=10; i++,dy+=50){
        iSetColor(0,0,90);
        iText(x1,height-40-dy,names_for_files[i],GLUT_BITMAP_TIMES_ROMAN_24);
        itoa(scores_for_files[i], temp, 10);
        iSetColor(0,90,0);
        iText(x2,height-40-dy,temp,GLUT_BITMAP_TIMES_ROMAN_24);
    }
}

void openingscr()
{
    iClear();
    iSetColor(255,255,255);
    iFilledRectangle(0,0,1000,600);
    iSetColor(255,0,0);
    iFilledRectangle(200,400,150,70);
}
void myFunction()
{
    iSetColor(50,50,50);
    iFilledRectangle(0,0,width,height);

}
void start_scr(){ //Second screen
    iClear();
    myFunction();
    iSetColor(0,180,255);
    iText(375,550,"THE GREEDY SNAKE!!",GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(200, 50, 200);
    iText(150,450, "THE SNAKE LOSES IT'S LIFE IF IT HITS THE BOUNDARY OR THE TAIL!",GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(250, 0, 150);
    iText(150,350, "PLEASE PRESS 'SPACE BAR' TO START THE GAME.",GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(0,250,0);
    iText(150,250 ,"OR PRESS 'END' BUTTON TO QUIT",GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(250,0,0);
    iText(150,150, "GRAB THE SPECIAL APPLE TO INCREASE LIFE!!",GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(150,0,150);
    iText(150,50,"PRESS 'H' BUTTON TO DISPLAY SCORE-BOARD!!",GLUT_BITMAP_TIMES_ROMAN_24);
}
void gameover(){ //when snake hits boundary or tail or any block
    int i;
    gameover_var=0;
    movex=0;
    movey=0;
    for(i=1;i<snake_len;i++){
        snake[i][0]=0;
        snake[i][1]=0;
    }
    Sleep(50);
    iClear();
    itoa(lives-1,c,10);
    if(lives-1>=1){
        iSetColor(150,0,150);
        iFilledRectangle(0,0,width,height);
        iSetColor(250, 250, 250);
        iText(410,400,"LIVES LEFT: ",GLUT_BITMAP_TIMES_ROMAN_24);
        iText(570,400,c,GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(0,200,200);
        iText(250,300,"TO CONTINUE, PRESS 'C', TO QUIT PRESS 'Q'",GLUT_BITMAP_TIMES_ROMAN_24);
    }
    else{
        iSetColor(0,0,250);
        iFilledRectangle(0,0,width,height);
        iSetColor(255,0,0);
        iText(410,400,"GAME OVER!!",GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(200, 150, 100);
        iText(240,300,"TO RESTART, PRESS 'R', TO QUIT PRESS 'Q'",GLUT_BITMAP_TIMES_ROMAN_24);
    }
}
void menu(){ //When paused
    iClear();
    iSetColor(0,0,0);
    iFilledRectangle(0,0,width,height);
    iSetColor(0,255,255);
    iFilledRectangle(250,500,500,50);
    iSetColor(0,0,20);
    iText(400, 520, "CONTINUE GAME",GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(200, 55, 250);
    iFilledRectangle(250,300,500,50);
    iSetColor(0,0,20);
    iText(435, 320, "QUIT GAME",GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(0,0,255);
    iFilledRectangle(250,100,500,50);
    iSetColor(0,0,20);
    iText(430, 120, "SCORE BOARD",GLUT_BITMAP_TIMES_ROMAN_24);
}

void eat_apple(){
    applex=((rand()%(width-50))/(2*r))*(2*r);
    appley=((rand()%(height-103))/(2*r))*(2*r);
    if(applex<=10 || appley<=10){
        eat_apple();
    }
    int i;
    for(i=0;i<snake_len;i+=1){
        if(applex == snake[i][0] && appley == snake[i][1])
            eat_apple();
    }
}
void f()
{
    flag = !flag;
}
void apple()
{
    appler = r ;
    if(score==0 || score%10){
        iSetColor(255,255,0);
        iFilledCircle(applex, appley, appler);
    }
}
void specialApple()
{
    f();
    if(((score>=1) && ((score%10)==0)))
    {
        if(flag){
            iSetColor(250,250,250);
            iFilledCircle(applex,appley,appler*1.2);
        }
    }
}

void block(){
    iSetColor(23,250,0);
    iFilledRectangle(700,300,100,50);
    iFilledRectangle(900,100,50,100);
    iFilledRectangle(100,400,100,50);

    if(snake[0][0]>=700-r && snake[0][0]<=800+r && snake[0][1]>=300-r && snake[0][1]<=350+r){
        gameover();
        PlaySound("jump19.wav",NULL,SND_SYNC);
    }
    else if(snake[0][0]>=900-r && snake[0][0]<=950+r && snake[0][1]>=100-r && snake[0][1]<=200+r){
        gameover();
        PlaySound("jump19.wav",NULL,SND_SYNC);
    }
    else if(snake[0][0]>=100-r && snake[0][0]<=200+r && snake[0][1]>=400-r && snake[0][1]<=450+r){
        gameover();
        PlaySound("jump19.wav",NULL,SND_SYNC);
    }

    if(applex>=700-2*r && applex<=900+2*r && appley>=300-2*r && appley<=350+2*r){
        eat_apple();
    }
    else if(applex>=900-2*r && applex<=950+2*r && appley>=100-2*r && appley<=300+2*r){
        eat_apple();
    }
    else if(applex>=100-2*r && applex<=300+2*r && appley>=400-2*r && appley<=450+2*r){
        eat_apple();
    }
}
void block2(){
    iSetColor(100,0,100);
    int one[10] = {400,600,200} , two[10]= {100,100,50} , l[10] = {200,50,200} , w[10] = {50,200,50};
    int i ;
    for(i=0; i<3; i++)
        iFilledRectangle(one[i],two[i],l[i],w[i]);

    for(i=0; i<3; i++){
        if(snake[0][0]>=one[i]-r && snake[0][0]<=one[i]+l[i]+r && snake[0][1]>=two[i]-r && snake[0][1]<=two[i]+w[i]+r){
            gameover();
            PlaySound("jump19.wav",NULL,SND_SYNC);
        }
    }
    for(i=0; i<3; i++)
    {
        if(applex>=one[i]-2*r && applex<=one[i]+l[i]+2*r && appley>=two[i]-2*r && appley<=two[i]+w[i]+2*r){
            eat_apple();
        }
    }
}
void block3()
{
    iSetColor(120,00,200);
    int one[10] = {400,600,200,10,750} , two[10]= {100,100,50,10,50} , l[10] = {200,50,200,150,100} , w[10] = {50,200,50,50,50};
    int i ;
    for(i=0; i<5; i++)
        iFilledRectangle(one[i],two[i],l[i],w[i]);

    for(i=0; i<5; i++){
        if(snake[0][0]>=one[i]-r && snake[0][0]<=one[i]+l[i]+r && snake[0][1]>=two[i]-r && snake[0][1]<=two[i]+w[i]+r){
            gameover();
            PlaySound("jump19.wav",NULL,SND_SYNC);
        }
    }
    for(i=0; i<5; i++)
    {
        if(applex>=one[i]-2*r && applex<=one[i]+l[i]+2*r && appley>=two[i]-2*r && appley<=two[i]+w[i]+2*r){
            eat_apple();
        }
    }
}
void block4(){
    iSetColor(10,0,10);
    int one[10] = {400,600,200,10,750,870,20} , two[10]= {100,100,50,10,50,400,210} , l[10] = {200,50,200,150,100,50,50} , w[10] = {50,200,50,50,50,100,100};
    int i ;
    for(i=0; i<10; i++)
        iFilledRectangle(one[i],two[i],l[i],w[i]);

    for(i=0; i<10; i++){
        if(snake[0][0]>=one[i]-r && snake[0][0]<=one[i]+l[i]+r && snake[0][1]>=two[i]-r && snake[0][1]<=two[i]+w[i]+r){
            gameover();
            PlaySound("jump19.wav",NULL,SND_SYNC);
        }
    }
    for(i=0; i<10; i++)
    {
        if(applex>=one[i]-2*r && applex<=one[i]+l[i]+2*r && appley>=two[i]-2*r && appley<=two[i]+w[i]+2*r){
            eat_apple();
        }
    }
}

void gameLinesAndShapes()
{
    iSetColor(204,0,200);
    iFilledRectangle(0,height-103,1000,2);
    iSetColor(0,100,10);
    iFilledRectangle(0,height-600,1000,496);

    iSetColor(100,250,250);
    iFilledRectangle(0,height-99,1000,600);
    iSetColor(204,0,200);
    iFilledRectangle(0,0,2,600);
    iFilledRectangle(998,0,2,600);
    iFilledRectangle(0,0,1000,2);

}
void reset_pos()
{
    int i;
    for(i=0;i<snake_len;i+=1){
        snake[i][0]=0;
        snake[i][1]=0;
    }
    snake[0][0]=100;
    snake[0][1]=300;
    PlaySound("Powerup4.wav",NULL,SND_SYNC);
    movex=10; movey=0;
}

void gameloop(){
    int i;

    iClear();
    gameLinesAndShapes();

    itoa(score,a,10);
    itoa(level,b,10);
    itoa(lives,c,10);

    iSetColor(250, 0, 250);
    iText(30,height-20,"LEVEL : ",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(120,height-20,b,GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(0,0,205);
    iText(400,height-20,"LIVES : ",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(490,height-20,c,GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(250,120,0);
    iText(800,height-20,"SCORE : ",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(890,height-20,a,GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(255,0,0);
    iText(340,height-70,"PRESS 'P' TO PAUSE",GLUT_BITMAP_TIMES_ROMAN_24);

    if(level==2){
        block();
        if(level_up==0){
            reset_pos();
            level_up=1;
        }
    }
    else if(level==3)
    {
        block();
        block2();
        if(level_up==1){
            reset_pos();
            level_up=0;
        }
    }
    else if(level==4)
    {

        block();
        block2();
        block3();
        if(level_up==0){
            reset_pos();
            level_up=1;
        }
    }
    else if(level>=5)
    {
        block();
        block2();
        block3();
        block4();

        if(level_up==1){
            reset_pos();
            level_up=0;
        }
    }

    iSetColor(255,20,0);

    for(i=snake_len-1;i>=1;i--){
        snake[i][0]=snake[i-1][0];
        snake[i][1]=snake[i-1][1];
    }

    snake[0][0]+=movex;
    snake[0][1]+=movey;

    for(i=0;i<snake_len;i++){
        iSetColor(255,20+i*10,0); //change color when length increases
        if(i==0){
            iFilledCircle(snake[i][0],snake[i][1],r*1.3); //head size bigger
            continue;
        }
        iFilledCircle(snake[i][0],snake[i][1],r);
    }
    iSetColor(255,255,0);
    apple();
    iSetColor(250,0,250);
    specialApple();
    iSetColor(255,255,0);

     if(snake[0][0]>=width || snake[0][0]<=0 || snake[0][1]>=(height-100) || snake[0][1]<=0){
        gameover();
        PlaySound("jump19.wav",NULL,SND_SYNC);
    }
    for(i=1;i<snake_len;i++){
        if(snake[0][0]==snake[i][0] && snake[0][1]==snake[i][1]){
            gameover();
            PlaySound("jump19.wav",NULL,SND_SYNC);
        }
    }

    if(snake[0][0]==applex && snake[0][1]==appley){
        score+=1;
        if(score%5==0 && score%10!=1){
            level+=1;
            game_slower-=2;
        }
        if(score>=10 && score%10==1)
        {
            lives+=1 ;
            game_slower-=2;
        }
        snake_len+=1;
        eat_apple();
    }
    if(lives>=5)
    {
        lives = 5 ;
    }

    switch(prev_key){
        case 1:
            current_key=0;
            break;
        case 0:
            current_key=1;
            break;
    }
    Sleep(game_slower);
}

void restart(){
    movex=10, movey=0;
    game_slower=50;
    eat_apple();
    score=0, level=1;
    snake[0][0]=100, snake[0][1]=300, snake_len=10, gameover_var=1,game_pause=0;
    start_var=1, lives=5;
}

void structureScreen()
{
    iClear();
    iSetColor(0,0,255);
    iFilledRectangle(0,0,width,height);
    iSetColor(250,250,250);
    iFilledRectangle(150,100,700,450);
    iSetColor(250,0,0);
    iText(350,height-100,"PLEASE ENTER YOUR NAME",GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(250,0,0);
    iText(480,height-150,"AND",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(340,height-200,"PRESS ENTER TO CONTINUE",GLUT_BITMAP_TIMES_ROMAN_24);

    if(mode == 1)
    {
        iSetColor(250,0,0);
        iRectangle(300,height-350,400,40);
        iSetColor(0,0,0);
        iText(420,height-340,str,GLUT_BITMAP_TIMES_ROMAN_24);
    }
}
void iDraw(){
    if(start_var==1)
    {
        iClear();
        structureScreen();
        if(player==0){
            binary_filing();
            player = 1; //player variable for calling binary_filing function only once
        }
    }
	else if(start_var==2){
        iClear();
        start_scr();
        if( score_var==1 ){
            score_screen();
        }

    }
    else{
        if(game_pause==1){
            menu();
            if( score_var==1){
                score_screen();
            }
        }

        else if(game_pause==0){
            gameloop();
        }
    }
}

void binary_filing()
{
    int x = 500 ;
    int N = x , i = 0 ;
    FILE *fp ;
    fp = fopen("HIGH SCORE.txt","ab");
    if(fp == NULL)
    {
        printf("ERROR APPENDING FILE !\n");
    }
    hScore *highScore ;
    highScore = (struct h*)malloc(N*sizeof(hScore));

    strcpy((highScore+i)->name,str2); //To store in str2
    (highScore+i)->sc = score ;
    if(fwrite(highScore+i,sizeof(hScore),1,fp)!=1)
    {
        printf("Error writing file !\n"); //error check
    }
    i++ ;
    fclose(fp);

    //FILE READ
    fp = fopen("HIGH SCORE.txt","rb");
    if(!ferror(fp))
    {
        if(fread(highScore,sizeof(hScore),N,fp)!=N)
        {
            printf("ERROR READING FILE!\n"); //error check
        }
    }
    fclose(fp); //Reading done
    int j = 0 ;
    for(j=0; j<=20; j++)
    {
        printf("Name: %s, Score: %d\n",(highScore+j)->name,(highScore+j)->sc);
        //Printing on the console for checking
    }

    printf("AFTERWARDS:\n");
    int swapped = 0 ;
    int index ;
    char tmp[30] ;
    int temp ;
    while(1)
    {
        swapped = 0 ;
        for(index=0; index<(N-1); index++)
        {
            if(((highScore+index)->sc)<((highScore+index+1)->sc))
            {
                swapped = 1 ;
                temp = ((highScore+index)->sc) ;
                strcpy(tmp,(highScore+index)->name);
                ((highScore+index)->sc) = ((index+highScore+1)->sc);
                strcpy((highScore+index)->name,(highScore+index+1)->name);
                ((highScore+index+1)->sc) = temp ;
                strcpy((highScore+index+1)->name,tmp);
            }

        }
        if(!(swapped))
            break ;
    }
    int k ;
    for(k=0; k<11; k++)
    {
        scores_for_files[k] = (highScore+k)->sc ;
        strcpy(names_for_files[k],(highScore+k)->name);
    } //To store in the global variable for graphical output

    for(k=0; k<10; k++)
    {
        printf("%2d)  %s %d\n",k+1,names_for_files[k],scores_for_files[k]);
    }   //Normal console printing for checking

    i++ ;
}
void iMouseMove(int mx, int my) {
}

void iMouse(int button, int state, int mx, int my) {  //mx , my for mouse pointer
    if(game_pause==1 && score_var==0){
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

            if(mx>= 250 && mx<= 750 && my>= 500 && my <=550){
                game_pause=0;
            }
            else if(mx>= 250 && mx<= 750 && my>= 300 && my <=350){
                binary_filing();
                exit(0);
            }
            else if(mx>= 250 && mx<= 750 && my>= 100 && my <=150){
                score_var=1;
            }
        }
    }

    else if(score_var==1){
            if((start_var>2 && game_pause==1)||(start_var==2 && game_pause!=1))
            {
                if(mx>=380 && mx<=(380+273) && my>=height-45 && my<=height)
                    score_var ^=1 ;
            }
        }
}

void iKeyboard(unsigned char key) {
    int i = 0 ;
	if(mode == 1)
	{
        //str is for graphical output of name whenever typing
        //str2 saves the name when enter is pressed
        if(key == '\b'){
                str[len-1] = '\0';
                len-- ;
                if(len<0)
                    len = 0;
        }

        else if(key == '\r')
		{
			mode = 0;
			strcpy(str2, str);
			for(i = 0; i < len; i++)
				str[i] = 0;
			len = 0;
		}

		else
		{
			str[len] = key;
			if(len<=20)
                len++;
        } //max name length 20 characters

	}

	if ((key == 'q' || key == 'Q') && gameover_var==0) {
        binary_filing();
		exit(0);
	}
	else if(start_var ==1 && key == '\r' ){
        start_var = 2;
	}
	else if(start_var==2 && key==' ' && score_var==0)
    {
        start_var += 1 ;
    }

    else if ( key == 'p' || key== 'P' ){
        game_pause^=1; //Game pause variable will flip bet one and zero etc
    }
	else if(gameover_var==0 && (key == 'c' || key == 'C') && lives>1){
        snake[0][0] = 100, snake[0][1] = 300, movex=10, movey=0;
        gameover_var=1;
        lives-=1;
        iDraw();
	}
	else if(gameover_var==0 && (key == 'r' || key == 'R') && lives==1){
        binary_filing();
        file_var=1;
        restart();
	}
	else if(start_var!=1 && ( key== 'h' || key == 'H') ){
        score_var ^= 1 ; //flip bet 0 and 1
	}
}
//For special keys iSpecialKeyboard used
void iSpecialKeyboard(unsigned char key) {
    if(gameover_var==1 && current_key!=prev_key){
        if (key == GLUT_KEY_END) {
            exit(0);
        }
        else if (key == GLUT_KEY_UP && movey!=-(2*r)) { //Stops movement in opposite direction
            movey=(2*r);
            movex=0;
        }
        else if (key == GLUT_KEY_DOWN && movey!=(2*r)) {
            movey=-(2*r);
            movex=0;
        }
        else if (key == GLUT_KEY_RIGHT && movex!=-(2*r)) {
            movex=(2*r);
            movey=0;
        }
        else if (key == GLUT_KEY_LEFT && movex!=(2*r)) {
            movex=-(2*r);
            movey=0;
        }
        prev_key=current_key;
        /*Simultaneously two buttons are stopped from being
        used before iDraw is called*/
    }
}

int main(){
	iInitialize(width, height, "MY SNAKES (1505020 && 1505022)!!");
	return 0;
}
