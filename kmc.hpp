#pragma once

#include "box.hpp"
#include<set>
#include<random>
#include "struct_template.hpp"
#include<armadillo>
#include "match.hpp"

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
    //int init_box_site();//初始化格点，包括添加所有格点，更新嵌入对应
    int add_frame_embeding_of_site();
    int update_site_event(int siteid);//更新site的事件
    double getrate(int siteid,int frame_id,int eventid);//得到事件速率
    int add_all_event_of_site(int siteid);//对没有事件的site添加event
    int add_site_frame(int site_id,int frame_id);//对某个site,添加所有该frame的embed
    int init_all_embeding();
    bool is_match_events(int siteid,int embedid,int eventid);//某个格点的某个嵌入是否匹配某个事件（相对于frame)
    int delete_site_event(int site_id);//删除某个site的所有事件
    int update_area_with_site(int siteid);//更新某个area
    int perform_with_event(int change_event);//执行某个事件
    int init_all_event();//初始化所有的事件
    int run_one_step();//完整的运行一步
    int init();
    int run_N(int N);
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
    frame.push_back(*(read_file_to_temp("template_example.dat")));
    frameNumber=1;
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
    return 0;
}

int kmc::perform_with_event(int change_event){
    event *nevent=event_storage+change_event;
    site *nsite=lattice->sitelist+nevent->event_site;
    int frameid=nevent->frame_index;
    int embedid=nevent->embed_index;
    int frame_event_index=nevent->frame_e_index;
    for (int i = 0; i < nsite->embed_list[embedid].size(); i++)
    {
        if (frame[frameid].end_state[frame_event_index][i] != -1){
            lattice->sitelist[nsite->embed_list[embedid][i]].state=frame[frameid].end_state[frame_event_index][i];
        }
    }
    return 0;   
}

int kmc::init_all_event(){
    for (auto i: lattice->mysites){
        update_site_event(i);
    }
    return 0;
}

int kmc::run_one_step(){
    double dt;
    int eventid;
    eventid=choose_event(&dt);
    perform_with_event(eventid);
    update_area_with_site(event_storage[eventid].event_site);
    t += dt;
    return eventid;
}

int kmc::init(){
    init_all_event();
    return 0;
}

int kmc::run_N(int N){
    for (int i = 0; i < N; i++)
    {
        run_one_step();
    }
    return 0;
}

double length_vector(double *a){
    return sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
}

double inner_product(double *a,double *b){
    return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}


int kmc::add_site_frame(int site_id,int frame_id){
    struct_template *nf=&(frame[frame_id]);
    double error1=0.1,error2;
    double *a=nf->nodes[(nf->nodes[0].neighboors[0])].position;
    double *b=nf->nodes[(nf->nodes[0].neighboors[1])].position;
    double a2[3],b2[3];
    double M[9];//存储变换矩阵
    int *embede_list=new int[nf->noden];
    site *nowsite = lattice->sitelist+site_id;
    if (nowsite->type != nf->nodes[0].type){return 0;}//site类型不匹配
    for (int i = 0; i < nowsite->neighbors.size(); i++)
    {
        for (int j = 0; j < nowsite->neighbors.size(); j++)
        {
            if(i!= j){
                //类型不匹配
                if(lattice->sitelist[nowsite->neighbors[i]].type != nf->nodes[(nf->nodes[0].neighboors[0])].type || lattice->sitelist[nowsite->neighbors[j]].type != nf->nodes[(nf->nodes[0].neighboors[1])].type){
                    return 0;
                }

                a2[0]=lattice->sitelist[nowsite->neighbors[i]].position[0]-nowsite->position[0];
                a2[1]=lattice->sitelist[nowsite->neighbors[i]].position[1]-nowsite->position[1];
                a2[2]=lattice->sitelist[nowsite->neighbors[i]].position[2]-nowsite->position[2];
                b2[0]=lattice->sitelist[nowsite->neighbors[j]].position[0]-nowsite->position[0];
                b2[1]=lattice->sitelist[nowsite->neighbors[j]].position[1]-nowsite->position[1];
                b2[2]=lattice->sitelist[nowsite->neighbors[j]].position[2]-nowsite->position[2];
            
                if (fabs(length_vector(a)-length_vector(a2))<error1 && fabs(length_vector(b)-length_vector(b2))<error1 && fabs(inner_product(a,b)-inner_product(a2,b2))<error1){
                    generate_rotation(a2,b2,a,b,M);
                    //由变换矩阵得到嵌入的算法
                    for (int kkk = 0; kkk < nf->noden; kkk++)
                    {
                        embede_list[kkk] = -1;
                    }
                    embede_list[0]=nowsite->siteid;
                    embede_list[nf->nodes[0].neighboors[0]] = nowsite->neighbors[i];
                    embede_list[nf->nodes[0].neighboors[1]] = nowsite->neighbors[j];
                    for(int num_of_nodes=nf->noden-3;num_of_nodes > 0;){
                        for (int index_fnbode = 0; index_fnbode < nf->noden; index_fnbode++)
                        {
                            if(embede_list[index_fnbode]<0){
                                for(int index=0;index<nf->nodes[index_fnbode].neighNumber;index++){
                                    if(embede_list[nf->nodes[index_fnbode].neighboors[index]]>=0){
                                        site *nb_site=lattice->sitelist+embede_list[nf->nodes[index_fnbode].neighboors[index]];
                                        for (int jji = 0; jji < nb_site->neighbors.size(); jji++)
                                        {
                                            if(lattice->sitelist[nb_site->neighbors[jji]].type == nf->nodes[index_fnbode].type && ismatch(M,lattice->sitelist[nb_site->neighbors[jji]].position,nowsite->position,nf->nodes[index_fnbode].position,error1)){
                                                embede_list[index_fnbode]=lattice->sitelist[nb_site->neighbors[jji]].siteid;
                                                num_of_nodes --;
                                                continue;
                                            }
                                        
                                        }
                                        if(embede_list[index_fnbode]<0){

                                           goto jump;//不匹配
                                        }
                                    }
                                }
                            }
                        }
                        
                    }
                    
                    //frame嵌入添加到site
                    nowsite->embed_framework_id.push_back(frame_id);
                    nowsite->embed_list.push_back(std::vector<int>());
                    for (int ipi = 0; ipi < nf->noden; ipi++)
                    {
                        nowsite->embed_list.back().push_back(embede_list[ipi]);
                    }
                    
                }
            }

jump:
embede_list[0]=-1;//没有这一句不知道为什么不能编译
        }
    }
    delete [] embede_list;
    return 0;
}

int kmc::init_all_embeding(){
    for(auto i:lattice->mysites){
        for (int j = 0; j < frameNumber; j++)
        {
            add_site_frame(i,j);
        }
        
    }
    return 0;
}