There is only one c file, but we have written it into a make file. Make the program. Then, run
.\program inputFile.txt outputFile.txt
where inputFile.txt is the name of the file for the room, and outputFile.txt is the name of a blank text file
that the output can be written to. You enter these names, they are not hardcoded. 

The program will run, and if the room is valid, it will output the path of the robots in the outputFile.txt.
It does this by moving the robot on space and printing the graph after each step, until both robots are on their
endpoints. If the graph is not valid or if the robots do not havge paths to their endpoints, it will simply exit and not
proguce output in the output file. If there was a valid path, it will display in the stdout which order the robots moved in, R1 first, or R2 first.
