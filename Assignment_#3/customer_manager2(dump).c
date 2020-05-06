/*Gyubin Lee, 20190439, Assignment #3*/
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

#define HASH_MULTIPLIER 65599
#define BASE_SIZE 1024

struct UserInfo {
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
  struct UserInfo *next_name;
  struct UserInfo *prev_name;
  struct UserInfo *next_id;
  struct UserInfo *prev_id;
};

struct DB {
  struct UserInfo **hashtable_id;   // pointer to the array
  struct UserInfo **hashtable_name;
  int curHashSize_id;
  int curHashSize_name;         // current array size (max # of elements)
  int numItems;             // # of stored items, needed to determine
};

/*------------static functions--------------*/
static int hash_function(const char *pcKey, int iBucketCount)
/* Return a hash code for pcKey that is between 0 and iBucketCount-1,
   inclusive. Adapted from the EE209 lecture notes. */
{
   int i;
   unsigned int uiHash = 0U;
   for (i = 0; pcKey[i] != '\0'; i++)
      uiHash = uiHash * (unsigned int)HASH_MULTIPLIER
               + (unsigned int)pcKey[i];
   return (int)(uiHash % (unsigned int)iBucketCount);
}

static struct UserInfo **resize_id(struct UserInfo **origin, int origin_size){
  int new_size = 2*origin_size;
  struct UserInfo **new = calloc(new_size, sizeof(struct UserInfo*));

  if(!new){
    fprintf(stderr, "Fail to realloc id hashtable\n");
  }

  /*re-hashing*/
  for(int i=0;i<origin_size;i++){
    if(!origin[i]){
      int hash_id = hash_function(origin[i]->id, new_size);
      new[hash_id] = origin[i];
    }
  }
  
  /*free original hashtable*/
  struct UserInfo *p;
  struct UserInfo *nextp;
  for(int b=0;b<origin_size;b++){
    for(p=origin[b];p!=NULL;p = nextp){
      nextp = p->next_id;
      free(p);
    }
  }
  free(origin);

  return new;
}

static struct UserInfo **resize_name(struct UserInfo **origin, int origin_size){
  int new_size = 2*origin_size;
  struct UserInfo **new = calloc(new_size, sizeof(struct UserInfo*));

  if(!new){
    fprintf(stderr, "Fail to realloc name hashtable\n");
  }

  /*re-hashing*/
  for(int i=0;i<origin_size;i++){
    if(!origin[i]){
      int hash_name = hash_function(origin[i]->name, new_size);
      new[hash_name] = origin[i];
    }
  }
  
  /*free original hashtable*/
  struct UserInfo *p;
  struct UserInfo *nextp;
  for(int b=0;b<origin_size;b++){
    for(p=origin[b];p!=NULL;p = nextp){
      nextp = p->next_name;
      free(p);
    }
  }
  free(origin);

  return new;
}
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

  d->curHashSize_id = BASE_SIZE; // start with 1024 elements
  d->curHashSize_name = BASE_SIZE;

  d->hashtable_id = (struct UserInfo **)calloc(d->curHashSize_id,
               sizeof(struct UserInfo*));
  if (d->hashtable_id == NULL) {
    fprintf(stderr, "Can't allocate a memory for array(hashtable_id) of size %d\n",
	    d->curHashSize_id);   
    free(d);
    return NULL;
  }

  d->hashtable_name = (struct UserInfo **)calloc(d->curHashSize_name,
               sizeof(struct UserInfo*));
  if (d->hashtable_name == NULL) {
    fprintf(stderr, "Can't allocate a memory for array(hashtable_name) of size %d\n",
	    d->curHashSize_name);   
    free(d);
    return NULL;
  }

  return d;
}
/*--------------------------------------------------------------------*/
void
DestroyCustomerDB(DB_T d)
{
  /* fill out this function */
  if(!d){
    return;
  }

  struct UserInfo *p;
  struct UserInfo *nextp;

  for(int b=0;b<d->curHashSize_id;b++){
    for(p=d->hashtable_id[b];p!=NULL;p = nextp){
      nextp = p->next_id;
      free(p);
    }
  }
  free(d->hashtable_id);

  for(int b=0;b<d->curHashSize_name;b++){
    for(p=d->hashtable_name[b];p!=NULL;p = nextp){
      nextp = p->next_name;
      free(p);
    }
  }
  free(d->hashtable_name);

  free(d);
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

  unsigned int i;
  int hash_id = hash_function(id, d->curHashSize_id);
  int hash_name = hash_function(name, d->curHashSize_name);
  
  struct UserInfo *usernode, *next;

  /*check whether the item already exists*/
  if(d->numItems){
    for(usernode = d->hashtable_id[hash_id];usernode!=NULL;usernode = next){
      if(!strcmp(usernode->id, id)){
        fprintf(stderr, "ID already exist\n");
        return -1;
      }
      next = usernode->next_id;
    }
  }

  if(d->numItems){
    for(usernode = d->hashtable_name[hash_name];usernode!=NULL;usernode = next){
      if(!strcmp(usernode->name, name)){
        fprintf(stderr, "name already exist\n");
        return -1;
      }
      next = usernode->next_name;
    }
  }

  /*resize*/
  if(d->numItems > 0.75*d->curHashSize_id && d->curHashSize_id<500000){
    d->hashtable_id = resize_id(d->hashtable_id, d->curHashSize_id);
  }
  if(d->numItems > 0.75*d->curHashSize_name && d->curHashSize_name<500000){
    d->hashtable_name = resize_name(d->hashtable_name, d->curHashSize_name);
  }
  
  /*register customer*/
  struct UserInfo *user, *head;

  head = d->hashtable_id[hash_id];
  user = calloc(1, sizeof(struct UserInfo));
  user->id = strdup(id);
  user->name = strdup(name);
  user->purchase = purchase;
  
  if(!head){
    d->hashtable_id[hash_id] = user;
  } else{
    d->hashtable_id[hash_id] = user;
    user->next_id = head;
    head->prev_id = user;
  }
  
  head = d->hashtable_name[hash_name];
  
  if(!head){
    d->hashtable_name[hash_name] = user;
  } else{
    d->hashtable_name[hash_name] = user;
    user->next_name = head;
    head->prev_name = user;
  }

  d->numItems++;

  return 0;
}
/*--------------------------------------------------------------------*/


int
UnregisterCustomerByID(DB_T d, const char *id)
{
  /* fill out this function */

  if(!d || !id){
    return (-1);
  }

  int hash_id = hash_function(id, d->curHashSize_id);
  struct UserInfo *usernode, *next;

  if(d->numItems){
    for(usernode = d->hashtable_id[hash_id];usernode!=NULL;usernode = next){
      if(!strcmp(usernode->id, id)){
        if(!usernode->prev_id){
          printf("debugging\n");
          d->hashtable_id[hash_id] = usernode->next_id;
          printf("debugging\n");
          if(!usernode->next_id){
            printf("debugging\n");
            usernode->next_id->prev_id = NULL;
            printf("debugging\n");
          }
        } else{
          usernode->prev_id->next_id = usernode->next_id;
          if(!usernode->next_id){
            usernode->next_id->prev_id = usernode->prev_id;
          }         
        }
        free(usernode);
        printf("remove success\n");
        return 0;
      }
    }
  }

  fprintf(stderr, "No such id\n");
  //assert(0);
  return (-1);
}

/*--------------------------------------------------------------------*/
int
UnregisterCustomerByName(DB_T d, const char *name)
{
  /* fill out this function */

  if(!d || !name){
    return (-1);
  }

  int hash_name = hash_function(name, d->curHashSize_name);
  struct UserInfo *usernode, *next;

  if(d->numItems){
    for(usernode = d->hashtable_name[hash_name];usernode!=NULL;usernode = next){
      if(!strcmp(usernode->name, name)){
        if(!usernode->prev_name){
          d->hashtable_name[hash_name] = usernode->next_name;
          usernode->next_name->prev_name = NULL;
        } else{
          usernode->prev_name->next_name = usernode->next_name;
          usernode->next_name->prev_name = usernode->prev_name;
        }
        free(usernode);
        printf("remove success\n");
        return 0;
      }
    }
  }

  fprintf(stderr, "No such name\n");
  //assert(0);
  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByID(DB_T d, const char* id)
{
  /* fill out this function */

  if(!d || !id) return (-1);

  int hash_id = hash_function(id, d->curHashSize_id);
  struct UserInfo *usernode, *next;

  if(d->numItems){
    for(usernode = d->hashtable_id[hash_id];usernode!=NULL;usernode = next){
      if(!strcmp(usernode->id, id)){
        return usernode->purchase;
      }
      next = usernode->next_id;
    }
  }

  fprintf(stderr, "No such id");
  //assert(0);
  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByName(DB_T d, const char* name)
{
  /* fill out this function */
  if(!d || !name) return (-1);

  int hash_name = hash_function(name, d->curHashSize_name);
  struct UserInfo *usernode, *next;

  if(d->numItems){
    for(usernode = d->hashtable_name[hash_name];usernode!=NULL;usernode = next){
      if(!strcmp(usernode->name, name)){
        return usernode->purchase;
      }
      next = usernode->next_name;
    }
  }

  fprintf(stderr, "No such name");
  //assert(0);
  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
  /* fill out this function */
  if(!d || !fp) return (-1);

  int sum = 0;
  unsigned int i;

  struct UserInfo *usernode;

  for(i=0;i<d->curHashSize_id;i++){
    usernode = d->hashtable_id[i];
    while(usernode){
      sum += fp(usernode->id, usernode->name, usernode->purchase);
      usernode = usernode->next_id;
    }
  }

  return sum;
}
