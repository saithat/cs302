#! /bin/sh

R=$1
C=$2

Mazemake ${R:=60} ${C:=80} maze.txt
Mazeimage maze.txt 

Mazesolve maze.txt path.txt 
Mazeimage maze.txt path.txt 

convert maze.ppm maze.png
convert path.ppm path.png
