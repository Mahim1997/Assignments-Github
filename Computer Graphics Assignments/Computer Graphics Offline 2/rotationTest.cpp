#include <iostream>
#include <fstream>
#include <string>
#include<algorithm> // ?
#include<cstdio>
#include<cmath>
#include<cstdlib>
#include <stack> //Stack inspiration from https://www.geeksforgeeks.org/stack-push-and-pop-in-c-stl/

#define SIZE 4

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

int main()
{

    double a1 = cos(90.0);
    printf("a1 = cos(90.0) = %lf\n", a1);
}
