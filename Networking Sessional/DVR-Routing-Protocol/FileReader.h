#include <iostream>
#include <fstream>
#include<istream>
#include <string>
#include<vector>
#include<sstream>
#include<set>
using namespace std;

class FileReader{
    string filename;
    int numberOfRouters;
public:
    std::vector<string> sourceIps;
    std::vector<string> destIps;
    std::vector<string> costs_string;
    std::vector<int> costs;
    set<string> setOfIPAddresses;

    int getNumberOfRouters(){return numberOfRouters;}

    FileReader(string s){
        filename = s;
    }
    void readFile(){
        int cnt = 0;
        fstream file;
        string word, t, q;
        file.open(filename.c_str());

        vector<string>vec;
        while (file >> word)
        {
            vec.push_back(word);
            if(cnt == 2){
                cnt = 0;
                sourceIps.push_back(vec[0]);
                destIps.push_back(vec[1]);
                costs_string.push_back(vec[2]);

                string str1 = vec[2];
                stringstream geek(str1);
                int x = 0;
                geek >> x;
                costs.push_back(x);

                vec.clear();
                continue;
            }
            else{
                cnt++;
            }
        }

        for(int i=0; i<sourceIps.size(); i++){
            setOfIPAddresses.insert(sourceIps[i]);
            setOfIPAddresses.insert(destIps[i]);
        }
        numberOfRouters = (int)setOfIPAddresses.size();

//        cout << "Inside FileReader.h .. size vectors = " << sourceIps.size()<< endl;
       // cout << "FINALLY SET SIZE : " << setOfIPAddresses.size();

    }
};
