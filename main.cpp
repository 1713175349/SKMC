#include<iostream>
#include"kmc.hpp"

int main(){
    kmc aa=kmc();
    aa.lattice->add_site(1,1,1);
    std::cout<<aa.lattice->Number;
    return 0;
}