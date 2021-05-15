#include "Inicializacion.h"
#include <commons/config.h>
#include "src/sockets.h"
#include "src/mensajeria.h"
#include <pthread.h>


#define OPEN 2
#define NW_UPDATE_POS 3
#define NW_INSERT_POS 4
#define NW_SAVE 5
#define EXIST 8
#define NOT_EXIST 6
#define NOT_EXIST_POSITION 9
#define RESPONDER 7
#define CHECK_POSITION 8
#define NW_SAVE_DEL_POSITION 10


typedef struct{
	char* file;
	/*char* parent;
	char* type;
	char* open;*/
	int size;
	t_list* blocks;
	t_list* positions;
}t_files;






int NewPokemon(cola_NEW_POKEMON* Pokemon);

