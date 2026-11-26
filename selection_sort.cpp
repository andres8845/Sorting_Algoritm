#include "sort_algorithms.hpp"
#include <algorithm>
void selectionSort(std::vector<int>& a,const ProgressCallback& cb){
 const std::size_t n=a.size();if(n<2){if(cb)cb(100);return;}
 for(std::size_t i=0;i<n;++i){std::size_t m=i;for(std::size_t j=i+1;j<n;++j)if(a[j]<a[m])m=j;if(m!=i)std::swap(a[i],a[m]);
 if(cb&&(i%64==0||i==n-1))cb(static_cast<int>((100ULL*(i+1))/n));}
}
