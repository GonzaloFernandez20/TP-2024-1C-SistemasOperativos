#!/bin/bash

cd memoria 
make all
cd ..

cd cpu 
make all
cd ..

cd kernel
make all
cd ..

cd entradasalida
make all
cd ..

