On fornax: append the following in the ~/.bashrc file
Fornax has two CPU, total 36 cores, supporting 72 threads

CUDA_PATH=/opt/nvidia/cuda
export PATH=${CUDA_PATH}/bin:$PATH

. /opt/intel/bin/iccvars.sh -arch intel64 -platform linux
unset SSH_ASKPASS
ulimit -c unlimited

./fib_ 40 with 1, 2, 4, 8, 16, 32, 64

./axpy_ 100000000 1, 2, 4, 8, 16, 32, 64
