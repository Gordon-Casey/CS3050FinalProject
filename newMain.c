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

node* newNode(node* head, int num1, int num2, char symbol);
void printCharArray(char **x, int rowLength, int columnHeight); // Helper function.
void printIntArray(int **x, int rowLength, int columnHeight); // Helper function.
int getDimensions(int *maxLength, char *inputFile); // Gets depth and width of room.
char **buildRoomCharArray(int maxLineLength, int rowHeight); // Malloc's array of size of room.
int **buildRoomIntArray(int maxLineLength, int rowHeight); // Malloc's array of size of room.
int checkFiles(char *inputFile, char *outputFile);
int **populateArraysAndMakeList(node **sendHead, char **roomCharArray, int **roomIntArray, int maxLineLength, int rowHeight, char *inputFile, int *ptrLengthOfList);
int **buildAdjacencyMatrixArray(int lengthOfList);
void printList(node *head);
int **populateAdjacencyMatrix(int **adjacencyMatrix, node *head, int **roomIntArray, char **roomCharA

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {           //room.txt output.txt

    int y = checkFiles(argv[1], argv[2]);
    if(y == 1) exit(-1); //input file failed
    if(y == 2) exit(-2); //output file failed
    char *inputFile = argv[1];
    char *outputFile = argv[2];

    int *maxLineLength = malloc(sizeof(int));
    int rowHeight = getDimensions(maxLineLength, argv[1]);    
    
    char **roomCharArray = buildRoomCharArray(*maxLineLength, rowHeight);
    int **roomIntArray = buildRoomIntArray(*maxLineLength, rowHeight);

    node **sendHead = malloc(sizeof(node*));
    node *head = NULL;
    int *ptrLengthOfList = malloc(sizeof(int));

    int **adjacencyMatrix = populateArraysAndMakeList(sendHead, roomCharArray, roomIntArray, *maxLineLength, rowHeight, inputFile, ptrLengthOfList);
    int lengthOfList = *ptrLengthOfList;
    free(ptrLengthOfList);
    head = *sendHead;
    int u, v;
    for(u = 0; u < lengthOfList; u++){
        for(v = 0; v < lengthOfList; v++){
            printf("%d ", adjacencyMatrix[u][v]);
        }
        printf("\n");
    }

    //printf("\n******Int Array after function ***\n");
    //printIntArray(roomIntArray, *maxLineLength, rowHeight);
    //printCharArray(roomCharArray, *maxLineLength, rowHeight);
    //printList(head);





    /*
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
*/    
/*    for(row = 0; row < (rowCounter); row++){
        fprintf(bptr, "\n");
        for(column = 0; column < (maxLineLength-1); column++){
            fprintf(bptr,"%d", matrixArray[row][column]);
        }
    }*/
    
    
    return (0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
node* newNode(node* head, int num1, int num2, char symbol){ 
    static int counter = 1;

    if(head == NULL){
        node* aNode = (node*)malloc(sizeof(node));
        aNode->row = num1;
        aNode->column = num2;
        aNode->symbol = symbol;
        aNode->nodeNum = counter;
        aNode->nextPtr = NULL;
        head = aNode;
        //printf("Node key = %d. Position = %d,%d. Character = %c.\n", aNode->nodeNum, aNode->row, aNode->column, aNode->symbol);
    }
    else{
        node *temp = head;
        while(temp->nextPtr != NULL)
            temp = temp->nextPtr;
        
        node* aNode = (node*)malloc(sizeof(node));
        aNode->row = num1;
        aNode->column = num2;
        aNode->symbol = symbol;
        aNode->nodeNum = counter;
        aNode->nextPtr = NULL;
        temp->nextPtr = aNode;
        //printf("Node key = %d. Position = %d,%d. Character = %c.\n", aNode->nodeNum, aNode->row, aNode->column, aNode->symbol);
    }
    counter++;
    //printf("Node key = %d. Position = %d,%d. Character = %c.\n", aNode->nodeNum, aNode->row, aNode->column, aNode->symbol);
    return(head);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void printCharArray(char **x, int rowLength, int columnHeight) {
    int j ,k;
    printf("\n\n *** Char Array of the Graph***\n");
    for(j = 0; j < columnHeight; j++){
        for(k = 0; k < rowLength; k++){
            printf("%c ", x[j][k]);
        }
        printf("\n");
    }
    printf("*** End Char Array *** \n\n");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void printIntArray(int **x, int rowLength, int columnHeight) {
    int j ,k;
    //printf("\n\n *** Int Array ***\n");
    for(j = 0; j < columnHeight; j++){
        for(k = 0; k < rowLength; k++){
            printf("%d ", x[j][k]);
        }
        printf("\n");
    }
    printf("*** End Int Array *** \n\n");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int getDimensions(int *maxLength, char *inputFile){
    FILE *fptr = fopen(inputFile, "r"); // need do some error checking here
    *maxLength = 0;
    int height = 0;
    char *buffer = (char*)malloc(sizeof(char)* 1000);

    int readVal = 0;
    while(!feof(fptr)) {
        fgets(buffer, 1000, fptr);
        readVal = strlen(buffer);
        if(readVal > *maxLength)
            *maxLength = readVal;
        height++; 
    }
    *maxLength -= 1;
    free(buffer);
    return (height - 1);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char **buildRoomCharArray(int maxLineLength, int rowHeight){
    char **x = malloc(sizeof(char*) * maxLineLength);
    int k;
    for(k = 0; k < maxLineLength; k++){
        *(x + k) = malloc(sizeof(char) * rowHeight);
    }
    return x;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int **buildRoomIntArray(int maxLineLength, int rowHeight){
    int **x = malloc(sizeof(int*) * maxLineLength);
    int k;
    for(k = 0; k < maxLineLength; k++){
        *(x + k) = malloc(sizeof(int) * (rowHeight * 100));
    }

    int mm, vv; // This block is helper code. It initializes the char array to 'q' and int array to '5';
    for(mm = 0; mm < rowHeight; mm++){
        for(vv = 0; vv < maxLineLength; vv++){
            x[mm][vv] = 5;
            x[mm][vv] = 'q'; } }
    return x;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int checkFiles(char *inputFile, char *outputFile){
    
    FILE* aptr = fopen(inputFile, "r");
    if(aptr == NULL) return 1;

    FILE* bptr = fopen(outputFile, "w");
    if(bptr == NULL) return 2;
    
    fclose(aptr);
    
    fclose(bptr);

    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int  **populateArraysAndMakeList(node **sendHead, char **roomCharArray, int **roomIntArray, int maxLineLength, int rowHeight, char *inputFile, int *ptrLengthOfList){
    node *localHead = NULL; // Push a node onto the list every time there is a hit
    int k = 0, j = 0;
    
    FILE *fptr = fopen(inputFile, "r");

    char c;

    while(!feof(fptr)){  //parses through all the characters and load the data into the roomArray
        c = fgetc(fptr);
        if(feof(fptr)) break; // Added bc of Brandon
        //printf("%c\n", c); // just a helper print to print each character, I only use it in GDB
        if(c == '\n') {
            k = 0;
            ++j;
        }
        else {
            roomCharArray[j][k] = c;

            if(c == ' '){ // If the char is a space, make the int array position a 1
                roomIntArray[j][k] = 1;
                localHead = newNode(localHead, j, k, c);
                k++;
            }

            else if(c == '#'){ // If the char is a #, make the int array position an 8
                roomIntArray[j][k] = 0;
                k++;
            }
        } 
    }
    
    node *temp2 = localHead;
    while(temp2->nextPtr != NULL) temp2 = temp2->nextPtr;
    *ptrLengthOfList = temp2->nodeNum;
    int u, v;
    int **x = malloc(sizeof(int*) * temp2->nodeNum);
    for(u = 0; u < temp2->nodeNum; u++){
        *(x + u) = malloc(sizeof(int) * temp2->nodeNum);
    }

    *sendHead = localHead;   
    fclose(fptr);
    
    return x;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int **buildAdjacencyMatrixArray(int lengthOfList) {
    int **x = malloc(sizeof(int*) * lengthOfList);
    int k;
    for(k = 0; k < lengthOfList; k++){
        *(x + k) = malloc(sizeof(int) * lengthOfList);
    }
    return x;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void printList(node *head){
    node *temp = head;

    printf("Node key = %d. Position = %d,%d. Character = %c.\n", temp->nodeNum, temp->row, temp->column, temp->symbol);
    while(temp->nextPtr != NULL){
        temp = temp->nextPtr;
        printf("Node key = %d. Position = %d,%d. Character = %c.\n", temp->nodeNum, temp->row, temp->column, temp->symbol);
    }
}

