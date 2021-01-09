#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <readline/readline.h>

char *remove_spaces(char* s) {
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
        if(s[i]==' '|| s[i]=='\t'/*||(s[i]=='|' && s[i+1]==' ')*/)
        {
            k++;
            i--;
        }
    }
  /*  for (int j = 0; j < strlen(s); ++j) {
        if (s[j]=='|') s[j]=' ';

    }*/
    if (s[strlen(s)-1]==' ')
        s[strlen(s)-1]='\0';

    return s;


} // сначала убирваем пробелы строчка слиплась

char * makeSpaces(char*str) {
    int bufSize = 2048;
    char *s = malloc(bufSize * sizeof(char));
    strcpy(s, str);
    char swap;
    char symb;
    char buf;
    int k;
    for (int i = 0; i < strlen(s); i++) {
        if ((s[i] == '|' && s[i - 1] != ' ') || (s[i] == '|' && s[i + 1] != ' ')) {
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
} // теперь разлепляем  |

char * makeSpacesR(char*str){
        int bufSize = 2048;
        char* s = malloc(bufSize*sizeof (char ));
        strcpy(s,str);
        char swap;
        char symb;
        char buf;
        char space;
        int k;
        for (int i = 0; i < strlen(s); i++) {
            if ( (s[i] == '<' && s[i-1]!= ' ' ) ||(s[i] == '<' && s[i+1]!=' ') || ((s[i] == '>' && s[i-1]!= ' ' && s[i-1]!='>' ) ||(s[i] == '>' && s[i+1]!=' ' && s[i+1]!='>')) ||((s[i] == '>' && s[i+1] == '>'&& s[i-1]!= ' ' ) ||(s[i] == '>' && s[i+1] == '>' && s[i+2]!=' ')) || ((s[i] == '2' && s[i+1] == '>'&& s[i-1]!= ' ' ) ||(s[i] == '2' && s[i+1] == '>' && s[i+2]!=' '))){
                k=i+1;
                symb=s[k];
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

        for (int i = 0; i <strlen(s) ; i++) {
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


} // вроде работает, команды арги и перенаправления отделены

int Arrlen(char **Arr){ // функция вычисляет длину массива строк
    int k=0;
    while (Arr[k]!=NULL){
        k++;
    }
    return k;
}


int Arrlens(char ***Arr){ // функция вычисляет длину массива строк
    int k=0;
    while (Arr[k]!=NULL){
        k++;
    }
    return k;
}
#define addSize 2048
char *readLines(void){ // число команд и размеры буффера заранее неизвестны, будем читать строку и реаллоком по необходимости расширять
    int bufferSize = addSize;
    char *buffer = (char *)malloc(bufferSize*sizeof(char ));
    int symbol=0;
    int pos = 0;

    if (buffer == NULL){
        fprintf(stderr,"malloc error occured"); // malloc check
        exit(EXIT_FAILURE);
    }
    while (1){

        symbol = fgetc(stdin); // если eof или конец строки записываем признак конца строки, сохраняем буффер
        if (symbol== EOF || symbol == '\n' ){
            buffer[pos] = '\0';
            return buffer;
        } else buffer[pos] = symbol; // иначе продолжаем писать посимвольно в буффер - строку
        pos++;

        if (pos >= bufferSize){    // если забили буффер - реалокнуть
            bufferSize += addSize;
            buffer=realloc(buffer,bufferSize);
            if (!buffer){
                fprintf(stderr,"realloc error occured");
                exit(EXIT_FAILURE);
            }
        }

    }

}
#define parseConst 128
char **lineParsing(char *string){
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
                fprintf(stderr,"realloc parce err");
                exit(EXIT_FAILURE);
            }
        }
        token=strtok(NULL," ");
    }
    parsedArr[position] = NULL;
    parsedArr[position+1] = NULL;



    return parsedArr;
} // parsing strings
char ***arrSplit(char ** inputArr){
    int reallocConst = parseConst;
    char ** bufArr = malloc(sizeof (char *) * reallocConst);
    char ***splited = malloc(sizeof (char **)*reallocConst);
    for (int i = 0; i < reallocConst; i++) {
        splited[i] = malloc(sizeof (char *) * reallocConst);
    }
    int k =0;
    int l = 0;
    int flag = 0;
    for (int i = 0; i < Arrlen(inputArr); i++) {
        if (inputArr[i][0]!='|' ){

            bufArr[l] = inputArr[i];
            l++;
        }
        else{
            flag=1;
          /*  for (int j = 0; j < Arrlen(bufArr); j++) {
                printf("buffar %s\n",bufArr[j]);
            }*/
            for (int j = 0; j < Arrlen(bufArr); j++) {
                splited[k][j] = bufArr[j];
            }
            splited[k][Arrlen(bufArr)]= NULL;

           for (int j = 0; j <= Arrlen(splited[k]) ; j++) {
                printf("for k :%d splited %s \n",k,splited[k][j]);
            }
           bufArr = (bufArr+Arrlen(bufArr));
            //for (int j = 0; j < Arrlen(bufArr); j++)
               // bufArr[j][0] = '\0';

            k++;
            l = 0;

        }
    }

    for (int j = 0; j < Arrlen(bufArr); j++) {
        splited[k][j] = bufArr[j];
    }
    splited[k][Arrlen(bufArr)]= NULL;

//    for (int j = 0; j <= Arrlen(splited[k]) ; j++) {
  //      printf("k is: %d splited %s \n",k,splited[k][j]);
        /*if (splited[k][j]==NULL){
            printf("yap");
        }*/
   // }
    splited[k+1] = NULL;
    return splited;
} // making 3 dim char arr

int* redir(char **args){ // to understand which descriptors to open for currecnt part of conv
    int *required[2]={0};// есл нет редиректа, то оставляем конвейер как есть; первое откуда читать второе на запись
    int readd=0; // <
    int writee=0; // > >>
    int writes =0;
    int *retArr=malloc(sizeof(int ) * 3);
    int switcher=0;
    char *fileName=malloc(100* sizeof(char ));
    int flag = 0;

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
            kill(getppid(),SIGUSR1);
        }
        switch (switcher) {
            case 1: readd = open(fileName,O_RDONLY);break;
            case 2:writee= creat(fileName,0666);
            writee = open(fileName,O_WRONLY); break;
            case 3:writee = open(fileName,O_WRONLY|O_APPEND) ;if(writee == -1) writee = creat(fileName,0666);break;
            case 4: writes = open(fileName,O_WRONLY);break;
            case -1: printf("check aars"); break;
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

}  // cleaning redirect from cur command list

void ign(pid_t s){
    pid_t Father;
    signal(SIGINT,SIG_IGN);
  // signal(SIGTSTP,SIG_IGN);
}
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
}
int shell(char ***argsArr){
pid_t pid,ppid;
int status;
int fd[2];
int i =0;
int *redirr=malloc(3* sizeof(int));

    while (argsArr[i] != NULL){


        if (pipe(fd)<0){perror("pipe err");return 1;}
        int oldfd0;
        switch ((pid = fork())) {
            case -1: return 1;
            case 0:
                printf("%d",pid);
                signal(SIGINT,stop);
                signal(SIGTSTP,stop);
                redirr = redir(argsArr[i]);
                argsArr[i]=clearRed(argsArr[i]);
                if (i+1!=Arrlens(argsArr) )// пока не последняя команда перенаправляем вывод
                    dup2(fd[1],1);
                if (i!=0){
                    dup2(oldfd0,0);   // ошибка тут, почему дескриптор мешает вводу строк в дальнейшем
                }
                if (redirr[2]!=0){
                    dup2(redirr[2],2);
                    close(redirr[2]);
                }

               if (redirr[1]!=0){
                    dup2(redirr[1],1);
                    close(redirr[1]);
                }
                if (redirr[0]!=0){
                    dup2(redirr[0],0); // close?
                    close(redirr[0]);
                }
                close(fd[1]);
                close(fd[0]);

                execvp(argsArr[i][0],argsArr[i]);
                if (execvp(argsArr[i][0],argsArr[i])==-1)
                    printf("execvp err,check args");
                exit(0);


        }
        oldfd0 = fd[0];
        close(fd[1]);

        i++;
    }
    while (wait(NULL)!=-1);
    return 1;


}


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


void builtInCheck(char **args) {   // работа со встренными функциями

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
    char cwd[1024];
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
    /*for (int i = 0; i < Arrlen(argumentArr); i++) {
        printf("%s\n",argumentArr[i]);
    }*/

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

// TODO
//  built in funcs, realisation is done need implementation, implementation done
//  signal
//  redirect dup2 probably done
//  make usage of exit
