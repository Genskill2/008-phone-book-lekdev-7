@@ -1,118 +1,119 @@
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const char *DB="directory.db";
struct entry0 
{
  char name[20];
  char phone[20];
  struct entry0 *next;
};
typedef struct entry0 entry;
/* Command handlers */
void add(char *, char *);
int search(FILE *,char *);
void list(FILE *);
int delete(FILE *, char *);
/* Utility functions  */
FILE * open_db_file(); //fulldef /* Opens the database file. Prints error and quits if it's not available */
void print_usage(char *, char *);//fulldef  /* Prints usage */
entry *load_entries(FILE *); //missignfunc /* Load all entries from the database file. Returns pointer to first entry */
entry *create_entry_node(char *, char *); //fulldef /* Create a new entry node. Has to be freed by user. */
void free_entries(entry *); /* TBD Given the first node of a linked list of entries, will free all the nodes */ 
void write_all_entries(entry *); //fulldef /* Given the first node of a linked list of entries, will delete the database file on the disk and save  the given entries into the file */
                                   
                          
int main(int argc, char *argv[])
{
  if (argc == 1) 
  {
    print_usage("Insufficient arguments", argv[0]);
    exit(1);
  } 
  if (strcmp(argv[1], "add") == 0) 
  {   /* Handle add */
    if (argc != 4) 
    {
      print_usage("Improper arguments for add", argv[0]);
      exit(1);
    }
    char *name = argv[2];
    char *phone = argv[3];
    add(name, phone);
    exit(0);
  } 
  else if (strcmp(argv[1], "list") == 0) 
  {  /* Handle list */
    if (argc != 2) 
    {
      print_usage("Improper arguments for list", argv[0]);
      exit(1);
    }
    FILE *fp = open_db_file();
    list(fp);
    fclose(fp);
    exit(0);
  }
  else if (strcmp(argv[1], "search") == 0) 
  {  /* Handle search */
  	if(argc!=3)
  	{
  		print_usage("Improper arguments for search", argv[0]);
      exit(1);
  	}
  	FILE *fp = open_db_file();
  	char *name = argv[2];
  	if (!search(fp, name)) 
    {
      printf("no match\n");
      fclose(fp);
      exit(1);
    }
    fclose(fp);
    exit(0);
  	//printf("NOT IMPLEMENTED!\n"); /* TBD  */
  } 
  else if (strcmp(argv[1], "delete") == 0) 
  {  /* Handle delete */
    if (argc != 3) 
   	{
      print_usage("Improper arguments for delete", argv[0]);
      exit(1);
    }
    FILE *fp = open_db_file();
    char *name = argv[2];
    if (!delete(fp, name)) 
    {
      printf("no match\n");
      fclose(fp);
      exit(1);
    }
    fclose(fp);
    exit(0);
  } 
  else
  {
    print_usage("Invalid command", argv[0]);
    exit(1);
  }
}
FILE *open_db_file() 
{
  FILE *fp=fopen(DB, "r");
  if (fp == NULL) 
  {
    perror("Couldn't open database file");
    exit(1);
  }
  //printf("opened\n");
  return fp;
}

@@ -183,24 +184,47 @@ entry *load_entries(FILE *fp)
    %20[^,\n] will match a string of characters with a maximum length
     of 20 characters that doesn't have a comma(,) or a newline(\n).
  */        
  */   
  int count=-1;   
  while (fscanf(fp, "%20[^,\n],%20[^,\n]\n", name, phone) != EOF) 
  {
    tmp = create_entry_node(name, phone);
    //printf("tmp %s\t%s\t%p\n",tmp->name,tmp->phone,tmp->next);
    count++;
    if (ret == NULL)
      {
      	ret = tmp;
      	current=ret;
      	//ret->next=tmp->next;
      	//current->next=ret->next;
      	current=tmp;
      }
    else
     /* //previously 
     current->next = tmp;
    current = tmp;*/
    	if(count==1)
    	{
    		current->next = tmp;
    		current = tmp;
    		ret->next=tmp;
    		current->next=tmp;
    		current=tmp;	
    	}
    	else
    	{
    		current->next=tmp;
    		current=tmp;
    	}
    //printf("current %s\t%s\t%p\n",current->name,current->phone,current->next);
  }
  //printf("ret %s\t%s\t%p\n",ret->name,ret->phone,ret->next);
  entry *p=ret;
 	for(int i=0;p!=NULL;i++)
  {  	
  	//printf("p %p\t%s\t%s\t%p\n",p,p->name,p->phone,p->next);
  	p=p->next;
  }
  free(p);
  //free(tmp);
  //free(current);
  return ret;
}

@@ -286,25 +310,34 @@ int delete(FILE *db_file, char *name)
    }
  }
  write_all_entries(base);
  //free(prev);
  free(p);
  free_entries(base);
  return deleted;
}

int search( FILE *db_file, char* name)
{
	int found=0;
	//printf("in search 1\n");
	entry *p=load_entries(db_file);
	while(p!=NULL);
	entry *base=p;
	//printf("in search 2\n");
	//printf("p %s\t%s\t%p\n",p->name,p->phone,base->next);
	while(p!=NULL)
	{
		if(p->name==name)
		//printf("in search 3\n");
		if(strcmp(p->name,name)==0)
		{
			printf("%s",p->phone);
			//printf("IN\n");
			printf("%s\n",p->phone);
			found=1;
			break;
		}
		else
			p=p->next;
	}
	free(p);
	free_entries(base);
	return found;
}
