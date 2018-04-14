//  main.c
//  SSA
//
//  Created by Yun Yuan on 14-2-24.
//  Copyright (c) 2014年 Yun Yuan. All rights reserved.
//
#define N 12
#define M 400
#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
//type Solution
typedef struct Solution{
    int x[N];
    float fitness;
}Solution;
//type Node
typedef struct Node{
    struct Solution s;
    struct Node *next;
}Node;
//variable statement
int n=N;
int p=5;
//demand
float c[N] = { 15, 10, 12, 18, 5, 24, 11, 16, 13, 22, 19, 20 };
//OD Cost Matrix
float d[N][N] = {
    { 0, 15, 37, 55, 24, 60, 18, 33, 48, 40, 58, 67 },
    { 15, 0, 22, 40, 38, 52, 33, 48, 42, 55, 61, 61 },
    { 37, 22, 0, 18, 16, 30, 41, 28, 20, 58, 39, 39 },
    { 55, 40, 18, 0, 34, 12, 59, 46, 24, 62, 43, 34 },
    { 24, 38, 16, 34, 0, 36, 25, 12, 24, 47, 37, 43 },
    { 60, 52, 30, 12, 36, 0, 57, 43, 12, 50, 31, 22 },
    { 18, 33, 41, 59, 25, 57, 0, 15, 45, 22, 40, 61 },
    { 33, 48, 28, 46, 12, 42, 15, 0, 30, 37, 25, 46 },
    { 48, 42, 20, 24, 24, 12, 45, 30, 0, 38, 19, 19 },
    { 40, 55, 58, 62, 47, 50, 22, 37, 38, 0, 19, 40 },
    { 58, 61, 39, 43, 37, 31, 40, 25, 19, 19, 0, 21 },
    { 67, 61, 39, 34, 43, 22, 61, 46, 19, 40, 21, 0 } };
//initialize list
void initList(Node **pHead)
{
    *pHead = NULL;
    printf("Initialzing a new linked list...Done\n");
}
//print list
void printList(Node *pList)
{
    if(NULL == pList)   //链表为空
    {
        printf("list is empty\n");
    }
    else
    {
        while(NULL != pList)
        {
            printf("%f ",pList->s.fitness);
            pList = pList->next;
        }
        printf("\n");
    }
}
//get the fitness in a pos
float getFitness(Node *pList, int pos)
{
    int i=0;
    
    if(pos < 1)
    {
        printf("getFitness，pos is too small");
        return 0;
    }
    if(pList == NULL)
    {
        printf("getFitness，list is empty\n");
        return 0;
        //exit(1);
    }
    if(i < pos)
    {
        printf("getFitness，pos is too large\n");
        return 0;
    }
    while(pList !=NULL)
    {
        ++i;
        if(i == pos)
        {
            break;
        }
        pList = pList->next; //move next
    }
    return pList->s.fitness;
}
//get a pos according to fitness
int getPos(Node *pList, float fitness)
{
    if(pList == NULL)
    {
        printf("getFitness，list is empty\n");
        return 0;
        //exit(1);
    }
    int pos=1;
    while(NULL != pList){
        if (pList->s.fitness==fitness) {
            break;
        } else {
            pList = pList->next;
        }
        pos++;
    }
    return pos;
}
//insert a node to list
int insertList(Node **pHead,Node **pFirst,Node **pLast,Solution s)
{
    Node *pInsert;
    pInsert = (Node *)malloc(sizeof(struct Node));
    pInsert->s = s;
    if (*pHead==NULL) {
        *pHead=pInsert;
        pInsert->next=NULL;
    }
    else if (*pFirst==NULL&&*pLast!=NULL){
        *pHead=pInsert;
        pInsert->next=*pLast;
    }
    else if (*pLast==NULL&&*pFirst!=NULL){
        (*pFirst)->next=pInsert;
        pInsert->next = NULL;
    }
    else if (*pFirst!=NULL&&*pLast!=NULL)
    {
        (*pFirst)->next=pInsert;
        pInsert->next=*pLast;
    }
    printf("insert %f\n",s.fitness);
    return 0;
}
//fitness calculation
float fitness(int x[12])
{
    //init y
    int y[12][12];
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            y[i][j]=0;
        }
    }
    float temp;
    int b;//min distance index
    for (int j = 0; j < n; j++)
    {
        temp = FLT_MAX;
        for (int i = 0; i < n; i++)
        {
            if (x[i] == 1 && d[i][j] <= temp)
            {
                temp = d[i][j];
                b = i;
            }
        }
        y[b][j] = 1;
    }
    float f=0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (y[i][j] == 1)
                f = f + c[j]*d[i][j];
        }
    }
    return f;
}
//generate a solution
Solution genSolution()
{
    Solution s;
    for (int i=0; i<n; i++) {
        s.x[i]=0;
    }
    int flag=0;
    int r;
    while (flag<p) {
        srand48(time(NULL));
        r=rand()%n;
        if (s.x[r]==0) {
            s.x[r]=1;
            flag++;
        }
    }
    s.fitness=fitness(s.x);
    return s;
}
int randomSampling(Node **pList, int *length, float t[N][2])
{
    //generate random samples
    Solution sample[M];
    for (int j=0; j<M; j++) {
        sample[j]=genSolution();
    }
    //check-in
    Node *p1=NULL;
    Node *p2=NULL;
    for (int j=0; j<M; j++) {
        if (*length==0) {//list is empty, jion
            insertList(pList, &p1, &p2, sample[j]);
            (*length)++;
        }
        else {
            p1=NULL;
            p2=*pList;
            while (p2!=NULL) {
                if (p2->s.fitness>=sample[j].fitness) {
                    if (p2->s.fitness==sample[j].fitness) {//p2->s.fitness==sample[j].fitness
                        bool flag=false;
                        for (int k=0; k<n; k++) {
                            if (p2->s.x[k]!=sample[j].x[k]) {
                                flag=true;
                                break;
                            }
                        }
                        if (flag) {//not existing, jion
                            insertList(pList, &p1, &p2, sample[j]);
                            (*length)++;
                            for (int k=0; k<n; k++) {
                                if (sample[j].x[k]==1) {
                                    t[k][1]+=sample[j].fitness;
                                } else {
                                    t[k][0]+=sample[j].fitness;
                                }
                            }
                            break;
                        } else {//existing, move next
                            if (p2->next!=NULL&&p2->next->s.fitness==sample[j].fitness) {
                                p1=p2;
                                p2=p2->next;
                                continue;
                            } else {
                                break;
                            }
                        }
                    } else {//p2->s.fitness>sample[j].fitness, jion
                        insertList(pList, &p1, &p2, sample[j]);
                        for (int k=0; k<n; k++) {
                            if (sample[j].x[k]==1) {
                                t[k][1]+=sample[j].fitness;
                            } else {
                                t[k][0]+=sample[j].fitness;
                            }
                        }
                        (*length)++;
                        break;
                    }
                    
                } else {//p2->s.fitness<sample[j].fitness, move next
                    p1=p2;
                    p2=p2->next;
                }
            }
            if (p2==NULL) {
                insertList(pList, &p1, &p2, sample[j]);//nothing is larger, join
                for (int k=0; k<n; k++) {
                    if (sample[j].x[k]==1) {
                        t[k][1]+=sample[j].fitness;
                    } else {
                        t[k][0]+=sample[j].fitness;
                    }
                }
            }
            
        }
    }
    return 0;
}
int inteliSampling(Node **pList, int *length, float score[N], float t[N][2])
{
    Solution s;
    for (int i=0; i<n; i++) {
        s.x[i]=0;
    }
    //sort score
    int index[n];
    float grade[n];
    int t_i;
    float t_d;
    //copy score to grade
    for (int i=0; i<n; i++) {
        grade[i]=score[i];
    }
    for (int i=0; i<n; i++) {
        index[i]=i;
    }
    for(int i=0; i<n; i++ )
    {
        for(int j=0 ; j < i ; j++ )
        {
            if( grade[j]  > grade[j+1])
            {
                t_d = grade[j];
                grade[j] = grade[j+1];
                grade[j+1] = t_d;
                t_i = index[j];
                index[j]=index[j+1];
                index[j+1]=t_i;
            }
        }
    }
    //generate a new solution
    for (int i=0; i<p; i++) {
        s.x[index[i]]=1;
        s.fitness=fitness(s.x);
    }
    //check-in
    Node *p1=NULL;
    Node *p2=NULL;
    for (int j=0; j<M; j++) {
        if (*length==0) {//list is empty, jion
            insertList(pList, &p1, &p2, s);
        }
        else {
            p1=NULL;
            p2=*pList;
            while (p2!=NULL) {
                if (p2->s.fitness>=s.fitness) {
                    if (p2->s.fitness==s.fitness) {//p2->s.fitness==sample[j].fitness
                        bool flag=false;
                        for (int k=0; k<n; k++) {
                            if (p2->s.x[k]!=s.x[k]) {
                                flag=true;
                                break;
                            }
                        }
                        if (flag) {//not existing, jion
                            insertList(pList, &p1, &p2, s);
                            (*length)++;
                            for (int k=0; k<n; k++) {
                                if (s.x[k]==1) {
                                    t[k][1]+=s.fitness;
                                } else {
                                    t[k][0]+=s.fitness;
                                }
                            }
                            break;
                        } else {//existing, jump out
                            break;
                        }
                    } else {//p2->s.fitness>sample[j].fitness, jion
                        insertList(pList, &p1, &p2, s);
                        for (int k=0; k<n; k++) {
                            if (s.x[k]==1) {
                                t[k][1]+=s.fitness;
                            } else {
                                t[k][0]+=s.fitness;
                            }
                        }
                        (*length)++;
                        break;
                    }
                    
                } else {//p2->s.fitness<sample[j].fitness, move next
                    p1=p2;
                    p2=p2->next;
                }
            }
            if (p2==NULL) {
                insertList(pList, &p1, &p2, s);//nothing is larger, join
                for (int k=0; k<n; k++) {
                    if (s.x[k]==1) {
                        t[k][1]+=s.fitness;
                    } else {
                        t[k][0]+=s.fitness;
                    }
                }
            }
        }
    }
    return 0;
}

//main function
int main(int argc, const char * argv[])
{
    //clock
    clock_t start, finish;
    float duration;
    start=clock();
    //load data
    printf("Problem:");
    printf("n=12,p=5\n");
    printf("Start...\n");
    //initialization
    int length = 0;
    Node *pList;
    initList(&pList);
    float scoreTable[N];
    float countTable[N][2];
    //iteration
    //random sampling
    randomSampling(&pList, &length, countTable);
    printList(pList);
    //intelligent sampling
    for (int j=0; j<n; j++) {
        scoreTable[j]=countTable[j][1]/(countTable[j][0]+countTable[j][1]);
    }
    inteliSampling(&pList, &length, scoreTable, countTable);
    //convergence check
    
    //output result
    finish=clock();
    duration = (float)(finish - start) / CLOCKS_PER_SEC;
    printf("best fitness:%f \n",pList->s.fitness);
    printf("duration:%f\n", duration);
    printf("scoreTable: ");
    for (int j=0; j<n; j++) {
        printf("%f ", scoreTable[j]);
    }
    printf("\n");
    return 0;
}
