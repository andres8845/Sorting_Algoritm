#define UNICODE
#define _UNICODE
#include <windows.h>
#include <commctrl.h>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <random>
#include <string>
#include <thread>
#include <vector>
#include "sort_algorithms.hpp"

#define WM_PROGRESS (WM_APP+1)
#define WM_FINISHED (WM_APP+2)
static HWND hAlgo,hSize,hRun,hStatus,hProgress,hPercent;
static HANDLE workerHandle=nullptr;
static bool running=false;

struct Job{HWND hwnd;int algorithm;unsigned long long size;};

static void logRow(const std::string& row){
 std::ofstream f("gui_results.csv",std::ios::app);if(!f)return;f.seekp(0,std::ios::end);
 if(f.tellp()==0)f<<"Algorithm,Array_Size,Time_ms,Status\n";
 f<<row<<"\n";f.flush();
}

static DWORD WINAPI doSort(LPVOID raw){
 Job* job=static_cast<Job*>(raw);HWND hwnd=job->hwnd;int selected=job->algorithm;auto n=job->size;delete job;
 const std::string names[]={"Insertion Sort","Selection Sort","Bubble Sort","Merge Sort","Heap Sort","Quick Sort"};
 const std::string name=names[selected];logRow(name+","+std::to_string(n)+",,STARTED");
 std::vector<int> a;
 try{a.resize(static_cast<std::size_t>(n));}catch(...){logRow(name+","+std::to_string(n)+",,MEMORY_ALLOCATION_FAILED");PostMessageW(hwnd,WM_FINISHED,0,0);return 0;}
 std::mt19937 rng(20261125U+static_cast<std::uint32_t>(n%1000000007ULL));std::uniform_int_distribution<int>d(0,1000000000);
 for(int&v:a)v=d(rng);
 auto cb=[hwnd](int p){PostMessageW(hwnd,WM_PROGRESS,static_cast<WPARAM>(p),0);};
 auto start=std::chrono::steady_clock::now();
 switch(selected){case 0:insertionSort(a,cb);break;case 1:selectionSort(a,cb);break;case 2:bubbleSort(a,cb);break;case 3:mergeSort(a,cb);break;case 4:heapSort(a,cb);break;case 5:quickSort(a,cb);break;}
 auto finish=std::chrono::steady_clock::now();double ms=std::chrono::duration<double,std::milli>(finish-start).count();
 bool ok=std::is_sorted(a.begin(),a.end());logRow(name+","+std::to_string(n)+","+std::to_string(ms)+","+(ok?"OK":"INCORRECT"));
 PostMessageW(hwnd,WM_FINISHED,ok?1:0,0);return 0;
}

static void startTest(HWND hwnd){
 if(running)return;int selected=static_cast<int>(SendMessageW(hAlgo,CB_GETCURSEL,0,0));if(selected<0||selected>=6)return;
 wchar_t buf[64]{};GetWindowTextW(hSize,buf,64);unsigned long long n=0;
 try{std::wstring s(buf);std::size_t pos=0;n=std::stoull(s,&pos);if(pos!=s.size())throw std::invalid_argument("bad");}
 catch(...){MessageBoxW(hwnd,L"Introduce una cantidad valida.",L"Error",MB_ICONWARNING);return;}
 if(n<1||n>100000000ULL){MessageBoxW(hwnd,L"El tamano debe estar entre 1 y 100,000,000.",L"Error",MB_ICONWARNING);return;}
 Job*job=new Job{hwnd,selected,n};running=true;EnableWindow(hRun,FALSE);SendMessageW(hProgress,PBM_SETPOS,0,0);SetWindowTextW(hPercent,L"0%");SetWindowTextW(hStatus,L"Generando datos y preparando el ordenamiento...");
 workerHandle=CreateThread(nullptr,0,doSort,job,0,nullptr);
 if(!workerHandle){delete job;running=false;EnableWindow(hRun,TRUE);SetWindowTextW(hStatus,L"No se pudo iniciar el hilo.");}
}

static LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
 switch(msg){
 case WM_CREATE:{
  CreateWindowW(L"STATIC",L"Algoritmo:",WS_CHILD|WS_VISIBLE,20,20,100,22,hwnd,nullptr,nullptr,nullptr);
  hAlgo=CreateWindowW(L"COMBOBOX",L"",WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST,120,16,220,180,hwnd,(HMENU)101,nullptr,nullptr);
  const wchar_t*opts[]={L"Insertion Sort",L"Selection Sort",L"Bubble Sort",L"Merge Sort",L"Heap Sort",L"Quick Sort"};
  for(auto s:opts)SendMessageW(hAlgo,CB_ADDSTRING,0,reinterpret_cast<LPARAM>(s));SendMessageW(hAlgo,CB_SETCURSEL,0,0);
  CreateWindowW(L"STATIC",L"Elementos:",WS_CHILD|WS_VISIBLE,20,60,100,22,hwnd,nullptr,nullptr,nullptr);
  hSize=CreateWindowW(L"EDIT",L"1000",WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL,120,56,220,26,hwnd,(HMENU)102,nullptr,nullptr);
  hRun=CreateWindowW(L"BUTTON",L"Ejecutar prueba",WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON,120,100,150,32,hwnd,(HMENU)103,nullptr,nullptr);
  hStatus=CreateWindowW(L"STATIC",L"Listo. Elige algoritmo y cantidad.",WS_CHILD|WS_VISIBLE,20,150,490,35,hwnd,nullptr,nullptr,nullptr);
  hProgress=CreateWindowW(PROGRESS_CLASSW,L"",WS_CHILD|WS_VISIBLE,20,195,440,24,hwnd,(HMENU)104,nullptr,nullptr);SendMessageW(hProgress,PBM_SETRANGE,0,MAKELPARAM(0,100));
  hPercent=CreateWindowW(L"STATIC",L"0%",WS_CHILD|WS_VISIBLE,470,195,45,24,hwnd,nullptr,nullptr,nullptr);
  CreateWindowW(L"STATIC",L"Progreso estimado. Resultados guardados en gui_results.csv.",WS_CHILD|WS_VISIBLE,20,235,490,30,hwnd,nullptr,nullptr,nullptr);
  return 0;}
 case WM_COMMAND:if(LOWORD(wp)==103&&HIWORD(wp)==BN_CLICKED)startTest(hwnd);return 0;
 case WM_PROGRESS:{int p=static_cast<int>(wp);SendMessageW(hProgress,PBM_SETPOS,p,0);std::wstring s=std::to_wstring(p)+L"%";SetWindowTextW(hPercent,s.c_str());return 0;}
 case WM_FINISHED:running=false;if(workerHandle){CloseHandle(workerHandle);workerHandle=nullptr;}EnableWindow(hRun,TRUE);
  if(wp==1){SendMessageW(hProgress,PBM_SETPOS,100,0);SetWindowTextW(hPercent,L"100%");SetWindowTextW(hStatus,L"Terminado correctamente. Resultado guardado.");}
  else SetWindowTextW(hStatus,L"La prueba fallo. Revisa gui_results.csv.");return 0;
 case WM_CLOSE:if(running){MessageBoxW(hwnd,L"Hay una prueba en curso. Espera a que termine.",L"Prueba en curso",MB_OK|MB_ICONWARNING);return 0;}DestroyWindow(hwnd);return 0;
 case WM_DESTROY:PostQuitMessage(0);return 0;
 }
 return DefWindowProcW(hwnd,msg,wp,lp);
}

int WINAPI wWinMain(HINSTANCE instance,HINSTANCE,PWSTR,int show){
 INITCOMMONCONTROLSEX icc{sizeof(icc),ICC_PROGRESS_CLASS};InitCommonControlsEx(&icc);
 const wchar_t cls[]=L"SortBenchmarkProgressWindow";WNDCLASSW wc{};wc.lpfnWndProc=WndProc;wc.hInstance=instance;wc.lpszClassName=cls;wc.hCursor=LoadCursor(nullptr,IDC_ARROW);wc.hbrBackground=reinterpret_cast<HBRUSH>(COLOR_WINDOW+1);
 if(!RegisterClassW(&wc))return 1;
 HWND hwnd=CreateWindowExW(0,cls,L"Sorting Algorithms Benchmark",WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,CW_USEDEFAULT,CW_USEDEFAULT,550,320,nullptr,nullptr,instance,nullptr);
 if(!hwnd)return 1;ShowWindow(hwnd,show);UpdateWindow(hwnd);MSG msg{};while(GetMessageW(&msg,nullptr,0,0)>0){TranslateMessage(&msg);DispatchMessageW(&msg);}return 0;
}
