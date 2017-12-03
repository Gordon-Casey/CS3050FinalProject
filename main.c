//FinalProject
//jltmh3 14275992

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct Node{
    
    int row;
    int column;
    int finished;
    int distance;
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
int **populateAdjacencyMatrix(int **adjacencyMatrix, node *head, int **roomIntArray, char **roomCharArray, int lengthOfList, int maxLineLength, int rowHeight);
int searchList(node *head, int x, int y);
node* dijkstra(node *head, node *root, node *endPoint, int lengthOfList, int **adjacencyMatrix);

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
    adjacencyMatrix = populateAdjacencyMatrix(adjacencyMatrix, head, roomIntArray, roomCharArray, lengthOfList, *maxLineLength, rowHeight);
    /*for(u = 0; u < lengthOfList; u++){
        for(v = 0; v < lengthOfList; v++){
            printf("%d ", adjacencyMatrix[u][v]);
        }
        printf("\n");
    }*/
    node* fuckList = NULL;
    //printf("\n******Int Array after function ***\n");
    //printIntArray(roomIntArray, *maxLineLength, rowHeight);
    //printCharArray(roomCharArray, *maxLineLength, rowHeight);
    //printList(head);
    node *temp = head;
    while(temp->symbol != 'S') temp = temp->nextPtr;
    node *root = temp;
    temp = head;
    while(temp->symbol != 'E') temp = temp->nextPtr;
    fuckList = dijkstra(head, root, temp, lengthOfList, adjacencyMatrix); 
   
    while(fuckList != NULL){
        printf("%d %d\n", fuckList->row, fuckList->column);
        fuckList = fuckList->nextPtr;
    }
    
    /*printList(head);
    int w = 0;
    int p = 0;
    for(p = 0; p < lengthOfList; p++){
        for(w = 0; w < lengthOfList; w++){
            printf("%d ", adjacencyMatrix[p][w]);
        }
        printf("\n");
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
        aNode->finished = 0;
        aNode->distance = -1;
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
        aNode->finished = 0;
        aNode->distance = -1;
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
    for(j = 0; j < columnHeight + 1; j++){
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
    printf("*** Int Array ***\n");
    //printf("\n\n *** Int Array ***\n");
    for(j = 0; j < columnHeight + 1; j++){
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
        *(x + k) = malloc(sizeof(char) * (rowHeight * 10000));
    }
    int mm, vv; // This block is helper code. It initializes the char array to 'q' and int array to '5';
        for(mm = 0; mm < rowHeight; mm++){
            for(vv = 0; vv < maxLineLength; vv++){
                x[mm][vv] = ' '; } }
    return x;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int **buildRoomIntArray(int maxLineLength, int rowHeight){
    int **x = malloc(sizeof(int*) * maxLineLength);
    int k;
    for(k = 0; k < maxLineLength; k++){
        *(x + k) = malloc(sizeof(int) * (rowHeight * 10000));
    }

    int mm, vv; // This block is helper code. It initializes the char array to 'q' and int array to '5';
    for(mm = 0; mm < rowHeight; mm++){
        for(vv = 0; vv < maxLineLength; vv++){
            x[mm][vv] = 9;} }
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
    int flag = 0;
    FILE *fptr = fopen(inputFile, "r");

    char c;

    while(!feof(fptr)){  //parses through all the characters and load the data into the roomArray
        c = fgetc(fptr);
        if(feof(fptr)) break; // Added bc of Brandon
        if(c == '\n') {
            k = 0;
            ++j;
            flag = 0;
        }
        else {
            roomCharArray[j][k] = c;
            if(c == '#'){
                roomIntArray[j][k] = 0;
                k++;
                flag = 1;
            }
            else if(c == ' ' && flag == 1){ // If the char is a space, make the int array position a 1
                roomIntArray[j][k] = 1;
                localHead = newNode(localHead, j, k, c);
                k++;
            }
            else if(c == ' ' && flag == 0){
                roomIntArray[j][k] = 0;
                k++;
            }
            else if(c == 'S'){ 
                roomIntArray[j][k] = 2;
                localHead = newNode(localHead, j, k, c);
                k++;
            }
            else if(c == 'E'){
                roomIntArray[j][k] = 3;
                localHead = newNode(localHead, j, k, c);
                k++;
            }
            else if(c == 'F'){
                roomIntArray[j][k] = 4;
                localHead = newNode(localHead, j, k, c);
                k++;
            }
            else if(c == 'L'){
                roomIntArray[j][k] = 5;
                localHead = newNode(localHead, j, k, c);
                k++;
            }
            else {
                roomIntArray[j][k] = 9;
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

    printf("Node key = %d. Position = %d,%d. Character = %c. Distance = %d.\n", temp->nodeNum, temp->row, temp->column, temp->symbol, temp->distance);
    while(temp->nextPtr != NULL){
        temp = temp->nextPtr;
        printf("Node key = %d. Position = %d,%d. Character = %c. Distance = %d.\n", temp->nodeNum, temp->row, temp->column, temp->symbol, temp->distance);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int **populateAdjacencyMatrix(int **adjacencyMatrix, node *head, int **roomIntArray, char **roomCharArray, int lengthOfList, int maxLineLength, int rowHeight){
    node *temp = head;
    int **tempAdjacencyMatrix = adjacencyMatrix;
    int workingX = 0;
    int workingY = 0;
    int searchResult = 0;
    int u, v;
    while(temp->nextPtr != NULL) {
        workingX = temp->row - 1;
        workingY = temp->column - 1;
        
        for(u = 0; u < 3; u++){
            for(v = 0; v < 3; v++){
                if(roomIntArray[workingX][workingY] != 0 && roomIntArray[workingX][workingY] != 9){
                    searchResult = searchList(head, workingX, workingY);
                    tempAdjacencyMatrix[temp->nodeNum - 1][searchResult - 1] = 1;
                    tempAdjacencyMatrix[searchResult - 1][temp->nodeNum - 1] = 1;
                }
                workingY++;
            }
            workingX++;
            workingY = temp->column - 1;
        }
        temp = temp->nextPtr;
    }
    workingX = temp->row - 1;
    workingY = temp->column - 1; 
    for(u = 0; u < 3; u++){
        for(v = 0; v < 3; v++){
            if(roomIntArray[workingX][workingY] != 0 && roomIntArray[workingX][workingY] != 9){
                searchResult = searchList(head, workingX, workingY);
                tempAdjacencyMatrix[temp->nodeNum - 1][searchResult - 1] = 1;
                tempAdjacencyMatrix[searchResult - 1][temp->nodeNum - 1] = 1;
            }
            workingY++;
        }
        workingX++;
        workingY = temp->column - 1;
    }
return tempAdjacencyMatrix;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int searchList(node *head, int x, int y) {
    node *temp = head;

    if(temp->row == x && temp->column == y) return temp->nodeNum;

    while(temp->nextPtr != NULL){
        temp = temp->nextPtr;
        if(temp->row == x && temp->column == y) return temp->nodeNum;
    }
    return -1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
node* dijkstra(node *head, node *root, node *endPoint, int lengthOfList, int **adjacencyMatrix){
    adjacencyMatrix[root->row][root->column] = 0;
    root->distance = 0;
    

    node *temp = head;
    node *listTemp = NULL;
    node *listTemp2 = NULL;
    node *holder = NULL;
    node *list = NULL;
    int count;
    int min = 0;
    node *tempPrime = NULL;
    int ctrA = 0, ctrB = 0, distance = 0;
    while(ctrA < lengthOfList){
        distance = 100000;
        temp = head;
        ctrB = 0;
        tempPrime = NULL;
        while(ctrB < lengthOfList){
            if(temp->finished == 0 && temp->distance != -1 && temp->distance < distance){
                distance = temp->distance;
                tempPrime = temp;
            }
            ctrB++;
        temp = temp->nextPtr;
        }

        if(tempPrime == NULL) {
            return(NULL);
            
             // no more verts in the list
        }
        else{
        tempPrime->finished = 1;
        temp = head;
        ctrB = 0;
        while(ctrB < lengthOfList){
            if(temp->distance == -1){
                if(adjacencyMatrix[tempPrime->nodeNum - 1][temp->nodeNum - 1] != 0) {
                temp->distance = tempPrime->distance + 1;
                }
            }
            else {
                if(temp->finished != 1 && adjacencyMatrix[tempPrime->nodeNum - 1][temp->nodeNum - 1] != 0 && (tempPrime->distance + 1) < temp->distance) {
                    temp->distance = tempPrime->distance + 1;
                }
            }
            ctrB++;
            temp = temp->nextPtr;
            }
        }
        ctrA++;
    }
    
    listTemp = head;
    listTemp2 = head;
    while(listTemp->symbol != endPoint->symbol){
        listTemp = listTemp->nextPtr;
    }
    list = newNode(list, listTemp->row, listTemp->column, listTemp->symbol);
    while(listTemp != root){
        min = 10000;
        for(count = 0; count < lengthOfList; count++){
                    
            if(adjacencyMatrix[count][listTemp->nodeNum-1] == 1){
                listTemp2 = head;
                while(listTemp2->nodeNum != (count+1)){
                    listTemp2 = listTemp2->nextPtr;
                }
                if(listTemp2->distance < min){
                    min = listTemp2->distance;
                    holder = listTemp2;
                }
                        
                    
            }
        }
        list = newNode(list, holder->row, holder->column, holder->symbol);
        listTemp = holder;
    }
           
    return(list);
}
