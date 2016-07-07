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
typedef Dot11::address_type address_type; //mac address
typedef set<address_type> ssids_type; //ssid
typedef set<string> attendance;
ssids_type ssids;
attendance atten;
string interface;
string address; //mac address
//curr_time setting
time_t curr_time;
struct tm *curr_tm;
struct student
{
    string name;
    address_type mac;
    string DB_mac;
    string check;

};
struct _time
{

    string year = std::to_string(curr_tm->tm_year + 1900);
    string month = std::to_string(curr_tm->tm_mon +1);
    string day = std::to_string(curr_tm->tm_mday);
    string hour = std::to_string(curr_tm->tm_hour);
    string minute = std::to_string(curr_tm->tm_min);
};
int size = 3;
student stu[500];
class DB
{
public :
    _time t;
    void insertdata(int i, int select);
    void print(int i);
};
void DB::insertdata(int i,int select)
{
    MYSQL mysql;
    std::string query0 = "SELECT * FROM information WHERE mac_address = '"+stu[i].DB_mac+"'";
    std::string query1 = "INSERT INTO information(name,mac_address) VALUES ('"+stu[i].name+"','"+stu[i].DB_mac+"')";
    std::string query2 = "INSERT INTO timelog(year,month,day,hour,minute,name,mac,attendance) VALUES ('"+t.year+"',"+t.month+"','"+t.day+"','"+t.hour+"',"+t.minute+"','"+stu[i].name+"','"+address+"','"+stu[i].check+"')";
    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql, NULL, "root","123","student_data",3306, (char *)NULL, 0))
    {
        printf("%s＼n",mysql_error(&mysql));
        exit(1) ;
    }
    printf("connect.....\n") ;

    if(mysql_query(&mysql, "USE student_data") )
    {
        printf("%s＼n", mysql_error(&mysql));
        exit(1) ;

    }
    switch(select)
    {
    case 1 :
        //if(!mysql_query(&mysql,query0.c_str()))
        //{
            if(mysql_query(&mysql,query1.c_str()))
            {
                printf("%s＼n", mysql_error(&mysql));
                exit(1) ;
            }

        //}
        break;
    case 2 :
        if(mysql_query(&mysql,query2.c_str()))
        {
            printf("%s＼n", mysql_error(&mysql));
            exit(1) ;
        }
        break;
    }
    mysql_close(&mysql) ;
}
void DB::print(int i)
{
    MYSQL mysql ;
    MYSQL_RES* res ;
    std::string query0 = "SELECT * FROM information";
    std::string query1 = "SELECT * FROM timelog";
    MYSQL_ROW row ;
    mysql_init(&mysql) ;
    int field;
    if(!mysql_real_connect(&mysql, NULL, "root","123","student_data",3306, (char *)NULL, 0))
    {
        printf("%s＼n",mysql_error(&mysql));
        exit(1) ;
    }
    printf("성공적으로 연결되었습니다.\n") ;
    if(mysql_query(&mysql, "USE student_data") )
    {
        printf("%s＼n", mysql_error(&mysql));
        exit(1) ;

    }
    switch(i)
    {
    case 1 :
        if(mysql_query(&mysql,query0.c_str()))
        {
            printf("%s＼n", mysql_error(&mysql));
            exit(1) ;

        }
        break;
    case 2 :
        if(mysql_query(&mysql,query1.c_str()))
        {
            printf("%s＼n", mysql_error(&mysql));
            exit(1) ;
        }
        break;
    }

    res = mysql_store_result( &mysql );
    field = mysql_num_fields(res);

    while( ( row = mysql_fetch_row( res ) ))
    {
        for(int cnt = 0 ; cnt < field ; ++cnt)
            printf("%12s",row[cnt]);
        printf("\n");
    }

    mysql_free_result( res ) ;
    mysql_close(&mysql) ;
}

class clear_section
{
public :
    friend class DB;
    void time_setting();
    void log_section();
};



void clear_section::log_section()
{
    DB data;
    time_setting();
    if(curr_tm->tm_min / 5 == 0 && curr_tm->tm_sec == 0)
    {

        for(int i = 0 ; i < size ; i++)
            data.insertdata(i,2); // information save to database
        atten.clear();//use thread
    }
}
void clear_section::time_setting()
{
    curr_time = time(NULL);
    curr_tm = localtime(&curr_time);
}

class stu_info
{
public :
    friend class DB;
    typedef HWAddress<6> HW;
    bool save_info(); //in order saving
    void time_log();
private:
};
bool stu_info::save_info()
{
    int i ;
    cout<<"input number students :"<<endl;
    cin>>size;
    for(i = 0 ; i < size ; i++)
    {
        cout<<"input student name :";
        cin>>stu[i].name;
        cout<<"input student mac address :";
        cin>>address;
        stu[i].DB_mac = address;
        stu[i].mac = HW(address);
        ssids_type::iterator it = ssids.find(stu[i].mac);
        if(it == ssids.end()){
            try{
                ssids.insert(stu[i].mac);
                cout<<"save"<<endl;
            }
            catch(runtime_error&) {
                return false;
            }
        }
    }
    return true;
}

void stu_info::time_log()
{
    int i,count = 0;
    clear_section time;
    time.time_setting();
    cout << curr_tm->tm_year + 1900 << " year " << curr_tm->tm_mon + 1 << " month " << curr_tm->tm_mday << " day ";
    cout << curr_tm->tm_hour << " hour " << curr_tm->tm_min << " min "<< endl;
    for(i = 0 ; i < size ; i++)
    {
        try
        {
            attendance::iterator it = atten.find(stu[i].name);
            if(it != atten.end())
            {
                cout<<"name :"<<stu[i].name<<"   mac :"<<stu[i].mac<<"   attendance"<<endl;
                count++;
            }
            else if(it == atten.end())
                cout<<"name :"<<stu[i].name<<"   mac :"<<"  no attendance"<<endl;
        }
        catch(runtime_error&)
        {
            //No ssid, just ignore it.
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
                    atten.insert(stu[i].name);
            }

        }
        catch (runtime_error&) {
        }

    }
    return true;
}

#endif // PROJECT_H
