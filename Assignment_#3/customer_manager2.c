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
  struct UserNode *next_id;
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

/*resize the size of the hash-tables*/
static void resize(DB_T d){
  struct UserNode **newIDTable, **newNameTable;
  struct UserNode *head, *temp, *next, *user;
  int i, hash_id, hash_name;

  int newSize = 2*d->curHashSize;

  /*make new ID hashtable*/
  newIDTable = 
  (struct UserNode **)calloc(newSize, sizeof(struct UserNode *));
  if(newIDTable){
    for(i=0;i<newSize;i++){
      head = (struct UserNode *)calloc(1, sizeof(struct UserNode));
      if(!head){
        fprintf(stderr, "fail to calloc usernode\n");
        assert(0);
      }
      newIDTable[i] = head;
    }
  } else{
    fprintf(stderr, "fail to calloc newIDTable\n");
    assert(0);
  }
  /*make new Name hashtable*/
  newNameTable = 
  (struct UserNode **)calloc(newSize, sizeof(struct UserNode *));
  if(newNameTable){
    for(i=0;i<newSize;i++){
      head = (struct UserNode *)calloc(1, sizeof(struct UserNode));
      if(!head){
        fprintf(stderr, "fail to calloc usernode\n");
        assert(0);
      }
      newNameTable[i] = head;
    }
  } else{
    fprintf(stderr, "fail to calloc newNameTable\n");
    assert(0);
  }

  /*copy user information*/
  for(i=0;i<d->curHashSize;i++){
    temp = d->hashtable_id[i]->next_id;
    while(temp){
      user = (struct UserNode*)calloc(1, sizeof(struct UserNode));
      if(!user){
        fprintf(stderr, "Fail to calloc user\n");
        assert(0);
      }
      user->id = (char *)malloc(100);
      if(!user->id) assert(0);
      memcpy(user->id, temp->id, strlen(temp->id)+1);
      user->name = (char *)malloc(100);
      if(!user->name) assert(0);
      memcpy(user->name, temp->name, strlen(temp->name)+1);
      user->purchase = temp->purchase;
      temp = temp->next_id;
      
      hash_id = hash_function(user->id, newSize);
      hash_name = hash_function(user->name, newSize);

      head = newIDTable[hash_id];
      user->next_id = head->next_id;
      head->next_id = user;

      head = newNameTable[hash_name];
      user->next_name = head->next_name;
      head->next_name = user;
    }
  }

  /*destroy previous tables*/
  for(int b=0;b<d->curHashSize;b++){
    for(temp=d->hashtable_id[b];temp!=NULL;temp=next){
      next = temp->next_id;
      free(temp->id);
      free(temp->name);
      free(temp);
    }
  }
  free(d->hashtable_id);
  free(d->hashtable_name);

  /*uptade DB_T b*/
  d->hashtable_id = newIDTable;
  d->hashtable_name = newNameTable;
  d->curHashSize = newSize;
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
  table = 
  (struct UserNode **)calloc(BASE_SIZE, sizeof(struct UserNode *));
  if(table){
    for(int i=0;i<BASE_SIZE;i++){
      head = (struct UserNode *)calloc(1, sizeof(struct UserNode));
      if(!head){
        fprintf(stderr, "fail to calloc head\n");
        assert(0);
      }
      table[i] = head;
    }
    d->hashtable_id = table;
  } else{
    fprintf(stderr, "fail to calloc table\n");
    assert(0);
  } 
  /*make name hashtable*/
  table = 
  (struct UserNode **)calloc(BASE_SIZE, sizeof(struct UserNode *));
  if(table){
    for(int i=0;i<BASE_SIZE;i++){
      head = (struct UserNode *)calloc(1, sizeof(struct UserNode));
      if(!head){
        fprintf(stderr, "fail to calloc head\n");
        assert(0);
      }
      table[i] = head;
    }
    d->hashtable_name = table;
  } else{
    fprintf(stderr, "fail to calloc table\n");
    assert(0);
  }

  return d;
}

/*destory db and its associated memory */
void DestroyCustomerDB(DB_T d){
  /*If d is empty, return and end the function*/
  if(!d) return;

  struct UserNode *temp, *next;
  int b;

  /*free all UserNodes*/
  for(b=0;b<d->curHashSize;b++){
    for(temp=d->hashtable_id[b];temp!=NULL;temp=next){
      next = temp->next_id;
      free(temp->id);
      free(temp->name);
      free(temp);
    }
  }
  /*free all hashtables*/
  free(d->hashtable_id);
  free(d->hashtable_name);

  /*free the DB*/
  free(d);
}

/* register a customer with (name, id, purchase) */
int  RegisterCustomer
(DB_T d, const char *id, const char *name, const int purchase){
  /*input validation*/
  if(!d || !id || !name || purchase<=0){
    return -1;
  }

  /*resize the hashtables*/
  if(d->numItems > 0.75*d->curHashSize && d->curHashSize<=524288){
    resize(d);
  }

  int hash_id = hash_function(id, d->curHashSize);
  int hash_name = hash_function(name, d->curHashSize);
  
  struct UserNode *temp;

  /*check whether the item already exists*/
  if(d->numItems){
    temp = d->hashtable_id[hash_id]->next_id;
    while(temp){
      if(!strcmp(temp->id, id)){
        fprintf(stderr, "ID already exists\n");
        return -1;
      }
      temp = temp->next_id;
    }
    temp = d->hashtable_name[hash_name]->next_name;
    while(temp){
      if(!strcmp(temp->name, name)){
        fprintf(stderr, "Name already exists\n");
        return -1;
      }
      temp = temp->next_name;
    }
  }

  /*register*/
  struct UserNode 
  *newUser = (struct UserNode*)calloc(1, sizeof(struct UserNode));
  if(!newUser){
    fprintf(stderr, "Fail to calloc newUser\n");
    return -1;
  }

  newUser->id = (char *)malloc(100);
  if(!newUser->id) assert(0);
  memcpy(newUser->id, id, strlen(id)+1);
  newUser->name = (char *)malloc(100);
  if(!newUser->name) assert(0);
  memcpy(newUser->name, name, strlen(name)+1);
  newUser->purchase = purchase;

  struct UserNode *head;
  
  head = d->hashtable_id[hash_id];
  newUser->next_id = head->next_id;
  head->next_id = newUser;

  head = d->hashtable_name[hash_name];
  newUser->next_name = head->next_name;
  head->next_name = newUser;

  d->numItems++;

  return 0;
}

/* unregister a customer with 'id' */
int UnregisterCustomerByID(DB_T d, const char *id){
  /*input validation*/
  if(!d || !id){
    return (-1);
  }

  int hash_id = hash_function(id, d->curHashSize);
  struct UserNode *temp_id, *prev_id, *temp_name, *prev_name;

  /*find id and unregister*/
  if(d->numItems){
    temp_id = d->hashtable_id[hash_id]->next_id;
    prev_id = d->hashtable_id[hash_id];
    while(temp_id){
      if(!strcmp(temp_id->id, id)){
        /*remove from ID hash table*/
        prev_id->next_id = temp_id->next_id;
        /*remove from name hash table*/
        int hash_name = hash_function(temp_id->name, d->curHashSize);
        temp_name = d->hashtable_name[hash_name]->next_name;
        prev_name = d->hashtable_name[hash_name];
        while(temp_name){
          if(temp_id == temp_name){
            prev_name->next_name = temp_name->next_name;
            break;
          }
          prev_name = temp_name;
          temp_name = temp_name->next_name;
        }
        free(temp_id->id);
        free(temp_id->name);
        free(temp_id);
        
        d->numItems--;
        return 0;
      }
      prev_id = temp_id;
      temp_id = temp_id->next_id;
    }
  }

  fprintf(stderr, "No such ID\n");
  return (-1);  
}

/* unregister a customer with 'name' */
int UnregisterCustomerByName(DB_T d, const char *name){
  /*input validation*/
  if(!d || !name){
    return (-1);
  }

  /*find name and unregister*/
  int hash_name = hash_function(name, d->curHashSize);
  struct UserNode *temp_id, *prev_id, *temp_name, *prev_name;

  if(d->numItems){
    temp_name = d->hashtable_name[hash_name]->next_name;
    prev_name = d->hashtable_name[hash_name];
    while(temp_name){
      if(!strcmp(temp_name->name, name)){
        /*remove from name hash table*/
        prev_name->next_name = temp_name->next_name;

        /*remove from ID hash table*/
        int hash_id = 
        hash_function(temp_name->id, d->curHashSize);
        temp_id = d->hashtable_id[hash_id]->next_id;
        prev_id = d->hashtable_id[hash_id];
        while(temp_id){
          if(temp_id == temp_name){
            prev_id->next_id = temp_id->next_id;
            break;
          }
          prev_id = temp_id;
          temp_id = temp_id->next_id;
        }
        free(temp_name->id);
        free(temp_name->name);
        free(temp_name);
        
        d->numItems--;
        //iteration(d);
        return 0;
      }
      prev_name = temp_name;
      temp_name = temp_name->next_name;
    }
  }

  fprintf(stderr, "No such name\n");
  return (-1);
}

/* get the purchase amount of a user whose ID is 'id' */
int GetPurchaseByID(DB_T d, const char* id){
  /*input validation*/
  if(!d || !id) return (-1);

  /*find id and return purchase*/
  int hash_id = hash_function(id, d->curHashSize);
  struct UserNode *temp;

  if(d->numItems){
    temp = d->hashtable_id[hash_id]->next_id;
    while(temp){
      if(!strcmp(temp->id, id)){
        return temp->purchase;
      }
      temp = temp->next_id;
    }
  }

  fprintf(stderr, "No such id\n");
  return (-1);
}

/* get the purchase amount of a user whose name is 'name' */
int GetPurchaseByName(DB_T d, const char* name){
  /*input validation*/
  if(!d || !name) return (-1);

  /*find name and return purchase*/
  int hash_name = hash_function(name, d->curHashSize);
  struct UserNode *temp;

  if(d->numItems){
    temp = d->hashtable_name[hash_name]->next_name;
    while(temp){
      if(!strcmp(temp->name, name)){
        return temp->purchase;
      }
      temp = temp->next_name;
    }
  }

  fprintf(stderr, "No such name\n");
  return (-1);
}

/* iterate all valid user items once, evaluate fp for each valid user
   and return the sum of all fp function calls */
int GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp){
  /*input validation*/
  if(!d || !fp) return (-1);

  int sum = 0;
  unsigned int i;

  struct UserNode *temp;

  /*iterating the hashtable*/
  for(i=0;i<d->curHashSize;i++){
    temp = d->hashtable_id[i]->next_id;
    while(temp){
      sum += fp(temp->id, temp->name, temp->purchase);
      temp = temp->next_id;
    }
  }

  return sum;
}