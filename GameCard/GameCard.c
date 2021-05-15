/*
 * Broker.c
 *
 *  Created on: 11 abr. 2020
 *      Author: utnso
 */
#include "GameCard.h"
#include "Inicializacion.h"
#include "MetodosGC.h"


int main(){//int argc, char **argv) {

	iniciar_log();

	leer_configFile(CONFIG_PATH);

	inicializar_semaforos();
	crearBloques();  //Verifico si los bloques existen y si coinciden con la cantidad de bloques del archivo de configuración.
	creacionDeArchivoBitmap(PuntoMontaje->BITMAP,config_MetaData->cantidad_bloques); //Verifico la existencia del Bitmap.
	crearBitmap(); //Cargo el contenigo del bitmap.
	loadPokemons();
	crearHilos();


}

