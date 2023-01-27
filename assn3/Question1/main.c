#include <stdio.h>
#include <semaphore.h>

struct monitorDataType {
    sem_t Read;
    sem_t Write;
    int readerCount;
    int isBusyWriting;
    int readRequested;
};
struct monitorDataType monitorData;

int monitorInitialized();
void monitorStartWrite();
void monitorEndWrite();
void monitorStartRead();
void monitorEndRead();

int monitorInitialized(){
    int r = 1;
    monitorData.readerCount = 0;
    monitorData.isBusyWriting = 0;
    monitorData.readRequested = 0;
    if(sem_init(&(monitorData.Write),0,1) == 0 && sem_init(&(monitorData.Read),0,1) == 0){
        r = 0;
    }
    return r;
}

void monitorStartWrite(){
    if(monitorData.isBusyWriting || monitorData.readerCount != 0){
        sem_wait(&(monitorData.Write));
    }
    monitorData.isBusyWriting++;
}

void monitorEndWrite(){
    monitorData.isBusyWriting--;
    if(monitorData.readRequested){
        sem_post(&(monitorData.Read));
    } else {
        sem_post(&(monitorData.Write));
    }
}

void monitorStartRead(){
    if(monitorData.isBusyWriting){
        monitorData.readRequested++;
        sem_wait(&(monitorData.Read));
        monitorData.readRequested--;
    }
    monitorData.readerCount++;
    sem_post(&(monitorData.Read));
}

void monitorEndRead(){
    monitorData.readerCount--;
    if(monitorData.readerCount == 0){
    sem_post(&(monitorData.Write));
    }
}

int main(){

    if(monitorInitialized() == 0){
        printf("Initialized\n");
        monitorStartWrite();
        printf("Im writing now...\n");
        monitorEndWrite();
        monitorStartRead();
        printf("Im reading now...\n");
        monitorEndRead();
    }
    return 0;
}
