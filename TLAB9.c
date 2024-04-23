#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void compareVersions(FILE *f){
    char v1[100][100];
    char v2[100][100];

    //strcpy(v1,"");
    //strcpy(v2,"");

    printf("aici");
    char line[100];
    int i=0;
    while(fgets(line,sizeof(line),f) != NULL){
        if(strstr(line,"stop") != NULL){
            break;
        }
        strcat(v1[i++],line);
    }
    i=0;
    while(fgets(line,sizeof(line),f) != NULL){
        if(strstr(line,"stop") != NULL){
            break;
        }
        strcat(v2[i++],line);
    }

    int j;
    for(j=0;j<=i;j++)
       if(strcmp(v1[j],v2[j]) != 0){
           printf("Fisierul s a modificat\n");
       }
    
    for(j=0;j<=i;j++){
        if(strcmp(v1[j],v2[j]) != 0){
            printf("%s <->\n%s\n",v1[j],v2[j]);
        }
    }
    

    
    
    

    
    
}


void parcurgereRecursiva(FILE *f,char *nume){
    DIR *dir;
    struct dirent *in;
    if((dir = opendir(nume)) == NULL){
        perror("Eroare la opendir\n");
        return;
    }

     while((in = readdir(dir)) != NULL){
          if(in->d_type == DT_DIR){
              if(strcmp(in->d_name,".") == 0 || strcmp(in->d_name,"..") == 0)
                     continue;
              else{
                fprintf(f,"Director: %s\n",in->d_name);
                char x[1000];
                strcpy(x,nume);
                strcat(x,"/");
                strcat(x,in->d_name);
                parcurgereRecursiva(f,x);
              }
          }
          else{
            fprintf(f,"Fisier: %s\n",in->d_name);
          }
        
    }


    closedir(dir);
    
}


int main(int argv,char *argc[]){
    FILE *f;
    if(argv > 11){
        printf("Prea multe argumente\n");
        exit(-1);
    }
    int i;
    for(i=1;;i++){
        if(strcmp(argc[i],"-o") == 0)
            break;
    }
    i++;
    
    printf("%s\n",argc[1]);
    if((f = fopen(argc[i],"a+")) == NULL){
        printf("Eroare la deschidere de fisier\n");
        exit(-1);
    }
    int x = 1;
   /* for(x=1;x<=argv;x++){
        if(x == i-1)
            continue;
        DIR *dir;
        struct dirent *in;
        if((dir = opendir(argc[x])) == NULL){
             continue;
        }
        in = readdir(dir);
        if(in->d_type == DT_DIR){
            parcurgereRecursiva(f,argc[x]);
            fprintf(f,"stop\n");
        }
    }*/
    
    parcurgereRecursiva(f,argc[2]);
    
      





}