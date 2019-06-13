#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define DEBUG 0
#define DEBUG_CYLINDER 0
#define DEBUG_INITIAL_POSITION 1

#define pi (2*acos(0.0))
#define DEGREE_TO_RAD(x) ((x * pi) / 180)
#define RAD_TO_DEGREE(x) (x * 180 / pi)

#define INIT_X -80
#define INIT_Y -40
#define INIT_Z 50

#define RADIUS 30
#define HEIGHT 15

#define DEL_ANGLE 5 ///How many degrees to move while MOVING

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

struct point
{
	double x,y,z;
	double colorShade;
	void printPoint()
	{
	    printf("Point: <%lf, %lf, %lf, %lf>\n", x, y, z, colorShade);
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

double angle_axis_wheel; ///angle of axle i.e. angle to change when wheel will move forward / backward
double angle_turnOf_wheel;   /// angle for turning i.e. angle to change when rotation is done

double radiusCylinder ;//= 25;
double heightCylinder ;//= 15;

struct point initialCenterPoint, center_wheel;

struct point zeroThPoint, halfRevPoint; ///To maintain the cylinder's points for rectangle drawing

void initialiseParameters()
{
    angle_axis_wheel = 0;
    angle_turnOf_wheel = 0;

    radiusCylinder = RADIUS;
    heightCylinder = HEIGHT;

    center_wheel = makePoint(INIT_X, INIT_Y, INIT_Z);
    initialCenterPoint = makePoint(INIT_X, INIT_Y, INIT_Z);

    printf("---------------------- After Initializing Printing parameters begin------------------------------\n\n");
    printf("Center at %lf, %lf, radiusCylinder = %lf, heightCylinder = %lf, angle_axis = %lf, angle_turn = %lf\n",
           center_wheel.x, center_wheel.y, radiusCylinder, heightCylinder, angle_axis_wheel, angle_turnOf_wheel);
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

///------------------------------------------------------------------------------------------------------------------
void positionCylinderOnScreen()
{
    int stacks = 50, slices = 100;
    struct point points[100][200];
    int i,j;
    double h,r;

    glColor3f(0, 1, 0); //generate points
    h = 0;
    for(i=0; i<=stacks; i++)
    {
        r = radiusCylinder;
        h = i*(heightCylinder/stacks);
        //r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0; j<=slices; j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)* (2 * pi));   ///CIRCLES are drawn with respect to x-z axis
            points[i][j].z=r*sin(((double)j/(double)slices)* (2 * pi));
            points[i][j].y=h;
        }
    }
    //draw quads using generated points

    double shade = 0.5;

    for(int i=0; i<stacks; i++){
        for(int j=0; j<slices/4; j++){
            shade = (double)(j) / (double)(slices / 4);
//            shade = min(shade, 0.7);    //limit white
//            shade = max(shade, 0.3);    //limit black
            points[i][j].colorShade = shade;
        }
        for(int j=(slices/4); j<slices/2; j++){
                //Corner cases
            if(j == (slices / 4)){
//                printf("Changing point[%d][%d] shade to point[%d][%d]'s shade = %lf\n", i, j, i, j-1, points[i][j - 1].colorShade);
                points[i][j].colorShade = points[i][j - 1].colorShade;
            }
            else{
                int j_idx = slices/2 - j;
                points[i][j].colorShade = points[i][j_idx].colorShade;
            }
        }
        for(int j=slices/2; j<(slices * 0.75); j++){
            points[i][j].colorShade = points[i][j - slices/2].colorShade;
        }
        for(int j=(slices * 0.75); j<slices; j++){
            if(j == (slices * 0.75)){
                points[i][j].colorShade = points[i][j - 1].colorShade;
            }
            else{
                int j_idx = j - (slices/2);
                points[i][j].colorShade = points[i][j_idx].colorShade;
            }
        }


    }

    for(i=0; i<stacks; i++)
    {
//        initialShade = (double)(i)/(double)stacks;
        for(j=0; j<slices; j++)
        {
//            printf("j = %d, color = %lf\n", j, shade);
            shade = points[i][j].colorShade;
            glColor3f(shade, shade, shade);

            glBegin(GL_QUADS);
            {
                //upper hemisphere ONLY
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
            }
            glEnd();
        }
    }
    zeroThPoint = points[0][0];
    halfRevPoint = points[stacks/2][slices/2];

//    printf("\n\n===++++-->>> The two points returned are ... \n");
//    zeroThPoint.printPoint();
//    halfRevPoint.printPoint();
//    printf("\n\n");

}
void positionRect()
{
    glBegin(GL_QUADS);  ///zeroThPoint (0 rev)  ----- halfRevPoint (half rev) ---- fullRevPoint (full rev)
    {
        glVertex3f(zeroThPoint.x,(heightCylinder * 0.2), zeroThPoint.z);
        glVertex3f(zeroThPoint.x,(heightCylinder * 0.8), zeroThPoint.z);
        glVertex3f(halfRevPoint.x,(heightCylinder * 0.2), halfRevPoint.z);
        glVertex3f(halfRevPoint.x,(heightCylinder * 0.8), halfRevPoint.z);
    }
    glEnd();
}
void positionTwoRectanglesOnScreen()
{
    glColor3f(0.8, 0.4, 0);   //Orangish color
    positionRect();

    //rotate 90 degrees wrt y-axis and redraw rectangle
    glPushMatrix();
    {
        glRotatef(90, 0, 1, 0);
        positionRect();
    }
    glPopMatrix();
}

void drawWheel()
{
    ///Apply transformations wrt angle and positions then call the function positionWheelOnScreen()
    glPushMatrix();
    {
        //Translate wrt -ve x-axis , +ve y-axis and 0 z-axis
        glTranslatef(-center_wheel.x, center_wheel.y, 0);   //To MOVE wrt to the center

    //Rotation for angle of turning i.e. when calling rotate_left, rotate_right
        glRotatef(angle_turnOf_wheel, 0, 0, -1);    ///Rotate with respect to down

//        glTranslatef(-center_wheel.x, center_wheel.y, 0);   //To MOVE wrt to the center //This order will cause the wheel to DRIFT
    //Without this , we CANNOT SIMPLY ROTATE WITHOUT causing the wheel to move its position
        glTranslatef(0, -heightCylinder*0.5 , radiusCylinder); ///Translate upto the center of circle/cylinder point
    //Without the -height/2 and radius translations, wheel will MOVE positions while rotating

    //Initial rotation of axis of wheel [MOVE]
        glRotatef(angle_axis_wheel, 0, -1, 0);  ///Rotate wrt -ve y axis [INITIAL ROTATION for MOVE]

        positionCylinderOnScreen();

        positionTwoRectanglesOnScreen();
    }
    glPopMatrix();
}

void moveWheel(double x_delta, double y_delta)
{
    center_wheel.x += x_delta;
    center_wheel.y += y_delta;

///For testing how much wheel has moved
    double x_del = center_wheel.x - initialCenterPoint.x;
    double y_del = center_wheel.y - initialCenterPoint.y;

    struct point p = makePoint(x_del, y_del, 0);
    double perimeter_wheel = 2 * pi * radiusCylinder;
    printf("Moved <%lf, %lf> magnitude = %lf, angle_axis_wheel = %lf, angle_turn = %lf, perimeter_wheel = %lf\n\n",
           x_del, y_del, magnitude(p), angle_axis_wheel, angle_turnOf_wheel, perimeter_wheel);
}

void fixAngles()
{
    //fix axle angle
    if(angle_axis_wheel < 0){
        angle_axis_wheel = 360 + angle_axis_wheel;
    }
    else if(angle_axis_wheel > 360){
        angle_axis_wheel = angle_axis_wheel - 360;
    }
    //fix turn angle
    if(angle_turnOf_wheel < 0){
        angle_turnOf_wheel = 360 + angle_turnOf_wheel;
    }
    else if(angle_turnOf_wheel > 360){
        angle_turnOf_wheel = angle_turnOf_wheel - 360;
    }
}

void moveForward()  ///w
{
    ///center should move forward i.e. with respect to -ve x-axis direction
    angle_axis_wheel = angle_axis_wheel + DEL_ANGLE;
    fixAngles();    //keep within 360 degrees for easier debugging

    double x_delta = cos(DEGREE_TO_RAD(angle_turnOf_wheel)) * /*For projection on x-axis, cosine is taken*/
                    ( ((double)DEL_ANGLE / (double)(360.0) ) * (2 * pi * radiusCylinder));

    double y_delta = sin(DEGREE_TO_RAD(angle_turnOf_wheel)) * /*For projection on y-axis, sine is taken*/
                    ( ((double)DEL_ANGLE / (double)(360.0) ) * (2 * pi * radiusCylinder));

    moveWheel(x_delta, y_delta);
}
void moveBackward() ///s
{
    angle_axis_wheel = angle_axis_wheel - DEL_ANGLE;
    fixAngles();    //keep within 360 degrees for easier debugging

    double x_delta = cos(DEGREE_TO_RAD(angle_turnOf_wheel)) *  /*For projection on x-axis, cosine is taken*/
                    ( ((double)DEL_ANGLE / (double)(360.0) ) * (2 * pi * radiusCylinder));

    double y_delta = sin(DEGREE_TO_RAD(angle_turnOf_wheel)) * /*For projection on y-axis, sine is taken*/
                    ( ((double)DEL_ANGLE / (double)(360.0) ) * (2 * pi * radiusCylinder));

    moveWheel(-x_delta, -y_delta);  //just reverse direction ??
}
void rotateLeft()   ///a
{
    angle_turnOf_wheel = angle_turnOf_wheel - DEL_ANGLE;    //left so, -ve delta
    fixAngles();
    printf("\n\n--->>> Left Rotate, angle_turn_wheel = %lf, angle_axis = %lf, center.x = %lf, center.y = %lf\n",
           angle_turnOf_wheel, angle_axis_wheel, center_wheel.x, center_wheel.y);
}
void rotateRight()  ///d
{
    angle_turnOf_wheel = angle_turnOf_wheel + DEL_ANGLE;    //right so, +ve delta
    fixAngles();
    printf("\n\n--->>> Right Rotate, angle_turn_wheel = %lf, angle_axis = %lf, center.x = %lf, center.y = %lf\n",
           angle_turnOf_wheel, angle_axis_wheel, center_wheel.x, center_wheel.y);
}
///------------------------------------------------ Drawing functions end ------------------------------------
//For debugging
void setInitialPositionHere()
{
    initialCenterPoint.x = center_wheel.x;
    initialCenterPoint.y = center_wheel.y;
#if DEBUG_INITIAL_POSITION == 1
    printf("\n<DEBUG MODE>Setting initial pos here %lf, %lf\n\n", initialCenterPoint.x, initialCenterPoint.y);
#endif // DEBUG
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

        case 'v':   ///For debugging [perimeter check for 360 degrees rotation]
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
	drawaxes=0;
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
	initialiseParameters(); ///Initializing parameters for center_wheel & angles

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
