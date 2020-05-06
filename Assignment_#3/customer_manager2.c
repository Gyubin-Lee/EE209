/*Gyubin Lee, 20190439, Assignment #3*/
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

#define HASH_MULTIPLIER 65599
#define BASE_SIZE 1024

struct UserNode{
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
  struct UserNode *next_name;
  struct UserNode *prev_name;
  struct UserNode *next_id;
  struct UserNode *prev_id;
};

struct DB{
  struct UserNode **hashtable_id;
  struct UserNode **hashtable_name;
  int curHashSize;
  int numItems;
};
/*----------------static functions-------------------*/
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

static void iteration(DB_T d){
  struct UserNode *temp;
  int i;

  printf("------Iterating by ID------\n");
  for(i=0;i<d->curHashSize;i++){
    if(d->hashtable_id[i]->next_id){
      temp = d->hashtable_id[i]->next_id;
      while(temp){
        printf("(%s, %s, %d)\n", temp->id, temp->name, temp->purchase);
        temp = temp->next_id;
      }
    }
  }

  printf("-----Iterating by name-----\n");
  for(i=0;i<d->curHashSize;i++){
    if(d->hashtable_name[i]->next_name){
      temp = d->hashtable_name[i]->next_name;
      while(temp){
        printf("(%s, %s, %d)\n", temp->id, temp->name, temp->purchase);
        temp = temp->next_name;
      }
    }
  }
}

/*--------------non-static functions-----------------*/
DB_T CreateCustomerDB(void){
  DB_T d;
  
  d = (DB_T) calloc(1, sizeof(struct DB));
  if (!d) {
    fprintf(stderr, "Can't allocate a memory for DB_T\n");
    return NULL;
  }

  d->curHashSize = BASE_SIZE;

  struct UserNode **table;
  struct UserNode *head;
  
  /*make id hashtable*/
  table = (struct UserNode **)calloc(BASE_SIZE, sizeof(struct UserNode *));
  if(table){
    for(int i=0;i<BASE_SIZE;i++){
      head = (struct UserNode *)calloc(1, sizeof(struct UserNode));
      if(head) table[i] = head;
    }
    d->hashtable_id = table;
  }


  /*make name hashtable*/
  table = (struct UserNode **)calloc(BASE_SIZE, sizeof(struct UserNode *));
  if(table){
    for(int i=0;i<BASE_SIZE;i++){
      head = (struct UserNode *)calloc(1, sizeof(struct UserNode));
      if(head) table[i] = head;
    }
    d->hashtable_name = table;
  }
}

/* destory db and its associated memory */
void DestroyCustomerDB(DB_T d){
  if(!d) return;

  struct UserNode *p, *nextp;
  int b;

  /*free all UserNodes*/
  for(b=0;b<d->curHashSize;b++){
    for(p=d->hashtable_id[b];p!=NULL;p=nextp){
      nextp = p->next_id;
      free(p);
    }
  }
  /*free all hashtables*/
  free(d->hashtable_id);
  free(d->hashtable_name);

  free(d);
}

/* register a customer with (name, id, purchase) */
int  RegisterCustomer(DB_T d, const char *id, const char *name, const int purchase){
  if(!d || !id || !name || purchase<=0){
    return -1;
  }

  unsigned int i;
  int hash_id = hash_function(id, d->curHashSize);
  int hash_name = hash_function(name, d->curHashSize);
  
  struct UserNode *usernode, *next;

  /*check whether the item already exists*/
  if(d->numItems){
    for(usernode = d->hashtable_id[hash_id]->next_id;usernode!=NULL;usernode = next){
      if(!strcmp(usernode->id, id)){
        fprintf(stderr, "ID already exist\n");
        return -1;
      }
      next = usernode->next_id;
    }
  }
  if(d->numItems){
    for(usernode = d->hashtable_name[hash_name]->next_name;usernode!=NULL;usernode = next){
      if(!strcmp(usernode->name, name)){
        fprintf(stderr, "Name already exist\n");
        return -1;
      }
      next = usernode->next_name;
    }
  }

  /*resize*/

  /*register*/
  struct UserNode *newUser = (struct UserNode*)calloc(1, sizeof(struct UserNode));

  newUser->id = strdup(id);
  newUser->name = strdup(name);
  newUser->purchase = purchase;

  struct UserNode *head;
  
  head = d->hashtable_id[hash_id];
  newUser->prev_id = head;
  newUser->next_id = head->next_id;
  if(head->next_id){
    head->next_id->prev_id = newUser;
  }
  head->next_id = newUser;

  head = d->hashtable_name[hash_name];
  newUser->prev_name = head;
  newUser->next_name = head->next_name;
  if(head->next_name){
    head->next_name->prev_name = newUser;
  }
  head->next_name = newUser;

  d->numItems++;

  //iteration(d);

  return 0;
}

/* unregister a customer with 'id' */
int UnregisterCustomerByID(DB_T d, const char *id){

  if(!d || !id){
    return (-1);
  }

  int hash_id = hash_function(id, d->curHashSize);
  struct UserNode *temp, *next;

  for(temp=d->hashtable_id[hash_id]->next_id;temp!=NULL;temp=next){
    if(!strcmp(temp->id, id)){
      if(temp->prev_id){
        temp->prev_id->next_id = temp->next_id;
      }
      if(temp->prev_name){
        temp->prev_name->next_name = temp->next_name;
      }
      if(temp->next_id){
        temp->next_id->prev_id = temp->prev_id;
      }
      if(temp->next_name){
        temp->next_name->prev_name = temp->prev_name;
      }
      free(temp);
      d->numItems--;
      //iteration(d);
      return 0;
    }
    next = temp->next_id;
  }

  fprintf(stderr, "No such name\n");
  //iteration(d);
  return (-1);
}

/* unregister a customer with 'name' */
int UnregisterCustomerByName(DB_T d, const char *name){
  if(!d || !name){
    return (-1);
  }

  int hash_name = hash_function(name, d->curHashSize);
  struct UserNode *temp, *next;

  for(temp=d->hashtable_name[hash_name]->next_name;temp!=NULL;temp=next){
    if(!strcmp(temp->name, name)){
      if(temp->prev_id){
        temp->prev_id->next_id = temp->next_id;
      }
      if(temp->prev_name){
        temp->prev_name->next_name = temp->next_name;
      }
      if(temp->next_id){
        temp->next_id->prev_id = temp->prev_id;
      }
      if(temp->next_name){
        temp->next_name->prev_name = temp->prev_name;
      }
      free(temp);
      d->numItems--;
      //iteration(d);
      return 0;
    }
    next = temp->next_name;
  }

  fprintf(stderr, "No such name\n");
  //iteration(d);
  return (-1);
}

/* get the purchase amount of a user whose ID is 'id' */
int GetPurchaseByID(DB_T d, const char* id){

  if(!d || !id) return (-1);

  int hash_id = hash_function(id, d->curHashSize);
  struct UserNode *temp, *next;

  if(d->numItems){
    for(temp = d->hashtable_id[hash_id]->next_id;temp!=NULL;temp = next){
      if(!strcmp(temp->id, id)){
        return temp->purchase;
      }
      next = temp->next_id;
    }
  }

  fprintf(stderr, "No such id");
  return (-1);
}

/* get the purchase amount of a user whose name is 'name' */
int GetPurchaseByName(DB_T d, const char* name){
  if(!d || !name) return (-1);

  int hash_name = hash_function(name, d->curHashSize);
  struct UserNode *temp, *next;

  if(d->numItems){
    for(temp = d->hashtable_name[hash_name]->next_name;temp!=NULL;temp = next){
      if(!strcmp(temp->name, name)){
        return temp->purchase;
      }
      next = temp->next_name;
    }
  }

  fprintf(stderr, "No such name");
  return (-1);
}

/* iterate all valid user items once, evaluate fp for each valid user
   and return the sum of all fp function calls */
int GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp){
  if(!d || !fp) return (-1);

  int sum = 0;
  unsigned int i;

  struct UserNode *temp;

  for(i=0;i<d->curHashSize;i++){
    temp = d->hashtable_id[i]->next_id;
    while(temp){
      sum += fp(temp->id, temp->name, temp->purchase);
      temp = temp->next_id;
    }
  }

  return sum;
}