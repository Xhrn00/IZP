#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/*The structure where will writtten  name and number are read from the seznam.txt file.*/
typedef struct Contact {
    char name[255];
    char number[255];
} Contact;

/*Checks for the presence of an included file when compiling the program*/
 bool isFileProvided() {
    return !isatty(STDIN_FILENO);
}

/*Convert to int query numbers*/
int convertToInt(char ch) {
    return ch - '0';
}

/*Checks the number of elements in a string, the strtok( ) function splits strings into substrings.
 Using the ASCII table, we check that our name consists of uppercase and lowercase letters of the English alphabet, 
 and the name can also include "."  , such as Bedrich Smetana ml.*/

int isNameValid(char *name) {
    if (strlen(name) > 100) {
        return 0;
    }
    
    char *token;
    token = strtok(name, " ");
    while( token != NULL ) {
        for (unsigned i=0; i<strlen(token); i++) {
            if(token[i] == '.' ||
            (token[i] >= 'a' && token[i] <= 'z') ||
            (token[i] >= 'A' && token[i] <= 'Z')) {
                continue;
            } else {
                return 0;
            }
        }
        token = strtok(NULL, " ");
    }
    return 1;
}

/*Checks the number of elements in a string, checks using the ASCII table that the number consists of digits.*/

int isNumber(char *text)
{
    int j;
    j = strlen(text);
    if (j > 100) {
        return 0;
    }
    while(j--)
    {
        if(text[j] > 47 && text[j] < 58)
            continue;

        return 0;
    }
    return 1;
}

/*checks if number includes query*/
int isNumberMatchedQuery(char *number, char *query) {
   char *res = strstr(number, query);
   return !!res ? 1 : 0;
}

int isMatched(char n, char *query) {
   for (unsigned i=0; i<strlen(query); i++) {
        char ch = query[i];
        if (tolower(n) == ch) {
            return 1;
        }
    }
    return 0;
}

/*The IsNameMatchedQuery function does a spell search.
 First, we create an array Keys , in which we write our letters and "+", so that each index of the array corresponds to a specific digit.
 
 *For example, we passed the value "38" to the program.
 We convert to int query numbers (our parameters passed to the program), but now only the first element. 
  Now this number is the index of our array, that is, this number corresponds to the corresponding element from the array, for example: 
  number 3 , *Keys[3]- 3 element of the letter "def".
 Then we check each letter of the name for a match with the elements of the array, 
 and if a match is found, we convert the next element of the array (of the parameters passed to the program) to int and check 
 for a match the element after the element that already matches.
 
 *If the element matches, return our name.*/
int isNameMatchedQuery(char *name, char *queryNumbers) {
    char *KEYS[] = {"+", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
    int index = convertToInt(queryNumbers[0]);
    int matched = 0;
    char *query;
    query = KEYS[index];

    for(unsigned i=0; i<strlen(name); i++) {
        char symb = name[i];
        matched = isMatched(symb, query);
        int nextSymbIndex=i;
        unsigned q=0;
        while (matched)
        {
            nextSymbIndex+=1;
            q+=1;
            if (q < strlen(queryNumbers)) {
                int queryIndex = convertToInt(queryNumbers[q]);
                char nextSymb = name[nextSymbIndex];
                char *nextQuery = KEYS[queryIndex];
                matched = isMatched(nextSymb, nextQuery);
            } else {
                if (matched) {
                    return matched;
                } else {
                    break;
                }
            }
        }
    }
    return matched;
}

/*Check data for validity*/

int isValid(Contact contact){
    int isValid = isNumber(contact.number) && isNameValid(contact.name);
    return isValid;
}

/*Displays our contact*/

void print(Contact contact) {
    printf("%s, %s", contact.name, contact.number);
    printf("\n");
}

/*The run() function, at first we pass the Contact contact structure to the function;
 
 *Then, using a while cycle, we read the name and number from the seznam.txt file.
 
 *Then using if check this data for validity, 
 if the data is not valid, a message is displayed: "Skipped invalid contact:" and the contact.
 
 *IsNumberMatchedQuery() and IsNameMatchedQuery (read about them above)
 If at least 1 of these functions worked correctly (| |),  print ( ) function is called, 
 which displays our contact, if none of the functions worked correctly, a message is displayed on the screen: "Not found".

*Then return to while from read another contact until the file ends (!=EOF)*/

void run(char *query) {
    Contact contact;
    int records = 0;
    while( scanf(" %[^\n]", contact.name) != EOF &&
       scanf(" %[^\n]", contact.number) != EOF) {
        if (!isValid(contact)) {
            printf("Skipped invalid contact: %s \n", contact.name);
            continue;
        }

        int isNumberMatched = query ? isNumberMatchedQuery(contact.number, query) : 1;
        int isNameMatched = query ? isNameMatchedQuery(contact.name, query) : 1;
        if (isNumberMatched || isNameMatched) {
            print(contact);
            records++;
        }
    }

    if (records == 0) {
        printf("Not found \n");
    }
}

/*In the main() function, 
at first checks for the presence of an included file when compiling the program using a function type of bool: IsFileProvided().

 *If the file is not connected, the message "No file provided" is displayed.

 *If the file is included, then a check is made for the number of parameters passed by the user to the program:
 -If only 1(program name) is entered, then we call the run() function, thereby displaying the entire seznam.txt.
 -If 2, then the program is executed correctly (search starts).
 -If more than 2, then a message is displayed: "Too many parameters passed to the search!*/

int main(int argc, char *argv[]) {
    if (isFileProvided())
    {
        char *query = NULL;
       if (argc == 1)
       {
         run(query);
       } else if (argc == 2) {
            query = argv[1];
            run(query);
        } else {
            printf("Too many parameters passed to the search!");
        }
        } else {
        printf("No file provided \n");
    }

    return 0;
}
