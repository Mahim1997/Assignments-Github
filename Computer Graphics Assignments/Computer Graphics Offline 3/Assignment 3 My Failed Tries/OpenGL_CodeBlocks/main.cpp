#include "bitmap_image.hpp"
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<fstream>
#include<vector>
#include <windows.h>
#include <glut.h>

#define MAX_VAL 9999999
#define NULL_VALUE_T -1000

#define DEBUG_SPHERE 0
#define DEBUG 0
#define DEBUG_MID_POINTS 1

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

#define NUM_TILES 66 // 2*1000 / 30 = 2000/30 = 66.6667 =~ 67
#define NUM_TILES_HALF 33

#define WIDTH_CHECKER_BOARD 30 //width of each tile of checker-board is 30
#define INFINITE_INDEX 1000

#define PIXEL_NUM 768 //pixel-window-size = 768 X 768
//#define PIXEL_NUM 200 //debug

using namespace std ;

int drawaxes;
int angle;
int drawgrid = 0;
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

///------------- Ray Class -----------------
class Ray
{
public:
    Vector3D initial_position;
    Vector3D direction_vector;
    void normalise() //normalize the direction vector
    {
        direction_vector = vectorNormalize(direction_vector);
    }
    void assignRay(Vector3D init, Vector3D dir)
    {
        initial_position = init;
        direction_vector = dir;
        normalise();
    }
    void assignRay(double x_pos, double y_pos, double z_pos, double x_dir, double y_dir, double z_dir)
    {
        Vector3D init(x_pos, y_pos, z_pos);
        Vector3D dirn(x_dir, y_dir, z_dir);
        assignRay(init, dirn);
        normalise();
    }
};

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

    double find_intersecting_value_t(Ray ray)
    {
//        cout << "Inside find_intersecting_value_t of sphere ... " ;

        double t = NULL_VALUE_T;

        //quadratic equation at^2 + bt + c = 0
        Vector3D initial_pos = ray.initial_position;
        Vector3D direction_vect = ray.direction_vector;
        direction_vect = vectorNormalize(direction_vect);

        double a = vectorDotProduct(initial_pos, initial_pos);
        double b = 2 * vectorDotProduct(direction_vect, initial_pos);
        double c = vectorDotProduct(initial_pos, initial_pos) - (radius * radius) ;

        double discriminant = (b * b) - (4.0 * a * c);

//        cout << "discriminant = " << discriminant << " a = " << a << " b = " << b << " c = " << c ;

        if(discriminant < 0){
            return NULL_VALUE_T;
        }
        //else
        discriminant = sqrt(discriminant);

        double t1 = (-b - discriminant)/(2.0 * a);
        double t2 = (-b + discriminant)/(2.0 * a);

//        cout << "  ,  t1 = " << t1 << " , t2 = " << t2;

        t = NULL_VALUE_T; //initialize as null
        if(t1 >= 0){
            t = t1; //for now initialize as t1
            if(t2 >= 0){
                t = min(t1, t2); //take the minimum of both ONLY if t2 is >= 0
            }
        }else{ // t1 is < 0
            if(t2 >= 0){
                t = t2; //simply t2 is now value of t
            }
        }

//        cout << " , finally returning t = " << t << endl;

        return t;
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

class Matrix
{
public:
    double elements[3][3];
    void printMatrix()
    {
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                cout << elements[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    double getDeterminant()
    {
        double det1 = elements[0][0] * ( (elements[1][1] * elements[2][2]) - (elements[2][1] * elements[1][2]) );
        double det2 = elements[0][1] * ( (elements[1][0] * elements[2][2]) - (elements[1][2] * elements[2][0]) );
        double det3 = elements[0][2] * ( (elements[1][0] * elements[2][1]) - (elements[2][0] * elements[1][1]) );

        return (det1 - det2 + det3);
    }
};

//Surface
class Triangle
{
public:
    int id_triangle = -1; //debug
    Vector3D point1, point2, point3;
    Vector3D colors;
    void assignTriangle(Vector3D p1, Vector3D p2, Vector3D p3){point1 = p1; point2 = p2; point3 = p3;}
    void assignTriangle(Vector3D p1, Vector3D p2, Vector3D p3, Vector3D col){
        point1 = p1; point2 = p2; point3 = p3;
        colors.x = col.x; colors.y = col.y; colors.z = col.z;
    }
    void assignColors(double c1, double c2, double c3){
        colors.x = c1; colors.y = c2; colors.z = c3;
    }
    void printTriangle(){
        cout << "Triangle ID: " << id_triangle << endl;
        point1.printVector(false);
        cout << " ";
        point2.printVector(false);
        cout << " ";
        point3.printVector(false);
        cout << "Colors <R,G,B> " << colors.x << " " << colors.y << " " << colors.z;
        cout << endl;
    }
    double find_intersecting_value_t(Ray ray)
    {
        //find CLOSEST intersecting point with THIS ray.
        ray.normalise(); //normalize the direction vector
        Vector3D direction_vect = ray.direction_vector;

        Matrix A;
        A.elements[0][0] = point1.x - point2.x;
        A.elements[0][1] = point1.x - point3.x;
        A.elements[0][2] = direction_vect.x;
        A.elements[1][0] = point1.y - point2.y;
        A.elements[1][1] = point1.y - point3.y;
        A.elements[1][2] = direction_vect.y;
        A.elements[2][0] = point1.z - point2.z;
        A.elements[2][1] = point1.z - point3.z;
        A.elements[2][2] = direction_vect.z;

        Matrix mat_for_beta;
        mat_for_beta.elements[0][0] = point1.x - ray.initial_position.x;
        mat_for_beta.elements[0][1] = point1.x - point3.x;
        mat_for_beta.elements[0][2] = direction_vect.x;
        mat_for_beta.elements[1][0] = point1.y - ray.initial_position.y;
        mat_for_beta.elements[1][1] = point1.y - point3.y;
        mat_for_beta.elements[1][2] = direction_vect.y;
        mat_for_beta.elements[2][0] = point1.z - ray.initial_position.z;
        mat_for_beta.elements[2][1] = point1.z - point3.z;
        mat_for_beta.elements[2][2] = direction_vect.z;

        Matrix mat_for_gamma;
        mat_for_gamma.elements[0][0] = point1.x - point2.x;
        mat_for_gamma.elements[0][1] = point1.x - ray.initial_position.x;
        mat_for_gamma.elements[0][2] = direction_vect.x;
        mat_for_gamma.elements[1][0] = point1.y - point2.y;
        mat_for_gamma.elements[1][1] = point1.y - ray.initial_position.y;
        mat_for_gamma.elements[1][2] = direction_vect.y;
        mat_for_gamma.elements[2][0] = point1.z - point2.z;
        mat_for_gamma.elements[2][1] = point1.z - ray.initial_position.z;
        mat_for_gamma.elements[2][2] = direction_vect.z;

        Matrix mat_for_t;
        mat_for_t.elements[0][0] = point1.x - point2.x;
        mat_for_t.elements[0][1] = point1.x - point3.x;
        mat_for_t.elements[0][2] = point1.x - ray.initial_position.x;
        mat_for_t.elements[1][0] = point1.y - point2.y;
        mat_for_t.elements[1][1] = point1.y - point3.y;
        mat_for_t.elements[1][2] = point1.y - ray.initial_position.y;
        mat_for_t.elements[2][0] = point1.z - point2.z;
        mat_for_t.elements[2][1] = point1.z - point3.z;
        mat_for_t.elements[2][2] = point1.z - ray.initial_position.z;

        double alpha, beta, gamma, t;

        beta = mat_for_beta.getDeterminant() / A.getDeterminant();
        gamma = mat_for_gamma.getDeterminant() / A.getDeterminant();
        alpha = 1.0 - (beta + gamma);
        t = mat_for_t.getDeterminant() / A.getDeterminant();

        if(t >= 0){
            //intersects ...
            //each should be greater or equal than 0
            if((alpha >= 0) && (beta >= 0) && (gamma >= 0)){
                //Inside the triangle THIS intersecting point lies.
                return t;
            }
        }
        //else return NULL_VALUE_T
        return NULL_VALUE_T;
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
        mid_point.y = 0.5 * (bottom_left.y + top_left.y);
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
        for(int i=0; i<NUM_TRIANGLES_IN_PYRAMID; i++){
            triangles[i].colors.x = colors[0];
            triangles[i].colors.y = colors[1];
            triangles[i].colors.z = colors[2];
        }
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
        cout << endl;
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
    void printCheckerBoardTile()
    {
        cout << "Left-most point:";
        left_most_point.printVector(false);
        cout << endl << "Color:" << color << endl;
    }
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

//Rotate things
double scalar_upDown = 3;
double scalar_forwardBackward = 3;
double scalar_rightLeft = 5;


///------------------------- Global Variables End --------------------------------

void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}
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

void drawSquare(double b)
{
    double a = (double)(b * 0.5);
    //glColor3f(1.0,0.0,0.0);
//    glPushMatrix();
//    glTranslatef(translation_vect.x, translation_vect.y, translation_vect.z);
    glBegin(GL_QUADS);
    {
        glVertex3f( a, a,0);
        glVertex3f( a,-a,0);
        glVertex3f(-a,-a,0);
        glVertex3f(-a, a,0);
    }
    glEnd();
//    glPopMatrix();
}

void drawSphere(double radius)
{
    double slices = 24;
    double stacks = 30;
	Vector3D points[100][100];
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
//        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
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

void initialiseParams()
{
    //------------ Just like in Assignment 1 -------------
    u.assignVector(0, 0, 1);
    r.assignVector(-1/sqrt(2), 1/sqrt(2), 0);
    l.assignVector(-1/sqrt(2), -1/sqrt(2), 0);
//    pos.assignVector(100, 100, 0);
//    pos.assignVector(50,50,100); //50,50,100
    pos.assignVector(100,100,50);
}

void printAllData()
{
    cout << "----------------------- Printing Data Begin ---------------------" << endl ;
    cout << "Printing Spheres:\n";
    for(int i=0; i<spheres_list.size(); i++){
        spheres_list[i].printSphere();
    }
    cout << "\nPrinting Pyramids\n";
    for(int i=0; i<pyramids_list.size(); i++){
        pyramids_list[i].printPyramid();
    }
//    cout << "\nPrinting Checkerboard\n";
//    for(int i=0; i<NUM_TILES; i++){
//        for(int j=0; j<NUM_TILES; j++){
//            checker_board[i][j].printCheckerBoardTile();
//        }
//    }
    cout << "\nPrinting Light Sources num_light_sources = " << num_light_sources << "\n";
    for(int i=0; i<num_light_sources; i++){
        light_sources[i].printVector();
    }

    cout << endl << "----------------------- Printing Data ENDS ---------------------" << endl ;
}

void loadCheckerBoard()
{
    checker_board[0][0] = CheckerBoardTile(WHITE);
    checker_board[0][0].left_most_point.assignVector(0, 0, 0); //origin as left-most point having WHITE color

    Vector3D point_left_most(0, 0, 0); //the left-most point
    int col;

    double x_last, y_last;
    x_last = -INFINITE_INDEX * WIDTH_CHECKER_BOARD;
    y_last = -INFINITE_INDEX * WIDTH_CHECKER_BOARD;

    ///---------- NOTHING IS DONE FOR NOW -------------------
}
string trim(const string& str) //trim string function
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

void loadAllData()
{
    initialiseParams(); //load look, right, up, pos vectors just like in assignment 1
    //Initial parameters reading
    fin >> recursion_level;
    fin >> num_pixels_along_axes;
    fin >> num_objects;

//    cout << "----->> RECURSION LEVEL = " << recursion_level << ", NUM_PIXELS_ALONG_AXES = " << num_pixels_along_axes << ", NUM_OBJ = " << num_objects << endl;

    //Now read the objects
    string inputString;

    double x, y, z, c_r, c_g, c_b, base, height, radius, coef_am, coef_dif, coef_spec, coef_ref, spec_exp;
    //Objects reading
    int num_ob_rec_so_far = 0;
    while(num_ob_rec_so_far < num_objects){
        getline(fin, inputString); // Saves the line in inputString [input command]
//        cout << "----->>>getLIne inputsString got is <" << inputString << ">\n";
        if(inputString == ""){
            continue;
        }
        if(inputString == "\n"){
            continue;
        }
//        cout << "===+++>>>> Now num_ob_so_far = " << num_ob_rec_so_far << endl;
        num_ob_rec_so_far++;
        if(trim(inputString) == "pyramid"){
            //load pyramid
//            cout << "---------->>> PYRAMID READING\n";
            fin >> x >> y >> z; //lowest points
            fin >> base >> height; //base length and height
            fin >> c_r >> c_g >> c_b; //colors r,g,b
            fin >> coef_am >> coef_dif >> coef_spec >> coef_ref; //co-efficients of ambient, diffuse, specular, reflection
            fin >> spec_exp; //specular exponent
            Pyramid pyramid;
            pyramid.formPyramid(x, y, z, base, height, c_r, c_g, c_b, coef_am, coef_dif, coef_spec, coef_ref, spec_exp);
            pyramid.id = num_ob_rec_so_far - 1;
            pyramids_list.push_back(pyramid);
        }
        else if(trim(inputString) == "sphere"){
            //load sphere
//            cout << "---------->>> SPHERE READING\n";
            fin >> x >> y >> z; //center
            fin >> radius; //radius
            fin >> c_r >> c_g >> c_b; //colors r,g,b
            fin >> coef_am >> coef_dif >> coef_spec >> coef_ref; //co-efficients of ambient, diffuse, specular, reflection
            fin >> spec_exp; //specular exponent

            Sphere sphere;
            sphere.id = num_ob_rec_so_far - 1;
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
    printAllData();
}

void drawCheckerBoard()
{
    int col_inner = 1, col_outer = 1;
    int deb_val = INFINITE_INDEX; //1000
    int checkerBoardWidth = WIDTH_CHECKER_BOARD;
//    int i = 0;
    for(int i= -deb_val; i<deb_val; i+=checkerBoardWidth){
        col_inner = col_outer;
        for(int j= -deb_val; j<deb_val; j+=checkerBoardWidth){
//            printf("-->> Value at (%d, %d), col = %d\n", i, j, col);
            glColor3f(col_inner, col_inner, col_inner); //1,1,1 -> white
            glPushMatrix();
            glTranslatef((double)j, (double)i, 0.0);
            drawSquare(checkerBoardWidth);
            glPopMatrix();
            if(col_inner == 1){col_inner = 0;}
            else{col_inner = 1;}

//            drawSquare(vect, CHECKER_BOARD_WIDTH);
        }
        if(col_outer == 1){col_outer = 0;}
        else{col_outer = 1;}
    }
}

void drawTriangle(Triangle triangle)
{
    Vector3D p1; Vector3D p2; Vector3D p3;
    p1 = triangle.point1;
    p2 = triangle.point2;
    p3 = triangle.point3;
//    printf("--->>About to draw triangle ... \n");
//    cout << "p1:"; p1.printVector();
//    cout << "p2:"; p2.printVector();
//    cout << "p3:"; p3.printVector();
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
        glVertex3f(p3.x, p3.y, p3.z);
    }
    glEnd();
}

void drawPyramids()
{
    for(int i=0; i<pyramids_list.size(); i++){ //for each pyramid
        Pyramid pyramid = pyramids_list[i];
        glColor3f(pyramid.colors[0], pyramid.colors[1], pyramid.colors[2]);
        for(int j=0; j<NUM_TRIANGLES_IN_PYRAMID; j++){ //for each triangle of $pyramid
            Triangle triangle = pyramid.triangles[j];
            drawTriangle(triangle);
        }
    }
}
void drawSpheres()
{
    for(int i=0; i<spheres_list.size(); i++){
        Sphere sphere = spheres_list[i];
        glColor3f(sphere.colors[0], sphere.colors[1], sphere.colors[2]);
        glPushMatrix();
//        printf("--->>Sphere radius = %lf, <%lf, %lf, %lf>\n", sphere.radius, sphere.center.x, sphere.center.y, sphere.center.z);
        glTranslatef(sphere.center.x, sphere.center.y, sphere.center.z);
        drawSphere(sphere.radius);
        glPopMatrix();
    }
}

void drawCircle(double radius)
{
    int segments = 50;
    int i;
    Vector3D points[100];

    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}
void drawDebugCircle(double x, double y, double z)
{
    glPushMatrix();
    {
        glTranslatef(x, y, z);
    }
    drawCircle(10);
    glPopMatrix();
}

void drawLightSources()
{
    for(int i=0; i<num_light_sources; i++){
        Vector3D light = light_sources[i];
        glPushMatrix();
        {
            glColor3f(1, 0, 1); //white
            glTranslatef(light.x, light.y, light.z);
            drawSphere(2);
        }
        glPopMatrix();
    }
}

void drawAllObjects()
{
    //Draw all objects in opengl
    drawCheckerBoard(); //draw checker-board
    drawPyramids(); //draw pyramids
    drawSpheres(); //draw spheres
    drawLightSources(); //draw light-sources
//    glColor3f(1,0,0);
//    drawDebugCircle(50, 50, 100); //debugger circle
//    glColor3f(1, 0, 0);
//    drawDebugCircle(50, 50, 0); //debugger circle
}


///------------------------------- My Functions End ---------------------------------------

//---------- Further global variables --------------
Vector3D pixel_window_mid_points[PIXEL_NUM][PIXEL_NUM];
double fovX = 90, fovY = 90;

void computePixelsWindow()
{
    //near plane
    Vector3D mid_point_pixel_window;

    //determine mid-point of the pixel_window by look * nearDistance
    mid_point_pixel_window = vectorAddition(pos, vectorScale(l, nearDistance)); //camera-eye + look * nearDistance

    //now determine horizontal and vertical i.e. width and height
    double width_pixel_window  = 2.0 * (double)(nearDistance * tan(DEGREE_TO_RAD(fovX * 0.5))); // width = nearDistance * tan(fovX/2)
    double height_pixel_window = 2.0 * (double)(nearDistance * tan(DEGREE_TO_RAD(fovY * 0.5))); // width = nearDistance * tan(fovY/2)


    double increment_width  = (double)(width_pixel_window  / PIXEL_NUM);
    double increment_height = (double)(height_pixel_window / PIXEL_NUM);

    cout << "Inside drawPixelsWindow() ... \n";
    cout << "--->> Width of pixel_window = " << width_pixel_window << " , Height of pixel_window = " << height_pixel_window << endl;
    cout << "-->Mid-point of pixel window is "; mid_point_pixel_window.printVector();
    cout << "Here, increment_width = " << increment_width << ", increment_height = " << increment_height << endl;

    //384 down, 384 left
    double to_move_left = increment_width * (PIXEL_NUM / 2);
    double to_move_down = increment_height * (PIXEL_NUM / 2);

    cout << "To Move left = " << to_move_left << ", to move down = " << to_move_down << endl;

//    ------------------------------------------------------------- TO DO ------------------------------------

//    Vector3D bottom_most_left_point = vectorAddition(mid_point_pixel_window, vectorAddition(vectorScale(r, -(to_move_left * 1.0)), vectorScale(u, -(to_move_down * 1.0))));

    cout << "Here, u = "; u.printVector(false); cout << " , r = "; r.printVector();

    Vector3D bottom_most_left_point = vectorAddition(mid_point_pixel_window, vectorAddition(vectorScale(u, -1), vectorScale(r, -1)));


    Vector3D bottom_most_left_mid_point = vectorAddition(bottom_most_left_point,
                                                         vectorAddition(vectorScale(r, (0.5 * increment_width)),
                                                                        vectorScale(u, (0.5 * increment_height))));

    cout << "BOTTOM MOST LEFT POINT "; bottom_most_left_point.printVector();
    cout << "Bottom most left mid-point :"; bottom_most_left_mid_point.printVector();

    //make each zero
    for(int i=0; i<PIXEL_NUM; i++){
        for(int j=0; j<PIXEL_NUM; j++){
            pixel_window_mid_points[i][j].assignVector(0, 0, 0);
        }
    }

    for(int i=0; i<PIXEL_NUM; i++){
        for(int j=0; j<PIXEL_NUM; j++){
            pixel_window_mid_points[i][j] = vectorAddition(
                                        vectorAddition(bottom_most_left_mid_point, vectorScale(r, (j * increment_width))),
                                        vectorScale(u, (i * increment_height)));
        }
    }

    cout << "FIRST POINT IS: "; pixel_window_mid_points[0][0].printVector();
    cout << "LAST POINT IS: "; pixel_window_mid_points[PIXEL_NUM - 1][PIXEL_NUM - 1].printVector();

#ifdef DEBUG_MID_POINTS
    #define DEBUG_MID_POINTS 0
#endif // DEBUG_MID_POINTS


#if DEBUG_MID_POINTS == 1
    ofstream out;
    out.open("mid_points.txt");
    for(int i=0; i<PIXEL_NUM; i++){
        for(int j=0; j<PIXEL_NUM; j++){
//            cout << "for i = " << i << " , j = " << j << endl;
            out << "Index:(" << i << "," << j << ") -> " << pixel_window_mid_points[i][j].x << ", " << pixel_window_mid_points[i][j].y << ", " << pixel_window_mid_points[i][j].z << endl;
        }
//        cout << endl << endl;
        out << endl << endl;
    }
    out.close();
#endif // DEBUG_MID_POINTS

}

int color_of_pixel_checker_board(Vector3D point)
{
    int col_inner = 1, col_outer = 1;
    int deb_val = INFINITE_INDEX; //1000
    int checkerBoardWidth = WIDTH_CHECKER_BOARD;

    int color = -1;

    for(int i= -deb_val; i<deb_val; i+=checkerBoardWidth){
        col_inner = col_outer;
        for(int j= -deb_val; j<deb_val; j+=checkerBoardWidth){
//            printf("-->> Value at (%d, %d), col = %d\n", i, j, col);
            glColor3f(col_inner, col_inner, col_inner); //1,1,1 -> white
            glPushMatrix();
            glTranslatef((double)j, (double)i, 0.0);
            drawSquare(checkerBoardWidth);
            glPopMatrix();

            //check if within bounds ...
            double left_x, right_x, down_y, up_y;
            left_x = j - (0.5 * checkerBoardWidth);
            right_x = j + (0.5 * checkerBoardWidth);
            down_y = i - (0.5 * checkerBoardWidth);
            up_y = i + (0.5 * checkerBoardWidth);

            if((point.x <= right_x) && (point.x >= left_x)){
                if((point.y <= up_y && (point.y >= down_y))){
                    //IS_WITHIN_BOUNDS
                    if(col_inner == 1){//WHITE
                        return 1;
                    }else{
                        return 0;
                    }
                }
            }


            //change turns of color
            if(col_inner == 1){col_inner = 0;}
            else{col_inner = 1;}
        }
        if(col_outer == 1){col_outer = 0;}
        else{col_outer = 1;}
    }


    return 1; //normally return WHITE
}

ofstream pixel_deb; //for pixel debugging ...
bitmap_image image_bitmap_pixel;
Vector3D pixels_image_colors[PIXEL_NUM][PIXEL_NUM];

Vector3D find_intersection_color_for_each_pixel(Ray ray, int row_val, int col_val) //ray.initial_position contains the initial position
{
    ray.normalise(); //normalize just in case !! [LoL]
    //FOR EACH OBJECT ... first start with triangle's surface
    double min_t = MAX_VAL; //MAX VALUE
    double t = MAX_VAL;
    Vector3D colors_so_far;
    colors_so_far.assignVector(0, 0, 0);

    bool is_triangle = false, is_sphere = false, is_checker = false;
    //Triangles...
    for(int i=0; i<pyramids_list.size(); i++){
        for(int j=0; j<NUM_TRIANGLES_IN_PYRAMID; j++){
            //FOR EACH TRIANGLE ...
            Triangle triangle = pyramids_list[i].triangles[j];
            double temp = triangle.find_intersecting_value_t(ray);
            if(temp != NULL_VALUE_T){
                t = temp;
                if(t < min_t){
//                    cout << "--->>>TRIANGLE MATCH for i = " << i << " , j = " << j << " , col : " << triangle.colors.x << " " << triangle.colors.y << " " << triangle.colors.z << endl;

                    is_triangle = true;
                    min_t = t; //min_t stores t.
                    colors_so_far.assignVector(triangle.colors.x, triangle.colors.y, triangle.colors.z); ////assign the color of this triangle
                }
            }
        }
    }
    //Spheres ...
    for(int i=0; i<spheres_list.size(); i++){
        Sphere sphere = spheres_list[i];
        t = sphere.find_intersecting_value_t(ray);
        if(t != NULL_VALUE_T){
            //if not null then compare if min or not
            if(t < min_t){
                is_triangle = false;
                is_sphere = true;
//                cout << "++--++-->> SPHERE MATCH for i = " << row_val << ", j = " << col_val << " , col: " << sphere.colors[0] << " " << sphere.colors[1] << " " << sphere.colors[2] << endl;
                min_t = t;
                colors_so_far.assignVector(sphere.colors[0], sphere.colors[1], sphere.colors[2]); //assign the color of this sphere
            }
        }
    }
    //Checkerboard...
    //O_z + t.direction_z = 0 --> find value of t
    bool is_checker_board = false;
    t = -(ray.initial_position.z) / (ray.direction_vector.z);
    if(t < min_t){
//        cout << "====-->>FLOOR MATCH FOR i = " << row_val << " , j = " << col_val << endl;
        is_checker = true;
        is_sphere = false;
        min_t = t;
        is_checker_board = true;
    }

    //Find the intersecting point's co-ordinates

    Vector3D intersection_point;
    if(min_t != MAX_VAL){
        intersection_point = vectorAddition(ray.initial_position, vectorScale(ray.direction_vector, min_t));
    }else{
        intersection_point.assignVector(farDistance, farDistance, farDistance);
    }

//    cout << "--==---==->FINALLY for i = " << row_val << " , j = " << col_val << " , isChecker = " << is_checker << " , is_sphere = " << is_sphere << " , is_triangle = " << is_triangle << endl << endl;

    //check if checker_board ... then find which color i.e. WHITE or BLACK
    if(is_checker_board == true)
    {
        //find which color's tile is this vector in.
        double color_ret = color_of_pixel_checker_board(intersection_point);
        colors_so_far.assignVector(color_ret, color_ret, color_ret);
    }

//    pixel_deb << "IntersectionPoint:" << intersection_point.x << " " << intersection_point.y << " " << intersection_point.z;
//    pixel_deb << " Col: " << colors_so_far.x << " " << colors_so_far.y << " " << colors_so_far.z << endl;
//    cout << "t = " << t << " , min_t = " << min_t << " , COL: " << colors_so_far.x << " " << colors_so_far.y << " " << colors_so_far.z << endl;
    //Spheres....
    //pixels_image_colors[row_val][col_val].assignVector(colors_so_far.x, colors_so_far.y, colors_so_far.z);
    return colors_so_far;
}

void find_intersection_points()
{
//FOR EACH PIXEL
    cout << "--->>Inside find_intersection_points ... saving image " << endl ;
    Ray ray;
    Vector3D initial_pos, direction_vect, pixel_pos;

    initial_pos.assignVector(pos.x, pos.y, pos.z);

    //debugging with only one pixel


    for(int i=0; i<PIXEL_NUM; i++){
        for(int j=0; j<PIXEL_NUM; j++){
            pixel_pos = pixel_window_mid_points[i][j];
            direction_vect.x = pixel_pos.x - initial_pos.x;
            direction_vect.y = pixel_pos.y - initial_pos.y;
            direction_vect.z = pixel_pos.z - initial_pos.z;
            direction_vect = vectorNormalize(direction_vect);
//            cout << "For i = " << i << " , j = " << j << " " ;
//            pixel_deb << "Idx:(" << i << "," << j << ")" ;
            ray.assignRay(initial_pos, direction_vect); //initialize the ray.
//            Vector3D color_this_pixel = find_intersection_color_for_each_pixel(ray, i, j);

//            printf("To set pic i = %d, j = %d window .. x = %lf, y = %lf, pixel colors = %lf, %lf, %lf\n",i, j, pixel_window_mid_points[i][j].x, pixel_window_mid_points[i][j].y, pixels_image_colors[i][j].x,
//                   pixels_image_colors[i][j].y, pixels_image_colors[i][j].z);
//            image_bitmap_pixel.set_pixel(767 - i, j, color_this_pixel.x, color_this_pixel.y, color_this_pixel.z);
                        image_bitmap_pixel.set_pixel(767 - i, j, 255, 255, 255);
            //image_bitmap_pixel.set_pixel(pixel_window_mid_points[i][j].x, pixel_window_mid_points[i][j].y, pixels_image_colors[i][j].x, pixels_image_colors[i][j].y, pixels_image_colors[i][j].z);
//            image_bitmap_pixel.set_pixel(i, j, pixels_image_colors[767-i][j].x, pixels_image_colors[767-i][j].y, pixels_image_colors[767-i][j].z);
        }
//        pixel_deb << endl << endl;
        //if(i%10 == 0){
        //    cout << "Row " << i << " is complete\n";
        //}
    }
    cout << "Printing to file pixel_deb.txt done .. PRINTING IMAGE" << endl;
    image_bitmap_pixel.save_image("out_test.bmp");
}


void captureImage()
{
//    cout << "-------->> TO DO ... capture image" << endl;

    computePixelsWindow(); //computePixelsWindow [most probably OK]
    find_intersection_points(); // find the points of intersection of each ray with each obj
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
        captureImage();
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
    case GLUT_KEY_UP:		///Move with respect to LOOK
        pos = vectorAddition(pos, vectorScale(l, scalar_forwardBackward));
        cout << "Now pos = "; pos.printVector();
        break;
    case GLUT_KEY_DOWN:		// up arrow key
        pos = vectorSubtraction(pos, vectorScale(l, scalar_forwardBackward));
        cout << "Now pos = "; pos.printVector();
        break;
    case GLUT_KEY_RIGHT:    ///Move with respect to RIGHT
        pos = vectorAddition(pos, vectorScale(r, scalar_rightLeft));
        cout << "Now pos = "; pos.printVector();
        break;
    case GLUT_KEY_LEFT:
        pos = vectorSubtraction(pos, vectorScale(r, scalar_rightLeft));
        cout << "Now pos = "; pos.printVector();
        break;
    case GLUT_KEY_PAGE_UP:  ///Move with respect to UP
        pos = vectorAddition(pos, vectorScale(u, scalar_upDown));
        cout << "Now pos = "; pos.printVector();
        break;
    case GLUT_KEY_PAGE_DOWN:
        pos = vectorSubtraction(pos, vectorScale(u, scalar_upDown));
        cout << "Now pos = "; pos.printVector();
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
        if(state == GLUT_DOWN){
            drawgrid = 1 - drawgrid;
        }
        break;

    default:
        break;
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
//    drawaxes=1;
//    cameraHeight=150.0;
//    cameraAngle=1.0;
    angle=0;
    drawaxes = 1;
    drawgrid = 0;
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
//    gluPerspective(80,	1,	1,	1000.0);
    gluPerspective(field_angle,	1,	1,	1000.0); //field_angle = 90
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
    pixel_deb.open("pixel_deb.txt");
    image_bitmap_pixel.setwidth_height(PIXEL_NUM, PIXEL_NUM);
    ///Initialize pos, u, l, and r done


    glutInit(&argc,argv);
    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);   //glutInitWindowSize(500, 500);
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
