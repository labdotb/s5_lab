#include <stdio.h>
#include <stdlib.h>

int main()
{
        int RQ[50], Total_head_movement = 0, initial, current, n, position, i, side = 0, value;
        printf("Enter the no of requests: ");
        scanf("%d", &n);
        printf("Enter the request sequence: ");
        for (i = 0; i < n; i++)
                scanf("%d", &RQ[i]);
        printf("Enter the initial head position: ");
        scanf("%d", &initial);
        int m = n;

        while (n >= 0)
        {
                if (side == 0)
                        value = 0;
                else
                        value = 200;
                for (i = 0; i < m; i++)
                {
                        switch (side)
                        {
                        case 0:
                                if (RQ[i] > value && RQ[i] < initial)
                                {
                                        value = RQ[i];
                                        position = i;
                                }
                                break;
                        case 1:
                                if (RQ[i] < value && RQ[i] > initial && RQ[i] != -1)
                                {
                                        value = RQ[i];
                                        position = i;
                                }
                                break;
                        }
                }
                current = RQ[position];
                RQ[position] = -1;
                if (current == -1)
                {
                        Total_head_movement += abs(initial - 0);
                        side = 1 - side;
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