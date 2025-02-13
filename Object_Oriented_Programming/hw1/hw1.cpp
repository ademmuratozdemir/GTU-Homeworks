#include <iostream>  //for cout and cin
#include <cstring>  //for strlen and strcmp
#include <time.h>    //for srand(time(NULL))

using namespace std;


int control1(int arr[],int digit)   // In this function, we check if the digits are different 
{                                   // if there are digits are same,function returns 0, if it is not returns 1.
    int a=1;
    for(int i=0;i<digit;i++)
    {
        for(int j=i+1;j<digit;j++)
        {
            if(arr[i]==arr[j])
            {
                a=0;
            }
        }
    }
    return a;
}
int control2(char *str)
{
    int a=strlen(str);
    for(int i=0;i<a;i++)    // program checks if number is integer.if there is an character that is not integer,program will return 0.
    {
        if(isdigit(str[i])==0)
        {
            return 0;
        }

    }
    return 1;
}

int main(int argc,char** argv)
{
    int digitNumber;          //declaring variables
    srand(time(NULL));        //so that the function does not always return the same number 
    int a,d,e;
    int input,temp1,temp2;
    int secretNumber;
    
    if(argc!=3)
    {
        cout<<"E0\n";        // here, we check the parameter counts,if there are parameters bigger than 3, program will exit.
        exit(1);
    }
    else
    {
        if((strcmp(argv[1],"-r")!=0) && (strcmp(argv[1],"-u")!=0))
        {
            cout<<"E0\n";     // here , we check the argv[1], if argv[1] is not -r or -u,program will exit.
            exit(1);
        }
        else if(strcmp(argv[1],"-r")==0 )
        {
            int c=control2(argv[2]);  // program checks if number is integer.
            if(c==0)
            {
                cout<<"E0\n";
                exit(1);
            }
            digitNumber=stoi(argv[2]);    //here,argv[1] is -r ,we check the argv[2].we take the value on argv[2]
            if(digitNumber<1 || digitNumber>9)  // and equalize it digitNumber.if digitNumber<1 or digitNumber>9 , program will exit.
            {
                cout<<"E0\n";
                exit(1);
            }
            else
            {
                
                int arr1[digitNumber];   //here we declare an array for secretnumber.
                int arr2[digitNumber];   //here we declare an array for input.
                while(a!=1)
                {
                    arr1[0]=rand()%9 + 1;
                    for(int z=1;z<digitNumber;z++)
                    {
                        arr1[z]=rand()%9;             //here program will a unique secretnumber.
                    }
                    a=control1(arr1,digitNumber);
                    
                }
                
                int j=digitNumber;
                int count;
                int count1;    // declaring variables for -r process
                int count2;
                int moveNumber=0;
                
                while(moveNumber!=100)
                {
                    
                    
                    cin>>input;   // program will ask for a guess
                    if (cin.fail())
                    {
                    cout<<"E2\n";   // if user enters something but not integer,program will be terminated.
                    exit(1);
                    }
                    temp1=input;
                    
                    count=0;
                    count1=0;
                    count2=0;
                    j=digitNumber;
                    
                    while(temp1>0)
                    {
                        arr2[j-1]=temp1%10;   // program will split the number into array
                        temp1=temp1/10;       
                        j--;
                        count++;
                    }
                    if(count!=digitNumber)
                    {
                        cout<<"E1";     // if the digit of numbers of guess does not equal digit of numbers of secretnumber,program will exit.
                        exit(1);
                    }
                    d=control1(arr2,digitNumber);
                    if(d==0)
                    {
                        cout<<"E1\n";   // if the guess is not unique, program will exit.
                        exit(1);
                    }
                    
                    for(int t=0;t<digitNumber;t++)
                    {
                        if(arr1[t]==arr2[t])
                        {
                            count1++;         // here program finds the count of correct digits.
                            continue;
                        }
                        for(int k=0;k<digitNumber;k++)
                        {
                            if(arr1[k]==arr2[t])
                            {
                                count2++;    // here program finds the count of misplaced numbers.
                                break;
                            }
                        }
                    }
                    
                    moveNumber++;   // in every loop ,the program will increase the number of moves.
                    
                    if(count1==digitNumber)
                    {
                        cout<<"FOUND "<<moveNumber;   // if user finds the secretnumber,program will print the number of moves and exit.
                        exit(1);
                    }
                    cout<<count1<<" ";   // here program will print the count of correct digits.
                    cout<<count2<<"\n";  // here program will print the count of misplaced numbers.
                    
                }
                cout<<"FAILED\n";
                exit(1);
            }
        }
        else if(strcmp(argv[1],"-u")==0)   
        {
            int g=control2(argv[2]);   // program checks if number is integer.
            if(g==0)
            {
                cout<<"E0\n";
                exit(1);
            }
            secretNumber=stoi(argv[2]);  // if argv[1] is -u ,secretnumber is argv[2].
            if(secretNumber<=0)
            {
                cout<<"E0\n";
                exit(1);
            }
            int z=strlen(argv[2]);   //here we equalize the z to count of digits of secretnumber.
            if(z<1 || z>9 )
            {
                cout<<"E0\n";
                exit(1);
            }
            int arr3[z];   
            int arr4[z];
            temp2=secretNumber;   // declaring variables for -u process
            int r=z;
            int b=1;
            int moveNumber2=0,p;
            int input2;
            int temp3;
            int count3,count4,count5;
            while(temp2>0)
            {
                arr3[r-1]=temp2%10;   // here program will split the number into array
                temp2=temp2/10;
                r--;
            }
            b=control1(arr3,z);   // here program will check the number uniqueness
            if(b==0)
            {
                cout<<"E0\n";
                exit(1);
            }
            while(moveNumber2!=100)
            {
                    
                    
                cin>>input2;  // program will ask for a guess
                if (cin.fail())  // if the user enters something but not integer, program will be terminated.
                {
                    cout<<"E2\n";
                    exit(1);
                }
                temp3=input2;
                    
                count3=0;
                count4=0;
                count5=0;
                p=z;
                    
                while(temp3>0)
                {
                    arr4[p-1]=temp3%10;
                    temp3=temp3/10;  // we check the guess number of digits
                    p--;
                    count3++;
                }
                if(count3!=z)
                {
                    cout<<"E1\n";
                    exit(1);
                }
                e=control1(arr4,z);
                if(e==0)
                {
                    cout<<"E1\n";  // we check the uniqueness of guess
                    exit(1);
                }
                    
                for(int x=0;x<z;x++)
                {
                    if(arr3[x]==arr4[x])
                    {
                        count4++;   // program finds the count of correct digits
                        continue;
                    }
                    for(int y=0;y<z;y++)
                    {
                        if(arr3[y]==arr4[x])
                        {
                            count5++;   // program finds the count of misplaced numbers.
                            break;
                        }
                    }
                }
                    
                moveNumber2++;   // in every loop ,the program will increase the number of moves.
                    
                if(count4==z)
                {
                    cout<<"FOUND "<<moveNumber2;   // if guess is correct,program will print number of moves and exit.
                    exit(1);
                }
                cout<<count4<<" ";   // here program will print count of correct digits.
                cout<<count5<<"\n";  // here program will print count of misplaced numbers.
                    
            }
            cout<<"FAILED\n";
            exit(1);


        }

    }
return 0;
}