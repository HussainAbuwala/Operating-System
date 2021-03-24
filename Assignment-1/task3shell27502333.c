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

            a) All text file are shown in color "yellow". To open text file, do a left click on the yellow option.

        IV) Copy and paste

            a) Executable files (orange color) and text files (yellow color) can be copy and pasted to whichever
               directory the user wants.

            b) Right click on any executable or text file. There will be a copy option which will appear on the top of the page.

            c) Do a left click on the copy option.

            d) After the copy option is selected, an paste option will appear. 

            e) Go to any directory and do a left click on the paste option and the file will appear.
        
            f) If the copied file already exists, the new file will have the name "copy of [oldfileName]".

        VI) Cut and paste

            a) Executable files (orange color) and text files (yellow color) can be cut and pasted to whichever
               directory the user wants.

            b) Right click on any executable or text file. There will be a cut option which will appear on the top of the page.

            c) Do a left click on the cut option.

            d) After the cut option is selected, an paste option will appear. 

            e) Go to any directory and do a left click on the paste option and the file will appear.
        
            f) The directory from which the file was cut will no longer have that file after the paste operation.

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

int listDirContents(char *directory,char *check,int* val);
void printError(int r);
int exists(const char *fname);
 
int main(void)
{
    
    
    int counter = 0;
    char userInput[PATH_MAX];   //user input
    char check[PATH_MAX];
    char cpy_cwd[PATH_MAX];
    char copy_file[PATH_MAX];
    int i;
    int val = 0;

    listDirContents(".",check,&val);   //list all files in the current directory
    


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
                                            //respond correctly to ".."
            printError(ret1);               

            int ret2 = setenv("PWD", cwd, 1); //update PWD environment variable
      		printError(ret2);
			if(ret1 == 0){listDirContents(".",check,&val);}     //if everything ok,print contents of new directory.

            
        }

        //check for option x:
        else if (userInput[0] == 'x' && userInput[1] == ':'){
            printf("<copy.clear>\n");
            printf("<errBox.clear>\n");
            printf("<errBox.push {Status: <span {Ok} color = x00FF00 >} background-color = x000000 color = {white} margin = 10 width = 600>\n");
            printf("<cut.clear>\n");
 
            struct stat st;
		    stat(stripedInput,&st);         //get info about file
            
            if (access(stripedInput,F_OK) != 0 || access(stripedInput,X_OK) != 0){      //check if file exists and is executable
printf("<errBox.clear>\n");
                 printf("<errBox.push {Status: <span {Permission Denied} color = {red} >} background-color = x000000 color = {white} margin = 10 width = 600>\n");
            }
            else{
                pid_t pid;
                pid = fork();           //create new child process
                if (pid == 0){
                    
                    char string1[PATH_MAX] = "xterm -e ./";
                    strcat(string1,stripedInput);
                    int r = system(string1);        //execute file using systems command
         
                    return 0;
                    
                }              
           }

        }
        
        else if (userInput[0] == 'v' && userInput[1] == ':'){
            printf("<copy.clear>\n");
            printf("<cut.clear>\n");
 printf("<errBox.clear>\n");
            printf("<errBox.push {Status: <span {Ok} color = x00FF00 >} background-color = x000000 color = {white} margin = 10 width = 600>\n");
            printf("<copy.push visible=false>\n");
            printf("<cut.push visible=false>\n");

            struct stat st;
            char *params[] = {"/usr/bin/xterm","-e","less",stripedInput,0};
            stat(stripedInput,&st);         //get file info
            long long size = st.st_size;
                
            if (size <= 500000) {       //check file size
                if (access(stripedInput,F_OK) != 0 || access(stripedInput,R_OK) != 0){      //check if file exists and is readable
 printf("<errBox.clear>\n");
                    printf("<errBox.push {Status: <span {Permission Denied} color = {red} >} background-color = x000000 color = {white} margin = 10 width = 600>\n");
                    printf("\n");
                }
                else{
                    pid_t pid;
                    pid = fork();       //create child process
                    if (pid == 0) {
                        int r = execv("/usr/bin/xterm",params);         //execute less command on file
                        printError(r);
                        return(0);
                    }
                }
            }
            else{
                //printf("File size is too large\n");
                 printf("<errBox.clear>\n");
                printf("<errBox.push {Status: <span {File Size is too large} color = {red} >} background-color = x000000 color = {white} margin = 10 width = 600>\n");
                printf("\n");                
            }
        }


        else if (userInput[0] == 'a'){
            printf("<copy.clear>\n");
            printf("<cut.clear>\n");
            printf("<copy.push {copy: %s} visible =true onclick = <putln {c:%s}>>\n",stripedInput,stripedInput);    //insert copy button
            printf("<cut.push {cut: %s} visible=true onclick = <putln {m:%s}>>\n",stripedInput,stripedInput);       //insert cut button
        }
        
        else if (userInput[0] == 'c'){
            printf("<copy.clear>\n");
            printf("<cut.clear>\n");
            printf("<paste.clear>\n");

            printf("<copy.push visible=false>\n");
            printf("<cut.push visible=false>\n");

            val = 1;
            getcwd(cpy_cwd,sizeof(cpy_cwd));        //get current working directory path where file to be copied exists
            strcat(cpy_cwd,"/");
            strcpy(copy_file,stripedInput);
            strcat(cpy_cwd,stripedInput);
            printf("<paste.push {paste: %s} visible=true onclick = <putln {p:%s}>>\n",stripedInput,stripedInput);   //introduce push button
            strcpy(check,stripedInput);
            
        }

        else if (userInput[0] == 'm'){

            printf("<copy.clear>\n");
            printf("<cut.clear>\n");
            printf("<paste.clear>\n");

            printf("<copy.push visible=false>\n");
            printf("<cut.push visible=false>\n");

            val = 2;
            getcwd(cpy_cwd,sizeof(cpy_cwd));        //get current working directory path where file to be moved exists
            strcat(cpy_cwd,"/");
            strcat(cpy_cwd,stripedInput);

            
            printf("<paste.push {paste: %s} visible=true onclick = <putln {t:%s}>>\n",stripedInput,stripedInput);   //introduce paste button
            strcpy(check,stripedInput);
        
        }
        
        else if(userInput[0] == 'p'){

            char string3[PATH_MAX] = "cp ";
            char newName[PATH_MAX];
    
            if(exists(copy_file) == 1){     //if file to be pasted already exists
                
                strcpy(newName," copy_of_");  //prepend "copy of"
                strcat(newName,copy_file);  //"copy of fileName"
                strcat(string3,copy_file);
                strcat(string3,newName);
                system(string3);                    //copy the file
                listDirContents(".",check,&val);    //list new directory contents
            }
            else{
                strcat(string3,cpy_cwd);
                strcat(string3," .");
                system(string3);                    //copy file
                listDirContents(".",check,&val);    //list new directory contents
            }
            
            
            
        }


        else if(userInput[0] == 't'){

            char string3[PATH_MAX] = "mv ";         
            strcat(string3,cpy_cwd);
            strcat(string3," .");
            system(string3);                    //move file from one directory to another.
            val = 0;
            listDirContents(".",check,&val);    //list contents of new directory.
            
        }
    
            
    }
    return 0;
}

void printError(int r){
	
	if (r < 0){     //print error
		printf("<errBox.clear>\n");
        printf("<errBox.push {Status: <span {%s} color = {red} >} background-color = x000000 color = {white} margin = 10 width = 600>\n",strerror(errno));
		printf("\n");
	}
}


int listDirContents(char *directory,char *check,int *val){
    
    printf("<program.clear></n>\n");
    
    printf("<copy:box visible = false>\n");
    printf("<cut:box visible = false>\n");
    printf("<paste:box visible = false>\n");
    printf("</n>"); 
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
    getcwd(cwd,sizeof(cwd));            //get path of current working directory
    printf("</#x1F4C2></ ></ ></ ><span {Current Directory: %s} underline = true></n>\n",cwd);
    printf("***Directory Listing Follows***</n>\n");
	printf("</n>\n");
 
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir()
    while ((de = readdir(dr)) != NULL)          //loop through all files in directory
            
            //skip hidden files
            if (de->d_name[0] != '.'){          //skip hidden files

                stat(de->d_name,&buf);

                if (S_ISREG(buf.st_mode)) {     //check if file is non-directory
                    /* Handle regular file */
                    if(access(de->d_name,X_OK)){        //check if file is not executable
                        printf(
                           "</#x1F39E></t>        \n"
                           "<box                  \n"
                           "    color = {yellow}  \n"
                           "    width = 400  \n"
                           "    {File: %s}        \n"
                           "    visible = true    \n"
                           "    onclick =         \n" 
                           "        <putln {v:%s}>\n"
                           "    oncontextclick =  \n"
                           "        <putln {a:%s}>\n" 
                           ">                     \n"
                           "</n>   </n>                \n"
                          ,de->d_name,de->d_name,de->d_name);
                    }
                    else{
                        printf(
                           "</#x1F3A0></t>           \n"
                           "<box                  \n"
                           "    color = {orange}  \n"
                           "    width = 400  \n"
                           "    {Executable: %s}  \n"
                           "    visible = true    \n"
                           "    onclick =         \n" 
                           "        <putln {x:%s}>\n" 
                           "    oncontextclick =  \n"
                           "        <putln {a:%s}>\n" 
                           ">                     \n"
                           "</n>  </n>                 \n"
                          ,de->d_name,de->d_name,de->d_name);
                    }
                }
                
                else if (S_ISDIR(buf.st_mode)) {            //check if file is a directory
                    /* Handle Directory */
                    printf(
                           "</#x1F4C1></t>            \n"
                           "<box                  \n"
                           "    color = {green}   \n"
                           "    width = 400  \n"
                           "    {Directory: %s}   \n"
                           "    visible = true    \n"
                           "    onclick =         \n" 
                           "        <putln {d:%s}>\n" 
                           ">                     \n"
                           "</n> </n>                  \n"
                          ,de->d_name,de->d_name);
                }

            }
	printf("</n>\n");
    if(*val == 1){
        printf("<paste.push {paste: %s} visible=true onclick = <putln {p:%s}>>\n",check,check);
    }
    else if(*val == 2){
        printf("<paste.push {paste: %s} visible=true onclick = <putln {t:%s}>>\n",check,check);
    }
    closedir(dr);           //close directory

}

//return 1 if file exists or else return 1
int exists(const char *fname){

    FILE *file;
    if((file = fopen(fname,"r"))){
        fclose(file);
        return 1;    
    }

    return 0;
}

