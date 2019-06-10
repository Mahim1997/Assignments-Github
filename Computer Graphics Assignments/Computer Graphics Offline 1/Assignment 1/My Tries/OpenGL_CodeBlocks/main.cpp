#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define DEBUG 1
#define DRAW_GRID 1
#define RADIUS_COMMON 20

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

///----------------------------- My Variables Begin ---------------------------------------

double radiusSphere, translation_unit_sphere;  ///Sphere
double radiusCylinder, heightCylinder, translation_unit_cylinder; ///Cylinder

double upDownScaler = 3;
double forwardBackwardScalar = 3;
double rightLeftScalar = 5;

//Struct vector for u, l, and r
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
//        cout << "Name: " << name << ", x = " << x << ", y = " << y << ", z = " << z << endl ;
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
    struct vect multiplier(double m)
    {
        vect v(m*x, m*y, m*z);
        return v;
    }
};

struct point
{
    double x,y,z;
    void makePoint(double a, double b, double c)
    {
        x = a;
        y = b;
        z = c;
    }
    void makePointWithVector(struct vect ve)
    {
        x = ve.x; y = ve.y; z = ve.z;
    }
    void printPoint()
    {
        printf("Point: <x = %lf, y = %lf, z = %lf>\n", x, y, z);
    }
};

///Global Variables to maintain
struct vect u, l, r;
struct point pos ;


void initialiseParamters()
{
    u.makeVectorWithName(0, 0, 1, "u");
    r.makeVectorWithName(-1/sqrt(2), 1/sqrt(2), 0, "r");
    l.makeVectorWithName(-1/sqrt(2), -1/sqrt(2), 0, "l");
    pos.makePoint(100, 100, 50);

    printf("-------------------------- Initializing parameters begin------------------------\n");
    u.printVectorWithName();
    l.printVectorWithName();
    r.printVectorWithName();
    pos.printPoint();
    printf("-------------------------- Initializing parameters end------------------------\n");

    ///Initializing other parameters
    radiusSphere = RADIUS_COMMON;
    translation_unit_sphere = 20;

    radiusCylinder = RADIUS_COMMON;
    heightCylinder = 40;
}

///------------------------------------- My Variables End ---------------------------------------

void drawAxes()
{
    if(drawaxes==1)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f( 100,0,0);
            glVertex3f(-100,0,0);

            glVertex3f(0,-100,0);
            glVertex3f(0, 100,0);

            glVertex3f(0,0, 100);
            glVertex3f(0,0,-100);
        }
        glEnd();
    }
}


void drawGrid()
{
    int i;
    if(drawgrid==1)
    {
        glColor3f(0.6, 0.6, 0.6);	//grey
        glBegin(GL_LINES);
        {
            for(i=-8; i<=8; i++)
            {

                if(i==0)
                    continue;	//SKIP the MAIN axes

                //lines parallel to Y-axis
                glVertex3f(i*10, -90, 0);
                glVertex3f(i*10,  90, 0);

                //lines parallel to X-axis
                glVertex3f(-90, i*10, 0);
                glVertex3f( 90, i*10, 0);
            }
        }
        glEnd();
    }
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
    glBegin(GL_QUADS);
    {
        glVertex3f( a, a,2);
        glVertex3f( a,-a,2);
        glVertex3f(-a,-a,2);
        glVertex3f(-a, a,2);
    }
    glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0; i<=segments; i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0; i<segments; i++)
    {
        glBegin(GL_LINES);
        {
            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0; i<=segments; i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0; i<segments; i++)
    {
        //create shading effect
        if(i<segments/2)
            shade=2*(double)i/(double)segments;
        else
            shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);
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
    for(i=0; i<=stacks; i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0; j<=slices; j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
            points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
            points[i][j].z=h;
        }
    }
    //draw quads using generated points
    for(i=0; i<stacks; i++)
    {
        glColor3f(1, 0, 0); ///To change color to RED = 1, GREEEN = 0, BLUE = 0
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        for(j=0; j<slices; j++)
        {
            glBegin(GL_QUADS);
            {
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
            }
            glEnd();
        }
    }
}


void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}

void keyboardListener(unsigned char key, int x,int y)
{
    switch(key)
    {

    case '1':
        drawgrid=1-drawgrid;
        break;

    default:
        break;
    }
}


void specialKeyListener(int key, int x,int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:		///Move with respect to LOOK
        pos.x = pos.x + forwardBackwardScalar*l.x;
        pos.y = pos.y + forwardBackwardScalar*l.y;
        pos.z = pos.z + forwardBackwardScalar*l.z;
        break;
    case GLUT_KEY_DOWN:		// up arrow key
        pos.x = pos.x - forwardBackwardScalar*l.x;
        pos.y = pos.y - forwardBackwardScalar*l.y;
        pos.z = pos.z - forwardBackwardScalar*l.z;
        break;

    case GLUT_KEY_RIGHT:    ///Move with respect to RIGHT
        pos.x = pos.x + rightLeftScalar*r.x;
        pos.y = pos.y + rightLeftScalar*r.y;
        pos.z = pos.z + rightLeftScalar*r.z;
        break;
    case GLUT_KEY_LEFT:
        pos.x = pos.x - rightLeftScalar*r.x;
        pos.y = pos.y - rightLeftScalar*r.y;
        pos.z = pos.z - rightLeftScalar*r.z;
        break;

    case GLUT_KEY_PAGE_UP:  ///Move with respect to UP
        pos.x = pos.x + upDownScaler*u.x;
        pos.y = pos.y + upDownScaler*u.y;
        pos.z = pos.z + upDownScaler*u.z;
        break;
    case GLUT_KEY_PAGE_DOWN:
        pos.x = pos.x - upDownScaler*u.x;
        pos.y = pos.y - upDownScaler*u.y;
        pos.z = pos.z - upDownScaler*u.z;
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


void mouseListener(int button, int state, int x, int y) 	//x, y is the x-y of the screen (2D)
{
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN) 		// 2 times?? in ONE click? -- solution is checking DOWN or UP
        {
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



///----------------------------- My Functions Begin ---------------------------------------

void drawSphere_UpperPart(double radius,int slices,int stacks)
{
    struct point points[100][100];
    int i,j;
    double h,r;
    //generate points
    for(i=0; i<=stacks; i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0; j<=slices; j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
            points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
            points[i][j].z=h;
        }
    }
    //draw quads using generated points
    for(i=0; i<stacks; i++)
    {
        glColor3f(1, 0, 0); ///To change color to RED = 1, GREEEN = 0, BLUE = 0
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        for(j=0; j<slices; j++)
        {
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
}

void drawCylinder(double rad, double height, int segments)
{

}

void placeSpheresPositions()  ///Function to place sphere positions
{
    int angles_degrees[] = {0, 90, 180, 270};
    for(int i = 0; i < 4; i++)
    {
        glPushMatrix();
        {
            glRotatef(angles_degrees[i], 0, 0, 1);
            glTranslatef(translation_unit_sphere, translation_unit_sphere, translation_unit_sphere);
            drawSphere_UpperPart(radiusSphere, 50, 30);
        }
        glPopMatrix();
    }
}
void placeCylinderPositions()
{
    int angles_degrees[] = {0, 90, 180, 270};
    for(int i = 0; i < 4; i++)
    {
        glPushMatrix();
        {
            glRotatef(angles_degrees[i], 0, 0, 1);
            glTranslatef(translation_unit_sphere, translation_unit_sphere, translation_unit_sphere);
            drawCylinder(radiusCylinder, heightCylinder, 30);
        }
        glPopMatrix();
    }
}
///---------------------------- Full Object Drawing Functions ------------------------------

void drawSpherePartForObject()
{
    glPushMatrix();
    {
        placeSpheresPositions();    /// Four (1/8th of a sphere) s
        glRotatef(180, 0, 1, 0);       /// Rotate at 180 degrees
        placeSpheresPositions();    /// Four (1/8th of a sphere) s
    }
    glPopMatrix();
}

void drawCylinderPartOfObject()
{
    placeCylinderPositions();   ///One-fourth of a cylinder

}

///----------------------------- My Functions End ---------------------------------------

void test_draw_cylinder()
{
    int i, segments = 30, radius = radiusCylinder;
    int height = 0;

    struct point points1[100], points2[100];
    glColor3f(0,1.0,0); ///Green color (cylinder)

    ///generate points top circle
    for(i=0; i<=segments; i++)
    {
        points1[i].x = radius*cos(((double)i/(double)segments)*2*pi);
        points1[i].y = radius*sin(((double)i/(double)segments)*2*pi);
        points1[i].z = height;
    }

    ///draw segments using generated points
    for(i=0; i<segments; i++)
    {
        glBegin(GL_LINES);
        {
            glVertex3f(points1[i].x, points1[i].y, points1[i].z);
            glVertex3f(points1[i+1].x, points1[i+1].y, points1[i].z);
        }
        glEnd();
    }

    ///generate points bottom circle
    for(i=0; i<=segments; i++)
    {
        points2[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points2[i].y=radius*sin(((double)i/(double)segments)*2*pi);
        points2[i].z = height + heightCylinder;
    }

    ///draw segments using generated points
    for(i=0; i<segments; i++)
    {
        glBegin(GL_LINES);
        {
            glVertex3f(points2[i].x,points2[i].y, points2[i].z);
            glVertex3f(points2[i+1].x,points2[i+1].y, points2[i].z);
        }
        glEnd();
    }

    ///Join the points ... Take two points from top circle, two points from bottom, draw rectangle ...

#if DEBUG == 1
    printf("PRINTING points 1st one ... \n");

    for(i=0; i<segments; i++)
    {
        points1[i].printPoint();
    }
    printf("\n\nNow 2nd one \n");
    for(i=0; i<segments; i++)
    {
        points2[i].printPoint();
    }
#endif // DEBUG

    for(i=0; i<segments - 1; i++)
    {
        glBegin(GL_TRIANGLES);    ///To draw a rectangle
        {
            glVertex3f(points1[i].x, points1[i].y, points1[i].z);
            glVertex3f(points1[i+1].x, points1[i+1].y, points1[i+1].z);
            glVertex3f(points2[i+1].x, points2[i+1].y, points2[i+1].z);


            glVertex3f(points2[i+1].x, points2[i+1].y, points2[i+1].z);
            glVertex3f(points2[i].x, points2[i].y, points2[i].z);
            glVertex3f(points1[i].x, points1[i].y, points1[i].z);

        }
        glEnd();
    }
}

void display()
{

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
    //gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
//    gluLookAt(0,0,200,	0,0,0,	0,1,0);

    ///--------- To Look At using u, l, r, and pos ---------
    gluLookAt(pos.x, pos.y, pos.z, (pos.x + l.x),(pos.y + l.y),(pos.z + l.z), u.x, u.y, u.z);

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

    //drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

    //drawSphere(30,24,20);

    //drawSpherePartForObject();
    test_draw_cylinder();

    ///Code for drawing the object ends


    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}


void animate()
{
    angle+=0.05;
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}

void init()
{
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

int main(int argc, char **argv)
{

    ///Initialize pos, u, l, and r begin
    initialiseParamters();
    ///Initialize pos, u, l, and r done


    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("Assignment 1 (1505022)");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();		//The main loop of OpenGL

    return 0;
}
