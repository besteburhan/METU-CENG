#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#define LIMIT 1000
#define K_SIZE 3
#define V_SIZE 5
typedef struct node{
int key;
char value[V_SIZE];
struct node *next;
struct node *down;
} node;

int is_empty(node *list);int num_levels(node *list);int num_nodes(node *list);int delete(node *list, int key);
void clear(node *list);void print(node *list);void print_level(node *list, int level);void path(node *list, int key);
node *init(int branch);node *insert(node *list, int key, char *value);node *find(node *list, int key);
node *mknode(int key,char * value);
int isitin(node *list, int key);
int spenode(node *list,int key,int L);

int is_empty(node *list){
    if(list->down->key==0 && list->down->next==NULL) return 1;
    return 0;
}
int num_levels(node *list){
    int t=(-(list->down->key))+1;
    return t;
}
int num_nodes(node *list){
    node *temp;
    int n=0;
    temp=list;
    while(temp->down!=NULL) temp=temp->down;
    while(temp->next!=NULL){ 
        n++;
        temp=temp->next;
    }
    return n;
}
void clear(node *list){
    node *curr;
    node *t,*t_c;
    curr=list;
    while(curr->down != NULL){
        curr=curr->down;
        t=curr->next;
        curr->next=NULL;
        while(t != NULL){
            t_c=t;
            t=t->next;
            free(t_c);
        }
    }
    curr=list;
    while(curr->down!=NULL){
        list=list->down;
        free(curr);
        curr=list;
    }
    free(list);
}
node *init(int branch){
    node *list;
    node *blue;
    list = (node*) malloc(sizeof(node)); 
    blue = (node*) malloc(sizeof(node)); 
    if(blue){
        blue->key=0;
        *(blue->value)='\0';
        blue->down=NULL;
        blue->next=NULL;
    }
    if(list){
        list->key=branch*1000;
        *(list->value)='\0';
        list->next=NULL;
        list->down=blue;
    }
    return list;
}
node *mknode(int key,char * value){
    node *np;
    np=(node*)malloc(sizeof(node));
    if(np){
        np->key=key;
        if(value=='\0') *(np->value)='\0';
        else strncpy(np->value,value,5);
        np->down=NULL;
        np->next=NULL;
    }
    return np;
}
int isitin(node *list, int key){
    node *curr;
    node *blue;
    int L=-(num_levels(list)) + 1; 
    blue=list;
    while(L!=1){
        
        curr=blue->down;
        blue=blue->down;
        curr=curr->next;
        while(curr!=NULL){
            
            if(key==curr->key) return 1;
            else {
                curr=curr->next;
            }   
        }
        L++;
    }
    return 0;
}
node *insert(node *list, int key, char *value){
    node *np;
    node *d;
    node *curr;
    node *prev;
    node *blue;
    int branch,N;
    int L_perm,L;
    if(isitin(list,key)==1) {
        L=-(num_levels(list)) + 1; 
        blue=list;
        while(L!=1){
            prev=blue->down;
            curr=blue->down;
            blue=blue->down;
            curr=curr->next;
            while(curr!=NULL){
                if(key==curr->key) {
                    strncpy(curr->value,value,5);
                    break;
                }
                else {
                    prev=curr;
                    curr=curr->next;
                }
            }
            if(L==0 && key==curr->key) {
                list->key=(list->key/1000)*1000+num_nodes(list);
                return curr;
                }
            L++;
        }
    }
    else{
        L_perm = 0;
        branch= (list->key)/1000;
        N = num_nodes(list)+1;
        
        if(N%branch != 0 ) L_perm=0;
        else{
            while(N%branch==0 && N!=0){
                N=N/branch;
                L_perm++;
            }
        }
        L_perm=-L_perm;
        L=L_perm;
        if(L<list->down->key){
            prev=list;
            for( curr=list->down ; L<curr->key ;  L++){
                np=mknode(L,'\0');
                np->down=curr;
                prev->down=np;
                prev=prev->down;
            }
        }
        L=L_perm;
        while(L!=1){
            curr=list->down;
            while(curr->key!=L) curr=curr->down;
            prev=curr;
            curr=curr->next;
            while(curr!=NULL && curr->key<key  ) {prev=curr;curr=curr->next;}
            if(L==L_perm){
                np=mknode(key,value);
                prev->next=np;
                np->next=curr;
                if(L!=0){
                    d=mknode(key,value);
                    np->down=d;
                }
                L++;
            }
            else{
                prev->next=d;
                d->next=curr;
                if(L!=0){
                    d=mknode(key,value);
                    prev->next->down=d;
                }
                L++;
            }
        }
        np=find(list,key);
        while(np->down!=NULL) np=np->down;
        list->key=(list->key/1000)*1000+num_nodes(list);
        return np;
    }
    return NULL;
    }
int delete(node *list,int key){
    node *curr;
    node *prev;
    node *blue;
    int L=-(num_levels(list))+1;
    if(isitin(list,key)){
        blue=list;
        while(L!=1){
            prev=blue->down;
            curr=blue->down;
            blue=blue->down;
            curr=curr->next;
            while(curr!=NULL){
                if(key==curr->key) {
                    prev->next=curr->next;
                    if(blue->next==NULL && blue->key==0){L=0;break;}
                    else if(blue->next==NULL) {
                        list->down=blue->down;
                        blue=list;
                    }
                    break;
                }
                else {
                    prev=curr;
                    curr=curr->next;
                }   
            }
            L++;
        }
        list->key=(list->key/1000)*1000+num_nodes(list);
        return 1;
    }
    else return 0;
}
void print(node *list){
    node *curr;
    node *blue;
    node *temp;
    int n;
    int key;
    int numnode=num_nodes(list);
    int numlevel=num_levels(list);
    int L=0;
    printf("%3d%7s",numnode," ");
    while(L<numlevel-1) {
        printf("%d ",L);
        L++;
    }
    printf("%d\n",L);
    printf("%10s"," ");
    L=0;
    while(L<numlevel-1) {
        printf("+ ");
        L++;
    }
    printf("+ -\n");

    temp=list;
    while(temp->down!=NULL) temp=temp->down;
    temp=temp->next;
    while(temp!=NULL){ 
        key=temp->key;
        printf("%3d:%5s ",key,temp->value);
        L=-numlevel+1;
        blue=list;
        while(L!=1){
            curr=blue->down;
            blue=blue->down;
            curr=curr->next;
            while(curr!=NULL){
                if(key==curr->key) {
                    n=0;
                    while(n<-L) {printf("+ ");n++;}
                    printf("+\n");
                    temp=temp->next;
                    L=0;
                    break;
                    }
                else {
                    curr=curr->next;
                }   
            }
            L++;
        }
    }
   }
void print_level(node *list, int level){
    int L=-level;
    int n;
    node *curr;
    node *prev=NULL;
    if(list->down->key==L){
        printf("%3d:%5s ",L," ");
        curr=list->down->next;
        while(curr){
            printf("-> %3d:%5s ",curr->key,curr->value);
            curr=curr->next;
        }
        printf("-> \n");
    }
    else{
        L=-level-1;
        curr=list->down;
        
        while(L!=curr->key) curr=curr->down;
        
        curr=curr->next;
        n=spenode(list,curr->key,L+1);
        printf("%13s"," ");
        if(n==0) printf("%3d:%5s ->",curr->key,curr->value);
        else printf("%*s%3d:%5s ->",13*n," ",curr->key,curr->value);
        prev=curr;
        curr=curr->next;
        while(curr){
            n=spenode(list,prev->key,L+1);
            n=spenode(list,curr->key,L+1)-n-1;
            if(n==0) printf(" %3d:%5s ->",curr->key,curr->value);
            else printf("%*s %3d:%5s ->",13*n," ",curr->key,curr->value);
            prev=curr;
            curr=curr->next;
        }
        printf(" \n");
        L=-level-1;
        curr=list->down;
        while(L!=curr->key) curr=curr->down;
        curr=curr->next;
        n=spenode(list,curr->key,L+1);
        printf("%13s"," ");
        if(n==0) printf("%3s|%9s"," "," ");
        else printf("%*s%3s|%9s",13*n," "," "," ");
        prev=curr;
        curr=curr->next;
        while(curr){
            n=spenode(list,prev->key,L+1);
            n=spenode(list,curr->key,L+1)-n-1;
            if (n==0) printf("%3s|%9s"," "," ");
            else printf("%*s%3s|%9s",13*n," "," "," ");
            prev=curr;
            curr=curr->next;
        }
        printf("\n");
        L=-level;
        printf("%3d:%5s ",L," ");
        curr=list->down;
        while(curr->key!=L) curr=curr->down;
        curr=curr->next;
        while(curr){
            printf("-> %3d:%5s ",curr->key,curr->value);
            curr=curr->next;
        }
        printf("-> \n");
    }
}
int spenode(node *list,int key,int L){
    int n=0;
    node *curr=list->down;
    while(curr->key!=L) curr=curr->down;
    while(curr->next->key!=key){
        curr=curr->next;
        n++;
    }
    return n;
}
node *find(node *list, int key){
    node *curr;
    node *prev;
    if(isitin(list,key)==0) return NULL;
    prev=list->down;
    curr=list->down->next;
    while(curr){
        if(curr->key > key){
            prev=prev->down;
            curr=prev;
        }
        if(curr->key==key){
            return curr;
        }
        prev=curr;
        curr=curr->next;
        if(curr==NULL){
            prev=prev->down;
            curr=prev;
        }
    }
    return NULL;    
}
void path(node *list, int key){
    node *curr;
    node *prev;
    node *blue;
    printf("%4d v %3d",list->key,list->down->key);
    blue=list->down;
    prev=list->down;
    curr=list->down->next;
    while(curr){
        if(curr->key > key){
            while(blue->next!=curr){
                printf(" > %3d",blue->next->key);
                blue=blue->next;
            }
            prev=prev->down;
            blue=prev;;
            curr=prev;
            printf(" v %3d",prev->key);
        }
        if(curr->key==key){
            while(blue->next!=curr){
                printf(" > %3d",blue->next->key);
                blue=blue->next;
            }
            printf(" > %3d:%5s",curr->key,curr->value);
            break;
        }
        prev=curr;
        curr=curr->next;
        if(curr==NULL){
            while(blue->next!=curr){
                printf(" > %3d",blue->next->key);
                blue=blue->next;
            }
            prev=prev->down;
            blue=prev;
            curr=prev;
            printf(" v %3d",prev->key);
        }
    }
    printf("\n");
}
