#include <stdio.h>
#include <string.h>
#include <stdbool.h>  
#include <unistd.h> 
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>

// Assume no input line will be longer than 1024 bytes
#define MAX_INPUT 1024
char cwd[MAX_INPUT];
char oldpwd[MAX_INPUT];

char** argInputs(char * input);
int getIndexRedir(char** args);

int main (int argc, char ** argv, char **envp) {

  int finished = 0;
  char *prompt = "320sh> ";
  char cmd[MAX_INPUT];
  int fd, commandCounter;
  int strCounter = 0; //used for right arrow checking
  char c[1];
  char commands[50][1024];


while(read(fd,c,1)){


      if (c[0] == '\n'){
        i = 0;
      }
    }

  //if file exists already, this will fail.
  //if this doesn't fail, we must create the file, and make a counter, and write it.
  commandCounter = 0;
  int moveCount = 0;
  fd = open("320sh_history.txt", O_RDWR | O_CREAT | O_EXCL, 0666);

  //file already exists, extract counter of commands from file by counting newlines.
  if (fd == -1){
    printf("file exists\n");
    fd = open("320sh_history.txt", O_RDWR | O_CREAT, 0666);


    //c is just gonna be a character, and we read the history file one character at a time,
    // and everytime we find a newline, we increment the commandcounter. commandcounter starts at 0
    //this is also good because it brings the file handle to the end of the file, where we can now continue 
    // writing to the file (as opposed to writing to the top and overwriting stuff)
    int i = 0;

    while(read(fd,c,1)){


      if (c[0] == '\n'){
        i = 0;
      }
    }

    //TEST TO SEE IF ARRAY OF STRING COMMANDS IS PROPERLY READ INTO
    // int j = 0;
    // for (j = 0; j < 50; j++){
      
    //   if (strlen(commands[j]) == 0){
    //     break;
    //   }
    //   printf("Command #%d\n",j+1);
    //   write(1,commands[j],strlen(commands[j]));
    //   printf("\n");
    // }

    //ENDTEST
    //printf("Found a total of %d commands.\n",commandCounter);
  }


  //main shell loop
  while (!finished) {
    char *cursor;
    char last_char;
    int rv;
    int count;
    bool valid = false;
    

    // Print the prompt
    rv = write(1, prompt, strlen(prompt));
    if (!rv) { 
      finished = 1;
      break;
    }
    
    // read and parse the input
    //command loop
    int commandStringIndex = 0;
    char currentCommand[1024];
    for(commandStringIndex = 0,rv = 1, count = 0, cursor = cmd, last_char = 1; 
        rv && (++count < (MAX_INPUT-1))&& (last_char != '\n');
        cursor++) 
    { 

        rv = read(0, cursor, 1);
      // cursor points to the character



        //instead of writing to the file character by character, write each command to this string. if the command is an actual command
        //rather than an up/down arrow, then we write to the file. otherwise don't write.
        //char currentCommand[1024];

        //make sure that the last character is not END OF TEXT (3)
        last_char = *cursor;
        if(last_char == 3) {
          write(1, "^c", 2);
        } else {

          
          //write to the history file one character at a time
          //this is if the character is NOT an up/down/left/right arrow/backspace
          if (!(((*cursor == 65 || *cursor == 66 || *cursor == 67 || *cursor == 68) && *(cursor-1) == 91 && *(cursor-2) == 27) || 
              (*(cursor) == 91 && *(cursor-1) == 27) ||
              (*(cursor) == 27)) && *(cursor) != 127){

            if(*cursor == 10 && strCounter > 0){
              write(fd, cmd, strCounter);
              write(fd, "\n", 1);
            }
            if(!(strCounter == 0 && *cursor == 10)){ // check for empty commands
            
            if(!(strCounter == 0 && *cursor == 10))
              currentCommand[commandStringIndex++] = *cursor;

            }
            write(1,cursor,1);

            strCounter++;
            //printf("\n %d  %d\n", count, strCounter);
            if(count < strCounter){
              char * cur = malloc(1);
              char * nex = malloc(1);
              cur[0]= cmd[count-1];
              nex[0]= cmd[count];

              
              //printf("\n%d  %d\n", cur[0], nex[0]);


              for(int i = count; i < strCounter; i++){

                  write(1, cur, 1);

                  cmd[i] = cur[0];
                  cur[0] = nex[0];
                  nex[0] = cmd[i+1];
                  }
                //cmd[strCounter] = 0;
                for(int i = count; i < strCounter; i++){
                  write(1, "\b", 1);
                }


            }

            //printf("\n%s  %d\n", cursor, count);
            
            cmd[count-1] = *cursor;
                      //printf("\nCMD %s %d  %d  l\n", cmd, count,  strCounter);

          }



          if (*cursor == 10){
            //printf("newline\n");
            strCounter = 0;
          }
          //check if character is back space
          if (*cursor == 127){
              //printf("%s\n", cmd);
            count--;
            if(count > 0){
              strCounter--;
              count--;
              //printf("  %d   %d\n", strCounter, count);
              if(count < strCounter){
                //printf("\n%c\n", cmd[count]);
                write(1, "\b", 1);

                for(int i = count+1; i <= strCounter; i++){
                  char * c = malloc(1);

                  c[0]= cmd[i];
                  cmd[i-1] = cmd[i];
                  write(1, c, 1);
                }
                //cmd[strCounter] = 0;
                write(1, " ", 1);
                for(int i = count; i <= strCounter; i++){
                  write(1, "\b", 1);
                }
                //move characters back when cursor in middle
              } else{
                cmd[strCounter] = 0;
                write(1, "\b \b", 3);
              }
                
            }
                                  //printf("\nCMD %s %d  %d  l\n", cmd, count,  strCounter);

          //printf("curs %s\n", cursor-count+1);
          }


          //escape sequence for UPARROW ([[A) is 27 91 65

          if (*cursor == 65 && *(cursor-1) == 91 && *(cursor-2) == 27){
              count -= 2;
                              //this write to standard output the most recent command, and decrements the commandCounter
                //this will need to be cleaned up, because right now commandCounter is the only variable that tells us
                // how many commands we have, and we are decrementing it, so we lose that information,
                // so we will need another variable later...
                // we also dont check if we go below 0 or over 50
                //so do that later

                //write(1, prompt, strlen(prompt));
            if(commandCounter > 0){
              for(int i = 0; i < count-1; i++){// clears the line
                write(1, "\b \b", 3);
              }

                write(1,commands[commandCounter-1],strlen(commands[commandCounter-1])-1); // writes to std out

                count = strlen(commands[commandCounter-1])-1;   //updates how many char in the stdout
                strCounter = count;
                strcpy(cmd, commands[commandCounter-1]); //updates the command itself

                commandCounter--;   //keep track of up 
                moveCount++;        
            } else{
              count--;        
            }
          }

          //escape sequence for DOWNARROW ([[B) is 27 91 66
          else if (*cursor == 66 && *(cursor-1) == 91 && *(cursor-2) == 27){
              //write(1, prompt, strlen(prompt));
            count -= 2;
            if(commandCounter <  sizeof(commands) / sizeof(commands[0])-1){
              for(int i = 0; i < count-1; i++){
                write(1, "\b \b", 3);
              }
                write(1,commands[commandCounter],strlen(commands[commandCounter])-1);

                strcpy(cmd, commands[commandCounter]);
                //printf("commandCounter: %d\n", commandCounter);
                commandCounter++;
                moveCount++;
                count = strlen(commands[commandCounter-1])-1;
                strCounter = count;

              } else{
                count--;
              }
          } // end of down if
           else if (*cursor == 68 && *(cursor-1) == 91  && *(cursor-2) == 27){
              
              //printf("\ncmdLength: %zu\n", cmdLength - 3);
              //pri
              if(count > 3){                  
                write(1, cursor -2, 1);
                write(1, cursor -1, 1);
                write(1, cursor, 1);
              } else{
                count++;
              }
              count -= 4;

            }
    
        
          else if (*cursor == 67 && *(cursor-1) == 91 && *(cursor-2) == 27){
            count--;
//            printf("   %d    %d\n", strCounter, count);

            if(strCounter+2 > count) {
              count++;
            write(1, cursor -2, 1);
            write(1, cursor -1, 1);
            write(1, cursor, 1);
          }
            count -= 2;
          }




        }
    } 

    //printf("Current Command: %s\n",currentCommand);
    if(!(strCounter == 0 && *cursor == 10)){ // check for empty commands and write into tracker
      strcpy(commands[commandCounter++],currentCommand);
    }

    
  /*
 // TEST TO SEE IF ARRAY OF STRING COMMANDS IS PROPERLY WRITTEN INTO
    int j = 0;
    for (j = 0; j < 50; j++){
      
      if (strlen(commands[j]) == 0){
        break;
      }
      printf("Command #%d\n",j+1);
      write(1,commands[j],strlen(commands[j]));
      printf("\n");
    }


    // ENDTEST

    */


    // end of for loop

    //turn this into a function?
    *(cursor-1) = '\0'; // removes \n at the end
    *cursor = '\0';

    //printf("\n1   %s   1\n", cmd);

    if(strcmp(cmd, "") != 0){
    int z = 0;
    while(cmd[z] != '\b' || cmd[z] != '\0' || cmd[z] != 10){
      //printf("sdfsdf\n");
        if(cmd[z] == 10){
          cmd[z] = '\0';
          break;
        }
        z++;
    }
    //printf("\n%d\n",  cmd[2] );

    char ** argVar = argInputs(cmd);

    //if(getIndexRedir(argVar))
      //printf("has <\n");
    //printf("%s ,  %s,\n", argVar[0], argVar[1]);


    //char argVar[0][MAX_INPUT];
    
    bool implemented = false;
    if(strcmp(argVar[0], "cd") == 0 || 
      strcmp(argVar[0], "echo") == 0 || 
      strcmp(argVar[0], "pwd") == 0 || 
      strcmp(argVar[0], "set") == 0|| 
      strcmp(argVar[0], "help") == 0|| 
      strcmp(argVar[0], "exit") == 0) {
        implemented = true;
    } else
    implemented = false;

if(implemented){
    if(strcmp(argVar[0], "cd") == 0){
    
      if(strcmp(argVar[1], "-") == 0){
         
          char temp[MAX_INPUT];
          strcpy(temp, getcwd(cwd, sizeof(cwd)));
          chdir(oldpwd);
          memcpy(oldpwd, temp, MAX_INPUT);
          valid = true;        

      } else if (strcmp(argVar[1], "..") == 0){
          strcpy(oldpwd, getcwd(cwd, sizeof(cwd)));
          strcpy(cwd, getcwd(cwd, sizeof(cwd)));

          char * lastSlash = strrchr(getcwd(cwd, sizeof(cwd)), '/');
          *lastSlash = '\0';

          chdir(cwd);
          valid = true;
        } else if (argVar[1][0] == '.' && argVar[1][1] != '.'){  // new stuff
          
          getcwd(cwd, sizeof(cwd));
          memcpy(oldpwd, cwd, MAX_INPUT);
          strcat(cwd, "/");  
          strcat(cwd, strtok(argVar[1], "./"));  
          printf("next: %s\n", cwd);


          chdir(cwd);

          valid = true;
        } else if(!valid){
          strcpy(oldpwd, getcwd(cwd, sizeof(cwd)));
          chdir(getenv("HOME"));
          valid = true;
        }


          //////////////
    } else if(strcmp(argVar[0], "pwd") == 0){
        getcwd(cwd, sizeof(cwd));
        fprintf(stdout, "%s\n", cwd);
      valid = true;

      //need to make it so it reads in echo only
    } else if(strcmp(argVar[0], "echo") == 0){

      char toPrint[1024];
      memset(toPrint, 0, 1024);
      char strArgs[1024];
      strcpy(strArgs, cmd);
      strtok(strArgs, " ");
      strcpy(strArgs, strtok(NULL, ""));

      int i = 0;
      int k;

      for(k = 0; k < strlen(strArgs); k++){

      if(strArgs[k] == '$' && i == 0){
        toPrint[i] = '$';
        i++; k++;

        if(strArgs[k] == '?'){
          toPrint[i] = '?';
          i++; k++;
          if(strArgs[k] == ' ' || strArgs[k] == 0){
            i++;
            printf("%s", getenv("?"));
          }
        } else{
            while(isalnum(strArgs[k])){
              toPrint[i] = strArgs[k];
              i++; k++;
            }
            toPrint[i] = 0;
            i = 0;
            k--;

            printf("%s", getenv(toPrint+1));

        }        
      } else{        
        printf("%c", strArgs[k]);
      }

      }
      printf("\n");

      i = 0;

      valid = true;
    } else if(strcmp(argVar[0], "set") == 0){
      /*char * ptr = */strtok(cmd, " ");
      //printf("%s\n",  ptr); //get set

      char * varName = strtok(NULL, "="); // get variable name
      //printf("%s\n",  varName); 
      char * setVar = strtok(NULL, " "); //get the set var
      //printf("%s\n",  setVar); 

      char* i = varName;
      char* j = varName;
      while(*j != 0)
      {
        *i = *j++;
        if(*i != ' ')
          i++;
      }
      *i = 0;


      printf("%s\n", varName);
      if(*varName == '$')
        setenv(varName+1, setVar, 1);
      else
        printf("invalid variable\n");
      //printf("%s is %s\n", varName, getenv(varName));


      valid = true;
    } else if(strcmp(argVar[0], "help") == 0){
      valid = true;
        printf("\n============Help Menu==============\n");
        printf("cd   change directory\n");
        printf("pwd  change directory to previous directory\n");
        printf("echo echos stuff\n");
        printf("set  set a variable starting with $\n");
        printf("help print this help menu\n");
        printf("exit exit program\n");
        printf("\n");
    } else if(strcmp(argVar[0], "exit") == 0){
      exit(0);
      valid = true;
    } 


    if(!valid){
            printf("%s: command not found\n", argVar[0]);

    }

  }// end of if implemented
  else {
                                  //printf("\nCMD %s %d  %d  l\n", cmd, count,  strCounter);
//    printf("\nhere  %d\n", cmd[3]);

    char * pathStr = getenv("PATH");
    char copy[1024];
    char originalPath[1024];
    strcpy(originalPath, pathStr);

   

    struct stat   buffer;

    pathStr = strtok(pathStr, ":");
    strcpy(copy, pathStr);
    strcat(copy, "/");
    strcat(copy,  argVar[0]);
    //printf("%s\n", copy);

    int reIndex = getIndexRedir(argVar);
    int red;
    if(reIndex >= 0)
      red = open(argVar[reIndex+1], O_RDWR | O_CREAT, 0666);//argVar[reIndex+1] = redirection Name

   

    if(stat(copy, &buffer) == 0){// DOES THE THING
          pid_t pid; int status;

          if ((pid = fork()) ==-1)
            perror("fork error");
          else if (pid == 0) {

            if(reIndex >= 0)
              dup2(red, 1);

            execvp(copy, argv);


          } else {
            pid = wait(&status);
          }
      }

    while((pathStr = strtok(NULL, ":")) != NULL){
      memset(copy, 0, 1024);
      strcpy(copy, pathStr);
      strcat(copy, "/");
      strcat(copy,  argVar[0]);


      if(stat(copy, &buffer) == 0){// DOES THE THING
          pid_t pid; int status;

          if ((pid = fork()) ==-1)
            perror("fork error");
          else if (pid == 0) {
        if(reIndex >= 0){
          int i = 0;;
          while(argVar[i] != NULL){

            
          if(reIndex >= 0 && strcmp(argVar[reIndex], ">") == 0){
            red = open(argVar[reIndex+1], O_RDWR | O_CREAT, 0666);
            dup2(red, 1);


          }
          if(reIndex >= 0 && strcmp(argVar[reIndex], "<") == 0){
            red = open(argVar[reIndex+1], O_RDWR | O_CREAT, 0666);
            dup2(red, 0);
          }
          
            execvp(copy, argv);
            i++;
          }
        }// end if reindex
        else{
          execvp(copy, argv);
        }
          } else {
            pid = wait(&status);
          }
      }
      if(reIndex >= 0)
        close(red);

    }
    setenv("PATH", originalPath, 1); // revert changes to PATH

  }
}// end of null string checking if


    if (!rv) { 
      finished = 1;
      break;
    }


    // Execute the command, handling built-in commands separately 
    // Just echo the command line for now
    // write(1, cmd, strnlen(cmd, MAX_INPUT));

  }

  return 0;
}

int getIndexRedir(char ** argsVar)
{
  int i = 0; 
  while(argsVar[i] != NULL){
    if(strcmp(argsVar[i], ">") == 0 ||
      strcmp(argsVar[i], "0>") == 0 ||
      strcmp(argsVar[i], "1>") == 0 ||
      strcmp(argsVar[i], "2>") == 0 ||
      strcmp(argsVar[i], "<") == 0)
      return i;
    i++;
  }
  return -1;
}

char** argInputs(char * input){
      char ** returnArgs;
      char holderInput[1024];
      strcpy(holderInput, input);
      returnArgs = malloc(1024);
      returnArgs[1] = malloc(1024);
      memset(returnArgs[1], 0, 1024);

      //printf("User inputed: %s\n", input);

      returnArgs[0] = strtok(holderInput, " ");
      //printf("Command Input: %s\n", returnArgs[0]);
      
      //printf("Input: %s\n", returnArgs[0]);
      //strcat(restString, "");
      //printf("lol %s\n", strtok(input, ""));

      if(strtok(NULL, "")){

      strcpy(holderInput, input);

      char restString[1024];
      strtok(holderInput, " ");
      strcpy(restString, strtok(NULL, ""));


    //  printf("Rest of String: %s\n", restString);


      
    //char word[1024];
    //char argsv[50][1024];
    int i = 0; //word
    int j = 1;
    int k =0;

    while(i <= strlen(restString )){
      /*printf(" %c ", restString[i]);
      i++;*/
           
      if(restString[i] == '"'){
          i++;
          while(restString[i] != '"'){
            returnArgs[j][k] = restString[i];
            i++;
            k++;

          }
          returnArgs[j][k] = 0;
          //printf(" %s,", returnArgs[j]);
            k = 0;
            i++;
            i++;
            j++;

            returnArgs[j] = malloc(1024);
            memset(returnArgs[j], 0, 1024);
        } else{    

      if(restString[i] == ' ' || restString[i] == '\0'){
          returnArgs[j][k] = 0;
         //printf(" %s,", returnArgs[j]);

            k = 0;
            i++;
            j++;
            returnArgs[j] = malloc(1024);
            memset(returnArgs[j], 0, 1024);

        } else{
          returnArgs[j][k] = restString[i];
          //printf("no \n");
          i++;
          k++;
        }
    }
  }
   // printf("\n");
}
      return returnArgs;  
}