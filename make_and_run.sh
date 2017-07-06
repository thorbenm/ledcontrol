#/bin/bash
echo "making ..." && g++ -lwiringPi -std=c++14 main.cpp ../lasercontrol/spi.cpp ../pid/pid.cpp && echo "done" && echo "running:" && rm data.txt && ./a.out >> data.txt && echo "finished"
