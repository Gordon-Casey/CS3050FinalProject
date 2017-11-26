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


int main(int argc, char** argv) {

    char* inputfile = (char*)malloc(sizeof(char)*strlen(argv[1]));     
    strcpy(inputfile, argv[1]);                                            
    
    FILE* aptr;
    
    if((aptr = fopen(inputfile, "r")) == NULL){
        exit(1);
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
    
    rewind(aptr);
    
    char roomArray[rowCounter][maxLine];
    int numArray[rowCounter][maxLine];
    memset(numArray, -1, sizeof(numArray));         //initializes array to -1
    int row = 0;
    int column = 0;
    int cat = 0;
    int counter = 1;
    
    node* head = NULL;
     
    while(!feof(aptr)){                 //parses through all the characters
        
        char c;
        c = fgetc(aptr);
        if(c == '\n'){
            column = 0;
            row++;
            cat = 0;
        }
        else if(c == ' ' && cat == 0){
            column++;
        }
        else if(c == '#'){
            numArray[row][column] = 0;
            column++;
            cat = 1;
        }
        else if(c == ' ' && cat == 1){
            head = newNode(head, row, column, c, counter);      //adds to  linked list
            numArray[row][column] = 1;                          //edits numArray to show its a reachable node
            counter++;
            column++;
        }
        else if(isupper(c)){
            head = newNode(head, row, column, c, counter);      //adds to  linked list                  
            numArray[row][column] = 1;                          //edits numArray to show its a reachable node
            counter++;
            column++;
        }
    }
    
    int matrixArray[counter][counter];
    memset(matrixArray, 0, sizeof(matrixArray));            //initializes array to 0
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
    
    fclose(aptr);
    
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