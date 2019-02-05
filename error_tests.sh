#!/bin/bash

printf "QuercusAvgError = ["
./errortest QuercusOaxaca.txt -I1 -I2 --memory=4GB 2> bah.txt | grep GGG | tee quercus_error.txt | grep "AVERAGE" | sed --expression='s/GGG: AVERAGE //g' | awk '!/0$/{printf $0}/0$/' | sed --expression='s/)(/),(/g' 
echo "]"

printf "LobataeAvgError = ["
./errortest lobatae.txt -I1 --memory=4GB 2> bah.txt | grep GGG | tee lobatae_error.txt | grep "AVERAGE" | sed --expression='s/GGG: AVERAGE //g' | awk '!/0$/{printf $0}/0$/' | sed --expression='s/)(/),(/g' 
echo "]"

printf "RojosAvgError = ["
./errortest rojos.txt -I0 --memory=4GB 2> bah.txt | grep GGG | tee rojos_error.txt | grep "AVERAGE" | sed --expression='s/GGG: AVERAGE //g' | awk '!/0$/{printf $0}/0$/' | sed --expression='s/)(/),(/g' 
echo "]"

printf "CentroAvgError = ["
./errortest redes_centro.txt -I5 -I6 --memory=4GB 2> bah.txt | grep GGG | tee centro_error.txt | grep "AVERAGE" | sed --expression='s/GGG: AVERAGE //g' | awk '!/0$/{printf $0}/0$/' | sed --expression='s/)(/),(/g' 
echo "]"

printf "QuercusError = ["
cat quercus_error.txt | grep "GGG: (" | sed --expression='s/GGG: //g' | awk '!/0$/{printf $0}/0$/' | sed --expression='s/)(/),(/g' 
echo "]"

printf "LobataeError = ["
cat lobatae_error.txt | grep "GGG: (" | sed --expression='s/GGG: //g' | awk '!/0$/{printf $0}/0$/' | sed --expression='s/)(/),(/g' 
echo "]"

printf "RojosError = ["
cat rojos_error.txt | grep "GGG: (" | sed --expression='s/GGG: //g' | awk '!/0$/{printf $0}/0$/' | sed --expression='s/)(/),(/g' 
echo "]"

printf "CentroError = ["
cat centro_error.txt | grep "GGG: (" | sed --expression='s/GGG: //g' | awk '!/0$/{printf $0}/0$/' | sed --expression='s/)(/),(/g' 
echo "]"
