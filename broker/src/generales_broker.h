/*
 * generales.h
 *
 *  Created on: 29 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_GENERALES_BROKER_H_
#define SRC_GENERALES_BROKER_H_

#include <locale.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <pthread.h>
#include <limits.h>
#include <semaphore.h>

#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>

#include "../digiCommons/src/protocolos_comunicacion.h"
#include "../digiCommons/src/generales.h"
#include "../digiCommons/src/sockets.h"
#include "../digiCommons/src/mensajeria.h"

#define FALSE 0
#define TRUE 1
#define ERROR -1

#define LOG_PATH "../brokerCatedra.log"
#define LOG_PATH_INTERNO "../broker.log"

#define CONFIG_BROKER "../configBroker.txt"

#define CONEXIONES_PERMITIDAS 100

typedef struct ConfigFile{
	int TAMANO_MEMORIA;
	int TAMANO_MINIMO_PARTICION;
	char* ALGORITMO_MEMORIA;
	char* ALGORITMO_REEMPLAZO;
	char* ALGORITMO_PARTICION_LIBRE;
	char* IP_BROKER;
	int PUERTO_BROKER;
	int FRECUENCIA_COMPACTACION;
} ConfigFile;

int TAMANIO_EXTRA_MSJ ;

typedef struct {
	suscriptor * laSus;
	int suSocket;
} losSuscriptores;

//int posicion_puntero_fifo;
//int contadorHilos;

ConfigFile* config_File;

t_log* logger;
t_log* loggerCatedra;

pthread_mutex_t mxHilos;
pthread_mutex_t mxSocketsFD;

pthread_t hilo_servidor;
pthread_t hilo_consola;
pthread_t hilo_Publisher;
//


fd_set setMaestro;

int fdBroker; //Socket Base
int comandoNuevo; // Socket de Escucha

int frecuencia_compactacion;
int32_t id_msj;
int32_t id_tracking;
//int contador_msjs_en_cola;
//int puntero_reemplazo;
//int cantidad_liberaciones;

int pid;

//_Bool compacte;
_Bool reenvieMsj;
//int cantidad_particiones_liberadas;

losSuscriptores * suscripcionC ;

//int  msj_a_enviar(int suSocket , int head , void * mensaje);


void * memoria_cache;

//SEMAFOROS CONTADOR

sem_t sem_contador_msjs_cola;

//SEMAFOROS MUTEX

pthread_mutex_t mutex_memoria_cache;
pthread_mutex_t mutex_id_msj;
pthread_mutex_t mutex_contador_msjs_cola;

pthread_mutex_t mutex_socket;

pthread_mutex_t mutex_suscripcion;

pthread_mutex_t mutex_suscriptores_new_pokemon;
pthread_mutex_t mutex_suscriptores_localized_pokemon;
pthread_mutex_t mutex_suscriptores_get_pokemon;
pthread_mutex_t mutex_suscriptores_appeared_pokemon;
pthread_mutex_t mutex_suscriptores_catch_pokemon;
pthread_mutex_t mutex_suscriptores_caught_pokemon;
pthread_mutex_t mutex_lista_suscritores;
pthread_mutex_t mutex_lista_ack;

pthread_mutex_t mutex_puntero_reemplazo;

pthread_mutex_t mutex_cola_new_pokemon;
pthread_mutex_t mutex_cola_localized_pokemon;
pthread_mutex_t mutex_cola_get_pokemon;
pthread_mutex_t mutex_cola_appeared_pokemon;
pthread_mutex_t mutex_cola_catch_pokemon;
pthread_mutex_t mutex_cola_caught_pokemon;

pthread_mutex_t mutex_lista_particiones;
pthread_mutex_t mutex_posicion_puntero_fifo;

pthread_mutex_t mutex_logs;
pthread_mutex_t desserializar;

pthread_mutex_t msjPendientesCaught;


//t_list* lista_msjs;
t_list* lista_particiones;
t_list* lista_nodos_particiones;
t_list* lista_ack;

t_list* suscriptores_new_pokemon;
t_list* suscriptores_localized_pokemon;
t_list* suscriptores_get_pokemon;
t_list* suscriptores_appeared_pokemon;
t_list* suscriptores_catch_pokemon;
t_list* suscriptores_caught_pokemon;

t_list* cola_new_pokemon;
t_list* cola_localized_pokemon;
t_list* cola_get_pokemon;
t_list* cola_appeared_pokemon;
t_list* cola_catch_pokemon;
t_list* cola_caught_pokemon;

//t_list * caught_pokemon_pendientes;
//t_list * appreared_pokemon_pendientes;
//t_list * localized_pokemon_pendientes;



//void* reservarMemoria(int size);
void leerArchivoDeConfiguracion(char *ruta,t_log * logger);
void iniciar_log();
void iniciar_estructuras();
void* reservarMemoria(int size);

//FUNCIONES HILOS
void consola();
void servidor();
void crearHilosBroker();
void publisher();
int thread_Broker(int fdCliente);

//FUNCIONES MSJ
void reenviarMsjs_Cola(int head, t_list * lista_Msjs_Cola, t_list * lista_de_suscriptores);
void suscribirse(losSuscriptores * suscp);
void agregar_contador_msj();
void reenviarMsjCache(losSuscriptores * laSus);
//void obtener_msj(int id_msj , Mensaje * msj);
int32_t obtener_idMsj();
_Bool buscarEnLista( t_list * lista , suscriptor * buscado ) ;
void procesarMsj(void * mensaje, int fdCliente, int head , int bufferTam );
void liberarRecursos();
int32_t obtener_idtracking();

void envidoDesdeCache(void * laParti , int colaAsignada , int id_msj , losSuscriptores *laSus);


#endif /* SRC_GENERALES_BROKER_H_ */

