#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <err.h>


char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

char* strToBin(int nb,size_t bits) {
    int save = 0;
    size_t i = 0;
    char* res = malloc(11 * sizeof(char));
    int x = nb;
    while (x >= 1) {	
        save = x%2;
        res[i] = save + '0';
        x /= 2;
        i++;

    }
    while (strlen(res) < bits) {
        res[i] = '0';
        i++;
    }
    strrev(res);
    res[i] = 0;
    return res;

}



int charDeter(char c) {
    switch (c) {

        case ' ':
            return 36;
        case '$':
            return 37;
        case '%':
            return 38;
        case '*':
            return 39;
        case '+':
            return 40;
        case '-':
            return 41;
        case '.':
            return 42;
        case '/':
            return 43;
        case ';':
            return 44;
    }
    return -1;
}


char* urlEncoder(char* url) {
    int len = strlen(url);
    char* res = malloc(len * 6 * sizeof(char));
    int isOdd;
    if (len % 2 != 0)
        isOdd = 0;
    else
        isOdd = 1;
    int count = 0;

    int saveNum = 0;

    int alValue = 0;

    while (url[count] != 0) {
    char curChar = url[count];
        
    if (isdigit(curChar)) {
        alValue = curChar - '0';
        
    }


    else if (isalpha(curChar)) {
        char maj = toupper(curChar);
        alValue = maj - 'A' + 10;
        
    }

    else
        alValue = charDeter(curChar);


    if (!isOdd && url[count + 1] == 0)
        {
            strcat(res,strToBin(alValue,11));
        }
    
    else if  (count != 0 && (count+1)%2 == 0){
        saveNum = saveNum * 45 + alValue;
        strcat(res,strToBin(saveNum,11));
    }
    else
        {
            saveNum = alValue;
        }
    count++;
    
    }
    
    char* finalDest = malloc(((len *11)/2)  * sizeof(char) + 13);
    strcat(finalDest,strToBin(len,9));
    strcat(finalDest,"0010");
     strcat(finalDest,res);
    printf("%zu\n",strlen(finalDest));
    return finalDest;
}


int main(int argc, char** argv) {
if (argc < 2)
    errx(1,"Invalid Argument Number !");

printf("%s\n",urlEncoder(argv[1]));


return 0;

}
