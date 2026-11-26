#include "sort_algorithms.hpp"
#include <algorithm>
void bubbleSort(std::vector<int>& a,const ProgressCallback& cb){
 const std::size_t n=a.size();if(n<2){if(cb)cb(100);return;}
 for(std::size_t i=0;i<n;++i){bool changed=false;for(std::size_t j=0;j+1<n-i;++j)if(a[j]>a[j+1]){std::swap(a[j],a[j+1]);changed=true;}
 if(cb&&(i%64==0||i==n-1))cb(static_cast<int>((100ULL*(i+1))/n));if(!changed){if(cb)cb(100);break;}}
}
