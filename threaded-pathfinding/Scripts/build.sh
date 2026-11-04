#!/bin/bash

base_name=$1

v1="${base_name}1"
v2="${base_name}2"
v3="${base_name}3"

echo "Building $v1"
g++ -std=c++11 ./Programs/Version1/*.cpp -o "$v1"

echo "Building $v2"
g++ -Wall -std=c++11 ./Programs/Version2/*.cpp -o "$v2"

echo "Building $v3"
g++ -Wall -std=c++11 ./Programs/Version3/*.cpp -o "$v3"