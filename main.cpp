#include "project.h"
 DB data;
void reset()
{
    while(1)
    {
        curr_time = time(NULL);
        curr_tm = localtime(&curr_time);
        if(curr_tm->tm_min % 5 == 0 && curr_tm->tm_sec == 0)
        {
            t.year = std::to_string(curr_tm->tm_year + 1900);
            t.month = std::to_string(curr_tm->tm_mon +1);
            t.day = std::to_string(curr_tm->tm_mday);
            t.hour = std::to_string(curr_tm->tm_hour);
            t.minute = std::to_string(curr_tm->tm_min);
            sleep(1);
            data.insertdata(2);
            atten.clear();
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
    size = 3;
    std::thread([] {
        probeSniffer  probe;
        probe.running(interface);
    }).detach();
    std::thread first(reset);
    first.detach();
/*
    stu[0].name="한승균";
    stu[0].DB_mac="90:00:db:bb:98:c5";
    stu[0].mac ="90:00:db:bb:98:c5";
    stu[1].name="백종열";
    stu[1].DB_mac="64:bc:0c:68:e5:71";
    stu[1].mac ="64:bc:0c:68:e5:71";
    stu[2].name ="이혜빈";
    stu[2].DB_mac= "48:59:29:f4:a5:87";
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
    */
    int select;
    cout<<"student data load?(yes : 0)";
    cin>>select;
    if(select == 0)
    {
        data.load(select);

    }

    while(1)
    {
        printf("\e[1;1H\e[2J");
        cout<<"1. let's save student impormation"<<endl;
        cout<<"2. view student attendance log"<<endl; //stu[0].name != null db log thread
        cout<<"3. print DB log"<<endl;
        cout<<"4. exit"<<endl;
        cin>>select;

        switch(select)
        {
        case 1 :
            printf("\e[1;1H\e[2J");
            student_information.save_info();
            for(int i = 0 ; i < size ; i ++)
            //data.insertdata(1);
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
            cout<<"year  mon  day  hour  min  name  addr  attendance"<<endl;
            data.load(1);
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


