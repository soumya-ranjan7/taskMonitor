#!/bin/bash

mkdir /home/$USER/taskMonitor
cp main.cpp /home/$USER/taskMonitor
cp README /home/$USER/taskMonitor
cp uninstall.sh /home/$USER/taskMonitor
cd /home/$USER/taskMonitor
g++ -std=c++11 main.cpp -o proc -lpthread
#echo "bind -x '\"\ez\":'/home/$USER/taskMonitor/proc''" >> /home/$USER/.bashrc
#bash

