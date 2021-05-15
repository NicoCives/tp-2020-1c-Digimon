
#ifndef INICIALIZACION_H_
#define INICIALIZACION_H_
#include "GameCard.h"
#include <commons/config.h>
#include <commons/string.h>
#include <commons/bitarray.h>
#include <sys/types.h>
#include <dirent.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <pthread.h>
#include "src/protocolos_comunicacion.h"
#include "src/mensajeria.h"




#define CONFIG_PATH "./config.txt"
#define LOG_PATH "./GAMECARD.log"
#define CONFIG_PATH_METADATA "../configGBM.txt"
#define CONEXIONES_PERMITIDAS 100


#define SUSCRIBIR 2
#define RECIBIR 3
#define ESCUCHANDO 4
#define INIT 5
#define ESPERA_ACK 6
//codigos de error

#define KEY_HANDSHAKE "GameCard"

typedef struct{
	int TIEMPO_DE_REINTENTO_CONEXION;
	int TIEMPO_DE_REINTENTO_OPERACION;
	int TIEMPO_RETARDO_OPERACION;
	char* PUNTO_MONTAJE_TALLGRASS;
	char* IP_BROKER;
	int PUERTO_BROKER;
	int PUERTO_GAMECARD;
	unsigned int TOKEN;
} ConfigFile;

typedef struct{
	  	int tamanio_bloques;
	 	char* magic_number;
	 	int cantidad_bloques;
}t_config_MetaData;



typedef struct{
	char* PUNTOMONTAJE;
	char* METADATA;
	char* METADATA_FILE;
	char* BITMAP;
	char* BLOCKS;
	char* FILES;

}t_config_PuntosMontaje;

typedef struct{
	char* pokemon;
	int idPokemon;
}t_Pokemones;


void leer_configFile(char* ruta);
void crearPuntoMontaje();
void consola();
void suscribir();
void suscribir_COLA_NEW();
void suscribir_COLA_CATCH();
void suscribir_COLA_GET();
void servidor();
void cargarArbolDirectorios(char* Directorio);
void thread_GameBoy(int fdSocket);
void thread_GetPokemon(cola_GET_POKEMON* get_poke);
void thread_CatchPokemon(cola_CATCH_POKEMON* catch_poke);
void thread_NewPokemon(cola_NEW_POKEMON* new_poke);

ConfigFile* config_File;
t_config_MetaData* config_MetaData;
t_config_PuntosMontaje* PuntoMontaje;

t_list* pokeList;
t_list* dirList;
t_log* logger;
t_bitarray *bitarray;
pthread_t hilo_servidor;
pthread_t hilo_suscribir;
pthread_t hilo_consola;
pthread_t hilo_New;
pthread_t hilo_Catch;
pthread_t hilo_Get;

pthread_mutex_t mxHilos;
pthread_mutex_t mxSocketsFD;
pthread_mutex_t h_reconectar;
pthread_mutex_t *mxPokemones;
pthread_mutex_t mxPokeList;
pthread_mutex_t mxLog;

pthread_rwlock_t mxBitmap;
pthread_mutex_t mxNewPokemonsList;


int fdBroker;
int cantFiles;
int comandoIn;
int comandoBroker;
int HANDSHAKE_SIZE;
fd_set setMaestro;


#endif /* INICIALIZACION_H_ */
