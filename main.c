//FinalProject
//jltmh3 14275992

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct Node{
    
    int row;
    int column;
    char symbol;            //symbol is used to mark start & stop points
    int nodeNum;
    struct Node* nextPtr;
    
}node;

node* newNode(node* head, int num1, int num2, char symbol, int value);


int main(int argc, char** argv) {           //room.txt output.txt

    char* inputfile = (char*)malloc(sizeof(char)*strlen(argv[1]));     
    strcpy(inputfile, argv[1]); 
    
    char* outputfile = (char*)malloc(sizeof(char)*strlen(argv[2]));
    strcpy(outputfile, argv[2]);
    
    FILE* aptr;
    FILE* bptr;
    
    if((aptr = fopen(inputfile, "r")) == NULL){
        exit(1);
    }
    
    if((bptr = fopen(outputfile, "w")) == NULL){
        exit(2);
    }
    rewind(aptr);
    
    char* inputLine;
    int maxLine = 0;
    int readVal = 0;
    int rowCounter = 0;
    
    while(!feof(aptr))
    {
        inputLine = (char*)malloc(sizeof(char)* 100);
        fgets(inputLine, 100, aptr);
        readVal = strlen(inputLine);
        if(readVal > maxLine)
            maxLine = readVal;
        free(inputLine);
        inputLine = NULL;
        rowCounter++;             //this creates one column extra if the final line of the room.txt is empty
                                    //is it safe to assume there will always be one blank line at the end or malloc the extra space?
        
    }
    
    printf("\n%d %d", maxLine, rowCounter);
    
    rewind(aptr);
    
    char **roomArray = malloc(sizeof(char*) * maxLine);
    int k = 0;
    while(k < maxLine){
        *(roomArray + k) = malloc(sizeof(char) * rowCounter);
        k++;
    }
    
    int **numArray = malloc(sizeof(int*) * maxLine);
    k = 0;
    while(k < maxLine){
        *(numArray + k) = malloc(sizeof(int) * rowCounter);
        k++;
    }
    
       
    
    
    
    k = 0;
    int j = 0;
    while(k < rowCounter){
        j = 0;
        while(j< maxLine){
           // printf(" %d", numArray[k][j]);
            numArray[k][j] = 0;
            j++;
        }
        printf("\n");
        k++;
    }
    
    
    int row = 0;
    int column = 0;
    int cat = 0;
    int counter = 1;
    
    node* head = NULL;
    char c;
    j = 0, k = 0;
    int flag = 0;
    while(!feof(aptr)){                 //parses through all the characters and load the data into the roomArray
        c = fgetc(aptr);
        if(c == '\n'){
            roomArray[j][k] = c;
            j++;
            flag = 0;
            k = 0;
        }
        else {
            roomArray[j][k] = c;
            if(c == '#'){
                numArray[j][k] = 0;
                flag = 1;
            }
            else if(c == ' ' && flag == 0){
                numArray[j][k] = -3;
            }
            else if(c == ' ' && flag == 1){
                numArray[j][k] = 1;
                head = newNode(head, j, k, c, counter);     //add nodes here
                counter++;
            }
            k++;
        }
    }
    
    printf("\n");
    
    j = 0;
    while(j < rowCounter){
        k = 0;
        while(k< maxLine){
            printf(" %d", numArray[j][k]);
            k++;
        }
        printf("\n");
        j++;
    }
    
    printf("\n"); 
    while(head != NULL){
        printf("%d %d\n", head->row, head->column);
        head = head->nextPtr;
    }
    
    node* temp4 = head;
    while(temp4->nextPtr != NULL)
        temp4 = temp4->nextPtr;
    
    printf("\n%d", temp4->nodeNum);
    
    int **matrixArray = malloc(sizeof(int*) * temp4->nodeNum);
    k = 0;
    while(k < temp4->nodeNum){
        *(matrixArray + k) = malloc(sizeof(int) * temp4->nodeNum);
        k++;
    }

   /* printf("\n");
    j = 0;
    while(j < rowCounter){
        k = 0;
        while(k< maxLine){
            printf(" %d", matrixArray[k][j]);
            k++;
        }
        printf("\n");
        j++;
    }*/
    
    int tempRow = 0;
    int tempColumn = 0;
    int count;
    int count2;
    node* temp = head;
    node* temp2;
 
    while(temp != NULL){                //creates adj matrix
        
        tempRow = (head->row + 1);
        tempColumn = (head->column - 1);
        
        for(count = 0; count < 3; count++){             //2 for loops look at all nodes adj to the node we're looking at
            for(count2 = 0; count2 < 3; count2++){
                
                temp2 = head;
                if(numArray[tempRow][tempColumn] == 1){     //checks if adj node is reachable
                    
                    while(temp2 != NULL){               //cycles through adj list to find node that corresponds to x,y coordinates
                        if(temp2->column == tempColumn && temp2->row == tempRow){       
                            matrixArray[temp->nodeNum][temp2->nodeNum] = 1;
                            matrixArray[temp2->nodeNum][temp->nodeNum] = 1;
                            break;
                        }
                        temp2 = temp2->nextPtr;
                    }
                }
                tempColumn++;
            }
            tempRow--;
        }
        temp = temp->nextPtr;
    }
    
    for(row = 0; row < (rowCounter); row++){
        fprintf(bptr, "\n");
        for(column = 0; column < (maxLine-1); column++){
            fprintf(bptr,"%d", matrixArray[row][column]);
        }
    }
    
    fclose(aptr);
    fclose(bptr);
    
    return (0);
}

node* newNode(node* head, int num1, int num2, char symbol, int value){ 
    
    node* temp = head;
    
    if(head == NULL){
        node* aNode = (node*)malloc(sizeof(node));
        aNode->row = num1;
        aNode->column = num2;
        aNode->symbol = symbol;
        aNode->nodeNum = value;
        aNode->nextPtr = NULL;
        head = aNode;
    }
    else{
        
        while(temp->nextPtr != NULL)
            temp = temp->nextPtr;
        
        node* aNode = (node*)malloc(sizeof(node));
        aNode->row = num1;
        aNode->column = num2;
        aNode->symbol = symbol;
        aNode->nodeNum = value;
        aNode->nextPtr = NULL;
        temp->nextPtr = aNode;
    }
    return(head);
}