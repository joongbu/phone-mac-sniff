/* mac address information
 *
    "한승균" mac="90:00:db:bb:98:c5"
    "백종열" mac="64:bc:0c:68:e5:71"
    "이혜빈" mac="48:59:29:f4:a5:87"
    "지우중" mac="c4:43:8f:d1:51:56"
    "유치호" mac="94:76:b7:b9:f7:19"
    known mac : 78:0c:b8:75:4c:9d
    known mac : 28:27:bf:ef:3f:6c
//input mac address 00:00:00:00:00:00
*/
#include "project.h"
int ts;
void reset() //thread
{
    while(1)
    {
        curr_time = time(NULL);
        curr_tm = localtime(&curr_time);
        if(ts == 5 && curr_tm->tm_min % 5 == 0 && curr_tm->tm_sec == 0)
        {
            t.year = std::to_string(curr_tm->tm_year + 1900);
            t.month = std::to_string(curr_tm->tm_mon +1);
            t.day = std::to_string(curr_tm->tm_mday);
            t.hour = std::to_string(curr_tm->tm_hour);
            t.minute = std::to_string(curr_tm->tm_min);
            date = t.year + "-" + t.month + "-" + t.day;
            d_time = t.hour + ":" + t.minute;
            sleep(1);
            data.insertdata(3);
            data.insertdata(4);
            mtx.lock();
            atten.clear();
            un.clear();
            unsize=0;
            mtx.unlock();
        }
        if(ts == 30 && curr_tm->tm_min % 30 == 0 && curr_tm->tm_sec == 0)
        {
            t.year = std::to_string(curr_tm->tm_year + 1900);
            t.month = std::to_string(curr_tm->tm_mon +1);
            t.day = std::to_string(curr_tm->tm_mday);
            t.hour = std::to_string(curr_tm->tm_hour);
            t.minute = std::to_string(curr_tm->tm_min);
            date = t.year + "-" + t.month + "-" + t.day;
            d_time = t.hour + ":" + t.minute;
            sleep(1);
            data.insertdata(3);
            data.insertdata(4);
            mtx.lock();
            atten.clear();
            unsize=0;
            un.clear();
            mtx.unlock();
        }
        if(ts == 60 && curr_tm->tm_min == 0 && curr_tm->tm_sec == 0)
        {
            t.year = std::to_string(curr_tm->tm_year + 1900);
            t.month = std::to_string(curr_tm->tm_mon +1);
            t.day = std::to_string(curr_tm->tm_mday);
            t.hour = std::to_string(curr_tm->tm_hour);
            t.minute = std::to_string(curr_tm->tm_min);
            date = t.year + "-" + t.month + "-" + t.day;
            d_time = t.hour + ":" + t.minute;
            sleep(1);
            data.insertdata(3);
            data.insertdata(4);
            mtx.lock();
            atten.clear();
            unsize=0;
            un.clear();
            mtx.unlock();
        }
    }
}
int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " <<* argv << " <interface>" << endl;
        return 1;
    }
    // search probe request
    interface = argv[1];
    stu_info student_information;
    cout<<"setting time :(5, 30, 60)";
    cin>>ts;
    if(ts == 5 || ts == 30 || ts == 60)
    {
    std::thread first(reset);
    first.detach();

    std::thread([] {
        probeSniffer  probe;
        probe.running(interface);
    }).detach();
    int select;
    cout<<"student data load?(yes : 1)";
    cin>>select;
    if(select == 1)
        data.load(select);
    while(1)
    {
        printf("\e[1;1H\e[2J");
        cout<<"1. let's save student impormation"<<endl;
        cout<<"2. view student attendance log"<<endl;
        cout<<"3. print DB log"<<endl;
        cout<<"4. exit"<<endl;
        cin>>select;
        switch(select)
        {
        case 1 :
            printf("\e[1;1H\e[2J");
            student_information.save_info();
            cout<<"exit : 0"<<endl;
            cin>>select;
            break;
        case 2:
            printf("\e[1;1H\e[2J");
            student_information.time_log();
            cout<<"exit : 0"<<endl;
            cin>>select;
            break;
        case 3:
            printf("\e[1;1H\e[2J");
            data.load();
            cout<<"exit : 0"<<endl;
            cin>>select;
            break;
        default :
            printf("\e[1;1H\e[2J");
            break;
        }
        if(select == 4) break;
    }
    }
    else
        cout<<"fail timelog setting"<<endl;
}
