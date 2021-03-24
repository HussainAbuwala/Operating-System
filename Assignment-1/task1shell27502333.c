/*

    Name: HUSSAIN SADIQ ABUWALA
    Student-ID: 27502333
    Start-Date: 11 August 2018
    Last-Modified: 26 August 2018

    High-level Functionalities:

    Functionalities available for the user

        I) Change directory
            
            --- type "d:NameOfDirectory" inside terminal.

        II) Execute an executable file 

            --- type "x:NameOfExecutable" inside terminal

        III) Display the contents of text file

            --- type "v:NameOfTextFile" inside terminal

        IV) Quit

            --- type "q:" to quit the program.
    


 */


#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int listDirContents(char *directory);
void printError(int r);

int main(void)
{
    listDirContents(".");   //list all files in the current directory

    char userInput[PATH_MAX];   //get input from user
    int i;

    while (userInput[0] != 'q' || userInput[1] != ':') {   //run loop while user doesn't quites
        
        strcpy(userInput,"");
        scanf("%s",userInput);  //get input from user

        char stripedInput[PATH_MAX] = "";
        
        //filter user input and store in stipedInput;

        for (i = 2; i < strlen(userInput); i++) {      
            stripedInput[i - 2] = userInput[i];          
        }
        
        //check for option d:
        if (userInput[0] == 'd' && userInput[1] == ':') {

            char cwd[PATH_MAX];
            getcwd(cwd,sizeof(cwd));        //get current working directory
            strcat(cwd,"/");
            strcat(cwd,stripedInput);

            char *directory = cwd;
            int ret1 = chdir(directory);     //change the current working directory
                                            
            printError(ret1);               //print if any error

            int ret2 = setenv("PWD", cwd, 1); //update PWD environment variable
      		printError(ret2);
			if(ret1 == 0){listDirContents(".");}    //if everything ok, then print contents of new directory
            
            
        }

        //check for option x:
        else if (userInput[0] == 'x' && userInput[1] == ':'){

            struct stat st;
		    stat(stripedInput,&st);         //get info about file
            if (access(stripedInput,F_OK) != 0 || access(stripedInput,X_OK) != 0){      //check if file exists and is executable
                perror("Error: ");
		        printf("\n");            
            }
            else{
                pid_t pid;
                pid = fork();           //create a child process
                if (pid == 0){
                    
                    char string1[PATH_MAX] = "xterm -e ./";
                    strcat(string1,stripedInput);
                    int r = system(string1);            //execute file using systems command
                    return 0;
                    
                }              
           }

        }
        
        else if (userInput[0] == 'v' && userInput[1] == ':'){
            
            struct stat st;
            char *params[] = {"/usr/bin/xterm","-e","less",stripedInput,0};
            stat(stripedInput,&st);         //get info about file
            long long size = st.st_size;

            
                
            if (size <= 500000) {       //check if size less than 500kb
                if (access(stripedInput,F_OK) != 0 || access(stripedInput,R_OK) != 0){  //check if file exists and is readable
                    perror("Error: ");
		            printf("\n");
                }
                else{
                    pid_t pid;
                    pid = fork();       //create child process
                    if (pid == 0) {
                        int r = execv("/usr/bin/xterm",params);     //execute less command on file
                        printError(r);
                        return(0);
                    }
                }
            }
            else{
                printf("File size is too large\n");
                printf("\n");                
            }
        }
    
            
    }
    return 0;
}

void printError(int r){
	
	if (r < 0){             //print error message
		perror("Error: ");
		printf("\n");
	}
}


int listDirContents(char *directory){

    struct dirent *de;  // Pointer for directory entry
    struct stat buf;

    // opendir() returns a pointer of DIR type. 
    DIR *dr = opendir(directory);               //open directory
 
    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory" );
        return 0;
    }
    char cwd[PATH_MAX];
    getcwd(cwd,sizeof(cwd));            //get path of current directory
    printf("Current Directory: %s\n",cwd);
    printf("***Directory Listing Follows***\n");
	printf("\n");
 
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir()
    while ((de = readdir(dr)) != NULL)      //read through all files in directory
            
            //skip hidden files
            if (de->d_name[0] != '.'){      //skip hidden files

                stat(de->d_name,&buf);

                if (S_ISREG(buf.st_mode)) {     //check if file is non-directory
                    /* Handle regular file */
                    if(access(de->d_name,X_OK) != 0){       //check if file is a regular file
                        printf("File:	%s\n",de->d_name);
                    }
                    else{
                        printf("Executable:	%s\n",de->d_name);
                    }
                }
                
                else if (S_ISDIR(buf.st_mode)) {        //check if file is a directory
                    /* Handle Directory */
                    printf("Directory:	%s\n",de->d_name);
                }

            }
	printf("\n");
    closedir(dr);       //close directory

}


