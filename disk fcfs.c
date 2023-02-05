#include <stdio.h>
#include <stdlib.h>

int main()
{
        int RQ[50], Total_head_movement=0, initial, n;
        printf("Enter the no of requests: ");
        scanf("%d",&n);
        printf("Enter the request sequence: ");
        for(int i=0;i<n;i++)
                scanf("%d",&RQ[i]);
        printf("Enter the initial head position: ");
        scanf("%d", &initial);

        for (int i = 0; i < n; i++)
        {
                Total_head_movement = Total_head_movement + abs(RQ[i]-initial);
                initial = RQ[i];
        }

        printf("Total head movement : %d", Total_head_movement);
        
}