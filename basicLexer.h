//
//  basicLexer.h
//  basicLexer
//
//  Created by Anurag Gupta on 03/02/17.
//  Copyright © 2017 Anurag Gupta. All rights reserved.
//

#ifndef basicLexer_h
#define basicLexer_h

typedef enum {
    LOOKUP,
    INTERGER,
    CHAR,
    IDENTIFIER,
    LITERALS,
    KEYWORD,
    FOR,
    IF,
    ELSE,
    NUMBER,
    RELOP, 		// such as ==  <  >  =!=    =>  =<
    OP,			// such as = :  +  -  *  / %
    DELIM,		// such as . (  ) , { } ; [ ]
    UNDEF,		// undefined
    EOT 		// end of token
} TokenType;

// Token Structure in symbol table
struct Tokens {
    char *varName;
    char *tokenName;
    TokenType tokenType;
    int lineno;
    struct Tokens *nextToken;
};

// Symbol table
struct Tokens *SymbolTable=NULL;

// Functions to be used.
int installTokens(char *, char *, TokenType, int);     // To install tokens into the symbol table
void displayTokens(struct Tokens *);                   // Display tokens
void loadSymbolTable(struct Tokens *);
//void displaySymbolTable(void);                       // Display Symbol Table
int lookup(char *);                                    // Searching tokens in symbol table

// Arrays to hold information for initially into symbol table
char *keywords[5] = {"int", "char", "for", "if", "else"};
char *relationalOperators[6] = {"==", "<", ">", "!=", "=>", "=<"};
char otherOperators[6] = {'=', '+', '-', '*', '/', '%'};
char delimiters[12] = {'.', '(', ')', ',', '{', '}', ';', '"', '\''};

// Variables
int totalTokenCount;
int whiteSpaces;
int lineno;

#endif /* basicLexer_h */
