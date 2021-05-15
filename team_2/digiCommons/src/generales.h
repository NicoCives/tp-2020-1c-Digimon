/*
 * generales.h
 *
 *  Created on: 29 abr. 2020
 *      Author: utnso
 */

#ifndef SRC_GENERALES_H_
#define SRC_GENERALES_H_

#include "protocolos_comunicacion.h"

unsigned long long obtener_timestamp();
unsigned int token() ;
int devolverTipoMsj (char * comando);
char * tipoMsjIntoToChar (int head);
char * devolverModulo(int modulo);
int devolverEnumModulo (char * modulo);


#endif /* SRC_GENERALES_H_ */
