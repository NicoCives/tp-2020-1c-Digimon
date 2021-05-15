/*
 * generales.c
 *
 *  Created on: 29 abr. 2020
 *      Author: utnso
 */
#include "generales.h"
#include <time.h>
#include <sys/time.h>


unsigned long long obtener_timestamp()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	unsigned long long timestamp = ((unsigned long long)tv.tv_sec*1e3) + ((unsigned long long)tv.tv_usec/1000);
	return timestamp;
}

unsigned int token() {

	srand(time(NULL));

	return (unsigned int) rand() % random();
}

int devolverTipoMsj (char * comando) {
	if (strcasecmp("NEW_POKEMON",comando) == 0 ) return NEW_POKEMON ;
	if (strcasecmp("GET_POKEMON",comando) == 0 ) return GET_POKEMON ;
	if (strcasecmp("LOCALIZED_POKEMON",comando) == 0 ) return LOCALIZED_POKEMON ;
	if (strcasecmp("APPEARED_POKEMON",comando) == 0 ) return APPEARED_POKEMON ;
	if (strcasecmp("CATCH_POKEMON",comando) == 0 ) return CATCH_POKEMON ;
	if (strcasecmp("CAUGHT_POKEMON",comando) == 0 ) return CAUGHT_POKEMON ;
	if (strcasecmp("ACK",comando) == 0 ) return ACK ;
	return false;
}

char * tipoMsjIntoToChar (int head) {

					switch( head ){

									case NEW_POKEMON :{
										 return "NEW_POKEMON";
										break;
									}
									case CATCH_POKEMON :{
										return "CATCH_POKEMON";
															break;
									}
									case GET_POKEMON :{
										return "GET_POKEMON";
										break;
									}

									case APPEARED_POKEMON :{
										return "APPEARED_POKEMON";
										break;
									}

									case CAUGHT_POKEMON :{
										return "CAUGHT_POKEMON";
										break;
									}

									case LOCALIZED_POKEMON :{
										return "LOCALIZED_POKEMON";
										break;
									}
							}
					return false;
}

char * devolverModulo (int head) {

					switch( head ){

									case GAMEBOY :{
										 return "GAMEBOY";
										break;
									}
									case GAMECARD :{
										return "GAMECARD";
															break;
									}
									case TEAM :{
										return "TEAM";
										break;
									}

									case BROKER :{
										return "BROKER";
										break;
									}
							}

				return false;
}

int devolverEnumModulo (char * comando) {

	if (strcasecmp("GAMECARD",comando) == 0 ) return GAMECARD ;
	if (strcasecmp("BROKER",comando) == 0 ) return BROKER ;
	if (strcasecmp("TEAM",comando) == 0 ) return TEAM ;
	if (strcasecmp("GAMEBOY",comando) == 0 ) return GAMEBOY ;
	if (strcasecmp("SUSCRIPTOR",comando) == 0 ) return GBSUSCRIPTOR ;
	return false;

}
