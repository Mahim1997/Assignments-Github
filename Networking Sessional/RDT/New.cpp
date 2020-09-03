#include<iostream>
#include<ctime>
#include<windows.h>
#include<cstdio>
#include<string>
#include<cstring>
#include<cstdlib>
#include<vector>
#include <bitset>
#include <cmath>
#include<random>

//#include <bits/stdc++.h> //Slow
////////////////////////Modes for debug///////////////

#define DEBUG_INPUT 3
#define FULL_DEB 0
#if FULL_DEB == 1
    #define DEBUG 0
    #define NEW_DEB 0
    #define LATEST_DEB 1
    #define FINAL_DEB 1
#endif // DEBUG

#define INPUT_STRING_MODE 2
#define TESTING 0

#define LINE_SPACE_CNT 80
#define MAX_NUM 10000
#define NORMAL_RAND 0
//////////////////////Colors////////////////////////
#define GREEN 10
#define CYAN 11
#define RED 12
//#define WHITE 7


using namespace std ;
////////////////////////////To Get Current White Color///////////////////////
short WHITE;
bool getCurrentColor(short &ret)
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
        return false;
    ret = info.wAttributes;
    return true;
}

/////////////////////////Global variables///////////////////
string dataString;
int num_chars_in_row;
float probability_bit_toggle;
string generator_polynomial;
int num_parity_bits_used_hamming;
string outputFinalDataString;
/////////////////////Function declarations//////////////////
///Task 0
void debugInput();
void printInputs();
///Task 1
void padDataString();
///Task 2
void createDataBlockNormal();
void printDataBlockNormal();
///Task 3
void createDataBlockWithCheckBits();
void printDataBlockWithCheckBits();
///Task 4
void serialiseDataBitsInColumnMajor();
void printColumnMajor();
///Task 5
void appendCRCCheckSum();
void printAfterAppendingCRC();
///Task 6
void receiveFrameWithTogglingBits();
void printReceivedFrame();
///Task 7
void verifyReceivedFrameAndPrintResult();
///Task 8
void removeCRC_deserializeIntoDataBlock();
///Task 9
void removeCheckBitsAndPrint();
///Task 10
void recoverDataFinally();
///////////////Helper Class for Coloring///////////////
void printChar(char, int);
void printChar(char data, char color)
{
    if(color == 'W')
    {
        printChar(data, WHITE);
    }
    else if(color == 'C')
    {
        printChar(data, CYAN);
    }
    else if(color == 'G')
    {
        printChar(data, GREEN);
    }
    else if(color == 'R')
    {
        printChar(data, RED);
    }
}
class MyContainer
{
public:
    vector<string> string_vectors;
    vector<string> color_vectors ;
    MyContainer() {}
    void clearContainer()
    {
        string_vectors.clear();
        color_vectors.clear();
    }
    void printVectorWithColor(bool flag = false)
    {
        if(flag == true)
        {
            for(int i=0; i<string_vectors.size(); i++)
            {
                string strToPrint = string_vectors[i];
                string colorToPrint = color_vectors[i];
                for(int j=0; j<strToPrint.size(); j++)
                {
                    char data = strToPrint[j];
                    char color = colorToPrint[j];
                    printChar(data, color);
                }
                cout << endl ;
            }
            cout << endl ;
            return;
        }
        for(int i=0; i<string_vectors.size(); i++)
        {
            string str = string_vectors[i];
            string colors = color_vectors[i];
            for(int j=0; j<str.size(); j++)
            {
                if((i+j != 0) && ((i+j) % LINE_SPACE_CNT == 0))
                    cout << endl ;
                if(colors[j] == 'W')
                {
                    printChar(str[j], WHITE);
                }
                else if(colors[j] == 'C')
                {
                    printChar(str[j], CYAN);
                }
                else if(colors[j] == 'G')
                {
                    printChar(str[j], GREEN);
                }
                else if(colors[j] == 'R')
                {
                    printChar(str[j], RED);
                }
            }
        }
    }
};
class MyChar
{
    char data;
    int color;
public:
    MyChar(char d, int c)
    {
        data = d;
        color = c;
    }
    MyChar(char d)
    {
        data = d;
        color = WHITE;
    }
    void printWithColor()
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); ///Switch to self color
        cout << data ;  //Print the data in this color
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE); ///Switch back to white
    }
    char getData()
    {
        return data ;
    }
    int getColor()
    {
        return color ;
    }
    void setData(char c)
    {
        data = c;
    }
    void setColor(int col)
    {
        color = col;
    }
};
class MyString
{
    vector<MyChar> my_string_vector;
public:
    MyChar myCharAt(int idx)
    {
        if(idx < 0 || idx >= my_string_vector.size())
            return NULL ;
        return my_string_vector[idx];
    }
    string asString()
    {
        string s = "";
        for(int i=0; i<my_string_vector.size(); i++)
        {
            s += (my_string_vector[i].getData());
        }
        return s;
    }
    MyString(string str)
    {
        copyString(str);
    }
    MyString(string str, int col)
    {
        copyString(str, col);
    }
    int size_vector()
    {
        return my_string_vector.size();
    }
    void pushBack(string str, int col = WHITE)
    {
        for(int i=0; i<str.size(); i++)
        {
            char d = str[i];
            MyChar myChar(d, col);
            my_string_vector.push_back(myChar);
        }
    }
    void pushBack(char c, int col = WHITE)
    {
        MyChar myChar(c, col);
        my_string_vector.push_back(myChar);
    }
    void pushBack(MyChar mc)
    {
        pushBack(mc.getData(), mc.getColor());
    }

    void clearString()
    {
        my_string_vector.clear();
    }
    void copyString(string str, int col = WHITE)
    {
        clearString();
        pushBack(str, col);
    }
    void appendToExistingString(string str, int col = WHITE)
    {
        pushBack(str, col);
    }
    void appendToExistingString(char c, int col = WHITE)
    {
        pushBack(c, col);
    }

    void printMyStringVector()
    {
        for(int i=0; i<my_string_vector.size(); i++)
        {
            my_string_vector[i].printWithColor();
        }
        cout << endl ;
    }
};
void printChar(char data, int color = WHITE)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); ///Switch to self color
    cout << data ;  //Print the data in this color
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE); ///Switch back to white
}
void print(string str, int color = WHITE)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); ///Switch to self color
    for(int i=0; i<str.size(); i++)
    {
        if((i%LINE_SPACE_CNT) == 0 && i!=0)
        {
            cout << endl ;
        }
        cout << str[i];
    }
    cout << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE); ///Switch back to white
}
/////////////////////////Global Variables part 2. Data Blocks////////////////
vector<string> dataBlockNormal;
vector<MyString> dataBlockWithCheckBits;
string columnSerialData;
string crc_checksum;
string whole_frame_sender_side;
string whole_frame_receiver_side;

vector<string>deserializedDataBlock;
MyContainer receivedContainer;
MyContainer deserialisedContainer;

vector<string>finalBlockWithCheckBitsCorrected;
vector<string>finalBlockWithoutCheckBits;
////////////////////////////////////////////////////////////////////////////////////////////////////////
void takeInputFromConsole();
int main()
{

//If we want , we can use color level = 7 as WHITE but it is dim in intensity
///Task 0. Take inputs [also get current white color]
    getCurrentColor(WHITE);

#if DEBUG_INPUT == 0
    takeInputFromConsole();
#elif DEBUG_INPUT == 1
    debugInput();
    printInputs();
#elif DEBUG_INPUT == 2
    debugInput();
    printf("Enter data block bits: ");
    string inputEntered;
    getline(cin, inputEntered);
    dataBlockNormal.clear();
    dataBlockNormal.push_back(inputEntered);
    printf("Enter poly: ");
    generator_polynomial = "";
    cin >> generator_polynomial;

    printf("\n\n");
#elif DEBUG_INPUT == 3
    dataString = "d";
    num_chars_in_row = dataString.size();
    cout << "Data String is : " << dataString;
    printf("\nEnter generating poly: ");
    cin >> generator_polynomial;
    printf("Enter trigger prob: ");
    cin >> probability_bit_toggle;
    printf("You've entered generating poly : "); cout << generator_polynomial << endl ;
#endif
    cout << endl << endl ;
///Task 1. Pad the data string
#if DEBUG_INPUT != 2
    padDataString();
    cout << "data string after padding: " << dataString << endl << endl;
#endif // DEBUG_INPUT
///Task 2. Creating data block
    createDataBlockNormal();
    printDataBlockNormal();

    cout << endl << endl ;
///Task 3. Adding check bits to correct at most one bit error in each row of data block
    createDataBlockWithCheckBits();
    printDataBlockWithCheckBits();

///Task 4. Serializing data bits got above in column major order
    cout << endl << endl;
    serialiseDataBitsInColumnMajor();
    printColumnMajor();

///Task 5.
    cout << endl << endl ;
    appendCRCCheckSum();
    printAfterAppendingCRC();
///Task 6.
    cout << endl << endl ;
    //srand(9999);
    srand(time(0));
    receiveFrameWithTogglingBits();
    printReceivedFrame();

///Task 7.
    cout << endl << endl ;
    verifyReceivedFrameAndPrintResult();

///Task 8.
    cout << endl << endl ;
    removeCRC_deserializeIntoDataBlock();

///Task 9.
    cout <<  endl ;
    removeCheckBitsAndPrint();

///Task 10.
    cout << endl ;
    recoverDataFinally();

    cout << endl << endl << endl ;
    return 0;
}

///----------------------------------------------------------Taking inputs end------------------------------------------------------
void takeInputFromConsole()
{
    cout << "enter data string: ";
    getline(cin, dataString);
//    cin >> dataString;
    cout << "enter number of data bytes in a row <m> : ";
    cin >> num_chars_in_row;
    cout << "enter probability_bit_toggle <p> : ";
    cin >> probability_bit_toggle;
    cout << "enter generator polynomial: ";
    cin >> generator_polynomial;

}
void printVectorString(vector<string> v)
{
    for(int i=0; i<v.size(); i++)
    {
        cout << v[i] << endl ;
    }
    cout << endl;
}
void printInputs()
{
    cout << "Data String is : <" << dataString << "> , m = <" << num_chars_in_row << ">";
    cout << ", p = <" << probability_bit_toggle << ">, generator_poly = " << generator_polynomial << endl ;
}
///----------------------------------------------------------Taking inputs end------------------------------------------------------


///----------------------------------------------------------Task 1 begin------------------------------------------------------

void padDataString()
{
    ///If size of data string is NOT a multiple of 'm', append ~ to data string
    int mod = dataString.size()%num_chars_in_row;
    if(mod != 0)
    {
        for(int i=0; i<(num_chars_in_row - mod); i++)
        {
            dataString = dataString + "~";
        }
    }
}
///----------------------------------------------------------Task 1 done------------------------------------------------------

///----------------------------------------------------------Task 2 begin------------------------------------------------------

string getASCII(int num)
{
    string binary = std::bitset<8>(num).to_string();
    return binary;
}

void createDataBlockNormal()
{
    int num_rows = (dataString.size() / num_chars_in_row);
    int idx = 0 ; //Index to capture the character of dataString
    for(int i=0; i<num_rows; i++)
    {
        string rowStr = "";
        for(int j=0; j<num_chars_in_row; j++)
        {
            int num = (int)dataString[idx];
            string ascii_bin = getASCII(num);
            rowStr = rowStr + ascii_bin;
            idx++;
        }
        dataBlockNormal.push_back(rowStr);
    }

}
///Print normal data block [Task 1]
void printDataBlockNormal()
{
    cout << "data block <ascii code of m characters per row>:" << endl;
    for(int i=0; i<dataBlockNormal.size(); i++)
    {
        cout << dataBlockNormal[i] << endl;
    }
}
///----------------------------------------------------------Task 2 done------------------------------------------------------

///----------------------------------------------------------Task 3 begin------------------------------------------------------
//Function to check power of 2
bool is_power_of_2(int n)
{
    if(n == 0)
    {
        return false ;
    }
    return ((n & (n - 1)) == 0) ;
}
//Get number of ones in a row wrt power_idx
string fix(string inputString)
{
    string str = inputString;
    ///NOW ADDED at 12 pm
    //reverse(str.begin(), str.end());
    ///NOW ADDED at 12 pm
    //cout << "Inputstring size = " << inputString.size() << " , input string = " << inputString << endl ;

    int num_bits_in_row = num_chars_in_row * 8;
    int MAX_POW_USED = num_parity_bits_used_hamming;

    int cnts[MAX_POW_USED];
    for(int i=0; i<MAX_POW_USED; i++)
    {
        cnts[i] = 0;
    }

    //Loop 1 over powered indices loop
    for(int power_idx = 0; power_idx < MAX_POW_USED; power_idx++)
    {
        int powered_plus_1 = power_idx + 1;
        //Loop 2 over whole input string
        for(int i=0; i<inputString.size(); i++)
        {
            //Check for bit match
            int i_plus_1 = i+1;
            int mask = (1 << power_idx);
            if(i_plus_1 & mask)
            {
                if((is_power_of_2(i_plus_1) == false) && (i_plus_1 != powered_plus_1) && (inputString[i] == '1'))
                {
                    cnts[power_idx]++;
                }
            }
        }
    }
    ///Now fixing string
    for(int p_idx=0; p_idx<MAX_POW_USED; p_idx++)
    {
        int actual_idx = pow(2, p_idx);
        actual_idx--;
        if( (cnts[p_idx] % 2) == 0)
        {
            //Even -> Place '0'  in check bit .... 'Z' for now ,,, later replaced with color
            str[actual_idx] = 'Z';
        }
        else
        {
            //Odd -> Place '1'    in check bit .... 'M' for now ,,, later replaced with color
            str[actual_idx] = 'M';
        }
    }
    return str;
}
//Return a vector of string of new block WITHOUT check bits ... '@' in position of check bits
vector<string> getNewBlockWithoutCheckBits()
{
    int num_bits_in_row = num_chars_in_row * 8;
    int a = num_bits_in_row;
    int r = 0;
    while (a + r + 1 > pow (2, r))
    {
        r++;
    }

//    int max_num_parity_bits_row = log(num_bits_in_row) / log(2);
    int max_num_parity_bits_row = r;
    num_parity_bits_used_hamming = r;
    //printf("------------>>>>>>>NUMNPARITYT BITS USED = %d\n", num_parity_bits_used_hamming);

    //printf("-->>In getNewBlockWithoutCheckBits() .... num_bits_in_row = %d, max_parity_bit_no = %d\n", num_bits_in_row, max_num_parity_bits_row);
    int total_bits_in_row = num_bits_in_row + max_num_parity_bits_row + 1;  //Plus 1 because 2^0 th position is also taken

    vector<string> vectors;

    for(int row=0; row<dataBlockNormal.size(); row++)
    {
        string data_str_row = dataBlockNormal[row];
        string str = "";
        int data_row_idx = 0;
        for(int col=1; col<=total_bits_in_row; col++)
        {
            if(!is_power_of_2(col))
            {
                char data = data_str_row[data_row_idx];
                //myStr_row.appendToExistingString(data);
                str += data ;
                data_row_idx++;
            }
            else
            {
                str += '@';
                //myStr_row.appendToExistingString('@', GREEN);
            }
        }
        //dataBlockWithCheckBits.push_back(myStr_row);
        // cout << "--->>>Returning to vector string <" << str + "> len = " << str.size() <<"\n";
        vectors.push_back(str);
    }
    return vectors;
}
void createDataBlockWithCheckBits()
{
    vector<string> initialBlock = getNewBlockWithoutCheckBits(); ///Obtain initial Block with '@' in place of check bits
    vector<string> with_M_Z_block;
    for(int i=0; i<initialBlock.size(); i++)
    {
        string str = initialBlock[i];
        string newStr = fix(str);
        with_M_Z_block.push_back(newStr);   ///Obtain block with check bits but instead of 1->'M' and 0->'Z'
    }
    ///Now forming actual check bits block
    for(int i=0; i<with_M_Z_block.size(); i++)
    {
        string str = with_M_Z_block[i];
        MyString my_str("");
        for(int i=0; i<str.size(); i++)
        {
            char d = str[i];
            if(d == 'Z')
            {
                //Change to 0 and color GREEN
                MyChar my_char('0', GREEN);
                my_str.pushBack(my_char);
            }
            else if(d == 'M')
            {
                //Change to 1 and color GREEN
                MyChar my_char('1', GREEN);
                my_str.pushBack(my_char);
            }
            else
            {
                //No change and color WHITE
                MyChar my_char(d);
                my_str.pushBack(my_char);
            }
        }

        dataBlockWithCheckBits.push_back(my_str);
    }
}




///Print data block with check bits [Task 2]
void printDataBlockWithCheckBits()
{
///Coloring added check bits with green
    cout << "data block after adding check bits: " << endl;

    for(int i=0; i<dataBlockWithCheckBits.size(); i++)
    {
        dataBlockWithCheckBits[i].printMyStringVector();
    }
}

///----------------------------------------------------------Task 3 done------------------------------------------------------


///----------------------------------------------------------Task 4 begin------------------------------------------------------

void serialiseDataBitsInColumnMajor()
{
    vector<string> checkbitsData ;
    for(int i=0; i<dataBlockWithCheckBits.size(); i++)
    {
        string str = dataBlockWithCheckBits[i].asString();
        checkbitsData.push_back(str);
    }
    int ROW = checkbitsData.size();
    int COL = checkbitsData[0].size();
    int NEW_ROW = COL;
    int NEW_COL = ROW;
    string serialData = "";
    for(int i=0; i<NEW_ROW; i++)
    {
        for(int j=0; j<NEW_COL; j++)
        {
            serialData += checkbitsData[j][i];
        }
    }
    columnSerialData = serialData;
    string msg = "";
    int i=0;
    while(columnSerialData[i] != NULL)
    {
        msg += columnSerialData[i++];
    }
    columnSerialData = msg;
}

void printColumnMajor()
{
    cout << "data bits after column-wise serialization: " << endl;
    int cnt = LINE_SPACE_CNT;
    int x = 0 ;
    while(x < columnSerialData.size())
    {
        if(x != 0 && x%cnt == 0)
        {
            cout << endl ;
        }
        cout << columnSerialData[x];
        x++;
    }
}

///----------------------------------------------------------Task 4 done------------------------------------------------------

///----------------------------------------------------------Task 5 begin------------------------------------------------------

void appendCRCCheckSum()
{
    int num_zeroes_to_append = generator_polynomial.size() - 1;
    string appended_msg = columnSerialData;
//    appended_msg += 'A';

#if DEBUG == 1
    printf("Printing generator polynomial (size = %d) : ", generator_polynomial.size());
    cout << generator_polynomial << endl;
    printf("Serial data size = %d, appended data size = %d\n", columnSerialData.size(), appended_msg.size());

#endif // DEBUG
//Initially 0 boshano
    for(int i=0; i<num_zeroes_to_append; i++)
    {
        appended_msg += '0';
    }

#if DEBUG == 1
    printf("In Sender , BEFORE DIVIDING ... ,,,, printing appended msg ... columnSerialData.size() = %d \n", columnSerialData.size());
    print(appended_msg);
#endif // DEBUG

//Dividing
#if DEBUG == 1
    printf("The checking is with columnSerialData.size = %d in SENDER\n", columnSerialData.size());
#endif // DEBUG
    for(int i=0; i<columnSerialData.size(); i++)
    {
        int j = 0;
        int k = i;
        //Check whether divisible ..
        if(appended_msg[k] >= generator_polynomial[j])
        {
            for(j = 0, k = i; j < generator_polynomial.size() ; j++, k++)
            {
//                printf("Sender side if appended[%d] <><> poly[%d]\n", k, j);
                if(appended_msg[k] == generator_polynomial[j])
                {
                    appended_msg[k] = '0';
                }
                else
                {
                    appended_msg[k] = '1';
                }
            }
        }

    }

#if DEBUG == 1
    printf("AFTER DIVISION ... prinitng apppended msg ... \n");
    print(appended_msg);
#endif // DEBUG
    crc_checksum = "";
    for(int i=columnSerialData.size(); i<appended_msg.size(); i++)
    {
        crc_checksum += appended_msg[i];
    }
//    makeFinalFrame();
    whole_frame_sender_side = (columnSerialData + crc_checksum);

}

void printAfterAppendingCRC()
{
    cout << "data bits after appending CRC checksum <sent frame> \n";

    for(int i=0; i<whole_frame_sender_side.size(); i++)
    {
        if((i != 0) && (i%LINE_SPACE_CNT == 0))
        {
            cout << endl;
        }
        if(i < columnSerialData.size())
        {
            printChar(whole_frame_sender_side[i]);
        }
        else
        {
            printChar(whole_frame_sender_side[i], CYAN);
        }
    }
#if DEBUG == 1
    printf("\nOnly checksum <");
    cout << crc_checksum << ">\n";
#endif // DEBUG
}

///----------------------------------------------------------Task 5 done------------------------------------------------------

///----------------------------------------------------------Task 6 begin------------------------------------------------------
float myRand()
{
    int MAX = 10;   ///Smaller MAX -> Less number of RED toggled bits

    int randNum = rand()%MAX;
    float percent = (float)randNum / (float)MAX ;
    return percent;
}
void receiveFrameWithTogglingBits()
{
#if DEBUG == 1
    //cout << "Inside receiveFrameWithTogglingBits() .... " << endl ;
#endif
    whole_frame_receiver_side = whole_frame_sender_side;

    float thresh = probability_bit_toggle/10; //For threshold

    int range_to = MAX_NUM;
    int range_from = 0;
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  distr(range_from, range_to);

    for(int i=0; i<whole_frame_receiver_side.size(); i++)
    {
        float rn = distr(generator);
        float p = ((float)rn / (float)(range_to - range_from));

        float randNumNow = 0.0 ;
#if NORMAL_RAND == 0
        randNumNow = p;
#elif NORMAL_RAND == 1
        randNumNow = myRand();
#endif // NORMAL_RAND

#if DEBUG == 1
//       printf("Rand num for idx = %d, got = %f\n", i, randNumNow);
#endif // DEBUG
        if((randNumNow-probability_bit_toggle) <= thresh)
        {
#if DEBUG == 1
            //         printf("------>>>TOGGLING this bit now ... for idx = %d", i);
#endif // DEBUG
            if(whole_frame_receiver_side[i] == '0')
            {
                whole_frame_receiver_side[i] = '1';
            }
            else
            {
                whole_frame_receiver_side[i] = '0';
            }

        }
    }
}
void formReceivedContainer()
{
    receivedContainer.clearContainer();
    //Just one length er str array howa uchit
    receivedContainer.string_vectors.push_back(whole_frame_receiver_side);
    string colorFormed = "";
    for(int i=0; i<whole_frame_receiver_side.size(); i++)
    {
        if(whole_frame_sender_side[i] != whole_frame_receiver_side[i])
        {
//            receivedContainer.color_vectors.push_back('R');
            colorFormed += 'R';
        }
        else
        {
//            receivedContainer.color_vectors.push_back('W');
            colorFormed += 'W';
        }
    }
    receivedContainer.color_vectors.push_back(colorFormed);
}
void printReceivedFrame()
{
    cout << "received frame: " << endl ;
    for(int i=0; i<whole_frame_receiver_side.size(); i++)
    {
        if((i != 0) && (i%LINE_SPACE_CNT) == 0)
        {
            cout << endl ;
        }
        if(whole_frame_receiver_side[i] != whole_frame_sender_side[i])
        {
            ///mismatch so print in red ...
            printChar(whole_frame_receiver_side[i], RED);
        }
        else
        {
            printChar(whole_frame_receiver_side[i]);
        }
    }
    formReceivedContainer();
}

///----------------------------------------------------------Task 6 done------------------------------------------------------

///----------------------------------------------------------Task 7 begin------------------------------------------------------
void verifyReceivedFrameAndPrintResult()
{
//Same thing done before
    string appended_msg = "";
    for(int i=0; i<whole_frame_receiver_side.size(); i++)
    {
        appended_msg += whole_frame_receiver_side[i];
    }
//    appended_msg += "###";
#if DEBUG == 1
    printf("In receiver side ... Printing generator polynomial (size = %d) : ", generator_polynomial.size());
    cout << generator_polynomial << endl;
    printf("BEFORE DIVIDING ... ,,,, printing appended msg size = %d ... \n", appended_msg.size());
    print(appended_msg);
#endif // DEBUG
    ///No need to append zeros because redundant code was already passed along
//Dividing
    int actual_frame_size = whole_frame_receiver_side.size() - (generator_polynomial.size() - 1); //CRC_CHECKSUM len = polynomial.len - 1
    int num_zeroes_appended = (generator_polynomial.size() - 1);
#if DEBUG == 1
    printf("Receiver ... actual_frame_size = %d, num_zeroes_app = %d\n", actual_frame_size, num_zeroes_appended);
#endif // DEBUG

    for(int i=0; i<(actual_frame_size) /*+ num_zeroes_appended)*/ ; i++)
    {
        //cout << "i = " << i << endl ;
        int j = 0;
        int k = i;
        //Check whether divisible ..
        if(appended_msg[k] >= generator_polynomial[j])
        {
            for(j = 0, k = i; j < generator_polynomial.size() ; j++, k++)
            {
//                printf("if appended_msg[%d] CHECK poly[%d]\n", k, j);
                if(appended_msg[k] == generator_polynomial[j])
                {
                    appended_msg[k] = '0';
                }
                else
                {
                    appended_msg[k] = '1';
                }
            }
        }

    }

#if DEBUG == 1
    printf("AFTER DIVISION ... prinitng apppended msg ... \n");
    print(appended_msg);
#endif // DEBUG



    string remainder = "";
#if DEBUG == 1
    printf("Actual_Frame_Size = %d, whole_frame_recSize = %d\n", actual_frame_size, whole_frame_receiver_side.size());
#endif // DEBUG
    for(int i=actual_frame_size; i<appended_msg.size(); i++)
    {
        remainder += appended_msg[i];  ///Take last bits
    }
#if DEBUG == 1
    printf("After cutting remainder ... printing it ");
    print(remainder);
#endif // DEBUG


    bool hasChanged = false;
    for(int i=0; i<remainder.size(); i++)
    {
        if(remainder[i] != '0')
        {
            hasChanged = true;
        }
    }
    if(hasChanged == true)
    {
        cout << "result of CRC checksum matching: error detected\n";
    }
    else
    {
        cout << "result of CRC checksum matching: no error detected\n";
    }
}
///----------------------------------------------------------Task 7 done------------------------------------------------------


///----------------------------------------------------------Task 8 begin------------------------------------------------------

void removeCRC_deserializeIntoDataBlock()
{
//Changing to data block

    //First removing checksum bits
    string whole = receivedContainer.string_vectors[0];
    string col_whole = receivedContainer.color_vectors[0];
    string str_withoutCheckSum = "";
    string col_withoutCheckSum = "";
    int num_zeroes = generator_polynomial.size() - 1;
    int actual_data_frame_size = whole.size() - num_zeroes;
    for(int i=0; i<actual_data_frame_size; i++)
    {
        str_withoutCheckSum += whole[i];
        col_withoutCheckSum += col_whole[i];
    }

    MyContainer container_withoutCheckSum;
    container_withoutCheckSum.clearContainer();
    container_withoutCheckSum.string_vectors.push_back(str_withoutCheckSum);
    container_withoutCheckSum.color_vectors.push_back(col_withoutCheckSum);

///Now transfer back to blocks of data
    int num_bits_in_row = (num_parity_bits_used_hamming + dataBlockNormal[0].size());
    int num_rows = (dataString.size() / num_chars_in_row);

    //cout << "num bits in a row = " << num_bits_in_row << endl ;
    //cout << "num rows = " << num_rows << endl ;
//str_withoutCheckSum,col_withoutCheckSum
    string arr_str[num_rows];
    string arr_col[num_rows];
    for(int i=0; i<num_rows; i++)
    {
        arr_str[i] = "";
        arr_col[i] = "";
    }
    int cnt = 0;
    int pos_row = 0;
    while(cnt < str_withoutCheckSum.size())
    {
        char nowCharData = str_withoutCheckSum[cnt];
        char nowColor = col_withoutCheckSum[cnt];
        arr_str[pos_row] += nowCharData;
        arr_col[pos_row] += nowColor;
        cnt++;
        pos_row = (pos_row + 1) % num_rows;
    }
    deserialisedContainer.clearContainer();
    for(int i=0; i<num_rows; i++)
    {
        deserialisedContainer.string_vectors.push_back(arr_str[i]);
        deserialisedContainer.color_vectors.push_back(arr_col[i]);
    }
    printf("data block after removing CRC checksum bits: \n");
    deserialisedContainer.printVectorWithColor(true);
}
///----------------------------------------------------------Task 8 done------------------------------------------------------

///----------------------------------------------------------Task 9 begin------------------------------------------------------
int getCountParityData(int power_idx, string str)
{
    //std::reverse(str.begin(), str.end());

    int actual_plus_1 = pow(2, power_idx);
    int actual_position = actual_plus_1 - 1;
    int mask = 1 << power_idx;
#if NEW_DEB == 1
    printf("Inside newF powerIdx = %d ,numParityBitsHam = %d, actualpos = %d, actual+1 = %d, mask = %d ,\nstrSize = %d, str = ",
            power_idx, num_parity_bits_used_hamming , actual_position, actual_plus_1 ,mask, str.size() );
    cout << str << endl;
#endif

///This below ALSO INCLUDES PARITY BIT
    int total_num_ones = 0;
#if NEW_DEB == 1
    printf("Printing str with idx\n");
    for(int i=0; i<str.size(); i++){
        printf("<%d, %c>\n", i, str[i]);
    }
#endif // NEW_DEB
    for(int i=0; i<str.size(); i++){
        int numToCheck = i+1;
        if( (numToCheck & (mask) )!= 0){
            ///!ZERO -> YES!!
            string binary = std::bitset<8>(i).to_string();
            int nc = numToCheck - 1;
#if NEW_DEB == 1
            printf("For numCheck = %d, numCheck - 1   = %2d, BitMap: ",numToCheck, nc); cout << binary;
            printf(" MATCHED , powerIdx = %d, actualLoc+1 = %d \n",power_idx, actual_plus_1);
#endif // NEW_DEB
            if(str[nc] == '1'){
#if NEW_DEB == 1
            printf("totalNUM= %d ++ for nc = %d\n", total_num_ones, nc);
#endif // NEW_DEB
                total_num_ones++;
            }
        }
    }


#if NEW_DEB == 1
    printf("\nReturning totalNumOnes = %d\n\n", total_num_ones);
#endif // NEW_DEB
    return total_num_ones;
}

void removeCheckBitsAndPrint()
{

    finalBlockWithoutCheckBits.clear();
    vector<string> nowBlock ;

    for(int i=0; i<deserialisedContainer.string_vectors.size(); i++){
        nowBlock.push_back(deserialisedContainer.string_vectors[i]);
    }


#if NEW_DEB == 1
    printf("Printing the intermediate block\n");
    printVectorString(nowBlock);
#endif // NEW_DEB

    int cnts[num_parity_bits_used_hamming];
    for(int i=0; i<num_parity_bits_used_hamming; i++){cnts[i] = 0;}

    for(int row=0; row<nowBlock.size(); row++)
    {
        int fullCnt = 0;
        string toCorrect = nowBlock[row];
        for(int power_idx=0; power_idx<num_parity_bits_used_hamming; power_idx++)
        {
            cnts[power_idx] = getCountParityData(power_idx, toCorrect);
        }
//        cout << endl ;
        for(int pIdx=0; pIdx < num_parity_bits_used_hamming; pIdx++)
        {
            if(cnts[pIdx]%2 != 0)
            {
                ///Odd parity OVERALL ....
                fullCnt += (pow(2, (pIdx)));  ///THE ACTUAL LOCATION
            }
        }
        if(fullCnt != 0){
            ///ERROR HAS OCCURED at fullCNT bit
            int bitPos = fullCnt - 1;
#if LATEST_DEB == 1
            printf("-->> ERROR has occured for row = %d, at fullCnt = %d, bitPos = %d , str = ", row, fullCnt, bitPos);
            cout << toCorrect << endl ;
#endif
            string newStr = toCorrect;
            ///Toggling the bit of error
            if(newStr[bitPos] == '0'){
                newStr[bitPos] = '1';
            }
            else{
                newStr[bitPos] = '0';
            }
            finalBlockWithCheckBitsCorrected.push_back(newStr);
        }
        else{ ///Undetectable error .. push this string
            finalBlockWithCheckBitsCorrected.push_back(toCorrect);
        }
    }

#if LATEST_DEB == 1
        printf("AFTER Correcting error bits ... printing WITH check bits ...\n");
        printVectorString(finalBlockWithCheckBitsCorrected);
#endif // LATEST_DEB
    ///Removing check bits now
    finalBlockWithoutCheckBits.clear();
    for(int i=0; i<finalBlockWithCheckBitsCorrected.size(); i++){
        string withCheckBitsStr = finalBlockWithCheckBitsCorrected[i];
        string correctedStr = "";
        for(int i=0; i<withCheckBitsStr.size(); i++){

            if(is_power_of_2(i+1) == false){
#if LATEST_DEB == 1
                //printf("COPYING FOR i = %d\n", i);
#endif // LATEST_DEB
                correctedStr += withCheckBitsStr[i];
            }
        }
        finalBlockWithoutCheckBits.push_back(correctedStr);
#if LATEST_DEB == 1
            cout << endl ;
#endif // LATEST_DEB
    }
    printf("data block after removing check bits: \n");
    printVectorString(finalBlockWithoutCheckBits);
}

///----------------------------------------------------------Task 9 done------------------------------------------------------

///----------------------------------------------------------Task 10 begin------------------------------------------------------
int binaryToDecimal(string n)
{
    string num = n;
    int dec_value = 0;

    // Initializing base value to 1, i.e 2^0
    int base = 1;

    int len = num.length();
    for (int i=len-1;i>=0;i--)
    {
        if (num[i] == '1')
            dec_value += base;
        base = base * 2;
    }

    return dec_value;
}
void recoverDataFinally()
{
//outputFinalDataString
//    finalBlockWithoutCheckBits

    vector<string> ascii_binary;
    ascii_binary.clear();
    vector<int> ascii_int;
    string temp = "";
    int cnt = 1;
    for(int i=0; i<finalBlockWithoutCheckBits.size(); i++){
        string rowStr = finalBlockWithoutCheckBits[i];
        for(int j=0; j<rowStr.size(); j++){
            //printf("---->>> j = %d\n", j);
            temp += rowStr[j];
            if(cnt % 8 == 0){
//                printf("-->>> cnt = %d, push temp = ", cnt); cout << temp << endl ;
                ascii_binary.push_back(temp);
//                int ascii_temp = std::stoi(temp, nullptr, 2);
                int ascii_temp = binaryToDecimal(temp);
                ascii_int.push_back(ascii_temp);
                temp = "";
            }
            cnt++ ;
        }
    }
#if FINAL_DEB == 1
    printf("-->>Final printing vector of ASCII binary strings ... \n");
    printVectorString(ascii_binary);
    printf("-->Printing ascii vector int \n");
    for(int i=0; i<ascii_int.size(); i++){
        printf("%d ", ascii_int[i]);
    }
    printf("\n");
#endif // FINAL_DEB
    outputFinalDataString = "";
    for(int i=0; i<ascii_int.size(); i++){
        outputFinalDataString += (char)(ascii_int[i]);
    }
    cout << "output frame: " << outputFinalDataString << endl ;
}



///----------------------------------------------------------Task 10 done------------------------------------------------------

////////////////////////Test Cases Input////////////////
void debugInput()
{
#if INPUT_STRING_MODE == 1
    dataString = "Hamming Code";
    num_chars_in_row = 2;
    probability_bit_toggle = 0.05;
    generator_polynomial = "10101";
#elif INPUT_STRING_MODE == 2
    dataString = "a";
    num_chars_in_row = 1;
    probability_bit_toggle = 0;
    generator_polynomial = "101";
#elif INPUT_STRING_MODE == 3
    dataString = "Computer Networks";
    num_chars_in_row = 4;
    probability_bit_toggle = 0.04;
    generator_polynomial = "1010111";
#elif INPUT_STRING_MODE == 4
    dataString = "Error Detection";
    num_chars_in_row = 3;
    probability_bit_toggle = 0.02;
    generator_polynomial = "10111";
#elif INPUT_STRING_MODE == 5
    dataString = "physical layer";
    num_chars_in_row = 3;
    probability_bit_toggle = 0.05;
    generator_polynomial = "1010";
#elif INPUT_STRING_MODE == 6
    dataString = "Error Correction";
    num_chars_in_row = 4;
    probability_bit_toggle = 0.02;
    generator_polynomial = "101011";
#elif INPUT_STRING_MODE == 7
    dataString = "no error";
    num_chars_in_row = 3;
    probability_bit_toggle = 0;
    generator_polynomial = "101";
#elif INPUT_STRING_MODE == 8
    dataString = "many errors";
    num_chars_in_row = 4;
    probability_bit_toggle = 0.1;
    generator_polynomial = "10001";
#endif
}
