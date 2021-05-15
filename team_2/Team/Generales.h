#ifndef GENERALES_H
#define GENERALES_H

#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <signal.h>
#include <pthread.h>
#include <limits.h>
#include <semaphore.h>
#include "../digiCommons/src/protocolos_comunicacion.h"
#include <math.h>
#include "../digiCommons/src/mensajeria.h"

#define LOG_PATH "./CatedraTeam.log"
#define LOG_PATH_INTERNO "./Team.log"
#define RUTA_CONFIG_TEAM "./configTeam.txt"
#define FALSE 0
#define TRUE 1
#define OK 1
#define ERROR -1
#define CONEXIONES_PERMITIDAS 100


t_log* logger;
t_log* loggerCatedra;
t_list* objetivoTeam;
t_list* colaNew;
t_list* colaReady;
t_list* colaBlocked;
t_list* colaExit;
t_list* entrenadoresEnDeadlock;
t_list* listaCatchPokemon;
t_list* pokemonesBuscados;
t_list* pokemonesEjecutando;

t_config *config;
int EXIT_TEAM;
int fdBroker;
int ciclosEnCPU;
int fdTeam;
int fdBroker;
//int conBroker;
int idMensaje;
int crearLista;
int cantEntrenadores;
int cantCambiosContexto;
int cantDeadlocks;
int getEnviado;

pthread_t hilo_servidor;
pthread_t hilo_gameboy;
pthread_t hilo_consola;
pthread_t hilo;
pthread_t hilo_conexion;
pthread_t hilo_ejecucion;

pthread_t hilo_APPEARED;
pthread_t hilo_LOCALIZED;
pthread_t hilo_CAUGHT;

pthread_mutex_t mxHilos;
pthread_mutex_t mxSocketsFD;
pthread_mutex_t h_reconectar;
pthread_mutex_t mutexLog;
pthread_mutex_t mutexLogCatedra;
pthread_mutex_t mutex_idMensaje;
pthread_mutex_t mutex_ejecutar;
pthread_mutex_t entrenadores;
pthread_mutex_t mutexColaReady;
pthread_mutex_t mutexColaBlocked;
pthread_mutex_t mutexColaExit;
pthread_mutex_t mutexColaNew;
pthread_mutex_t mutexBuscados;
pthread_mutex_t mxListaEjecutando;
pthread_mutex_t mxListaCatch;
pthread_mutex_t mutexBuscados;
pthread_mutex_t mxEjecutando;
pthread_mutex_t mxPokeEjecutando;
pthread_mutex_t* mxEntrenadores;
pthread_mutex_t mutexObjetivoTeam;
pthread_mutex_t mutexListaCatch;
pthread_mutex_t mxEntrenadoresList;
pthread_mutex_t mxEntrenadoresDeadLock;
pthread_mutex_t mxCiclosCPU;
sem_t semEntrenadores;
sem_t semPokemonesBuscados;
sem_t elementosEnReady;
sem_t entrenadoresLibres;

typedef struct archivoConfigFile {
	t_list* posicionEntrenadores;
	t_list* pokemonEntrenadores;
	t_list* objetivosEntrenadores;
	int tiempoReconexion;
	int retardoCicloCPU;
	char* algoritmoPlanificacion;
	int quantum;
	char* ipBroker;
	int estimacionInicial;
	int puertoBroker;
	int puertoTeam;
	char* logFile;
	double alpha;
	unsigned int token;
} archivoConfigFile;

archivoConfigFile *configFile;

typedef struct posicionPokemon {
	char* tipoPokemon;
	t_list* posiciones;

} posicionPokemon;

typedef struct pokemon {
	int id_mensaje;
	char* nombre_pokemon;
	int tamanio_nombre;
	int posicion_x;
	int posicion_y;
}pokemon;

typedef struct entrenadorPokemon {
	int idEntrenador;
	int posicion_x;
	int posicion_y;
	t_list* pokemonesAtrapados;
	t_list* pokemonesObjetivo;
	int ciclosEnCPU;
	char* proximaAccion;
	pthread_mutex_t semaforMutex;
	int idMsjEsperado;
	double estimacionUltimaRafaga;
} entrenadorPokemon;

entrenadorPokemon* exec;
char ** aux ;

void leerArchivoDeConfiguracion(char *ruta);
void capturarError(int signal);
void * reservarMemoria(int size);
void inicializar_semaforos();
char* obtenerElementoPorPosicion(char* array, int posicionEnArray, char* corte);
void quitarPokemonDeAtrapados(entrenadorPokemon* entrenador, char* pokemon);
void verificarIntercambios();
char* obtenerPokemonObjetivoFaltante(entrenadorPokemon* entrenador);
char* obtenerPokemonAtrapadoInnecesario(entrenadorPokemon* entrenador);
void quitarDeColaBlocked(entrenadorPokemon* entrenador);
void realizarAccion(entrenadorPokemon* entrenador, int tiempo);
void servidor();
void consola();
int thread_Team(int fdCliente);
void crearHilos();
void planificador_Broker();
void suscripcion_APPEARED_POKEMON();
void suscripcion_LOCALIZED_POKEMON();
void suscripcion_CAUGHT_POKEMON();
void sendACK( int idMsj);


void planificador_GameBoy();
void thread_NewGameboy(int comandoNuevo);
void reconectar();
void thread_Entrenador(int idEntrenador);
void iniciar_log();
void iniciar_logCatedra();
//void crearLogger(char * nombre , char * path);
void obtenerEntrenadores();
void planificador();
void crearEstructuras();
void seleccionarEntrenadorMasCercano(posicionPokemon *pokemonAparecido, posicion* pos, entrenadorPokemon* proximoEntrenadorEnEjecutar);
void catchPokemon(entrenadorPokemon* entrenador, char* nombrePokemon, int posicionX, int posicionY);
int verificarMensajeRecibido (int idMensajeRecibido, entrenadorPokemon* entrenador);
void pokemonAtrapado(entrenadorPokemon* entrenador, cola_CAUGHT_POKEMON* pokemon);
void pokemonAtrapadoSinConexion(entrenadorPokemon* entrenador, cola_CATCH_POKEMON* pokemonRecibido);
void planificador();
int moverEntrenador(entrenadorPokemon* entrenador, int posicionXDestino, int posicionYDestino);
double calcularRafagaCPU(entrenadorPokemon* entrenador);
int pokemonNecesario(char* pokemonAparecido);
void getPokemon();
void threadAppeared(cola_APPEARED_POKEMON* app_poke);
void threadCaught(cola_CAUGHT_POKEMON* caug_poke);
void threadLocalized(cola_LOCALIZED_POKEMON* loc_poke);
int mensajeNoRecibido(cola_LOCALIZED_POKEMON* pokemonLocalizado);
void grabarToken(unsigned int token);

#endif
