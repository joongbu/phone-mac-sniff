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
struct student
{
    string name;
    address_type mac;
    string DB_mac;
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
int size = 3;
student stu[500];
class DB
{
public :

    void insertdata(int select);
    void load(int select);
};
void DB::insertdata(int select)
{
    int i=0;
    MYSQL mysql;
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
        exit(1);
    }
    for(i = 0 ; i <size ; i++)
    {
        std::string query = "INSERT INTO timelog(year,mon,day,hour,minute,name,mac_addr,attendance) VALUES ('"+t.year+"','"+t.month+"','"+t.day+"','"+t.hour+"','"+t.minute+"','"+stu[i].name+"','"+stu[i].DB_mac+"','"+stu[i].check+"')";
        std::string query1 = "INSERT INTO information(name,mac_address) VALUES ('"+stu[i].name+"','"+stu[i].DB_mac+"')";
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
            if(mysql_query(&mysql,query.c_str()))
            {
                printf("%s＼n", mysql_error(&mysql));
                exit(1) ;
            }


        }
    }
    mysql_close(&mysql) ;
}
void DB::load(int select)
{
    MYSQL mysql ;
    MYSQL_RES* res ;
    std::string query = "SELECT * FROM timelog";
    std::string query1 = "SELECT * FROM information";
    MYSQL_ROW row ;
    mysql_init(&mysql) ;
    int field;
    if(!mysql_real_connect(&mysql, NULL, "root","123","student_data",3306, (char *)NULL, 0))
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

    if(select == 0)
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
                printf("%5s",row[0]);
            printf("\n");
        }

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
        cout<<i+1<<"input student name :";
        cin>>stu[i].name;
        cout<<i+1<<"input student mac address :";
        cin>>stu[i].DB_mac;
        stu[i].mac = HW(stu[i].DB_mac);
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

#endif // PROJECT_H
