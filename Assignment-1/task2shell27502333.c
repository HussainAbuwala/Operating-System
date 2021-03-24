/*

    Name: HUSSAIN SADIQ ABUWALA
    Student-ID: 27502333
    Start-Date: 11 August 2018
    Last-Modified: 26 August 2018

    High-level Functionalities:

    Functionalities available for the user

        I) Change directory
            
           a) All the directories are shown in color "green". To get into any directory, do a left click on the green option.
           b) You can also go to the parent directory by doing a left click on the blue option.

        II) Execute an executable file 

           a) All the executable files are shown in color "orange". To execute the file, do a left click on the orange option.

        III) Display the contents of text file

            --- All text file are shown in color "yellow". To open text file, do a left click on the yellow option.

        IV) Quit

            --- Do a left click on the quit option which has a red background color.
    


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

    char userInput[PATH_MAX];   //user input
    int i;


    while (userInput[0] != 'q' || userInput[1] != ':') {

        scanf("%s",userInput);  //get input from user
        char stripedInput[PATH_MAX] = "";
        
        //filter user input and store in stipedInput;

        for (i = 2; i < strlen(userInput); i++) {      
            stripedInput[i - 2] = userInput[i];          
        }
        
        //check for option d:
        if (userInput[0] == 'd' && userInput[1] == ':') {
            printf("<errBox.clear>\n");
            printf("<errBox.push {Status: <span {Ok} color = x00FF00 >} background-color = x000000 color = {white} margin = 10 width = 600>\n");
            char cwd[PATH_MAX];
            getcwd(cwd,sizeof(cwd));        //get current working directory
            strcat(cwd,"/");
            strcat(cwd,stripedInput);

            char *directory = cwd;
            int ret1 = chdir(directory);     //change the current working directory
                                            
            printError(ret1);

            int ret2 = setenv("PWD", cwd, 1); //update PWD environment variable
      		printError(ret2);
			if(ret1 == 0){listDirContents(".");}        //if everything ok, list contents of new directory
            
            
        }

        //check for option x:
        else if (userInput[0] == 'x' && userInput[1] == ':'){
            printf("<errBox.clear>\n");
            printf("<errBox.push {Status: <span {Ok} color = x00FF00 >} background-color = x000000 color = {white} margin = 10 width = 600>\n");
            struct stat st;             //get info about file
		    stat(stripedInput,&st);
            
            if (access(stripedInput,F_OK) != 0 || access(stripedInput,X_OK) != 0){  //check if file exists and is executable
                printf("<errBox.clear>\n");
                 printf("<errBox.push {Status: <span {Permission Denied} color = {red} >} background-color = x000000 color = {white} margin = 10 width = 600>\n");           
            }
            else{
                pid_t pid;
                pid = fork();       //create new child process
                if (pid == 0){
                    
                    char string1[PATH_MAX] = "xterm -e ./";
                    strcat(string1,stripedInput);
                    int r = system(string1);        //execute file using systems command.
    
               
                    return 0;
                    
                }              
           }
            
        }
        
        else if (userInput[0] == 'v' && userInput[1] == ':'){
             printf("<errBox.clear>\n");
            printf("<errBox.push {Status: <span {Ok} color = x00FF00 >} background-color = x000000 color = {white} margin = 10 width = 600>\n");
            struct stat st;
            char *params[] = {"/usr/bin/xterm","-e","less",stripedInput,0};
            stat(stripedInput,&st);         //get info about file
            long long size = st.st_size;
                
            if (size <= 500000) {       //check file size
                if (access(stripedInput,F_OK) != 0 || access(stripedInput,R_OK) != 0){
                     printf("<errBox.clear>\n");
                    printf("<errBox.push {Status: <span {Permission Denied} color = {red} >} background-color = x000000 color = {white} margin = 10 width = 600>\n");
                    printf("\n");
                }
                else{
                    pid_t pid;
                    pid = fork();       //create child process
                    if (pid == 0) {
                        int r = execv("/usr/bin/xterm",params);     //execute less command
                        printError(r);
                        return(0);
                    }
                }
            }
            else{
                printf("<errBox.clear>\n");
                printf("<errBox.push {Status: <span {File Size is too large} color = {red} >} background-color = x000000 color = {white} margin = 10 width = 600>\n");
                printf("\n");                
            }
        }
    
            
    }
    return 0;
}

void printError(int r){
	
	if (r < 0){     //print appropriate error message
		printf("<errBox.clear>\n");
        printf("<errBox.push {Status: <span {%s} color = {red} >} background-color = x000000 color = {white} margin = 10 width = 600>\n",strerror(errno));
		printf("\n");
	}
}


int listDirContents(char *directory){
    
    printf("<program.clear></n>\n");
    printf("<program.push text-align = {center} size = 20,{px}></n>\n");
    printf("<errBox:box {Status: <span {Ok} color = x00FF00 >} background-color = x000000 color = {white} margin = 10 width = 600>\n");
    printf("</n>");   
    printf("</n>"); 
    printf(
            "<box                  \n"
            "    color = xFFFFFF   \n"
            "    background-color = {red}\n"
            "    {Quit}\n"
            "   margin = 10\n"
            "   width = 200\n"
            "    visible = true    \n"
            "    onclick =         \n" 
            "        <putln {q:}>\n" 
            ">                     \n"
            "</n>                  \n"
          );
    printf("</n>\n");
    printf(
            "<box                  \n"
            "    color = xFFFFFF   \n"
            "    background-color = {blue}\n"
            "   margin = 10\n"
            "   width = 200\n"
            "    {Parent Directory}\n"
            "    visible = true    \n"
            "    onclick =         \n" 
            "        <putln {d:..}>\n" 
            ">                     \n"
            "</n>                  \n"
          );
    printf("</n>\n");


    struct dirent *de;  // Pointer for directory entry
    struct stat buf;

    // opendir() returns a pointer of DIR type. 
    DIR *dr = opendir(directory);
 
    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory" );
        return 0;
    }

    char cwd[PATH_MAX];
    getcwd(cwd,sizeof(cwd));            //get current directory
    printf("</#x1F4C2></ ></ ></ ><span {Current Directory: %s} underline = true></n>\n",cwd);
    printf("***Directory Listing Follows***</n>\n");
	printf("</n>\n");
 
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir()
    while ((de = readdir(dr)) != NULL)          //loop through all files in directory
            
            //skip hidden files
            if (de->d_name[0] != '.'){          //skip hidden files

                stat(de->d_name,&buf);

                if (S_ISREG(buf.st_mode)) {         //check if file is non-directory
                    /* Handle regular file */
                    if(access(de->d_name,X_OK)){        //check if file is non-executable
                        printf(
                           "</#x1F39E></t>        \n"
                           "<box                  \n"
                           "    color = {yellow}  \n"
                           "    width = 400  \n"
                           "    {File: %s}        \n"
                           "    visible = true    \n"
                           "    onclick =         \n" 
                           "        <putln {v:%s}>\n"
                           ">                     \n"
                           "</n>   </n>                \n"
                          ,de->d_name,de->d_name);
                    }
                    else{
                        printf(
                           "</#x1F3A0></t>            \n"
                           "<box                  \n"
                           "    color = {orange}  \n"
    "    width = 400  \n"
                           "    {Executable: %s}  \n"
                           "    visible = true    \n"
                           "    onclick =         \n" 
                           "        <putln {x:%s}>\n" 
                           ">                     \n"
                           "</n> </n>                 \n"
                          ,de->d_name,de->d_name);
                    }
                }
                
                else if (S_ISDIR(buf.st_mode)) {            //check if file is directory
                    /* Handle Directory */
                    printf(
                           "</#x1F4C1>  </t>          \n"
                           "<box                  \n"
                           "    color = {green}   \n"
    "    width = 400  \n"
                           "    {Directory: %s}   \n"
                           "    visible = true    \n"
                           "    onclick =         \n" 
                           "        <putln {d:%s}>\n" 
                           ">                     \n"
                           "</n>            </n>      \n"
                          ,de->d_name,de->d_name);
                }

            }
	printf("</n>\n");
    closedir(dr);           //close directory

}


