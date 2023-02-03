#include <iostream>
#include <fstream>
#include <ctype.h>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

struct Timestamp {
    int hour;
    int minu;
    int sec;
    int mili;
};

int totalMili(int hour, int minu, int sec, int mili)
{
    return (mili + 1000*sec + 1000*60*minu + 1000*60*60*hour);
}

Timestamp stringToTimestamp(string strTime)
{
    Timestamp t_stamp;

    t_stamp.hour = stoi(strTime.substr(0,2));
    t_stamp.minu= stoi(strTime.substr(3,2));
    t_stamp.sec = stoi(strTime.substr(6,2));
    t_stamp.mili = stoi(strTime.substr(9,3));

    return t_stamp;
}

Timestamp breakdownMili(int sum_in_mili)
{
    Timestamp t_stamp;

    t_stamp.hour = sum_in_mili/3600000;
    t_stamp.minu = (sum_in_mili - t_stamp.hour*3600000)/60000;
    t_stamp.sec = (sum_in_mili - t_stamp.hour*3600000 - t_stamp.minu*60000)/1000;
    t_stamp.mili = (sum_in_mili - t_stamp.hour*3600000 - t_stamp.minu*60000 - t_stamp.sec*1000);

    return t_stamp;
}

string timestampToString(Timestamp t_stamp)
{
    stringstream ss;
    ss << setw(2) << setfill('0') << to_string(t_stamp.hour)
    << ":" << setw(2) << setfill('0') << to_string(t_stamp.minu)
    << ":" << setw(2) << setfill('0') << to_string(t_stamp.sec)
    << "," << setw(3) << setfill('0') << to_string(t_stamp.mili);
    string s = ss.str();
    return s;
}

string getNewTimestamp(string strTime, int delayTime)
{
    Timestamp t_stamp;
    Timestamp new_t_stamp;

    t_stamp = stringToTimestamp(strTime);

    int sum_in_mili = totalMili(t_stamp.hour, t_stamp.minu, t_stamp.sec, t_stamp.mili);

    sum_in_mili += delayTime;

    new_t_stamp = breakdownMili(sum_in_mili);

    return timestampToString(new_t_stamp);
}

int main(int argc, char **argv)
{
    string myText;
    string beginTimestamp;
    string endTimestamp;
    string t1;
    string t2;
    string newTimestamp;
    int delayTime = 25000;
    string inputPath(argv[1]);
//    string inputPath = "/home/ducdung/Downloads/adventure-time-s01-outsync/S01E03 - Prisoners of Love.eng.srt";
    string episodeName = inputPath.substr(inputPath.find("S01E") + 1);
    string outputPath = "S01E" + episodeName.substr(3, 2) + "_synced.eng.srt";

    ifstream MyReadFile(inputPath.c_str());
    ofstream MyWriteFile(outputPath.c_str());

//    cout << episodeName << "\n";

    while(getline(MyReadFile, myText)){
        if(myText[2]==':'){             // check if the line contain the ':' character

            beginTimestamp = myText.substr(0, 12);
            t1 = getNewTimestamp(beginTimestamp, delayTime);

            endTimestamp = myText.substr(17, 12);
            t2 = getNewTimestamp(endTimestamp, delayTime);

            newTimestamp = t1 + " --> " + t2;
            MyWriteFile << newTimestamp << "\r";
        }
        else{
            MyWriteFile << myText;
        }
    }

    MyWriteFile.close();
    cout << "Write new subtitle to " << outputPath << ": Done!" << "\n";
    return 0;
}
