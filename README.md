# expression_templates_benchmark
Benchmark of expression templates libraries [eigen, blaze, fastor, armadilloa, xtensor]

To compile the benchmark download all the aforementioned libraries first and then

~~~
make all EIGENROOT=... BLAZEROOT=... FASTORROOT=... ARMAROOT=... XTENSORROOT=... CXX=g++/clang++/icpc/...
~~~

where `...` implies path to the library. To run the benchmark

~~~
make run N=size
~~~

where `size=100,150,200` is the grid size.

For more details:
https://medium.com/@romanpoya/a-look-at-the-performance-of-expression-templates-in-c-eigen-vs-blaze-vs-fastor-vs-armadillo-vs-2474ed38d982
