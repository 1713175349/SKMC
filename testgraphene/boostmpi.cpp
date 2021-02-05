#include<boost/mpi.hpp>
#include<iostream>
#include<vector>
namespace mpi=boost::mpi;
int main(int argc,char **argv){
    mpi::environment env(argc,argv);
    mpi::communicator world;
    std::vector<int> aa({world.rank()});
    if(world.rank()==0){
        world.recv(1,2,aa);
        std::cout<<world.rank()<<"   "<<aa[0]<<std::endl;
    }
    else if(world.rank() == 1)
    {
        aa[0]=12;
        world.send(0,2,aa);
        std::cout<<world.rank()<<"   "<<aa[0]<<std::endl;
    }
    
    std::cout<<world.rank()<<std::endl;
    return 0;
}