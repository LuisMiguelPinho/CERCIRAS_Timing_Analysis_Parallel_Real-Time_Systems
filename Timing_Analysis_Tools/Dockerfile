FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive
ENV TERM=xterm-256color

RUN apt-get update
RUN apt-get upgrade -y

RUN apt install -y software-properties-common

RUN apt install -y git clang-15 cmake ninja-build python3
RUN apt install -y autoconf gfortran binutils binutils-dev libiberty-dev libomp-15-dev python3-pip
RUN apt install -y libtool


ENV USER=cts
RUN apt install -y sudo
RUN useradd -m ${USER} -g sudo
RUN passwd -d ${USER}
USER ${USER}

ENV CC=/usr/bin/clang-15
ENV CXX=/usr/bin/clang++-15

ENV TAFLOW_SRC_PATH=/home/${USER}/taflow/srcs
ENV TAFLOW_PATH=/home/${USER}/taflow/tools
ENV TAFLOW_COURSE_PATH=/home/${USER}/course

RUN mkdir -p $TAFLOW_SRC_PATH
RUN mkdir -p $TAFLOW_PATH
RUN mkdir -p $TAFLOW_COURSE_PATH

#BUILD PAPI
WORKDIR ${TAFLOW_SRC_PATH} 
RUN git clone https://github.com/icl-utk-edu/papi.git 
WORKDIR ${TAFLOW_SRC_PATH}/papi/src 
RUN ./configure --prefix=${TAFLOW_PATH}/papi
RUN make -j 8
RUN sudo make install  

#BUILD EXTRAE
WORKDIR ${TAFLOW_SRC_PATH}
RUN git clone https://gitlab.bsc.es/ampere-sw/wp2/extrae.git
WORKDIR ${TAFLOW_SRC_PATH}/extrae

RUN autoreconf -vfi
RUN ./configure --prefix=${TAFLOW_PATH}/extrae --without-mpi --without-dyninst --without-unwind --with-papi=${TAFLOW_PATH}/papi --enable-openmp-intel
RUN make -j4
RUN make install

#BUILD LLVM
WORKDIR $TAFLOW_SRC_PATH
RUN git clone -b omp-taskgraph --depth 1 https://gitlab.bsc.es/ampere-sw/wp2/llvm.git
RUN mkdir $TAFLOW_SRC_PATH/llvm/build
WORKDIR ${TAFLOW_SRC_PATH}/llvm/build

RUN cmake -DLLVM_ENABLE_PROJECTS="clang;openmp" -DLLVM_TARGETS_TO_BUILD="X86" -DCMAKE_BUILD_TYPE=MinSizeRel -G "Ninja" -DCMAKE_INSTALL_PREFIX=${TAFLOW_PATH}/llvm -DLIBOMP_TASKGRAPH=1 -DOPENMP_ENABLE_LIBOMPTARGET=OFF ../llvm
# if the target system is not a x86, please try one of the following.
# multiple targets can be used, at a cost of more compilation time and more space
# -DLLVM_TARGETS_TO_BUILD="X86;ARM;AArch64"
RUN ninja -j 8 install

#DOWNLOAD TDG instr. script
WORKDIR ${TAFLOW_SRC_PATH} 
RUN git clone https://gitlab.bsc.es/ampere-sw/wp2/tdg-instrumentation-script.git

#INSTALL TIME ANALYSIS FRAMEWORK
ENV TIME_PREDICTABILITY_TOKEN=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
RUN pip3 install git+https://${TIME_PREDICTABILITY_TOKEN}@github.com/tiagodrcarvalho/time-predictability.git

ENV PATH=/home/${USER}/.local/bin:${TAFLOW_PATH}/llvm/bin:${TAFLOW_PATH}/extrae/bin:${TAFLOW_PATH}/papi/bin:${PATH}
ENV LD_LIBRARY_PATH=${TAFLOW_PATH}/llvm/lib:${TAFLOW_PATH}/extrae/lib:${LD_LIBRARY_PATH}

ENV EXTRAE_CONFIG_FILE=${TAFLOW_COURSE_PATH}/resources/extrae.xml
ENV EXTRAE_SKIP_AUTO_LIBRARY_INITIALIZE=1

ENV OMP_PATH=${TAFLOW_PATH}/llvm/lib
ENV OMP_NUM_THREADS=8

ENV CC=${TAFLOW_PATH}/llvm/bin/clang
ENV CXX=${TAFLOW_PATH}/llvm/bin/clang++

ENV SHELL /bin/bash

COPY course ${TAFLOW_COURSE_PATH}
RUN sudo chown -R cts:sudo ${TAFLOW_COURSE_PATH}