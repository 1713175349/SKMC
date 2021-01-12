#include "box.hpp"
#include<set>
class kmc
{
private:
    /* data */
public:
    box *lattice;//格子
    event *event_storage;//存储事件的列表，避免频繁分配内存
    std::set<int> using_events;
    std::set<int> free_events;
    int MaxEventNumber;
    double t=0;//事件
    kmc(/* args */);
    event *add_event();//添加一个事件
    int delete_event(int id);
    int choose_event();
    ~kmc();
};

kmc::kmc()
{
    MaxEventNumber=100000
    lattice =new box(1.0,1.0,1.0,100,100,10,100000);
    event_storage = new event[MaxEventNumber];
    for (int i = 0; i < MaxEventNumber; i++)
    {
        event_storage.id = i;
        free_events.insert(i);
    }
}

kmc::~kmc()
{
    delete [] event_storage;
}

event *kmc::add_event(){
    int nid = *free_events.begin()
    free_events.erase(free_events.begin());
    using_events.insert(nid);
    return event_storage+nid;
}

int kmc::delete_event(int id){
    using_events.erase(id);
    free_events.insert(id);
}

int kmc::choose_event(){
    std::vector<int> ev(using_events.begin(),using_events.end());
    int len=ev.size();
    double rate_list[ev.size()];
    rate_list[0] = event_storage[ev[0]].rate;
    for (int i = i; i < len; i++)
    {
        rate_list[i] = rate_list[i-1]+event_storage[ev[i]].rate;
    }
    
}