#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#define process_number 5
#define resources_number 3
int allocation[process_number][resources_number];
int max[process_number][resources_number];
int available[resources_number];
int need[process_number][resources_number];
int safe[process_number];
int picked[process_number] = {0};
char file_name[] = "snap.txt";
void Banker_algorithm();
int main()
{
    // open file and allow to append it
    FILE *fp_a = fopen(file_name, "a");
    // append a new line to the file
    fputs("\n", fp_a);
    // close the file
    fclose(fp_a);

    // open file as read only
    FILE *fp = fopen(file_name, "r");
    int line = 0, i=0;
    // line_str will store each line as string
    char line_str[2*resources_number]; 
    
    // if the file can't be opened
    if(fp == NULL)
    {
        printf("Error: could not open file %s", file_name);
        return 1;
    }

    // read file lines
    while(fgets(line_str, 128, fp))
    {
        line_str[strlen(line_str) - 1] = '\0';

        // keep dividing the line string acoourding to the spaces between the resources
        char *ptr = strtok(line_str, " ");
        i=0;
        while(ptr != NULL)
        {
            // save the allocation array with converting each element to int
            if(line<process_number)
            {
                allocation[line][i] = atoi(ptr);
                ptr = strtok(NULL, " ");
                i++;
            }
            // save the max array with converting each element to int
            else if(line<process_number*2)
            {
                max[line-process_number][i] = atoi(ptr);
                ptr = strtok(NULL, " ");
                i++;
            }
            // save the available array with converting each element to int
            else
            {
                available[i] = atoi(ptr);
                ptr = strtok(NULL, " ");
                i++;
            }
        }
        line++;
        i=0;
    }
    // close the file
    fclose(fp);

    // calculate the need (max-allocation)
    for(line=0;line<process_number;line++)
    {
        for(i=0;i<resources_number;i++)
        {
            need[line][i] = max[line][i] - allocation[line][i];
        }
    }

    // apply banker algorithm
    Banker_algorithm();

    return 0;
}
void Banker_algorithm()
{
    int count = 0, safe_count = 0, safe_condition = 0, dead_flag = 0;

    // loop the process till they are taken or deadlock happen (dead_flag)
    while(count<process_number)
    {
        count = 0;
        dead_flag = 0;
        for(int i = 0; i<process_number; i++)
        {
            safe_condition = 0;
            if(picked[i]==1)
            {
                // save the number of picked proccess
                count++;
            }
            else
            {
                // loop the resources to check the safty of the proccess working
                for(int j = 0; j<resources_number; j++)
                {
                    if(need[i][j]<=available[j])
                    {
                        // safe flag
                        safe_condition++;
                    }
                }
                if(safe_condition==resources_number)
                {
                    // increase the proccess number and each safe flag
                    count++;
                    picked[i] = 1;
                    
                    // update the work 
                    for(int j = 0; j<resources_number; j++)
                    {
                        available[j] = available[j] + allocation[i][j];
                        safe[safe_count] = i;
                    }
                    safe_count++;


                    printf("P%d is safe, availale: ",i);
                    for(int j = 0; j<resources_number; j++)
                    {
                        printf("%d ",available[j]);
                    }printf("\n");

                    dead_flag = 1;
                }
                else
                {
                    printf("P%d is not safe, availale: ",i);
                    for(int j = 0; j<resources_number; j++)
                    {
                        printf("%d ",available[j]);
                    }printf("\n");
                }
            }
        }
        // if(dead_flag==0)
        if(dead_flag==0 && count!=process_number)
        {
            printf("deadlock happened\n");
            exit(0);
        }
    }

    printf("safe sequence is: ");
    for(int i = 0; i<process_number; i++)
    {
        printf("%d ",safe[i]);
    }printf("\n");
}