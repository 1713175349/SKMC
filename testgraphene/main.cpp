#include<iostream>
#include"kmc.hpp"

int main(){
    kmc &aa=*(new kmc(1));
    //aa.lattice->add_site(1,1,1);
    std::cout<<aa.lattice->Number;
    //aa.add_site_frame(604,0);
    aa.init_all_embeding();
    aa.change_state();
    //aa.add_site_frame(0,0);
    // for (auto i: aa.lattice->mysites)
    // {
    //     int &&r=aa.lattice->sitelist[i].embed_framework_id.size();
    //     std::cout<<i<<"  "<<r<<std::endl;
    // }
    aa.init_all_event();
    aa.run_N(900);
    std::fstream fp;
    fp.open("a.xyz",std::ios::out);
    fp<<aa.lattice->out_to_xyz(1);
    fp.close();
    return 0;
}