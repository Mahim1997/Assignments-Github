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

#define CHECKER_BOARD_ULTIMATE_VAL 900

#define AMBIENT_INDEX 0
#define DIFFUSE_INDEX 1
#define SPECULAR_INDEX 2
#define REFLECTION_INDEX 3

#define MAX_VAL 9999999
#define NULL_VALUE_T -1000

#define DRAW_GRID 1

#define WHITE 7
#define BLACK 10

#define DEGREE_ANGLE_INIT 1.0  //movement angle

#define SPHERE_TYPE 1
#define TRIANGLE_TYPE 2
#define FLOOR_TYPE 3

#define EPSILON_COMPARISON 0.00001  // for comparison of two doubles or floats = 0.00001
#define NUM_TRIANGLES_IN_PYRAMID 6  // NUM_TRIANGLES_IN_PYRAMID is 6 [4 triangles + square base (2 triangles)]

#define pi (2*acos(0.0))
#define DEGREE_TO_RAD(x) ((x * pi) / 180)
#define RAD_TO_DEGREE(x) (x * 180 / pi)

#define WIDTH_CHECKER_BOARD 30 //width of each tile of checker-board is 30
#define INFINITE_INDEX 1000

#define PIXEL_NUM 768 //pixel-window-size = 768 X 768
//#define PIXEL_NUM 200 //debug

using namespace std ;

///FOR DEBUGGING one pixel purposes
int ROW_TO_DEB = 368;
int COL_TO_DEB = 456;
int row_idx_deb ;//= 329;
int col_idx_deb ;//= 446;

bool IS_DEBUG_CONDITION()
{
    return ( (row_idx_deb == ROW_TO_DEB) && (col_idx_deb == COL_TO_DEB) ) ;
}

int drawaxes;
int angle;
///------------------- My global variables and functions -----------------
ifstream fin; // for description.txt reading

double FLOOR_COEFFICIENTS[4] = {0.4, 0.2, 0.2, 0.2};
double FLOOR_SPECULAR_EXPONENT = 1.0;

bool isEqual(double val1, double val2, double threshold = EPSILON_COMPARISON)  //Is equal wrt the threshold?
{
    if(abs(val1 - val2) < threshold)
    {
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
    void assignVector(Vector3D v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
    }
    void printVector(bool flag = false)
    {
        if(flag == true)
        {
            cout << x << " " << y << " " << z ;
        }
        else
        {
            cout << x << " " << y << " " << z << endl ;
        }
    }

    double magnitude()
    {
        double ans = (x*x) + (y*y) + (z*z);
        ans = sqrt(ans);
        return ans;
    }
    void normalise()
    {
        double factor = this->magnitude();
        factor = (double)(1.0 / factor);
        this->x = this->x * factor;
        this->y = this->y * factor;
        this->z = this->z * factor;
    }
};
bool isEqualVector(Vector3D p1, Vector3D p2, double thresh = EPSILON_COMPARISON)
{
    if(isEqual(p1.x, p2.x, thresh) == false)
    {
        return false;
    }
    if(isEqual(p1.y, p2.y, thresh) == false)
    {
        return false;
    }
    if(isEqual(p1.z, p2.z, thresh) == false)
    {
        return false;
    }
    //All equal
    return true;
}
//-------------------------------------- Vector functions begin ------------------------------
double vectorGetDistanceBetweenTwo(Vector3D a, Vector3D b)
{
    double dist = 0;
    dist = ((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)) + ((a.z - b.z) * (a.z - b.z)) ;
    dist = sqrt(dist);
    return dist;
}

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
int recursion_level = 1; //default -> 1 [READ FROM FILE]
int num_pixels_along_axes;
int num_objects;

//Rotate things
double scalar_upDown = 3;
double scalar_forwardBackward = 3;
double scalar_rightLeft = 5;


///------------------------- Global Variables End --------------------------------


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
    void printRay()
    {
        cout << "InitialPos[Ray]: ";
        initial_position.printVector(false);
        cout << " , DirectionVect[Ray]: ";
        direction_vector.printVector(false);
        cout << " , mag_direction_vec = " << direction_vector.magnitude() << endl;
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
        for(int i=0; i<3; i++)
        {
            colors[i] = 0.0;   //initalise as black
        }
    }
    void assignCoefficients(double a, double d, double s, double r)
    {
        co_efficients[0] = a;
        co_efficients[1] = d;
        co_efficients[2] = s;
        co_efficients[3] = r;
    }
    void assignColors(double r, double b, double g)
    {
        colors[0] = r;
        colors[1] = b;
        colors[2] = g;
    }
    void assignSpecularExponent(double sp)
    {
        specular_exponent = sp;
    }
    void assignRadius(double r)
    {
        radius = r;
    }
    void assignCenter(double x, double y, double z)
    {
        center.x = x;
        center.y = y;
        center.z = z;
    }

    void formSphere(double cx, double cy, double cz, double r, double c_r, double c_g, double c_b,
                    double am, double dif, double sp, double refl, double spec_exp)
    {
        assignCenter(cx, cy, cz);
        assignRadius(r);
        assignColors(c_r, c_g, c_b);
        assignCoefficients(am, dif, sp, refl);
        assignSpecularExponent(spec_exp);
    }

    double find_intersecting_value_t(Ray ray, double near_distance, double far_distance)
    {
//        cout << "Inside find_intersecting_value_t of sphere ... " ;

        double t = NULL_VALUE_T;

        //quadratic equation at^2 + bt + c = 0
        Vector3D initial_pos = vectorAddition(ray.initial_position, vectorScale(center, -1.0)); //translate to center
        Vector3D direction_vect = ray.direction_vector;
        direction_vect = vectorNormalize(direction_vect);

        double a = 1.0;//double a = vectorDotProduct(direction_vect, direction_vect);
        double b = 2 * vectorDotProduct(direction_vect, initial_pos);
        double c = (initial_pos.magnitude() * initial_pos.magnitude()) - (radius * radius);  //double c = vectorDotProduct(initial_pos, initial_pos) - (radius * radius) ;

        double discriminant = (b * b) - (4.0 * a * c);

        if(discriminant < 0)
        {
            return NULL_VALUE_T;
        }
        //else
        discriminant = sqrt(discriminant);

        double t1 = (-b - discriminant)/(2.0 * a);
        double t2 = (-b + discriminant)/(2.0 * a);


        t = NULL_VALUE_T; //initialize as null
        double dist1 = MAX_VAL, dist2 = MAX_VAL; //to take which 't'

        if(t1 >= 0)  //compare with ACTUAL eye/camera position
        {
            Vector3D point1 = vectorAddition(ray.initial_position, vectorScale(ray.direction_vector, t1));
            dist1 = vectorGetDistanceBetweenTwo(ray.initial_position, point1);
        }
        if(t2 >= 0)
        {
            Vector3D point2 = vectorAddition(ray.initial_position, vectorScale(ray.direction_vector, t2));
            dist2 = vectorGetDistanceBetweenTwo(ray.initial_position, point2);
        }
        if((dist1 < dist2) && (dist1 >= near_distance) && (dist1 <= far_distance))
        {
            t = t1;
        }
        else if((dist2 < dist1) && (dist2 >= near_distance) && (dist2 <= far_distance))
        {
            t = t2;
        }

        return t;
    }

    void printSphere()
    {
        cout << "------------------ Printing Sphere -------------- ID = " << id << "--------" << endl;
        cout << "Center:" << center.x << " " << center.y << " " << center.z << " " << endl;
        cout << "Radius:" << radius << ",";
        cout << "Color:";
        for(int i=0; i<3; i++)
        {
            cout << colors[i] << " ";
        }
        cout << endl;
        cout << "Coeffcients:";
        for(int i=0; i<4; i++)
        {
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
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<3; j++)
            {
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
    void assignTriangle(Vector3D p1, Vector3D p2, Vector3D p3)
    {
        point1 = p1;
        point2 = p2;
        point3 = p3;
    }
    void assignTriangle(Vector3D p1, Vector3D p2, Vector3D p3, Vector3D col)
    {
        point1 = p1;
        point2 = p2;
        point3 = p3;
        colors.x = col.x;
        colors.y = col.y;
        colors.z = col.z;
    }
    void assignColors(double c1, double c2, double c3)
    {
        colors.x = c1;
        colors.y = c2;
        colors.z = c3;
    }
    void printTriangle()
    {
        cout << "Triangle ID: " << id_triangle << endl;
        point1.printVector(false);
        cout << " ";
        point2.printVector(false);
        cout << " ";
        point3.printVector(false);
        cout << "Colors <R,G,B> " << colors.x << " " << colors.y << " " << colors.z;
        cout << endl;
    }
    double find_intersecting_value_t(Ray ray, double near_dist, double far_dist)
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

        if(t >= 0)
        {
            //intersects ...
            //each should be greater or equal than 0
            if((alpha >= 0) && (beta >= 0) && (gamma >= 0))
            {
                //Inside the triangle THIS intersecting point lies.
                //Check if WITHIN range DISTANCE
//                return t;
                ray.normalise(); ///TRIANGLE DISTANCE CHECKING
                Vector3D intersection_point = vectorAddition(ray.initial_position, vectorScale(ray.direction_vector, t));
                double dist = vectorGetDistanceBetweenTwo(intersection_point, ray.initial_position);
                if((dist >= near_dist) && (dist <= far_dist)){
                    return t;
                }
                else{
                    return NULL_VALUE_T;
                }
            }
        }
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
        this->typeOfObject = TRIANGLE_TYPE;
        for(int i=0; i<NUM_TRIANGLES_IN_PYRAMID; i++)
        {
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

    void assignCoefficients(double a, double d, double s, double r)
    {
        co_efficients[0] = a;
        co_efficients[1] = d;
        co_efficients[2] = s;
        co_efficients[3] = r;
    }
    void assignColors(double r, double b, double g)
    {
        colors[0] = r;
        colors[1] = b;
        colors[2] = g;
        for(int i=0; i<NUM_TRIANGLES_IN_PYRAMID; i++)
        {
            triangles[i].colors.x = colors[0];
            triangles[i].colors.y = colors[1];
            triangles[i].colors.z = colors[2];
        }
    }
    void assignSpecularExponent(double sp)
    {
        specular_exponent = sp;
    }

    void formPyramid(double lowest_x, double lowest_y, double lowest_z, double len_base, double len_height,
                     double color_r, double color_g, double color_b,
                     double co_am, double co_dif, double co_spec, double co_ref,
                     double spec_exp)
    {
        this->typeOfObject = TRIANGLE_TYPE;
        formTriangles(lowest_x, lowest_y, lowest_z, len_base, len_height);
        assignCoefficients(co_am, co_dif, co_spec, co_ref);
        assignColors(color_r, color_g, color_b);
        assignSpecularExponent(spec_exp);
    }

    void printPyramid()
    {
        cout << "------------------ Printing PYRAMID -------------- ID = " << id << "--------" << endl;

        cout << "BASE:" << endl;
        for(int i=0; i<2; i++)
        {
            triangles[i].printTriangle();
        }
        cout << "SIDES:" << endl;
        for(int i=2; i<6; i++)
        {
            triangles[i].printTriangle();
        }
        cout << endl;
        cout << "Color:";
        for(int i=0; i<3; i++)
        {
            cout << colors[i] << " ";
        }
        cout << endl;

        cout << "Coeffcients:";
        for(int i=0; i<4; i++)
        {
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
    CheckerBoardTile() {}
    CheckerBoardTile(double col)
    {
        color = col;
    }
    void printCheckerBoardTile()
    {
        cout << "Left-most point:";
        left_most_point.printVector(false);
        cout << endl << "Color:" << color << endl;
    }
};


///------------------------------ My Objects End ----------------------------------

///------------------------------ GLOBAL vars again begin ----------------------------------
// ------------------ Light Sources -------------
int num_light_sources;
vector<Vector3D> light_sources; //positions of light sources

// ---------------------- Objects list i.e. vector<Objects> ------------------
vector<Sphere> spheres_list;
vector<Pyramid> pyramids_list;

///------------------------------ GLOBAL vars again end ----------------------------------


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
    glBegin(GL_QUADS);
    {
        glVertex3f( 0, 0, 0);
        glVertex3f( 0, b, 0);
        glVertex3f( b, b, 0);
        glVertex3f( b, 0, 0);
    }
    glEnd();
}

void drawSphere(double radius)
{
    double slices = 24;
    double stacks = 30;
    Vector3D points[100][100];
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
//        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
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

void initialiseParams()
{
    //------------ Just like in Assignment 1 -------------
    u.assignVector(0, 0, 1);
    r.assignVector(-1/sqrt(2), 1/sqrt(2), 0);
    l.assignVector(-1/sqrt(2), -1/sqrt(2), 0);
    pos.assignVector(100,100,50);
}

void printAllShapes()
{
    cout << "----------------------- Printing Data Begin ---------------------" << endl ;
    cout << "RECURSION LEVEL REFLECTION: " << recursion_level << endl;
    cout << "Printing Spheres:\n";
    for(int i=0; i<spheres_list.size(); i++)
    {
        spheres_list[i].printSphere();
    }
    cout << "\nPrinting Pyramids\n";
    for(int i=0; i<pyramids_list.size(); i++)
    {
        pyramids_list[i].printPyramid();
    }
    cout << "\nPrinting Light Sources num_light_sources = " << num_light_sources << "\n";
    for(int i=0; i<num_light_sources; i++)
    {
        light_sources[i].printVector();
    }
    cout << "\nPrinting Checker-Board coefficients: \n";
    for(int i=0; i<4; i++){
        cout << coefficients_arr[i] << ":" << FLOOR_COEFFICIENTS[i] << "  ";
    }
    cout << " Floor Specular Exponent: " << FLOOR_SPECULAR_EXPONENT << endl;
    cout << "RECURSION LEVEL DEPTH: " << recursion_level << endl;
    cout << "WHITE Spheres indicate light sources" << endl;
    cout << endl << "----------------------- Printing Data ENDS ---------------------" << endl ;
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
    string inputString;

    double x, y, z, c_r, c_g, c_b, base, height, radius, coef_am, coef_dif, coef_spec, coef_ref, spec_exp;
    //Objects reading
    int num_ob_rec_so_far = 0;
    while(num_ob_rec_so_far < num_objects)
    {
        getline(fin, inputString); // Saves the line in inputString [input command]
        if(inputString == "")
        {
            continue;
        }
        if(inputString == "\n")
        {
            continue;
        }
        num_ob_rec_so_far++;
        if(trim(inputString) == "pyramid")
        {
            //load pyramid
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
        else if(trim(inputString) == "sphere")
        {
            //load sphere
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
    for(int i=0; i<num_light_sources; i++)
    {
        fin >> x >> y >> z;
        Vector3D vec(x, y, z);
        light_sources.push_back(vec);
    }


}

void drawCheckerBoard()
{
    int col_inner = 1, col_outer = 1;
    int deb_val = CHECKER_BOARD_ULTIMATE_VAL; //1000
    int checkerBoardWidth = WIDTH_CHECKER_BOARD;
    for(int i= -deb_val; i<deb_val; i+=checkerBoardWidth)
    {
        col_inner = col_outer;
        for(int j= -deb_val; j<deb_val; j+=checkerBoardWidth)
        {
            glColor3f(1 - col_inner, 1 - col_inner, 1 - col_inner); //To keep consistent with bmp image ...
            glPushMatrix();
            glTranslatef((double)j, (double)i, 0.0);
            drawSquare(checkerBoardWidth);
            glPopMatrix();
            if(col_inner == 1)
            {
                col_inner = 0;
            }
            else
            {
                col_inner = 1;
            }

        }
        if(col_outer == 1)
        {
            col_outer = 0;
        }
        else
        {
            col_outer = 1;
        }
    }
}

void drawTriangle(Triangle triangle)
{
    Vector3D p1;
    Vector3D p2;
    Vector3D p3;
    p1 = triangle.point1;
    p2 = triangle.point2;
    p3 = triangle.point3;
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
    for(int i=0; i<pyramids_list.size(); i++)  //for each pyramid
    {
        Pyramid pyramid = pyramids_list[i];
        glColor3f(pyramid.colors[0], pyramid.colors[1], pyramid.colors[2]);
        for(int j=0; j<NUM_TRIANGLES_IN_PYRAMID; j++)  //for each triangle of $pyramid
        {
            Triangle triangle = pyramid.triangles[j];
            drawTriangle(triangle);
        }
    }
}
void drawSpheres()
{
    for(int i=0; i<spheres_list.size(); i++)
    {
        Sphere sphere = spheres_list[i];
        glColor3f(sphere.colors[0], sphere.colors[1], sphere.colors[2]);
        glPushMatrix();
        glTranslatef(sphere.center.x, sphere.center.y, sphere.center.z);
        drawSphere(sphere.radius);
        glPopMatrix();
    }
}

void drawLightSources()
{
    for(int i=0; i<num_light_sources; i++)
    {
        Vector3D light = light_sources[i];
        glPushMatrix();
        {
            glColor3f(1, 1, 1); //white
            glTranslatef(light.x, light.y, light.z);
            drawSphere(1);
        }
        glPopMatrix();
    }
}

void drawAllObjects() ///Main draw all objects function
{
    //Draw all objects in opengl
    drawCheckerBoard(); //draw checker-board
    drawPyramids(); //draw pyramids
    drawSpheres(); //draw spheres
    drawLightSources(); //draw light-sources
}


///------------------------------- My Functions End ---------------------------------------

//---------- Further global variables --------------
Vector3D pixel_window_mid_points[PIXEL_NUM][PIXEL_NUM];
Vector3D bottom_most_left_point;
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


    //384 down, 384 left
    double to_move_left = increment_width * (PIXEL_NUM / 2);
    double to_move_down = increment_height * (PIXEL_NUM / 2);


    bottom_most_left_point = vectorAddition(mid_point_pixel_window, vectorAddition(vectorScale(u, -1), vectorScale(r, -1)));


    Vector3D bottom_most_left_mid_point = vectorAddition(bottom_most_left_point,
                                          vectorAddition(vectorScale(r, (0.5 * increment_width)),
                                                  vectorScale(u, (0.5 * increment_height))));

    //make each zero
    for(int i=0; i<PIXEL_NUM; i++)
    {
        for(int j=0; j<PIXEL_NUM; j++)
        {
            pixel_window_mid_points[i][j].assignVector(0, 0, 0);
        }
    }
    Vector3D top_moft_left_mid_point = vectorAddition(bottom_most_left_mid_point, Vector3D(0, (PIXEL_NUM - 1) * increment_height, 0));
    for(int i=0; i<PIXEL_NUM; i++)
    {
        for(int j=0; j<PIXEL_NUM; j++)
        {
            pixel_window_mid_points[i][j] = vectorAddition(
                                                vectorAddition(bottom_most_left_mid_point, vectorScale(r, (j * increment_width))),
                                                vectorScale(u, (i * increment_height)));
        }
    }
    ///Pixels calculations DONE
}

Vector3D color_of_pixel_checker_board(Vector3D p)
{
    Vector3D bottom_left_point_checker_board;
    bottom_left_point_checker_board.assignVector(-900.0, -900.0, 0.0);

    p.x = p.x + abs(bottom_left_point_checker_board.x);
    p.y = p.y + abs(bottom_left_point_checker_board.y);

    int checker_x = (int)(p.x/WIDTH_CHECKER_BOARD);
    int checker_y = (int)(p.y/WIDTH_CHECKER_BOARD);

    Vector3D color(0, 0, 0); //auto return black

    if((checker_x % 2) == 0)
    {
        if((checker_y) % 2 != 0)
        {
            color.assignVector(1, 1, 1); //make white
        }
    }
    else
    {
        if((checker_y) % 2 == 0)
        {
            color.assignVector(1, 1, 1); //make white
        }
    }
    return color;
}

bitmap_image image_bitmap_pixel; ///for image production
//Object for intersection things...
class IntersectionObject
{
public:
    double specular_exp;
    int type = -1;
    Vector3D normal;
    Vector3D color;
    Vector3D intersection_point;
    double co_efficients[4]; //co-efficients of those surfaces
    void assignIntersectionObject(Vector3D c, Vector3D ip, double ambient, double diffuse, double specular, double reflective, double sp)
    {
        intersection_point.assignVector(ip.x, ip.y, ip.z);
        co_efficients[0] = ambient;
        co_efficients[1] = diffuse;
        co_efficients[2] = specular;
        co_efficients[3] = reflective;
        specular_exp = sp;
    }
    void assignColor(Vector3D c)
    {
        color.assignVector(c.x, c.y, c.z);
    }
    void assignIntersectionPoint(Vector3D p)
    {
        intersection_point.assignVector(p.x, p.y, p.z);
    }
    void assignNormal(Vector3D n)
    {
        normal.assignVector(n.x, n.y, n.z);
        normal = vectorNormalize(normal);
    }
    void assignCoEfficients(double ambient, double diffuse, double specular, double reflective)
    {
        co_efficients[0] = ambient;
        co_efficients[1] = diffuse;
        co_efficients[2] = specular;
        co_efficients[3] = reflective;
    }
    void assignCoEfficients(double c[4])
    {
        for(int i=0; i<4; i++)
        {
            co_efficients[i] = c[i];
        }
    }
    void assignExponent(double exp)
    {
        specular_exp = exp;
    }
};



IntersectionObject find_intersection_color_for_each_pixel(Ray &ray, int row_val, int col_val
        ,double near_distance, double far_distance) //ray.initial_position contains the initial position
{
    IntersectionObject intersectingObj; //return this object
    ray.normalise(); //normalize just in case !! [LoL]
    //FOR EACH OBJECT ... first start with triangle's surface
    double min_t = MAX_VAL; //MAX VALUE
    double t = MAX_VAL;
    Vector3D colors_so_far;
    colors_so_far.assignVector(0, 0, 0);

    bool is_triangle = false, is_sphere = false;


    for(int i=0; i<pyramids_list.size(); i++) //Triangles...
    {
        for(int j=0; j<NUM_TRIANGLES_IN_PYRAMID; j++)
        {
            //FOR EACH TRIANGLE ...
            Triangle triangle = pyramids_list[i].triangles[j];
            double temp = triangle.find_intersecting_value_t(ray, near_distance, far_distance);
            if(temp != NULL_VALUE_T)
            {
                t = temp;
//                cout << "TRIANGLE t returned for i = " << row_val << " , j = " << col_val << " is " << t << endl;
                if(t < min_t)
                {
//                    cout << "--->>>TRIANGLE MATCH for i = " << i << " , j = " << j << " , col : " << triangle.colors.x << " " << triangle.colors.y << " " << triangle.colors.z << endl;
                    is_triangle = true;
                    min_t = t; //min_t stores t.
                    colors_so_far.assignVector(triangle.colors.x, triangle.colors.y, triangle.colors.z); ////assign the color of this triangle
                    intersectingObj.assignCoEfficients(pyramids_list[i].co_efficients);
                    intersectingObj.type = TRIANGLE_TYPE;
                    intersectingObj.specular_exp = pyramids_list[i].specular_exponent;
                    //assign normal here...
                    Vector3D _normal_vector, _P, _A, _B, _AP, _BP;
                    _A.assignVector(triangle.point1.x, triangle.point1.y, triangle.point1.z);
                    _B.assignVector(triangle.point2.x, triangle.point2.y, triangle.point2.z);
                    ray.normalise();
                    _P = vectorAddition(ray.initial_position, vectorScale(ray.direction_vector, min_t));
                    _AP = vectorSubtraction(_P, _A);
                    _BP = vectorSubtraction(_P, _B);
                    _normal_vector = vectorCrossProduct(_AP, _BP);
                    _normal_vector.normalise();
                    intersectingObj.normal.assignVector(_normal_vector);
                }
            }
        }
    }

    for(int i=0; i<spheres_list.size(); i++) //Spheres ...
    {
        Sphere sphere = spheres_list[i];
        t = sphere.find_intersecting_value_t(ray, near_distance, far_distance);
//        cout << "SPHERE t returned for i = " << row_val << " , j = " << col_val << " is " << t << endl;
        if(t != NULL_VALUE_T)
        {
            //if not null then compare if min or not
            if(t < min_t)
            {
                is_triangle = false;
                is_sphere = true;
//                cout << "++--++-->> SPHERE MATCH for i = " << row_val << ", j = " << col_val << " , col: " << sphere.colors[0] << " " << sphere.colors[1] << " " << sphere.colors[2] << endl;
                min_t = t;
                colors_so_far.assignVector(sphere.colors[0], sphere.colors[1], sphere.colors[2]); //assign the color of this sphere
                intersectingObj.assignCoEfficients(sphere.co_efficients);
                intersectingObj.type = SPHERE_TYPE;
                Vector3D _normal;
                ray.normalise();
                Vector3D _P = vectorAddition(ray.initial_position, vectorScale(ray.direction_vector, min_t));
                //_normal.assignVector(vectorAddition(sphere.center, vectorScale(_P , -1))); //center - intersection = normal
                _normal = vectorSubtraction(_P, sphere.center);
                _normal.normalise();
                intersectingObj.assignNormal(_normal);
                intersectingObj.specular_exp = sphere.specular_exponent;
            }
        }
    }

    //Checkerboard...
    //O_z + t.direction_z = 0 --> find value of t
    bool is_checker_board = false; //Checkerboard...
    t = -(ray.initial_position.z) / (ray.direction_vector.z);
    if((t < min_t) && (t >= 0))
    {
        is_sphere = false;
        min_t = t;
        is_checker_board = true;
    }

    //Find the intersecting point's co-ordinates

    Vector3D intersection_point;
    if(min_t != MAX_VAL)
    {
        intersection_point = vectorAddition(ray.initial_position, vectorScale(ray.direction_vector, min_t));
    }
    else
    {
        intersection_point.assignVector(farDistance, farDistance, farDistance);
    }
    if(is_checker_board == true)
    {
        intersectingObj.type = FLOOR_TYPE;
        intersectingObj.assignNormal(Vector3D(0, 0, 1)); //(0, 0, 1) is the normal i.e. z-axis
        //find which color's tile is this vector in.
        colors_so_far = color_of_pixel_checker_board(intersection_point);
        intersectingObj.assignCoEfficients(FLOOR_COEFFICIENTS); //assign floor's co-efficients ...
        intersectingObj.specular_exp = FLOOR_SPECULAR_EXPONENT;
    }
    intersectingObj.assignIntersectionPoint(intersection_point);
    intersectingObj.assignColor(colors_so_far);


    return intersectingObj;
}

//===================================== TO DO =============================================

bool does_object_exist_in_between(Vector3D source, Vector3D intersecting_point_initial)
{
    //double init_dist=get_dist(source,intersect);
    Vector3D intersecting_point_temporary, approx_point, dir_vect ;
    intersecting_point_temporary.assignVector(intersecting_point_initial);
    dir_vect = vectorSubtraction(source, intersecting_point_initial);
    dir_vect.normalise();
    intersecting_point_temporary = vectorAddition(intersecting_point_temporary, vectorScale(dir_vect, 1.0));  //just change a lil bit

    double initial_distance_of_point = vectorGetDistanceBetweenTwo(intersecting_point_temporary,source);

    for(int i=0; i<pyramids_list.size(); i++)
    {
        for(int j=0; j<NUM_TRIANGLES_IN_PYRAMID; j++)
        {
            Triangle triangle = pyramids_list[i].triangles[j];
            Ray ray;
            ray.assignRay(intersecting_point_temporary, dir_vect);
            double t = triangle.find_intersecting_value_t(ray, 0, farDistance); //for now ... farDistance is kept
            if(t != NULL_VALUE_T)
            {
                approx_point = vectorAddition(intersecting_point_temporary, vectorScale(dir_vect, t));
                double distance_now_of_point = vectorGetDistanceBetweenTwo(intersecting_point_temporary,approx_point);
                if(distance_now_of_point < initial_distance_of_point)
                {
                    return true;
                }
            }
        }
    }

    //sphere check
    for(int i=0; i<spheres_list.size(); i++)
    {
        Sphere sphere = spheres_list[i];
        Ray ray;
        ray.assignRay(intersecting_point_temporary, dir_vect);
        double t = sphere.find_intersecting_value_t(ray, 0, farDistance); //for now ... farDistance is kept
        if(t != NULL_VALUE_T)
        {
            approx_point = vectorAddition(intersecting_point_temporary, vectorScale(dir_vect, t));
            double distance_now_of_point = vectorGetDistanceBetweenTwo(intersecting_point_temporary,approx_point);
            if(distance_now_of_point < initial_distance_of_point)
            {
                return true;
            }
        }
    }

    return false;
}


///Recursive call
Vector3D getColorOfPixel(Ray ray, int depth)
{
    if(depth <= 0){
        return Vector3D(0.0, 0.0, 0.0);
    }
    ///Function call
    IntersectionObject intersectionObj = find_intersection_color_for_each_pixel(ray, -1, -1, nearDistance, farDistance); // -1, -1 is row, col ... not needed
    //Function call done
    Vector3D color_this_pixel = intersectionObj.color;
    Vector3D intersecting_point = intersectionObj.intersection_point;
    Vector3D normal_vector = intersectionObj.normal;
    normal_vector.normalise();
    //add the ambient effects
    double ambient_coefficient = intersectionObj.co_efficients[AMBIENT_INDEX];
    double diffuse_coefficient = intersectionObj.co_efficients[DIFFUSE_INDEX];
    double specular_coefficient = intersectionObj.co_efficients[SPECULAR_INDEX];
    double reflection_coefficient = intersectionObj.co_efficients[REFLECTION_INDEX];
    double specular_exponent =  intersectionObj.specular_exp;

    //add diffuse effect FOR EACH LIGHT SOURCE ...
    Vector3D lightSourcePosition, _incident_ray, _reflected_ray, _ray_from_eye_to_intersection;
    double cos_theta_sum = 0.0, cos_phi_sum = 0.0;

    Vector3D null_intersection_point(farDistance, farDistance, farDistance);
    bool is_dark_intersection = isEqualVector(null_intersection_point, intersecting_point);
    if(is_dark_intersection == true)
    { //no intersecting points available ... so return black color ... no need for further computation
        return Vector3D(0.0, 0.0, 0.0);
    }
    else
    {
        //----- FOR EACH LIGHT SOURCE -----

        for(int light_src_cnt = 0; light_src_cnt < light_sources.size(); light_src_cnt++)
        {
            lightSourcePosition = light_sources[light_src_cnt];
//                _incident_ray = vectorSubtraction(intersecting_point, lightSourcePosition);
            // ---------------------------------------------------------------------------------
            //Cast ray using this lightSourcePosition as origin
            Ray ray2;
            ray2.initial_position.assignVector(lightSourcePosition);
            Vector3D dir_vect_for_ray = vectorSubtraction(intersecting_point, lightSourcePosition);
            dir_vect_for_ray.normalise();
            ray2.direction_vector.assignVector(dir_vect_for_ray);
            ray2.normalise();

            double near_dist = 0 + EPSILON_COMPARISON; ///0.00001
            double far_dist = vectorGetDistanceBetweenTwo(intersecting_point, lightSourcePosition) - EPSILON_COMPARISON;

            bool does_obj_exist_bet = does_object_exist_in_between(lightSourcePosition, intersecting_point);
            if(does_obj_exist_bet == false)  // no objects
            {
                //No object in between
                _incident_ray = vectorSubtraction(lightSourcePosition, intersecting_point);
                _incident_ray.normalise();

                normal_vector = intersectionObj.normal;
                normal_vector.normalise();
                //refl = 2*(L.N)N - L
                _reflected_ray = vectorSubtraction(_incident_ray,
                                                   vectorScale(normal_vector, (2 * vectorDotProduct(_incident_ray, normal_vector))));
                //                _reflected_ray = vectorSubtraction(vectorScale(normal_vector, (2 * vectorDotProduct(_incident_ray, normal_vector))),
                //                                       _incident_ray); //Sir method
                _reflected_ray.normalise();

                //                _ray_from_eye_to_intersection = vectorSubtraction(pos, intersecting_point); //intersection point - pos
                _ray_from_eye_to_intersection = vectorSubtraction(intersecting_point, pos); //intersection point - pos
                _ray_from_eye_to_intersection.normalise();


                cos_theta_sum += max((vectorDotProduct(_incident_ray, normal_vector) / _incident_ray.magnitude()), 0.0);

//                        double temp = max( ( (vectorDotProduct(_reflected_ray, _ray_from_eye_to_intersection))), 0.0); // cos phi
                double temp = max(vectorDotProduct(_reflected_ray, _ray_from_eye_to_intersection), 0.0); // cos phi
                double pow_temp = pow(temp, specular_exponent); //cos phi to the power specular_exponent

                cos_phi_sum += pow_temp;
            }
        }
        // FOR EACH LIGHT SOURCE COMPUTED VALUES
    }

    Vector3D _ambient_rgb_component = vectorScale(color_this_pixel, ambient_coefficient); //ambient_coeff * [r, g, b]

    double diffuse_multiplier = diffuse_coefficient * cos_theta_sum;
    Vector3D _diffuse_rgb_component = vectorScale(color_this_pixel, diffuse_multiplier);

    double specular_multiplier = specular_coefficient * cos_phi_sum;
    Vector3D _specular_rgb_component = vectorScale(Vector3D(1.0, 1.0, 1.0), specular_multiplier);

    //Only ambient [initialize]
    color_this_pixel.assignVector(_ambient_rgb_component);  //vectorAddition(Vector3D(0.0, 0.0, 0.0), _ambient_rgb_component);

    //Add diffuse component too
    color_this_pixel = vectorAddition(color_this_pixel, _diffuse_rgb_component);

    //Add specular component too
    color_this_pixel = vectorAddition(color_this_pixel, _specular_rgb_component);
//            cout << "Color print : "; color_this_pixel.printVector();

    ///--------------- TO DO ----------------
    /*
        Construct new ray here ...
        1. Reflected Ray
        2. Call using this function, depth - 1
    */
    //-------------- FOR REFLECTION ------------
    if(is_dark_intersection == false){
        Vector3D intersc_point = intersectionObj.intersection_point;
        Vector3D _incident_ray = ray.direction_vector;
        Vector3D normal_vector = intersectionObj.normal;
        normal_vector.normalise();
        _incident_ray.normalise();
        //Compute reflected ray
        Vector3D _reflected_ray = vectorSubtraction(_incident_ray,
                                                   vectorScale(normal_vector, (2 * vectorDotProduct(_incident_ray, normal_vector))));
        _reflected_ray.normalise();
        Ray newRay;
        Vector3D new_intersec_point = vectorAddition(intersc_point, vectorScale(_reflected_ray, 1.0));
        newRay.assignRay(new_intersec_point, _reflected_ray);
        ///RECURSIVE CALL INITIATION
        Vector3D color_ret = getColorOfPixel(newRay, depth - 1);
        ///RECURIVE CALL ANSWER OBTAINED
        color_ret = vectorScale(color_ret, reflection_coefficient); //Multiply with reflection coefficient
        color_this_pixel = vectorAddition(color_this_pixel, color_ret); //Ultimately add with previous colors
    }

    color_this_pixel.x = min(color_this_pixel.x, 1.0);
    color_this_pixel.y = min(color_this_pixel.y, 1.0);
    color_this_pixel.z = min(color_this_pixel.z, 1.0);

    return color_this_pixel;
}

///------------------------------------ Find intersection points DOUBLE array ------------------------------------

void find_intersection_points()
{
//FOR EACH PIXEL
    cout << "--->>Trying to render image... Please wait ... " << endl ;
    //debugging with only one pixel
    Vector3D initial_pos, pixel_pos;
    initial_pos.assignVector(pos.x, pos.y, pos.z);

    for(int i=0; i<PIXEL_NUM; i++)
    {
        for(int j=0; j<PIXEL_NUM; j++)
        {
            row_idx_deb = i;
            col_idx_deb = j;
            pixel_pos = pixel_window_mid_points[i][j];

            Vector3D direction_vect = vectorSubtraction(pixel_pos, initial_pos);
            direction_vect.normalise();

            Ray ray;
            ray.assignRay(initial_pos, direction_vect); //initialize the ray.

            Vector3D color_this_pixel = getColorOfPixel(ray, recursion_level); ///Using recursion_level [Global variable] call ...

            //image_bitmap_pixel.set_pixel(767-i, j, 255 * color_this_pixel.x, 255 * color_this_pixel.y, 255 * color_this_pixel.z);
            image_bitmap_pixel.set_pixel(j,PIXEL_NUM - 1 - i, color_this_pixel.x * 255, color_this_pixel.y * 255,
                                         color_this_pixel.z * 255);
        }

    }
    cout << "\n---->>>SUCCESSFULLY rendered image.. in out.bmp file\n" << endl;
    image_bitmap_pixel.save_image("out.bmp");
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
        cout << "Now pos = ";
        pos.printVector();
        break;
    case GLUT_KEY_DOWN:		// up arrow key
        pos = vectorSubtraction(pos, vectorScale(l, scalar_forwardBackward));
        cout << "Now pos = ";
        pos.printVector();
        break;
    case GLUT_KEY_RIGHT:    ///Move with respect to RIGHT
        pos = vectorAddition(pos, vectorScale(r, scalar_rightLeft));
        cout << "Now pos = ";
        pos.printVector();
        break;
    case GLUT_KEY_LEFT:
        pos = vectorSubtraction(pos, vectorScale(r, scalar_rightLeft));
        cout << "Now pos = ";
        pos.printVector();
        break;
    case GLUT_KEY_PAGE_UP:  ///Move with respect to UP
        pos = vectorAddition(pos, vectorScale(u, scalar_upDown));
        cout << "Now pos = ";
        pos.printVector();
        break;
    case GLUT_KEY_PAGE_DOWN:
        pos = vectorSubtraction(pos, vectorScale(u, scalar_upDown));
        cout << "Now pos = ";
        pos.printVector();
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
    drawaxes = 0; ///do not draw axes
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
    loadAllData(); //load all data from input file
    printAllShapes(); //print all the shapes accordingly
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
