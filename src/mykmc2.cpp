#include "kmc.hpp"
#include<glog/logging.h>
#define SEED 33
extern "C"{
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

class mykmc:public kmc{
private:
    lua_State *L;

public:
    int Nsteps = 0;
    int Num_site1=0;
    mykmc();
    double center[3];
    void update_center();
    int change_state() override;
    int getint_lua(const char *name);
    int lua_lattice_state(double x,double y, double z,int type);
    double lua_get_rate(double x,double y, double z,int frameid,int eventid,double cx,double cy, double cz);
    double getrate(int siteid,int frame_id,int eventid) override;
    double getConcentration(int siteid,int frame_id,int eventid);//获取事件对应点的浓度
    int diffusion_all(double t);//整体浓度扩散
    int init_concetration();//初始化浓度
    int add_all_event_of_site(int siteid) override;
    int perform_with_event(int change_event) override;
    int run_one_step() override;
    int init() override;
    int update_rate_after_concentration_changed();//在浓度改变后更新事件速率
    ~mykmc();
};

mykmc::mykmc():kmc(SEED){
    L=luaL_newstate();
    luaL_openlibs(L);
    luaL_dofile(L,"my.lua");
    lua_getglobal(L,"runsteps");
    Nsteps=(int)lua_tointeger(L,-1);
    lua_pop(L,1);
    //添加附加数据描述浓度//另一个用于扩散过程存储数据
    for (int i = 0; i < lattice->MaxSite; i++)
    {
        lattice->sitelist[i].add_data(2);
    }
    // struct_template *dftemp=read_file_to_temp("diffusion.dat");
    // frame.push_back(*(dftemp));
    // frameNumber +=1;
}



int mykmc::lua_lattice_state(double x,double y, double z,int type){
    lua_getglobal(L,"state");
    lua_pushnumber(L,x);
    lua_pushnumber(L,y);
    lua_pushnumber(L,z);
    lua_pushnumber(L,type);
    lua_call(L,4,1);
    int state=(int)lua_tointeger(L,-1);
    lua_pop(L,1);
    return state;
}

double mykmc::lua_get_rate(double x,double y, double z,int frameid,int eventid,double cx,double cy, double cz){
    lua_getglobal(L,"getrate");
    lua_pushnumber(L,x);
    lua_pushnumber(L,y);
    lua_pushnumber(L,z);
    lua_pushnumber(L,frameid);
    lua_pushnumber(L,eventid);
    lua_pushnumber(L,cx);
    lua_pushnumber(L,cy);
    lua_pushnumber(L,cz);
    lua_call(L,8,1);
    double state=(double)lua_tonumber(L,-1);
    lua_pop(L,1);
    return state;
}

double mykmc::getConcentration(int siteid,int frame_id,int eventid){
    site &ns=lattice->sitelist[siteid];
    event &ne=event_storage[eventid];
    int chg_site[14] = {28,21,20,21,20,21,20,0,20,21,21,21,0,0};//定义事件中改变的site
    if (frame_id == 0){
        int change_site_id = ns.embed_list[ne.embed_index][chg_site[ne.frame_e_index]];
        return lattice->sitelist[change_site_id].data[0];
    }else
    {//frame_id==1,不受浓度影响
        return 1;
    }
    return 1;
}

/*
获取事件速率，暂时不考虑浓度影响
*/
double mykmc::getrate(int siteid,int frame_id,int eventid){
    site &ns=lattice->sitelist[siteid];
    event &ne=event_storage[eventid];
    double rate[]={0.5,0.4,0.4,0.4,0.4,0.6,0.5,1.3,0.55,0.55,0.5,0.65,0.5,0.3};
    //int chg_site[14] = {28,21,20,21,20,21,20,0,20,21,21,21,0,0};//定义事件中改变的site
    // if (frame_id == 0){
        //int change_site_id = ns.embed_list[ne.embed_index][chg_site[ne.frame_e_index]];
        //return lua_get_rate(ns.position[0],ns.position[1],ns.position[2],frame_id,ne.frame_e_index,center[0],center[1],center[2]);//*lattice->sitelist[change_site_id].data[0];
    // }else
    // {//沉积事件速率
    //     return 0.1;//TODO:需要改变速率
    // }
    return exp(-rate[ne.frame_e_index]*10);
}

void mykmc::update_center(){
    double cen[]={0,0,0};
    int num = 0;
    for(auto i :lattice->mysites){
        if(lattice->sitelist[i].state != 0){
            num ++;
            cen[0] += lattice->sitelist[i].position[0];
            cen[1] += lattice->sitelist[i].position[1];
            cen[2] += lattice->sitelist[i].position[2];
        }
    }
    center[0]=cen[0]/num;
    center[1]=cen[1]/num;
    center[2]=cen[2]/num;
}

int mykmc::update_rate_after_concentration_changed(){
    for(auto ei:using_events){
        event &nowe=event_storage[ei];
        if(nowe.frame_index =0 ){
        nowe.rate=nowe.primary_rate*getConcentration(nowe.event_site,nowe.frame_index,ei);}
    }
    return 0;
}

int mykmc::diffusion_all(double t){//TODO
    int nbnum;
    double nbconcsum;
    double k=1e5;
    for(auto i:lattice->mysites){
        if(lattice->sitelist[i].state != 0){continue;}
        nbnum=0;
        nbconcsum=0;
        for(auto j:lattice->sitelist[i].neighbors){
            if (lattice->sitelist[j].state == 0){
                nbnum ++;
                nbconcsum += lattice->sitelist[j].data[0];
            }
        }
        lattice->sitelist[i].data[1] = lattice->sitelist[i].data[0]-0.001*(nbnum*lattice->sitelist[i].data[0]-nbconcsum);
    }
    for(auto i:lattice->mysites){
        if(lattice->sitelist[i].state != 0){continue;}
        lattice->sitelist[i].data[0] = lattice->sitelist[i].data[1];
    }
    return 0;
}

int mykmc::perform_with_event(int change_event){
    if (event_storage[change_event].type==-1&&event_storage[change_event].frame_index==0){
        perform_with_frame_event(change_event);
        //生长之后应该改变扩散
        update_events_after_perform(change_event);
        Num_site1 ++;
    }else if(event_storage[change_event].type==1){//扩散事件
        lattice->sitelist[event_storage[change_event].event_site].data[0] +=30;
    }//else if(event_storage[change_event].type ==-1&&event_storage[change_event].frame_index == 1) //沉积沉积
    // {
    //     lattice->sitelist[event_storage[change_event].event_site].data[0] += 1;//增加浓度
    //     update_events_after_perform(change_event);
    // }    
    return 0;
}

int mykmc::run_one_step(){
    double dt;
    int eventid;
    if (using_events.size()==0){std::cout<<"empty event set"<<std::endl;}
    eventid=choose_event(&dt);
    LOG(INFO)<<"当前时间"<<t<<"事件id："<<eventid<<"耗时："<<dt<<"事件类型:"<<event_storage[eventid].type<<"总事件个数："<<using_events.size()
            <<"event type id:"<<event_storage[eventid].frame_e_index<<"event site:"<<event_storage[eventid].event_site<<"事件点浓度："<<lattice->sitelist[event_storage[eventid].event_site].data[0]
            <<"site position:"<<lattice->sitelist[event_storage[eventid].event_site].position[0]<<"  "<<lattice->sitelist[event_storage[eventid].event_site].position[1]<<"  "<<lattice->sitelist[event_storage[eventid].event_site].position[2];
    perform_with_event(eventid);//包含执行后处理
    //根据dt扩散多少//生长吞掉单个点的浓度//生长后的点不参与扩散
    diffusion_all(dt);
    //重新确认所有速率
    update_rate_after_concentration_changed();
    t += dt;
    return eventid;
}

int mykmc::change_state(){
    for(auto i : lattice->mysites){
        site &now=lattice->sitelist[i];
        now.state=lua_lattice_state(now.position[0],now.position[1],now.position[2],now.type);
        if(now.state == 1){
            Num_site1 ++;
        }
    }
    return 1;
}

//一般更新事件只要重载这个就能适合大多数情况
int mykmc::add_all_event_of_site(int siteid){
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
                now_event->primary_rate=getrate(siteid,frameid,now_event->id);
                now_event->rate=now_event->primary_rate*getConcentration(siteid,frameid,now_event->id);
                now_event->type=-1;//表示为frame定义的类型
                num_e ++;
                nsite->site_events.push_back(now_event->id);
            }
        }
    }
    //添加沉积事件
    if(nsite->state==0){
        now_event=add_event();
        now_event->event_site=siteid;
        now_event->type=1;
        now_event->rate=exp(-0.5*10)*200/18000000;
        nsite->site_events.push_back(now_event->id);
    }
    return num_e;
}

int mykmc::init_concetration(){
    for(auto i:lattice->mysites){
        lattice->sitelist[i].data[0] = 0.01;
    }
    return 0;
}

int mykmc::init(){
    init_all_embeding();
    change_state();
    init_concetration();
    init_all_event();
    // event &kuosan = *add_event();
    // kuosan.type=1;
    // kuosan.rate=(lattice->mysites.size()-Num_site1)*0.1;
    return 0;
}

int main(int argc,char **argv){
    google::InitGoogleLogging(argv[0]);
    mykmc &aa=*(new mykmc());
    // kmc &aa=*(new kmc(12));
    std::fstream fp;
    fp.open("a.xyz",std::ios::out);
    // //aa.lattice->add_site(1,1,1);
    LOG(INFO)<<"总site数："<<aa.lattice->Number;
    // //aa.add_site_frame(604,0);
    LOG(INFO)<<"INIT START";
    aa.init();
    LOG(INFO)<<"INIT END";
    //aa.run_N(aa.Nsteps);
    LOG(INFO)<<"运行步数："<<aa.Nsteps;
    
    for(int i=0;i<aa.Nsteps;i++){
        LOG(INFO)<<"第"<<i<<"步 start";
        aa.run_one_step();
        // if (i%200==0){
        // LOG(INFO)<<"输出第"<<i<<"状态，"<<"原子数："<<aa.Num_site1;
        // fp<<aa.lattice->out_to_xyz(1);
        // fp<<"\n";
        // }
        if(aa.Num_site1>=8000){
            break;
        }
    }
    fp<<aa.lattice->out_to_xyz(1);
    fp.close();
    return 0;
}