#include<vector>
#include <algorithm>
#include "site.hpp"
class box
{
private:
    int Number;//site的数量
    int MaxSite;//最大Site数量
    std::vector<int> mysites;//所有site
    double basisa,basisb,basisc;//各个小网格的单位长度 
    int nbasisa,nbasisb,nbasisc;
    std::vector<int> ***areas;//局域
    site *sitelist;//存储所有的site
    std::vector<int> empty_site;//所有空位点

public:
    box(double ba,double bb,double bc,int na,int nb,int nc,int sitemax);
    ~box();
    site *add_site(double x,double y,double z);//添加一个site
    int add_edge(int i,int j);//i,j 为在sitelist中的位置
    //int delete_site(int i);//删除位点，暂时没有需要
};

box::box(double ba,double bb,double bc,int na,int nb,int nc,int sitemax)
{
    basisa=ba;
    basisb=bb;
    basisc=bc;
    nbasisa=na;
    nbasisb=nb;
    nbasisc=nc;
    MaxSite=sitemax;
    Number = 0;
    sitelist = new site[MaxSite];
    //初始化局域集合
    areas = new std::vector<int>**[nbasisa];
    for (int i = 0; i < nbasisa; i++)
    {
        areas[i] = new std::vector<int>*[nbasisb];
        for (int j = 0; j < nbasisb; j++)
        {
            areas[i][j] = new std::vector<int>[nbasisc];
        }
    }
    //初始化未使用的Site vector
    for (int i = 0; i < MaxSite; i++)
    {
        empty_site.insert(empty_site.end(),i);
    }
    
}

box::~box()
{
    delete [] sitelist;
    for (int i = 0; i < nbasisa; i++)
    {
        for (int j = 0; j < nbasisb; j++)
        {
            delete [] areas[i][j];
        }
        delete [] areas[i];
    }
    delete [] areas;

}

site *box::add_site(double x,double y,double z){
    //获取坐标在领域数据中的坐标
    int nx=int(x/basisa);
    int ny=int(y/basisb);
    int nz=int(z/basisc);
    //获取一个空位点
    int new_site_n=*empty_site.begin();
    empty_site.erase(empty_site.begin());
    areas[nx][ny][nz].insert(areas[nx][ny][nz].end(),new_site_n);//添加到临域
    mysites.insert(mysites.end(),new_site_n);//添加到site列表
    //修改邻域
    site *ns=sitelist+new_site_n;
    ns->position[0]=x;
    ns->position[1]=y;
    ns->position[2]=z;
    ns->areaid[0]=nx;
    ns->areaid[1]=ny;
    ns->areaid[2]=nz;
    ns->siteid=new_site_n;
    ns->isusing=true;
    return ns;
}

int box::add_edge(int ei,int ej){
    site *start_site=sitelist+ei;
    site *end_site = sitelist+ej;
    if (!(start_site->isusing && end_site->isusing)){
        throw "未分配某个位点,却需要创造边";
    }
    //是否已经有这条边了
    if (std::find(start_site->neighbors.begin(),start_site->neighbors.end(),ej)!=start_site->neighbors.end()){
        return 2;
    }

    start_site->neighbors.insert(start_site->neighbors.end(),ej);
    end_site->neighbors.insert(end_site->neighbors.end(),ei);
    return 1;
}