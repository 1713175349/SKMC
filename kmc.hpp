#pragma once

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
    std::mt19937 gen; //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<double> dis;
    event *event_storage;//存储事件的列表，避免频繁分配内存
    std::set<int> using_events;
    std::set<int> free_events;

    std::vector<struct_template> frame;//结构架构
    int frameNumber;

    int MaxEventNumber;
    double t=0;//time
    kmc(int seed);
    event *add_event();//添加一个事件
    int delete_event(int id);
    int choose_event(double *dt);
    int init_box_site();//初始化格点，包括添加所有格点，更新嵌入对应
    int update_site_event(int siteid);//更新site的事件
    double getrate(int siteid,int frame_id,int eventid);//得到事件速率
    int add_all_event_of_site(int siteid);//对没有事件的site添加event
    //int add_site_frame(int site_id,int frame_id);
    bool is_match_events(int siteid,int embedid,int eventid);//某个格点的某个嵌入是否匹配某个事件（相对于frame)
    int delete_site_event(int site_id);//删除某个site的所有事件
    int update_area_with_site(int siteid);//更新某个area
    int perform_with_event(int change_event);//执行某个事件
    
    ~kmc();
};

kmc::kmc(int seed)
{
    this->gen = std::mt19937(seed);
    this->dis = std::uniform_real_distribution<double>(0,1);
    MaxEventNumber=100000;
    //lattice =new box(1.0,1.0,1.0,100,100,10,100000);
    lattice = new box("myboxlattice.dat");
    event_storage = new event[MaxEventNumber];
    for (int i = 0; i < MaxEventNumber; i++)
    {
        event_storage[i].id = i;//很重要
        free_events.insert(i);
    }
}

kmc::~kmc()
{
    delete [] event_storage;
}

event *kmc::add_event(){
    int nid = *free_events.begin();
    free_events.erase(free_events.begin());
    using_events.insert(nid);
    return event_storage+nid;
}

int kmc::delete_event(int id){
    using_events.erase(id);
    free_events.insert(id);
    return id;
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


bool kmc::is_match_events(int siteid,int embedid,int eventid){
    site *nsite;
    nsite=lattice->sitelist+siteid;
    int frameid=nsite->embed_framework_id[embedid];
    int *state_list=frame[frameid].start_state[eventid];
    //nsite->embed_list[embedid]
    for (int i = 0; i < frame[frameid].noden; i++)
    {
       if ((lattice->sitelist[nsite->embed_list[embedid][i]].state != state_list[i])&&(state_list[i] != -1)){
           return false;
       }
    }
    return true;
}

int kmc::delete_site_event(int siteid){
    site *nsite=lattice->sitelist+siteid;
    for (auto i : nsite->site_events){
        delete_event(i);
    }
    nsite->site_events.clear();
    return 0;
}

int kmc::add_all_event_of_site(int siteid){
    site *nsite=lattice->sitelist+siteid;
    event *now_event;
    struct_template *nframe;
    int frameid;
    int num_e=0;
    for (int i = 0; i < nsite->embed_framework_id.size(); i++)
    {   
        frameid = nsite->embed_framework_id[i];
        nframe = &(frame[frameid]);
        for (int k = 0; k < nframe->eventn; k++)
        {
            if (is_match_events(siteid,i,k)){
                now_event = add_event();
                now_event->event_site=siteid;
                now_event->embed_index=i;
                now_event->frame_index=frameid;
                now_event->frame_e_index=k;
                now_event->rate=getrate(siteid,frameid,k);
                num_e ++;
                nsite->site_events.push_back(now_event->id);
            }
        }
    }
    return num_e;
}

int kmc::update_site_event(int siteid){
    delete_site_event(siteid);
    return add_all_event_of_site(siteid);
}

double kmc::getrate(int siteid,int frame_id,int eventid){
    return 0.1;
}

int kmc::update_area_with_site(int siteid){
    site *nsite=lattice->sitelist+siteid;
    int areaid[3];
    areaid[0]=nsite->areaid[0];
    areaid[1]=nsite->areaid[1];
    areaid[2]=nsite->areaid[2];
    int N=1;//更新领域范围
    int ixa,iya,iza;
    for (int i = -N; i <(N+1); i++)
    {
        for (int j = -N; j < (N+1); j++)
        {
            for (int k = -N; k < (N+1); k++)
            {
                ixa=areaid[0]+i;
                iya=areaid[1]+j;
                iza=areaid[2]+k;
                if (ixa>=0&&iya>=0&&iza>=0){
                    for (auto hg : lattice->areas[ixa][iya][iza])
                    {
                        update_site_event(hg);
                    }
                    
                }
            }
            
        }
        
    }
}

int kmc::perform_with_event(int change_event){
    event *nevent=event_storage[change_event];
    site *nsite=lattice->sitelist[nevent->event_site];
    int frameid=nevent->frame_index;
    int embedid=nevent->embed_index;
    int frame_event_index=nevent->frame_e_index;
    for (int i = 0; i < nsite->embed_list[embedid].size(); i++)
    {
        if (frame[frameid].end_state[i] != -1){
            lattice->sitelist[nsite->embed_list[embedid][i]]=frame[frameid].end_state[i];
        }
    }
    return 0;   
}