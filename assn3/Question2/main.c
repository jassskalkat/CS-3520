////
////  main.c
////  DeadlockDetectionAlgorithm
////
////  Created by James Hall on 9/12/15.
////  Copyright (c) 2015 James Hall. All rights reserved.
////  Converting Bankers Algorithm to Deadlock Detection Algorithm.
////  Should be able to simulate Deadlock Detection and Recovery.
//
//#include <stdio.h>
///* Our Data Structure */
//int available[10];// m number of resource types
//int allocation[10][10];// Matrix of nxm, n = #resources m= #resourcetypes
//int request[10][10];// max [n][m] n = resource, m = resourcesTypes
//int need[10][10];
//
//int numberOfProcesses, numberOfResources;// process, resource
//
//// it's C so declare your methods at the top.
//void input();// Asks for all inputs
//void show();// Shows all outpus
//void compute();// Does all the heavy lifting.
//
//// In main we put all of our functions together so they
//int main()
//{
//    printf("Deadlock Detection Algorithm\n");
//    input();
//    show();
//    compute();
//    return 0;
//}
//// I could put in some validation, but it's more important to show the concepts
//void input()
//{
//    int i, j;// counters
//    printf("Enter the number of Processes:\t");// single integer, no limits
//    scanf("%d", &numberOfProcesses);
//    printf("Enter the number of Resource Instances:\t");// single integer, no limits
//    scanf("%d",&numberOfResources);
//    printf("Enter the Request Matrix:\n");
//    for( i = 0; i < numberOfProcesses; i++)
//    {
//        printf("process %d:\n", i);
//        for(j = 0; j < numberOfResources; j++)
//        {
//            scanf("%d", &request[i][j]);
//        }
//    }
//    printf("Enter the Allocation Maxtrix:\n");
//    for(i = 0; i < numberOfProcesses; i++)
//    {
//        printf("process %d:\n", i);
//        for (j = 0; j < numberOfResources; j++)
//        {
//            scanf("%d", &allocation[i][j]);
//        }
//    }
//    printf("Enter the available Resources:\n");
//    for(j = 0; j < numberOfResources; j++)
//    {
//        scanf("%d", &available[j]);
//    }
//}
//
//void show()
//{
//    int i, j;
//    printf("Process\t Allocation\t Request\t Available\t ");
//
//    for(i = 0; i < numberOfProcesses; i++)
//    {
//        printf("\nP%d\t", i+1);
//
//        for(j = 0; j < numberOfResources; j++)
//        {
//            printf("%d ", allocation[i][j]);
//        }
//        printf("\t ");
//
//        for(j = 0; j < numberOfResources; j++)
//        {
//            printf("%d ", request[i][j]);
//        }
//        printf("\t");
//
//        if (i == 0) {
//            for (j = 0; j < numberOfResources; j++)
//            {
//                printf("%d ", available[j]);
//            }
//        }
//    }
//}
//
//void compute()
//{
//    int finish[10], need[10][10], flag = 1, k;// for the integer arrays, we are declared the maximum space we could take up.
//    int dead[10];
//    int i, j;
//    for( i = 0; i < numberOfProcesses; i ++ )
//    {
//        finish[i]=0;
//    }
//    for(i = 0; i < numberOfProcesses; i++)
//    {
//        for(j = 0; j < numberOfResources; j++)
//        {
//            need[i][j] = request[i][j] - allocation[i][j];
//        }
//    }
//    while(flag)
//    {
//        flag = 0;
//        for (i = 0; i < numberOfProcesses; i++)
//        {
//            int c = 0;
//            for( j = 0; j < numberOfResources; j++)
//            {
//                if ( ( finish[i] == 0 ) && ( need[i][j] <= available[j] ) )
//                {
//                    c++;
//                    if ( c == numberOfResources )
//                    {
//                        for(k = 0; k < numberOfResources; k++)
//                        {
//                            available[k] += allocation[i][j];// slightly different from the algorithm in our paper.
//                            finish[i] = 1;
//                            flag = 1;
//                        }
//                        if (finish[i]==1)
//                        {
//                            i = numberOfProcesses;
//                        }
//                    }
//                }
//            }
//        }
//    }
//    // go through and find the deadlocked processes.
//    j = 0;
//    flag = 0;
//    for(i = 0; i < numberOfProcesses; i++)
//    {
//        if(finish[i] == 0)
//        {
//            dead[j] = i;
//            j++;
//            flag = 1;
//        }
//    }
//    if(flag == 1)
//    {
//        printf("\n\nSystem is in Deadlock and the Deadlock process are \n");
//        for(i = 0; i < numberOfProcesses; i++)
//        {
//            printf("Process#%d\t", dead[i]);
//        }
//    }else{
//        printf("\n No Deadlock");
//    }
//}



#include<stdio.h>

int max[100][100];
int alloc[100][100];
int avail[100];
int n,r;
void input();
void display();
void calculate();
int main()
{
    input();
    display();
    calculate();
    return 0;
}

void input()
{
    int i,j;
    printf("Enter the # of Processes: ");
    scanf("%d",&n);
    printf("Enter the # of resource instances: ");
    scanf("%d",&r);
    printf("Enter the Max Matrix:\n");
    for(i=0;i<n;i++)
    {
        for(j=0;j<r;j++)
        {
            scanf("%d",&max[i][j]);
        }
    }
    printf("Enter the Allocation Matrix:\n");
    for(i=0;i<n;i++)
    {
        for(j=0;j<r;j++)
        {
            scanf("%d",&alloc[i][j]);
        }
    }
    printf("Enter the available Resources: \n");
    for(j=0;j<r;j++)
    {
        scanf("%d",&avail[j]);
    }
}
void display()
{
    int i,j;
    printf("Process\t\tMax\t\tAllocation\t\tAvailable");
    for(i=0;i<n;i++)
    {
        printf("\nP%d\t\t\t",i+1);
        for(j=0;j<r;j++)
        {
            printf("%d ",max[i][j]);
        }
        printf("\t\t");
        for(j=0;j<r;j++)
        {
            printf("%d ",alloc[i][j]);
        }
        printf("\t\t");
        if(i==0)
        {
            for(j=0;j<r;j++)
            {
                printf("%d ",avail[j]);
            }
        }
    }
}
void calculate()
{
    int finish[100],dead[100],need[100][100];
    int flag=1,i,j,k;
    for(i=0;i<n;i++)
    {
        finish[i]=0;
    }
    for(i=0;i<n;i++)
    {
        for(j=0;j<r;j++)
        {
            need[i][j]=max[i][j]-alloc[i][j];
        }
    }
    while(flag==1)
    {
        flag=0;
        for(i=0;i<n;i++)
        {
            int c=0;
            for(j=0;j<r;j++)
            {
                if((finish[i]==0)&&(need[i][j]<=avail[j]))
                {
                    c++;
                    if(c==r)
                    {
                        for(k=0;k<r;k++)
                        {
                            avail[k]= avail[k] + alloc[i][j];
                            finish[i]=1;
                            flag=1;
                        }
                        if(finish[i]==1)
                        {
                            i=n;
                        }
                    }
                }
            }
        }
    }
    j=0;
    flag=0;
    for(i=0;i<n;i++)
    {
        if(finish[i]==0)
        {
            dead[j]=i;
            j++;
            flag=1;
        }
    }
    if(flag==1)
    {
        printf("\n\nSystem is in Deadlock and the Deadlock process are:\n");
        for(i=0;i<n;i++)
        {
            printf("P%d\t",dead[i]+1);
        }
    }
    else
    {
        printf("\n\nNo Deadlock Detected");
    }
}