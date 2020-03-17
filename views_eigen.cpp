#define EIGEN_STACK_ALLOCATION_LIMIT 10000000000000
#include "helper.h"
#include <Eigen/Core>
using namespace Eigen;
using namespace bench_views;


template<typename T, int num, int SO>
T finite_difference_block_impl(Matrix<T,num,num,SO> &u) {

    Matrix<T,num,num,SO> u_old = u;

    // asm("#BEGINN");
    u.block(1,1,num-2,num-2) =
        ((  u_old.block(0,1,num-2,num-2) + u_old.block(2,1,num-1,num-2) +
            u_old.block(1,0,num-2,num-3) + u_old.block(1,2,num-2,num-1) )*4.0 +
            u_old.block(0,0,num-3,num-3) + u_old.block(0,2,num-3,num-1) +
            u_old.block(2,0,num-1,num-3) + u_old.block(2,2,num-1,num-1) ) /20.0;
    // asm("#ENDD");

    return (u-u_old).norm();
}


template<typename T, int num, int SO>
T finite_difference_fblock_impl(Matrix<T,num,num,SO> &u) {

    Matrix<T,num,num,SO> u_old = u;

    u.template block<num-2,num-2>(1,1) =
        ((  u_old.template block<num-2,num-2>(0,1) + u_old.template block<num-2,num-2>(2,1) +
            u_old.template block<num-2,num-2>(1,0) + u_old.template block<num-2,num-2>(1,2) )*4.0 +
            u_old.template block<num-2,num-2>(0,0) + u_old.template block<num-2,num-2>(0,2) +
            u_old.template block<num-2,num-2>(2,0) + u_old.template block<num-2,num-2>(2,2) ) /20.0;

    return (u-u_old).norm();
}


template<typename T, int num, int SO>
T finite_difference_seq_impl(Matrix<T,num,num,SO> &u) {

    Matrix<T,num,num,SO> u_old = u;
    constexpr int last = num;

    u(seq(1,num-2),seq(1,num-2)) =
        ((  u_old(seq(0,num-3),seq(1,num-2)) + u_old(seq(2,num-1),seq(1,num-2)) +
            u_old(seq(1,num-2),seq(0,num-3)) + u_old(seq(1,num-2),seq(2,num-1)) )*4.0 +
            u_old(seq(0,num-3),seq(0,num-3)) + u_old(seq(0,num-3),seq(2,num-1)) +
            u_old(seq(2,num-1),seq(0,num-3)) + u_old(seq(2,num-1),seq(2,num-1)) ) /20.0;

    return (u-u_old).norm();
}

template<typename T, int num, int SO>
T finite_difference_fseq_impl(Matrix<T,num,num,SO> &u) {

    Matrix<T,num,num> u_old = u;
    constexpr int last = num;

    u(seq(fix<1>,fix<last-2>),seq(fix<1>,fix<last-2>)) =
        ((  u_old(seq(fix<0>,fix<last-3>),seq(fix<1>,fix<last-2>)) + u_old(seq(fix<2>,fix<last-1>),seq(fix<1>,fix<last-2>)) +
            u_old(seq(fix<1>,fix<last-2>),seq(fix<0>,fix<last-3>)) + u_old(seq(fix<1>,fix<last-2>),seq(fix<2>,fix<last-1>)) )*4.0 +
            u_old(seq(fix<0>,fix<last-3>),seq(fix<0>,fix<last-3>)) + u_old(seq(fix<0>,fix<last-3>),seq(fix<2>,fix<last-1>)) +
            u_old(seq(fix<2>,fix<last-1>),seq(fix<0>,fix<last-3>)) + u_old(seq(fix<2>,fix<last-1>),seq(fix<2>,fix<last-1>)) ) /20.0;

    return (u-u_old).norm();
}



template<typename T, int num>
void run_finite_difference() {

    T pi = 4*std::atan(1.0);
    T err = 2.;
    int iter = 0;

    Matrix<T,num,1> x;
    for (auto i=0; i<num; ++i) {
        x(i) = i*pi/(num-1);
    }

    // Matrix<T,num,num,RowMajor> u; u.setZero();
    Matrix<T,num,num> u; u.setZero();
    u.col(0) = x.array().sin();
    u.col(num-1) = x.array().sin()*std::exp(-pi);

    while (iter <100000 && err>1e-6) {
        // err = finite_difference_block_impl(u);
        // err = finite_difference_fblock_impl(u);
        err = finite_difference_seq_impl(u);
        // err = finite_difference_fseq_impl(u);
        iter++;
    }

    println(" Relative error is: ", err, '\n');
    println("Number of iterations: ", iter, '\n');
}



int main(int argc, char *argv[]) {

    using T = float;
    int N;
    if (argc == 2) {
       N = atoi(argv[1]);
    }
    else {
       print("Usage: \n");
       print("      ./exe N \n", argv[0]);
       exit(-1);
    }


    timer<double> t_j;
    t_j.tic();
    if (N==100) run_finite_difference<T,100>();
    if (N==150) run_finite_difference<T,150>();
    if (N==200) run_finite_difference<T,200>();
    t_j.toc();

    return 0;
}