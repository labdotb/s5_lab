w#include <stdio.h>
#include <stdlib.h>

int main()
{
        int n, m, i, j;
        printf("Enter the no of processes: ");
        scanf("%d", &n);
        printf("Enter the no of resources: ");
        scanf("%d", &m);
        int MAX[n][m], ALLOCATION[n][m], AVAILABLE[m], NEED[n][m];
        printf("Enter the MAX matrix: \n");
        for (i = 0; i < n; i++)
        {
                for (j = 0; j < m; j++)
                {
                        scanf("%d", &MAX[i][j]);
                }
        }
        printf("Enter the ALLOCATION matrix: \n");
        for (i = 0; i < n; i++)
        {
                for (j = 0; j < m; j++)
                {
                        scanf("%d", &ALLOCATION[i][j]);
                }
        }
        printf("Enter the AVAILABLE array: \n");
        for (i = 0; i < m; i++)
        {
                scanf("%d", &AVAILABLE[i]);
        }

        // calculating need
        for (i = 0; i < n; i++)
        {
                for (j = 0; j < m; j++)
                {
                        NEED[i][j] = MAX[i][j] - ALLOCATION[i][j];
                }
        }
        printf("NEED Matrix: \n");
        for (i = 0; i < n; i++)
        {
                for (j = 0; j < m; j++)
                {
                        printf("%d", NEED[i][j]);
                }
                printf("\n");
        }

        // applying safety algorithm
        int WORK[m], FINISH[n], need_less_than_work;
        for (i = 0; i < n; i++)
                FINISH[i] = 0; // 0 means false
        for (i = 0; i < m; i++)
        {
                WORK[i] = AVAILABLE[i];
        }
        int next_loop = 1, PROCESS_ORDER[n], pno = 0;
        while (next_loop == 1)
        {
                next_loop = 0;
                for (i = 0; i < n; i++)
                {
                        if (FINISH[i] == 0)
                        {
                                need_less_than_work = 1;
                                for (j = 0; j < m; j++)
                                {
                                        if (WORK[j] < NEED[i][j])
                                        {
                                                need_less_than_work = 0;
                                                break;
                                        }
                                }

                                if (need_less_than_work == 1)
                                {
                                        for (int k = 0; k < m; k++)
                                        {
                                                WORK[k] += ALLOCATION[i][k];
                                        }
                                        FINISH[i] = 1;
                                        PROCESS_ORDER[pno++]=i;
                                        next_loop = 1;
                                }
                        }
                }
        }
        if(pno == n)
        {
                printf("\nSystem is in safe state");
                printf("\nThe order of exicution is ");
                for ( i = 0; i < n; i++)
                {
                        printf("%d ", PROCESS_ORDER[i]);
                }   
        }
        else
                printf("\nSystem is not in safe state");
}