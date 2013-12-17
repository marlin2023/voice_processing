#!/bin/bash
#subscription:this shell script use to generate mix_pcm.lib
##########define current variable###############
PROJ_PATH=$(pwd)
THIRD_PARTY_PATH=${PROJ_PATH}/third_party
THIRD_INSTALL_PATH=${PROJ_PATH}/output
SPEEX_PATH=speex-1.2rc1
CMAKE_BUILD_PATH=${PROJ_PATH}/build

if [ $? -ne 0 ]
then
    echo "THIRD_PARTY_PATH error"
    exit 1
else
    echo ${THIRD_PARTY_PATH}
fi


#first ,to compile the speex code 
cd ${THIRD_PARTY_PATH}/${SPEEX_PATH}
echo $?
if [ -d ${THIRD_INSTALL_PATH} ]
then 
    echo "third install path is exisit"
else
    mkdir -p ${THIRD_INSTALL_PATH}
fi
./configure --prefix=${THIRD_INSTALL_PATH} --disable-shared
make && make install

echo "install speex header and library success"


#####compile static library
cd ${PROJ_PATH}
##cmake CMakeLists.txt
if [ -d ${CMAKE_BUILD_PATH} ]
then 
    echo "CMAKE_BUILD_PATH exisit"
else
    mkdir -p ${CMAKE_BUILD_PATH}
fi

cd ${CMAKE_BUILD_PATH}
#cmake .. && make && make install


