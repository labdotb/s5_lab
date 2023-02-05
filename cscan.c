#include <stdio.h>
#include <stdlib.h>
int end = 199;
int main()
{
        int Total_head_movement = 0, initial, current, n, position, i, value;
        printf("Enter the no of requests: ");
        scanf("%d", &n);
        int RQ[n];
        printf("Enter the request sequence: ");
        for (i = 0; i < n; i++)
                scanf("%d", &RQ[i]);
        printf("Enter the initial head position: ");
        scanf("%d", &initial);
        int m = n;

        while (n >= 0)
        {
                value = end;
                for (i = 0; i < m; i++)
                {
                        if (RQ[i] < value && RQ[i] > initial && RQ[i] != -1)
                        {
                                value = RQ[i];
                                position = i;
                        }
                }
                current = RQ[position];
                RQ[position] = -1;
                if (current == -1)
                {
                        Total_head_movement += abs(initial - end);
                        Total_head_movement += end;
                        initial = 0;
                }
                else
                {

                        Total_head_movement = Total_head_movement + abs(initial - current);
                        initial = current;
                }
                n--;
        }

        printf("Total head movement : %d", Total_head_movement);
}