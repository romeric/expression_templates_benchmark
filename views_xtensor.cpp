#include "helper.h"
#include <xsimd/xsimd.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xfixed.hpp>
#include <xtensor/xview.hpp>
#include <xtensor/xnorm.hpp>
#include <xtensor-blas/xlinalg.hpp>

using namespace bench_views;

#define USE_DEFAULT 0


template<typename T, size_t nn>
T finite_difference_seq_impl(xt::xtensor_fixed<T, xt::xshape<nn,nn>> &u) {
    using xt::view;
    using xt::range;

    xt::xtensor_fixed<T, xt::xshape<nn,nn>> u_old = u;
    constexpr int num = nn;

    view(u,range(1,num-1),range(1,num-1)) =
        ((  view(u_old,range(0,num-2),range(1,num-1)) + view(u_old,range(2,num-0),range(1,num-1)) +
            view(u_old,range(1,num-1),range(0,num-2)) + view(u_old,range(1,num-1),range(2,num-0)) )*4.0 +
            view(u_old,range(0,num-2),range(0,num-2)) + view(u_old,range(0,num-2),range(2,num-0)) +
            view(u_old,range(2,num-0),range(0,num-2)) + view(u_old,range(2,num-0),range(2,num-0)) ) /20.0;

#if USE_DEFAULT
    // xt::xtensor_fixed<T, xt::xshape<nn,nn>> tmp = u-u_old;
    // return xt::linalg::norm(tmp);
    return xt::linalg::norm(u-u_old);
#else
    T err = 0.;
    for (auto i=0; i<num; ++i) {
        for (auto j=0; j<num; ++j) {
            const auto tmp = u(j,i) - u_old(j,i);
            err += tmp*tmp;
        }
    }
    return std::sqrt(err);

    // SIMD impl - not used to be fair with armadillo
    // xsimd::batch<T, xsimd::simd_type<T>::size> vec_err(T(0));
    // T err = 0;
    // constexpr std::size_t simd_size = xsimd::simd_type<T>::size;
    // for(std::size_t i = 0; i < num; ++i) {
    //     std::size_t j = 0;
    //     for(; j < num; j += simd_size) {
    //         auto u_vec     = xsimd::load_unaligned(&u.data()[i*num+j]);
    //         auto u_old_vec = xsimd::load_unaligned(&u_old.data()[i*num+j]);
    //         xsimd::batch<T, xsimd::simd_type<T>::size> diff = u_vec - u_old_vec;
    //         vec_err += diff*diff;
    //     }
    //     for(; j < num; ++j) {
    //         auto diff = u(i,j) - u_old(i,j);
    //         err += diff*diff;
    //     }
    // }
    // return std::sqrt(xsimd::hadd(vec_err) + err);
#endif
}


template<typename T, int num>
void run_finite_difference() {

    T pi = 4*std::atan(1.0);
    T err = 2.;
    int iter = 0;

    xt::xtensor_fixed<T, xt::xshape<num>> x;
    for (auto i=0; i<num; ++i) {
        x[i] = i*pi/(num-1);
    }

    xt::xtensor_fixed<T, xt::xshape<num,num>> u;
    u = xt::zeros<T>({num,num});
    xt::col(u,0) = xt::sin(x);
    xt::col(u,num-1) = sin(x)*std::exp(-pi);

    while (iter <100000 && err>1e-6) {
        err = finite_difference_seq_impl<T,num>(u);;
        iter++;
    }

    println(" Relative error is: ", err, '\n');
    println("Number of iterations: ", iter, '\n');
}


int main(int argc, char *argv[]) {

    using T = double;
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