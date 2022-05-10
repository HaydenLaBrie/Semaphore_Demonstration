#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//I hand made the semaphores
//On macos I was running into errors when using sem_init and such...
//These should perform the exact same

int b = 1;
void loopwait(){
  while(b < 1){
    ;
  }
  b--;
}

void loopsignal(){
  b++;
}


int d = 1;
void box2wait(){
  while(d < 1){

  }
  d--;
}

void box2signal(){
  d++;
}

int e = 1;
void box1wait(){
  while(e < 1){

  }
  e--;
}

void box1signal(){
  e++;
}

int f = 0;

void ticketwait(){
  while(f < 1){

  }
  f--;
}

void ticketsignal(){
  f++;
}

int g = 0;

void concessionwait(){
  while(g < 1){

  }
  g--;
}

void concessionsignal(){
  g++;
}


char* movie0[5];
char* movie1[5];
char* movie2[5];
char* movie3[5];
char* movie4[5];

int sizeof0 = -1;
int sizeof1 = -1;
int sizeof2 = -1;
int sizeof3 = -1;
int sizeof4 = -1;

int tickets0;
int tickets1;
int tickets2;
int tickets3;
int tickets4;


//this is used to keep track of how many customer threads are existing
int x;

//struct to hold customer information together in a package
struct customer{
  int id;
  bool hasticket;
  bool concession;
  char movie[250];

};

struct customer customers[50];

int customerid = 0;

//Customer thread predetermines what movie the customer will pick and decide if they will be able to even have
//a ticket ahead of time. It also determines if they will be going to the concession stand
void *customer(void *args){
  customerid++;
  customers[customerid - 1].id = customerid;
  int movie;
  movie = (rand() % 5);
  char str1[100], str2[100];
  //logic on deciding which movie the customer picks
  if(movie == 0){
    //string concatination
    strcpy(str1, movie0[0]);
    strcpy(str2, " ");
    strcat(str1, str2);
    strcpy(str2, movie0[1]);
    strcat(str1, str2);
    strcat(customers[customerid - 1].movie, str1);
    //deciding if there will be a ticket available
    if(tickets0 > 0){
      tickets0--;
      customers[customerid - 1].hasticket = true;
    }
    else{
      customers[customerid - 1].hasticket = false;
    }
  }
  if(movie == 1){
    //string concatination
    strcpy(str1, movie1[0]);
    strcpy(str2, " ");
    strcat(str1, str2);
    strcpy(str2, movie1[1]);
    strcat(str1, str2);
    strcpy(str2, " ");
    strcat(str1, str2);
    strcpy(str2, movie1[2]);
    strcat(str1, str2);
    strcat(customers[customerid - 1].movie, str1);
    //deciding if there will be a ticket available
    if(tickets1 > 0){
      tickets1--;
      customers[customerid - 1].hasticket = true;
    }
    else{
      customers[customerid - 1].hasticket = false;
    }
  }
  if(movie == 2){
    //string concatination
    strcpy(str1, movie2[0]);
    strcpy(str2, " ");
    strcat(str1, str2);
    strcpy(str2, movie2[1]);
    strcat(str1, str2);
    strcpy(str2, " ");
    strcat(str1, str2);
    strcpy(str2, movie2[2]);
    strcat(str1, str2);
    strcpy(str2, " ");
    strcat(str1, str2);
    strcpy(str2, movie2[3]);
    strcat(str1, str2);
    strcat(customers[customerid - 1].movie, str1);
    //deciding if there will be a ticket available
    if(tickets2 > 0){
      tickets2--;
      customers[customerid - 1].hasticket = true;
    }
    else{
      customers[customerid - 1].hasticket = false;
    }
  }
  if(movie == 3){
    //string concatination
    strcpy(str1, movie3[0]);
    strcpy(str2, " ");
    strcat(str1, str2);
    strcpy(str2, movie3[1]);
    strcat(str1, str2);
    strcat(customers[customerid - 1].movie, str1);
    //deciding if there will be a ticket available
    if(tickets3 > 0){
      tickets3--;
      customers[customerid - 1].hasticket = true;
    }
    else{
      customers[customerid - 1].hasticket = false;
    }
  }
  if (movie == 4){
    //string concatination
    strcpy(str1, movie4[0]);
    strcpy(str2, " ");
    strcat(str1, str2);
    strcpy(str2, movie4[1]);
    strcat(str1, str2);
    strcat(customers[customerid - 1].movie, str1);
    //deciding if there will be a ticket available
    if(tickets4 > 0){
      tickets4--;
      customers[customerid - 1].hasticket = true;
    }
    else{
      customers[customerid - 1].hasticket = false;
    }
  }
  loopsignal(); //Signal that customer is made
  return NULL;
}

int ticketcounter = -1;

//Tickettaker thread handles having customers that purchase a ticket to wait in a line
//as well as get there ticket torn
void *ticketTaker(){
  //needs to wait to run until a box office is finished being created
  while(ticketcounter < 49){
    ticketwait();
    ticketcounter++;
    if(customers[ticketcounter].hasticket == true){
      printf("Ticket taker: Customer %d is in line.\n", customers[ticketcounter].id);
    }
    if(customers[ticketcounter].hasticket == true){
      printf("Ticket taker: ticket has been taken from Customer %d.\n", customers[ticketcounter].id);
      usleep(150000); //0.15 seconds of sleep
    }
    concessionsignal();
  }
  return NULL;
}

int boxcounter = -1;

//boxoffice1 thread to handle movie ticket purchases
void *boxoffice01(){
  while(boxcounter < 49){
    usleep(1500000); //sleep for 1.5 seconds
    box2wait();
    boxcounter++;
    if(customers[boxcounter].hasticket == true){
      printf("Box Office 0: Customer %d has purchased ticket for %s.\n", customers[boxcounter].id, customers[boxcounter].movie);

      int concession;
      concession = (rand() % 2);
      if(concession == 0){
        customers[boxcounter].concession = false;
      }
      else{
        customers[boxcounter].concession = true;
      }
    }
    else{
      printf("Box Office 0: %s is sold out, thus customer %d is leaving.\n", customers[boxcounter].movie, customers[boxcounter].id);
      customers[boxcounter].concession = false;
    }
    ticketsignal();
    box1signal();
  }
  return NULL;
}

//boxoffice2 thread to handle movie ticket purchases
void *boxoffice02(){
  while(boxcounter < 49){
    usleep(1500000); //sleep for 1.5 seconds
    box1wait();
    boxcounter++;
    if(customers[boxcounter].hasticket == true){
      printf("Box Office 1: Customer %d has purchased ticket for %s.\n", customers[boxcounter].id, customers[boxcounter].movie);

      int concession;
      concession = (rand() % 2);
      if(concession == 0){
        customers[boxcounter].concession = false;
      }
      else{
        customers[boxcounter].concession = true;
      }
    }
    else{
      printf("Box Office 1: %s is sold out, thus customer %d is leaving.\n", customers[boxcounter].movie, customers[boxcounter].id);
      customers[boxcounter].concession = false;
    }
    ticketsignal();
    box2signal();
  }
  return NULL;
}

int concesscount = -1;

//concession thread to handle popcorn and soda purchases after a customer went through the ticket line
void *concession(){
  while(concesscount < 49){
    //Needs to wait until tickettaker is completed
    concessionwait();
    concesscount++;
    if(customers[concesscount].concession == true){
      int snack;
      snack = (rand() % 3);
      if(snack == 0){
        printf("Concession: Customer %d has purchased popcorn.\n", customers[concesscount].id);
      }
      if(snack == 1){
        printf("Concession: Customer %d has purchased soda.\n", customers[concesscount].id);
      }
      if(snack == 2){
        printf("Concession: Customer %d has purchased popcorn and soda.\n", customers[concesscount].id);
      }
      usleep(3000000); //3 seconds of sleep
    }
  }
  return NULL;
}


int main(){
  //Read input file of movie and ticket number file line by line
  //File pointer
  FILE *fp;
  fp = fopen("movies.txt", "r");
  if(fp == NULL){
    printf("file open error\n");
    return 1;
  }
  char* tokens[50];
  char *line = NULL;
  size_t len = 0;
  //j is used to track which substring part
  int j;
  int i = 0;
  while ((getline(&line, &len, fp)) != -1){
    //line is an array of characters
    //piece is a char pointer that stores the sub strings of a line
    //where a string is broken into sub strings by a space
    char *piece = strtok(line, " ");
    j = 0;
    while(piece != NULL){
      if(i == 0){
        movie0[j] = strdup(piece);
        sizeof0++;
      }
      if(i == 1){
        movie1[j] = strdup(piece);
        sizeof1++;
      }
      if(i == 2){
        movie2[j] = strdup(piece);
        sizeof2++;
      }
      if(i == 3){
        movie3[j] = strdup(piece);
        sizeof3++;
      }
      if(i == 4){
        movie4[j] = strdup(piece);
        sizeof4++;
      }

      piece = strtok(NULL, " ");
      j++;
    }
    i++;
  }

  //making a int ticket to the last string in array
  tickets0 = atoi(movie0[sizeof0]);
  tickets1 = atoi(movie1[sizeof1]);
  tickets2 = atoi(movie2[sizeof2]);
  tickets3 = atoi(movie3[sizeof3]);
  tickets4 = atoi(movie4[sizeof4]);

  //Create array of 50 customer threads as well as the other threads, then join
  pthread_t customerthreads[50];
  pthread_t tickettaker;
  pthread_t boxoffice1, boxoffice2;
  pthread_t concess;

  for(x = 0; x < 50; x++){
    loopwait(); //If customer thread is being made, then wait
    pthread_create((customerthreads + x), NULL, &customer, NULL);
    }

    for(x = 0; x < 50; x++){
      pthread_join(customerthreads[x], NULL);
    }
    pthread_create(&boxoffice1, NULL, boxoffice01, NULL);
    pthread_create(&boxoffice2, NULL, boxoffice02, NULL);
    pthread_create(&tickettaker, NULL, &ticketTaker, NULL);
    pthread_create(&concess, NULL, &concession, NULL);
    pthread_join(boxoffice1, NULL);
    pthread_join(boxoffice2, NULL);
    pthread_join(tickettaker, NULL);
    pthread_join(concess, NULL);

  return 0;
}
