#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>

char *remove_spaces(char* s) {  // function removes spaces in string:  example: a mds dsf -> amdsdsf
    int i,k =0;
    if (s[0] == ' '){
        while (s[k]==' ') k++;
    }
    s=(s+k);
    k=0;
    for(i=0;s[i];i++)
    {
        s[i]=s[i+k];
        if ((s[i]==' '&& s[i-1]!=' ') || (s[i] == '|')) continue;
        if(s[i]==' '|| s[i]=='\t')
        {
            k++;
            i--;
        }
    }
    if (s[strlen(s)-1]==' ')
        s[strlen(s)-1]='\0';
    return s;


}  // removing spaces

char * makeSpaces(char*str) { // function makes spaces before and after '|' symbol, to split your string in future. example: ls|wc|wc -> ls | wc | wc
    int bufSize = 2048;
    char *s = malloc(bufSize * sizeof(char));
    strcpy(s, str); // copying string
    char swap;
    char symb;
    char buf;
    int k;
    for (int i = 0; i < strlen(s); i++) {
        if ((s[i] == '|' && s[i - 1] != ' ') || (s[i] == '|' && s[i + 1] != ' ')) {   // making spaces before/after |, saving symbols, moving them forward
            k = i + 1;
            symb = s[k];
            buf = s[k + 1];
            s[k] = ' ';
            s[k + 1] = ' ';
            k += 2;
            while (s[k] != '\0' || s[k + 1] != '\0' || s[k - 1] != '\0') {
                swap = s[k];
                s[k] = symb;
                symb = swap;
                swap = s[k + 1];
                s[k + 1] = buf;
                buf = swap;
                k += 2;
            }


        }
    }

    for (int i = 0; i < strlen(s); i++) {
        if (s[i] == '|' && s[i - 1] != ' ') {
            s[i + 1] = s[i];
            s[i] = ' ';
        }
    }
    return s;
}  // adding spaces after/ before |

char * makeSpacesR(char*str){ // making spaces if redirection symbol is met. Example: ls>a.txt -> ls > a.txt
        int bufSize = 2048;
        char* s = malloc(bufSize*sizeof (char )); // func structure is similar to makeSpaces func, can be merged :)
        strcpy(s,str);
        char swap;
        char symb;
        char buf;
        int k;
        for (int i = 0; i < strlen(s); i++) { // making spaces before/after redirection symbols
            if ( (s[i] == '<' && s[i-1]!= ' ' ) ||(s[i] == '<' && s[i+1]!=' ') || ((s[i] == '>' && s[i-1]!= ' ' && s[i-1]!='>' ) ||(s[i] == '>' && s[i+1]!=' ' && s[i+1]!='>')) ||((s[i] == '>' && s[i+1] == '>'&& s[i-1]!= ' ' ) ||(s[i] == '>' && s[i+1] == '>' && s[i+2]!=' ')) || ((s[i] == '2' && s[i+1] == '>'&& s[i-1]!= ' ' ) ||(s[i] == '2' && s[i+1] == '>' && s[i+2]!=' '))){
                k=i+1;
                symb=s[k]; // saving current symb
                buf =s[k+1];
                s[k] =' ';
                s[k+1] = ' ';
                k+=2;
                while (s[k]!='\0' || s[k+1] != '\0' || s[k-1]!='\0'){
                    swap = s[k];
                    s[k] = symb;
                    symb =swap;
                    swap = s[k+1];
                    s[k+1] =buf;
                    buf = swap;
                    k+=2;
                }


            }
        }

        for (int i = 0; i <strlen(s) ; i++) {   // conditions for all redirections : 2>, >>, <,>
            if (s[i] == '2' && s[i-1]!= ' ' || (s[i] == '>' && s[i-1]!= ' ' && s[i-1]!='>')){
                s[i+2] = s[i];
                s[i] = ' ';
            }
            if (((s[i] == '<' &&s[i-1]!=' ') )){
                s[i+1] = s[i];
                s[i]=' ';
            }
            if ((s[i] == '>' &&s[i+1]!=' ' && s[i-1]==' ')){
                s[i-1] = s[i];
                s[i] = ' ';
            }
        }

        for (int i = 0; i < strlen(s); i++) {
            if ((s[i]=='2' && s[i+1] == ' ' && s[i+2] ==' ' && s[i+3]=='>')||((s[i]=='>' && s[i+1] == ' ' && s[i+2] =='>'))){
                s[i+1] =s[i];
                s[i] = ' ';
            }
        }
        for (int i = 0; i < strlen(s); i++) {
            if ((s[i]=='2' && s[i+1] == ' '  && s[i+2]=='>')||((s[i]=='>' && s[i+1] == ' ' && s[i+2] =='>'))){
                s[i+1] =s[i];
                s[i] = ' ';
            }
        }

        for (int i = 0; i < strlen(s); i++) {
            if (s[i] == '>' && s[i+3] =='>'){
                s[i+2] = s[i];
                s[i] =' ';
            }

        }
        return s;


}  // adding spaces after/before redirections

int Arrlen(char **Arr){ // return length of the string array;
    int k=0;
    while (Arr[k]!=NULL){
        k++;
    }
    return k;
}// return length of the string array;


int Arrlens(char ***Arr){ // similar to Arrlen but now array of arrays of string
    int k=0;
    while (Arr[k]!=NULL){
        k++;
    }
    return k;
} // similar to Arrlen but now array of arrays of string

#define addSize 2048
char *readLines(void){ // don't know input string len, so using malloc; function is used for reading input string
    int bufferSize = addSize;
    char *buffer = (char *)malloc(bufferSize*sizeof(char ));
    int symbol=0;
    int pos = 0;

    if (buffer == NULL){
        fprintf(stderr,"malloc error occured"); // malloc check
        exit(EXIT_FAILURE);
    }
    while (1){

        symbol = fgetc(stdin); // if EOF or '\0' -> saving buffer
        if (symbol== EOF || symbol == '\n' ){
            buffer[pos] = '\0';
            return buffer;
        } else buffer[pos] = symbol; // or continue writing to buffer
        pos++;

        if (pos >= bufferSize){    // if buffer if full -> realloc
            bufferSize += addSize;
            buffer=realloc(buffer,bufferSize);
            if (!buffer){
                fprintf(stderr,"realloc error occured");
                exit(EXIT_FAILURE);
            }
        }

    }

} //function is used for reading input string

#define parseConst 128
char **lineParsing(char *string){ // function is used for string parcing e.g : ls | wc -> (ls,|,wc), making array of strings
    int realCoef = parseConst;
    char **parsedArr = malloc(realCoef*sizeof (char *));
    int position=0;
    char * token;

    if (parsedArr == NULL){
        fprintf(stderr,"malloc err");
    }
    token=strtok(string," ");

    while (token!=NULL){
        parsedArr[position]= token;
        position++;
        if (position>=realCoef){
            realCoef +=parseConst;
            parsedArr=realloc(
                    parsedArr,realCoef*sizeof (char *));
            if (!parsedArr){
                fprintf(stderr,"realloc parcing err");
                exit(EXIT_FAILURE);
            }
        }
        token=strtok(NULL," ");
    }
   // parsedArr[position] = NULL; // closing with NULL is necessary for exec func;
    //parsedArr[position+1] = NULL;
    return parsedArr;
} // parsing strings -> array of strings

char ***arrSplit(char ** inputArr){
    int reallocConst = parseConst;
    char ** bufArr = malloc(sizeof (char *) * reallocConst);
    char ***splited = malloc(sizeof (char **)*reallocConst);
    for (int i = 0; i < reallocConst; i++) {
        splited[i] = malloc(sizeof (char *) * reallocConst);
    }
    int k =0;
    int l = 0;
    for (int i = 0; i < Arrlen(inputArr); i++) {
        if (inputArr[i][0]!='|' ){
            bufArr[l] = inputArr[i];
            l++;
        }
        else{

            for (int j = 0; j < Arrlen(bufArr); j++) {
                splited[k][j] = bufArr[j];
            }
            splited[k][Arrlen(bufArr)]= NULL; //  closing with NULL for exec
            bufArr = (bufArr+Arrlen(bufArr));
            k++;
            l = 0;

        }
    }

    for (int j = 0; j < Arrlen(bufArr); j++) {
        splited[k][j] = bufArr[j];
    }
    splited[k][Arrlen(bufArr)]= NULL;//  closing with NULL for exec

    splited[k+1] = NULL;
    return splited;
} // making array of arrays of strings

int* redir(char **args){ // if we have redirection, have to prepare descriptors
    int readd=0; // <
    int writee=0; // > >>
    int writes =0;
    int *retArr=malloc(sizeof(int ) * 3);
    int switcher=0;
    char *fileName=malloc(100* sizeof(char ));
    char *redirectors[4];
    redirectors[0] = "<";
    redirectors[1] = ">";
    redirectors[2] = ">>";
    redirectors[3] = "2>";

    for (int i = 0; i < Arrlen(args); i++) {
        for (int j = 0; j <4 ; j++) {
            if (strcmp(args[i],redirectors[j])==0 && i+1!=Arrlen(args)){
                switcher=j+1;
                strcpy(fileName,args[i+1]);

            }
        }
        if ((strcmp(args[i],redirectors[1])==0 ||(strcmp(args[i],redirectors[0])==0 ) || (strcmp(args[i],redirectors[2])==0 ) || (strcmp(args[i],redirectors[3])==0 )) && args[i+1]==NULL)
            switcher = -2;

        if (switcher==-2){
            kill(getppid(),SIGUSR1);  // sending signal to parent, invalid redirection args
        }
        switch (switcher) {
            case 1: readd = open(fileName,O_RDONLY);break;
            case 2:writee= creat(fileName,0666);
            writee = open(fileName,O_WRONLY); break;
            case 3:writee = open(fileName,O_WRONLY|O_APPEND) ;if(writee == -1) writee = creat(fileName,0666);break;
            case 4: writes = open(fileName,O_WRONLY);break;
            case -1: printf("check args"); break;
            default:continue;
        }

    }
    retArr[0] = readd;
    retArr[1] = writee;
    retArr[2] = writes;
    return retArr;

}  // working with redirections

char **clearRed(char **arr){ // clear arg arr from redirectors
    char *redirectors[4];
    char ** retArr = malloc(100* sizeof(char *));
    int flag = 0;
    redirectors[0] = "<";
    redirectors[1] = ">";
    redirectors[2] = ">>";
    redirectors[3] = "2>";
    int k =0;
    int i=0;
    while (arr[i]!=NULL){
        for (int j = 0; j < 4; j++) {
            if (strcmp(arr[i], redirectors[j]) == 0) {
                flag=1;
                break;
            }

        }
        if (!flag){
            retArr[k] = arr[i];
            k++;
            i++;
        } else i+=2; // redirect plus file name


    }
    return retArr;

}  // cleaning redirect from cur command list e.g. (ls,>,a.txt) -> (ls,a.txt);

void ign(pid_t s){
    signal(SIGINT,SIG_IGN);
} // ignoring CTRL+C in shell process, can be checked with top command, top will be stopped shell process continue
void arg(int s){
    signal(SIGUSR1,arg);
    printf("arg!!!\n");
}
void stop(int s){
    pid_t pid;
    pid=getpid();
    printf("stopped pid = %d", pid );
    signal(SIGINT,SIG_DFL);
    signal(SIGTSTP,SIG_DFL);
} // used to igone CTRL+Z, working on it

int shell(char ***argsArr){ // shell func
pid_t pid,ppid;
int status;
int fd[2];
int i =0;
int *redirr=malloc(3* sizeof(int));

    while (argsArr[i] != NULL){
        if (pipe(fd)<0){perror("pipe err");return 1;} // making pipe
        int oldfd0; // saving descriptor from previous command, so each time new pipe is made, can't read from previous without saving
        switch ((pid = fork())) {
            case -1: return 1;
            case 0:
                signal(SIGINT,stop); // redirecting signal CTRL+C, works
                signal(SIGTSTP,stop); // redirecting signal CTRL+Z, doesn't work
                redirr = redir(argsArr[i]); // getting redirection if required
                argsArr[i]=clearRed(argsArr[i]); // cleaning from redirections
                if (i+1!=Arrlens(argsArr) )// while not last command
                    dup2(fd[1],1);// output to pipe
                if (i!=0){
                    dup2(oldfd0,0);   // if not first read from pipe
                }
                if (redirr[2]!=0){
                    dup2(redirr[2],2); // redir from stderr to file
                    close(redirr[2]);
                }

               if (redirr[1]!=0){
                    dup2(redirr[1],1);
                    close(redirr[1]);
                }
                if (redirr[0]!=0){
                    dup2(redirr[0],0);
                    close(redirr[0]);
                }
                close(fd[1]); // closing descriptors
                close(fd[0]);

                execvp(argsArr[i][0],argsArr[i]); // executing commands
                if (execvp(argsArr[i][0],argsArr[i])==-1)
                    printf("execvp err,check args");
                exit(0);


        }
        oldfd0 = fd[0];
        close(fd[1]);

        i++;
    }
    while (wait(NULL)!=-1); // waiting sons
    return 1;


} // shell function


// builtin funcs:
int cd(char **args){
    if (args[1] == NULL){
        fprintf(stderr, "shell: waiting for \"cd\"\n");

    } else {
        if (chdir(args[1])!= 0){
            perror("args");
        }
    }
    return 1;
}
int shellExit(char **args){
    printf("goodbye!");
    exit(0);

}
int mypwd(char **args){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s", cwd);
    return 0;
}


void builtInCheck(char **args) {   // working with built in shell funcs

    if (strcmp(args[0], "cd") == 0) {
        cd(args);
        mypwd(args);
    }
    if (strcmp(args[0], "exit") == 0)shellExit(args);
    for (int i = 0; i < Arrlen(args); i++) {
        if (strcmp(args[i], "pwd") == 0 && args[i+1]==NULL )
            mypwd(args);
    }
}
void commandFunc(void ){
    int status;
    int myComs =0;
    char *inputLine;
    char ** argumentArr;
    char ***executerArr;
    char *ListOfBuiltInFuncs[myComs];
    ListOfBuiltInFuncs[0] = "exit";
    ListOfBuiltInFuncs[1] = "cd";
    ListOfBuiltInFuncs[2] = "pwd";
    do {
        printf(">");
        inputLine=readLines();
        inputLine = remove_spaces(inputLine);
        inputLine=makeSpaces(inputLine);
        inputLine = makeSpacesR(inputLine);
       printf("%s\n",inputLine);

    argumentArr = lineParsing(inputLine);
    builtInCheck(argumentArr);


    printf("\n");
    executerArr = arrSplit(argumentArr);
   status = shell(executerArr);

    }
    while (status);

    }


int main() {
    pid_t fPid;
    fPid = getppid();
    ign(fPid);
    signal(SIGINT,ign);
    signal(SIGTSTP,ign);
    signal(SIGUSR1,arg);
commandFunc();

    return 0;
}


