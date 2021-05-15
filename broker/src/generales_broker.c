/*
 * generales.c
 *
 *  Created on: 29 abr. 2020
 *      Author: utnso
 */
#include "generales_broker.h"
#include "memoria.h"
#include "Broker.h"

void iniciar_estructuras(){

	//SE RESERVA LA MEMORIA TOTAL DEL BROKER

	memoria_cache = malloc(config_File->TAMANO_MEMORIA);
	memset(memoria_cache, '\0', config_File->TAMANO_MEMORIA);

	//VARIABLES GLOBALES

	frecuencia_compactacion = config_File->FRECUENCIA_COMPACTACION;
	id_msj = 0;
	id_tracking=9000;
	TAMANIO_EXTRA_MSJ = sizeof(uint32_t) + sizeof(uint32_t) ;

	//SE CREAN LAS LISTAS PARA LA ADMINISTRACION DE MEMORIA Y MSJS

	lista_particiones = list_create();
	lista_ack = list_create();

	//SE CREA LAS LISTAS DE LOS SUSCRIPTORES POR COLA

	suscriptores_new_pokemon = list_create();
	suscriptores_localized_pokemon = list_create();
	suscriptores_get_pokemon = list_create();
	suscriptores_appeared_pokemon = list_create();
	suscriptores_catch_pokemon = list_create();
	suscriptores_caught_pokemon = list_create();

	//SE CREAN LAS LISTAS PARA LAS COLAS DE MSJS

	cola_new_pokemon = list_create();
	cola_localized_pokemon = list_create();
	cola_get_pokemon = list_create();
	cola_appeared_pokemon = list_create();
	cola_catch_pokemon = list_create();
	cola_caught_pokemon = list_create();


	//CREO LA PARTICION INICIAL QUE CONTENGA TODA LA MEMORIA

	if(strcmp(config_File->ALGORITMO_MEMORIA, "PARTICIONES") == 0){

		Particion * particion_memoria = malloc(sizeof(Particion));

		particion_memoria->id_msj= 0;
		particion_memoria->punteroInicial = 0;
		particion_memoria->colaAsignada = 0;
		particion_memoria->punteroFinal = config_File->TAMANO_MEMORIA - 1;
		particion_memoria->tamano = config_File->TAMANO_MEMORIA;
		particion_memoria->libre = true;
		particion_memoria->tiempoLRU = 0;

		list_add(lista_particiones, particion_memoria);

	}else if(strcmp(config_File->ALGORITMO_MEMORIA, "BS") == 0){

		Particion_bs * particion_memoria_bs = malloc(sizeof(Particion_bs));

		particion_memoria_bs->esPadre = false;
		particion_memoria_bs->id_msj= 0;
		particion_memoria_bs->punteroInicial = 0;
		particion_memoria_bs->colaAsignada = 0;
		particion_memoria_bs->punteroFinal = config_File->TAMANO_MEMORIA - 1;
		particion_memoria_bs->tamano = config_File->TAMANO_MEMORIA;
		particion_memoria_bs->libre = true;
		particion_memoria_bs->tiempoLRU = 0;

		list_add(lista_particiones, particion_memoria_bs);
	}
}

void iniciar_semaforos()
{
	sem_init(&sem_contador_msjs_cola,0,1);
	//SE DEFINE MUTEX PARA DUMP DE MEMORIA CACHE
	pthread_mutex_init(&mutex_memoria_cache, NULL);
	pthread_mutex_init(&mutex_puntero_reemplazo, NULL);
	//SE DEFINE MUTEX PARA LA LISTA DE PARTICIONES
	pthread_mutex_init(&mutex_lista_particiones, NULL);
	//SE DEFINE MUTEX PARA VARIABLE DEL TIPO PRODUCTOR-CONSUMIDOR
	pthread_mutex_init(&mutex_id_msj, NULL);
	//SE DEFINE MUTEX PARA LA LISTA DE SUSCRIPTORES
	pthread_mutex_init(&mutex_suscriptores_new_pokemon, NULL);
	pthread_mutex_init(&mutex_suscriptores_localized_pokemon, NULL);
	pthread_mutex_init(&mutex_suscriptores_get_pokemon, NULL);
	pthread_mutex_init(&mutex_suscriptores_appeared_pokemon, NULL);
	pthread_mutex_init(&mutex_suscriptores_catch_pokemon, NULL);
	pthread_mutex_init(&mutex_suscriptores_caught_pokemon, NULL);
	pthread_mutex_init(&mutex_lista_suscritores, NULL);
	pthread_mutex_init(&mutex_lista_ack, NULL);
	//SE DEFINE MUTEX PARA LAS COLAS DE MSJS
	pthread_mutex_init(&mutex_contador_msjs_cola, NULL);
	pthread_mutex_init(&mutex_cola_new_pokemon, NULL);
	pthread_mutex_init(&mutex_cola_localized_pokemon, NULL);
	pthread_mutex_init(&mutex_cola_get_pokemon, NULL);
	pthread_mutex_init(&mutex_cola_appeared_pokemon, NULL);
	pthread_mutex_init(&mutex_cola_catch_pokemon, NULL);
	pthread_mutex_init(&mutex_cola_caught_pokemon, NULL);
	pthread_mutex_init(&mutex_suscripcion, NULL);

	//SE DEFINE MUTEX PARA LOS HILOS
	pthread_mutex_init(&mxHilos, NULL);
	pthread_mutex_init(&mxSocketsFD, NULL);

	pthread_mutex_init(&mxBuffer, NULL);
	pthread_mutex_init(&mutex_logs, NULL);
	pthread_mutex_init(&desserializar, NULL);

	pthread_mutex_init(&msjPendientesCaught, NULL);


}

void crearHilosBroker() {


	hilo_servidor= 0;
	hilo_consola= 0;
	hilo_Publisher = 0;
	//hilo=0;

	pthread_create(&hilo_servidor, NULL, (void*) servidor, NULL);
	pthread_create(&hilo_consola, NULL, (void*) consola, NULL);
	pthread_create(&hilo_Publisher, NULL, (void*) publisher, NULL);

	pthread_join(hilo_servidor, NULL);
	pthread_join(hilo_Publisher, NULL);
	pthread_join(hilo_consola, NULL);

}
/*
void* reservarMemoria(int size) {

		void *puntero = malloc(size);
		if(puntero == NULL) {
			fprintf(stderr, "Error al reservar %d bytes de memoria", size);
			exit(ERROR);
		}
		return puntero;
}
*/

void liberarRecursos(){




	//pthread_mutex_lock(&mutex_suscripcion);

	//pthread_mutex_unlock(&mutex_suscripcion);


	//pthread_mutex_lock(&mutex_lista_ack);
	int tamLista = list_size(lista_ack) - 1 ;

	for(int i=0 ; i <= tamLista ; i++){
	  respuesta_ACK * elack = list_get(lista_ack,0);
	  list_remove(lista_ack,0);
	  free(elack);
	}


	list_destroy(lista_ack);
	//pthread_mutex_unlock(&mutex_lista_ack);


	//pthread_mutex_lock(&mutex_lista_particiones);

	tamLista = list_size(lista_particiones) - 1 ;
	if(strcmp(config_File->ALGORITMO_MEMORIA, "PARTICIONES") == 0){
		for(int i=0 ; i <= tamLista ; i++){
			  Particion * laParti = list_get(lista_particiones,0);
			  list_remove(lista_particiones,0);
			  free(laParti);
		}
		}else if(strcmp(config_File->ALGORITMO_MEMORIA, "BS") == 0){
			for(int i=0 ; i <= tamLista ; i++){
			  Particion_bs * laParti = list_get(lista_particiones,0);
			  list_remove(lista_particiones,0);
			  free(laParti);
			}
		}

	list_destroy(lista_particiones);

	//pthread_mutex_unlock(&mutex_lista_particiones);

	pthread_mutex_lock(&mutex_cola_appeared_pokemon);
	list_destroy(cola_appeared_pokemon);
	//pthread_mutex_unlock(&mutex_cola_appeared_pokemon);

	//pthread_mutex_lock(&mutex_cola_catch_pokemon);
	list_destroy(cola_catch_pokemon);
	//pthread_mutex_unlock(&mutex_cola_catch_pokemon);

	//pthread_mutex_lock(&mutex_cola_caught_pokemon);
	list_destroy(cola_caught_pokemon);
	//pthread_mutex_unlock(&mutex_cola_caught_pokemon);

	//pthread_mutex_lock(&mutex_cola_get_pokemon);
	list_destroy(cola_get_pokemon);
	//pthread_mutex_unlock(&mutex_cola_get_pokemon);

	//pthread_mutex_lock(&mutex_cola_localized_pokemon);
	list_destroy(cola_localized_pokemon);
	//pthread_mutex_unlock(&mutex_cola_localized_pokemon);

	//pthread_mutex_lock(&mutex_cola_new_pokemon);
	list_destroy(cola_new_pokemon);
	//pthread_mutex_unlock(&mutex_cola_new_pokemon);

	//pthread_mutex_lock(&mutex_suscriptores_appeared_pokemon);
	tamLista = list_size(suscriptores_appeared_pokemon) - 1 ;

	for(int i=0 ; i <= tamLista ; i++){
		  losSuscriptores * suscrip = list_get(suscriptores_appeared_pokemon,0);

		  suscriptor * elSus = suscrip->laSus ;

		  int laOtraLista = list_size(elSus->cola_a_suscribir);

		  for(int j=0 ; j < laOtraLista ; j++){
			  list_remove(elSus->cola_a_suscribir,0);
		  }

		  list_destroy(elSus->cola_a_suscribir);
		  free(elSus);
		  list_remove(suscriptores_appeared_pokemon,0);
		  free(suscrip);
	}
	list_destroy(suscriptores_appeared_pokemon);
	//pthread_mutex_unlock(&mutex_suscriptores_appeared_pokemon);

	//pthread_mutex_lock(&mutex_suscriptores_catch_pokemon);
	tamLista = list_size(suscriptores_catch_pokemon) - 1 ;

	for(int i=0 ; i <= tamLista ; i++){
		  losSuscriptores * suscrip = list_get(suscriptores_catch_pokemon,0);

		  suscriptor * elSus = suscrip->laSus ;

		  int laOtraLista = list_size(elSus->cola_a_suscribir);

		  for(int j=0 ; j < laOtraLista ; j++){
			  list_remove(elSus->cola_a_suscribir,0);
		  }

		  list_destroy(elSus->cola_a_suscribir);
		  free(elSus);
		  list_remove(suscriptores_catch_pokemon,0);
		  free(suscrip);
	}
	list_destroy(suscriptores_catch_pokemon);
	//pthread_mutex_unlock(&mutex_suscriptores_catch_pokemon);

	//pthread_mutex_lock(&mutex_suscriptores_caught_pokemon);
	tamLista = list_size(suscriptores_caught_pokemon) - 1 ;

	for(int i=0 ; i <= tamLista ; i++){
		  losSuscriptores * suscrip = list_get(suscriptores_caught_pokemon,0);

		  suscriptor * elSus = suscrip->laSus ;

		  int laOtraLista = list_size(elSus->cola_a_suscribir);

		  for(int j=0 ; j < laOtraLista ; j++){
			  list_remove(elSus->cola_a_suscribir,0);
		  }

		  list_destroy(elSus->cola_a_suscribir);
		  free(elSus);
		  list_remove(suscriptores_caught_pokemon,0);
		  free(suscrip);
	}
	list_destroy(suscriptores_caught_pokemon);
	//pthread_mutex_unlock(&mutex_suscriptores_caught_pokemon);

	//pthread_mutex_lock(&mutex_suscriptores_get_pokemon);
	tamLista = list_size(suscriptores_get_pokemon) - 1 ;

	for(int i=0 ; i <= tamLista ; i++){
		  losSuscriptores * suscrip = list_get(suscriptores_get_pokemon,0);

		  suscriptor * elSus = suscrip->laSus ;

		  int laOtraLista = list_size(elSus->cola_a_suscribir);

		  for(int j=0 ; j < laOtraLista ; j++){
			  list_remove(elSus->cola_a_suscribir,0);
		  }

		  list_destroy(elSus->cola_a_suscribir);
		  free(elSus);
		  list_remove(suscriptores_get_pokemon,0);
		  free(suscrip);
	}
	list_destroy(suscriptores_get_pokemon);
	//pthread_mutex_unlock(&mutex_suscriptores_get_pokemon);

	//pthread_mutex_lock(&mutex_suscriptores_localized_pokemon);
	tamLista = list_size(suscriptores_localized_pokemon) - 1 ;

	for(int i=0 ; i <= tamLista ; i++){
		  losSuscriptores * suscrip = list_get(suscriptores_localized_pokemon,0);

		  suscriptor * elSus = suscrip->laSus ;

		  int laOtraLista = list_size(elSus->cola_a_suscribir);

		  for(int j=0 ; j < laOtraLista ; j++){
			  list_remove(elSus->cola_a_suscribir,0);
		  }

		  list_destroy(elSus->cola_a_suscribir);
		  free(elSus);
		  list_remove(suscriptores_localized_pokemon,0);
		  free(suscrip);
	}
	list_destroy(suscriptores_localized_pokemon);
	//pthread_mutex_unlock(&mutex_suscriptores_localized_pokemon);

	//pthread_mutex_lock(&mutex_suscriptores_new_pokemon);
	tamLista = list_size(suscriptores_new_pokemon) - 1 ;

	for(int i=0 ; i <= tamLista ; i++){
		  losSuscriptores * suscrip = list_get(suscriptores_new_pokemon,0);

		  suscriptor * elSus = suscrip->laSus ;

		  int laOtraLista = list_size(elSus->cola_a_suscribir);

		  for(int j=0 ; j < laOtraLista ; j++){
			  list_remove(elSus->cola_a_suscribir,0);
		  }

		  list_destroy(elSus->cola_a_suscribir);
		  free(elSus);
		  list_remove(suscriptores_new_pokemon,0);
		  free(suscrip);
	}
	list_destroy(suscriptores_new_pokemon);
	//pthread_mutex_unlock(&mutex_suscriptores_new_pokemon);

	pthread_mutex_lock(&mutex_memoria_cache);
	free(memoria_cache);
	//pthread_mutex_unlock(&mutex_memoria_cache);
}

void consola() {

	printf("Hola! Ingresá \"salir\" para finalizar módulo\n");
	size_t buffer_size = 100; //por el momento restringido a 100 caracteres
	char* comando = (char *) calloc(1, buffer_size);
	while (!string_equals_ignore_case(comando, "salir\n")) {
		printf(">");
		int bytes_read = getline(&comando, &buffer_size, stdin);
		if (bytes_read == -1) {
			log_error(logger,"Error en getline");
		}
		if (bytes_read == 1) {
			continue;
		}

		if(strcasecmp(comando,"dump\n") == 0)  kill(pid,SIGUSR1);
	}

	//pthread_mutex_lock(&mutex_logs);

	log_info(logger,"Comenzando a cerrar los hilos");

	//pthread_mutex_unlock(&mutex_logs);

	pthread_mutex_lock(&mxHilos);

	pthread_cancel(hilo_servidor);
	pthread_detach(hilo_servidor);

	pthread_cancel(hilo_Publisher);
	pthread_detach(hilo_Publisher);

	/*for(int i=0 ; i < contadorHilos ; i++){
		pthread_cancel(hilo);
		pthread_detach(hilo);
	}*/

	pthread_mutex_unlock(&mxHilos);

	log_info(logger,"Finalice todos los hilos");

	liberarRecursos();

	free(comando);

	log_destroy(logger);
	log_destroy(loggerCatedra);

	//free(loggerCatedra);
	//free(logger);

	free(config_File->ALGORITMO_MEMORIA);
	free(config_File->ALGORITMO_PARTICION_LIBRE);
	free(config_File->ALGORITMO_REEMPLAZO);
	free(config_File->IP_BROKER);
	free(config_File);

	pthread_detach( pthread_self() );
}

void servidor() {

	fdBroker = nuevoSocket();

	asociarSocket(fdBroker, config_File->PUERTO_BROKER);
	escucharSocket(fdBroker, CONEXIONES_PERMITIDAS);

	//pthread_mutex_lock(&mutex_logs);
	log_info(logger," Escuchando conexiones");
	//pthread_mutex_unlock(&mutex_logs);

	while(TRUE) {

		int conexionNueva = 0;

		while(conexionNueva == 0) {

			//pthread_mutex_lock(&mutex_socket);
			comandoNuevo = aceptarConexionSocket(fdBroker);
			//pthread_mutex_unlock(&mutex_socket);

			conexionNueva = handshake_servidor ( comandoNuevo,"Broker" , "Team",logger);

			if( ! validar_conexion(conexionNueva, 0,logger) ) {
					pthread_mutex_lock(&mxSocketsFD); //desbloquea el semaforo
					cerrarSocket(fdBroker);
					pthread_mutex_unlock(&mxSocketsFD);
			}
			pthread_mutex_lock(&mutex_logs);
			log_info(loggerCatedra,"Se conecto un Proceso al Broker");
			pthread_mutex_unlock(&mutex_logs);
		}
		pthread_mutex_lock(&mxHilos);
			pthread_t hilo;
			pthread_create(&hilo, NULL, (void*) thread_Broker,(int *) comandoNuevo);
			//contadorHilos++;
			//pthread_join(hilo,NULL);

		pthread_mutex_unlock(&mxHilos);

	}
}

int thread_Broker(int fdCliente) {

while(true){

	int head , bufferTam  ;

	int recibido = recibirProtocolo(&head,&bufferTam,fdCliente); // recibo head y tamaño de msj

	if (head < 1 || recibido <= 0){ // DESCONEXIÓN
		pthread_mutex_lock(&mxHilos);
		pthread_detach( pthread_self() );
		pthread_mutex_unlock(&mxHilos);
		//contadorHilos--;
		return false;
	}else{

						void * mensaje = malloc(bufferTam);

						recibirMensaje(fdCliente , bufferTam , mensaje ); // recibo msj serializado para el tratamiento deseado

						pthread_mutex_lock(&mutex_logs);
						log_info(logger,"aplicar_protocolo_recibir -> recibió el HEAD #%d",head);

						log_info(logger,"aplicar_protocolo_recibir -> recibió un tamaño de -> %d",bufferTam);

						log_info(logger,"aplicar_protocolo_recibir -> comienza a deserealizar");
						pthread_mutex_unlock(&mutex_logs);

							switch( head ){

							setlocale(LC_ALL,"");

										case NEW_POKEMON :{

											//cola_NEW_POKEMON  new_poke ;

											//pthread_mutex_lock(&desserializar);

											cola_NEW_POKEMON  * ptro_new_poke =  malloc(sizeof(cola_NEW_POKEMON));

											deserealizar_NEW_POKEMON ( head, mensaje, bufferTam, ptro_new_poke);

											//pthread_mutex_lock(&desserializar);

											pthread_mutex_lock(&mutex_logs);

											ptro_new_poke->id_mensaje = obtener_idMsj();
											ptro_new_poke->id_tracking = obtener_idtracking();

											log_info(loggerCatedra,"Recibí en la cola NEW_POKEMON . POKEMON: %s  , ID_TRACKING: %d , CANTIDAD: %d  , CORDENADA X: %d , CORDENADA Y: %d ",ptro_new_poke->nombre_pokemon,ptro_new_poke->id_tracking,ptro_new_poke->cantidad,ptro_new_poke->posicion_x,ptro_new_poke->posicion_y);

											pthread_mutex_unlock(&mutex_logs);



											guardar_msj(NEW_POKEMON, bufferTam - TAMANIO_EXTRA_MSJ, ptro_new_poke);

											pthread_mutex_lock(&mutex_cola_new_pokemon);
											list_add(cola_new_pokemon, ptro_new_poke);
											pthread_mutex_unlock(&mutex_cola_new_pokemon);

											respuesta_ACK * ack = malloc (sizeof(respuesta_ACK));
											ack->ack = TRUE;
											ack->id_msj = ptro_new_poke->id_tracking;
											ack->token = 0 ;
											aplicar_protocolo_enviar(fdCliente,ACK,ack);
											free(ack);

											sem_post(&sem_contador_msjs_cola);
											//agregar_contador_msj();
											break;
										}
										case CATCH_POKEMON :{

											//pthread_mutex_lock(&desserializar);

											cola_CATCH_POKEMON * cath_poke = malloc (sizeof(cola_CATCH_POKEMON));
											deserealizar_CATCH_POKEMON( head, mensaje, bufferTam, cath_poke);

											//pthread_mutex_unlock(&desserializar);

											pthread_mutex_lock(&mutex_logs);

											cath_poke->id_mensaje = obtener_idMsj();
											cath_poke->id_tracking = obtener_idtracking();

											log_info(loggerCatedra,"Recibí en la cola CATCH_POKEMON . POKEMON: %s  , ID_TRACKING: %d , CORDENADA X: %d , CORDENADA Y: %d ",cath_poke->nombre_pokemon,cath_poke->id_tracking,cath_poke->posicion_x,cath_poke->posicion_y);

											pthread_mutex_unlock(&mutex_logs);


											//GUARDAR O CACHEAR MSJ



											//pthread_mutex_lock(&msjPendientesCaught);
											//list_add(caught_pokemon_pendientes,cath_poke->id_mensaje);
											//pthread_mutex_unlock(&msjPendientesCaught);

											guardar_msj(CATCH_POKEMON, bufferTam - TAMANIO_EXTRA_MSJ, cath_poke);

											pthread_mutex_lock(&mutex_cola_catch_pokemon);
											list_add(cola_catch_pokemon, cath_poke);;
											pthread_mutex_unlock(&mutex_cola_catch_pokemon);

											respuesta_ACK * ack = malloc (sizeof(respuesta_ACK));
											ack->ack = TRUE;
											ack->id_msj = cath_poke->id_tracking;
											ack->token = 0 ;
											aplicar_protocolo_enviar(fdCliente,ACK,ack);
											free(ack);

											sem_post(&sem_contador_msjs_cola);
											//agregar_contador_msj();
											break;
										}
										case GET_POKEMON :{

											//pthread_mutex_lock(&desserializar);

											cola_GET_POKEMON * get_poke = malloc (sizeof(cola_GET_POKEMON));
											deserealizar_GET_POKEMON ( head, mensaje, bufferTam, get_poke);

											//pthread_mutex_unlock(&desserializar);

											get_poke->id_mensaje = obtener_idMsj();
											get_poke->id_tracking = obtener_idtracking();

											pthread_mutex_lock(&mutex_logs);

											log_info(loggerCatedra,"Recibí en la cola GET_POKEMON . POKEMON: %s, ID_TRACKING: %d ",get_poke->nombre_pokemon,get_poke->id_tracking);

											pthread_mutex_unlock(&mutex_logs);


											guardar_msj(GET_POKEMON, bufferTam - TAMANIO_EXTRA_MSJ, get_poke);

											pthread_mutex_lock(&mutex_cola_get_pokemon);
											list_add(cola_get_pokemon, get_poke);
											pthread_mutex_unlock(&mutex_cola_get_pokemon);

											respuesta_ACK * ack = malloc (sizeof(respuesta_ACK));
											ack->ack = TRUE;
											ack->id_msj = get_poke->id_tracking;
											ack->token = 0 ;
											aplicar_protocolo_enviar(fdCliente,ACK,ack);
											free(ack);

											sem_post(&sem_contador_msjs_cola);
											//agregar_contador_msj();
											break;
										}

										case APPEARED_POKEMON :{

											//pthread_mutex_lock(&desserializar);

											cola_APPEARED_POKEMON * app_poke = malloc (sizeof(cola_APPEARED_POKEMON));
											deserealizar_APPEARED_POKEMON ( head, mensaje, bufferTam, app_poke);

											//pthread_mutex_unlock(&desserializar);


											pthread_mutex_lock(&mutex_logs);

											app_poke->id_mensaje = obtener_idMsj();

											if (app_poke->id_tracking == 0 ) app_poke->id_tracking = obtener_idtracking();

											log_info(loggerCatedra,"Recibí en la cola APPEARED_POKEMON . POKEMON: %s  , ID_TRACKING: %d , CORDENADA X: %d , CORDENADA Y: %d ",app_poke->nombre_pokemon,app_poke->id_tracking,app_poke->posicion_x,app_poke->posicion_y);

											pthread_mutex_unlock(&mutex_logs);



											guardar_msj(APPEARED_POKEMON, bufferTam - TAMANIO_EXTRA_MSJ , app_poke);

											pthread_mutex_lock(&mutex_cola_appeared_pokemon);
											list_add(cola_appeared_pokemon, app_poke);
											pthread_mutex_unlock(&mutex_cola_appeared_pokemon);

											respuesta_ACK * ack = malloc (sizeof(respuesta_ACK));
																						ack->ack = TRUE;
																						ack->id_msj = app_poke->id_mensaje;
																						ack->token = 0 ;
																						aplicar_protocolo_enviar(fdCliente,ACK,ack);
																						free(ack);

											//agregar_contador_msj();
											sem_post(&sem_contador_msjs_cola);
											break;
										}

										case CAUGHT_POKEMON :{

											//pthread_mutex_lock(&desserializar);

											cola_CAUGHT_POKEMON * caug_poke = malloc (sizeof(cola_CAUGHT_POKEMON));
											deserealizar_CAUGHT_POKEMON ( head, mensaje, bufferTam, caug_poke);

											//pthread_mutex_unlock(&desserializar);

											pthread_mutex_lock(&mutex_logs);

											caug_poke->id_mensaje = obtener_idMsj();

											if (caug_poke->id_tracking == 0 ) caug_poke->id_tracking = obtener_idtracking();

											log_info(loggerCatedra,"Recibí en la cola CAUGHT_POKEMON . ID_TRACKING: %d , ATRAPO: %d",caug_poke->id_tracking,caug_poke->atrapo_pokemon);

											pthread_mutex_unlock(&mutex_logs);



											guardar_msj(CAUGHT_POKEMON, bufferTam - TAMANIO_EXTRA_MSJ , caug_poke);

											pthread_mutex_lock(&mutex_cola_caught_pokemon);
											list_add(cola_caught_pokemon, caug_poke);
											pthread_mutex_unlock(&mutex_cola_caught_pokemon);

											respuesta_ACK * ack = malloc (sizeof(respuesta_ACK));
											ack->ack = TRUE;
											ack->id_msj = caug_poke->id_mensaje;
											ack->token = 0 ;
											aplicar_protocolo_enviar(fdCliente,ACK,ack);
											free(ack);

											sem_post(&sem_contador_msjs_cola);
											//agregar_contador_msj();
											break;
										}

										case LOCALIZED_POKEMON :{

											//pthread_mutex_lock(&desserializar);

											cola_LOCALIZED_POKEMON * loc_poke = malloc (sizeof(cola_LOCALIZED_POKEMON));

											loc_poke->lista_posiciones = list_create();

											deserealizar_LOCALIZED_POKEMON ( head, mensaje, bufferTam, loc_poke);

											//pthread_mutex_unlock(&desserializar);

											pthread_mutex_lock(&mutex_logs);

											loc_poke->id_mensaje = obtener_idMsj();
											if (loc_poke->id_tracking == 0 ) loc_poke->id_tracking = obtener_idtracking();

											if(loc_poke->cantidad == 0){
												log_info(loggerCatedra,"Recibí en la cola LOCALIZED_POKEMON . POKEMON: %s  , ID_TRACKING: %d , sin posiciones y cantidad",loc_poke->nombre_pokemon,loc_poke->id_tracking);
											}
											else{
												for (int i = 0 ; i < loc_poke->cantidad; i++)
												{
													posicion * pos = list_get(loc_poke->lista_posiciones,i) ;

													log_info(loggerCatedra,"Recibí en la cola LOCALIZED_POKEMON . POKEMON: %s  , ID_TRACKING: %d , CANTIDAD: %d , POSICIÓN X: %d , POSICIÓN Y: %d",loc_poke->nombre_pokemon,loc_poke->id_tracking,loc_poke->cantidad,pos->posicion_x,pos->posicion_y);
												}
											}

											pthread_mutex_unlock(&mutex_logs);


											guardar_msj(LOCALIZED_POKEMON, bufferTam - TAMANIO_EXTRA_MSJ, loc_poke);
											pthread_mutex_lock(&mutex_cola_localized_pokemon);
											list_add(cola_localized_pokemon, loc_poke);
											pthread_mutex_unlock(&mutex_cola_localized_pokemon);

											respuesta_ACK * ack = malloc (sizeof(respuesta_ACK));
											ack->ack = TRUE;
											ack->id_msj = loc_poke->id_mensaje;
											ack->token = 0 ;
											aplicar_protocolo_enviar(fdCliente,ACK,ack);
											free(ack);

											sem_post(&sem_contador_msjs_cola);
											//agregar_contador_msj();
											//free(loc_poke->nombre_pokemon);
											//list_destroy(loc_poke->lista_posiciones);
											break;
										}

										case ACK :{

											//pthread_mutex_lock(&desserializar);

											respuesta_ACK * ack = malloc(sizeof(respuesta_ACK));
											deserealizar_ACK( head, mensaje, bufferTam, ack);

											//pthread_mutex_unlock(&desserializar);

											pthread_mutex_lock(&mutex_logs);

											log_info(loggerCatedra,"Recibí un ACK del token %d con los siguientes datos ESTADO: %d ID_TRACKING: %d ",ack->token,ack->ack,ack->id_msj);

											pthread_mutex_unlock(&mutex_logs);

											pthread_mutex_lock(&mutex_lista_ack);

											list_add(lista_ack,ack);

											pthread_mutex_unlock(&mutex_lista_ack);

											break;
										}

										case SUSCRIPCION :{

											pthread_mutex_lock(&mutex_suscripcion);

											suscripcionC = malloc(sizeof(losSuscriptores));
											suscripcionC->laSus = malloc(sizeof(suscriptor));
											suscripcionC->laSus->cola_a_suscribir = list_create();


											//pthread_mutex_lock(&mutex_socket);
											suscripcionC->suSocket = fdCliente;
											//pthread_mutex_unlock(&mutex_socket);

											deserealizar_suscriptor( head, mensaje, bufferTam, suscripcionC->laSus);

											pthread_mutex_lock(&mutex_logs);

											for ( int i = 0 ; i < list_size(suscripcionC->laSus->cola_a_suscribir) ; i++){
												log_info(loggerCatedra,"Recibí del modulo %s una suscribición a la cola %s con el token %d", (char*) devolverModulo(suscripcionC->laSus->modulo),tipoMsjIntoToChar( (int) list_get(suscripcionC->laSus->cola_a_suscribir,i)),suscripcionC->laSus->token);
											}

											pthread_mutex_unlock(&mutex_logs);

											suscribirse(suscripcionC);

											respuesta_ACK * ack = malloc (sizeof(respuesta_ACK));

											ack->ack = TRUE;
											ack->id_msj = obtener_idMsj();
											ack->token = 0 ;

											/*struct sockaddr_in peer;
											int peer_len = sizeof(peer);
											if (getpeername(fdCliente, &peer, &peer_len) != -1) {
											log_info(logger,"Cliente IP address is: %s", inet_ntoa(peer.sin_addr));
											log_info(logger,"Cliente port is: %d", (int) ntohs(peer.sin_port));
											 }*/
											//pthread_mutex_lock(&mutex_memoria_cache);
											aplicar_protocolo_enviar(fdCliente,ACK,ack);
											free(ack);
											//pthread_mutex_unlock(&mutex_memoria_cache);

											reenviarMsjCache(suscripcionC);

											pthread_mutex_unlock(&mutex_suscripcion);

											break;
										}
										default:

											pthread_mutex_lock(&mutex_logs);

											log_info(logger, "Instrucción no reconocida");

											pthread_mutex_unlock(&mutex_logs);

											break;
									}
							free(mensaje);
					}

	}

	pthread_mutex_lock(&mxHilos);
	pthread_detach( pthread_self() );
	//contadorHilos--;
	pthread_mutex_unlock(&mxHilos);
}


/*
void* reservarMemoria(int size) {
		void *puntero = malloc(size);
		if(puntero == NULL) {
			fprintf(stderr, "Error al reservar %d bytes de memoria", size);
			exit(ERROR);
		}
		return puntero;
}
*/

void leerArchivoDeConfiguracion(char *ruta,t_log * logger) {

	t_config *config;
	config = config_create(ruta);

	if (config != NULL) {

		log_info(logger, "Leyendo Archivo de Configuracion..");

		if (config_has_property(config, "TAMANO_MEMORIA")) {
			config_File->TAMANO_MEMORIA = config_get_int_value(config,
								"TAMANO_MEMORIA");
						log_info(logger,
								"Se encontró y cargó el contido del TAMANO_MEMORIA. Valor: %d",config_File->TAMANO_MEMORIA);
					} else {

						log_error(logger,
								"El archivo de configuracion no contiene el TAMANO_MEMORIA");

		}

		if (config_has_property(config, "TAMANO_MINIMO_PARTICION")) {
			config_File->TAMANO_MINIMO_PARTICION = config_get_int_value(config,
								"TAMANO_MINIMO_PARTICION");
						log_info(logger,
								"Se encontró y cargó el contido del TAMANO_MINIMO_PARTICION. Valor: %d",config_File->TAMANO_MINIMO_PARTICION);
					} else {

						log_error(logger,
								"El archivo de configuracion no contiene el TAMANO_MINIMO_PARTICION");

		}

		if (config_has_property(config, "ALGORITMO_MEMORIA")) {
					config_File->ALGORITMO_MEMORIA = strdup(
													config_get_string_value(config, "ALGORITMO_MEMORIA"));
											log_info(logger,
													"Se encontró y cargó el contenido de ALGORITMO_MEMORIA a la estructura. Valor: %s",config_File->ALGORITMO_MEMORIA);

				} else {
											log_error(logger,
													"El archivo de configuracion no contiene el ALGORITMO_MEMORIA");
		}

		if (config_has_property(config, "ALGORITMO_REEMPLAZO")) {
					config_File->ALGORITMO_REEMPLAZO = strdup(
													config_get_string_value(config, "ALGORITMO_REEMPLAZO"));
											log_info(logger,
													"Se encontró y cargó el contenido de ALGORITMO_REEMPLAZO a la estructura. Valor: %s",config_File->ALGORITMO_REEMPLAZO);

				} else {
											log_error(logger,
													"El archivo de configuracion no contiene el ALGORITMO_REEMPLAZO");
		}

		if (config_has_property(config, "ALGORITMO_PARTICION_LIBRE")) {
					config_File->ALGORITMO_PARTICION_LIBRE = strdup(
													config_get_string_value(config, "ALGORITMO_PARTICION_LIBRE"));
											log_info(logger,
													"Se encontró y cargó el contenido de ALGORITMO_PARTICION_LIBRE a la estructura. Valor: %s",config_File->ALGORITMO_PARTICION_LIBRE);

				} else {
											log_error(logger,
													"El archivo de configuracion no contiene el ALGORITMO_PARTICION_LIBRE");
		}

		if (config_has_property(config, "IP_BROKER")) {
					config_File->IP_BROKER = strdup(
													config_get_string_value(config, "IP_BROKER"));
											log_info(logger,
													"Se encontró y cargó el contenido de IP_BROKER a la estructura. Valor: %s",config_File->IP_BROKER);

				} else {
											log_error(logger,
													"El archivo de configuracion no contiene el IP_BROKER");
		}



		if (config_has_property(config, "PUERTO_BROKER")) {
			config_File->PUERTO_BROKER = config_get_int_value(config,
										"PUERTO_BROKER");
								log_info(logger,
										"Se encontró y cargó el contido del PUERTO_BROKER. Valor: %d",config_File->PUERTO_BROKER);
							} else {

								log_error(logger,
										"El archivo de configuracion no contiene el PUERTO_BROKER");

		}

		if (config_has_property(config, "FRECUENCIA_COMPACTACION")) {
			config_File->FRECUENCIA_COMPACTACION = config_get_int_value(config,
										"FRECUENCIA_COMPACTACION");
								log_info(logger,
										"Se encontró y cargó el contido del FRECUENCIA_COMPACTACION. Valor: %d",config_File->FRECUENCIA_COMPACTACION);
							} else {

								log_error(logger,
										"El archivo de configuracion no contiene el FRECUENCIA_COMPACTACION");

		}
	}
	config_destroy(config);
}

void iniciar_log(){

	char * archivoLog = strdup("broker.log");
	char * archivoLogCatedra = strdup("brokerCatedra.log");

	logger = log_create(LOG_PATH_INTERNO, archivoLog, FALSE, LOG_LEVEL_INFO);
	loggerCatedra = log_create(LOG_PATH, archivoLogCatedra, FALSE, LOG_LEVEL_INFO);

	free(archivoLog);
	archivoLog = NULL;

	free(archivoLogCatedra);
	archivoLogCatedra = NULL;
}

void publisher(){

	while(TRUE){

		sem_wait(&sem_contador_msjs_cola);

		pthread_mutex_lock(&mutex_cola_new_pokemon);
		if(list_size(cola_new_pokemon) != 0){
			reenviarMsjs_Cola(NEW_POKEMON, cola_new_pokemon, suscriptores_new_pokemon);
		}
		pthread_mutex_unlock(&mutex_cola_new_pokemon);

		pthread_mutex_lock(&mutex_cola_localized_pokemon);
		if(list_size(cola_localized_pokemon) != 0){
			reenviarMsjs_Cola(LOCALIZED_POKEMON, cola_localized_pokemon, suscriptores_localized_pokemon);
		}
		pthread_mutex_unlock(&mutex_cola_localized_pokemon);

		pthread_mutex_lock(&mutex_cola_get_pokemon);
		if(list_size(cola_get_pokemon) != 0){
			reenviarMsjs_Cola(GET_POKEMON, cola_get_pokemon, suscriptores_get_pokemon);
		}
		pthread_mutex_unlock(&mutex_cola_get_pokemon);

		pthread_mutex_lock(&mutex_cola_appeared_pokemon);
		if(list_size(cola_appeared_pokemon) != 0){
			reenviarMsjs_Cola(APPEARED_POKEMON, cola_appeared_pokemon, suscriptores_appeared_pokemon);
		}
		pthread_mutex_unlock(&mutex_cola_appeared_pokemon);

		pthread_mutex_lock(&mutex_cola_catch_pokemon);
		if(list_size(cola_catch_pokemon) != 0){
			reenviarMsjs_Cola(CATCH_POKEMON, cola_catch_pokemon, suscriptores_catch_pokemon);
		}
		pthread_mutex_unlock(&mutex_cola_catch_pokemon);

		pthread_mutex_lock(&mutex_cola_caught_pokemon);
		if(list_size(cola_caught_pokemon) != 0){
			reenviarMsjs_Cola(CAUGHT_POKEMON, cola_caught_pokemon, suscriptores_caught_pokemon);
		}
		pthread_mutex_unlock(&mutex_cola_caught_pokemon);
	}

}

void reenviarMsjs_Cola(int head, t_list * lista_Msjs_Cola, t_list * lista_de_suscriptores){

	while(!list_is_empty(lista_Msjs_Cola)){

		void * mensaje = list_get(lista_Msjs_Cola,0);

		cola_NEW_POKEMON  * new_poke = mensaje ;
		cola_APPEARED_POKEMON  * app_poke = mensaje ;
		cola_CATCH_POKEMON  * cath_poke = mensaje ;
		cola_GET_POKEMON * get_poke = mensaje ;
		cola_LOCALIZED_POKEMON  * loc_poke = mensaje ;

		t_list * aux_lista_de_suscriptores = list_create();

		switch( head ){
				case NEW_POKEMON :{
					pthread_mutex_lock(&mutex_suscriptores_new_pokemon);
					list_add_all(aux_lista_de_suscriptores,lista_de_suscriptores);
					pthread_mutex_unlock(&mutex_suscriptores_new_pokemon);
					break;
				}
				case APPEARED_POKEMON :{
					pthread_mutex_lock(&mutex_suscriptores_appeared_pokemon);
					list_add_all(aux_lista_de_suscriptores,lista_de_suscriptores);
					pthread_mutex_unlock(&mutex_suscriptores_appeared_pokemon);
					break;
				}
				case CATCH_POKEMON :{
					pthread_mutex_lock(&mutex_suscriptores_catch_pokemon);
					list_add_all(aux_lista_de_suscriptores,lista_de_suscriptores);
					pthread_mutex_unlock(&mutex_suscriptores_catch_pokemon);
					break;
				}
				case CAUGHT_POKEMON :{
					pthread_mutex_lock(&mutex_suscriptores_caught_pokemon);
					list_add_all(aux_lista_de_suscriptores,lista_de_suscriptores);
					pthread_mutex_unlock(&mutex_suscriptores_caught_pokemon);
					break;
				}
				case GET_POKEMON :{
					pthread_mutex_lock(&mutex_suscriptores_get_pokemon);
					list_add_all(aux_lista_de_suscriptores,lista_de_suscriptores);
					pthread_mutex_unlock(&mutex_suscriptores_get_pokemon);
					break;
				}
				case LOCALIZED_POKEMON :{
					pthread_mutex_lock(&mutex_suscriptores_localized_pokemon);
					list_add_all(aux_lista_de_suscriptores,lista_de_suscriptores);
					pthread_mutex_unlock(&mutex_suscriptores_localized_pokemon);
					break;
				}
			}

		while(!list_is_empty(aux_lista_de_suscriptores)){

			//SE ENVIA POR SOCKET EL MENSAJE

			losSuscriptores * suscriptor = list_get(aux_lista_de_suscriptores,0);

			int enviados ;

			switch( head ){
					case NEW_POKEMON :{
						enviados =  aplicar_protocolo_enviar(suscriptor->suSocket, head, new_poke);
						break;
					}
					case APPEARED_POKEMON :{
						enviados =  aplicar_protocolo_enviar(suscriptor->suSocket, head, app_poke);
						break;
					}
					case CATCH_POKEMON :{
						enviados =  aplicar_protocolo_enviar(suscriptor->suSocket, head, cath_poke);
						break;
					}
					case CAUGHT_POKEMON :{
						cola_CAUGHT_POKEMON  * cau_poke = mensaje ;
						enviados =  aplicar_protocolo_enviar(suscriptor->suSocket, head, cau_poke);
						break;
					}
					case GET_POKEMON :{
						enviados =  aplicar_protocolo_enviar(suscriptor->suSocket, head, get_poke);
						break;
					}
					case LOCALIZED_POKEMON :{
						enviados = aplicar_protocolo_enviar(suscriptor->suSocket, head, loc_poke);
						break;
					}
				}

			if (enviados == ERROR ) {

				//pthread_mutex_lock(&mutex_logs);

				log_info(logger,"No se puedo enviar correctamente el msj de la cola al suscriptor");

				//pthread_mutex_unlock(&mutex_logs);


			} else {

				//pthread_mutex_lock(&mutex_logs);

				log_info(loggerCatedra, "Se le envio un Mensaje al Suscriptor -> Modulo: %s de la cola %s y token: %d", devolverModulo(suscriptor->laSus->modulo), tipoMsjIntoToChar(head),suscriptor->laSus->token);
				log_info(logger,"Se puedo enviar correctamente el msj de la cola al suscriptor");

				//pthread_mutex_unlock(&mutex_logs);

			}
			list_remove(aux_lista_de_suscriptores, 0);
			//free(suscriptor);
		}

		if(head != CAUGHT_POKEMON ){
			free(new_poke->nombre_pokemon);
		}

		if(head == LOCALIZED_POKEMON ){
			int tamLista = list_size(loc_poke->lista_posiciones);
			for(int i=0 ; i < tamLista ; i++){
			  posicion * elack = list_get(loc_poke->lista_posiciones,i);
			  list_remove(loc_poke->lista_posiciones,0);
			  free(elack);
			}
		}

		free(mensaje);

		list_destroy(aux_lista_de_suscriptores);
		list_remove(lista_Msjs_Cola, 0);
	}
}

void envidoDesdeCache(void * laParti , int colaAsignada , int id_msj , losSuscriptores *laSus) {

	int recibidos;
	int idMsj ;
	int idTracking;

	_Bool estaPresente(respuesta_ACK * elAck){ return elAck->token == laSus->laSus->token && elAck->id_msj == id_msj; }
	pthread_mutex_lock(&mutex_lista_ack);
	_Bool fueRespondido = list_any_satisfy(lista_ack, (void*)estaPresente);
	pthread_mutex_unlock(&mutex_lista_ack);
		for ( int j = 0 ; j < list_size(laSus->laSus->cola_a_suscribir) ;j++){
			if(!fueRespondido && colaAsignada == (int) list_get(laSus->laSus->cola_a_suscribir,j)) {

									switch( colaAsignada ) {
										case NEW_POKEMON :{
											cola_NEW_POKEMON  * new_poke = malloc(sizeof(cola_NEW_POKEMON)) ;

											pthread_mutex_lock(&mutex_memoria_cache);

											deserealizar_mem_NEW_POKEMON(laParti ,new_poke);

											idMsj = new_poke->id_mensaje ;
											idTracking = new_poke->id_tracking ;

											pthread_mutex_unlock(&mutex_memoria_cache);

											recibidos = aplicar_protocolo_enviar(laSus->suSocket, colaAsignada , new_poke);

											free(new_poke->nombre_pokemon);
											free(new_poke);
											break;
										}
										case APPEARED_POKEMON :{
											cola_APPEARED_POKEMON  * app_poke = malloc(sizeof(cola_NEW_POKEMON)) ;

											pthread_mutex_lock(&mutex_memoria_cache);

											deserealizar_mem_APPEARED_POKEMON(laParti,app_poke);

											idMsj = app_poke->id_mensaje ;
											idTracking = app_poke->id_tracking ;

											pthread_mutex_unlock(&mutex_memoria_cache);

											recibidos = aplicar_protocolo_enviar(laSus->suSocket, colaAsignada , app_poke);

											free(app_poke->nombre_pokemon);

											free(app_poke);
											break;
										}
										case CATCH_POKEMON :{
											cola_CATCH_POKEMON  * cath_poke = malloc(sizeof(cola_CATCH_POKEMON)) ;

											pthread_mutex_lock(&mutex_memoria_cache);

											deserealizar_mem_CATCH_POKEMON (laParti,cath_poke);

											idMsj = cath_poke->id_mensaje ;
											idTracking = cath_poke->id_tracking ;

											pthread_mutex_unlock(&mutex_memoria_cache);

											recibidos = aplicar_protocolo_enviar(laSus->suSocket, colaAsignada , cath_poke);

											free(cath_poke->nombre_pokemon);

											free(cath_poke);
											break;
										}
										case CAUGHT_POKEMON :{

											cola_CAUGHT_POKEMON  * cau_poke  = malloc(sizeof(cola_CAUGHT_POKEMON)) ;

											pthread_mutex_lock(&mutex_memoria_cache);

											deserealizar_mem_CAUGHT_POKEMON(laParti,cau_poke);

											idMsj = cau_poke->id_mensaje ;
											idTracking = cau_poke->id_tracking ;

											pthread_mutex_unlock(&mutex_memoria_cache);

											recibidos = aplicar_protocolo_enviar(laSus->suSocket, colaAsignada , cau_poke);

											free(cau_poke);
											break;
										}
										case GET_POKEMON :{
											cola_GET_POKEMON * get_poke  = malloc(sizeof(cola_GET_POKEMON)) ;

											pthread_mutex_lock(&mutex_memoria_cache);

											deserealizar_mem_GET_POKEMON(laParti,get_poke);

											idMsj = get_poke->id_mensaje ;
											idTracking = get_poke->id_tracking ;

											pthread_mutex_unlock(&mutex_memoria_cache);

											recibidos = aplicar_protocolo_enviar(laSus->suSocket, colaAsignada , get_poke);

											free(get_poke->nombre_pokemon);

											free(get_poke);
											break;
										}
										case LOCALIZED_POKEMON :{
											cola_LOCALIZED_POKEMON  * loc_poke = malloc(sizeof(cola_LOCALIZED_POKEMON)) ;

											pthread_mutex_lock(&mutex_memoria_cache);

											deserealizar_mem_LOCALIZED_POKEMON(laParti,loc_poke);

											idMsj = loc_poke->id_mensaje ;
											idTracking = loc_poke->id_tracking ;

											pthread_mutex_unlock(&mutex_memoria_cache);

											recibidos = aplicar_protocolo_enviar(laSus->suSocket, colaAsignada , loc_poke);

											free(loc_poke->nombre_pokemon);

											free(loc_poke);
											break;
										}
									}

									if (recibidos == ERROR ) {

										//pthread_mutex_lock(&mutex_logs);

										log_info(logger,"No se puedo enviar correctamente el msj de la cola al suscriptor");

										//pthread_mutex_unlock(&mutex_logs);

									} else {
										reenvieMsj = true;

										//pthread_mutex_lock(&mutex_logs);

										log_info(loggerCatedra, "Se le envio un Mensaje ID %d y Tracking %d al Suscriptor -> Modulo: %s de la cola %s y token: %d", idMsj , idTracking , devolverModulo(laSus->laSus->modulo), tipoMsjIntoToChar(colaAsignada),laSus->laSus->token);
										log_info(logger,"Se puedo enviar correctamente el msj de la cola al suscriptor");

										//pthread_mutex_unlock(&mutex_logs);

									}
					}
		}
}

void reenviarMsjCache(losSuscriptores * laSus) {

	//int desplazamiento ;

	pthread_mutex_lock(&mutex_lista_particiones);
	int tamanioLista = list_size(lista_particiones) ;
	pthread_mutex_unlock(&mutex_lista_particiones);

	for ( int i = 0 ; i < tamanioLista ; i++ ) {
		if(strcasecmp(config_File->ALGORITMO_MEMORIA,"BS") == 0) {
			pthread_mutex_lock(&mutex_lista_particiones);
			Particion_bs * laParti = list_get(lista_particiones,i);
			 if ( !laParti->libre && !laParti->esPadre) {

					//pthread_mutex_lock(&mutex_logs);

				 log_info(logger,"La partición %d - %d tiene tiempo LRU %llu",laParti->punteroInicial,laParti->punteroFinal,laParti->tiempoLRU);

					//pthread_mutex_unlock(&mutex_logs);
				int cola = laParti->colaAsignada ;
				if ( cola != CAUGHT_POKEMON || cola != APPEARED_POKEMON || cola != LOCALIZED_POKEMON ) envidoDesdeCache(laParti ,cola  , laParti->id_msj , laSus);
				else envidoDesdeCache(laParti ,cola  , laParti->id_tracking , laSus);

				if(strcasecmp(config_File->ALGORITMO_REEMPLAZO,"LRU")==0 && reenvieMsj) {

					laParti->tiempoLRU = obtener_timestamp() ;
					//pthread_mutex_lock(&mutex_logs);
					log_info(logger,"se actulizo el tiempo LRU %llu de la partición %d - %d ",laParti->tiempoLRU,laParti->punteroInicial,laParti->punteroFinal);
					//pthread_mutex_unlock(&mutex_logs);
				}
			}
			 reenvieMsj=false;
			pthread_mutex_unlock(&mutex_lista_particiones);
		} else{
			pthread_mutex_lock(&mutex_lista_particiones);
			Particion * laParti = list_get(lista_particiones,i);
			 if ( !laParti->libre) {

					//pthread_mutex_lock(&mutex_logs);

				 log_info(logger,"La partición %d - %d tiene tiempo LRU %llu",laParti->punteroInicial,laParti->punteroFinal,laParti->tiempoLRU);

					//pthread_mutex_unlock(&mutex_logs);

					int cola = laParti->colaAsignada ;
					if ( cola != CAUGHT_POKEMON || cola != APPEARED_POKEMON || cola != LOCALIZED_POKEMON ) envidoDesdeCache(laParti ,cola  , laParti->id_msj , laSus);
					else envidoDesdeCache(laParti , cola  , laParti->id_tracking , laSus);

					if(strcasecmp(config_File->ALGORITMO_REEMPLAZO,"LRU")==0 && reenvieMsj) {
							laParti->tiempoLRU = obtener_timestamp() ;
							//pthread_mutex_lock(&mutex_logs);
							log_info(logger,"se actulizo el tiempo LRU %llu de la partición %d - %d ",laParti->tiempoLRU,laParti->punteroInicial,laParti->punteroFinal);
							//pthread_mutex_unlock(&mutex_logs);
						}			 }
			reenvieMsj=false;
			pthread_mutex_unlock(&mutex_lista_particiones);
		}

	}
}

_Bool buscarEnLista( t_list * lista , suscriptor * buscado ) {
	_Bool buscar_token(losSuscriptores * suscriptor_lista){return suscriptor_lista->laSus->token == buscado->token;}
	return list_any_satisfy(lista, (void*)buscar_token);
}

void suscribirse(losSuscriptores * suscp){

	int i = 0 , cantColas;

	cantColas = list_size(suscp->laSus->cola_a_suscribir);

	while(i != cantColas){
		int cola;
		cola = (int) list_get(suscp->laSus->cola_a_suscribir,i);

		_Bool presente = false ;

		switch (cola){

		case NEW_POKEMON :{
			//VERIFICO SI YA ESTABA INSCRIPTO, DE SER AFIRMATIVO LE REENVIO LOS MSJ
			pthread_mutex_lock(&mutex_suscriptores_new_pokemon);
			presente = buscarEnLista(suscriptores_new_pokemon,suscp->laSus) ;
			if ( ! presente ) {
				list_add(suscriptores_new_pokemon, suscp);
			}
			pthread_mutex_unlock(&mutex_suscriptores_new_pokemon);
			break;
		}
		case APPEARED_POKEMON :{
			pthread_mutex_lock(&mutex_suscriptores_appeared_pokemon);
			presente = buscarEnLista(suscriptores_appeared_pokemon,suscp->laSus) ;
			if ( !presente )  {
				list_add(suscriptores_appeared_pokemon, suscp);
			}
			pthread_mutex_unlock(&mutex_suscriptores_appeared_pokemon);
			break;
		}
		case CATCH_POKEMON :{
			pthread_mutex_lock(&mutex_suscriptores_catch_pokemon);
			presente = buscarEnLista(suscriptores_catch_pokemon,suscp->laSus) ;
			if ( !presente )  {
				list_add(suscriptores_catch_pokemon, suscp);
			}
			pthread_mutex_unlock(&mutex_suscriptores_catch_pokemon);
			break;
		}
		case GET_POKEMON :{
			pthread_mutex_lock(&mutex_suscriptores_get_pokemon);
			presente = buscarEnLista(suscriptores_get_pokemon,suscp->laSus) ;
			if ( !presente )  {
				list_add(suscriptores_get_pokemon, suscp);
			}
			pthread_mutex_unlock(&mutex_suscriptores_get_pokemon);
			break;
		}
		case LOCALIZED_POKEMON :{
			pthread_mutex_lock(&mutex_suscriptores_localized_pokemon);
			presente = buscarEnLista(suscriptores_localized_pokemon,suscp->laSus) ;
			if ( !presente )  {
				list_add(suscriptores_localized_pokemon, suscp);
			}
			pthread_mutex_unlock(&mutex_suscriptores_localized_pokemon);
			break;
		}
		case CAUGHT_POKEMON :{
			pthread_mutex_lock(&mutex_suscriptores_caught_pokemon);
			presente = buscarEnLista(suscriptores_caught_pokemon,suscp->laSus) ;
			if ( !presente ) {
				list_add(suscriptores_caught_pokemon, suscp);
			}
			pthread_mutex_unlock(&mutex_suscriptores_caught_pokemon);
			break;
		}
		default:
			//pthread_mutex_lock(&mutex_logs);

			log_info(logger, "Instrucción para suscribirse alguna cola no reconocida");

			//pthread_mutex_unlock(&mutex_logs);

			break;
		}
		i++;
	}

}
/*
void obtener_msj(int id_msj , Mensaje * msj)
{
	_Bool particion_libre(Mensaje* elMensajeBuscado){return elMensajeBuscado-> elack.id_msj == id_msj;}
	msj = list_find(lista_msjs, (void*)particion_libre);
}
*/

int32_t obtener_idMsj(){
	int32_t id_msj_aux;
	pthread_mutex_lock(&mutex_id_msj);
	id_msj++;
	id_msj_aux = id_msj;
	pthread_mutex_unlock(&mutex_id_msj);
	return id_msj_aux;
}

int32_t obtener_idtracking(){
	int32_t id_msj_aux;
	pthread_mutex_lock(&mutex_id_msj);
	id_tracking++;
	id_msj_aux = id_tracking;
	pthread_mutex_unlock(&mutex_id_msj);
	return id_msj_aux;
}
/*
void agregar_contador_msj(){
	pthread_mutex_lock(&mutex_contador_msjs_cola);
	contador_msjs_en_cola++;
	pthread_mutex_unlock(&mutex_contador_msjs_cola);
}

void actualizarLRU(Particion * particion){
	if(strcmp(config_File->ALGORITMO_MEMORIA, "BS") == 0){
		particion->tiempoLRU = (int) obtener_timestamp();
	}
}
*/

