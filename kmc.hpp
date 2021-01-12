#include "box.hpp"
#include<set>
#include<random>
#include "struct_template.hpp"
class kmc
{
private:
    /* data */
public:
    box *lattice;//格子
    std::random_device rd;  //获取随机数种子
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<double> dis(0, 1);
    event *event_storage;//存储事件的列表，避免频繁分配内存
    std::set<int> using_events;
    std::set<int> free_events;

    std::vector<struct_template> frame;//结构架构
    int frameNumber;

    int MaxEventNumber;
    double t=0;//事件
    kmc(/* args */);
    event *add_event();//添加一个事件
    int delete_event(int id);
    int choose_event();
    int 
    int init_box_site();//初始化格点，包括添加所有格点，更新嵌入对应
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

int kmc::choose_event(double *dt){
    std::vector<int> ev(using_events.begin(),using_events.end());
    int len=ev.size();
    double rate_list[ev.size()];
    rate_list[0] = event_storage[ev[0]].rate;
    for (int i = 0; i < len; i++)
    {
        rate_list[i] = rate_list[i-1]+event_storage[ev[i]].rate;
    }
    int a=0,b=len-1,middle;
    double r=dis(gen)*rate_list[len-1];
    *dt=log(r/rate_list[len-1])/rate_list[len-1];
    if (r<=rate_list[0]){
        return ev[0];
    }
    while ((b-a)>1)
    {
        middle=(a+b)/2;
        if (r<=rate_list[middle]){
            b=middle;
        }
        else
        {
            a=middle;
        }
    }
    return ev[b];
}