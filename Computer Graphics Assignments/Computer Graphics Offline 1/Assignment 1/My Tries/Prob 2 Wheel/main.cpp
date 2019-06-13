#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define DEBUG 0

#define pi (2*acos(0.0))
#define DEGREE_TO_RAD(x) ((x * pi) / 180)
#define RAD_TO_DEGREE(x) (x * 180 / pi)

#define INIT_X 20
#define INIT_Y 40
#define INIT_Z 50

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

struct point
{
	double x,y,z;
	void printPoint()
	{
	    printf("Point: <%lf, %lf, %lf>\n", x, y, z);
	}
};
struct point makePoint(double a, double b, double c)
{
    struct point p;
    p.x = a; p.y = b; p.z = c;
    return p;
};
/// --------------------------------------------------------------------------------

struct vect
{
    char name[10];
    double x;
    double y;
    double z;
    vect(double a, double b, double c)
    {
        x = a;
        y = b;
        z = c;
    }
    vect()
    {
        x = 0;
        y = 0;
        z = 0;
    }
    void printVector()
    {
        printf("Vector: <x = %lf, y = %lf, z = %lf>\n", x, y, z);
    }
    void printVectorWithName()
    {
        printf("Vector %s: <x = %lf, y = %lf, z = %lf>\n", name, x, y, z);
    }
    void makeVector(double a, double b, double c)
    {
        x = a;
        y = b;
        z = c;
    }
    void makeVectorWithName(double a, double b, double c, char n[10])
    {
        x = a;
        y = b;
        z = c;
        int i;
        for(i=0; i<10; i++)
        {
            name[i] = n[i];
        }
    }
};
double vector_magnitude(struct vect a)
{
    double ans = (a.x * a.x) + (a.y * a.y) + (a.z * a.z);
    return ( sqrt(ans) );
}

struct vect vectorCrossProduct(struct vect a, struct vect b)
{
    struct vect ans;
    ans.x = (a.y * b.z) - (b.y * a.z);
    ans.y = -((a.x * b.z) - (b.x * a.z));
    ans.z = (a.x * b.y) - (b.x * a.y);
    return ans;
};
struct vect vectorAddition(struct vect a, struct vect b)
{
    struct vect ans;
    ans.x = a.x + b.x;
    ans.y = a.y + b.y;
    ans.z = a.z + b.z;
    return ans;
};
struct vect vectorSubtraction(struct vect a, struct vect b)
{
    struct vect ans;
    ans.x = a.x - b.x;
    ans.y = a.y - b.y;
    ans.z = a.z - b.z;
    return ans;
};
double vectorDotProduct(struct vect a, struct vect b)
{
    return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
};
struct vect vectorScale(struct vect a, double f)
{
    struct vect ans;
    ans.x = a.x * f;
    ans.y = a.y * f;
    ans.z = a.z * f;
    return ans;
};
struct vect vectorNormalize(struct vect a)
{
    double scalar = vector_magnitude(a);
    scalar = 1.0/scalar;
    a = vectorScale(a, scalar);
    return a ;
};
/// -----------------------------------------------------------------------------------------------------------------------------------------------------

double angle_newX_wrt_oldX; ///angle of new X' with respect to old X axis
double angle_wheelX_wrt_newX;   /// angle of first quadrant of wheel (X') wrt new X' axis

struct point center_wheel;  ///To maintain the center of wheel

void initialiseParameters()
{
    angle_newX_wrt_oldX = 0;
    angle_wheelX_wrt_newX = 0;

    center_wheel = makePoint(INIT_X, INIT_Y, INIT_Z);
}

/// -----------------------------------------------------------------------------------------------------------------------------------------------------




void drawAxes()
{
    double maxLine = 150;
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( maxLine,0,0);
			glVertex3f(-maxLine,0,0);

			glVertex3f(0,-maxLine,0);
			glVertex3f(0, maxLine,0);

			glVertex3f(0,0, maxLine);
			glVertex3f(0,0,-maxLine);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	int numTimes = 15;
	double lineMax = 160;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-numTimes;i<=numTimes;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -lineMax, 0);
				glVertex3f(i*10,  lineMax, 0);

				//lines parallel to X-axis
				glVertex3f(-lineMax, i*10, 0);
				glVertex3f( lineMax, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
    double h = 0;
	glBegin(GL_QUADS);{
		glVertex3f( a, a,h);
		glVertex3f( a,-a,h);
		glVertex3f(-a,-a,h);
		glVertex3f(-a, a,h);
	}glEnd();
}

void drawCircle_custom(double initX, double initY, double initZ, double radius)
{
    int i;
    struct point points[100];

    int segments = 50;
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x = initX + radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y = initY + radius*sin(((double)i/(double)segments)*2*pi);
        points[i].z = initZ ;
    }
#if DEBUG == 1
    printf("\n\nPrinting points:\n");
    for(int i=0; i<=segments; i++){
        points[i].printPoint();
    }
    printf("\n");
#endif // DEBUG

    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x, points[i].y, points[i].z);
			glVertex3f(points[i+1].x, points[i+1].y, points[i+1].z);
        }
        glEnd();
    }
}

void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}



void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
		    if(state == GLUT_DOWN)
            {
                drawgrid = 1 - drawgrid;
            }
			//........
			break;

		default:
			break;
	}
}

///------------------------------------------------------------------------------------------------------------------


void drawCenterSmallCircle()
{
    double smallRad = 2;
    drawCircle_custom(center_wheel.x, center_wheel.y, center_wheel.z, smallRad);
}


void drawWheel()
{
    ///Every drawing shapes code comes here
    glColor3f(1, 0.3, 0.2);
    drawCenterSmallCircle();
}

///------------------------------------------------------------------------------------------------------------------


void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
//	gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

//    drawSS();
    drawWheel();    ///WHEEL drawing

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){

	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Assignment 1 - Problem 2 (1505022)");

	init();
	initialiseParameters(); ///Initializing parameters

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
