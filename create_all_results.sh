#!/bin/bash

./fuzzylogic -I1 -I2 --grid=5000 --memory=2GB QuercusOaxaca.txt > QuercusResults.txt
./fuzzylogic -I1 --grid=5000 --memory=2GB lobatae.txt > LobataeResults.txt
./fuzzylogic -I5 -I6 --grid=5000 --memory=2GB redes_centro.txt > CentroResults.txt

./fuzzylogic --fuzzy_min -I1 -I2 --grid=4000 --memory=2GB QuercusOaxaca.txt > QuercusMINResults.txt
./fuzzylogic --fuzzy_min -I1 --grid=4000 --memory=2GB lobatae.txt > LobataeMINResults.txt
./fuzzylogic --fuzzy_min -I5 -I6 --grid=4000 --memory=2GB redes_centro.txt > CentroMINResults.txt

./fuzzylogic --propincuity -I1 -I2 --grid=4000 --memory=2GB QuercusOaxaca.txt > QuercusPROPResults.txt
./fuzzylogic --propincuity -I1 --grid=4000 --memory=2GB lobatae.txt > LobataePROPResults.txt
./fuzzylogic --propincuity -I5 -I6 --grid=4000 --memory=2GB redes_centro.txt > CentroPROPResults.txt
