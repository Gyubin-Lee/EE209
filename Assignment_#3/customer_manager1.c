/*Gyubin Lee, 20190439, Assignment #3*/
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

#define UNIT_ARRAY_SIZE 1024

struct UserInfo {
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
};

struct DB {
  struct UserInfo **pArray;   // pointer to the array
  int curArrSize;            // current array size (max # of elements)
  int numItems;              // # of stored items, needed to determine
			     // # whether the array should be expanded
			     // # or not
};

/*--------------------------------------------------------------------*/
DB_T
CreateCustomerDB(void)
{
  DB_T d;
  
  d = (DB_T) calloc(1, sizeof(struct DB));
  if (d == NULL) {
    fprintf(stderr, "Can't allocate a memory for DB_T\n");
    return NULL;
  }
  d->curArrSize = UNIT_ARRAY_SIZE; // start with 1024 elements
  d->pArray = (struct UserInfo **)calloc(d->curArrSize,
               sizeof(struct UserInfo*));
  if (d->pArray == NULL) {
    fprintf(stderr, "Can't allocate a memory for array of size %d\n",
	    d->curArrSize);   
    free(d);
    return NULL;
  }
  return d;
}
/*--------------------------------------------------------------------*/
void
DestroyCustomerDB(DB_T d)
{
  /*If d is empty, return and end the function*/
  if(!d){
    return;
  }

  /*free all users*/
  for(unsigned int i=0;i<d->curArrSize; i++){
    if(d->pArray[i]){
      free(d->pArray[i]);
    }
  }
  /*free the user array*/
  if(d->pArray){
    free(d->pArray);
  }

  /*free the DB*/
  free(d);
}
/*--------------------------------------------------------------------*/
int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase)
{
  /*input validation*/
  if(!d || !id || !name || purchase<=0){
    return -1;
  }

  unsigned int i;

  /*check whether the item already exists*/
  if(d->numItems){
    for(i=0;i<d->curArrSize;i++){
      if(!(d->pArray[i])){
        continue;
      }

      if(!strcmp(d->pArray[i]->id, id) || 
      !strcmp(d->pArray[i]->name, name)){
        fprintf(stderr, "Item already exist\n");
        return -1;
      }
    }
  }

  /*resize the array*/
  if(d->curArrSize == d->numItems){
    struct UserInfo **past = d->pArray;
    unsigned int past_s = d->curArrSize;

    d->curArrSize += UNIT_ARRAY_SIZE;
    d->pArray = calloc(d->curArrSize, sizeof(struct UserInfo*));
    
    if(!d->pArray){
      fprintf(stderr, "Fail to realloc pArray\n");
      return -1;
    }

    memcpy(d->pArray, past, past_s*sizeof(struct UserInfo*));
  }
  
  /*register*/
  struct UserInfo* user = calloc(1, sizeof(struct UserInfo));
  if(!user){
    fprintf(stderr, "Fail to calloc userinfo\n");
    return -1;
  }

  user->id = (char *)malloc(100);
  if(!user->id) assert(0);
  memcpy(user->id, id, strlen(id)+1);
  user->name = (char *)malloc(100);
  if(!user->name) assert(0);
  memcpy(user->name, name, strlen(name)+1);
  user->purchase = purchase;

  for(i=0;i<d->curArrSize;i++){
    if(!(d->pArray[i])){
      d->pArray[i] = user;
      d->numItems++;
      return 0;
    }
  }

  return (-1);
}
/*--------------------------------------------------------------------*/
int
UnregisterCustomerByID(DB_T d, const char *id)
{
  /*input validation*/
  if(!d || !id){
    return (-1);
  }

  /*find id and unregister*/
  if(d->numItems){
    for(unsigned int i=0;i<d->curArrSize;i++){  
      if(!d->pArray[i]){
        continue;
      }
      
      if(!(strcmp(d->pArray[i]->id, id))){
        free(d->pArray[i]->id);
        free(d->pArray[i]->name);
        free(d->pArray[i]);

        d->pArray[i] = NULL;
        d->numItems--;

        printf("remove success\n");
        return 0;
      }
    }
  }

  fprintf(stderr, "No such id\n");
  return (-1);
}

/*--------------------------------------------------------------------*/
int
UnregisterCustomerByName(DB_T d, const char *name)
{
  /*input validation*/
  if(!d || !name){
    return (-1);
  }

  /*find name and unregister*/
  if(d->numItems){
    for(unsigned int i=0;i<d->curArrSize;i++){
      if(!d->pArray[i]){
        continue;
      }
      
      if(!(strcmp(d->pArray[i]->name, name))){
        free(d->pArray[i]->id);
        free(d->pArray[i]->name);
        free(d->pArray[i]);

        d->pArray[i] = NULL;
        d->numItems--;

        return 0;
      }
    }
  }

  fprintf(stderr, "No such name\n");
  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByID(DB_T d, const char* id)
{
  /*input validation*/
  if(!d || !id) return (-1);

  /*find id and return purchase*/
  if(d->numItems){
    for(int i=0;i<d->curArrSize;i++){
      struct UserInfo *temp = d->pArray[i];

      if(!temp){
        continue;
      }

      if(!(strcmp(temp->id, id))){
        return temp->purchase;
      }
    }
  }

  fprintf(stderr, "No such id");
  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByName(DB_T d, const char* name)
{
  /*input validation*/
  if(!d || !name) return (-1);

  /*find name and return purchase*/
  if(d->numItems){
    for(int i=0;i<d->curArrSize;i++){
      struct UserInfo *temp = d->pArray[i];
      
      if(!temp){
        continue;
      }

      if(!(strcmp(temp->name, name))){
        return temp->purchase;
      }
    }
  }

  fprintf(stderr, "No such name");
  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
  /*input validation*/
  if(!d || !fp) return (-1);

  int sum = 0;

  /*iterating the array*/
  for(unsigned int i=0;i<d->curArrSize;i++){ 
    struct UserInfo *temp = d->pArray[i];
    
    if(!temp){
      continue;
    }

    sum += fp(temp->id, temp->name, temp->purchase);
  }

  return sum;
}