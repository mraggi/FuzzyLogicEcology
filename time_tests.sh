#!/bin/bash

printf "QuercusTime = ["
./timetest QuercusOaxaca.txt -I1 -I2 --memory=4GB 2> bah.txt | grep GGG | sed --expression='s/GGG: //g' | awk '!/0$/{printf $0}/0$/' | sed --expression='s/)(/),(/g' 
echo "]"

printf "LobataeTime = ["
./timetest lobatae.txt -I1 --memory=4GB 2> bah.txt | grep GGG | sed --expression='s/GGG: //g' | awk '!/0$/{printf $0}/0$/' | sed --expression='s/)(/),(/g'
echo "]"

printf "RojosTime = ["
./timetest rojos.txt -I0 --memory=4GB 2> bah.txt | grep GGG | sed --expression='s/GGG: //g' | awk '!/0$/{printf $0}/0$/' | sed --expression='s/)(/),(/g'
echo "]"

printf "CentroTime = ["
./timetest redes_centro.txt -I5 -I6 --memory=4GB 2> bah.txt | grep GGG | sed --expression='s/GGG: //g' | awk '!/0$/{printf $0}/0$/' | sed --expression='s/)(/),(/g'
echo "]"
