#include <iostream>
#include <fstream>
#include <ctype.h>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

string increaseTime(string strTime, int delayTime)
{
    int hour = stoi(strTime.substr(0,2));
    int minu = stoi(strTime.substr(3,2));
    int sec = stoi(strTime.substr(6,2));
    int mili = stoi(strTime.substr(9,3));

    int sum_in_mili = mili + 1000*sec + 1000*60*minu + 1000*60*60*hour;

    sum_in_mili += delayTime;

    hour = sum_in_mili/3600000;
    minu = (sum_in_mili - hour*3600000)/60000;
    sec = (sum_in_mili - hour*3600000 - minu*60000)/1000;
    mili = (sum_in_mili - hour*3600000 - minu*60000 - sec*1000);

    stringstream ss;
    ss << setw(2) << setfill('0') << to_string(hour)
    << ":" << setw(2) << setfill('0') << to_string(minu)
    << ":" << setw(2) << setfill('0') << to_string(sec)
    << "," << setw(3) << setfill('0') << to_string(mili);
    string s = ss.str();
    return s;
}

int main()
{
    string myText;
    string beginTimestamp;
    string endTimestamp;
    struct tm tm = {0};
    char buffer [80];
    string t1;
    string t2;
    string newTimestamp;
    int delayTime = 25000;


    ifstream MyReadFile("/home/ducdung/Downloads/adventure-time-s01-outsync/S01E01 - Slumber Party Panic.eng.srt");
    ofstream MyWriteFile("S01E01_synced.eng.srt");

    while(getline(MyReadFile, myText)){
        if(myText[2]==':'){             // check if the line contain the ':' character

            beginTimestamp = myText.substr(0, 12);
            t1 = increaseTime(beginTimestamp, delayTime);

            endTimestamp = myText.substr(17, 12);
            t2 = increaseTime(endTimestamp, delayTime);

            newTimestamp = t1 + " --> " + t2;
            MyWriteFile << newTimestamp << "\r";
        }
        else{
            MyWriteFile << myText;
        }
    }

    MyWriteFile.close();
    return 0;
}
