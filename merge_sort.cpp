#include "sort_algorithms.hpp"
#include <algorithm>
static void mergePart(std::vector<int>&a,std::vector<int>&t,std::size_t l,std::size_t m,std::size_t r){
 std::size_t i=l,j=m,k=l;while(i<m&&j<r)t[k++]=(a[i]<=a[j])?a[i++]:a[j++];while(i<m)t[k++]=a[i++];while(j<r)t[k++]=a[j++];for(std::size_t p=l;p<r;++p)a[p]=t[p];
}
static void mergeRec(std::vector<int>&a,std::vector<int>&t,std::size_t l,std::size_t r,unsigned long long&done,unsigned long long total,const ProgressCallback&cb){
 if(r-l<2)return;std::size_t m=l+(r-l)/2;mergeRec(a,t,l,m,done,total,cb);mergeRec(a,t,m,r,done,total,cb);mergePart(a,t,l,m,r);done+=r-l;
 if(cb)cb(static_cast<int>(std::min(99ULL,(100ULL*done)/total)));
}
void mergeSort(std::vector<int>&a,const ProgressCallback&cb){
 if(a.size()<2){if(cb)cb(100);return;}std::vector<int>t(a.size());unsigned long long levels=0;for(std::size_t x=a.size();x>1;x=(x+1)/2)++levels;
 unsigned long long total=static_cast<unsigned long long>(a.size())*levels,done=0;mergeRec(a,t,0,a.size(),done,total,cb);if(cb)cb(100);
}
