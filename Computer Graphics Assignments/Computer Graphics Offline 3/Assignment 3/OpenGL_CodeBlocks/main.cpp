#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<fstream>
#include<vector>
#include <windows.h>
#include <glut.h>

#define DEBUG_SPHERE 0
#define DEBUG 0
#define DRAW_GRID 1

#define RADIUS_SHAPE 20
#define DEGREE_ANGLE_INIT 0.3

#define SPHERE_TYPE 1
#define PYRAMID_TYPE 2

#define EPSILON_COMPARISON 0.00001  // for comparison of two doubles or floats

#define pi (2*acos(0.0))
#define DEGREE_TO_RAD(x) ((x * pi) / 180)
#define RAD_TO_DEGREE(x) (x * 180 / pi)

using namespace std ;

int drawaxes;
int angle;

///------------------- My global variables and functions -----------------
ifstream fin; // for description.txt reading

bool isEqual(double val1, double val2, double threshold = EPSILON_COMPARISON){ //Is equal wrt the threshold?
    if(abs(val1 - val2) < threshold){
        return true;
    }
    return false;
}

///----------------------------- Class Vector begin -----------------------------

class Vector3D ///This is a 4X4 vector used throughout
{
public:
    double x, y, z;
    Vector3D()
    {
        x = 0;
        y = 0;
        z = 0;
    }
    Vector3D(double a, double b, double c)
    {
        this->x = a;
        this->y = b;
        this->z = c;
    }
    Vector3D(double a, double b, double c, double d)
    {
        this->x = a;
        this->y = b;
        this->z = c;
    }
    void assignVector(double a, double b, double c)
    {
        this->x = a;
        this->y = b;
        this->z = c;
    }

    void printVector()
    {
        cout << x << " " << y << " " << z << endl ;
    }

    double magnitude()
    {
        double ans = (x*x) + (y*y) + (z*z);
        ans = sqrt(ans);
        return ans;
    }
};

//-------------------------------------- Vector functions begin ------------------------------
Vector3D vectorScale(Vector3D a, double scale)
{
    Vector3D ans;
    ans.x = a.x * scale;
    ans.y = a.y * scale;
    ans.z = a.z * scale;
    return ans;
}
Vector3D vectorNormalize(Vector3D a)
{
    double factor = a.magnitude();
    factor = (double)(1.0 / factor);
    Vector3D v = vectorScale(a, factor);
    return v;
}
Vector3D vectorCrossProduct(Vector3D a, Vector3D b)
{
    Vector3D ans;
    ans.x = (a.y * b.z) - (b.y * a.z);
    ans.y = -((a.x * b.z) - (b.x * a.z));
    ans.z = (a.x * b.y) - (b.x * a.y);
    return ans;
};
Vector3D vectorAddition(Vector3D a, Vector3D b)
{
    Vector3D ans;
    ans.x = a.x + b.x;
    ans.y = a.y + b.y;
    ans.z = a.z + b.z;
    return ans;
};
Vector3D vectorSubtraction(Vector3D a, Vector3D b)
{
    Vector3D ans;
    ans.x = a.x - b.x;
    ans.y = a.y - b.y;
    ans.z = a.z - b.z;
    return ans;
};
double vectorDotProduct(Vector3D a, Vector3D b)
{
    return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
};

///------------------------------------- Vector Functions End ---------------------------------------

///---------------------- My Objects Begin -------------------------------

string coefficients_arr = {"Ambient", "Diffuse", "Specular", "Reflection"};

class Sphere
{
public:
    int id = -1; //default
    int typeOfObject; //1 -> Sphere, 2 -> Pyramid
    Vector3D center; //Center of sphere
    double radius;  //Radius of sphere
    double colors[3]; //three colors ... colors[0] -> r, colors[1] -> g, colors[2] -> b  [red, green, blue]
    double co_efficients[4]; //0->ambient, 1->diffuse, 2->specular, 3->reflection co-efficients
    double specular_exponent; //Final line

    Sphere(int type)
    {
        this->typeOfObject = SPHERE_TYPE; //Which type of object
        for(int i=0; i<3; i++){colors[i] = 0.0;} //initalise as black
    }
    void assignCoefficients(double a, double d, double s, double r){
        co_efficients[0] = a; co_efficients[1] = d; co_efficients[2] = s; co_efficients[3] = r;
    }
    void assignColors(double r, double b, double g){
        colors[0] = r; colors[1] = b; colors[2] = g;
    }
    void assignSpecularExponent(double sp){specular_exponent = sp;}
    void assignRadius(double r){radius = r;}
    void assignCenter(double x, double y, double z){center.x = x; center.y = y; center.z = z;}

    void assignSphere(double cx, double cy, double cz, double r, double c_r, double c_g, double c_b,
                      double am, double dif, double sp, double refl, double spec_exp){
        assignCenter(cx, cy, cz);
        assignRadius(r);
        assignColors(c_r, c_g, c_b);
        assignCoefficients(am, dif, sp, refl);
        assignSpecularExponent(spec_exp);
    }
    void printSphere()
    {
        cout << "------------------ Printing Sphere -------------- ID = " << id << "--------" << endl;
        cout << "Center:" << center.x << " " << center.y << " " << center.z << " " << end;
        cout << "Radius:" << radius << ",";
        cout << "Color:";
        for(int i=0; i<3; i++){cout << colors[i] << " ";}
        cout << endl;
        cout << "Coeffcients:";
        for(int i=0; i<4; i++){
            cout << coefficients_arr[i] << ":" << co_efficients[i] << " ";
        }
        cout << endl;
        cout << "Specular Exponent:" << specular_exponent << endl;
        cout << "------------------ Printing Sphere DONE --------------  " << endl;
    }

};

class Pyramid
{

};


///------------------------------ My Objects End ----------------------------------




///------------------------- Global Variables Begin --------------------------------

//---------- Angles for turning -----------
double angle_upDownRad = DEGREE_TO_RAD(DEGREE_ANGLE_INIT);
double angle_rightLeftRad = DEGREE_TO_RAD(DEGREE_ANGLE_INIT);
double angle_tiltRad = DEGREE_TO_RAD(DEGREE_ANGLE_INIT);
//-------------- Unit vectors for look, up, right, pos(camera position) ------------
Vector3D l, u, r, pos;
// ----------------- Other parameters ------------------
int recursion_level = 1; //default -> 1
int num_pixels_along_axes;
int num_objects;

// ------------------ Light Sources -------------
int num_light_sources;
vector<Vector3D> vector_light_sources; //positions of light sources

///------------------------- Global Variables End --------------------------------

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

    case GLUT_KEY_INSERT:
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


void loadAllData()
{

}

void drawAllObjects()
{

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

    drawAllObjects();

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
    cout << std::fixed ;

    ///Initialize pos, u, l, and r begin
    fin.open("description.txt");
    loadAllData();
    ///Initialize pos, u, l, and r done


    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("Assignment 3(1505022)");

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
