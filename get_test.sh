#!/bin/bash
g++ stress-test/get_test.cpp -o ./stress-test/get_test  --std=c++17 -pthread  
./stress-test/get_test