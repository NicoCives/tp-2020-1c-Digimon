#ifndef SRC_PROTOCOLOS_COMUNICACION_H_
#define SRC_PROTOCOLOS_COMUNICACION_H_

#include "sockets.h"
#include <commons/log.h>
#include "mensajeria.h"
#include <commons/string.h>
#include <locale.h>
#include <pthread.h>

typedef enum {
	NEW_POKEMON = 1,
	APPEARED_POKEMON,
	CATCH_POKEMON,
	CAUGHT_POKEMON,
	GET_POKEMON,
	LOCALIZED_POKEMON,
	ACK,
	SUSCRIPCION,
	FIN_DEL_PROTOCOLO,
	CACHE
} protocolo;

typedef enum {
	GAMECARD = 1,
	BROKER,
	TEAM,
	GAMEBOY,
	GBSUSCRIPTOR
} modulos;

void * buffer ;

pthread_mutex_t mxBuffer;

int mySocket ;

int validar_recive(int status, int modo);
int validar_servidor_o_cliente(char *id , char* mensajeEsperado,t_log* logger);
int validar_conexion(int ret, int modo,t_log* logger);

int handshake_servidor (int sockClienteDe, char *mensajeEnviado , char *mensajeEsperado,t_log* logger);
int handshake_cliente (int sockClienteDe, char *mensajeEnviado , char *mensajeEsperado,t_log* logger) ;
int conectarCon(int fdServer , char * ipServer , int portServer,t_log* logger);

void * serealizar(int head, void * mensaje ,  int tamanio);

void deserealizar_NEW_POKEMON (int head, void * buffer, int tamanio , cola_NEW_POKEMON * new_poke);
void deserealizar_APPEARED_POKEMON (int head, void * buffer, int tamanio , cola_APPEARED_POKEMON * app_poke) ;
void deserealizar_CATCH_POKEMON (int head, void * buffer, int tamanio , cola_CATCH_POKEMON* cat_poke);
void deserealizar_CAUGHT_POKEMON (int head, void * buffer, int tamanio , cola_CAUGHT_POKEMON* cau_poke) ;
void deserealizar_GET_POKEMON (int head, void * buffer, int tamanio , cola_GET_POKEMON * get_poke) ;
void deserealizar_LOCALIZED_POKEMON (int head, void * buffer, int tamanio , cola_LOCALIZED_POKEMON * loc_poke);
void deserealizar_suscriptor (int head, void * buffer, int tamanio , suscriptor * suscriptor);
void deserealizar_ACK(int head, void * buffer, int tamanio , respuesta_ACK * ack );

int calcularTamanioMensaje(int head, void* mensaje);

int recibirProtocolo(int * head , int * bufferTam ,int fdEmisor );
int recibirMensaje(int fdEmisor , int bufferTam , void * mensaje );

int conectar_enviar_recibir(char * modulo , char * ipServer , int puertoServer, char *handShake , char * handShakeEsperado ,int head, void *mensaje , t_log * logger ,t_log * loggerCatedra );

//void * recibirProtocoloSinEspera(int * head , int * bufferTam ,int fdEmisor );
//void recibirMensajeSinEspera(int fdEmisor , int bufferTam , void * mensaje );

int aplicar_protocolo_enviar(int fdReceptor, int head, void *mensaje);

int conectar_y_enviar(char * modulo , char * ipServer , int puertoServer, char *handShake , char * handShakeEsperado ,int head, void *mensaje , t_log * logger ,t_log * loggerCatedra ) ;

#endif /* SRC_PROTOCOLOS_COMUNICACION_H_ */
