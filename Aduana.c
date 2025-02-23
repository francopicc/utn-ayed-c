#include <stdio.h>
#include <stdlib.h>

// En la aduana se administran los depósitos con una lista ordenada por código interno. Los elementos en el deposito mantienen la siguiente información: 1) Código aduanero (numero de 10 dígitos); 2) Fecha de ingreso a la aduana; 3) Lugar de origen (cadena 20 caracteres); 4) Despachante (cadena 20 caracteres); 5) Destino (cadena 30 caracteres); 6) Destinatario (cadena 20 caracteres); 7) DNI destinatario; 8) Peso del paquete (real 5,2); y 9) Impuesto (real 5,2). Al comienzo del día se reciben todos los depósitos nuevos en una cola, para ser agregados al listado principal. Una vez agregados los nuevos elementos, comienza la atención al público. Cada vez que se presenta alguien a buscar un paquete, se busca en la lista por número de DNI, se entrega el producto (removiéndolo de la lista) y se cobra el impuesto correspondiente. Al finalizar el día, debe informarse lo recaudado por impuestos y recorrer el listado recursivamente para remover y apilar los elementos que hace 20 días llegaron y no fueron retirados.

// Estructuras de datos - 1. Lista ordenada por codigo 2. Cola 3. Pila de elementos que hace 20 dias no se retiran

struct fecha {
	int dd;
	int mm;
};

struct deposito {
	int codigo;
	struct fecha ingreso;
	char origen[30];
	char despachante[20];
	char destino[30];
	char destinatario[20];
	int dni_destinatario;
	float peso;
	float impuesto;
	int antiguedad;
};

struct nodo {
	struct deposito d;
	struct nodo *sig;
};

struct nodo *raiz_lista_ordenada = NULL;

struct nodo *raiz_cola = NULL;
struct nodo *fondo_cola = NULL;

struct nodo *raiz_pila = NULL;

// Lista

void insertar(struct nodo **lista, struct nodo **cola) {
	struct nodo *nuevo;
	nuevo = malloc(sizeof(struct nodo));
	if(!nuevo) return;
	nuevo->d = (*cola)->d;
	nuevo->sig = NULL;
	
	if(*lista == NULL) {
		*lista = nuevo;
	} else {
		if((*cola)->d.codigo < (*lista)->d.codigo) {
			nuevo->sig = *lista;
			*lista = nuevo;
		} else {
			struct nodo *reco = *lista;
			struct nodo *atras = *lista;
			while((*cola)->d.codigo >= reco->d.codigo && reco->sig == NULL) {
				atras = reco;
				reco = reco->sig;
			}
			
			if((*cola)->d.codigo >= reco->d.codigo) {
				reco->sig = nuevo;
			} else {
				nuevo->sig = reco;
				atras->sig = nuevo;
			}
		}
	}
}

void extraer (struct nodo **lista, int pos) {
	if(*lista == NULL || pos < 0) return;
	
	struct nodo *reco = *lista;
	struct nodo *atras = NULL;
	
	if(pos == 0) {
		*lista = reco->sig;
		free(reco);
		return;
	}
	
	for(int i = 0; reco != NULL && i < pos; i++) {
		atras = reco;
		reco = reco->sig;
	}
	
	if(reco == NULL) return;
	
	atras->sig = reco->sig;
	free(reco);
}

// Cola

void encolar(struct nodo **cola, struct nodo **fondo, struct deposito d) {
	struct nodo *nuevo;
	nuevo = malloc(sizeof(struct nodo));
	if(!nuevo) return;
	nuevo->d = d;
	nuevo->sig = NULL;
	
	if(*cola == NULL) {
		*cola = nuevo;
		*fondo = nuevo;
	} else {
		(*fondo)->sig = nuevo;
		*fondo = nuevo;
	}
}

void desencolar(struct nodo **cola) {
	if(*cola == NULL) return;
	struct nodo *bor = *cola;
	
	*cola = (*cola)->sig;
	free(bor);
}

// Pila

void apilar (struct nodo **pila, struct nodo **lista) {
	struct nodo *nuevo;
	nuevo = malloc(sizeof(struct nodo));
	if(!nuevo) return;
	nuevo->d = (*lista)->d;
	nuevo->sig = NULL;
	
	if(*pila == NULL) {
		*pila = nuevo;
		(*pila)->sig = NULL;
	} else {
		nuevo->sig = *pila;
		*pila = nuevo;
	}
}

// Funciones

void cargar (struct deposito *d) {
	printf("\nIngrese el codigo aduanero: ");
	scanf("%i", &d->codigo);
	printf("\nIngrese la fecha de ingreso a la aduana: (dia)");
	scanf("%i", &d->ingreso.dd);
	printf("\nIngrese la fecha de ingreso a la aduana: (mes)");
	scanf("%i", &d->ingreso.mm);
	printf("\nIngrese el lugar de origen: ");
	scanf("%s", d->origen);
	printf("\nIngrese el despachante: ");
	scanf("%s", d->despachante);
	printf("\nIngrese el destino: ");
	scanf("%s", d->destino);
	printf("\nIngrese el destinatario: ");
	scanf("%s", d->destinatario);
	printf("\nIngrese el DNI Destinatario: ");
	scanf("%i", &d->dni_destinatario);
	printf("\nIngrese el peso del paquete: ");
	scanf("%f", &d->peso);
	printf("\nIngrese el impuesto: ");
	scanf("%f", &d->impuesto);
	printf("\nIngrese antiguedad: ");
	scanf("%i", &d->antiguedad);
}

float buscar (struct nodo **lista, int dni) {
	if(*lista == NULL) return 0;
	int pos = 0;
	struct nodo *bor = *lista;
	while(bor != NULL) {
		if(dni == bor->d.dni_destinatario) {
			extraer(&(*lista), pos);
			return bor->d.impuesto;
		}
		pos++;
		bor = bor->sig;
	}
}

int recorrer_recursivamente(struct nodo **lista, struct nodo **pila) {
	if(*lista == NULL) return;
	
	if((*lista)->d.antiguedad >= 20) {
		apilar(&raiz_pila, &raiz_lista_ordenada);
	}
	
	return recorrer_recursivamente(&(*lista)->sig, &(*pila)->sig);
}

int main () {
	int hay_clientes = 1, mas_productos = 1, dni = 0, imp = 0;
	float impuesto = 0;
	struct deposito d;
	while(mas_productos) {
		cargar(&d);
		encolar(&raiz_cola, &fondo_cola, d);
		printf("\nExisten mas productos: 1 - SI . 0 - NO");
		scanf("%i", &mas_productos);
	}
	
	while(raiz_cola != NULL) {
		insertar(&raiz_lista_ordenada, &raiz_cola);
		desencolar(&raiz_cola);
	}
	
	while(hay_clientes) {
		printf("\nIngrese el DNI destinatario: ");
		scanf("%i", &dni);
		imp = buscar(&raiz_lista_ordenada, dni);
		impuesto += imp;
		printf("\nIndique si hay mas clientes: 1 - SI . 0 - NO");
		scanf("%i", &hay_clientes);
	}
	
	printf("\nTotal recaudado por impuestos: %.2f", impuesto);
	recorrer_recursivamente(&raiz_lista_ordenada, &raiz_pila);
	printf("\n");
	exit(0);
	return 0;
}
