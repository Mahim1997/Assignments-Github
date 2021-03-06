#include <iostream>
#include <fstream>
#include <string>
#include<algorithm> // ?
#include<cstdio>
#include<cmath>
#include<cstdlib>
#include <stack>

#include<iomanip>
#include<limits>

#define SIZE 4
#define INPUT_FILE_NAME "scene.txt"

#define pi (2*acos(0.0))
#define DEGREE_TO_RAD(x) ((x * pi) / 180)
#define RAD_TO_DEGREE(x) (x * 180 / pi)

#define DEBUG 0
#if DEBUG == 1
    #define DEBUG_TRANSFORMATION 0
    #define DEBUG_TRIANGLE 0
    #define DEBUG_TRANSFORMATION_FUNCTION 0
    #define DEBUG_STACK 0
    #define DEBUG_STAGE_2 1
    #define DEBUG_STAGE_3 1
#endif // DEBUG


using namespace std ;

///Output streams to stage1.txt, stage2.txt, stage3.txt respectively
ofstream ouptutStage1File;
ofstream ouptutStage2File;
ofstream ouptutStage3File;


class Vector ///This is a 4X4 vector used throughout
{
public:
    double x, y, z, w;
    Vector(){x = 0; y = 0; z = 0; w = 1;}
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
        cout << std::fixed ;
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
    void outputToStage1()
    {
        ouptutStage1File << x << " " << y << " " << z ;
        ouptutStage1File << endl ;
    }
    void outputToStage2()
    {
        ouptutStage2File << x << " " << y << " " << z ;
        ouptutStage2File << endl ;
    }
    void outputToStage3()
    {
        ouptutStage3File << x << " " << y << " " << z ;
        ouptutStage3File << endl ;
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
    factor = (double)(1.0 / factor);
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


};

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


Matrix MatrixProduct(Matrix a, Matrix b)
{
    Matrix mult;
    int row1 = SIZE, row2 = SIZE, col1 = SIZE, col2 = SIZE ;
    for(int i=0; i < row1; i++)
    {
        for(int j=0; j<col2; j++){
            for(int k=0; k<col1; k++){
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
///Above stack works as follows:
///When "push": insert 0, when new transformation added, increment top, when "pop" -> stack_transformations is popped "top" number of times (and pop this stack also)


Matrix view_matrix; ///Used for stage2.txt purposes
Matrix projection_matrix; ///Used for stage3.txt purposes

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

Matrix Transformation_Translation(double tx, double ty, double tz)  //Works
{
    Vector translationVector(tx, ty, tz, 1);
    Matrix m;
    m.formColumn(Vector(1, 0, 0, 0), 0);
    m.formColumn(Vector(0, 1, 0, 0), 1);
    m.formColumn(Vector(0, 0, 1, 0), 2);
    m.formColumn(translationVector, 3);
    return m;
}

Matrix Transformation_Scale(double sx, double sy, double sz) //Works
{
    Matrix m;
    m.formColumn(Vector(sx, 0, 0, 0), 0);
    m.formColumn(Vector(0, sy, 0, 0), 1);
    m.formColumn(Vector(0, 0, sz, 0), 2);
    m.formColumn(Vector(0, 0, 0,  1), 3);
    return m;
}

Vector Rodrigues_Formula(Vector x, Vector a, double theta)  //Rodrigues Formula works
{

    theta = DEGREE_TO_RAD(theta);
    Vector v1 = vectorScale(x, cos(theta));
//    Vector v2 = vectorScale(  a, vectorScale( vectorDotProduct(a, x) , (1 - cos(theta) ) )  );
    Vector v2 = vectorScale(a, (1 - cos(theta)) * (vectorDotProduct(a, x)));
    Vector v3 = vectorScale(vectorCrossProduct(a, x), sin(theta));
    Vector rodrigues_ans = vectorAddition(v1, vectorAddition(v2, v3));

//    printf("==>>Inside Rodrigues_Formula ... theta = %lf, x = ", theta); x.printVector(); printf("   and  a = "); a.printVector();
//    printf("---->> RETURNING FROM RODRIGUEZ ... ans is "); rodrigues_ans.printVector();

    return rodrigues_ans;
}

Matrix Transformation_Rotate(double angle, double ax, double ay, double az) //OK
{
    Vector a(ax, ay, az, 0); //Start with w = 0 ... after normalizing ... make w = 1
    a = vectorNormalize(a); //Normalize the vector
    a.w = 1; //To make 'w' = 1 since it will be added later

    Vector c1, c2, c3;
    Vector i(1, 0, 0, 1), j(0, 1, 0, 1), k(0, 0, 1, 1);  // w = 1 is kept
///Obtain the columns where unit axes x, y, and z are going to be mapped to as c1, c2, c3 respectively.
    c1 = Rodrigues_Formula(i, a, angle);
    c2 = Rodrigues_Formula(j, a, angle);
    c3 = Rodrigues_Formula(k, a, angle);


    Matrix rotate_matrix = MatrixFormationFromVector_WrtCol(Vector(c1.x, c1.y, c1.z, 0), /*Matrix formation using c1 as col1 , c2 as col2 and so on*/
                                                            Vector(c2.x, c2.y, c2.z, 0),
                                                            Vector(c3.x, c3.y, c3.z, 0),
                                                            Vector(0, 0, 0, 1));  /// Last column is simply 0, 0, 0, 1
    return rotate_matrix;
}


//----------------------------------------------------- Transformation functions end -------------------------------------------

void printLine()
{
    printf("----------------------------------------------------------------\n");
}

///Vector product
Vector vectorProductWithMatrix(Matrix mat, Vector v)
{
    ///Multiply matrix with vector
    Vector answer_vector;

    double elements_vector[4];

    ///Convert to array for multiplication
    elements_vector[0] = v.x;
    elements_vector[1] = v.y;
    elements_vector[2] = v.z;
    elements_vector[3] = v.w;

    double answer_array[4];
    //multiply now
    for(int row=0; row<SIZE; row++)
    {
        double ans_temp = 0.0; //keep a temporary variable ...
        for(int col=0; col<SIZE; col++){
            ans_temp += (mat.elements[row][col] * elements_vector[col]);
        }
        answer_array[row] = ans_temp;
    }
    ///Convert back to vector to return the answer
    answer_vector.x = answer_array[0];
    answer_vector.y = answer_array[1];
    answer_vector.z = answer_array[2];
    answer_vector.w = answer_array[3];

    return answer_vector; ///return answer
}

void obtainProjectionMatrix()
{
//Calculate parameters [Convert angle to radian mode]
    double fovX = fovY * aspectRatio;           //fovX = fovY * aspectRatio
    double t = nearDistance * tan(DEGREE_TO_RAD(fovY * 0.5)); //t = near * tan(fovY / 2)
    double r = nearDistance * tan(DEGREE_TO_RAD(fovX * 0.5)); //r = near * tan(fovX / 2)

//    printf("fovX = %lf, fovY = %lf, aspectRatio = %lf, t = %lf, r = %lf, nearDistance = %lf, farDis = %lf\n",
//           fovX, fovY, aspectRatio, t, r, nearDistance, farDistance);

//Row obtaining for projection matrix
    double element_row3_1 = (-1.0) * ( (farDistance + nearDistance) / (farDistance - nearDistance) ); //matrix[row=3][col=3]
    double element_row3_2 = (-1.0) * ( (2 * farDistance * nearDistance) / (farDistance - nearDistance) ); //matrix[row=3][col=4]
    Vector row1 = Vector((nearDistance / r), 0, 0, 0);
    Vector row2 = Vector(0, (nearDistance / t), 0, 0);
    Vector row3 = Vector(0, 0, element_row3_1, element_row3_2);
    Vector row4 = Vector(0, 0, -1, 0);
//Set the projection matrix
    projection_matrix = MatrixFormationFromVector_WrtRow(row1, row2, row3, row4);
}
//For stage3.txt
void outputTriangleToFileStage3(Vector v1, Vector v2, Vector v3)
{
///Obtain the product with the projection matrix of each triangle vector obtained from stage2.txt
    Vector transformed_v1 = vectorProductWithMatrix(projection_matrix, v1);
    Vector transformed_v2 = vectorProductWithMatrix(projection_matrix, v2);
    Vector transformed_v3 = vectorProductWithMatrix(projection_matrix, v3);
///Make homogenous again
    transformed_v1 = vectorMakeHomogenous(transformed_v1);
    transformed_v2 = vectorMakeHomogenous(transformed_v2);
    transformed_v3 = vectorMakeHomogenous(transformed_v3);

///Output to stage3.txt
    transformed_v1.outputToStage3();
    transformed_v2.outputToStage3();
    transformed_v3.outputToStage3();
    ouptutStage3File << endl ; //output a new line
}


///To obtain the view matrix
void obtainViewMatrix()
{
///Step 1 determining the l, r, u vectors
    Vector l = vectorSubtraction(look, eye);
    l = vectorNormalize(l);
    Vector r = vectorCrossProduct(l, up);
    r = vectorNormalize(r);
    Vector u = vectorCrossProduct(r, l);
    //Make each 'w' component = 1 now since it will be added later...
    l.w = 1; r.w = 1; u.w = 1;

///Step 2 determine the Translation matrix 'T' to translate eye to (0, 0, 0)
    Matrix translation_mat = Transformation_Translation(-eye.x, -eye.y, -eye.z);

///Step 3 determine the Rotation matrix 'R' to align l, u, r with -z, y, x axes respectively
    Matrix rotation_mat = MatrixFormationFromVector_WrtRow  (   Vector(r.x, r.y, r.z, 0),
                                                                Vector(u.x, u.y, u.z, 0),
                                                                Vector(-l.x, -l.y, -l.z, 0),
                                                                Vector(0, 0, 0, 1) );
///View Matrix V = RT [Global Variable]
    view_matrix = MatrixProduct(rotation_mat, translation_mat);
}

///For stage2.txt output
void outputTriangleToFileStage2(Vector v1, Vector v2, Vector v3)
{
///Obtain the product with the view matrix of each triangle vector obtained from stage1.txt
    Vector transformed_v1 = vectorProductWithMatrix(view_matrix, v1);
    Vector transformed_v2 = vectorProductWithMatrix(view_matrix, v2);
    Vector transformed_v3 = vectorProductWithMatrix(view_matrix, v3);
///Make homogenous again
    transformed_v1 = vectorMakeHomogenous(transformed_v1);
    transformed_v2 = vectorMakeHomogenous(transformed_v2);
    transformed_v3 = vectorMakeHomogenous(transformed_v3);

///Output to stage2.txt
    transformed_v1.outputToStage2();
    transformed_v2.outputToStage2();
    transformed_v3.outputToStage2();
    ouptutStage2File << endl ; //output a new line
//Go to do things for stage3.txt
    outputTriangleToFileStage3(transformed_v1, transformed_v2, transformed_v3);
}

int triangle_num = 1; ///For debugging purposes
void outputTriangleToFile(Vector v1, Vector v2, Vector v3)
{
    ///Get the matrix which lies at the top of the stack
    Matrix topStackMatrix = stack_transformations.top();

    ///Find the product of each vector of triangle with the top_stack transformation matrix
    Vector transformed_v1 = vectorProductWithMatrix(topStackMatrix, v1);
    Vector transformed_v2 = vectorProductWithMatrix(topStackMatrix, v2);
    Vector transformed_v3 = vectorProductWithMatrix(topStackMatrix, v3);

    ///Make each transformed vector homogenous again
    transformed_v1 = vectorMakeHomogenous(transformed_v1);
    transformed_v2 = vectorMakeHomogenous(transformed_v2);
    transformed_v3 = vectorMakeHomogenous(transformed_v3);

    ///Now output to the output file
    transformed_v1.outputToStage1();
    transformed_v2.outputToStage1();
    transformed_v3.outputToStage1();
    ouptutStage1File << endl ;

    ///To output to stage2.txt
    outputTriangleToFileStage2(transformed_v1, transformed_v2, transformed_v3);

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
#if DEBUG_STACK == 1
        printf("POP IS DONE ---> stack transformations . size = "); cout << stack_transformations.size() << endl ;
        stack_transformations.top().printMatrix();
#endif // DEBUG_STACK
    }


    return toReturn;
}


int main()
{
    cout << std::fixed;
    printf("Running ... \n\n");

    fileReaderStream.open(INPUT_FILE_NAME); ///Open the input_file_stream
    if(fileReaderStream.fail() || fileReaderStream.bad())
    {
        printf("FAIL IN READING FILE scene.txt\n\n");
        return -1;
    }


    ///Open the output file streams
    ouptutStage1File.open("stage1.txt");
    ouptutStage2File.open("stage2.txt");
    ouptutStage3File.open("stage3.txt");

    ///To remove scientific notations + to set 7 decimal digits
    ouptutStage1File <<  setprecision(7) << std::fixed ;
    ouptutStage2File <<  setprecision(7) << std::fixed ;
    ouptutStage3File <<  setprecision(7) << std::fixed ;


    //Push identity matrix
    Matrix m1;
    pushToStackInitial(m1.getIdentityMatrix());


    //while(true) //Wait until the 'END' command is found
    extractFirst4Lines();
    //printEyeLookUpParams(); //Done

///Calculate the view matrix
    obtainViewMatrix();
///Calculate the projection matrix
    obtainProjectionMatrix();

    while(true)
    {
        int ret = extractCommand();
        if(ret == -1){
            break;
        }
    }

//    testStack(); //Works

    fileReaderStream.close(); //close the ifstream
    ouptutStage1File.close(); //close the ofstreams
    ouptutStage2File.close();
    ouptutStage3File.close();

    printf("Completed\n\n");

}


