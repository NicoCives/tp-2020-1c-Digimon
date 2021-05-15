/*
 * GameCard.h
 *
 *  Created on: 11 abr. 2020
 *      Author: utnso
 */

#ifndef GAMECARD_H_
#define GAMECARD_H_
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>

typedef struct{
	int id_msj;
	int id_cola_msj;
	t_list* ALGORITMO_MEMORIA;
	char* ALGORITMO_REEMPLAZO;
	char* ALGORITMO_PARTICION_LIBRE;
	char* IP_BROKER;
	int PUERTO_BROKER;
	int FRECUENCIA_COMPACTACION;
	char* LOG_FILE;
} Mensaje;

typedef struct{

}Suscriptor;


void leer_configFile();
void iniciar_log();
void iniciar_servicio_broker();
void esperar_conexion(int servidor);
void atender(int socket);
void iniciar_estructuras();
void reservar_particion(int tamano, Mensaje msj);
void reservar_particion_dinamica(int tamano, Mensaje mensaje);
void reservar_particion_bs(int tamano, Mensaje mensaje);
void compactacion();
void eliminar_particion();



#endif /* GAMECARD_H_ */
