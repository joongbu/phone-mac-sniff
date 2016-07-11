#ifndef PROJECT_H
#define PROJECT_H
#include <iostream>
#include <stdio.h>
#include <set>
#include <string>
#include <tins/tins.h>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include <errno.h>
using std::cin;
using std::set;
using std::cout;
using std::endl;
using std::string;
using std::runtime_error;
using namespace std;
using namespace Tins;
typedef HWAddress<6> HW;
typedef Dot11::address_type address_type;
typedef set<address_type> ssids_type;//mac address
typedef set<string> attendance;//name
ssids_type ssids;//set <> mac address
attendance atten;//set <> name
string interface;
struct student
{
    string name;
    string DB_mac;
    address_type mac;
    string check ="x";

};
struct _time
{
    string year;
    string month;
    string day;
    string hour;
    string minute;
};
time_t curr_time;
struct tm *curr_tm;
_time t;
int size;
student stu[500];
//DB ID: root passwd : 123

class DB
{
public :
    void insertdata(int select);
    void load(int select);
private :
    const char *ID = "root";
    const char *passwd = "123";
};
void DB::insertdata(int select)
{
    int i=0;
    MYSQL mysql;
    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql, NULL,ID,passwd,"student_data",3306, (char *)NULL, 0))
    {
        printf("%s＼n",mysql_error(&mysql));
        exit(1) ;
    }
    printf("connect.....\n") ;
    if(mysql_query(&mysql, "USE student_data") )
    {
        printf("%s＼n", mysql_error(&mysql));
        exit(1);
    }

    for(i = 0 ; i <size ; i++)
    {
        std::string query1 = "INSERT INTO information(name,mac_address) VALUES ('"+stu[i].name+"','"+stu[i].DB_mac+"')";
        std::string query2 = "INSERT INTO timelog(year,mon,day,hour,minute,name,mac_addr,attendance) VALUES ('"+t.year+"','"+t.month+"','"+t.day+"','"+t.hour+"','"+t.minute+"','"+stu[i].name+"','"+stu[i].DB_mac+"','"+stu[i].check+"')";
        if(select == 1)
        {
            if(mysql_query(&mysql,query1.c_str()))
            {
                printf("%s＼n", mysql_error(&mysql));
                exit(1) ;
            }

        }
        else if(select == 2)
        {
            std::string query1 = "INSERT INTO information(name,mac_address) VALUES ('"+stu[size+1].name+"','"+stu[size+1].DB_mac+"')";
            if(mysql_query(&mysql,query1.c_str()))
            {
                printf("%s＼n", mysql_error(&mysql));
                exit(1) ;
            }

        }

        else if(select == 3)
        {
            if(mysql_query(&mysql,query2.c_str()))
            {
                printf("%s＼n", mysql_error(&mysql));
                exit(1) ;
            }


        }

    }
    mysql_close(&mysql) ;
}
void DB::load(int select = 0)
{
    MYSQL mysql;
    MYSQL_RES* res ;
    std::string query = "SELECT * FROM timelog";
    std::string query1 = "SELECT * FROM information";
    MYSQL_ROW row ;
    mysql_init(&mysql) ;
    int field;
    if(!mysql_real_connect(&mysql, NULL,ID,passwd,"student_data",3306, (char *)NULL, 0))
    {
        printf("%s＼n",mysql_error(&mysql));
        exit(1) ;
    }
    printf("connect...\n") ;
    if(mysql_query(&mysql, "USE student_data") )
    {
        printf("%s＼n", mysql_error(&mysql));
        exit(1) ;

    }

    if(select == 1)
    {
        if(mysql_query(&mysql,query1.c_str()))
        {
            printf("%s＼n", mysql_error(&mysql));
            exit(1) ;

        }
        res = mysql_store_result( &mysql );
        field = mysql_num_fields(res);
        int i = 0;
        while( ( row = mysql_fetch_row( res ) ))
        {
            for(int cnt = 0 ; cnt < field ; ++cnt)
            {

                if(cnt == 0)
                    stu[i].name = row[cnt];
                else if (cnt == 1)
                {
                    stu[i].DB_mac = row[cnt];
                    stu[i].mac = HW(stu[i].DB_mac);
                    ssids_type::iterator it = ssids.find(stu[i].mac);
                    if(it == ssids.end())
                    {
                        try
                        {
                            ssids.insert(stu[i].mac);
                            cout<<"save"<<endl;
                        }
                        catch(runtime_error&) {
                        }
                    }
                }
            }
            i++;
        }
        size = i;
    }
    else if(select != 1)
    {
        if(mysql_query(&mysql,query.c_str()))
        {
            printf("%s＼n", mysql_error(&mysql));
            exit(1) ;

        }
        res = mysql_store_result( &mysql );
        field = mysql_num_fields(res);

        while( ( row = mysql_fetch_row( res ) ))
        {
            for(int cnt = 0 ; cnt < field ; ++cnt)
                printf("%5s",row[cnt]);
            printf("\n");
        }

    }
    mysql_free_result( res ) ;
    mysql_close(&mysql) ;
}
DB data;
class clear_section
{
public :
    friend class DB;
    void time_setting();
    void log_section();
};
class stu_info
{
public :
    friend class DB;
    bool save_info();
    void time_log();
private:
};
bool stu_info::save_info()
{
    int i ;
    if(size == 0)
    {
            cout<<"input number students(max = 500) :";
            cin>>size;
            if(cin.fail())
            {
                cout<<"wrong Number Retry!!"<<endl;
                cin.clear();
                cin.ignore();
                return save_info();
            }

            for(i = 0 ; i < size ; i++)
            {
                cout<<i+1<<"input student name :";
                cin>>stu[i].name;
                cout<<i+1<<"input student mac address :";
                cin>>stu[i].DB_mac;
                stu[i].mac= HW(stu[i].DB_mac);
                ssids_type::iterator it = ssids.find(stu[i].mac);
                if(it == ssids.end()){
                    try{
                        ssids.insert(stu[i].mac);
                        cout<<"save"<<endl;
                    }
                    catch(runtime_error&){
                        return false;
                    }
                }
            }
            data.insertdata(1);

        }

        else if(size != 0)
        {
            cout<<"add student"<<endl;
            cout<<"input student name :";
            cin>>stu[size].name;
            cout<<"input student mac address :";
            cin>>stu[size].DB_mac;
            size++;
            stu[size].mac= HW(stu[size].DB_mac);
            ssids_type::iterator it = ssids.find(stu[size].mac);
            if(it == ssids.end()){
                try{
                    ssids.insert(stu[size].mac);
                    cout<<"save"<<endl;
                }
                catch(runtime_error&) {
                    return false;
                }

            }
                data.insertdata(2);
        }

    return true;
}
void stu_info::time_log()
{
    int i,count = 0;
    cout << curr_tm->tm_year + 1900 << " year " << curr_tm->tm_mon + 1 << " month " << curr_tm->tm_mday << " day ";
    cout << curr_tm->tm_hour << " hour " << curr_tm->tm_min << " min "<< endl;
    for(i = 0 ; i < size ; i++)
    {
        try
        {
            attendance::iterator it = atten.find(stu[i].name);
            if(it != atten.end())
            {
                cout<<"name :"<<stu[i].name<<"   mac : "<<stu[i].mac<<"   attendance"<<endl;
                count++;
            }
            else if(it == atten.end())
                cout<<"name :"<<stu[i].name<<"   mac : "<<"  no attendance"<<endl;
        }
        catch(runtime_error&)
        {
        }
    }
    cout<<"courrent_student : "<<count<<endl;
}
//probe request packet capturing
class probeSniffer {
public:
    void running(const string& iface);

private:
    bool call(PDU& pdu);
};
void probeSniffer::running(const std::string& iface) {
    SnifferConfiguration config;
    config.set_promisc_mode(true);
    config.set_rfmon(true);
    Sniffer sniffer(iface, config);
    sniffer.sniff_loop(make_sniffer_handler(this, &probeSniffer::call));
}
bool probeSniffer::call(PDU& pdu) {
    const Dot11ProbeRequest& probe = pdu.rfind_pdu<Dot11ProbeRequest>();
    address_type addr = probe.addr2(); //802.11 header second mac address sniffing
    ssids_type::iterator it = ssids.find(addr);
    int i;
    if (it != ssids.end()) {
        try {
            for(i = 0; i<size ; i++)
            {
                if(addr == stu[i].mac)
                {
                    atten.insert(stu[i].name);
                    stu[i].check = "0";
                }
            }
        }
        catch (runtime_error&) {
        }
    }
    return true;
}
//reset func
int ts;
void reset()
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
            sleep(1);
            data.insertdata(2);
            atten.clear();
        }
        if(ts == 30 && curr_tm->tm_min % 30 == 0 && curr_tm->tm_sec == 0)
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
        if(ts == 60 && curr_tm->tm_min == 0 && curr_tm->tm_sec == 0)
        {
            t.year = std::to_string(curr_tm->tm_year + 1900);
            t.month = std::to_string(curr_tm->tm_mon +1);
            t.day = std::to_string(curr_tm->tm_mday);
            t.hour = std::to_string(curr_tm->tm_hour);
            t.minute = std::to_string(curr_tm->tm_min);
            sleep(1);
            data.insertdata(3);
            atten.clear();
        }
    }
}
#endif // PROJECT_H
