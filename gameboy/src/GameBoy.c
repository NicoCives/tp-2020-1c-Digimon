/*
 ============================================================================
 Name        : GameBoy.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "GameBoy.h"

void comandoNoR(){
	fprintf(stderr,"Modulo no reconocido\n");
	free(comando);
	liberarRecursosComunes();
}

int main (int argc, char *argv[]) {

		crearLogger("GameBoy.log","GameBoyCatedra.log");

		configGB = reservarMemoria(sizeof(archivoConfigGB));

	if (argc < 2 || argc > 8 ){
			printf("No se ingreso la cantidad de parametros necesarios\n");
			log_info(logger,"No se ingreso la cantidad de parametros necesarios");
			return EXIT_FAILURE ;
	}

		inicializar_semaforos();

		log_info(logger, "Por setear los valores del archivo de configuracion");

		leerArchivoDeConfiguracion(RUTA_CONFIG_MEM,logger);

		comando = strdup( argv[1] ) ;

		/*
		struct hostent * dnsIP;

				if ((dnsIP=gethostbyname(configGB->ipBroker))==NULL) {
					log_info(logger, "No pude traducir el dns suministrado");
				}

				configGB->ipBroker = inet_ntoa(*((struct in_addr *)dnsIP->h_addr)) ;

				log_info(logger,"Se Tradujo IP_BROKER al Valor: %s",configGB->ipBroker);


				if ((dnsIP=gethostbyname(configGB->ipGameCard))==NULL) {
					log_info(logger, "No pude traducir el dns suministrado");
				}

				configGB->ipGameCard = inet_ntoa(*((struct in_addr *)dnsIP->h_addr)) ;

				log_info(logger,"Se Tradujo IP_GAMECARD al Valor: %s",configGB->ipGameCard);

				if ((dnsIP=gethostbyname(configGB->ipBroker))==NULL) {
					log_info(logger, "No pude traducir el dns suministrado");
				}

				configGB->ipTeam = inet_ntoa(*((struct in_addr *)dnsIP->h_addr)) ;

				log_info(logger,"Se Tradujo IP_TEAM al Valor: %s",configGB->ipTeam);
		*/

		//char * comando = strdup( "SUSCRIPTOR" ) ;

		/*
			Logs obligatorios

			Conexión a cualquier proceso.
			Suscripción a una cola de mensajes.
			Llegada de un nuevo mensaje afdReceptor una cola de mensajes.
			Envío de un mensaje a un suscriptor específico.
		 */

		log_info(logger, "Verificando los comandos ingresados");

		int modulo = devolverEnumModulo(comando);

		switch (modulo) {
			case BROKER : {
				broker(  comando, argc , argv) ;
				break;
			}
			case TEAM : {
				team(  comando, argc , argv) ;;
				break;
			}
			case GAMECARD : {
				gameCard(  comando, argc , argv) ;
				break;
			}
			case GBSUSCRIPTOR : {
				prc_suscriptor( comando, argc , argv) ;
				break;
			}
			default : {
				comandoNoR();
				return EXIT_FAILURE;
			}
		}

}

int gameCard( char * comando,int argc, char *argv[]) {

log_info(logger,"Trabajando con el GAMECARD");

			if ( argc < 3 ){
				printf("No se ingreso la cantidad de parametros necesarios\n");
				free(comando);
				return EXIT_FAILURE;
			}

			else {

				free(comando);

				comando = strdup(argv[2]);

				int modulo = devolverTipoMsj(comando) ;

				switch (modulo) {

					//./gameboy GAMECARD GET_POKEMON [POKEMON] ID_MSJ
					case GET_POKEMON:
						prc_get_pokemon( comando , argc, argv  ,
						"GAMECARD" , configGB->ipGameCard , configGB->puertoGameCard ,
						"Broker" , "GAMECARD" );
						break;
					//./gameboy GAMECARD CATCH_POKEMON [POKEMON] [POSX] [POSY] ID_MSJ
					case CATCH_POKEMON:
						prc_catch_pokemon( comando , argc, argv  ,
						"GAMECARD" , configGB->ipGameCard , configGB->puertoGameCard ,
						"Broker" , "GAMECARD" );
						break;
					//./gameboy GAMECARD NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD] ID_MSJ
					case NEW_POKEMON:
						prc_new_pokemon( comando , argc, argv  ,
						"GAMECARD" , configGB->ipGameCard , configGB->puertoGameCard ,
						"Broker" , "GAMECARD" );
					break;
					default: {
						comandoNoR();
						return EXIT_FAILURE;
						break;
					}

				}
			}
			return EXIT_FAILURE;

}

int team( char * comando,int argc, char *argv[]) {

log_info(logger,"Trabajando con el TEAM");

			if ( argc < 3 ){
				printf("No se ingreso la cantidad de parametros necesarios\n");
				free(comando);
				liberarRecursosComunes();
				return EXIT_FAILURE;
			}

			else {

				free(comando);

				comando = strdup(argv[2]);

				int modulo = devolverTipoMsj(comando) ;

					switch (modulo) {

						//./gameboy TEAM APPEARED_POKEMON [POKEMON] [POSX] [POSY]
							case APPEARED_POKEMON:
								prc_appeared_pokemon( comando , argc, argv  ,
								"TEAM" , configGB->ipTeam , configGB->puertoTeam ,
								"BROKER" , "TEAM" );
								break;

							//./gameboy TEAM ACK ESTADO ID_MSJ
							case ACK:{

								respuesta_ACK * ack =  malloc( sizeof(respuesta_ACK) );

								ack->ack = atoi(argv[3]);
								ack->id_msj = atoi(argv[4]);
								ack->token = configGB->token;

								conectar_y_enviar("TEAM", configGB->ipTeam , configGB->puertoTeam,"BROKER" ,
								"TEAM" ,ACK, ack , logger , loggerCatedra);

								free(ack);
								free(comando);
								liberarRecursosComunes();
								return EXIT_SUCCESS;
								break;
							}
							//./gameboy TEAM CAUGHT_POKEMON [ID_MENSAJE] [OK/FAIL]
							case CAUGHT_POKEMON:
								prc_caught_pokemon( comando , argc, argv  ,
								"TEAM" , configGB->ipTeam , configGB->puertoTeam ,
								"BROKER" , "TEAM" );
								break;

							//./gameboy TEAM LOCALIZED_POKEMON POKEMON CANTIDAD POSICIONES [ID_MENSAJE]

							//./gameboy TEAM LOCALIZED_POKEMON Pikachu 3 "1,2,3,4,5,6" 45

							case LOCALIZED_POKEMON:
								prc_localized_pokemon( comando , argc, argv  ,
								"TEAM" , configGB->ipTeam , configGB->puertoTeam ,
								"BROKER" , "TEAM" );
							break;
							default: {
								comandoNoR();
								return EXIT_FAILURE;
								break;
							}
					}
			}
			return EXIT_FAILURE;

}

int broker( char * comando,int argc, char *argv[]){

log_info(logger,"Trabajando con el BROKER");

	if ( argc < 3 ){
		printf("No se ingreso la cantidad de parametros necesarios\n");
		free(comando);
		return EXIT_FAILURE;
	}

	else {

		free(comando);

		comando = strdup(argv[2]);

		int modulo = devolverTipoMsj(comando) ;

			switch (modulo) {

					//./gameboy BROKER GET_POKEMON [POKEMON]
					case GET_POKEMON:
						prc_get_pokemon( comando , argc, argv  ,
						"BROKER" , configGB->ipBroker , configGB->puertoBroker ,
						"Team" , "BROKER" );
						break;
					//./gameboy BROKER CAUGHT_POKEMON [ID_MENSAJE] [OK/FAIL]
					case CAUGHT_POKEMON:{
						prc_caught_pokemon( comando , argc, argv  ,
						"BROKER" , configGB->ipBroker , configGB->puertoBroker ,
						"Team" , "BROKER" );
						break;
					}
					//./gameboy BROKER CATCH_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE]
					case CATCH_POKEMON:
						prc_catch_pokemon( comando , argc, argv  ,
						"BROKER" , configGB->ipBroker , configGB->puertoBroker ,
						"Team" , "BROKER" );
						break;

					//./gameboy BROKER APPEARED_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE_CORRELATIVO]

					case APPEARED_POKEMON:
						prc_appeared_pokemon( comando , argc, argv  ,
						"BROKER" , configGB->ipBroker , configGB->puertoBroker ,
						"Team" , "BROKER" );
						break;
					//./gameboy BROKER NEW_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE]

					case NEW_POKEMON:
						prc_new_pokemon( comando , argc, argv  ,
						"BROKER" , configGB->ipBroker , configGB->puertoBroker ,
						"Team" , "BROKER" );
					break;
					//./gameboy TEAM LOCALIZED_POKEMON POKEMON CANTIDAD POSICIONES [ID_MENSAJE]

					//./gameboy TEAM LOCALIZED_POKEMON Pikachu 3 "1,2,3,4,5,6" 45

					case LOCALIZED_POKEMON:
						prc_localized_pokemon( comando , argc, argv  ,
						"BROKER" , configGB->ipBroker , configGB->puertoBroker ,
						"Team" , "BROKER" );
					break;
					default: {
						comandoNoR();
						return EXIT_FAILURE;
						break;
					}
			}
	}
	return EXIT_FAILURE;

}

int prc_suscriptor( char * comando,int argc, char *argv[]) {

	//./gameboy SUSCRIPTOR [COLA_DE_MENSAJES] [TIEMPO]

	log_info(logger,"Trabajando con el SUSCRIPTOR");

		if ( argc < 4){
			printf("No se ingreso la cantidad de parametros necesarios\n");
			free(comando);
			liberarRecursosComunes();
			return EXIT_FAILURE;
		} else {

				free(comando);

				comando = strdup(argv[2]);

				int enumComando = devolverTipoMsj(comando) ;

				//comando = strdup("NEW_POKEMON");

				//tiempoSuscripcion = 240 ;

				tiempoSuscripcion = atoi(argv[3]);

				log_info(logger,"Ingresaron el comando -> %s durante %d segundos ",comando,tiempoSuscripcion);

				free(comando);

				laSuscripcion = (suscriptor * ) malloc(sizeof(suscriptor));

				laSuscripcion->modulo = GAMEBOY ;
				if(configGB->token == 0) laSuscripcion->token = token();
				else laSuscripcion->token = configGB->token;
				log_info("Genere el token nro: %d",configGB->token);
				laSuscripcion->cola_a_suscribir = list_create();
				list_add(laSuscripcion->cola_a_suscribir, enumComando);

				crearHilos();
				/*
				for ( int i = 0 ; i < list_size(laSuscripcion->cola_a_suscribir); i ++){
					int * laPosicion ;
					laPosicion = list_get(laSuscripcion->cola_a_suscribir,i);
					free(laPosicion);
				}
				*/

				liberarRecursosComunes();

				return EXIT_SUCCESS;
		}
}


int prc_caught_pokemon( char * comando,int argc, char *argv[] , char * modulo , char * ipServer , int puertoServer , char * Hand , char * HandEsperado ) {

	if ( argc < 4 ){
		printf("No se ingreso la cantidad de parametros necesarios\n");
		free(comando);
		liberarRecursosComunes();
		return EXIT_FAILURE;
	}

	cola_CAUGHT_POKEMON * cau_poke = (cola_CAUGHT_POKEMON * ) malloc(sizeof(cola_CAUGHT_POKEMON));

	cau_poke->id_mensaje = atoi(argv[3]);
	cau_poke->id_tracking = 0 ;

	int enviado;
	int head ; int tamanioMensaje;
	int larespuesta;
	void * mensaje;
	respuesta_ACK elACK;

	if ( strcasecmp(argv[4],"OK") == 0 ) cau_poke->atrapo_pokemon = TRUE ;
	else cau_poke->atrapo_pokemon = ERROR ;

	if (strcasecmp(modulo,"BROKER")==0){

		enviado = conectar_enviar_recibir(modulo, ipServer, puertoServer, Hand , HandEsperado ,CAUGHT_POKEMON, cau_poke , logger , loggerCatedra);

		larespuesta = recibirProtocolo(&head,&tamanioMensaje,mySocket); // recibo head y tamaño de msj

		if (head < 1 || larespuesta <= 0){ // DESCONEXIÓN
			cerrarSocket(mySocket);
			//return EXIT_SUCCESS;
		}else{
				mensaje = malloc(tamanioMensaje);

				larespuesta = recibirMensaje(mySocket , tamanioMensaje , mensaje ); // recibo msj serializado para el tratamiento deseado

				if ( head == ACK) {
					deserealizar_ACK(head,mensaje,tamanioMensaje,&elACK);
					free(mensaje);
				}

		}
	} else {
		enviado = conectar_y_enviar(modulo, ipServer, puertoServer, Hand , HandEsperado ,CAUGHT_POKEMON, cau_poke , logger , loggerCatedra);
	}

	if (enviado != ERROR) log_info(logger,"Le envio a la cola CAUGHT_POKEMON -> ID_MENSAJE: %d , ESTADO: %d",cau_poke->id_mensaje,cau_poke->atrapo_pokemon);

	if (larespuesta != ERROR) log_info(logger,"Recibí un ACK con los siguientes datos ESTADO: %d ID_MSJ: %d ",elACK.ack,elACK.id_msj);

	free(comando);

	free(cau_poke);

	cerrarSocket(mySocket);

	liberarRecursosComunes();

	return EXIT_SUCCESS;

}

void catch(cola_CATCH_POKEMON * cat_poke , int idMsj,int argc, char *argv[] ){

	cat_poke->id_mensaje = 0 ;
	cat_poke->id_tracking = idMsj ;
	cat_poke->nombre_pokemon = strdup(argv[3]);
	cat_poke->tamanio_nombre = string_length(cat_poke->nombre_pokemon ) ;
	cat_poke->posicion_x = atoi(argv[4]) ;
	cat_poke->posicion_y = atoi(argv[5]) ;
}

int prc_catch_pokemon( char * comando,int argc, char *argv[] , char * modulo , char * ipServer , int puertoServer , char * Hand , char * HandEsperado ){



	cola_CATCH_POKEMON * cat_poke = (cola_CATCH_POKEMON * ) malloc(sizeof(cola_CATCH_POKEMON));

	int enviado;
	int head ; int tamanioMensaje;
	int larespuesta;
	void * mensaje;
	respuesta_ACK elACK;

	if(strcasecmp(modulo,"BROKER") == 0 ) {
		//log_info(logger,"BROKER -> catch_pokemon");
		if ( argc < 6 ){
			printf("No se ingreso la cantidad de parametros necesarios\n");
			free(comando);
			liberarRecursosComunes();
			return EXIT_FAILURE;
		}
		catch( cat_poke , 0 ,argc, argv );

		enviado = conectar_enviar_recibir(modulo, ipServer , puertoServer , Hand , HandEsperado ,CATCH_POKEMON, cat_poke , logger , loggerCatedra);

		larespuesta = recibirProtocolo(&head,&tamanioMensaje,mySocket); // recibo head y tamaño de msj

		if (head < 1 || larespuesta <= 0){ // DESCONEXIÓN
			cerrarSocket(mySocket);
			//return EXIT_SUCCESS;
		}else{
				mensaje = malloc(tamanioMensaje);

				larespuesta = recibirMensaje(mySocket , tamanioMensaje , mensaje ); // recibo msj serializado para el tratamiento deseado

				if ( head == ACK) {
					deserealizar_ACK(head,mensaje,tamanioMensaje,&elACK);
					free(mensaje);
				}

		}
	} else {
		if ( argc < 7 ){
		printf("No se ingreso la cantidad de parametros necesarios\n");
		free(comando);
		liberarRecursosComunes();
		return EXIT_FAILURE;
		}
		catch( cat_poke , atoi(argv[6]) ,argc, argv );

		enviado = conectar_y_enviar(modulo, ipServer , puertoServer , Hand , HandEsperado ,CATCH_POKEMON, cat_poke , logger , loggerCatedra);
	}

	if (enviado != ERROR) log_info(logger,"Le envio a la cola CATCH_POKEMON -> POKEMON: %s  , CORDENADA X: %d , CORDENADA Y: %d ",cat_poke->nombre_pokemon,cat_poke->posicion_x,cat_poke->posicion_y);

	if (larespuesta != ERROR) log_info(logger,"Recibí un ACK con los siguientes datos ESTADO: %d ID_MSJ: %d ",elACK.ack,elACK.id_msj);

	free(comando);
	free(cat_poke->nombre_pokemon);
	free(cat_poke);

	cerrarSocket(mySocket);

	liberarRecursosComunes();

	return EXIT_SUCCESS;
}

void appeared(cola_APPEARED_POKEMON * app_poke , int idMsj,int argc, char *argv[] ) {

	app_poke->id_mensaje = 0 ;
	app_poke->id_tracking = idMsj ;
	app_poke->nombre_pokemon = strdup(argv[3]);
	app_poke->tamanio_nombre = string_length(app_poke->nombre_pokemon);
	app_poke->posicion_x = atoi(argv[4]) ;
	app_poke->posicion_y = atoi(argv[5]) ;
}

int prc_appeared_pokemon( char * comando,int argc, char *argv[] , char * modulo , char * ipServer , int puertoServer , char * Hand , char * HandEsperado ) {

	cola_APPEARED_POKEMON * app_poke =  malloc( sizeof(cola_APPEARED_POKEMON) );

 	if ( argc < 7 ){
 		appeared( app_poke , 0 ,argc, argv );
	} else {
		appeared( app_poke , atoi(argv[6]) ,argc, argv );
	}

	int enviado;
	int head ; int tamanioMensaje;
	int larespuesta;
	void * mensaje;
	respuesta_ACK elACK;


	/*
	app_poke->nombre_pokemon = strdup("pikachu");
	app_poke->tamanio_nombre = string_length(app_poke->nombre_pokemon);
	app_poke->posicion_x = 2 ;
	app_poke->posicion_y = 10 ;
	*/

	if(strcasecmp(modulo,"BROKER") == 0 ) {

		enviado = conectar_enviar_recibir(modulo, ipServer , puertoServer, Hand , HandEsperado ,APPEARED_POKEMON, app_poke , logger , loggerCatedra);

		larespuesta = recibirProtocolo(&head,&tamanioMensaje,mySocket); // recibo head y tamaño de msj

		if (head < 1 || larespuesta <= 0){ // DESCONEXIÓN
			cerrarSocket(mySocket);
			//return EXIT_SUCCESS;
		}else{
				mensaje = malloc(tamanioMensaje);

				larespuesta = recibirMensaje(mySocket , tamanioMensaje , mensaje ); // recibo msj serializado para el tratamiento deseado

				if ( head == ACK) {
					deserealizar_ACK(head,mensaje,tamanioMensaje,&elACK);
					free(mensaje);
				}

		}
	} else {

		enviado = conectar_y_enviar(modulo, ipServer , puertoServer, Hand , HandEsperado ,APPEARED_POKEMON, app_poke , logger , loggerCatedra);
	}


	if (enviado != ERROR) log_info(logger,"Le envio a la cola APPEARED_POKEMON -> POKEMON: %s  , CORDENADA X: %d , CORDENADA Y: %d ",app_poke->nombre_pokemon,app_poke->posicion_x,app_poke->posicion_y);

	if (larespuesta != ERROR) log_info(logger,"Recibí un ACK con los siguientes datos ESTADO: %d ID_MSJ: %d ",elACK.ack,elACK.id_msj);

	cerrarSocket(mySocket);

	free(comando);
	free(app_poke->nombre_pokemon);
	free(app_poke);

	liberarRecursosComunes();

	return EXIT_SUCCESS;
}

void new(cola_NEW_POKEMON * new_poke , int idMsj,int argc, char *argv[] ){

	new_poke->id_mensaje = 0 ;
	new_poke->id_tracking = idMsj ;
	new_poke->nombre_pokemon = strdup(argv[3]);
	new_poke->tamanio_nombre = string_length(new_poke->nombre_pokemon ) ;
	new_poke->posicion_x = atoi(argv[4]) ;
	new_poke->posicion_y = atoi(argv[5]) ;
	new_poke->cantidad = atoi(argv[6]) ;
}

int prc_new_pokemon( char * comando,int argc, char *argv[] , char * modulo , char * ipServer , int puertoServer , char * Hand , char * HandEsperado ) {

	cola_NEW_POKEMON * new_poke = (cola_NEW_POKEMON * ) malloc(sizeof(cola_NEW_POKEMON));

	int enviado;
	int head ; int tamanioMensaje;
	int larespuesta;
	void * mensaje;
	respuesta_ACK elACK;

	if(strcasecmp(modulo,"BROKER") == 0 ) {
		//log_info(logger,"BROKER -> catch_pokemon");
		if ( argc < 7 ){
			printf("No se ingreso la cantidad de parametros necesarios\n");
			free(comando);
			liberarRecursosComunes();
			return EXIT_FAILURE;
		}
		new( new_poke , 0 ,argc, argv );

		enviado = conectar_enviar_recibir( modulo, ipServer, puertoServer, Hand , HandEsperado ,NEW_POKEMON, new_poke , logger , loggerCatedra);

		larespuesta = recibirProtocolo(&head,&tamanioMensaje,mySocket); // recibo head y tamaño de msj

		if (head < 1 || larespuesta <= 0){ // DESCONEXIÓN
			cerrarSocket(mySocket);
			//return EXIT_SUCCESS;
		}else{
				mensaje = malloc(tamanioMensaje);

				larespuesta = recibirMensaje(mySocket , tamanioMensaje , mensaje ); // recibo msj serializado para el tratamiento deseado

				if ( head == ACK) {
					deserealizar_ACK(head,mensaje,tamanioMensaje,&elACK);
					free(mensaje);
				}

		}
	} else {
		if ( argc < 8 ){
			printf("No se ingreso la cantidad de parametros necesarios\n");
			free(comando);
			liberarRecursosComunes();
			return EXIT_FAILURE;
		}
		new( new_poke , atoi(argv[7]) ,argc, argv );

		enviado = conectar_y_enviar( modulo, ipServer, puertoServer, Hand , HandEsperado ,NEW_POKEMON, new_poke , logger , loggerCatedra);

	}

	if (enviado != ERROR) log_info(logger,"Le envio a la cola NEW_POKEMON -> POKEMON: %s  , CORDENADA X: %d , CORDENADA Y: %d , CANTIDAD: %d ",new_poke->nombre_pokemon,new_poke->posicion_x,new_poke->posicion_y,new_poke->cantidad);

	if (larespuesta != ERROR) log_info(logger,"Recibí un ACK con los siguientes datos ESTADO: %d ID_MSJ: %d ",elACK.ack,elACK.id_msj);

	free(comando);
	free(new_poke->nombre_pokemon);
	free(new_poke);

	cerrarSocket(mySocket);

	liberarRecursosComunes();

	return EXIT_SUCCESS;
}

void localized(cola_LOCALIZED_POKEMON * loc_poke , int idMsj,int argc, char *argv[] ){

	loc_poke->lista_posiciones = list_create(); // 16

	loc_poke->id_mensaje = 0  ;
	loc_poke->id_tracking = idMsj  ;//4
	loc_poke->cantidad = atoi(argv[4]) ; // 4
	loc_poke->nombre_pokemon = strdup(argv[3]);//  "raichu" 6

	char * posiciones = strdup(argv[5]);

	char ** listapokemon = string_split(posiciones,",");

	//char ** listapokemon = string_split("1,2,3,4",",");

	/*
	loc_poke->id_mensaje = 22  ; //4
	loc_poke->cantidad = 2 ; // 4
	loc_poke->nombre_pokemon = strdup("Pikachu");; //  "raichu" 6
	*/
	loc_poke->tamanio_nombre = string_length(loc_poke->nombre_pokemon); // 4
	/*
	posicion * laPosicion1 = malloc(sizeof(posicion));
	posicion * laPosicion2 = malloc(sizeof(posicion));

	laPosicion1->posicion_x = 1 ;
	laPosicion1->posicion_y = 2 ;
	laPosicion2->posicion_x = 3 ;
	laPosicion2->posicion_y = 4 ;

	list_add(loc_poke->lista_posiciones,laPosicion1);
	list_add(loc_poke->lista_posiciones,laPosicion2);
	*/

	int unaposicion = 0 ;
	while (listapokemon[unaposicion] != NULL){
		posicion * laPosicion = malloc( sizeof(posicion));
		laPosicion->posicion_x = atoi(listapokemon[unaposicion]);
		unaposicion++;
		laPosicion->posicion_y = atoi(listapokemon[unaposicion]);
		list_add(loc_poke->lista_posiciones,laPosicion);
		unaposicion++;
	}

	unaposicion = 0 ;

	while (listapokemon[unaposicion] != NULL){
		posicion * laPosicion ;
		laPosicion = listapokemon[unaposicion];
		free(laPosicion);
		unaposicion++;
	}
	free(posiciones);
	free(listapokemon);
}



int prc_localized_pokemon( char * comando,int argc, char *argv[] , char * modulo , char * ipServer , int puertoServer , char * Hand , char * HandEsperado ){

	int enviado;
	int head ; int tamanioMensaje;
	int larespuesta;
	void * mensaje;
	respuesta_ACK elACK;

	cola_LOCALIZED_POKEMON * loc_poke = malloc( sizeof(cola_LOCALIZED_POKEMON));

			if(strcasecmp(modulo,"BROKER") == 0 ) {

				if ( argc < 6 ){
					printf("No se ingreso la cantidad de parametros necesarios\n");
					free(comando);
					liberarRecursosComunes();
					return EXIT_FAILURE;
				}

				localized( loc_poke , 0 ,argc, argv );

				enviado = conectar_enviar_recibir(modulo, ipServer , puertoServer, Hand ,  HandEsperado ,LOCALIZED_POKEMON, loc_poke , logger , loggerCatedra);

				larespuesta = recibirProtocolo(&head,&tamanioMensaje,mySocket); // recibo head y tamaño de msj

				if (head < 1 || larespuesta <= 0){ // DESCONEXIÓN
					cerrarSocket(mySocket);
					//return EXIT_SUCCESS;
				}else{
						mensaje = malloc(tamanioMensaje);

						larespuesta = recibirMensaje(mySocket , tamanioMensaje , mensaje ); // recibo msj serializado para el tratamiento deseado

						if ( head == ACK) {
							deserealizar_ACK(head,mensaje,tamanioMensaje,&elACK);
							free(mensaje);
						}

				}
			} else {

				if ( argc < 7 ){
					printf("No se ingreso la cantidad de parametros necesarios\n");
					free(comando);
					liberarRecursosComunes();
					return EXIT_FAILURE;
				}

				localized( loc_poke , atoi(argv[6]) ,argc, argv );

				enviado = conectar_y_enviar(modulo, ipServer , puertoServer, Hand ,  HandEsperado ,LOCALIZED_POKEMON, loc_poke , logger , loggerCatedra);

			}
			if (enviado != ERROR ) {
				for ( int i = 0 ; i < list_size(loc_poke->lista_posiciones); i ++){
				posicion * pos = list_get(loc_poke->lista_posiciones,i) ;
				log_info(logger,"Le envio a la cola LOCALIZED_POKEMON -> POKEMON: %s  , CORDENADAX: %d , CORDENADA Y: %d ",loc_poke->nombre_pokemon,loc_poke->cantidad,pos->posicion_x,pos->posicion_y);
				}
			}

			if (larespuesta != ERROR) log_info(logger,"Recibí un ACK con los siguientes datos ESTADO: %d ID_MSJ: %d ",elACK.ack,elACK.id_msj);



			free(comando);
			free(loc_poke->nombre_pokemon);

			for ( int i = 0 ; i < list_size(loc_poke->lista_posiciones); i ++){
				posicion * laPosicion ;
				laPosicion = list_get(loc_poke->lista_posiciones,i);
				free(laPosicion);
			}

			list_destroy(loc_poke->lista_posiciones);

			free(loc_poke);

			cerrarSocket(mySocket);

			liberarRecursosComunes();

			return EXIT_SUCCESS;

}

void liberarRecursosComunes(){

	free(configGB->ipBroker);
	free(configGB->ipGameCard);
	free(configGB->ipTeam);
	free(configGB);
	log_destroy(logger);
	log_destroy(loggerCatedra);
}

void get(cola_GET_POKEMON * get_poke , int idMsj,int argc, char *argv[] ){

		get_poke->id_mensaje = 0 ;

		get_poke->id_tracking = idMsj ;

		get_poke->nombre_pokemon = strdup(argv[3]);

		get_poke->tamanio_nombre = string_length(get_poke->nombre_pokemon );
}

int prc_get_pokemon( char * comando,int argc, char *argv[] , char * modulo , char * ipServer , int puertoServer , char * Hand , char * HandEsperado ) {

	cola_GET_POKEMON * get_poke = (cola_GET_POKEMON * ) malloc(sizeof(cola_GET_POKEMON));

	int enviado;
	int head ; int tamanioMensaje;
	int larespuesta;
	void * mensaje;
	respuesta_ACK elACK;

	if(strcasecmp(modulo,"BROKER") == 0 ) {
		//log_info(logger,"BROKER -> catch_pokemon");
		if ( argc < 4 ){
			printf("No se ingreso la cantidad de parametros necesarios\n");
			free(comando);
			liberarRecursosComunes();
			return EXIT_FAILURE;
		}
		get( get_poke , 0 ,argc, argv );


		enviado = conectar_enviar_recibir( modulo , ipServer , puertoServer , Hand , HandEsperado ,GET_POKEMON, get_poke , logger , loggerCatedra);

		larespuesta = recibirProtocolo(&head,&tamanioMensaje,mySocket); // recibo head y tamaño de msj

		if (head < 1 || larespuesta <= 0){ // DESCONEXIÓN
			cerrarSocket(mySocket);
			//return EXIT_SUCCESS;
		}else{
				mensaje = malloc(tamanioMensaje);

				larespuesta = recibirMensaje(mySocket , tamanioMensaje , mensaje ); // recibo msj serializado para el tratamiento deseado

				if ( head == ACK) {
					deserealizar_ACK(head,mensaje,tamanioMensaje,&elACK);
					free(mensaje);
				}

		}
	} else {
		if ( argc < 5 ){
			printf("No se ingreso la cantidad de parametros necesarios\n");
			free(comando);
			liberarRecursosComunes();
			return EXIT_FAILURE;
		}
		get( get_poke , atoi(argv[4]) ,argc, argv );
		enviado = conectar_y_enviar( modulo , ipServer , puertoServer , Hand , HandEsperado ,GET_POKEMON, get_poke , logger , loggerCatedra);
	}



	if (enviado != ERROR) log_info(logger,"Le envio a la cola GET_POKEMON -> POKEMON: %s ",get_poke->nombre_pokemon);

	if (larespuesta != ERROR) log_info(logger,"Recibí un ACK con los siguientes datos ESTADO: %d ID_MSJ: %d ",elACK.ack,elACK.id_msj);

	free(comando);

	free(get_poke->nombre_pokemon);

	free(get_poke);

	cerrarSocket(mySocket);

	liberarRecursosComunes();

	return EXIT_SUCCESS;


}
