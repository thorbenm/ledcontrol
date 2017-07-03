#/bin/bash
echo "making ..." && g++ -lwiringPi -std=c++14 main.cpp ../lasercontrol/spi.cpp ../pid/pid.cpp && echo "done" && echo "running:" && ./a.out
