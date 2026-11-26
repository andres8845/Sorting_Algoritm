#include "sort_algorithms.hpp"
#include <algorithm>
static int med3(int a,int b,int c){if(a<b){if(b<c)return b;return a<c?c:a;}if(a<c)return a;return b<c?c:b;}
void quickSort(std::vector<int>&a,const ProgressCallback&cb){
 if(a.size()<2){if(cb)cb(100);return;}struct R{std::size_t l,r;};std::vector<R>st{{0,a.size()-1}};unsigned long long work=0,total=a.size();
 while(!st.empty()){R x=st.back();st.pop_back();std::size_t l=x.l,r=x.r;while(l<r){std::size_t mid=l+(r-l)/2;int p=med3(a[l],a[mid],a[r]);std::size_t lo=l,i=l,hi=r;
 while(i<=hi){if(a[i]<p)std::swap(a[lo++],a[i++]);else if(a[i]>p)std::swap(a[i],a[hi--]);else ++i;}work+=r-l+1;if(cb)cb(static_cast<int>(std::min(99ULL,(100ULL*work)/total)));
 std::size_t ls=lo-l,rs=r-hi;if(ls<rs){if(hi<r)st.push_back({hi+1,r});if(lo>l)r=lo-1;else break;}else{if(lo>l)st.push_back({l,lo-1});if(hi<r)l=hi+1;else break;}}}
 if(cb)cb(100);
}
