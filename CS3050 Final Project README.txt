1) read the file in and find the length of the longest row and the depth of the deepest column.
2) rewind the file, and malloc a char **array with those dimensions. Also malloc and int **array with those dimensions. 
3) Re-read the file, char by char, and enter the data exactly into the char**, and also, whenever the space is a space that a robot can traverse to, we will number that node. We will store that number in the integer array, at the same time, and we will create a linked list with that node, and in the node we will story the (x,y).
4)Malloc a int **array for the adjecency matrix, for the size of the list. 
5) Traverse through the int **array again, and for each numbered node, check to see what its neighbors are. If its neighbor is a node, that means that there is a link, and so we go to the matrix and enter that adjacency (at x,y and y,x).
6) profit.