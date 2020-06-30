#!/bin/bash

sudo dnf install gcc gcc-c++ make cmake3
sudo dnf install autogen libtool
sudo dnf install doxygen
sudo dnf install curl-devel jsoncpp-devel
sudo dnf install gumbo-parser gumbo-parser-devel uriparser uriparser-devel
sudo dnf install sqlite-devel

## 下载并安装gumbo-parser
cd ../
git clone https://github.com/google/gumbo-parser.git

cd gumbo-parser
sh autogen.sh
./configure
make
sudo  make install

## 安装gumbo-query
cd ../
git clone https://github.com/lazytiger/gumbo-query.git
cd gumbo-query/build && cmake .. && make
sudo make install
cd ../

cd bebe-gumbo/build 
cmake .. && make


## 生成xcode项目
mkdir xcode
cd xcode 
cmake .. -G "Xcode"
