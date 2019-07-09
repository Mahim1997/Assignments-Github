#include <iostream>
#include <fstream>
#include <string>
#include<algorithm> // ?
#include<cstdio>
#include<cmath>
#include<cstdlib>
#include <stack> //Stack inspiration from https://www.geeksforgeeks.org/stack-push-and-pop-in-c-stl/
#define SIZE 4
#define INPUT_FILE_NAME "scene.txt"

#define pi (2*acos(0.0))
#define DEGREE_TO_RAD(x) ((x * pi) / 180)
#define RAD_TO_DEGREE(x) (x * 180 / pi)

#define DEBUG 1
#if DEBUG == 1
    #define DEBUG_TRANSFORMATION 0
    #define DEBUG_TRIANGLE 1
#endif // DEBUG


using namespace std ;

class Vector ///This is a 4X4 vector used throughout
{
public:
    double x, y, z, w;
    Vector(){x = 0; y = 0; z = 0; w = 0;}
    Vector(double a, double b, double c)
    {
        this->x = a; this->y = b; this->z = c; this->w = 1;
    }
    Vector(double a, double b, double c, double d)
    {
        this->x = a; this->y = b; this->z = c; this->w = d;
    }
    void assignVector(double a, double b, double c, double d = 1){
        this->x = a; this->y = b; this->z = c; this->w = d;
    }

    void printVector(bool flag = false){
        if(flag == true){
            cout << x << " " << y << " " << z << endl ;
        }
        else{
            cout << x << " " << y << " " << z << " " << w << endl;
        }
    }
    double magnitude(){
        double ans = (x*x) + (y*y) + (z*z);
        ans = sqrt(ans);
        return ans;
    }
};

//--------------------------------------------------------- Vector functions begin --------------------------------------------------------------
Vector vectorScale(Vector a, double scale)
{
    Vector ans;
    ans.x = a.x * scale;
    ans.y = a.y * scale;
    ans.z = a.z * scale;
    ans.w = a.w * scale;
    return ans;
}
Vector vectorNormalize(Vector a)
{
    double factor = a.magnitude();
    Vector v = vectorScale(a, factor);
    return v;
}
Vector vectorMakeHomogenous(Vector a)
{
    double scale = 1;
    if(a.w != 1){
        scale = (double)(1.0 / a.w);
    }
    Vector v = vectorScale(a, scale);
    return v;
}
Vector vectorCrossProduct(Vector a, Vector b)
{
    Vector ans;
    ans.x = (a.y * b.z) - (b.y * a.z);
    ans.y = -((a.x * b.z) - (b.x * a.z));
    ans.z = (a.x * b.y) - (b.x * a.y);
    return ans;
};
Vector vectorAddition(Vector a, Vector b)
{
    Vector ans;
    ans.x = a.x + b.x;
    ans.y = a.y + b.y;
    ans.z = a.z + b.z;
    return ans;
};
Vector vectorSubtraction(Vector a, Vector b)
{
    Vector ans;
    ans.x = a.x - b.x;
    ans.y = a.y - b.y;
    ans.z = a.z - b.z;
    return ans;
};
double vectorDotProduct(Vector a, Vector b)
{
    return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
};

//--------------------------------------------------------- Vector functions end --------------------------------------------------------------

class Matrix ///This is 4X4 matrix used throughout
{
public:
    double elements[SIZE][SIZE]; //Elements of the matrix
    Matrix(){
        for(int i=0; i<SIZE; i++){
            for(int j=0; j<SIZE; j++){
                elements[i][j] = 0.0;
            }
        }
    }
    Matrix getIdentityMatrix()
    {
        Matrix m;
        for(int i=0; i<SIZE; i++){
            m.elements[i][i] = 1.0;
        }
        return m;
    }
    Matrix(double el[SIZE][SIZE])
    {
        for(int i=0; i<SIZE; i++){
            for(int j=0; j<SIZE; j++){
                elements[i][j] = el[i][j];
            }
        }
    }
    void printMatrix(bool flag = true)
    {
        int SIZE_USED = SIZE;
        if(flag == false){
            SIZE_USED = SIZE - 1;
        }
        for(int i=0; i<SIZE_USED; i++){
            for(int j=0; j<SIZE_USED; j++){
                cout << elements[i][j] << " ";
            }
            cout << endl ;
        }
        cout << endl ;
    }
    void assignMatrix(double el[SIZE][SIZE])
    {
        for(int i=0; i<SIZE; i++){
            for(int j=0; j<SIZE; j++){
                elements[i][j] = el[i][j];
            }
        }
    }
    void scale(double factor)
    {
        for(int i=0; i<SIZE; i++){
            for(int j=0; j<SIZE; j++){
                elements[i][j] *= factor;
            }
        }
    }
    double getValue_W()
    {
        return elements[SIZE - 1][SIZE - 1]; //The last element i.e. elements[3][3]
    }
    void makeHomogenousAgain()
    {
        double factor = getValue_W();
        factor = (double)1.0 / factor; //eg. if w = 5, then factor = 1/5 = 0.2
        scale(factor); //Scale back to make w = 1 again
    }
    void formRow(Vector v, int index_row)
    {
        this->elements[index_row][0] = v.x;
        this->elements[index_row][1] = v.y;
        this->elements[index_row][2] = v.z;
        this->elements[index_row][3] = v.w;
    }
    void formColumn(Vector v, int index_col)
    {
        this->elements[0][index_col] = v.x;
        this->elements[1][index_col] = v.y;
        this->elements[2][index_col] = v.z;
        this->elements[3][index_col] = v.w;
    }
    Vector getColumnVector(int index_col)
    {
        Vector v;
        v.x = elements[0][index_col];
        v.y = elements[1][index_col];
        v.z = elements[2][index_col];
        v.w = elements[3][index_col];
        return v;
    }
    Vector getRowVector(int index_row)
    {
        Vector v;
        v.x = elements[index_row][0];
        v.y = elements[index_row][1];
        v.z = elements[index_row][2];
        v.w = elements[index_row][3];
        return v;
    }

};

Matrix MatrixFormationFromVector_WrtCol(Vector v1, Vector v2, Vector v3)
{
    Matrix m;
    m.formColumn(v1, 0);
    m.formColumn(v2, 1);
    m.formColumn(v3, 2);
    Vector v4(0, 0, 0, 1);
    m.formColumn(v4, 3);
    for(int i=0; i<(SIZE - 1); i++){
        m.elements[SIZE - 1][i] = 0.0;
    }
    return m;
}
Matrix MatrixFormationFromVector_WrtCol(Vector v1, Vector v2, Vector v3, Vector v4)
{
    Matrix m;
    m.formColumn(v1, 0);
    m.formColumn(v2, 1);
    m.formColumn(v3, 2);
    m.formColumn(v4, 3);
    return m;
}

Matrix MatrixFormationFromVector_WrtRow(Vector v1, Vector v2, Vector v3, Vector v4)
{
    Matrix m;
    m.formRow(v1, 0);
    m.formRow(v2, 1);
    m.formRow(v3, 2);
    m.formRow(v4, 3);
    return m;
}

Matrix MatrixFormationFromVector_WrtRow(Vector v1, Vector v2, Vector v3)
{
    Matrix m;
    m.formRow(v1, 0);
    m.formRow(v2, 1);
    m.formRow(v3, 2);
    Vector v4(0, 0, 0, 1);
    m.formColumn(v4, 3);
    return m;
}

Matrix MatrixProduct(Matrix a, Matrix b)
{
    //Matrix multiplication code, inspiration from https://www.programiz.com/cpp-programming/examples/matrix-multiplication
    Matrix mult;
    int r1 = SIZE, r2 = SIZE, c1 = SIZE, c2 = SIZE ;
    for(int i=0; i < r1; i++)
    {
        for(int j=0; j<c2; j++){
            for(int k=0; k<c1; k++){
                mult.elements[i][j] += a.elements[i][k] * b.elements[k][j];
            }
        }
    }
    return mult;
}
///---------------------------------------------------------------- GLOBAL VARIABLES ---------------------------------------------------------------
ifstream fileReaderStream;  //ifstream used to read the file
Vector eye, look, up;
double fovY, aspectRatio, nearDistance, farDistance;
stack<Matrix> stack_transformations;
stack<int> stack_how_many_transformations_to_remove;


ofstream ouptutStage1File;
ofstream ouptutStage2File;
ofstream ouptutStage3File;

void pushToStackProduct(Matrix toPush)
{
    Matrix top = stack_transformations.top();
    Matrix prod = MatrixProduct(top, toPush);
//    prod.makeHomogenousAgain();
    stack_transformations.push(prod);
}
void pushToStackInitial(Matrix toPush)
{
    stack_transformations.push(toPush);
}

void printStack()
{
    stack<Matrix> dummy = stack_transformations;
    cout << "-----------------------------" << endl;
    while(dummy.empty() == false)
    {
        Matrix top = dummy.top();
        dummy.pop();
        top.printMatrix();
    }
    cout << "-----------------------------" << endl;
}
Matrix popFromStack()
{
    Matrix m = stack_transformations.top();
    stack_transformations.pop();
    return m;
}
///---------------------------------------------------------------- GLOBAL VARIABLES ---------------------------------------------------------------

void printEyeLookUpParams()
{
    printf("EYE: "); eye.printVector();
    printf("LOOK: "); look.printVector();
    printf("UP: "); up.printVector();
    printf("FOV_Y = %lf, Aspect Ratio = %lf, Near distance = %lf, Far distance = %lf\n", fovY, aspectRatio, nearDistance, farDistance);
}
void extractFirst4Lines()
{
    double a, b, c, d;
    fileReaderStream >> a >> b >> c;
//    printf("Inside extractFirst4Lines() ... a = %lf, b = %lf, c = %lf, d = %lf\n", a, b, c, d);
    eye.assignVector(a, b, c);
    fileReaderStream >> a >> b >> c;
    look.assignVector(a, b, c);
    fileReaderStream >> a >> b >> c;
    up.assignVector(a, b, c);

    fileReaderStream >> fovY >> aspectRatio >> nearDistance >> farDistance;
}

//----------------------------------------------------- Transformation functions begin -------------------------------------------

Matrix Transformation_Translation(double tx, double ty, double tz)
{
    Vector translationVector(tx, ty, tz, 1);
    Matrix m;
    m.formColumn(Vector(1, 0, 0, 0), 0);
    m.formColumn(Vector(0, 1, 0, 0), 1);
    m.formColumn(Vector(0, 0, 1, 0), 2);
    m.formColumn(translationVector, 3);
    return m;
}

Matrix Transformation_Scale(double sx, double sy, double sz)
{
    Matrix m;
    m.formColumn(Vector(sx, 0, 0, 0), 0);
    m.formColumn(Vector(0, sy, 0, 0), 1);
    m.formColumn(Vector(0, 0, sz, 0), 2);
    m.formColumn(Vector(0, 0, 0, 1), 3);
    return m;
}

Vector Rodrigues_Formula(Vector x, Vector a, double theta)
{
    theta = DEGREE_TO_RAD(theta);
    Vector v1 = vectorScale(x, cos(theta));
//    Vector v2 = vectorScale(  a, vectorScale( vectorDotProduct(a, x) , (1 - cos(theta) ) )  );
    Vector v2 = vectorScale(a, (1 - cos(theta)) * (vectorDotProduct(a, x)));
    Vector v3 = vectorScale(vectorCrossProduct(a, x), sin(theta));
    Vector rodrigues_ans = vectorAddition(v1, vectorAddition(v2, v3));
    return rodrigues_ans;
}

Matrix Transformation_Rotate(double angle, double ax, double ay, double az)
{
    Vector a(ax, ay, az, 1); //Shouldn't it be 0 ?
    a = vectorNormalize(a); //Normalize the vector

    Vector c1, c2, c3;
    Vector i(1, 0, 0, 0), j(0, 1, 0, 0), k(0, 0, 1, 0);
    c1 = Rodrigues_Formula(i, a, angle);
    c2 = Rodrigues_Formula(j, a, angle);
    c3 = Rodrigues_Formula(k, a, angle);


    printf("--------------------------------------------------------------------------------\n");
    printf("Inside Transformation_Rotate .... angle = %lf, ax = %lf, ay = %lf, az = %lf\n", angle, ax, ay, az);
    printf("Now printing c1, c2, c3\n");
    c1.printVector();
    c2.printVector();
    c3.printVector();
    Matrix rotate_matrix = MatrixFormationFromVector_WrtCol(Vector(c1.x, c1.y, c1.z, 0),
                                                            Vector(c2.x, c2.y, c2.z, 0),
                                                            Vector(c3.x, c3.y, c3.z, 0),
                                                            Vector(0, 0, 0, 1));

    printf("RETURNING THE MATRIX OF ROTATION \n");
    rotate_matrix.printMatrix();

    printf("--------------------------------------------------------------------------------\n");

    return rotate_matrix;
}


//----------------------------------------------------- Transformation functions end -------------------------------------------

void outputTriangleToFile(Vector v1, Vector v2, Vector v3)
{
    //Apply Transformation
    Matrix topStack = stack_transformations.top();


    //Output Triangle to output stream
//    cout << "Inside outputTriangleToFile ... printing vectors v1, v2, v3" << endl ;
//    v1.printVector(true);
//    v2.printVector(true);
//    v3.printVector(true);

    Matrix m;
    m = MatrixFormationFromVector_WrtCol(v1, v2, v3);
//    cout << "Printing the triangle matrix \n";
//    m.printMatrix(true);

    Matrix triangle;
    triangle = MatrixProduct(topStack, m);
    triangle.makeHomogenousAgain();

    Vector col1, col2, col3;
    col1 = triangle.getColumnVector(0);
    col2 = triangle.getColumnVector(1);
    col3 = triangle.getColumnVector(2);

#if DEBUG_TRIANGLE == 1
    cout << "Triangle" << endl;
    col1.printVector(true);
    col2.printVector(true);
    col3.printVector(true);
    cout << endl;
#endif // DEBUG_TRIANGLE

}

int extractCommand()
{
    string inputString;     //To store the input lines in this string
    getline(fileReaderStream, inputString); // Saves the line in inputString [input command]

    int toReturn = 1;

    //if-else conditions
    if(inputString == "end"){ //If 'end' command is encountered, main while(true) loop should break
        toReturn = -1;
    }
    else if(inputString == "triangle"){
        double x1, x2, x3, y1, y2, y3, z1, z2, z3;
        fileReaderStream >> x1 >> y1 >> z1;
        fileReaderStream >> x2 >> y2 >> z2;
        fileReaderStream >> x3 >> y3 >> z3;

        //cout << "TRIANGLE\n";
        //printf("%lf, %lf, %lf\n%lf, %lf, %lf\n%lf, %lf, %lf\n\n", x1, y1, z1, x2, y2, z2, x3, y3, z3);

        Vector v1(x1, y1, z1);
        Vector v2(x2, y2, z2);
        Vector v3(x3, y3, z3);

        outputTriangleToFile(v1, v2, v3);

    }
    else if(inputString == "translate"){
        double x, y, z;
        fileReaderStream >> x >> y >> z;
        //TO DO
        int top = stack_how_many_transformations_to_remove.top();
        top++;
        stack_how_many_transformations_to_remove.pop();
        stack_how_many_transformations_to_remove.push(top);

        Matrix translation_matrix = Transformation_Translation(x, y, z);

#if DEBUG_TRANSFORMATION == 1
        cout << "Translation matrix is : " << endl ;
        translation_matrix.printMatrix();
#endif // DEBUG_TRANSFORMATION

        //Push to matrix
        pushToStackProduct(translation_matrix);
    }
    else if(inputString == "scale"){
        double x, y, z;
        fileReaderStream >> x >> y >> z;
        //TO DO
        int top = stack_how_many_transformations_to_remove.top();
        top++;
        stack_how_many_transformations_to_remove.pop();
        stack_how_many_transformations_to_remove.push(top);

        Matrix scale_matrix = Transformation_Scale(x, y, z);

#if DEBUG_TRANSFORMATION == 1
        cout << "Scale matrix is : " << endl ;
        scale_matrix.printMatrix();
#endif // DEBUG_TRANSFORMATION

        //Push to matrix
        pushToStackProduct(scale_matrix);
    }
    else if(inputString == "rotate"){
        double angle, x, y, z;
        fileReaderStream >> angle >> x >> y >> z;
        //TO DO
        int top = stack_how_many_transformations_to_remove.top();
        top++;
        stack_how_many_transformations_to_remove.pop();
        stack_how_many_transformations_to_remove.push(top);

        Matrix rotate_matrix = Transformation_Rotate(angle, x, y, z);
#if DEBUG_TRANSFORMATION == 1
        cout << "Rotation matrix is : " << endl ;
        rotate_matrix.printMatrix();
#endif // DEBUG_TRANSFORMATION
        //Push to matrix
        pushToStackProduct(rotate_matrix);

    }
    else if(inputString == "push"){
        //TO DO
#if DEBUG_TRANSFORMATION == 1
        cout << "Pushing to count_stack 0\n";
#endif // DEBUG_TRANSFORMATION
        stack_how_many_transformations_to_remove.push(0); //Push how many indices to remove
    }
    else if(inputString == "pop"){
        //TO DO
        int how_many_to_remove = stack_how_many_transformations_to_remove.top(); //Pop how many to remove
        stack_how_many_transformations_to_remove.pop();
#if DEBUG_TRANSFORMATION == 1
        cout << "To remove [pop] from transformations_stack is " << how_many_to_remove << endl ;
#endif // DEBUG_TRANSFORMATION
        for(int i=0; i<how_many_to_remove; i++){
            stack_transformations.pop(); ///Pop THESE many items from stack_transformations
        }
    }


    return toReturn;
}

//Forward declarations
void testStack();

int main()
{
    //I/O inspiration from http://www.cplusplus.com/forum/beginner/8388/
    fileReaderStream.open(INPUT_FILE_NAME);
    ouptutStage1File.open("stage1.txt");
    ouptutStage2File.open("stage2.txt");
    ouptutStage3File.open("stage3.txt");

    //Push identity matrix
    Matrix m1;
    pushToStackInitial(m1.getIdentityMatrix());

    //while(true) //Wait until the 'END' command is found
    extractFirst4Lines();
    //printEyeLookUpParams(); //Done
    while(true)
    {
        int ret = extractCommand();
        if(ret == -1){
            break;
        }
    }

//    testStack(); //Works

    fileReaderStream.close(); //close the ifstream
}

void testStack()
{
    Matrix m1;
    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            m1.elements[i][j] = (int)(i + 1)*(j + 1);
        }
    }
    Matrix m2;
    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            m2.elements[i][j] = (int)(i + 4)*(j + 2);
        }
    }
    Matrix m3;
    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            m3.elements[i][j] = (int)(i - 1)*(j - 2);
        }
    }
    pushToStackInitial(m1.getIdentityMatrix());
    printStack();
    pushToStackProduct(m1);
    printStack();
    pushToStackProduct(m2);
    printStack();
    pushToStackProduct(m3);
    printStack();

    Matrix temp;
    temp = popFromStack();
    temp.printMatrix();
    printStack();
}

/*
    while(fileReaderStream.eof() != true)
    {
        extractFirst4Lines();
    }
*/

/*
        getline(fileReaderStream, inputString); // Saves the line in inputString
        cout << inputString;  // Prints our STRING.
        cout << endl ;
*/





