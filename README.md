# tp-2020-1c-Digimon

#Valgrind Leaks

--leak-check=full --show-leak-kinds=all

#Dump Broker

kill -s USR1 PID

## Utilización Commons

1) Se debe agregar la variable de la biblioteca nuestra con el comando -> export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/workspace/digiCommons/Debug
2) Validar con ldd ${PROJECT} si se agrego correctamente

## Para el Debug

Opción A
1) Dentro del eclipse hacer click en la flecha abajo de debug 
2) Ir a Run Configurations

Opción B
1) Ir a propiedas del proyecto (ALT + ENTER)
2) Ir a Run/Debug Setting


Flujo en Común

3) C/C++ Applications -> New
4) Ir a la pestaña Environments -> New
5) En name ingresar -> LD_LIBRARY_PATH
6) En value ingresar -> ${workspace_loc}/digiCommons/Debug
7) Reiniciar Eclipse

## Problemas de RED

Look into /etc/NetworkManager/NetworkManager.conf and change the line managed=false to managed=true then restart network-manager: sudo service network-manager restart

## Para generar el Makefile en el Server el Project con MovaXTerms

Abrimos el MovaXTerms creamos una nueva Session SSH con la IP del Server y el Specify Username como utnso -> OK.
git clone https://...
cd pathProject/Debug
make clean
make
