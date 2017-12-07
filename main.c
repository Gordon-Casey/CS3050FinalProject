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
void dijkstra(node *head, node *root, node *endPoint, int lengthOfList, int **adjacencyMatrix);
node* navigateList(node *head, node *root, node *endPoint, int lengthOfList, int **adjacencyMatrix);
void blackout(int **matrix, int nodeNum, int lengthOfList);
void printAdjacencyMatrix(int **x, int lengthOfList);
void resetList(node *head);
node *freeList(node *x);
void freeEverything(int **x, char **y, int **matrix, int maxLineLength, int rowHeight, int lengthOfList);
void moveRobots(char **x, node *moveMeFirst, node *moveMeSecond, int rowLength, int columnHeight, char *outputFile);
node *reverseList(node *me);
//void writeOutCharArray(char **x, FILE *fptr);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {           //   ./a.out room.txt out.txt

    int y = checkFiles(argv[1], argv[2]);
    if(y == 1){
        printf("\nInput file couldn't be opened. Exiting program.\n");
        exit(-1); //input file failed
    } 
    if(y == 2){
        printf("Output file couldn't be opened. Exiting program.\n");
        exit(-2);
    }
    
    
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
    free(sendHead);
    adjacencyMatrix = populateAdjacencyMatrix(adjacencyMatrix, head, roomIntArray, roomCharArray, lengthOfList, *maxLineLength, rowHeight);

    node *tempForS = head;
    while(tempForS->symbol != 'S') tempForS = tempForS->nextPtr;
    int sNodeNum = tempForS->nodeNum;

    node *tempForE = head;
    while(tempForE->symbol != 'E') tempForE = tempForE->nextPtr;
    int eNodeNum = tempForE->nodeNum;

    node *tempForF = head;
    while(tempForF->symbol != 'F') tempForF = tempForF->nextPtr;
    int fNodeNum = tempForF->nodeNum;

    node *tempForL = head;
    while(tempForL->symbol != 'L') tempForL = tempForL->nextPtr;
    int lNodeNum = tempForL->nodeNum;
    
    node* list1 = NULL;
    node* list2 = NULL;

    dijkstra(head, tempForS, tempForE, lengthOfList, adjacencyMatrix); 
    list1 = navigateList(head, tempForS, tempForE, lengthOfList, adjacencyMatrix);
    resetList(head);   
    
    printf("\n");
    dijkstra(head, tempForF, tempForL, lengthOfList, adjacencyMatrix);
    list2 = navigateList(head, tempForF, tempForL, lengthOfList, adjacencyMatrix);
    resetList(head);
        
    if(list1 == NULL){  
        printf("\nThere is not a path from S to E, regardless of where the second robot is. Exiting program...\n");
        exit(-5);
    }
    if(list2 == NULL){
        printf("\nThere is not a path from F to L, regardless of where the first robot is. Exiting program...\nb");
        exit(-6);
    }
    list1 = freeList(list1);
    list2 = freeList(list2);
    node *list3 = NULL;
    node *list4 = NULL;

    blackout(adjacencyMatrix, fNodeNum, lengthOfList); // See if S->E is blocked by F
    dijkstra(head, tempForS, tempForE, lengthOfList, adjacencyMatrix); 
    list1 = navigateList(head, tempForS, tempForE, lengthOfList, adjacencyMatrix);
    resetList(head);
    adjacencyMatrix = populateAdjacencyMatrix(adjacencyMatrix, head, roomIntArray, roomCharArray, lengthOfList, *maxLineLength, rowHeight);

    blackout(adjacencyMatrix, lNodeNum, lengthOfList);
    dijkstra(head, tempForS, tempForE, lengthOfList, adjacencyMatrix); // See if S->E is blocked by L
    list2 = navigateList(head, tempForS, tempForE, lengthOfList, adjacencyMatrix);
    resetList(head);
    adjacencyMatrix = populateAdjacencyMatrix(adjacencyMatrix, head, roomIntArray, roomCharArray, lengthOfList, *maxLineLength, rowHeight);

    blackout(adjacencyMatrix, sNodeNum, lengthOfList); // See if F->L is blocked by S
    dijkstra(head, tempForF, tempForL, lengthOfList, adjacencyMatrix); 
    list3 = navigateList(head, tempForF, tempForL, lengthOfList, adjacencyMatrix);
    resetList(head);
    adjacencyMatrix = populateAdjacencyMatrix(adjacencyMatrix, head, roomIntArray, roomCharArray, lengthOfList, *maxLineLength, rowHeight);

    blackout(adjacencyMatrix, eNodeNum, lengthOfList); // See if F->L is blocked by E
    dijkstra(head, tempForF, tempForL, lengthOfList, adjacencyMatrix); 
    list4 = navigateList(head, tempForF, tempForL, lengthOfList, adjacencyMatrix);
    resetList(head);
    adjacencyMatrix = populateAdjacencyMatrix(adjacencyMatrix, head, roomIntArray, roomCharArray, lengthOfList, *maxLineLength, rowHeight);

    if(list1 != NULL && list2 != NULL && list3 != NULL && list4 != NULL){
        moveRobots(roomCharArray, list1, list3, *maxLineLength, rowHeight, outputFile);
        printf("Nobody blocks anybody. Move R1 then R2.\n");
        exit(0);

    }
    else {
        if(list1 == NULL && list3 == NULL){
            printf("Both robots always block eachother. Exiting program...\n");
            exit(2);
        }

        if(list1 == NULL && list2 == NULL){
            printf("Robot 2 always blocks Robot 1. Exiting program...\n");
            exit(2);
        }
    
        if(list3 == NULL && list4 == NULL){
            printf("Robot 1 always blocks robot 2. Exiting program...\n");
            exit(2);
        }

        if(list2 == NULL && list4 == NULL){
            printf("Both robots always block eachother. Exiting program...\n");
            exit(2);
        }

        if(list1 == NULL && list2 != NULL && list3 != NULL && list4 != NULL){
            printf("R2 start blocks R1. Move R2 first.\n");
            moveRobots(roomCharArray, list3, list2, *maxLineLength, rowHeight, outputFile);
        //Move R2 first
        }

        if(list1 != NULL && list2 == NULL && list3 != NULL && list4 != NULL){
            printf("R2 end blocks R1. Move R1 first.\n");
            moveRobots(roomCharArray, list1, list3, *maxLineLength, rowHeight, outputFile);
        //Move R1 first
        }
    
        if(list1 != NULL && list2 != NULL && list3 == NULL && list4 != NULL){
            printf("R1 start blocks R2. Move R1 first.\n");
            moveRobots(roomCharArray, list1, list4, *maxLineLength, rowHeight, outputFile);
        //Move R1 first
        }
    
        if(list1 != NULL && list2 != NULL && list3 != NULL && list4 == NULL){
            printf("R1 end blocks R2. Move R2 first.\n");
            moveRobots(roomCharArray, list3, list1, *maxLineLength, rowHeight, outputFile);
        //Move R2 first
        }
    }
    freeEverything(roomIntArray, roomCharArray, adjacencyMatrix, *maxLineLength, rowHeight, lengthOfList);
    freeList(head);
    freeList(list1);
    freeList(list2);
    freeList(list3);
    freeList(list4);
    free(maxLineLength);
    return (0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
node *reverseList(node *me){
    node *temp = me;
    node *next = NULL;
    node *prev = NULL;
    while(temp != NULL){
        next = temp->nextPtr;
        temp->nextPtr = prev;
        prev = temp;
        temp = next;
    }
    return prev;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void moveRobots(char **x, node *moveMeFirst, node *moveMeSecond, int rowLength, int columnHeight, char *outputFile){
    node *temp = moveMeFirst;
    temp = reverseList(temp);
    char firstChar = temp->symbol;
    int u, v;
    FILE *fptr = fopen(outputFile, "w");
    for(u = 0; u < columnHeight; u++){
        for(v = 0; v < rowLength; v++){
            fprintf(fptr, "%c ", x[u][v]);
        }
        fprintf(fptr, "\n");
    }
    fprintf(fptr, "\n");
            
    while(temp->nextPtr != NULL){
        x[temp->row][temp->column] = ' ';
        temp = temp->nextPtr;
        x[temp->row][temp->column] = firstChar;
        for(u = 0; u < columnHeight; u++){
            for(v = 0; v < rowLength; v++){
                fprintf(fptr, "%c ", x[u][v]);
            }
            fprintf(fptr, "\n");
        }
        fprintf(fptr, "\n");
    }
    temp = moveMeSecond;
    temp = reverseList(temp);
    firstChar = temp->symbol;

    while(temp->nextPtr != NULL){
        x[temp->row][temp->column] = ' ';
        temp = temp->nextPtr;
        x[temp->row][temp->column] = firstChar;
            for(u = 0; u < columnHeight; u++){
        for(v = 0; v < rowLength; v++){
            fprintf(fptr, "%c ", x[u][v]);
        }
        fprintf(fptr, "\n");
    }
    fprintf(fptr, "\n");
    }
fclose(fptr);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void freeEverything(int **x, char **y, int **matrix, int maxLineLength, int rowHeight, int lengthOfList){
    int k;
    //for(k = 0; k < rowHeight; k++){
        //free( x[k] );
        //free( y[k] );
    //}
    //free(x);
    //free(y);

    for(k = 0; k < lengthOfList; k++){
        free( *(matrix + k) );
    }
    free(matrix);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
node *freeList(node *x){
    node *temp = x;
    node *prev;
    while(temp != NULL){
        prev = temp;
        temp = temp->nextPtr;
        free(prev);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void resetList(node *head){
    node *temp = head;
    while(temp->nextPtr != NULL){
        temp->distance = -1;
        temp->finished = 0;
        temp = temp->nextPtr;
    }
    temp->distance = -1;
    temp->finished = 0;
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
    printf("*** Int Array ***\n");
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
    if(fptr == NULL){
        printf("\nInput file couldn't be opened");
        exit(87);
    }
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
    if(fclose(fptr) == EOF){     //failed to close
        printf("\nFile failed to close");
        exit(34);
    }
    return (height - 1);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char **buildRoomCharArray(int maxLineLength, int rowHeight){
    char **x = malloc(sizeof(char*) * rowHeight);
    int k;
    for(k = 0; k < maxLineLength; k++){
        *(x + k) = malloc(sizeof(char) * maxLineLength);
    }
    int mm, vv; // This block is helper code. It initializes the char array to 'q' and int array to '5';
        for(mm = 0; mm < maxLineLength; mm++){
            for(vv = 0; vv < rowHeight; vv++){
                x[vv][mm] = ' '; } }
    return x;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int **buildRoomIntArray(int maxLineLength, int rowHeight){
    int **x = malloc(sizeof(int*) * rowHeight);
    int k;
    for(k = 0; k < maxLineLength; k++){
        *(x + k) = malloc(sizeof(int) * maxLineLength);
    }

    int mm, vv; // This block is helper code. It initializes the char array to 'q' and int array to '5';
    for(mm = 0; mm < maxLineLength; mm++){
        for(vv = 0; vv < rowHeight; vv++){
            x[vv][mm] = 9;} }
    return x;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int checkFiles(char *inputFile, char *outputFile){
    
    FILE* aptr = fopen(inputFile, "r");
    if(aptr == NULL) return 1;
    FILE *bptr = fopen(outputFile, "w");
    if(bptr == NULL) return 2;
    fclose(bptr);
    
    rewind(aptr);
    fseek(aptr, 0, SEEK_END);                       //check if file empty go to end see if the position changed
    if(ftell(aptr) == 0)
        return(1);
    
    if(fclose(aptr) == EOF){   //failed to close
        printf("\nFile failed to close");
        exit(34);
    }

    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int  **populateArraysAndMakeList(node **sendHead, char **roomCharArray, int **roomIntArray, int maxLineLength, int rowHeight, char *inputFile, int *ptrLengthOfList){
    node *localHead = NULL; // Push a node onto the list every time there is a hit
    int k = 0, j = 0;
    int flag = 0;
    FILE *fptr = fopen(inputFile, "r");
    if(fptr == NULL){
        printf("\nInput file couldn't be opened");
        exit(87);
    }

    char c;
    int exitFlag = 0;
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
                exitFlag++;
            }
            else if(c == 'E'){
                roomIntArray[j][k] = 3;
                localHead = newNode(localHead, j, k, c);
                k++;
                exitFlag++;
            }
            else if(c == 'F'){
                roomIntArray[j][k] = 4;
                localHead = newNode(localHead, j, k, c);
                k++;
                exitFlag++;
            }
            else if(c == 'L'){
                roomIntArray[j][k] = 5;
                localHead = newNode(localHead, j, k, c);
                k++;
                exitFlag++;
            }
            else{
                exit(30);
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
    if(fclose(fptr) == EOF){   //failed to close
        printf("\nFile failed to close");
        exit(34);
    }
    if(exitFlag != 4) {
        printf("The file did not contain two robots and two endpoints. Exiting program.\n");
        exit(-4);
    }

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
void dijkstra(node *head, node *root, node *endPoint, int lengthOfList, int **adjacencyMatrix){    
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
            break; // no more verts in the list
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
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
node* navigateList(node *head, node *root, node *endPoint, int lengthOfList, int **adjacencyMatrix){
    
    node* listTemp = NULL;
    node* listTemp2 = NULL;
    node* list = NULL;
    node *newTemp = NULL;
    int count;
    int min = 10000;
    node* holder = NULL;
    //printf("Root: %c, endPoint: %c.\n", root->symbol, endPoint->symbol);
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
        if(listTemp == newTemp) {
            return NULL;
        }
        newTemp = holder;
    }   
           
    return(list);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void blackout(int **matrix, int nodeNum, int lengthOfList){
    int uu;
    for(uu = 0; uu < lengthOfList; uu++){
        matrix[uu][nodeNum - 1] = 0;
        matrix[nodeNum - 1][uu] = 0;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void printAdjacencyMatrix(int **x, int lengthOfList){
    int uu, vv;
    printf("*** Adjacency Matrix ***\n");
    for(uu = 0; uu < lengthOfList; uu++){
        for(vv = 0; vv < lengthOfList; vv++){
            printf("%d ", x[uu][vv]);
        }
        printf("\n");
    }
    printf("*** End adjacency matrix ***\n");
}
