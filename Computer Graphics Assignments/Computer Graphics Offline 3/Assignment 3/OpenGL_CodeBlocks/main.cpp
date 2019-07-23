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

#define WHITE 7
#define BLACK 10

#define RADIUS_SHAPE 20
#define DEGREE_ANGLE_INIT 0.3

#define SPHERE_TYPE 1
#define PYRAMID_TYPE 2

#define EPSILON_COMPARISON 0.00001  // for comparison of two doubles or floats
#define NUM_TRIANGLES_IN_PYRAMID 6  // NUM_TRIANGLES_IN_PYRAMID is 6 [4 triangles + square base (2 triangles)]

#define pi (2*acos(0.0))
#define DEGREE_TO_RAD(x) ((x * pi) / 180)
#define RAD_TO_DEGREE(x) (x * 180 / pi)

#define NUM_TILES 67 // 2*1000 / 30 = 2000/30 = 66.6667 =~ 67

#define CHECKER_BOARD_WIDTH 30 //width of each tile of checker-board is 30
#define INFINITE_INDEX 1000


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

    void printVector(bool flag = false)
    {
        if(flag == true){
            cout << x << " " << y << " " << z ;
        }else{
            cout << x << " " << y << " " << z << endl ;
        }
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

string coefficients_arr[] = {"Ambient", "Diffuse", "Specular", "Reflection"};

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

    Sphere()
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

    void formSphere(double cx, double cy, double cz, double r, double c_r, double c_g, double c_b,
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
        cout << "Center:" << center.x << " " << center.y << " " << center.z << " " << endl;
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

//Surface
class Triangle
{
public:
    int id_triangle = -1; //debug
    Vector3D point1, point2, point3;
    double assignTriangle(Vector3D p1, Vector3D p2, Vector3D p3){point1 = p1; point2 = p2; point3 = p3;}
    void printTriangle(){
        cout << "Triangle ID: " << id_triangle << endl;
        point1.printVector(false);
        cout << " ";
        point2.printVector(false);
        cout << " ";
        point3.printVector(false);
        cout << endl;
    }
};

class Pyramid
{
public:
    //triangles[0] and triangles[1] are bases
    Triangle triangles[NUM_TRIANGLES_IN_PYRAMID]; //NUM_TRIANGLES_IN_PYRAMID = 6 [considered base as two triangles]
    int id = -1; //default [for debugging]
    int typeOfObject; //1 -> Sphere, 2 -> Pyramid
    double colors[3]; //three colors ... colors[0] -> r, colors[1] -> g, colors[2] -> b  [red, green, blue]
    double co_efficients[4]; //0->ambient, 1->diffuse, 2->specular, 3->reflection co-efficients
    double specular_exponent; //Final line

    Pyramid()
    {
        this->typeOfObject = PYRAMID_TYPE;
        for(int i=0; i<NUM_TRIANGLES_IN_PYRAMID; i++){
            triangles[i].id_triangle = i;
        }
    }
    //------------Form Triangles----------------
    void formTriangles(double lowest_x, double lowest_y, double lowest_z, double len_base, double len_height)
    {
        //First we form base triangles [2 triangles] ... we keep base of pyramid at x-y plane
        Vector3D lowest_point(lowest_x, lowest_y, lowest_z);

        Vector3D bottom_left, bottom_right, top_left, top_right, upper_most, mid_point;

        //------------------- Square Base points calculation ---------------------------------
        bottom_left = lowest_point;  // one point is the left-bottom-most point
        bottom_right = bottom_left;            // another point is len_base units away in x-direction
        bottom_right.x = lowest_point.x + len_base;
        top_left = bottom_left;            // another point is len_base units away in y-direction
        top_left.y = lowest_point.y + len_base;
        top_right.x = lowest_point.x + len_base; //top right most point
        top_right.y = lowest_point.y + len_base;
        top_right.z = lowest_point.z ;
        //-------------------------- Top most point and mid point calculation -------------------
        mid_point.z = lowest_point.z; //mid point
        mid_point.x = 0.5 * (bottom_left.x + bottom_right.x);
        mid_point.y = 0.5 * (bottom_left.y + bottom_right.y);
        upper_most = mid_point; //upper-most point [same x, y as mid-point, but z = mid.z + height]
        upper_most.z = lowest_point.z + len_height;

        //Triangle idx = 0 [base -> 1]
        triangles[0].assignTriangle(top_left, bottom_left, bottom_right); //anti-clockwise direction
        //Triangle idx = 1 [base -> 2]
        triangles[1].assignTriangle(bottom_right, top_right, top_left); //anti-clockwise direction
        //Now we assign Other triangles
        //Left-most face
        triangles[2].assignTriangle(top_left, upper_most, bottom_left);
        //Front face
        triangles[3].assignTriangle(bottom_right, upper_most, bottom_left);
        //Right-most face
        triangles[4].assignTriangle(top_right, upper_most, bottom_right);
        //Back face
        triangles[5].assignTriangle(top_left, upper_most, top_right);
    }

    void assignCoefficients(double a, double d, double s, double r){
        co_efficients[0] = a; co_efficients[1] = d; co_efficients[2] = s; co_efficients[3] = r;
    }
    void assignColors(double r, double b, double g){
        colors[0] = r; colors[1] = b; colors[2] = g;
    }
    void assignSpecularExponent(double sp){specular_exponent = sp;}

    void formPyramid(double lowest_x, double lowest_y, double lowest_z, double len_base, double len_height,
                     double color_r, double color_g, double color_b,
                     double co_am, double co_dif, double co_spec, double co_ref,
                     double spec_exp){
        this->typeOfObject = PYRAMID_TYPE;
        formTriangles(lowest_x, lowest_y, lowest_z, len_base, len_height);
        assignCoefficients(co_am, co_dif, co_spec, co_ref);
        assignColors(color_r, color_g, color_b);
        assignSpecularExponent(spec_exp);
    }

    void printPyramid()
    {
        cout << "------------------ Printing PYRAMID -------------- ID = " << id << "--------" << endl;

        cout << "BASE:" << endl;
        for(int i=0; i<2; i++){triangles[i].printTriangle();}
        cout << "SIDES:" << endl;
        for(int i=2; i<6; i++){triangles[i].printTriangle();}

        cout << "Color:";
        for(int i=0; i<3; i++){cout << colors[i] << " ";}
        cout << endl;

        cout << "Coeffcients:";
        for(int i=0; i<4; i++){
            cout << coefficients_arr[i] << ":" << co_efficients[i] << " ";
        }
        cout << endl;
        cout << "Specular Exponent:" << specular_exponent << endl;
        cout << "------------------ Printing PYRAMID DONE --------------  " << endl;
    }
};


class CheckerBoardTile
{
public:
    double color; //BLACK or WHITE
    Vector3D left_most_point; //left most point
    CheckerBoardTile(){}
    CheckerBoardTile(double col){color = col;}
};


///------------------------------ My Objects End ----------------------------------




///------------------------- Global Variables Begin --------------------------------

//---------- Angles for turning -----------
double angle_upDownRad = DEGREE_TO_RAD(DEGREE_ANGLE_INIT);
double angle_rightLeftRad = DEGREE_TO_RAD(DEGREE_ANGLE_INIT);
double angle_tiltRad = DEGREE_TO_RAD(DEGREE_ANGLE_INIT);
//-------------- Unit vectors for look, up, right, pos(camera position) ------------
Vector3D l, u, r, pos;
double nearDistance = 1;
double farDistance = 1000;
double WINDOW_SIZE = 500;
double field_angle = 90; //degrees
// ----------------- Other parameters ------------------
int recursion_level = 1; //default -> 1
int num_pixels_along_axes;
int num_objects;

// ------------------ Light Sources -------------
int num_light_sources;
vector<Vector3D> light_sources; //positions of light sources

// ---------------------- Objects list i.e. vector<Objects> ------------------
vector<Sphere> spheres_list;
vector<Pyramid> pyramids_list;
CheckerBoardTile checker_board[NUM_TILES][NUM_TILES]; //2D array
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
    case '0':
        cout << "-------->> TO DO ... capture image" << endl;
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


void initialiseParams()
{
    //------------ Just like in Assignment 1 -------------
    u.assignVector(0, 0, 1);
    r.assignVector(-1/sqrt(2), 1/sqrt(2), 0);
    l.assignVector(-1/sqrt(2), -1/sqrt(2), 0);
    pos.assignVector(100, 100, 0);
}


void loadCheckerBoard()
{
    checker_board[0][0] = CheckerBoardTile(WHITE);
    checker_board[0][0].left_most_point.assignVector(0, 0, 0); //origin as left-most point having WHITE color

    Vector3D point_left_most(0, 0, 0); //the left-most point
    int col;

    for(int i=0; i<(2*INFINITE_INDEX); i++){
        //Rows switch
        point_left_most.y += ((i * CHECKER_BOARD_WIDTH) - (INFINITE_INDEX * CHECKER_BOARD_WIDTH));
        for(int j=0; j<(2*INFINITE_INDEX); j++){
            //Columns switch

            if(((i + j) % 2) == 0){
                //even
                col = WHITE;
            }else{
                //odd
                col = BLACK;
            }
            point_left_most.x += ((j * CHECKER_BOARD_WIDTH) - (INFINITE_INDEX*CHECKER_BOARD_WIDTH));
            checker_board[i][j].left_most_point.assignVector(point_left_most.x, point_left_most.y, point_left_most.z);
            checker_board[i][j].color = col;
        }
    }
}

void loadAllData()
{
    initialiseParams(); //load look, right, up, pos vectors just like in assignment 1
    //Initial parameters reading
    fin >> recursion_level;
    fin >> num_pixels_along_axes;
    fin >> num_objects;
    //Now read the objects
    string inputString;

    double x, y, z, c_r, c_g, c_b, base, height, radius, coef_am, coef_dif, coef_spec, coef_ref, spec_exp;
    //Objects reading
    for(int i=0; i<num_objects; i++){
        getline(fin, inputString); // Saves the line in inputString [input command]
        if(inputString == "pyramid"){
            //load pyramid
            fin >> x >> y >> z; //lowest points
            fin >> base >> height; //base length and height
            fin >> c_r >> c_g >> c_b; //colors r,g,b
            fin >> coef_am >> coef_dif >> coef_spec >> coef_ref; //co-efficients of ambient, diffuse, specular, reflection
            fin >> spec_exp; //specular exponent
            Pyramid pyramid;
            pyramid.formPyramid(x, y, z, base, height, c_r, c_g, c_b, coef_am, coef_dif, coef_spec, coef_ref, spec_exp);
            pyramids_list.push_back(pyramid);
        }
        else if(inputString == "sphere"){
            //load sphere
            fin >> x >> y >> z; //center
            fin >> radius; //radius
            fin >> c_r >> c_g >> c_b; //colors r,g,b
            fin >> coef_am >> coef_dif >> coef_spec >> coef_ref; //co-efficients of ambient, diffuse, specular, reflection
            fin >> spec_exp; //specular exponent

            Sphere sphere;
            sphere.formSphere(x, y, z, radius, c_r, c_g, c_b, coef_am, coef_dif, coef_spec, coef_ref, spec_exp);
            spheres_list.push_back(sphere);
        }
    }
    //Light sources reading
    fin >> num_light_sources;
    for(int i=0; i<num_light_sources; i++){
        fin >> x >> y >> z;
        Vector3D vec(x, y, z);
        light_sources.push_back(vec);
    }

    loadCheckerBoard();
}

void drawAllObjects()
{
    //
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
