#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>

//fungsi isi angka.txt
void *t1(void *arg) {
    printf("t1\n");
    system("touch angka.txt");

    FILE *fl;
    fl = fopen("angka.txt", "w");
    if (fl == NULL) {
        perror("Error opening file");
        pthread_exit(NULL);
    }

    for(int i = 1; i <= 1000000; i++){
        fprintf(fl, "%d\n", i);
    }

    fclose(fl);
    pthread_exit(NULL);
}

//fungsi isi sub folder dengan angka.txt
void *t2(void *arg) {
    printf("t2\n");
    system("mkdir latihan");

    char cmd_sub[50];
    char path_sub[50];
    for(int i = 1; i <= 1000; i++){

        sprintf(cmd_sub, "mkdir -p latihan/%d", i);
        system(cmd_sub);
        
        sprintf(path_sub, "cp angka.txt latihan/%d", i);
        system(path_sub);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t tid[2];
    int i = 0;

    pthread_create(&(tid[0]), NULL, &t1, NULL);
    pthread_create(&(tid[1]), NULL, &t2, NULL);

    while (i < 2) {
        pthread_join(tid[i], NULL);
        i++;
    }

    return 0;
}