#/bin/bash
echo "making ..." && g++ -lwiringPi -std=c++14 *.cpp && echo "done" && echo "running:" && ./a.out
