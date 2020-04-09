#include <stdio.h>

void usage(void) 
{
    printf("LatSeq tool to check corectness of LATSEQ_P into OAI projects\nComplete the header for LatSeq Log file\n");
}

int find_LATSEQ_P(const char * pathToOaiP)
{
    //produce a file with a list of file where LATSEQ_P has been found
    return 1;
}

int check_LATSEQ_P(const char * pathToSearchResultsP)
{
    //check if pathToSearchResultsP exists
    //check include of latseq.h is found in the source
    //check arguments
        //check if argument exists in considered function
        //check the number of argument
    //check the length of the string
    return 1;
}

int main(int argc, char **argv) 
{
    if (argc != 2) {
        print_usage();
        exit(0);
    }
    print_usage();
    return 0;
}