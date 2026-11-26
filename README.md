# Sorting Benchmark con barra de progreso

Compilar en PowerShell con TDM-GCC/MinGW:
```powershell
g++ -O2 -std=c++17 -municode -mwindows main.cpp insertion_sort.cpp selection_sort.cpp bubble_sort.cpp merge_sort.cpp heap_sort.cpp quick_sort.cpp -lcomctl32 -o SortingBenchmark.exe
```
Ejecutar:
```powershell
.\SortingBenchmark.exe
```
La barra muestra el avance estimado basado en el trabajo interno de cada algoritmo. El ordenamiento corre en un hilo secundario para mantener la ventana receptiva. Los tiempos incluyen solo el ordenamiento; generación y validación quedan fuera. Resultados en gui_results.csv.
