/*  ---------------------------------------------------------------
 *	2nd sem MTech (CSE)												*
 *	Compilers & Advanced Operating Systems							*
 *  Assignment														*
 *  ---------------------------------------------------------------
 *	Sample C grammar:												*
 *  =================												*
 *  Data Types			:	int, char								*
 *	Expressions       	:	Arithmetic and Relational				*
 * 	Looping statements 	:	for, while 								*
 * 	Decision statements	:	if, if – else 							*
 *  ---------------------------------------------------------------
 *  Task 1: Lexical analyzer										*
 *  ---------------------------------------------------------------
 *  Description:													*
 *  ============													*
 *	For the given grammar, write & implement a lexical analyzer. 	*
 *	Identifying different classes of tokens like keywords, 			*
 *	identifiers & numbers. The tokens should be displayed suitably, *
 *	preferably one token per line along with the attribute value,	*
 *	if any. Also, a symbol table should be created and all the 		*
 *	identifiers should be entered in the symbol table. The 			*
 *	symbol table should contain at least two fields name and type.	*
 *  ----------------------------------------------------------------
 *
 *  main.c
 *  basicLexer
 *
 *  Created by Anurag Gupta on 03/02/17.
 *  Copyright © 2017 Anurag Gupta. All rights reserved.
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "basicLexer.h"
#define BUF_SIZE 1024
#define TEST 1

struct Tokens *holdRecentAddr;

extern int lineno;
extern int whiteSpaces;
extern int totalTokenCount;
extern char * keywords[];
extern char * relationalOperators[];
extern char otherOperators[];
extern char delimiters[];

void printFormatting(char *, char);

int main(int argc, char **argv) {
    char *buffer;
    char c;

    // Initial setup of variables
    lineno=1;
    whiteSpaces=0;
    totalTokenCount=0;

    // Load the Symbol Table
    loadSymbolTable(SymbolTable);
    
    // Open file
    FILE *fp = fopen("test.txt", "r");
    
    // Check for file availability
    if(fp == NULL) {
        (void) fprintf(stderr, "Error: Failed to open file.\n");
        exit(EXIT_FAILURE);
    }
    
    // Allocate memory to buffer.
    buffer = (char *)calloc(BUF_SIZE, sizeof(char));
    if(buffer == NULL) {
        (void) fprintf(stderr, "Buffer allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    // Set initial values of buffer to '\0'
    memset(buffer, '\0', BUF_SIZE);

    printFormatting("Contents of test.txt", '*');
    while((c=fgetc(fp)) != EOF) {
        putchar(c);
    }
    printf("\n--------------------------------------\n");

    // Resetting the file pointer
    rewind(fp);

    // Read from buffer unless it reaches to '\0'
    while( !feof(fp) ) {
        #ifdef TEST
            printf("-------Inside while loop.-------\n");
        #endif

        // Retreive the character first.
        c = fgetc(fp);

        if( c == ' ' || c == '\t') { // To identify whitespaces
            whiteSpaces++;
            #ifdef TEST
                printf("white space count: %d\n", whiteSpaces);
            #endif
        }
        else if( c == '\n' ) { // To identify end of
                               // line
            lineno++;
            #ifdef TEST
                printf("lineno: %d\n", lineno);
            #endif
        }
        else if( c == '"' ) { // To identify string literal         
            int index = 0;
            totalTokenCount += 1;
            
            buffer[index++] = c;
            
            c = fgetc(fp);
            while( c != '"' ) {
                buffer[index] = c;
                c = fgetc(fp);
                index++;
            }
            buffer[index] = '"';

            #ifdef TEST
                printf("%s, %d\n", buffer, index);
            #endif

            // Lookup and install into symbol table
            if((installTokens(buffer, "char", LITERALS, lineno))==0) {
                (void) fprintf(stdout, "%s at line %d is already installed in symbol table!\n", buffer, lineno);
                //(void) fprintf(stdout, "Error at line %d: failed to install %s in symbol table!\n", lineno, buffer);
            } else {
                (void) fprintf(stdout, "%s at line %d is installed in symbol table!\n", buffer, lineno);
            }
            
            // set the buffer to initial state
            memset(buffer, '\0', strlen(buffer));

        }
        else if(isalpha(c)) {     // Recognize keywords and identifiers
            int index = 0;
            
            do {
                buffer[index]=c;
                c=fgetc(fp);
                index++;
            } while(isalpha(c));

            // compare
            if(strcmp(buffer, "int")==0) {
                printf("'%s' is a keyword\n", buffer);
            }
            else {
                printf("'%s' is an idenfier\n", buffer);
            }

            // reset buffer;
            memset(buffer, '\0', strlen(buffer));
        }
        else if( c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' ) {
            int index=0;
            totalTokenCount += 1;

            while ( c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' ) {
                buffer[index] = c;
                c = fgetc(fp);
                index++;
            }

            // push the character back to the file
            ungetc(c, fp);

            printf("%s, %lu\n", buffer, strlen(buffer));
            
            // Lookup and install into symbol table
            if((installTokens(buffer, "integer", NUMBER, lineno))==0) {
                (void) fprintf(stdout, "%s at line %d is already installed in symbol table!\n", buffer, lineno);
                //(void) fprintf(stdout, "Error at line %d: failed to install %s in symbol table!\n", lineno, buffer);
            } else {
                (void) fprintf(stdout, "%s at line %d is installed in symbol table!\n", buffer, lineno);
            }
            
            // set the buffer to initial state
            memset(buffer, '\0', strlen(buffer));
        }
        else if( c == '.' || c == '(' || c == ')' || c == ',' || c == '{' || c == '}' || c == ';' || c == '\'' ) {
            totalTokenCount += 1;
            buffer[0] = c;
            printf("'%s', %lu\n", buffer, strlen(buffer));
            
            // Lookup and install into symbol table
            if((installTokens(buffer, "char", DELIM, lineno))==0) {
                (void) fprintf(stdout, "'%s' at line %d is already installed in symbol table!\n", buffer, lineno);
                //(void) fprintf(stdout, "Error at line %d: failed to install %s in symbol table!\n", lineno, buffer);
            } else {
                (void) fprintf(stdout, "'%s' at line %d is installed in symbol table!\n", buffer, lineno);
            }

            // set the buffer to initial state
            memset(buffer, '\0', strlen(buffer));
        }
        else if( c == '=' ) {
            totalTokenCount += 1;
            TokenType ttype;

            // get next character
            c = fgetc(fp);
            if( c == '=') {
                buffer[0] = c;
                buffer[1] = c;
                ttype = RELOP;
            } else {
                // set pointer to previous postion by 1
                fseek(fp, -1L, 1);
                buffer[0] = '=';
                ttype = OP;
            }
            printf("'%s', %d\n", buffer, 1);
            
            // Lookup and install into symbol table
            if((installTokens(buffer, "relational operators", ttype, lineno))==0) {
                (void) fprintf(stdout, "'%s' at line %d is already installed in symbol table!\n", buffer, lineno);
                //(void) fprintf(stdout, "Error at line %d: failed to install %s in symbol table!\n", lineno, buffer);
            } else {
                (void) fprintf(stdout, "'%s' at line %d is installed in symbol table!\n", buffer, lineno);
            }

            // set the buffer to initial state
            memset(buffer, '\0', strlen(buffer));
        }
        else if( c == '+' ) {
            totalTokenCount += 1;

            // get next character
            c = fgetc(fp);
            if( c == '+') {
                buffer[0] = c;
                buffer[1] = c;
            } else {
                // set pointer to previous postion by 1
                fseek(fp, -1L, 1);
                buffer[0] = '+';
            }
            printf("'%s', %d\n", buffer, 1);
            
            // Lookup and install into symbol table
            if((installTokens(buffer, "arithmetic operators", OP, lineno))==0) {
                (void) fprintf(stdout, "'%s' at line %d is already installed in symbol table!\n", buffer, lineno);
                //(void) fprintf(stdout, "Error at line %d: failed to install %s in symbol table!\n", lineno, buffer);
            } else {
                (void) fprintf(stdout, "'%s' at line %d is installed in symbol table!\n", buffer, lineno);
            }

            // set the buffer to initial state
            memset(buffer, '\0', strlen(buffer));
        }
        else if( c == '-' ) {
            totalTokenCount += 1;

            // get next character
            c = fgetc(fp);
            if( c == '-') {
                buffer[0] = c;
                buffer[1] = c;
            } else {
                // set pointer to previous postion by 1
                fseek(fp, -1L, 1);
                buffer[0] = '-';
            }
            printf("'%s', %d\n", buffer, 1);
            
            // Lookup and install into symbol table
            if((installTokens(buffer, "arithmetic operators", OP, lineno))==0) {
                (void) fprintf(stdout, "'%s' at line %d is already installed in symbol table!\n", buffer, lineno);
                //(void) fprintf(stdout, "Error at line %d: failed to install %s in symbol table!\n", lineno, buffer);
            } else {
                (void) fprintf(stdout, "'%s' at line %d is installed in symbol table!\n", buffer, lineno);
            }

            // set the buffer to initial state
            memset(buffer, '\0', strlen(buffer));
        }
        else if( c == '*' ) {
            totalTokenCount += 1;

            buffer[0] = c;
            printf("'%s', %lu\n", buffer, strlen(buffer));
            
            // Lookup and install into symbol table
            if((installTokens(buffer, "arithmetic operators", OP, lineno))==0) {
                (void) fprintf(stdout, "'%s' at line %d is already installed in symbol table!\n", buffer, lineno);
                //(void) fprintf(stdout, "Error at line %d: failed to install %s in symbol table!\n", lineno, buffer);
            } else {
                (void) fprintf(stdout, "'%s' at line %d is installed in symbol table!\n", buffer, lineno);
            }

            // set the buffer to initial state
            memset(buffer, '\0', strlen(buffer));
        }
        else if( c == '/' ) {
            totalTokenCount += 1;

            buffer[0] = c;
            printf("'%s', %lu\n", buffer, strlen(buffer));
            
            // Lookup and install into symbol table
            if((installTokens(buffer, "arithmetic operators", OP, lineno))==0) {
                (void) fprintf(stdout, "'%s' at line %d is already installed in symbol table!\n", buffer, lineno);
                //(void) fprintf(stdout, "Error at line %d: failed to install %s in symbol table!\n", lineno, buffer);
            } else {
                (void) fprintf(stdout, "'%s' at line %d is installed in symbol table!\n", buffer, lineno);
            }

            // set the buffer to initial state
            memset(buffer, '\0', strlen(buffer));
        }
        else if( c == '%' ) {
            totalTokenCount += 1;

            buffer[0] = c;
            printf("'%s', %lu\n", buffer, strlen(buffer));
            
            // Lookup and install into symbol table
            if((installTokens(buffer, "arithmetic operators", OP, lineno))==0) {
                (void) fprintf(stdout, "'%s' at line %d is already installed in symbol table!\n", buffer, lineno);
                //(void) fprintf(stdout, "Error at line %d: failed to install %s in symbol table!\n", lineno, buffer);
            } else {
                (void) fprintf(stdout, "'%s' at line %d is installed in symbol table!\n", buffer, lineno);
            }

            // set the buffer to initial state
            memset(buffer, '\0', strlen(buffer));
        }
        else if( c == '<' ) {
            totalTokenCount += 1;

            // get next character
            c = fgetc(fp);
            if( c == '=') {
                buffer[0] = '<';
                buffer[1] = c;
            } else {
                // set pointer to previous postion by 1
                fseek(fp, -1L, 1);
                buffer[0] = '<';
            }
            printf("'%s', %d\n", buffer, 1);
            
            // Lookup and install into symbol table
            if((installTokens(buffer, "relational operators", RELOP, lineno))==0) {
                (void) fprintf(stdout, "'%s' at line %d is already installed in symbol table!\n", buffer, lineno);
                //(void) fprintf(stdout, "Error at line %d: failed to install %s in symbol table!\n", lineno, buffer);
            } else {
                (void) fprintf(stdout, "'%s' at line %d is installed in symbol table!\n", buffer, lineno);
            }

            // set the buffer to initial state
            memset(buffer, '\0', strlen(buffer));
        }
        else if( c == '>' ) {
            totalTokenCount += 1;

            // get next character
            c = fgetc(fp);
            if( c == '=') {
                buffer[0] = '>';
                buffer[1] = c;
            } else {
                // set pointer to previous postion by 1
                fseek(fp, -1L, 1);
                buffer[0] = '>';
            }
            printf("'%s', %d\n", buffer, 1);
            
            // Lookup and install into symbol table
            if((installTokens(buffer, "relational operators", RELOP, lineno))==0) {
                (void) fprintf(stdout, "'%s' at line %d is already installed in symbol table!\n", buffer, lineno);
                //(void) fprintf(stdout, "Error at line %d: failed to install %s in symbol table!\n", lineno, buffer);
            } else {
                (void) fprintf(stdout, "'%s' at line %d is installed in symbol table!\n", buffer, lineno);
            }

            // set the buffer to initial state
            memset(buffer, '\0', strlen(buffer));
        }
        else if( c == '!' ) {

            // get next character
            c = fgetc(fp);
            if( c == '=') {
                totalTokenCount += 1;
                buffer[0] = '!';
                buffer[1] = c;
                printf("'%s', %d\n", buffer, 1);
            
                // Lookup and install into symbol table
                if((installTokens(buffer, "relational operators", RELOP, lineno))==0) {
                    (void) fprintf(stdout, "'%s' at line %d is already installed in symbol table!\n", buffer, lineno);
                    //(void) fprintf(stdout, "Error at line %d: failed to install %s in symbol table!\n", lineno, buffer);
                } else {
                    (void) fprintf(stdout, "'%s' at line %d is installed in symbol table!\n", buffer, lineno);
                }

                // set the buffer to initial state
                memset(buffer, '\0', strlen(buffer));
            } else {
                // set pointer to previous postion by 1
                fseek(fp, -1L, 1);
                (void) fprintf(stderr, "Error at line %d, expected '%s' but got '%c'\n", lineno, "!=", '!');
                exit(EXIT_FAILURE);
            }
        }
        else {
            printf("Error: unrecognized character '%c' at line %d.\n", c, lineno);
        }

    }
    
    // Display the symbol table
    displayTokens(SymbolTable);

    // Display total no. of tokens
    printf("\n\ntotalTokenCount: %d\n", totalTokenCount);

    free(SymbolTable);

    return 0;
}

void printFormatting(char *string, char c) {
    char len = strlen(string) + 18; // 18 is added for padding purpose

    for(int i=0; i<len; i++)
        printf("%c", c);

    printf("\n%*.*s\n", (len-9), (len-18), string);

    for(int i=0; i<len; i++)
        printf("%c", c);
    printf("\n");
}

int installTokens(char *varName, char *tokenName, TokenType tokenType, int lineno) {
    
    /* First check for the token in the symbol table */
    if(lookup(varName) != LOOKUP) {
        /* Do nothing as token already exist */
        return 0;
    }
    else {

        // Allocate memory to a new struct of type struct Tokens
        struct Tokens *tok;
        if((tok = (struct Tokens *) malloc(sizeof(struct Tokens))) == NULL) {
            (void) fprintf(stderr, "Error: Memory Allocation Failed!\n");
            exit(EXIT_FAILURE);
        }

        // Allocate memory to varName
        tok->varName = (char *)malloc(sizeof(char)*strlen(varName));
        if(tok==NULL) {
            (void) fprintf(stderr, "Error: Memory allocation failed for varName!\n");
            exit(EXIT_FAILURE);
        }

        // Allocate memory to varName
        tok->tokenName = (char *)malloc(sizeof(char)*strlen(tokenName));
        if(tok==NULL) {
            (void) fprintf(stderr, "Error: Memory allocation failed for tokenName!\n");
            exit(EXIT_FAILURE);
        }
        
        // Copy data from varName to tok->varName
        strcpy(tok->varName, varName);

        // Copy data from varName to tok->varName
        strcpy(tok->tokenName, tokenName);
        
        // Copy data in tokenType to tok->tokenType
        tok->tokenType = tokenType;

        // Copy data in lineno to tok->lineno
        tok->lineno = lineno;
        
        if(SymbolTable == NULL) {
            tok->nextToken = NULL;
            SymbolTable = tok;
            holdRecentAddr = tok;
            return 1;
        }

        /* token is not there, link it on the list */
        tok->nextToken = holdRecentAddr->nextToken;
        holdRecentAddr->nextToken = tok;
        holdRecentAddr = tok;
    }
    
    return 1;
}

void loadSymbolTable(struct Tokens *Symtable) { // It will load the Symbol 
                                                // Table on startup
    int i;

    // install keywords
    int len = strlen(keywords);
    for(i = 0; i < len; i++ ) {
        if(installTokens(keywords[i], "keyword", KEYWORD, 0) == 1) {
            fprintf(stdout, "%s is installed in SymbolTable\n", keywords[i]);
        } else {
            fprintf(stdout, "Error: %s is not installed in SymbolTable\n", keywords[i]);
            exit(EXIT_FAILURE);
        }
    }

    // install operators
    len = strlen(relationalOperators);
    for(i = 0; i < len; i++ ) {
        if(installTokens(relationalOperators[i], "relational operator", RELOP, 0) == 1) {
            fprintf(stdout, "%s is installed in SymbolTable\n", relationalOperators[i]);
        } else {
            fprintf(stdout, "Error: %s is not installed in SymbolTable\n", keywords[i]);
            exit(EXIT_FAILURE);
        }
    }

    len = strlen(otherOperators);
    for(i = 0; i < len; i++ ) {
        if(otherOperators[i] == '=') {
            if(installTokens(&otherOperators[i], "arithmetic operator", OP, 0) == 1) {
            fprintf(stdout, "%c is installed in SymbolTable\n", otherOperators[i]);
            } else {
                fprintf(stdout, "Error: %c is not installed in SymbolTable\n", otherOperators[i]);
                exit(EXIT_FAILURE);
            }
            continue;
        } else {
            if(installTokens(&otherOperators[i], "arithmetic operator", OP, 0) == 1) {
            fprintf(stdout, "%c is installed in SymbolTable\n", otherOperators[i]);
            } else {
                fprintf(stdout, "Error: %c is not installed in SymbolTable\n", otherOperators[i]);
                exit(EXIT_FAILURE);
            }    
        }
    }

}

void displayTokens(struct Tokens *refTable) {
    if(refTable == NULL) {
        fprintf(stdout, "SymbolTable is null.\n");
        exit(EXIT_SUCCESS);
    }
    while(refTable!=NULL) {
        printf("< %s, %s, %d, %d >\n", refTable->varName, refTable->tokenName, refTable->tokenType, refTable->lineno);
        refTable = refTable->nextToken;
    }
}

int lookup(char *token) {
    struct Tokens *symPtr;
    symPtr = SymbolTable;
    
    for(; symPtr; symPtr = symPtr->nextToken) {
        if(strcmp(symPtr->varName, token) == 0) {
            return symPtr->tokenType;
        }
    }
    
    return LOOKUP;	/* Token not found, undeclared. */
}
