/*
 * memoria.c
 *
 *  Created on: 16 jul. 2020
 *      Author: utnso
 */

#include "memoria.h"

void deserealizar_mem_NEW_POKEMON ( void * laParti , cola_NEW_POKEMON * new_poke ) {

	//pthread_mutex_lock(&mutex_memoria_cache);

	int desplazamiento ;

	if(strcasecmp(config_File->ALGORITMO_MEMORIA,"BS") == 0) {
		Particion_bs * laPartiBS = laParti ;
				new_poke->id_mensaje = laPartiBS->id_msj ;
				new_poke->id_tracking = laPartiBS->id_tracking ;
		desplazamiento =laPartiBS->punteroInicial ;
	} else{
		Particion  * laPartiComun = laParti ;
				new_poke->id_mensaje = laPartiComun->id_msj ;
				new_poke->id_tracking = laPartiComun->id_tracking ;
		desplazamiento =laPartiComun->punteroInicial ;
	}

							memcpy(&new_poke->tamanio_nombre,memoria_cache+desplazamiento,sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);

							new_poke->nombre_pokemon = malloc(1);

							new_poke->nombre_pokemon = realloc(new_poke->nombre_pokemon,new_poke->tamanio_nombre + 1);
							memcpy(new_poke->nombre_pokemon,memoria_cache+desplazamiento,new_poke->tamanio_nombre);
							desplazamiento += new_poke->tamanio_nombre;

							memcpy(&new_poke->posicion_x,memoria_cache+desplazamiento,sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);

							memcpy(&new_poke->posicion_y,memoria_cache+desplazamiento,sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);

							memcpy(&new_poke->cantidad,memoria_cache+desplazamiento,sizeof(uint32_t));

							new_poke->nombre_pokemon[new_poke->tamanio_nombre] = '\0';

	//pthread_mutex_unlock(&mutex_memoria_cache);
}

void deserealizar_mem_APPEARED_POKEMON ( void * laParti , cola_APPEARED_POKEMON * app_poke) {

	//pthread_mutex_lock(&mutex_memoria_cache);

	int desplazamiento ;

	if(strcasecmp(config_File->ALGORITMO_MEMORIA,"BS") == 0) {
		Particion_bs * laPartiBS = laParti ;
				app_poke->id_mensaje = laPartiBS->id_msj ;
				app_poke->id_tracking = laPartiBS->id_tracking ;
		desplazamiento =laPartiBS->punteroInicial ;
	} else{
		Particion  * laPartiComun = laParti ;
				app_poke->id_mensaje = laPartiComun->id_msj ;
				app_poke->id_tracking = laPartiComun->id_tracking ;
		desplazamiento =laPartiComun->punteroInicial ;
	}

			app_poke->nombre_pokemon = malloc(1);

			memcpy(&app_poke->tamanio_nombre,(memoria_cache+desplazamiento),sizeof(uint32_t));
			desplazamiento += sizeof(uint32_t);

			app_poke->nombre_pokemon = realloc(app_poke->nombre_pokemon,app_poke->tamanio_nombre +1);
			memcpy(app_poke->nombre_pokemon,(memoria_cache+desplazamiento),app_poke->tamanio_nombre);
			desplazamiento += app_poke->tamanio_nombre;

			memcpy(&app_poke->posicion_x,(memoria_cache+desplazamiento),sizeof(uint32_t));
			desplazamiento += sizeof(uint32_t);

			memcpy(&app_poke->posicion_y,(memoria_cache+desplazamiento),sizeof(uint32_t));

			app_poke->nombre_pokemon[app_poke->tamanio_nombre] = '\0';

			//pthread_mutex_unlock(&mutex_memoria_cache);

			//log_info(logger,"deserealizar CASE APPEARED_POKEMON -> ID:%d , POKEMON:%s , POSICIÓN X:%d , POSICIÓN Y: %d",app_poke->id_mensaje,app_poke->nombre_pokemon,app_poke->posicion_x,app_poke->posicion_y);

			//return app_poke;
}

void deserealizar_mem_CATCH_POKEMON ( void * laParti , cola_CATCH_POKEMON * cat_poke) {

	//pthread_mutex_lock(&mutex_memoria_cache);

	cat_poke->nombre_pokemon = malloc(1);

	int desplazamiento ;

	if(strcasecmp(config_File->ALGORITMO_MEMORIA,"BS") == 0) {
		Particion_bs * laPartiBS = laParti ;
				cat_poke->id_mensaje = laPartiBS->id_msj ;
				cat_poke->id_tracking = laPartiBS->id_tracking ;
		desplazamiento =laPartiBS->punteroInicial ;
	} else{
		Particion  * laPartiComun = laParti ;
				cat_poke->id_mensaje = laPartiComun->id_msj ;
				cat_poke->id_tracking = laPartiComun->id_tracking ;
		desplazamiento =laPartiComun->punteroInicial ;
	}

			memcpy(&cat_poke->tamanio_nombre,(memoria_cache+desplazamiento),sizeof(uint32_t));
			desplazamiento += sizeof(uint32_t);

			cat_poke->nombre_pokemon = realloc(cat_poke->nombre_pokemon,cat_poke->tamanio_nombre +1);
			memcpy(cat_poke->nombre_pokemon,(memoria_cache+desplazamiento),cat_poke->tamanio_nombre);
			desplazamiento += cat_poke->tamanio_nombre;

			memcpy(&cat_poke->posicion_x,(memoria_cache+desplazamiento),sizeof(uint32_t));
			desplazamiento += sizeof(uint32_t);

			memcpy(&cat_poke->posicion_y,(memoria_cache+desplazamiento),sizeof(uint32_t));

			cat_poke->nombre_pokemon[cat_poke->tamanio_nombre] = '\0';

			//pthread_mutex_unlock(&mutex_memoria_cache);
}

void deserealizar_mem_CAUGHT_POKEMON ( void * laParti , cola_CAUGHT_POKEMON * cau_poke) {

	//pthread_mutex_lock(&mutex_memoria_cache);

	int desplazamiento ;

	if(strcasecmp(config_File->ALGORITMO_MEMORIA,"BS") == 0) {
		Particion_bs * laPartiBS = laParti ;
				cau_poke->id_mensaje = laPartiBS->id_msj ;
				cau_poke->id_tracking = laPartiBS->id_tracking ;
		desplazamiento =laPartiBS->punteroInicial ;
	} else{
		Particion  * laPartiComun = laParti ;
				cau_poke->id_mensaje = laPartiComun->id_msj ;
				cau_poke->id_tracking = laPartiComun->id_tracking ;
		desplazamiento =laPartiComun->punteroInicial ;
	};

							memcpy(&cau_poke->atrapo_pokemon,(memoria_cache+desplazamiento),sizeof(uint32_t));

							//pthread_mutex_unlock(&mutex_memoria_cache);

}

void deserealizar_mem_GET_POKEMON ( void * laParti , cola_GET_POKEMON * get_poke) {

	//pthread_mutex_lock(&mutex_memoria_cache);

	int desplazamiento ;

	if(strcasecmp(config_File->ALGORITMO_MEMORIA,"BS") == 0) {
		Particion_bs * laPartiBS = laParti ;
				get_poke->id_mensaje = laPartiBS->id_msj ;
				get_poke->id_tracking = laPartiBS->id_tracking ;
		desplazamiento =laPartiBS->punteroInicial ;
	} else{
		Particion  * laPartiComun = laParti ;
				get_poke->id_mensaje = laPartiComun->id_msj ;
				get_poke->id_tracking = laPartiComun->id_tracking ;
		desplazamiento =laPartiComun->punteroInicial ;
	}
				get_poke->nombre_pokemon = malloc(1);

							memcpy(&get_poke->tamanio_nombre,(memoria_cache+desplazamiento),sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);

							get_poke->nombre_pokemon = realloc(get_poke->nombre_pokemon,get_poke->tamanio_nombre +1);
							memcpy(get_poke->nombre_pokemon,(memoria_cache+desplazamiento),get_poke->tamanio_nombre);

							get_poke->nombre_pokemon[get_poke->tamanio_nombre] = '\0';

							//pthread_mutex_unlock(&mutex_memoria_cache);
}

void deserealizar_mem_LOCALIZED_POKEMON ( void * laParti ,  cola_LOCALIZED_POKEMON * loc_poke_des) {

	//pthread_mutex_lock(&mutex_memoria_cache);

	int desplazamiento;

	if(strcasecmp(config_File->ALGORITMO_MEMORIA,"BS") == 0) {
		Particion_bs * laPartiBS = laParti ;
		loc_poke_des->id_mensaje = laPartiBS->id_msj ;
		loc_poke_des->id_tracking = laPartiBS->id_tracking ;
		desplazamiento =laPartiBS->punteroInicial ;
	} else{
		Particion  * laPartiComun = laParti ;
		loc_poke_des->id_mensaje = laPartiComun->id_msj ;
		loc_poke_des->id_tracking = laPartiComun->id_tracking ;
		desplazamiento =laPartiComun->punteroInicial ;
	}


							memcpy(&loc_poke_des->tamanio_nombre,(memoria_cache+desplazamiento),sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);

							loc_poke_des->nombre_pokemon = malloc(1);

							loc_poke_des->nombre_pokemon = realloc(loc_poke_des->nombre_pokemon,loc_poke_des->tamanio_nombre +1);
							memcpy(loc_poke_des->nombre_pokemon,(memoria_cache+desplazamiento),loc_poke_des->tamanio_nombre);
							desplazamiento += loc_poke_des->tamanio_nombre;

							memcpy(&loc_poke_des->cantidad,(memoria_cache+desplazamiento),sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);

							loc_poke_des->lista_posiciones = list_create();

							for (int i = 0 ; i < loc_poke_des->cantidad ; i++){
							posicion *laPosicion = malloc (sizeof(posicion));
							memcpy(&laPosicion->posicion_x,memoria_cache+desplazamiento,sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);
							memcpy(&laPosicion->posicion_y,memoria_cache+desplazamiento,sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);
							list_add(loc_poke_des->lista_posiciones,laPosicion);
							}

							loc_poke_des->nombre_pokemon[loc_poke_des->tamanio_nombre] = '\0';

							//pthread_mutex_unlock(&mutex_memoria_cache);
}
