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
    mykmc();
    double center[3];
    void update_center();
    int change_state() override;
    int getint_lua(const char *name);
    int lua_lattice_state(double x,double y, double z,int type);
    double lua_get_rate(double x,double y, double z,int frameid,int eventid,double cx,double cy, double cz);
    double getrate(int siteid,int frame_id,int eventid) override;
    int run_one_step() override;
    ~mykmc();
};

mykmc::mykmc():kmc(12){
    L=luaL_newstate();
    luaL_openlibs(L);
    luaL_dofile(L,"my.lua");
    lua_getglobal(L,"runsteps");
    Nsteps=(int)lua_tointeger(L,-1);
    lua_pop(L,1);
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
    return lua_get_rate(ns.position[0],ns.position[1],ns.position[2],frame_id,eventid,center[0],center[1],center[2]);
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

int mykmc::run_one_step(){
    double dt;
    int eventid;
    if (using_events.size()==0){std::cout<<"empty event set"<<std::endl;}
    eventid=choose_event(&dt);
    perform_with_event(eventid);
    update_events_after_perform(eventid);
    update_center();
    //update_area_with_site(event_storage[eventid].event_site);
    t += dt;
    return eventid;
}

int mykmc::change_state(){
    for(auto i : lattice->mysites){
        site &now=lattice->sitelist[i];
        now.state=lua_lattice_state(now.position[0],now.position[1],now.position[2],now.type);
        
    }
    return 1;
}




int main(){
    mykmc &aa=*(new mykmc());
    //aa.lattice->add_site(1,1,1);
    std::cout<<aa.lattice->Number<<std::endl;
    //aa.add_site_frame(604,0);
    aa.init_all_embeding();
    aa.change_state();
    //aa.add_site_frame(0,0);
    // for (auto i: aa.lattice->mysites)
    // {
    //     int &&r=aa.lattice->sitelist[i].embed_framework_id.size();
    //     std::cout<<i<<"  "<<r<<std::endl;
    // }
    aa.update_center();
    aa.init_all_event();
    aa.run_N(aa.Nsteps);
    std::cout<<aa.Nsteps<<std::endl;
    std::fstream fp;
    fp.open("a.xyz",std::ios::out);
    fp<<aa.lattice->out_to_xyz(1);
    fp.close();
    return 0;
}