#include "kmc.hpp"
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
    int add_all_event_of_site(int siteid) override;
    int perform_with_event(int change_event) override;
    int run_one_step() override;
    int init() override;
    ~mykmc();
};

mykmc::mykmc():kmc(12){
    L=luaL_newstate();
    luaL_openlibs(L);
    luaL_dofile(L,"my.lua");
    lua_getglobal(L,"runsteps");
    Nsteps=(int)lua_tointeger(L,-1);
    lua_pop(L,1);
    //添加附加数据描述浓度
    for (int i = 0; i < lattice->MaxSite; i++)
    {
        lattice->sitelist[i].add_data(1);
    }
    //frame.push_back(*(read_file_to_temp("diffusion.dat")));
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

double mykmc::getrate(int siteid,int frame_id,int eventid){
    site &ns=lattice->sitelist[siteid];
    event &ne=event_storage[eventid];
    int chg_site[14] = {28,21,20,21,20,21,20,0,20,21,21,21,0,0};//定义事件中改变的site
    if (frame_id == 0){
        int change_site_id = ns.embed_list[ne.embed_index][chg_site[ne.frame_e_index]];
        return lua_get_rate(ns.position[0],ns.position[1],ns.position[2],frame_id,eventid,center[0],center[1],center[2]);//*lattice->sitelist[change_site_id].data[0];
    }else
    {//浓度改变事件
        return 0.1;
    }
    
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

int mykmc::perform_with_event(int change_event){
    if (event_storage[change_event].type==-1&&event_storage[change_event].frame_index==0){
        perform_with_frame_event(change_event);
        //生长之后应该改变浓度
        update_events_after_perform(change_event);
    }else if(event_storage[change_event].type==1){//扩散事件

    }else if(event_storage[change_event].type ==-1&&event_storage[change_event].frame_index == 1) //沉积
    {
        lattice->sitelist[event_storage[change_event].event_site].data[0] += 1;//增加浓度
        update_events_after_perform(change_event);
    }
    return 0;

}
int mykmc::run_one_step(){
    double dt;
    int eventid;
    if (using_events.size()==0){std::cout<<"empty event set"<<std::endl;}
    eventid=choose_event(&dt);
    perform_with_event(eventid);//包含执行后处理
    
    //update_area_with_site(event_storage[eventid].event_site);
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
                now_event->rate=getrate(siteid,frameid,k);
                now_event->type=-1;//表示为frame定义的类型
                num_e ++;
                nsite->site_events.push_back(now_event->id);
            }
        }
    }
    return num_e;
}

int mykmc::init(){
    init_all_embeding();
    change_state();
    init_all_event();
    // event &kuosan = *add_event();
    // kuosan.type=1;
    // kuosan.rate=(lattice->mysites.size()-Num_site1)*0.1;
    return 0;
}


int main(){
    mykmc &aa=*(new mykmc());
    //aa.lattice->add_site(1,1,1);
    std::cout<<aa.lattice->Number<<std::endl;
    //aa.add_site_frame(604,0);
    aa.init();
    //aa.run_N(aa.Nsteps);
    std::cout<<aa.Nsteps<<std::endl;
    std::fstream fp;
    fp.open("a.xyz",std::ios::out);
    for(int i=0;i<aa.Nsteps;i++){
        aa.run_one_step();
        if (i%20==0){
        fp<<aa.lattice->out_to_xyz(1);
        fp<<"\n";
        }
    }
    fp.close();
    return 0;
}