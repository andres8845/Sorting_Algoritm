#include "sort_algorithms.hpp"
#include <algorithm>
static void sift(std::vector<int>&a,std::size_t n,std::size_t root){for(;;){std::size_t m=root,l=2*root+1,r=2*root+2;if(l<n&&a[l]>a[m])m=l;if(r<n&&a[r]>a[m])m=r;if(m==root)return;std::swap(a[root],a[m]);root=m;}}
void heapSort(std::vector<int>&a,const ProgressCallback&cb){
 std::size_t n=a.size();if(n<2){if(cb)cb(100);return;}for(std::size_t i=n/2;i>0;--i)sift(a,n,i-1);if(cb)cb(5);
 unsigned long long total=n-1,done=0;for(std::size_t e=n;e>1;--e){std::swap(a[0],a[e-1]);sift(a,e-1,0);++done;if(cb&&(done%128==0||done==total))cb(5+static_cast<int>((95ULL*done)/total));}
}
