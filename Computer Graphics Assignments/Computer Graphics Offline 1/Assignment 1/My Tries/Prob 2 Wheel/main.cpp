#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define DEBUG 0
#define DEBUG_CYLINDER 0

#define pi (2*acos(0.0))
#define DEGREE_TO_RAD(x) ((x * pi) / 180)
#define RAD_TO_DEGREE(x) (x * 180 / pi)

#define INIT_X -80
#define INIT_Y -40
#define INIT_Z 50

#define DEL_ANGLE 5 ///How many degrees to move while MOVING

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
double magnitude(struct point p)
{
    double ans = (p.x * p.x) + (p.y * p.y) + (p.z * p.z);
    return sqrt(ans);
}
/// -----------------------------------------------------------------------------------------------------------------------------------------------------

double angle_axis_wheel; ///angle of new X' with respect to old X axis
double angle_turnOf_wheel;   /// angle of first quadrant of wheel (X') wrt new X' axis

double center_wheel_x;
double center_wheel_y;

double radiusCylinder ;//= 25;
double heightCylinder ;//= 15;

struct point initialCenterPoint, centerPoint;

void initialiseParameters()
{
    angle_axis_wheel = 0;
    angle_turnOf_wheel = 0;

    center_wheel_x = INIT_X;
    center_wheel_y = INIT_Y;

    radiusCylinder = 30;
    heightCylinder = 15;

    initialCenterPoint.x = INIT_X;
    initialCenterPoint.y = INIT_Y;

    centerPoint.x = INIT_X;
    centerPoint.y = INIT_Y;

    printf("---------------------- After Initializing Printing parameters begin------------------------------\n\n");
    printf("Center at %lf, %lf, radiusCylinder = %lf, heightCylinder = %lf, angle_axis = %lf, angle_turn = %lf\n",
           center_wheel_x, center_wheel_y, radiusCylinder, heightCylinder, angle_axis_wheel, angle_turnOf_wheel);
    printf("---------------------- After Initializing Printing parameters done------------------------------\n\n");
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
	double lineMax = 150;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-numTimes;i<=numTimes;i++){

//				if(i==0)
//					continue;	//SKIP the MAIN axes

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
///------------------------------------------------------------------------------------------------------------------

void positionWheelOnScreen()
{
    ///First we draw the cylinder --> Draw two circles and Join them using rectangles
    double shade = 0;   ///To create shading effect
    int slices = 50;  ///Number of slices for the circle
    double r, h;
    struct point circle[100];  ///Upper circle
    struct point upperCircle[100];  ///Lower circle

    for(int i=0; i<=slices; i++){
        r = radiusCylinder;
        circle[i].x = r*cos(((double)i/(double)slices)* (2 * pi));  ///Draw wrt x-z axis
        upperCircle[i].x = circle[i].x;

        circle[i].z = r*sin(((double)i/(double)slices)* (2 * pi));
        upperCircle[i].z = circle[i].z;

        circle[i].y = 0;
        upperCircle[i].y = heightCylinder;
    }

///Now join the two circles using rectangles
    for(int i=0; i<=slices; i++){
        //create shading effect
        if(i<slices/2)shade=2*(double)i/(double)slices;
        else shade=2*(1.0-(double)i/(double)slices);
        glColor3f(shade,shade,shade);

        glBegin(GL_QUADS);
        {
            glVertex3f(circle[i%slices].x, circle[i%slices].y, circle[i%slices].z);
            glVertex3f(circle[(i + 1)%slices].x, circle[(i + 1)%slices].y, circle[(i + 1)%slices].z);
            glVertex3f(upperCircle[(i + 1)%slices].x, upperCircle[(i + 1)%slices].y, upperCircle[(i + 1)%slices].z);
            glVertex3f(upperCircle[i%slices].x, upperCircle[i%slices].y, upperCircle[i%slices].z);
        }
        glEnd();
    }

#if DEBUG_CYLINDER == 1
    printf("\n\n\nPrinting points for rectangle 1: \n");
    printf("%lf, %lf, %lf\n", circle[0].x, (heightCylinder * 0.2), circle[0].z);
    printf("%lf, %lf, %lf\n", circle[0].x, (heightCylinder * 0.8), circle[0].z);
    printf("%lf, %lf, %lf\n", circle[(slices/2)].x,(heightCylinder * 0.2), circle[(slices/2)].z);
    printf("%lf, %lf, %lf\n", circle[(slices/2)].x,(heightCylinder * 0.8), circle[(slices/2)].z);

    printf("------ ----- ");
    printf("%lf, %lf, %lf\n", circle[(slices)].x,(heightCylinder * 0.2), circle[(slices)].z);
    printf("%lf, %lf, %lf\n", circle[(slices)].x,(heightCylinder * 0.8), circle[(slices)].z);
#endif // DEBUG_CYLINDER

///Drawing the two rectangles

    ///Different color for the rectangles
    glColor3f(0.8, 0, 0);   //Red
    //Rectangle 1
    glBegin(GL_QUADS);  ///circle[0] (0 rev)  ----- circle[slices/2] (half rev) ---- circle[slices] (full rev)
    {
        glVertex3f(circle[0].x,(heightCylinder * 0.2), circle[0].z);
        glVertex3f(circle[0].x,(heightCylinder * 0.8), circle[0].z);
        glVertex3f(circle[(slices/2)].x,(heightCylinder * 0.2), circle[(slices/2)].z);
        glVertex3f(circle[(slices/2)].x,(heightCylinder * 0.8), circle[(slices/2)].z);
    }
    glEnd();

    //Rect 2 [rotate 90 degrees acw wrt y-axis
    glPushMatrix();
    {
        glRotatef(90, 0, 1, 0);
        glBegin(GL_QUADS);  //Rectangle 1 code
        {
            glVertex3f(circle[0].x,(heightCylinder * 0.2), circle[0].z);
            glVertex3f(circle[0].x,(heightCylinder * 0.8), circle[0].z);
            glVertex3f(circle[(slices/2)].x,(heightCylinder * 0.2), circle[(slices/2)].z);
            glVertex3f(circle[(slices/2)].x,(heightCylinder * 0.8), circle[(slices/2)].z);
        }
        glEnd();
    }
    glPopMatrix();

}


void drawWheel()
{
    ///Apply transformations wrt angle and positions then call the function positionWheelOnScreen()
    glPushMatrix();
    {
        //Translate wrt -ve x-axis , +ve y-axis and 0 z-axis
        glTranslatef(-1.0 * center_wheel_x, center_wheel_y, 0);
        glRotatef(angle_turnOf_wheel, 0, 0, -1);    ///Rotate with respect to down

        glTranslatef(0, -heightCylinder * 0.5, radiusCylinder); ///Translate upto the center of circle/cylinder point
        glRotatef(angle_axis_wheel, 0, -1, 0);  ///Rotate wrt -ve y axis

        positionWheelOnScreen();
    }
    glPopMatrix();
}

void moveWheel(double x_delta, double y_delta)
{
    center_wheel_x += x_delta;
    center_wheel_y += y_delta;

///For testing how much wheel has moved
    double x_del = center_wheel_x - initialCenterPoint.x;
    double y_del = center_wheel_y - initialCenterPoint.y;

    struct point p = makePoint(x_del, y_del, 0);
    double perimeter_wheel = 2 * pi * radiusCylinder;
    printf("Moved <%lf, %lf> magnitude = %lf, angle_axis_wheel = %lf, angle_turn = %lf, perimeter_wheel = %lf\n\n",
           x_del, y_del, magnitude(p), angle_axis_wheel, angle_turnOf_wheel, perimeter_wheel);
}

void moveForward()  ///w
{
    ///center should move forward i.e. with respect to -ve x-axis direction
    angle_axis_wheel = angle_axis_wheel + DEL_ANGLE;
    double x_delta = cos(DEGREE_TO_RAD(angle_turnOf_wheel)) * ( ((double)DEL_ANGLE / (double)(360.0) ) * (2 * pi * radiusCylinder));//(radiusCylinder * (pi / 180) * DEL_ANGLE);
    double y_delta = sin(DEGREE_TO_RAD(angle_turnOf_wheel)) * ( ((double)DEL_ANGLE / (double)(360.0)) * (2 * pi * radiusCylinder));//(radiusCylinder * (pi / 180) * DEL_ANGLE);
    moveWheel(x_delta, y_delta);
}
void moveBackward() ///s
{
    angle_axis_wheel = angle_axis_wheel - DEL_ANGLE;
    double x_delta = cos(DEGREE_TO_RAD(angle_turnOf_wheel)) * ( ((double)DEL_ANGLE / (double)(360.0) ) * (2 * pi * radiusCylinder));//(radiusCylinder * (pi / 180) * DEL_ANGLE);
    double y_delta = sin(DEGREE_TO_RAD(angle_turnOf_wheel)) * ( ((double)DEL_ANGLE / (double)(360.0)) * (2 * pi * radiusCylinder));//(radiusCylinder * (pi / 180) * DEL_ANGLE);
    moveWheel(-x_delta, -y_delta);
}
void rotateLeft()   ///a
{
    angle_turnOf_wheel = angle_turnOf_wheel - DEL_ANGLE;
}
void rotateRight()  ///d
{
    angle_turnOf_wheel = angle_turnOf_wheel + DEL_ANGLE;
}
///------------------------------------------------ Drawing functions end ------------------------------------
//For debugging
void setInitialPositionHere()
{
    initialCenterPoint.x = center_wheel_x;
    initialCenterPoint.y = center_wheel_y;
    printf("\nSetting initial pos here %lf, %lf\n\n", initialCenterPoint.x, initialCenterPoint.y);
}
void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;
    ///Codes for move forward, backward AND rotate left, right begin
        case 'w':
            moveForward();
            break;
        case 's':
            moveBackward();
            break;
        case 'a':
            rotateLeft();
            break;
        case 'd':
            rotateRight();
            break;
    ///Codes for move forward, backward AND rotate left, right end
    //For debugging
        case 'v':
            setInitialPositionHere();
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

    drawWheel();    ///WHEEL drawing

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
	drawgrid=1;
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
