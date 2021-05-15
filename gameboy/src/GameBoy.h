/*
 * GameBoy.h
 *
 *  Created on: 11 abr. 2020
 *      Author: utnso
 */

#ifndef GAMEBOY_H_
#define GAMEBOY_H_

#include "Generales.h"
#include <time.h>


int gameCard( char * comando,int argc, char *argv[]);
int team( char * comando,int argc, char *argv[]);
int broker( char * comando,int argc, char *argv[]);
int prc_suscriptor( char * comando,int argc, char *argv[]);


int prc_get_pokemon( char * comando,int argc, char *argv[] , char * modulo , char * ipServer , int puertoServer , char * Hand , char * HandEsperado );
int prc_caught_pokemon( char * comando,int argc, char *argv[] , char * modulo , char * ipServer , int puertoServer , char * Hand , char * HandEsperado );
int prc_catch_pokemon( char * comando,int argc, char *argv[] , char * modulo , char * ipServer , int puertoServer , char * Hand , char * HandEsperado );
int prc_appeared_pokemon( char * comando,int argc, char *argv[] , char * modulo , char * ipServer , int puertoServer , char * Hand , char * HandEsperado );
int prc_new_pokemon( char * comando,int argc, char *argv[] , char * modulo , char * ipServer , int puertoServer , char * Hand , char * HandEsperado );
int prc_localized_pokemon( char * comando,int argc, char *argv[] , char * modulo , char * ipServer , int puertoServer , char * Hand , char * HandEsperado );

void comandoNoR();

void catch(cola_CATCH_POKEMON * cat_poke , int idMsj,int argc, char *argv[] ) ;
void new(cola_NEW_POKEMON * new_poke , int idMsj,int argc, char *argv[] );
void get(cola_GET_POKEMON * get_poke , int idMsj,int argc, char *argv[] );
void localized(cola_LOCALIZED_POKEMON * loc_poke , int idMsj,int argc, char *argv[] );
void appeared(cola_APPEARED_POKEMON * app_poke , int idMsj,int argc, char *argv[] );


#endif /* GAMEBOY_H_ */



