#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define DEBUG_SPHERE 0
#define DEBUG 0
#define DRAW_GRID 1

#define RADIUS_SHAPE 20
#define DEGREE_ANGLE_INIT 0.3

#define pi (2*acos(0.0))
#define DEGREE_TO_RAD(x) ((x * pi) / 180)
#define RAD_TO_DEGREE(x) (x * 180 / pi)

int drawaxes;
int angle;
///----------------------------- My Variables Begin ---------------------------------------
double radiusObject, translation_unit, heightCylinder, side_cube;   ///Parameters for shape
double threshold_movement, reduction;  ///For sphere to cube movement threshold

double scalar_upDown = 3;
double scalar_forwardBackward = 3;
double scalar_rightLeft = 5;

double angle_upDownRad = DEGREE_TO_RAD(DEGREE_ANGLE_INIT);
double angle_rightLeftRad = DEGREE_TO_RAD(DEGREE_ANGLE_INIT);
double angle_tiltRad = DEGREE_TO_RAD(DEGREE_ANGLE_INIT);



struct point    ///For already existing functions ....
{
    double x,y,z;
};
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

///Global Variables to maintain
struct vect u, l, r;
struct vect pos ;


void initialiseParamters()
{
    u.makeVectorWithName(0, 0, 1, "u");
    r.makeVectorWithName(-1/sqrt(2), 1/sqrt(2), 0, "r");
    l.makeVectorWithName(-1/sqrt(2), -1/sqrt(2), 0, "l");
    pos.makeVectorWithName(100, 100, 0, "pos");

    printf("-------------------------- Initializing parameters begin------------------------\n");
    u.printVectorWithName();
    l.printVectorWithName();
    r.printVectorWithName();
    pos.printVectorWithName();
    printf("-------------------------- Initializing parameters end------------------------\n");

    ///Initializing other parameters
    radiusObject = RADIUS_SHAPE;
    translation_unit = 20;
    heightCylinder = 40;

//    side_cube = heightCylinder - radiusObject - radiusObject; ///exact initialization
    side_cube = translation_unit;   ///initialization of cube
    threshold_movement = 40;    ///full cube = full sphere diameter = 40
    reduction = 0.75;
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
void fixTranslationUnit()
{
    if(translation_unit < 0){
        translation_unit = 0;
    }
    else if(translation_unit > threshold_movement){
        translation_unit = threshold_movement;
    }
}
void computeParameters()
{
    radiusObject = threshold_movement - translation_unit;   ///radius of object + translation of object = 40 [Constraint #1]
    heightCylinder = translation_unit * 2;  ///Cylinder height gets double of the translation because cylinder is 2 X upper-lower
    side_cube = translation_unit;   ///Cube is 1 X upper-lower
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

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
    glBegin(GL_QUADS);
    {
        glVertex3f( a, a,0);
        glVertex3f( a,-a,0);
        glVertex3f(-a,-a,0);
        glVertex3f(-a, a,0);
    }
    glEnd();
}


void keyboardListener(unsigned char key, int x,int y)
{
    switch(key)
    {
    case '1':   ///Rotate LEFT
        ///Rotate 'r' vector with respect to 'u' vector ... 'l' is to be adjusted
        r = vectorAddition(vectorScale(r, cos(angle_rightLeftRad)),
                           vectorScale(l, sin(angle_rightLeftRad)));
        /// Fix l vector [mutual perpendicular]
        l = vectorCrossProduct(u, r);
        break;
    case '2':   ///Rotate RIGHT [Exactly as LEFT but with -ve of the angle]
        r = vectorAddition(vectorScale(r, cos(-angle_rightLeftRad)),
                           vectorScale(l, sin(-angle_rightLeftRad)));
        /// Fix l vector[mutual perpendicular]
        l = vectorCrossProduct(u, r);   /// l = u X r
        break;
    case '3':   ///LOOK UP
        ///Rotate 'l' vector acw with respect to 'r' vector i.e. Mutual perpendicular is 'u' in this case
        l = vectorAddition(vectorScale(l, cos(angle_upDownRad)),
                           vectorScale(u, sin(angle_upDownRad)));
        /// Fix 'u' [mutual perpendicular]
        u = vectorCrossProduct(r, l);   /// u = r X l

        break;
    case '4':   ///LOOK DOWN ... exactly same as Look Up but with -ve of the angle
        l = vectorAddition(vectorScale(l, cos(-angle_upDownRad)),
                           vectorScale(u, sin(-angle_upDownRad)));
        /// Fix 'u' [mutual perpendicular]
        u = vectorCrossProduct(r, l);   /// u = r X l
        break;
    case '5':   ///Tilt clockwise
        ///Rotate 'u' with respect to 'l' BUT at -ve angle [cw rotation]
        u = vectorAddition(vectorScale(u, cos(-angle_upDownRad)),
                           vectorScale(r, sin(-angle_upDownRad)));
        ///Fix 'r' [mutual perpendicular]
        r = vectorCrossProduct(l, u);
        break;
    case '6':   ///Tilt anti-clockwise
        ///Rotate 'u' with respect to 'l' BUT at +ve angle [acw rotation]
        u = vectorAddition(vectorScale(u, cos(angle_upDownRad)),
                           vectorScale(r, sin(angle_upDownRad)));
        ///Fix 'r' [mutual perpendicular]
        r = vectorCrossProduct(l, u);
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
        pos = vectorAddition(pos, vectorScale(l, scalar_forwardBackward));
        break;
    case GLUT_KEY_DOWN:		// up arrow key
        pos = vectorSubtraction(pos, vectorScale(l, scalar_forwardBackward));
        break;
    case GLUT_KEY_RIGHT:    ///Move with respect to RIGHT
        pos = vectorAddition(pos, vectorScale(r, scalar_rightLeft));
        break;
    case GLUT_KEY_LEFT:
        pos = vectorSubtraction(pos, vectorScale(r, scalar_rightLeft));
        break;
    case GLUT_KEY_PAGE_UP:  ///Move with respect to UP
        pos = vectorAddition(pos, vectorScale(u, scalar_upDown));
        break;
    case GLUT_KEY_PAGE_DOWN:
        pos = vectorSubtraction(pos, vectorScale(u, scalar_upDown));
        break;

    case GLUT_KEY_INSERT:
        break;

    case GLUT_KEY_HOME: ///Cube to Sphere
        translation_unit -= reduction;
        fixTranslationUnit();
        computeParameters();
        break;
    case GLUT_KEY_END:  ///Sphere to Cube
        translation_unit += reduction;
        fixTranslationUnit();
//        radiusObject += reduction;
        computeParameters();

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
            drawaxes = 1 - drawaxes;
        }
        break;

    case GLUT_RIGHT_BUTTON:
        //........
        break;

    case GLUT_MIDDLE_BUTTON:

        break;

    default:
        break;
    }
}



///----------------------------- My Functions Begin ---------------------------------------

void drawOneEigthSphere(double radius,int slices,int stacks)
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
            points[i][j].x=r*cos(((double)j/(double)slices)* (pi/2));   ///CIRCLES are drawn with 90 degrees
            points[i][j].y=r*sin(((double)j/(double)slices)* (pi/2));   ///i.e. 1/4th of a hemisphere = 1/8th of a sphere
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
                /*
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
                */
            }
            glEnd();
        }
    }
}
void drawCylinder_oneFourth(double radius, double height, int segments)
{
    int stacks = 50, slices = 30;
    struct point points[100][100];
    int i,j;
    double h,r;

    glColor3f(0, 1, 0); //generate points
    h = 0;
    for(i=0; i<=stacks; i++)
    {
        r = radius;
        h = i*(heightCylinder/stacks);
        //r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0; j<=slices; j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)* (pi/2));   ///CIRCLES are drawn with 90 degrees
            points[i][j].y=r*sin(((double)j/(double)slices)* (pi/2));
            points[i][j].z=h;
        }
    }
    //draw quads using generated points
    for(i=0; i<stacks; i++)
    {
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

///---------------------------- Full Object Drawing Functions ------------------------------


void drawSpherePartOfObject()
{
//    side_cube = 0;
    double angles[] = {90, 180, 270};
    for(int i=0; i<4; i++){         ///Top 4 (1/8th spheres)
        glPushMatrix();
        {
            if(i != 0){
                glRotatef(angles[i - 1], 0, 0, 1);
            }
            glTranslatef(translation_unit, translation_unit, translation_unit);
            drawOneEigthSphere(radiusObject, 50, 30);
        }
        glPopMatrix();
    }


    glPushMatrix();
    glRotatef(180, 0, 1, 0);

    for(int i=0; i<4; i++){         ///Bottom 4 (1/8th spheres)
        glPushMatrix();
        {
            if(i != 0){
                glRotatef(angles[i - 1], 0, 0, 1);
            }
            glTranslatef(translation_unit, translation_unit, translation_unit);
            drawOneEigthSphere(radiusObject, 50, 30);
        }
        glPopMatrix();
    }
    glPopMatrix();

}

void position4Cylinders()
{
    int numSegmentsCylinder = 50;

    for(int angle = 0; angle < 360; angle += 90)
    {
        glPushMatrix();
        {
//            glTranslatef(translation_unit, translation_unit, -translation_unit);
            glRotatef(angle, 0, 0, 1);
            glTranslatef(translation_unit, translation_unit, -translation_unit);
            drawCylinder_oneFourth(radiusObject, 0, numSegmentsCylinder);
        }
        glPopMatrix();
    }
}

void drawCylinderPartOfObject()
{
    position4Cylinders();   ///Vertical 4 [parallel to z-axis 4 cylinders]

    glPushMatrix();
    {
        glRotatef(90, 0, 1, 0);
        position4Cylinders();   ///With respect to y-axis 4 cylinders
    }
    glPopMatrix();

    glPushMatrix();
    {
        glRotatef(90, 1, 0, 0);
        position4Cylinders();   ///Wrt x-axis 4 cylinders
    }
    glPopMatrix();

}

void draw2Squares(double a, double translation_cube)
{
    glPushMatrix(); /// Bottom most
    {
        glTranslatef(0, 0, -translation_cube);
        drawSquare(side_cube);
    }
    glPopMatrix();

    glPushMatrix(); ///Top most
    {
        glTranslatef(0, 0, translation_cube);
        drawSquare(side_cube);
    }
    glPopMatrix();
}

void drawSquarePartOfObject(double a) ///a = side length
{
    glColor3d(1, 1, 1); ///White color
    double translation_cube = 40;

    draw2Squares(a, translation_cube);  ///Bottom and Top

    glPushMatrix(); ///Left front and right back
    {
        glRotatef(90, 0, 1, 0);
        draw2Squares(a, translation_cube);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glRotatef(90, 1, 0, 0);
        draw2Squares(a, translation_cube);
    }
    glPopMatrix();

}

///----------------------------- My Functions End ---------------------------------------


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

//    drawGrid();

///CODE FOR DRAWING OBJECT BEGIN

//    drawSphere_UpperPart(radiusObject, 50, 30);
#if DEBUG_SPHERE == 1
    side_cube = 0;
#endif // DEBUG_SPHERE

    drawSquarePartOfObject(side_cube);

    drawSpherePartOfObject();

    drawCylinderPartOfObject();


///CODE FOR DRAWING OBJECT END

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
//    drawgrid=0;
//    drawaxes=1;
//    cameraHeight=150.0;
//    cameraAngle=1.0;
    angle=0;
    drawaxes = 1;
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

///------------------------------------------------------------------------------


int main(int argc, char **argv)
{

    ///Initialize pos, u, l, and r begin
    initialiseParamters();
    ///Initialize pos, u, l, and r done


    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("Assignment 1 Part 1 (1505022)");

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



    //glColor3f(1,0,0);
    //drawSquare(10);
    //drawSS();
    //drawCircle(30,24);
    //drawCone(20,50,24);
    //drawSphere(30,24,20);
