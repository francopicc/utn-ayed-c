#include <stdio.h>
#include <stdlib.h>

// Playero en galpón de camiones: un playero que realiza el control de un galpón (que trabaja con empresas que realizan envíos en camiones) realiza el control de móviles que ingresan. El funcionamiento es el siguiente: 1) El playero mantiene un listado ordenado de camiones que alguna vez arribaron al galpón. 2) Cada vez que un camión ingresa al galpón, debe ser controlado para aparcar, y se verifica si ya estuvo antes ahí. 3) En caso de ser un camión ya registrado se actualizan los siguientes datos: origen, tipo de carga, día de arribo programado, peso de la carga y chofer. 4) En caso de ser un camión no registrado, se registra con los datos del punto anterior más los siguientes: capacidad real, capacidad permitida, empresa, tipo de camión e identificador (dato por el cual queda la lista ordenada). 5) Una vez ingresado al galpón, se asigna una prioridad de 1 a 15 para el control de acuerdo a varios datos (como retraso que lleva, incidentes registrados, etc.) y se encola en lo que será la cola de ingreso al control migratorio. 6) Una vez ingresados los camiones al galpón, comienzan a pasar por el control, registrando el mismo en una lista ordenada, con el retraso registrado y exceso de carga en cada caso. 7) Al finalizar los controles se desea recorrer recursivamente el listado indicando la cantidad de camiones que se excedieron en el tiempo programado y los que se excedieron en la carga permitida ese día. 
 
// Estructuras de datos - 1. Lista ordenada de camiones que llegaron alguna vez, 2. Cola, 3. Lista ordenada final

struct camion {
	char origen[30];
	char tipo_carga[30];
	int dd_arribo;
	int peso;
	char chofer[30];
	int capacidad_real;
	int capacidad_permitida;
	char empresa[30];
	char tipo_camion[30];
	int identificador;
};

struct nodo {
	struct camion c;
	struct nodo *sig;	
};

struct nodo_ctrl {
	struct camion c;
	struct nodo *sig;
	float retraso, exceso_carga;
};

struct nodo *raiz_lista_inicio = NULL;

struct nodo *raiz_cola = NULL;
struct nodo *fondo_cola = NULL;

struct nodo_ctrl *raiz_lista_ctrl = NULL;

// Lista Ordenada

void insertar(struct nodo **lista, struct camion c) {
	struct nodo *nuevo;
	nuevo = malloc(sizeof(struct nodo));
	if(!nuevo) return;
	nuevo->c = c;
	nuevo->sig = NULL;
	
	if(*lista == NULL) {
		*lista = nuevo;
	} else {
		if(c.identificador < (*lista)->c.identificador) {
			nuevo->sig = *lista;
			*lista = nuevo;
		} else {
			struct nodo *reco = *lista;
			struct nodo *atras = *lista;
			while(c.identificador >= reco->c.identificador && reco->sig != NULL) {
				atras = reco;
				reco = reco->sig;
			}
			if(c.identificador >= reco->c.identificador) {
				reco->sig = nuevo;
			} else {
				nuevo->sig = reco;
				atras->sig = nuevo;
			}
		}
	}
}

void insertar_ctrl(struct nodo_ctrl **lista, struct nodo **l) {
	struct nodo_ctrl *nuevo;
	nuevo = malloc(sizeof(struct nodo_ctrl));
	if(!nuevo) return;
	nuevo->c = (*l)->c;
	nuevo->sig = NULL;
	
	printf("\nIngrese el retraso: ");
	scanf("%f", &nuevo->retraso);
	printf("\nIngrese el exceso de carga: ");
	scanf("%f", &nuevo->exceso_carga);
	
	
	if(*lista == NULL) {
		*lista = nuevo;
	} else {
		if((*l)->c.identificador < (*lista)->c.identificador) {
			nuevo->sig = *lista;
			*lista = nuevo;
		} else {
			struct nodo *reco = *lista;
			struct nodo *atras = *lista;
			while((*l)->c.identificador >= reco->c.identificador && reco != NULL) {
				atras = reco;
				reco = reco->sig;
			}
			if((*l)->c.identificador >= reco->c.identificador) {
				reco->sig = nuevo;
			} else {
				nuevo->sig = reco;
				atras->sig = nuevo;
			}
		}
	}
}

// Cola

void encolar (struct nodo **cola, struct nodo **fondo, struct nodo **lista) {
	struct nodo *nuevo;
	nuevo = malloc(sizeof(struct nodo));
	if(!nuevo || *lista == NULL) return;
	nuevo->c = (*lista)->c;
	nuevo->sig = NULL;
	
	if(*cola == NULL) {
		*cola = nuevo;
		*fondo = nuevo;
	} else {
		(*fondo)->sig = nuevo;
		*fondo = nuevo;
	}
}

void desencolar (struct nodo **cola) {
	if(*cola == NULL) return;
	struct nodo *bor = *cola;
	*cola = (*cola)->sig;
	free(bor);
}

// Funciones

void cargar (struct camion *c, int identificador) {
	c->identificador = identificador;
	printf("\nIngrese el origen: ");
	scanf("%s", c->origen);
	printf("\nIngrese el tipo de carga: ");
	scanf("%s", c->tipo_carga);
	printf("\nIngrese el dia de arribo programado: ");
	scanf("%i", &c->dd_arribo);
	printf("\nIngrese el peso de la carga: ");
	scanf("%i", &c->peso);
	printf("\nIngrese el chofer: ");
	scanf("%s", c->chofer);
	printf("\nIngrese la capacidad real: ");
	scanf("%i", &c->capacidad_real);
	printf("\nIngrese la capacidad permitida: ");
	scanf("%i", &c->capacidad_permitida);
	printf("\nIngrese la empresa: ");
	scanf("%s", c->empresa);
	printf("\nIngrese el tipo de camion: ");
	scanf("%s", c->tipo_camion);
}

int buscar_v (struct nodo **lista, int identificador) {
	if(*lista == NULL) return 0;
	struct nodo *bor = *lista;
	while(bor != NULL) {
		if(identificador == bor->c.identificador) {
			return 1;
		}
		bor = bor->sig;
	}
	return 0;
}

void buscar (struct nodo **lista, int identificador) {
	if(*lista == NULL) return;
	struct nodo *bor = *lista;
	while(bor != NULL) {
		if(identificador == bor->c.identificador) {
			printf("\nIngrese el origen: ");
			scanf("%s", bor->c.origen);
			printf("\nIngrese el tipo de carga: ");
			scanf("%s", bor->c.tipo_carga);
			printf("\nIngrese el dia de arribo programado: ");
			scanf("%i", &bor->c.dd_arribo);
			printf("\nIngrese el peso de la carga: ");
			scanf("%i", &bor->c.peso);
			printf("\nIngrese el chofer: ");
			scanf("%s", bor->c.chofer);
		}
		bor = bor->sig;
	}
}

int recorrer_recursivamente_tiempo (struct nodo_ctrl **l) {
	if(*l == NULL) return 0;
	
	if((*l)->retraso >= 1) {
		return 1 + recorrer_recursivamente_tiempo(&(*l)->sig);
	}
	
	return recorrer_recursivamente_tiempo(&(*l)->sig);

}

int recorrer_recursivamente_carga (struct nodo_ctrl **l) {
	if(*l == NULL) return 0;
	
	if((*l)->exceso_carga >= (*l)->c.capacidad_permitida) {
		return 1 + recorrer_recursivamente_carga(&(*l)->sig);
	}
	
	return recorrer_recursivamente_carga(&(*l)->sig);

}


int main () {
	int hay_camiones = 1, identificador = 0;
	struct camion n;
	while(hay_camiones != 0) {
		printf("\nIngrese el identificador: ");
		scanf("%i", &identificador);
		if(buscar_v(&raiz_lista_inicio, identificador)) {
			buscar(&raiz_lista_inicio, identificador);
		} else {
			cargar(&n, identificador);
			insertar(&raiz_lista_inicio, n);
		}
		encolar(&raiz_cola, &fondo_cola, &raiz_lista_inicio);
		printf("Hay mas camiones (1- SI - 0- NO)");
		scanf("%i", &hay_camiones);
	}
	
	while(raiz_cola != NULL) {
		insertar_ctrl(&raiz_lista_ctrl, &raiz_cola);
		desencolar(&raiz_cola);
	}
	
	printf("\nHubo %i camiones con retraso", recorrer_recursivamente_tiempo(&raiz_lista_ctrl));
	printf("\nHubo %i camiones con exceso de carga", recorrer_recursivamente_carga(&raiz_lista_ctrl));
	
	printf("\n");
	exit(0);
	return 0;
}
