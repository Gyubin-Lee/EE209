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
  d->pArray = (struct UserInfo *)calloc(d->curArrSize,
               sizeof(struct UserInfo));
  if (d->pArray == NULL) {
    fprintf(stderr, "Can't allocate a memory for array of size %d\n",
	    d->curArrSize);   
    free(d);
    return NULL;
  }
  return d;

  return NULL;
}
/*--------------------------------------------------------------------*/
void
DestroyCustomerDB(DB_T d)
{
  /* fill out this function */
  if(!d){
    return;
  }

  if(d->pArray){
    free(d->pArray);
  }

  free(d);

  assert(0);
  return NULL;
}
/*--------------------------------------------------------------------*/
int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase)
{
  /* fill out this function */

  if(!d || !id || !name || purchase<=0){
    return -1;
  }

  int i;

  if(d->numItems != 0){
    for(i=0;i<d->curArrSize;i++){
      if(!strcmp(d->pArray[i]->id, id) || !strcmp(d->pArray[i]->name, name)){
        fprintf(stderr, "Item already exists");
        return -1;
      }
    }
  }

  if(d->curArrSize == d->numItems){
    struct UserInfo *old = d->pArray;
    d->curArrSize*=2;
    d->pArray = (struct UserInfo*)realloc(old, d->curArrSize*sizeof(struct UserInfo));
    if(!d->pArray){
      fprintf(stderr, "Fail to realloc pArray");
      return -1;
    }
  }

  char *id_temp, *name_temp;
  struct UserInfo *user;

  id_temp = strdup(id);
  name_temp = strdup(name);

  user = calloc(1, sizeof(struct UserInfo));
  if(!user){
    fprintf(stderr,"Fail to calloc user");
    return -1;
  }

  user->id = id_temp;
  user->name = name_temp;
  user->purchase = purchase;

  for(i=0;i<d->curArrSize;i++){
    if(!d->pArray[i]){
      d->pArray[i] = user;
      d->numItems++;
      return 0;
    }
  }

  assert(0);
  return (-1);
}
/*--------------------------------------------------------------------*/
int
UnregisterCustomerByID(DB_T d, const char *id)
{
  /* fill out this function */
  assert(0);
  return (-1);
}

/*--------------------------------------------------------------------*/
int
UnregisterCustomerByName(DB_T d, const char *name)
{
  /* fill out this function */
  assert(0);
  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByID(DB_T d, const char* id)
{
  /* fill out this function */
  assert(0);
  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByName(DB_T d, const char* name)
{
  /* fill out this function */
  assert(0);
  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
  /* fill out this function */
  assert(0);
  return (-1);
}
