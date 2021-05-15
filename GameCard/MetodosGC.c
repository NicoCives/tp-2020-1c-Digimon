#include "MetodosGC.h"
#include <commons/config.h>
#include "src/sockets.h"
#include <string.h>





int NewPokemon(cola_NEW_POKEMON* Pokemon){

	int step = 0;
	t_files dataPokemon;// = malloc (sizeof(t_files));
	t_Pokemones idPokemon;// = malloc (sizeof(t_Pokemones));
	while(step != ERROR){
	switch(step){

	case 0:{ //Paso 1: Verificar si existe
			step = findPokemon(Pokemon->nombre_pokemon, &idPokemon);
			break;

			}
	case EXIST: { //Aca debo verificar si está abierto o no el archivo
				step = loadMetadataFile(&idPokemon, &dataPokemon);
				break;
				}

	case NOT_EXIST:{
				step = CreatePokemon(Pokemon);
				break;
			}
	case OPEN:{ //el Pokemon existe pero está abierto el archivo. Cada X tiempo debe reintentar
				free(idPokemon.pokemon);
				idPokemon.pokemon = NULL;
				pthread_mutex_lock(&mxLog);
				log_info(logger,"Id Mensaje: %d, El Pokemon %s está abierto, no se puede utilizar porque está siendo utilizado por otro hilo.",Pokemon->id_mensaje, Pokemon->nombre_pokemon);
				pthread_mutex_unlock(&mxLog);
				usleep(config_File->TIEMPO_DE_REINTENTO_OPERACION *1000000);
				step = 0;
				break;
			}
	case OK:{ //el Pokemon existe y se puede utilizar, entonces busco las posiciones que tiene.
				leerBloques(Pokemon, &dataPokemon, NEW_POKEMON);
				step = NW_UPDATE_POS;
				break;
			}
	case NW_UPDATE_POS:{
				t_positions* newPosition = malloc (sizeof(t_positions));;

				newPosition->Pos_x = Pokemon->posicion_x;
				newPosition->Pos_y = Pokemon->posicion_y;
				newPosition->Cantidad = Pokemon->cantidad;
				InsertUpdatePosition(newPosition, &dataPokemon);
				step = NW_SAVE;
				break;
			}
	case NW_SAVE:{
				dataPokemon.size = SavePositionInBlocks(&dataPokemon);
				usleep(config_File->TIEMPO_RETARDO_OPERACION *1000000);
				pthread_mutex_lock(&mxNewPokemonsList);
				pthread_mutex_lock(mxPokemones + idPokemon.idPokemon);
				update_metaData_files(&dataPokemon,&idPokemon);
				updateStatusFile(&idPokemon,"N");
				pthread_mutex_unlock(mxPokemones + idPokemon.idPokemon);
				pthread_mutex_unlock(&mxNewPokemonsList);
				step= RESPONDER; //cambiar esto, se debe seguir con enviar la respuesta a quien pidió la instrucción
				break;
			}
	case ERROR:{ //el Pokemon no existe
				pthread_mutex_lock(&mxLog);
				log_info(logger,"Id Mensaje: %d,El Pokemon no existe, se debe crear",Pokemon->id_mensaje);
				pthread_mutex_unlock(&mxLog);
				for(int i = 0;i<list_size(dataPokemon.positions);i++){
					t_positions* pos;// = malloc (sizeof(t_positions));
					pos = list_get(dataPokemon.positions,i);
					free(pos);
				}
				list_destroy(dataPokemon.positions);
				for(int i = 0;i<list_size(dataPokemon.blocks);i++){
					char* block = list_get(dataPokemon.blocks,i);
					free(block);
				}
				list_destroy(dataPokemon.blocks);
				free(idPokemon.pokemon);
				idPokemon.pokemon = NULL;
				return ERROR;
				break;
				}

	case RESPONDER:{ //el Proceso se hizo OK. Se debe responder al usuario con APPEARED_POKEMON
				pthread_mutex_lock(&mxLog);
				log_info(logger,"Id Mensaje: %d,El Proceso New Pokemon de %s finalizó correctamente.",Pokemon->id_mensaje, Pokemon->nombre_pokemon);
				pthread_mutex_unlock(&mxLog);
				step = ERROR;
				for(int i = 0;i<list_size(dataPokemon.positions);i++){
					t_positions* pos;// = malloc (sizeof(t_positions));
					pos = list_get(dataPokemon.positions,i);
					free(pos);
				}
				list_destroy(dataPokemon.positions);
				for(int i = 0;i<list_size(dataPokemon.blocks);i++){
					char* block = list_get(dataPokemon.blocks,i);
					free(block);
				}
				list_destroy(dataPokemon.blocks);
				free(idPokemon.pokemon);
				idPokemon.pokemon = NULL;

				return OK;
				break;
				}
	}
	}
return OK; //retornar resultado
}




int CatchPokemon(cola_CATCH_POKEMON* Pokemon){

	int step = 0;
	t_files dataPokemon;// = malloc (sizeof(t_files));
	t_Pokemones idPokemon;// = malloc (sizeof(t_Pokemones));
	while(step != ERROR){
	switch(step){

	case 0:{ //Paso 1: Verificar si existe
				step = findPokemon(Pokemon->nombre_pokemon, &idPokemon);
				break;

				}
	case EXIST: { //Aca debo verificar si está abierto o no el archivo
				step = loadMetadataFile(&idPokemon, &dataPokemon);
				break;
				}

	case NOT_EXIST_POSITION:{ //el Pokemon no existe, se debe informar el error
				pthread_mutex_lock(&mxLog);
				log_info(logger,"Id Mensaje: %d,La posición solicitada de %s no existe", Pokemon->id_mensaje,Pokemon->nombre_pokemon);
				pthread_mutex_unlock(&mxLog);
				usleep(config_File->TIEMPO_RETARDO_OPERACION *1000000);
				pthread_mutex_lock(mxPokemones + idPokemon.idPokemon);
				updateStatusFile(&idPokemon,"N");
				pthread_mutex_unlock(mxPokemones + idPokemon.idPokemon);
				step = ERROR;
				for(int i = 0;i<list_size(dataPokemon.positions);i++){
					t_positions* pos;// = malloc (sizeof(t_positions));
					pos = list_get(dataPokemon.positions,i);
					free(pos);
				}
				list_destroy(dataPokemon.positions);
				for(int i = 0;i<list_size(dataPokemon.blocks);i++){
					char* block = list_get(dataPokemon.blocks,i);
					free(block);
				}
				list_destroy(dataPokemon.blocks);
				free(idPokemon.pokemon);
				idPokemon.pokemon = NULL;
				return ERROR;
				break;
			}
	case NOT_EXIST:{ //el Pokemon no existe, se debe informar el error
				pthread_mutex_lock(&mxLog);
				log_info(logger,"Id Mensaje: %d,El Pokemon %s no existe",Pokemon->id_mensaje,Pokemon->nombre_pokemon);
				pthread_mutex_unlock(&mxLog);
				pthread_mutex_unlock(&mxPokeList);
				step = ERROR;

				return ERROR;
				break;
			}
	case OPEN:{ //el Pokemon existe pero está abierto el archivo. Cada X tiempo debe reintentar
				free(idPokemon.pokemon);
				idPokemon.pokemon = NULL;
				pthread_mutex_lock(&mxLog);
				log_info(logger,"Id Mensaje: %d,El Pokemon %s está abierto, no se puede utilizar no se puede utilizar porque está siendo utilizado por otro hilo.",Pokemon->id_mensaje, Pokemon->nombre_pokemon);
				pthread_mutex_unlock(&mxLog);
				usleep(config_File->TIEMPO_DE_REINTENTO_OPERACION *1000000);
				step = 0;
				break;
			}
	case OK:{ //el Pokemon existe y se puede utilizar, entonces busco las posiciones que tiene.
				pthread_mutex_lock(&mxLog);
				log_info(logger,"Id Mensaje: %d,El Pokemon %s está listo para utilizarse",Pokemon->id_mensaje, Pokemon->nombre_pokemon);
				pthread_mutex_unlock(&mxLog);
				leerBloques(Pokemon, &dataPokemon, CATCH_POKEMON);
				step = NW_UPDATE_POS;
				break;
			}

	case NW_UPDATE_POS:{
				t_positions* newPosition = malloc (sizeof(t_positions));
				newPosition->Pos_x = Pokemon->posicion_x;
				newPosition->Pos_y = Pokemon->posicion_y;
				newPosition->Cantidad = -1; //decrementa la posición
				//Intenta actualizar la posición. Si la encuentra continua grabandola en los bloques
				//Si no encuentra la posición, devuelve NOT_EXIST para que se le informa el broker el error.
				step = InsertUpdatePosition(newPosition, &dataPokemon);
				if(step == NOT_EXIST_POSITION || step == NW_SAVE_DEL_POSITION){
					free(newPosition);
				}
				break;
			}
	case NW_SAVE: case NW_SAVE_DEL_POSITION :{
				dataPokemon.size = SavePositionInBlocks(&dataPokemon);
				usleep(config_File->TIEMPO_RETARDO_OPERACION *1000000);
				pthread_mutex_lock(&mxNewPokemonsList);
				pthread_mutex_lock(mxPokemones + idPokemon.idPokemon);
				update_metaData_files(&dataPokemon,&idPokemon);
				updateStatusFile(&idPokemon,"N");
				pthread_mutex_unlock(mxPokemones + idPokemon.idPokemon);
				pthread_mutex_unlock(&mxNewPokemonsList);
				step= RESPONDER; //cambiar esto, se debe seguir con enviar la respuesta a quien pidió la instrucción
				break;
			}
	case ERROR:{ //el Pokemon no existe
				pthread_mutex_lock(&mxLog);
				log_info(logger,"Id Mensaje: %d,El Pokemon %s no existe, se debe crear",Pokemon->id_mensaje, Pokemon->nombre_pokemon);
				pthread_mutex_unlock(&mxLog);
				return ERROR;
				break;
				}

	case RESPONDER:{ //el Proceso se hizo OK. Se debe responder al usuario con APPEARED_POKEMON
				//printf("\nId Mensaje: %d, El Proceso finalizó correctamente.",Pokemon->id_mensaje);
			//	pthread_detach( pthread_self() );
			//	step= ERROR; //le pongo ERROR para que salga del switch nada mas.
				for(int i = 0;i<list_size(dataPokemon.positions);i++){
					t_positions* pos;
					pos =  list_get(dataPokemon.positions,i);
					free(pos);
				}
				list_destroy(dataPokemon.positions);
				for(int i = 0;i<list_size(dataPokemon.blocks);i++){
					char* block = list_get(dataPokemon.blocks,i);
					free(block);
				}
				list_destroy(dataPokemon.blocks);
				free(idPokemon.pokemon);
				idPokemon.pokemon = NULL;
				return OK;
				break;
				}
	}
	}
return OK; //retornar resultado
}



int GetPokemon(cola_GET_POKEMON* Pokemon, cola_LOCALIZED_POKEMON *locPokemon){

	int step = 0;
	t_files dataPokemon;// = malloc (sizeof(t_files));
	t_Pokemones idPokemon;// = malloc (sizeof(t_Pokemones));
	locPokemon->lista_posiciones = list_create();
	while(step != ERROR){
	switch(step){

	case 0:{ //Paso 1: Verificar si existe
				step = findPokemon(Pokemon->nombre_pokemon, &idPokemon);
				break;

				}
	case EXIST: { //Aca debo verificar si está abierto o no el archivo
				step = loadMetadataFile(&idPokemon, &dataPokemon);
				break;
				}

	case NOT_EXIST_POSITION:{ //el Pokemon no existe, se debe informar el error
				pthread_mutex_lock(&mxLog);
				log_info(logger,"Id Mensaje: %d,La posición no existe",Pokemon->id_mensaje);
				pthread_mutex_unlock(&mxLog);
				usleep(config_File->TIEMPO_RETARDO_OPERACION *1000000);
				pthread_mutex_lock(mxPokemones + idPokemon.idPokemon);
				updateStatusFile(&idPokemon,"N");
				pthread_mutex_unlock(mxPokemones + idPokemon.idPokemon);
				list_destroy(dataPokemon.positions);
				list_destroy(dataPokemon.blocks);
				free(dataPokemon.file);
//				free(idPokemon.pokemon);
				return ERROR;
				break;
			}
	case NOT_EXIST:{ //el Pokemon no existe, se debe informar el error
				pthread_mutex_lock(&mxLog);
				log_info(logger,"Id Mensaje: %d, el Pokemon %s no existe",Pokemon->id_mensaje, Pokemon->nombre_pokemon);
				pthread_mutex_unlock(&mxLog);
				pthread_mutex_unlock (&mxPokeList);
				step = ERROR;
				return ERROR;
				break;
			}
	case OPEN:{ //el Pokemon existe pero está abierto el archivo. Cada X tiempo debe reintentar
				free(idPokemon.pokemon);
				idPokemon.pokemon = NULL;
				pthread_mutex_lock(&mxLog);
				log_info(logger,"Id Mensaje: %d, el Pokemon %s está abierto, no se puede utilizar no se puede utilizar porque está siendo utilizado por otro hilo.",Pokemon->id_mensaje, Pokemon->nombre_pokemon);
				pthread_mutex_unlock(&mxLog);
				usleep(config_File->TIEMPO_DE_REINTENTO_OPERACION *1000000);
				step = 0;
				break;
			}
	case OK:{ //el Pokemon existe y se puede utilizar, entonces busco las posiciones que tiene.

				leerBloques(Pokemon, &dataPokemon,GET_POKEMON);

				list_add_all(locPokemon->lista_posiciones, dataPokemon.positions);
				usleep(config_File->TIEMPO_RETARDO_OPERACION *1000000);
				pthread_mutex_lock(&mxNewPokemonsList);
				pthread_mutex_lock(mxPokemones + idPokemon.idPokemon);
				updateStatusFile(&idPokemon,"N");
				pthread_mutex_unlock(mxPokemones + idPokemon.idPokemon);
				pthread_mutex_unlock(&mxNewPokemonsList);
				list_destroy(dataPokemon.positions);
				for(int i = 0;i<list_size(dataPokemon.blocks);i++){
					char* block = list_get(dataPokemon.blocks,i);
					free(block);
				}
				list_destroy(dataPokemon.blocks);
				free(idPokemon.pokemon);
				idPokemon.pokemon = NULL;
				return OK;
				break;
			}

	}
	}
return OK; //retornar resultado
}





int CreatePokemon(cola_NEW_POKEMON* Pokemon){

	char* dirFile = malloc( 1 + string_length(PuntoMontaje->FILES) + string_length(Pokemon->nombre_pokemon));
	strcpy(dirFile,PuntoMontaje->FILES);
	strcat(dirFile,Pokemon->nombre_pokemon);
	int result = mkdir(dirFile,0777);
	if (result  == 0){
		char* dirMetadata = malloc (1 + string_length(dirFile) + string_length("/Metadata.bin"));
		strcpy(dirMetadata,dirFile);
		strcat(dirMetadata,"/Metadata.bin");
		FILE *fMetadata = fopen (dirMetadata, "wb");
		if(fMetadata == NULL){
		pthread_mutex_lock(&mxLog);
		log_info(logger,"Id Mensaje: %d, No se puede crear el Metadata del Pokemon %s\n",Pokemon->id_mensaje, Pokemon->nombre_pokemon);
		pthread_mutex_unlock(&mxLog);
		return ERROR;
		}else{
		fprintf(fMetadata,"DIRECTORY=N\nOPEN=N\nSIZE=0\nBLOCKS=[]\n");
		}
		fclose(fMetadata);
		free(dirMetadata);

		t_Pokemones* newPokemon = malloc (sizeof(t_Pokemones));
		newPokemon->pokemon = malloc (string_length(Pokemon->nombre_pokemon) + 1) ;
		strcpy(newPokemon->pokemon,Pokemon->nombre_pokemon);
		/*Bloqueo lista por si viene otro hilo y quiere crear en ese momento otro pokemon*/
		newPokemon->idPokemon = list_size(pokeList);
		list_add(pokeList, newPokemon);
		addMxPokemon(newPokemon);
		pthread_mutex_lock(&mxLog);
		log_info(logger,"Id Mensaje: %d, Se crear el Pokemon %s\n", Pokemon->id_mensaje, Pokemon->nombre_pokemon);
		pthread_mutex_unlock(&mxLog);


	}else
	{
		free(dirFile);
		return ERROR;
	}
	free(dirFile);
	pthread_mutex_unlock (&mxPokeList);
	return result;
}

int InsertUpdatePosition(t_positions* newPos, t_files *posPokemon){

	for(int j = 0; j < list_size(posPokemon->positions) ; j++ ){
		t_positions* pos;// = malloc (sizeof(t_positions));
		pos = list_get(posPokemon->positions,j);
		if(pos->Pos_x == newPos->Pos_x && pos->Pos_y == newPos->Pos_y){
			newPos->Cantidad = pos->Cantidad + newPos->Cantidad;
			free(pos);
			list_remove(posPokemon->positions,j);
			if(newPos->Cantidad > 0){
				list_add(posPokemon->positions,newPos);
				return NW_SAVE; //Si encontró y actualizó la posición, retorna para que grabe los cambios.
			}else{
				return NW_SAVE_DEL_POSITION;
			}
		}
		//free(pos);
	}
	if(newPos->Cantidad > 0){
		list_add(posPokemon->positions,newPos);
	}
	return NOT_EXIST_POSITION; //Si no retorna la posición retorna ERROR.
}


/*void leerFiles(){
	cantFiles = 0;
	dirList = list_create();
	cargarArbolDirectorios(PuntoMontaje->FILES);
	printf("Elementos de la lista: %i",list_size(dirList));
	  for(int i = 0; i< cantFiles; i++){
			  t_files* archivo = malloc (sizeof(t_files));
			  archivo = list_get(dirList, i);
			  printf("\nArchivo: %s   Padre:%s   Tipo:%s ", archivo->file, archivo->parent, archivo->type);
			  if (string_equals_ignore_case(archivo->type,"N")){
				  printf("  Estado: %s Bloques: ", archivo->open);

				  for(int j = 0; j < list_size(archivo->blocks); j++ ){
					  printf("%s  ",list_get(archivo->blocks,j));
				  }
			  }
			  char* subDir = malloc (string_length(archivo->parent) + string_length(archivo->file) + 2);
			  strcpy(subDir,archivo->parent);
			  strcat(subDir,archivo->file);
			  strcat(subDir,"/");
			  cargarArbolDirectorios(subDir);

		  }

}

*/



/*
void cargarArbolDirectorios(char* Directorio){

	  DIR *dir;

	  struct dirent *ent;

	  dir = opendir (Directorio);


	  if (dir == NULL){
	    log_info(logger,"No se pudo cargar la estructura de Directorios");
	  }else{


	  while ((ent = readdir (dir)) != NULL)
	    {

	      if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) && (strcmp(ent->d_name, "Metadata.bin")!=0))
	      {


	    	  t_files* File = malloc (sizeof(t_files));
	    	  File->file = malloc (string_length(ent->d_name) + 1) ;
	    	  File->parent = malloc (string_length(Directorio) + 1) ;

	    	  strcpy(File->file,ent->d_name);
	    	  strcpy(File->parent,Directorio);

	    	  t_config *MetadataFiles;

	    	  char* dirMetadata = string_new();
	    	  int res = getMetadataDir(&dirMetadata, File);
	    	  MetadataFiles = config_create(dirMetadata);
	    	  File->type = string_duplicate(config_get_string_value(MetadataFiles,"DIRECTORY"));

	    	  if (config_has_property(MetadataFiles, "SIZE")){
	    		  File->size = config_get_int_value(MetadataFiles,"SIZE");
	    	  }
	    	  if (config_has_property(MetadataFiles, "OPEN")){
	    		  File->open = string_duplicate(config_get_string_value(MetadataFiles,"OPEN"));
	    	  }

	    	  if (config_has_property(MetadataFiles, "BLOCKS")){
	    		  char** BlocksAux = config_get_array_value(MetadataFiles, "BLOCKS");
	    		  File->blocks = list_create();
	    		  int posicionLista = 0;
	    		  while(BlocksAux[posicionLista] != NULL) {
	    			  list_add(File->blocks, string_duplicate(BlocksAux[posicionLista]));
	    			  posicionLista++;
	    		  }
	    	  }

	    	  list_add(dirList, File);

	    	  cantFiles++;
	    	}
	    }
	  	  closedir (dir);
	  }
}
*/

int leer_metaData_files(){

	config_MetaData = reservarMemoria(sizeof(t_config_MetaData));

	t_config *archivo_MetaData;
	archivo_MetaData=config_create(PuntoMontaje->METADATA_FILE);
	config_MetaData->cantidad_bloques=config_get_int_value(archivo_MetaData,"BLOCKS");
    config_MetaData->magic_number=string_duplicate(config_get_string_value(archivo_MetaData,"MAGIC_NUMBER"));
	config_MetaData->tamanio_bloques=config_get_int_value(archivo_MetaData,"BLOCK_SIZE");

	return 0;
}

int update_metaData_files(t_files *dataPokemon,t_Pokemones *idPokemon){

	t_config *MetadataFiles;
	char* dirMetadata = string_new();
	int res = getMetadataDir(&dirMetadata, idPokemon);
	MetadataFiles = config_create(dirMetadata);

	if (config_has_property(MetadataFiles, "SIZE")){
		char * tempSize = string_itoa(dataPokemon->size);
		/*	char* tempSize = malloc(1+string_length(string_itoa(dataPokemon->size)));
			strcpy(tempSize,string_itoa(dataPokemon->size));*/
			config_set_value(MetadataFiles,"SIZE", tempSize);
			pthread_mutex_lock(&mxLog);
			log_info(logger,"Se utilizaron %s bytes para el Pokemon %s\n", tempSize,idPokemon->pokemon);
			pthread_mutex_unlock(&mxLog);
			free(tempSize);
			tempSize=NULL;
	}
	if (config_has_property(MetadataFiles, "BLOCKS")){
		char* tempBlocks = (char*) malloc(string_length("[")+string_length("]"));
		strcpy(tempBlocks,"[");
		for(int i = 0; i< list_size(dataPokemon->blocks); i++){
			char* bloque = list_get(dataPokemon->blocks,i);
			tempBlocks= realloc(tempBlocks, 1+ strlen(tempBlocks) + strlen(bloque));
			strcat(tempBlocks,bloque);
			if(i+1 < list_size(dataPokemon->blocks) ){
				tempBlocks= realloc(tempBlocks,1 + strlen(tempBlocks) + strlen(","));
				strcat(tempBlocks,",");
			}
		}
		tempBlocks= realloc(tempBlocks, 1 + strlen(tempBlocks) + strlen("]"));
		strcat(tempBlocks,"]");
		config_set_value(MetadataFiles,"BLOCKS", tempBlocks);
		pthread_mutex_lock(&mxLog);
		log_info(logger,"Se asignaron los bloques %s para el Pokemon %s\n", tempBlocks,idPokemon->pokemon);
		pthread_mutex_unlock(&mxLog);
		free(tempBlocks);
	}

	config_save(MetadataFiles);
	config_destroy(MetadataFiles);
	free(dirMetadata);
	dirMetadata=NULL;
}




int findPokemon(char* Pokemon,t_Pokemones *idPokemon){

	int exist = 0;

	pthread_mutex_lock (&mxPokeList);
	for(int j = 0; j < list_size(pokeList); j++ ){
		//t_Pokemones* tempPokemon = malloc(sizeof(t_Pokemones));
		t_Pokemones* tempPokemon = list_get(pokeList,j);
		if (string_equals_ignore_case(tempPokemon->pokemon,Pokemon)){   //Recorriendo se fija si encuentra el archivo en el FS

					pthread_mutex_unlock (&mxPokeList);
					idPokemon->pokemon = malloc(1 + string_length(tempPokemon->pokemon));
					strcpy(idPokemon->pokemon , tempPokemon->pokemon);
					idPokemon->idPokemon = tempPokemon->idPokemon;
					return EXIST;
			}
		}
	return NOT_EXIST; //No se encontró el Pokemon, no desbloqueo la lista porque lo tengo que agregar.
}


//Función que cambia el estado del Pokemon a abierto para que nadie mas pueda utilizarlo.
int updateStatusFile(t_Pokemones* tempPokemon, char* estado){

	t_config *archivo_MetaData;
	char* dirMetadata = string_new();
	int res = getMetadataDir(&dirMetadata, tempPokemon);
	archivo_MetaData=config_create(dirMetadata);
	config_set_value(archivo_MetaData, "OPEN", estado);
	config_save(archivo_MetaData);
	config_destroy(archivo_MetaData);
	free(dirMetadata);
	dirMetadata=NULL;
	pthread_mutex_lock(&mxLog);
	log_info(logger,"Se modifica el estado del archivo de %s a %s.",tempPokemon->pokemon, estado);
	pthread_mutex_unlock(&mxLog);
	return OK;
}

int loadMetadataFile(t_Pokemones* tempPokemon, t_files *dataPokemon){

	t_config *archivo_MetaData;
	char* dirMetadata = string_new();
	int res = getMetadataDir(&dirMetadata, tempPokemon);
	pthread_mutex_lock(&mxNewPokemonsList);
	pthread_mutex_lock(mxPokemones + tempPokemon->idPokemon);
	archivo_MetaData=config_create(dirMetadata);
	if (config_has_property(archivo_MetaData, "OPEN")){
   	  if( string_equals_ignore_case(config_get_string_value(archivo_MetaData,"OPEN"),"N")){
   		  updateStatusFile(tempPokemon,"Y");
   		  pthread_mutex_unlock(mxPokemones + tempPokemon->idPokemon);
   		pthread_mutex_unlock(&mxNewPokemonsList);
   		 if (config_has_property(archivo_MetaData, "SIZE")){
   			    dataPokemon->size = config_get_int_value(archivo_MetaData,"SIZE");
   		 	 }
   		 if (config_has_property(archivo_MetaData, "BLOCKS")){
   			    char** BlocksAux = config_get_array_value(archivo_MetaData, "BLOCKS");
   			    dataPokemon->blocks = list_create();
   			    int posicionLista = 0;
   			    while(BlocksAux[posicionLista] != NULL) {
   			    	char* tmpBlock = malloc(string_length(BlocksAux[posicionLista]) + 1);
   			    	strcpy(tmpBlock,BlocksAux[posicionLista]);
   			    	list_add(dataPokemon->blocks, tmpBlock );
   			    	posicionLista++;
   			    }
   			 for(int a = 0;a < list_size(dataPokemon->blocks);a++){
   			 	free(BlocksAux[a]);
   			}
   			    free(BlocksAux);
   		 }
   		config_destroy(archivo_MetaData);
   		free(dirMetadata);
   		dirMetadata=NULL;
   		  return OK;
		 }
   	  else{
   		  pthread_mutex_unlock(mxPokemones + tempPokemon->idPokemon);
   		pthread_mutex_unlock(&mxNewPokemonsList);
   		config_destroy(archivo_MetaData);
   		free(dirMetadata);
   		dirMetadata=NULL;
   		  return OPEN;
   	  }
	}
	pthread_mutex_unlock(mxPokemones + tempPokemon->idPokemon);
	pthread_mutex_unlock(&mxNewPokemonsList);
	config_destroy(archivo_MetaData);
	free(dirMetadata);
	dirMetadata=NULL;
	return ERROR;
}

void getMetadataDir(char** result, t_Pokemones* tempPokemon){

	char* dirMetadata = (char*) malloc( string_length(PuntoMontaje->FILES) +string_length(tempPokemon->pokemon) + string_length("/Metadata.bin") + 1 );
	strcpy(dirMetadata, PuntoMontaje->FILES);
	string_append(&dirMetadata, tempPokemon->pokemon);
	string_append(&dirMetadata, "/Metadata.bin");

	*result = realloc(*result, strlen(*result) + strlen(dirMetadata) + 1);
	string_append(result,dirMetadata);
	free(dirMetadata);
	dirMetadata = NULL;
}




void leerBloques(cola_NEW_POKEMON* Pokemon, t_files *dataPokemon,int tipo)
{
	t_files* tempPokemon = malloc(sizeof(t_files));
	char* lecturaBloques;// = string_new();
	lecturaBloques = malloc (1 + dataPokemon->size);//config_MetaData->tamanio_bloques * list_size(tempPokemon->blocks));
	strcpy(lecturaBloques,"");
	int tamanio_a_leer = dataPokemon->size;
	for(int i = 0; i< list_size(dataPokemon->blocks); i++){
		FILE* block;
		char* bloque = list_get(dataPokemon->blocks,i);
		char* dirBloque = malloc(1 + string_length(PuntoMontaje->BLOCKS)+ string_length(bloque)+ string_length(".bin"));
		strcpy(dirBloque,PuntoMontaje->BLOCKS);
		strcat(dirBloque,bloque);
		strcat(dirBloque,".bin");
		block = fopen(dirBloque,"r");

		int desplazamiento = 0;
		char cadena[config_MetaData->tamanio_bloques];
		while (fgets(cadena,config_MetaData->tamanio_bloques,(FILE*) block) != NULL)
			strcat(lecturaBloques,cadena);
		fclose(block);
		free(dirBloque);
		dirBloque=NULL;
		}

		dataPokemon->positions = list_create();
		char** strPos = string_split(lecturaBloques,"\n");
		//t_list* tempPos = list_create();
		int i = 0;

		while(strPos[i] != NULL){

			char** posCant = string_split(strPos[i], "=");
			char** coordenadas = string_split(posCant[0],"-");
		if (tipo != GET_POKEMON){
			t_positions *readedPos= malloc (sizeof(t_positions));
			readedPos->Pos_x=atoi(coordenadas[0]);
			readedPos->Pos_y =atoi(coordenadas[1]);
			readedPos->Cantidad = atoi(posCant[1]);
			list_add(dataPokemon->positions, readedPos);

		}else{
			posicion *readedPos= malloc (sizeof(t_positions));
			readedPos->posicion_x=atoi(coordenadas[0]);
			readedPos->posicion_y=atoi(coordenadas[1]);

			list_add(dataPokemon->positions, readedPos);
		}

			i++;
			/*free(pos);
			for(int a = 0;a < 2;a++){
						free(posCant[a]);
						free(coordenadas[a]);
			}*/
			free(posCant[0]);
			free(posCant[1]);
			free(coordenadas[0]);
			free(coordenadas[1]);
			free(posCant);
			free(coordenadas);
		}
		for(int a = 0;a < list_size(dataPokemon->positions);a++){
			free(strPos[a]);
		}
		free(strPos);
		free(tempPokemon);
		free(lecturaBloques);
		lecturaBloques=NULL;

}


int SavePositionInBlocks(t_files *dataPokemon){
	//Limpio los bloques existentes. Reescribo todos los bloques para evitar fragmentación interna,
	//o que el tamaño del bloque se exceda del límite.
	CleanBlocks(dataPokemon);
	char* buffer = string_new();
	int bloquesUsados = 0;
	bloquesUsados = list_size(dataPokemon->blocks);
	serializarPosiciones(&buffer,dataPokemon);

	int sizeBuffer = string_length(buffer);
	int resultBuffer = sizeBuffer;
	//Me fijo cuantos bloques necesito para grabar la información actualizada.
	int bloquesNecesarios = 0;
	if((string_length(buffer)%config_MetaData->tamanio_bloques)==0){
		bloquesNecesarios = sizeBuffer/config_MetaData->tamanio_bloques;
	}else{
		bloquesNecesarios = (sizeBuffer/config_MetaData->tamanio_bloques) + 1;
	}

	/*1° Verifico si tengo la cantidad de bloques que necesito.
	 * 2° Si necesito los mismos o mas bloques
	 * */

if (bloquesNecesarios <= bloquesUsados + cantidadDeBloquesLibres()){

	t_list* bloquesExtras = list_create();

	for (int i = 0; i < (bloquesNecesarios - bloquesUsados); i++) //Si necesito bloques extra, ya los reservo para que otro hilo no me los utilice.
	{
		int nextBlock = proximobloqueLibre();
		list_add(bloquesExtras,nextBlock);
	}

	int desplazamiento = 0;
	for(int i = 0; i<bloquesNecesarios;i++)
	{
		char* bloque = list_get(dataPokemon->blocks,i);
		int intBloque;
		if(bloque != NULL){
			intBloque =atoi(bloque);
		}else{
			intBloque = list_get(bloquesExtras,0);
			list_add(dataPokemon->blocks, string_itoa(intBloque));
			list_remove(bloquesExtras,0);
		}

		char* tempData = string_substring(buffer, desplazamiento,config_MetaData->tamanio_bloques);
		grabarBloque(tempData,intBloque);
		free(tempData);
		sizeBuffer -= config_MetaData->tamanio_bloques;
		desplazamiento += config_MetaData->tamanio_bloques;

	}

	//Ahora debo liberar los bloques de mas
		while( bloquesNecesarios < list_size(dataPokemon->blocks))
		{
			int bit = atoi(list_get(dataPokemon->blocks,bloquesNecesarios));
			char* tmpBlock = list_get(dataPokemon->blocks,bloquesNecesarios);
			free (tmpBlock);
			list_remove(dataPokemon->blocks,bloquesNecesarios);
			pthread_rwlock_wrlock(&mxBitmap);
			bitarray_clean_bit(bitarray,bit);
			pthread_rwlock_unlock(&mxBitmap);
		}

	list_destroy(bloquesExtras);
	free(buffer);
	return resultBuffer;
}

else{
	pthread_mutex_lock(&mxLog);
	log_info(logger,"No hay bloques suficientes para grabar lo solicitado. Se descarta solicitud.");
	pthread_mutex_unlock(&mxLog);
	return ERROR;
}

}

void CleanBlocks(t_files *dataPokemon){

	for(int i = 0; i< list_size(dataPokemon->blocks); i++){
		FILE* block;
		char* bloque = list_get(dataPokemon->blocks,i);
		char* dirBloque = (char*) malloc(1 + string_length(PuntoMontaje->BLOCKS)+ string_length(bloque)+ string_length(".bin"));
		strcpy(dirBloque,PuntoMontaje->BLOCKS);
		strcat(dirBloque,bloque);
		strcat(dirBloque,".bin");
		block = fopen(dirBloque,"w");
		fclose(block);
		free(dirBloque);
	}
}

void serializarPosiciones(char** buffer, t_files *dataPokemon){

	for(int j = 0; j < list_size(dataPokemon->positions) ; j++ ){
			t_positions* BufferPos = list_get(dataPokemon->positions,j);
			char* tempPos_X = string_itoa(BufferPos->Pos_x);
			char* tempPos_Y = string_itoa(BufferPos->Pos_y);
			char* tempCantidad = string_itoa(BufferPos->Cantidad);

			char* temp = malloc(1 + string_length(tempPos_X) + string_length("-") +string_length(tempPos_Y) + string_length("=") +string_length(tempCantidad)+ string_length("\n"));
			strcpy(temp,tempPos_X);
			strcat(temp,"-");
			strcat(temp,tempPos_Y);
			strcat(temp,"=");
			strcat(temp,tempCantidad);
			strcat(temp,"\n");
			*buffer = realloc(*buffer,string_length(*buffer) + string_length(temp) + 1);
			//strcat(buffer,temp);
			string_append(buffer,temp);
			free(tempPos_X);
			free(tempPos_Y);
			free(tempCantidad);
			free(temp);

	}

}


int grabarBloque(char* data, int bloque)
{
	FILE *block;

	char* tempBloque = string_itoa(bloque);
	char* dirBloque =  malloc(1 + string_length(PuntoMontaje->BLOCKS)+ string_length(tempBloque)+ string_length(".bin"));
	strcpy(dirBloque,PuntoMontaje->BLOCKS);
	strcat(dirBloque,tempBloque);
	strcat(dirBloque,".bin");

	block = fopen (dirBloque, "w");
	//printf("dirBloque: %s\n",dirBloque);
	if(block != NULL)
	{
		fprintf(block,"%s",data);
		pthread_rwlock_wrlock(&mxBitmap);
		bitarray_set_bit(bitarray, bloque);
		pthread_rwlock_unlock(&mxBitmap);
		fclose (block);
	}else
	{
		pthread_mutex_lock(&mxLog);
		log_info(logger,"\nNo se pudo grabar el bloque %i", bloque);
		pthread_mutex_unlock(&mxLog);
	}


	int acum=0;
	int i;
	for(i=0; i<string_length(data); i++) // se fija cuantas veces aparece el \n para no contarlo entre los bytes que dumpeo.
	{
		 if(data[i]=="\n")
		 {
	        acum++;

		 }
	}

	free(tempBloque);
	free(dirBloque);


	return string_length(data) - acum;
}


