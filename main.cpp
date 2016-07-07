#include "project.h"
#include <mysql/mysql.h>
/*
void database()
{
    DB data;
    while(1)
    {
    for(int i = 0; i< size; i++)
    data.insertdata(i,1);
    }
}
*/
int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " <<* argv << " <interface>" << endl;
        return 1;
    }
    // search probe request

    interface = argv[1];
    stu_info student_information;
    size = 3;
    std::thread([] {
        probeSniffer  probe;
        probe.running(interface);
    }).detach();
    stu[0].name="한승균";
    stu[0].mac ="90:00:db:bb:98:c5";
    stu[1].name="백종열";
    stu[1].mac ="64:bc:0c:68:e5:71";
    stu[2].name ="이혜빈";
    stu[2].mac = "48:59:29:f4:a5:87";
    int i;
    for(i=0;i<size;i++)
    {
        ssids_type::iterator it = ssids.find(stu[i].mac);
        if(it == ssids.end()){
            try{
                ssids.insert(stu[i].mac);
            }
            catch(runtime_error&) {
                // No ssid, just ignore it.
            }
        }
    }

    //use db thread
    int select;
    while(1)
    {
        printf("\e[1;1H\e[2J");
        cout<<"1. let's save student impormation"<<endl;
        cout<<"2. view student attendance log"<<endl; //stu[0].name != null db log thread
        cout<<"3. quit(3)"<<endl;
        cin>>select;

        switch(select)
        {
        case 1 :
            printf("\e[1;1H\e[2J");
            //자료불러오기기능 추가 하고,bool형으로 해서  true 가되면 save 스킵
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
        default :
            printf("\e[1;1H\e[2J");
            break;
        }
        if(select == 3) break;
    }

}
//내일은 clear 문제 도 해결해야한다.

