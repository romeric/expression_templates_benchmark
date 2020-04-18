# expression_templates_benchmark
Benchmark of expression templates libraries [eigen, blaze, fastor, armadilloa, xtensor]

To compile the benchmark download all the aforementioned libraries first and then

~~~
make all EIGENROOT=... BLAZEROOT=... FASTORROOT=... ARMAROOT=... XTENSORROOT=...
~~~

where `...` implies the paath to the library.

For more details:
https://medium.com/@romanpoya/a-look-at-the-performance-of-expression-templates-in-c-eigen-vs-blaze-vs-fastor-vs-armadillo-vs-2474ed38d982
