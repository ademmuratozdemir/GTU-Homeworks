

/*This program is a program that shows the number of integers in a txt file.*/
/*Since it compares the numbers one by one, the number of processes and the duration of the process increase depending on the file size and the number of integers in the file.*/




#include <stdio.h>
#include <stdlib.h>

int main() {
  FILE *fin, *fout;
  int number, flag=1, temp=0, flag2, x;
  int arr[100];
  int count[100]={0};
  int i=0, j=0, k=0;
  fin=fopen("input.txt","r");
  fout=fopen("output.txt","w");

  if(fin==NULL){
    printf("The file does not exist.\n");     /*We check if it is opened.If it is not,program will be terminated.*/
    exit(1);
  }

  if(fout==NULL){
		printf("Couldn't create text file.\n");    /*We check if it is created for writing.If it is not,program will be terminated.*/
		exit(1);
  }

  fscanf(fin,"%d", &number);      /*Program will take a integer and it puts it 0. index of an array.*/
  arr[i]=number;
  while(!feof(fin)){
    fscanf(fin,"%d", &number);     /*At this stage,program will save all different integers in txt file in another array*/
    for(j=0; j<i+1; j++){
      if(arr[j]==number){
        flag=0;
      }
    }
    if(flag!=0){
      arr[i+1]=number;
      i++;
    }
    flag=1;
  }

  for (j = 0; j < i+1; j++) {
        for (k = j+1; k < i+1; k++) {      /*At this stage,program will reorder the integers in this array from least to greatest.*/
           if(arr[j] > arr[k]) {
               temp = arr[j];
               arr[j] = arr[k];
               arr[k] = temp;
           }
        }
    }

  for(j=0; j<i+1; j++){
    fseek(fin,0,SEEK_SET);
    while(fscanf(fin,"%d", &number)!=EOF){     /*At this stage,program will compare all the elements in the array with different numbers with the numbers in the original txt file and save their numbers in the count array.*/
      if(number==arr[j]){
        count[j]++;
      }
    }
  }

  x=1;
  while (x<=i+1) {
    for(j=0; j<i+1; j++){
      if(count[j]==x){
        fprintf(fout, "%d:%d\n", arr[j], count[j]);   /*At this stage,program will print the number of elements in the array with different integers to the output file.*/
      }
    }
    x++;
  }
  fclose(fin);     /*Here,we close the input file.*/
  fclose(fout);    /*Here,we close the output file.*/

  return 0;
}
