#pragma once
#include<vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "site.hpp"
#include<boost/multi_array.hpp>
#include<iostream>
#include<string>
class box
{
private:


public:
    int Number;//site的数量
    int MaxSite;//最大Site数量
    std::vector<int> mysites;//所有site
    double basisa,basisb,basisc;//各个小网格的单位长度 
    int nbasisa,nbasisb,nbasisc;
    std::vector<int> ***areas;//局域
    site *sitelist;//存储所有的site
    std::vector<int> empty_site;//所有空位点
    box();
    box(const char *boxfile);
    box(double ba,double bb,double bc,int na,int nb,int nc,int sitemax);
    ~box();
    site *add_site(double x,double y,double z);//添加一个site
    int add_edge(int i,int j);//i,j 为在sitelist中的位置
    std::string out_to_xyz();
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

box::box(){}

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
    areas[nx][ny][nz].push_back(new_site_n);//添加到临域
    mysites.push_back(new_site_n);//添加到site列表
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
    Number++;
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

box::box(const char *boxfile){
    std::fstream fp;
    const int buffer_size = 1000;
    char buff[buffer_size];
    std::stringstream sbuf;
    //打开结构描述文件
    fp.open(boxfile,std::ios::in);

    //读取box大小和领域大小
    fp.getline(buff,buffer_size);//跳过注释
    fp.getline(buff,buffer_size);
    sbuf.clear();
    sbuf.str(buff);
    sbuf.seekg(std::ios_base::beg); 
    sbuf>>basisa;
    sbuf>>basisb;
    sbuf>>basisc;
    sbuf>>nbasisa;
    sbuf>>nbasisb;
    sbuf>>nbasisc;
    sbuf>>MaxSite;
    //读取site的lattice
    double labc[3][3];
    int nabc[3];
    fp.getline(buff,buffer_size);//跳过空行
    fp.getline(buff,buffer_size);//跳过注释
    //a
    fp.getline(buff,buffer_size);
    sbuf.clear();
    sbuf.str(buff);
    sbuf.seekg(std::ios_base::beg);
    sbuf>>labc[0][0]>>labc[0][1]>>labc[0][2]>>nabc[0];
    //b
    fp.getline(buff,buffer_size);
    sbuf.clear();
    sbuf.str(buff);
    sbuf.seekg(std::ios_base::beg);
    sbuf>>labc[1][0]>>labc[1][1]>>labc[1][2]>>nabc[1];
    //c
    fp.getline(buff,buffer_size);
    sbuf.clear();
    sbuf.str(buff);
    sbuf.seekg(std::ios_base::beg);
    sbuf>>labc[2][0]>>labc[2][1]>>labc[2][2]>>nabc[2];
    //原点坐标
    double origin_position[3];
    fp.getline(buff,buffer_size);//跳过空行
    fp.getline(buff,buffer_size);//跳过注释
    fp.getline(buff,buffer_size);
    sbuf.clear();
    sbuf.str(buff);
    sbuf.seekg(std::ios_base::beg);
    sbuf>>origin_position[0]>>origin_position[1]>>origin_position[2];
    //cell中的site数目
    int site_cell_n;
    fp.getline(buff,buffer_size);//跳过空行
    fp.getline(buff,buffer_size);//跳过注释
    fp.getline(buff,buffer_size);
    sbuf.clear();
    sbuf.str(buff);
    sbuf.seekg(std::ios_base::beg);
    sbuf>>site_cell_n;
    //MaxSite=site_cell_n*nabc[0]*nabc[1]*nabc[2];
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

    //读取site
    std::vector<std::vector<double>> sites_position(site_cell_n,std::vector<double>(3));
    std::vector<int> sites_type(site_cell_n);
    fp.getline(buff,buffer_size);//跳过空行
    fp.getline(buff,buffer_size);//跳过注释
    for (int i = 0; i < site_cell_n; i++)
    {
        int site_id_cell=0;
        fp.getline(buff,buffer_size);
        sbuf.clear();
        sbuf.str(buff);
        sbuf.seekg(std::ios_base::beg);
        sbuf>>site_id_cell;
        sbuf>>sites_position[site_id_cell][0]>>sites_position[site_id_cell][2]>>sites_position[site_id_cell][2];
        sbuf>>sites_type[site_id_cell];
    }
    
    //写入所有site
    site *dealing;
    //boost::multi_array<int,4> cells(boost::extents[nabc[0]][nabc[1]][nabc[2]][site_cell_n]);
    //std::vector<std::vector<std::vector<std::vector<int>>>> cells(nabc[0],std::vector<std::vector<std::vector<int>>>(nabc[1],std::vector<std::vector<int>>(nabc[2],std::vector<int>(site_cell_n))));
    std::vector<std::vector<std::vector<std::vector<int>>>> cells(nabc[0],std::vector<std::vector<std::vector<int>>>(nabc[1],std::vector<std::vector<int>>(nabc[2],std::vector<int>(site_cell_n))));
    
    
    for (int indexa = 0; indexa < nabc[0]; indexa++)
    {
        for (int  indexb = 0; indexb < nabc[1]; indexb++)
        {
            for (int indexc = 0; indexc < nabc[2]; indexc++)
            {
                for (int site_id_in_cell = 0; site_id_in_cell < site_cell_n; site_id_in_cell++)
                {
                    dealing = add_site(indexa*labc[0][0]+indexb*labc[1][0]+indexc*labc[2][0]+sites_position[site_id_in_cell][0],
                                    indexa*labc[0][1]+indexb*labc[1][1]+indexc*labc[2][1]+sites_position[site_id_in_cell][1],
                                    indexa*labc[0][2]+indexb*labc[1][2]+indexc*labc[2][2]+sites_position[site_id_in_cell][2]);
                    dealing->type = sites_type[site_id_in_cell];
                    cells[indexa][indexb][indexc][site_id_in_cell]=dealing->siteid;

                }
                
            }
            
        }
        
    }
    
    //读取边
    std::vector<std::vector<std::vector<int>>> cell_edge(site_cell_n,std::vector<std::vector<int>>());
    std::vector<int> sites_neighbor_numbers(site_cell_n);
    fp.getline(buff,buffer_size);//跳过空行
    fp.getline(buff,buffer_size);//跳过注释
    for (int i = 0; i < site_cell_n; i++)
    {
        fp.getline(buff,buffer_size);
        sbuf.clear();
        sbuf.str(buff);
        sbuf.seekg(std::ios_base::beg);
        int id_incell;
        sbuf>>id_incell;
        sbuf>>sites_neighbor_numbers[id_incell];
        for (int j = 0; j < sites_neighbor_numbers[id_incell]; j++)
        {
            cell_edge[id_incell].push_back(std::vector<int>(4));
            fp.getline(buff,buffer_size);
            sbuf.clear();
            sbuf.str(buff);
            sbuf.seekg(std::ios_base::beg);
            sbuf>>cell_edge[id_incell][j][0]>>cell_edge[id_incell][j][1]>>cell_edge[id_incell][j][2]>>cell_edge[id_incell][j][3];
        }
    }
    
    //写入边
    for (int indexa = 0; indexa < nabc[0]; indexa++)
    {
        for (int  indexb = 0; indexb < nabc[1]; indexb++)
        {
            for (int indexc = 0; indexc < nabc[2]; indexc++)
            {
                for (int site_id_in_cell = 0; site_id_in_cell < site_cell_n; site_id_in_cell++)
                {
                    for (int neighbor_index = 0; neighbor_index < sites_neighbor_numbers[site_id_in_cell]; neighbor_index++)
                    {
                        int aneigh=indexa+cell_edge[site_id_in_cell][neighbor_index][0];
                        int bneigh=indexb+cell_edge[site_id_in_cell][neighbor_index][1];
                        int cneigh=indexc+cell_edge[site_id_in_cell][neighbor_index][2];
                        if (aneigh>=0&&bneigh>=0&&cneigh>=0&&aneigh<nabc[0]&&bneigh<nabc[1]&&cneigh<nabc[2]){
                            int neighborid=cells[aneigh][bneigh][cneigh][cell_edge[site_id_in_cell][neighbor_index][3]];
                            int nowid=cells[indexa][indexb][indexc][site_id_in_cell];
                            add_edge(nowid,neighborid);
                        }
                    }
                    
                }
            }
        }
    }
    fp.close();

}

std::string box::out_to_xyz(){
    std::stringstream xyzst;
    xyzst<<Number<<"\n";
    xyzst<<"box\n";
    for (auto i : mysites)
    {
        xyzst<<sitelist[i].state<<"    "
            <<sitelist[i].position[0]<<"    "
            <<sitelist[i].position[1]<<"    "
            <<sitelist[i].position[2]<<"    "<<"\n";
    }
    return xyzst.str();
}