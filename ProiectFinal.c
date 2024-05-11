#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>






    void parcurgereRecursiva(FILE *f, char *nume) {
    DIR *dir;
    struct dirent *in;
    if ((dir = opendir(nume)) == NULL) {
        perror("Eroare la deschiderea directorului\n");
        return;
    }

    while ((in = readdir(dir)) != NULL) {
        if (in->d_type == DT_DIR) {
            if (strcmp(in->d_name, ".") == 0 || strcmp(in->d_name, "..") == 0)
                continue;
            else {
                fprintf(f, "Director: %s\n", in->d_name);
                char x[1000];
                snprintf(x, sizeof(x), "%s/%s", nume, in->d_name); // construieste full path pentru a folosi in stat cand e nevoie
                parcurgereRecursiva(f, x);
            }
        } else {
            fprintf(f, "Fisier: %s\n", in->d_name);
            char nume_fisier[100];
            snprintf(nume_fisier, sizeof(nume_fisier), "%s/%s", nume, in->d_name); // construieste full path pentru a folosi in stat
            struct stat st;
            if (stat(nume_fisier, &st) == 0) {
                if (st.st_mode == 32768) {
                    mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH;
                    if(chmod(nume_fisier,mode) < 0){
                        perror("Eroare la chmod\n");
                        exit(EXIT_FAILURE);
                    }
                    pid_t pidx;
                    pidx = fork();
                    if(pidx < 0){
                        perror("Eroare\n");
                        exit(1);
                    }
                    if(pidx == 0){
                        if(execl("/bin/sh","sh","verify_for_malicious.sh",nume_fisier,NULL) == -1){
                            perror("Eroare la exec\n");
                            exit(EXIT_FAILURE);
                        }
                    }
                    int status;
                    pid_t pidy=wait(&status);
                }
            }
        }
    }

    closedir(dir);
}



int main(int argv,char *argc[]){
    int i;
    
    if(argv > 10){
        printf("Prea multe argumente\n"); // verificare nr argumente
        exit(-1);
    }

    int nr_dir_ies;
    int nr_dir_safe;
    for(i=1;i<argv;i++){
        if(strcmp(argc[i],"-o") == 0){
            nr_dir_ies = i+1;            // gasire numar director in care se stocheaza toate snapshoturile
            break;
        }
    }

    for(i=1;i<argv;i++){
        if(strcmp(argc[i],"-s") == 0){    // gasire nr director safe
            nr_dir_safe = i+1;
            break;
        }
    }
    struct stat st;
    if(stat(argc[nr_dir_ies],&st) == -1){    // cazul in care directorul de stocare nu exista , il creeaza
          mkdir(argc[nr_dir_ies],0777);
    }

    struct stat st2;
    if(stat(argc[nr_dir_safe],&st2) == -1){    // cazul in care directorul safe nu exista , il creeaza
          mkdir(argc[nr_dir_safe],0777);
    }

    char cwd[100];
    getcwd(cwd,sizeof(cwd));                 // aflu numele directorului curent

   chdir(argc[nr_dir_ies]);                  // schimba directorul catre cel cu snapshoturile
   pid_t pid;
    for(i=1;i<argv;i++){
        char filename[100];
        chdir(argc[nr_dir_ies]);                  // schimba directorul catre cel cu snapshoturile
        if(i==nr_dir_ies || i==nr_dir_ies-1 || i == nr_dir_safe || i == nr_dir_safe-1)
              continue;
        pid = fork();
        if(pid < 0){
            perror("Eroare\n");
            exit(1);
        }
        if(pid == 0){
            sprintf(filename,"SNAPSHOT%d",i);
                FILE *f;
                if((f=fopen(filename,"w")) == NULL){
                       printf("Eroare la deschidere\n");
                       exit(-1);
                }
                chdir(cwd);                  // schimba directorul catre cel cu directoarele
                fprintf(f,"Director principal: %s\n",argc[i]);
                parcurgereRecursiva(f,argc[i]);
                fclose(f);
                printf("Snapshot for %s created successfully\n",argc[i]);
                exit(0);
        }
        }
        int status;
        for(i=1;i<argv;i++){
            if(i==nr_dir_ies || i==nr_dir_ies-1 || i == nr_dir_safe || i == nr_dir_safe-1)
                continue;
            pid_t pid2 = wait(&status);
            printf(" Procesul cu PID-ul : %d s a incheiat cu codul: %d\n",pid2,WEXITSTATUS(status));
        


    


    }
}