#include <iostream>
#include <fstream>
#include <string>
#include<algorithm> // ?
#include<cstdio>
#include<cstdlib>
#include <stack> //Stack inspiration from https://www.geeksforgeeks.org/stack-push-and-pop-in-c-stl/
#define SIZE 4
#define INPUT_FILE_NAME "scene.txt"

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

    void printVector(){
        cout << x << " " << y << " " << z << " " << w << endl;
    }
};

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
    void printMatrix()
    {
        for(int i=0; i<SIZE; i++){
            for(int j=0; j<SIZE; j++){
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

};

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

void pushToStackProduct(Matrix toPush)
{
    Matrix top = stack_transformations.top();
    Matrix prod = MatrixProduct(top, toPush);
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

    }
    else if(inputString == "translate"){

    }
    else if(inputString == "scale"){

    }
    else if(inputString == "rotate"){

    }
    else if(inputString == "push"){

    }
    else if(inputString == "pop"){

    }


    return toReturn;
}

int main()
{
    //I/O inspiration from http://www.cplusplus.com/forum/beginner/8388/
    fileReaderStream.open(INPUT_FILE_NAME);
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

    //testStack(); //Works

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





