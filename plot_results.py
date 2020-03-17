from __future__ import print_function
import os, platform, sys, subprocess, imp
import numpy as np
import matplotlib.pyplot as plt
plt.rcdefaults()


# double 100
# 0.13622926
# 0.1312754
# 0.13287197
# 0.43718526
# 0.78880578

# double 150
# 0.80114001
# 0.6937311
# 0.73783923
# 2.299045
# 3.5829124

# double 200
# 2.406574
# 2.379607
# 2.2677328
# 6.522
# 11.1189

# single 100
# 0.0807457
# 0.28706351
# 0.0723373
# 0.41288866
# 0.76346416

# single 150
# 0.52801878
# 1.5160282
# 0.43575986
# 2.2451304
# 3.8300779

def run_command(command):
    p = subprocess.Popen(command,
                         stdout=subprocess.PIPE,
                         stderr=subprocess.STDOUT,
                         shell=True)
    return iter(p.stdout.readline, b'')

def main():

    # num_iter = 100
    # exes = ["./out_cpp_eigen.exe", "./out_cpp_blaze.exe", "./out_cpp_fastor.exe", "./out_cpp_armadillo.exe", "./out_cpp_xtensor.exe"]
    # # exes = ["./out_cpp_eigen.exe", "./out_cpp_blaze.exe", "./out_cpp_fastor.exe"]

    # performance = []
    # for exe in exes:
    #     mean_elapsed = 0.
    #     for i in range(num_iter):
    #         for counter, line in enumerate(run_command(exe + " 150")):
    #             if counter == 2:
    #                 sline = line.split(" ")[4]
    #                 elapsed = float(sline)
    #                 mean_elapsed += elapsed
    #     mean_elapsed /= float(num_iter)
    #     print(mean_elapsed, )
    #     performance.append(mean_elapsed)
    # performance = np.array(performance)

    # with hand-written norms for arma and xtensor
    # performance = [0.13681212, 0.1312151, 0.1321486, 0.43759528, 0.73877798] # double 100
    # performance = [0.80114001, 0.6937311, 0.73783923, 2.299045, 3.5829124] # double 150
    # performance = [2.406574, 2.379607, 2.2677328, 6.522, 11.1189] # double 200

    # performance = [2.406574, 2.379607, 2.2677328, 87.1129, 59.4571]

    performance = [4.49, 6.73, 2.54, 3.45, 13.77]
    # objects = ('Eigen', 'Blaze', 'Fastor')
    objects = ('Eigen', 'Blaze', 'Fastor', 'Armadillo', 'XTensor')
    # objects = ('Fortran','Eigen', 'Blaze', 'Fastor', 'Armadillo', 'XTensor')
    # performance = np.array([0.157,0.142, 0.134, 0.136, 0.450, 0.76602])
    y_pos = np.arange(len(objects))

    plt.bar(y_pos, performance, align='center', alpha=0.5)
    plt.xticks(y_pos, objects)
    plt.ylabel('Time in seconds')
    # plt.title('Performance of views')
    plt.title('Compilation time of views')
    plt.grid(True)

    plt.show()


main()

