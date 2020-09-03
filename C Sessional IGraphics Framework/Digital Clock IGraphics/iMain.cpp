# include "iGraphics.h"
int x = 300, y = 300, r = 20;
int w = 100 , h = 10 , k = 5 ;
int H = 80 , V = 450 ;
int delx = 5 , dely = 5 ;
int dx = 5 , dy = 5 ;
void myNine(int hor , int ver);
void myEight(int hor , int ver);
void mySeven(int hor , int ver);
void mySix(int hor , int ver);
void myFive(int hor , int ver);
void myFour(int hor , int ver);
void myThree(int hor , int ver);
void myTwo(int hor , int ver);
void myOne(int hor , int ver);
void myZero(int hor , int ver);
void fillers();
void gettime(int t[]);
void iDraw() {
    //function iDraw() is called again and again by the system.
	//place your drawing codes here
	iClear();
	//iSetColor(255,255,255);
	int botx[6] = {H , (3*H) , (7*H) , (9*H) , (13*H) , (15*H)} ;
	int boty[6] = {V,V,V,V,V,V} ;
	iSetColor(0, 160, 10);
	iText(550, 70, "MY DIGITAL CLOCK ( 1505022 )",GLUT_BITMAP_TIMES_ROMAN_24);
	iSetColor(255,70,00);
	//iSetColor(255,0,00);
    int i , j , t[6] , T[6] = {} ;
    gettime(t);
    T[0] = t[0]/10 ;
    T[1] = t[0]%10 ;
    T[2] = t[1]/10 ;
    T[3] = t[1]%10 ;
    T[4] = t[2]/10 ;
    T[5] = t[2]%10 ;
    for(i=0; i<6; i++)
    {
        if(T[i]==0)
            myZero(botx[i],V);
        else if(T[i]==1)
            myOne(botx[i],V);
        else if(T[i]==2)
            myTwo(botx[i],V);
        else if(T[i]==3)
            myThree(botx[i],V);
        else if(T[i]==4)
            myFour(botx[i],V);
        else if(T[i]==5)
            myFive(botx[i],V);
        else if(T[i]==6)
            mySix(botx[i],V);
        else if(T[i]==7)
            mySeven(botx[i],V);
        else if(T[i]==8)
            myEight(botx[i],V);
        else if(T[i]==9)
            myNine(botx[i],V);
    }
    if(T[5]%2)
    {
        fillers();
    }
}
void gettime(int t[]) {
    time_t ctime;
    struct tm * current;
    time ( &ctime );
    current = localtime ( &ctime );
    t[0] = current->tm_hour;
    t[1] = current->tm_min;
    t[2] = current->tm_sec;
}
void myEight(int hor , int ver)
{// w >>> h
    int botx = hor , boty = ver ;
    iFilledRectangle(botx,boty,w,h);    //top
    iFilledRectangle(botx,boty-dely-w,h,w);   //top left
    iFilledRectangle(botx+w-h,boty-dely-w,h,w); //top right
    iFilledRectangle(botx+h,boty-(2*dely)-w-h,w-(2*h),h); //mid
    //iFilledRectangle(botx,boty,w,h);    //top
    boty = boty - (2*dely) - w - h ;
    iFilledRectangle(botx,boty-dely-w,h,w);   //bot left
    iFilledRectangle(botx+w-h,boty-dely-w,h,w); //bot right
    iFilledRectangle(botx,boty-(2*dely)-w-h,w,h); //bottom
}
void myZero(int hor , int ver)
{
    int botx = hor , boty = ver ;
    iFilledRectangle(botx,boty,w,h);    //top
    iFilledRectangle(botx,boty-dely-w,h,w);   //top left
    iFilledRectangle(botx+w-h,boty-dely-w,h,w); //top right
    //iFilledRectangle(botx+h,boty-(2*dely)-w-h,w-(2*h),h); //mid
    //iFilledRectangle(botx,boty,w,h);    //top
    boty = boty - (2*dely) - w - h ;
    iFilledRectangle(botx,boty-dely-w,h,w);   //bot left
    iFilledRectangle(botx+w-h,boty-dely-w,h,w); //bot right
    iFilledRectangle(botx,boty-(2*dely)-w-h,w,h); //bottom
}
void myOne(int hor , int ver)
{
    int botx = hor , boty = ver ;
    //iFilledRectangle(botx,boty,w,h);    //top
    //iFilledRectangle(botx,boty-dely-w,h,w);   //top left
    iFilledRectangle(botx+w-h,boty-dely-w,h,w+h); //top right
    //iFilledRectangle(botx+h,boty-(2*dely)-w-h,w-(2*h),h); //mid
    //iFilledRectangle(botx,boty,w,h);    //top
    boty = boty - (2*dely) - w - h ;
    //iFilledRectangle(botx,boty-dely-w,h,w);   //bot left
    iFilledRectangle(botx+w-h,boty-dely-w,h,w+h); //bot right
    //iFilledRectangle(botx,boty-(2*dely)-w-h,w,h); //bottom
}
void myTwo(int hor , int ver)
{
    int botx = hor , boty = ver ;
    iFilledRectangle(botx,boty,w,h);    //top
    //iFilledRectangle(botx,boty-dely-w,h,w);   //top left
    iFilledRectangle(botx+w-h,boty-dely-w,h,w); //top right
    iFilledRectangle(botx+h,boty-(2*dely)-w-h,w-(2*h),h); //mid
    //iFilledRectangle(botx,boty,w,h);    //top
    boty = boty - (2*dely) - w - h ;
    iFilledRectangle(botx,boty-dely-w,h,w);   //bot left
    //iFilledRectangle(botx+w-h,boty-dely-w,h,w); //bot right
    iFilledRectangle(botx,boty-(2*dely)-w-h,w,h); //bottom
}
void myThree(int hor , int ver)
{
    int botx = hor , boty = ver ;
    iFilledRectangle(botx,boty,w,h);    //top
    //iFilledRectangle(botx,boty-dely-w,h,w);   //top left
    iFilledRectangle(botx+w-h,boty-dely-w,h,w); //top right
    iFilledRectangle(botx+h,boty-(2*dely)-w-h,w-(2*h),h); //mid
    //iFilledRectangle(botx,boty,w,h);    //top
    boty = boty - (2*dely) - w - h ;
    //iFilledRectangle(botx,boty-dely-w,h,w);   //bot left
    iFilledRectangle(botx+w-h,boty-dely-w,h,w); //bot right
    iFilledRectangle(botx,boty-(2*dely)-w-h,w,h); //bottom
}
void myFour(int hor , int ver)
{
    int botx = hor , boty = ver ;
    //iFilledRectangle(botx,boty,w,h);    //top
    iFilledRectangle(botx,boty-dely-w,h,w+h);   //top left
    iFilledRectangle(botx+w-h,boty-dely-w,h,w+h); //top right
    iFilledRectangle(botx+h,boty-(2*dely)-w-h,w-(2*h),h); //mid
    //iFilledRectangle(botx,boty,w,h);    //top
    boty = boty - (2*dely) - w - h ;
    //iFilledRectangle(botx,boty-dely-w,h,w);   //bot left
    iFilledRectangle(botx+w-h,boty-dely-w,h,w); //bot right
    //iFilledRectangle(botx+w-h,boty-dely-w-w,h,h);
    //iFilledRectangle(botx,boty-(2*dely)-w-h,w,h); //bottom
}
void myFive(int hor , int ver)
{
    int botx = hor , boty = ver ;
    iFilledRectangle(botx,boty,w,h);    //top
    iFilledRectangle(botx,boty-dely-w,h,w);   //top left
    //iFilledRectangle(botx+w-h,boty-dely-w,h,w); //top right
    iFilledRectangle(botx+h,boty-(2*dely)-w-h,w-(2*h),h); //mid
    //iFilledRectangle(botx,boty,w,h);    //top
    boty = boty - (2*dely) - w - h ;
    //iFilledRectangle(botx,boty-dely-w,h,w);   //bot left
    iFilledRectangle(botx+w-h,boty-dely-w,h,w); //bot right
    iFilledRectangle(botx,boty-(2*dely)-w-h,w,h); //bottom
}
void mySix(int hor , int ver)
{
    int botx = hor , boty = ver ;
    iFilledRectangle(botx,boty,w,h);    //top
    iFilledRectangle(botx,boty-dely-w,h,w);   //top left
    //iFilledRectangle(botx+w-h,boty-dely-w,h,w); //top right
    iFilledRectangle(botx+h,boty-(2*dely)-w-h,w-(2*h),h); //mid
    //iFilledRectangle(botx,boty,w,h);    //top
    boty = boty - (2*dely) - w - h ;
    iFilledRectangle(botx,boty-dely-w,h,w);   //bot left
    iFilledRectangle(botx+w-h,boty-dely-w,h,w); //bot right
    iFilledRectangle(botx,boty-(2*dely)-w-h,w,h); //bottom
}
void myNine(int hor , int ver)
{
    int botx = hor , boty = ver ;
    iFilledRectangle(botx,boty,w,h);    //top
    iFilledRectangle(botx,boty-dely-w,h,w);   //top left
    iFilledRectangle(botx+w-h,boty-dely-w,h,w); //top right
    iFilledRectangle(botx+h,boty-(2*dely)-w-h,w-(2*h),h); //mid
    //iFilledRectangle(botx,boty,w,h);    //top
    boty = boty - (2*dely) - w - h ;
    //iFilledRectangle(botx,boty-dely-w,h,w);   //bot left
    iFilledRectangle(botx+w-h,boty-dely-w,h,w); //bot right
    iFilledRectangle(botx,boty-(2*dely)-w-h,w,h); //bottom
}
void mySeven(int hor , int ver)
{
    int botx = hor , boty = ver ;
    iFilledRectangle(botx,boty,w,h);    //top
    //iFilledRectangle(botx,boty-dely-w,h,w);   //top left
    iFilledRectangle(botx+w-h,boty-dely-w,h,w); //top right
    //iFilledRectangle(botx+h,boty-(2*dely)-w-h,w-(2*h),h); //mid
    //iFilledRectangle(botx,boty,w,h);    //top
    boty = boty - (2*dely) - w - h ;
    //iFilledRectangle(botx,boty-dely-w,h,w);   //bot left
    iFilledRectangle(botx+w-h,boty-dely-w,h,w); //bot right
    //iFilledRectangle(botx,boty-(2*dely)-w-h,w,h); //bottom
}
void fillers()
{
//    iFilledRectangle((5.5*H),V*.82,30,30);
//    iFilledRectangle((5.5*H),V*0.6,30,30);
//    iFilledRectangle((11.5*H),V*.92,30,30);
//    iFilledRectangle((11.5*H),V*0.6,30,30);
    iFilledCircle(5.65*H,V*0.65,20);
    iFilledCircle(11.5*H,V*0.65,20);
    iFilledCircle(5.65*H,V*0.85,20);
    iFilledCircle(11.5*H,V*0.85,20);
}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouseMove(int mx, int my) {
	printf("x = %d, y= %d\n",mx,my);
	//place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		//place your codes here
        printf("x = %d, y= %d\n",mx,my);
		x += 10;
		y += 10;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		//place your codes here
		x -= 10;
		y -= 10;
	}
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
	*/
void iKeyboard(unsigned char key) {
	if (key == 'q') {
		exit(0);
	}
	//place your codes for other keys here
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
	*/
void iSpecialKeyboard(unsigned char key) {

	if (key == GLUT_KEY_END) {
		exit(0);
	}
	//place your codes for other keys here
}


int main() {
	//place your own initialization codes here.
	//iSetTimer(1000,iDraw());
	iInitialize(1350, 600, "My Digtial Watch");
	iSetTimer(1000,iDraw);
	//iSetTimer(1000,fillers);
	return 0;
}
