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

  if(map == NULL) return;
  enlarge_called = 1; //no borrar (testing purposes)

  Pair** old_buckets = map->buckets;
  long old_capacity = map->capacity;
  map->capacity = map->capacity * 2;
  map->buckets = (Pair**)malloc(sizeof(Pair*) * map->capacity);
  map->size = 0;
  
  for(long i = 0 ; i < old_capacity ; i++){
    if(old_buckets[i] != NULL && old_buckets[i]->key != NULL){
      insertMap(map, old_buckets[i]->key, old_buckets[i]->value);
      map->size++;
    }
  }
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
      map->size--;
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

  if(map==NULL) return NULL;

  for(long posicion = 0 ; posicion < map->capacity ; posicion++){
    Pair *current = map->buckets[posicion];
    if (current != NULL && map->buckets[posicion]->key != NULL){
      map->current = posicion;
      return current;
    }
  }
  
  
  return NULL;
}

Pair * nextMap(HashMap * map) {
  if(map==NULL) return NULL;
  
  for(long posicion = map->current + 1 ; posicion < map->capacity ; posicion++){
    Pair *current = map->buckets[posicion];
    if (current != NULL && map->buckets[posicion]->key != NULL){
      map->current = posicion;
      return current;
    }
  }
    
  return NULL;
}
