#include <stdio.h>

int main()
{
    FILE *fp;
    fp = fopen ("file.txt", "r+");

    int status;
    int y,temp1,countTemp,temp2,i,count;     /* declaration of variables */
    status = fscanf(fp,"%d",&y);
    
    if(status==1)    /*checks if it can't open the file*/
    {
        
        fseek(fp,0,SEEK_SET);   
        while(!feof(fp))
        {
            fscanf(fp,"%d",&y);
    
            countTemp=y;
            temp1=ftell(fp);      /* location of the variable we keep in outer loop */
            count=0;
            if(countTemp<0)
            {
           countTemp= countTemp*(-1);
            }
           
            while(countTemp>0)    /* checking the number of digits of the number we hold */
            {
                countTemp /= 10;
                count++;
            }
            while(!feof(fp))      /* reading the file until end of file */
            {
               fscanf(fp,"%d",&temp2);
                
                    if(temp2==y)
                    {
                        fseek(fp,-count,SEEK_CUR);
                        for(i=1;i<=count;i++)
                        {
                            fprintf(fp," ");
                        }
                    }
                                  

            }
            fseek(fp,+temp1,SEEK_SET);      /* after the second loop ends, sending the cursor to second number */


  
        }
    }
    return 0;
}