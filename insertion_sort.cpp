#include "sort_algorithms.hpp"
void insertionSort(std::vector<int>& a,const ProgressCallback& cb){
 const std::size_t n=a.size(); if(n<2){if(cb)cb(100);return;}
 for(std::size_t i=1;i<n;++i){int key=a[i];std::size_t j=i;while(j>0&&a[j-1]>key){a[j]=a[j-1];--j;}a[j]=key;
 if(cb&&(i%256==0||i==n-1))cb(static_cast<int>((100ULL*(i+1))/n));}
}
