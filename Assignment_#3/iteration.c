/*Just for debugging*/
static void iteration(DB_T d){
  struct UserNode *temp;
  int i, count;

  printf("------Iterating by ID------\n");
  count = 0;
  for(i=0;i<d->curHashSize;i++){
    if(d->hashtable_id[i]->next_id){
      temp = d->hashtable_id[i]->next_id;
      while(temp){
        printf("(%s, %s, %d)\n", temp->id, temp->name, temp->purchase);
        temp = temp->next_id;
        count++;
      }
    }
  }
  printf("The number of Items is %d\n", count);
  count=0;
  printf("-----Iterating by name-----\n");
  for(i=0;i<d->curHashSize;i++){
    if(d->hashtable_name[i]->next_name){
      temp = d->hashtable_name[i]->next_name;
      while(temp){
        printf("(%s, %s, %d)\n", temp->id, temp->name, temp->purchase);
        temp = temp->next_name;
        count++;
      }
    }
  }
  printf("The number of Items is %d\n", count);
}