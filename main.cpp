#include<iostream>
#include"kmc.hpp"

int main(){
    kmc aa(1);
    aa.lattice->add_site(1,1,1);
    std::cout<<aa.lattice->Number;

    std::fstream fp;
    fp.open("a.xyz",std::ios::out);
    fp<<aa.lattice->out_to_xyz();
    fp.close();
    return 0;
}