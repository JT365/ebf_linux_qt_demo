#!/bin/bash

set -v

export PATH=/opt/qt-everywhere-src-5.14.1/bin:$PATH

rm *.tar.bz2 
#make distclean
#svn up

qmake && make 

mkdir -p ./run_dir

cd ./run_dir
rm -rf libskin libqui libpld
rm -rf App

mkdir libskin
mkdir libqui
mkdir libpld

cp ../app_bin/App -arf .
cp ../thirdpart/libskin/lib/* -arf ./libskin
cp ../thirdpart/libqui/lib/* -arf ./libqui
cp ../thirdpart/libpld/lib/* -arf ./libpld

#cp ../app_bin/App -arf ~/FireRtfs/opt
#cp ../thirdpart/libskin -arf ~/FireRtfs/opt/
#cp ../thirdpart/libqui -arf ~/FireRtfs/opt/

#tar 
tar -jcf ../elephant-$(date +%Y%m%d).tar.bz2 *
cd ..
