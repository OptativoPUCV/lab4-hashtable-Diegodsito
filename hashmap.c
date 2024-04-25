#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  if(map==NULL || key==NULL) return;
  
  long posicion = hash(key , map->capacity);
    
  Pair *current = map->buckets[posicion];
    
  while(current != NULL){//Si esta la key
      
    if(is_equal(current->key , key)){
      map->buckets[posicion]->value = value;
      return;
    }
      
    posicion = (posicion + 1) % map->capacity;
    current = map->buckets[posicion];
  }

  Pair *nuevoPair = createPair(key,value);
  map->buckets[posicion] = nuevoPair;
  map->size++;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
}


HashMap * createMap(long capacity) {

  HashMap * map = (HashMap *)malloc(sizeof(HashMap));
  map->buckets = (Pair **)calloc(capacity, sizeof(Pair *));
  map->size = 0;
  map->capacity = capacity;
  map->current = -1;
  return map;
    return NULL;
}

void eraseMap(HashMap * map,  char * key) {   
  if(map==NULL || key==NULL) return;

  long posicion = hash(key , map->capacity);
  Pair *current =map->buckets[posicion];

  while(current != NULL){
    
    if(is_equal(current->key, key)){
      map->buckets[posicion]->key = NULL;
      return;
    }
    posicion = (posicion + 1) % map->capacity;
    current = map->buckets[posicion];
  }
}


Pair * searchMap(HashMap * map,  char * key) {   

  if(map==NULL || key==NULL) return NULL;

  long posicion = hash(key , map->capacity);
  Pair *current = map->buckets[posicion];

  while(current != NULL && current->key != NULL ){
    map->current = posicion;
    if(is_equal(current->key , key)) return map->buckets[posicion];
    posicion = (posicion + 1) % map->capacity;
    current = map->buckets[posicion];
  }


  return NULL;
}

Pair * firstMap(HashMap * map) {

    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
