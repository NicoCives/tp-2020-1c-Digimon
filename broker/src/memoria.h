/*
 * memoria.h
 *
 *  Created on: 16 jul. 2020
 *      Author: utnso
 */

#ifndef SRC_MEMORIA_H_
#define SRC_MEMORIA_H_

#include "Broker.h"
#include "generales_broker.h"


void deserealizar_mem_NEW_POKEMON (void * laParti , cola_NEW_POKEMON * new_poke );
void deserealizar_mem_APPEARED_POKEMON (void * laParti ,  cola_APPEARED_POKEMON * app_poke) ;
void deserealizar_mem_CATCH_POKEMON (void * laParti , cola_CATCH_POKEMON* cat_poke);
void deserealizar_mem_CAUGHT_POKEMON (void * laParti ,cola_CAUGHT_POKEMON* cau_poke) ;
void deserealizar_mem_GET_POKEMON (void * laParti ,cola_GET_POKEMON * get_poke) ;
void deserealizar_mem_LOCALIZED_POKEMON (void * laParti , cola_LOCALIZED_POKEMON * loc_poke);

#endif /* SRC_MEMORIA_H_ */
