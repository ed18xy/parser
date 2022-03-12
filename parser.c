// Elya Denysova
// 6667596
// ed18xy
// Assignment 2
/* front.in - a text file needs to be included  in the same folder */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
/* Global declarations */
/* Variables */
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp, *fopen();
/* Function declarations */
void addChar();
void getChar();
void ifstmt();
void getNonBlank();
int lex();
void expr();
void term();
void factor();
void program();
void stmt();
void stmts();
void assign();
void id();
void error();
void whilestmt();
void forstmt();
/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define LEFT_B 27
#define RIGHT_B 28
#define IF_CODE 31
#define SEMICOLON 40
#define FOR_CODE 41
#define WHILE_CODE 42
/******************************************************/
/* main driver */
int main()
{
    /* Open the input data file and process its contents */
    if ((in_fp = fopen("front.in", "r")) == NULL)
        printf("ERROR - cannot open front.in \n");
    else
    {
        getChar();
        lex();
        program();
    }
    printf("\n");
}
/*****************************************************/
/* lookup - a function to lookup operators and parentheses
and return the token */
int lookup(char ch)
{
    switch (ch)
    {
    case '(':
        addChar();
        nextToken = LEFT_PAREN;
        break;
    case ')':
        addChar();
        nextToken = RIGHT_PAREN;
        break;
    case '+':
        addChar();
        nextToken = ADD_OP;
        break;
    case '-':
        addChar();
        nextToken = SUB_OP;
        break;
    case '*':
        addChar();
        nextToken = MULT_OP;
        break;
    case '/':
        addChar();
        nextToken = DIV_OP;
        break;
    case ';':
        addChar();
        nextToken = SEMICOLON;
        break;
    case '=':
        addChar();
        nextToken = ASSIGN_OP;
        break;
    case '{':
        addChar();
        nextToken = LEFT_B;
        break;
    case '}':
        addChar();
        nextToken = RIGHT_B;
        break;
    default:
        addChar();
        nextToken = EOF;
        break;
    }
    return nextToken;
}
/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar()
{
    if (lexLen <= 98)
    {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
    else
        printf("Error - lexeme is too long \n");
}
/*****************************************************/
/* getChar - a function to get the next character of
input and determine its character class */
void getChar()
{
    if ((nextChar = getc(in_fp)) != EOF) // missed "!"?
    {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    }
    else
        charClass = EOF;
}
/*****************************************************/
/* getNonBlank - a function to call getChar until it
returns a non-whitespace character */
void getNonBlank()
{
    while (isspace(nextChar))
        getChar();
}
/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic
 * expressions */
int lex()
{
    lexLen = 0;
    getNonBlank();
    switch (charClass)
    {
        /* Parse identifiers */
    case LETTER:
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT)
        {
            addChar();
            getChar();
        }
        nextToken = IDENT;
        break;
        /* Parse integer literals */
    case DIGIT:
        addChar();
        getChar();
        while (charClass == DIGIT)
        {
            addChar();
            getChar();
        }
        nextToken = INT_LIT;
        break;
        /* Parentheses and operators */
    case UNKNOWN:
        lookup(nextChar);
        getChar();
        break;
        /* EOF */
    case EOF:
        nextToken = EOF;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        lexeme[3] = 0;
        break;
    }
    if (lexeme[0] == 'i' && lexeme[1] == 'f')
        nextToken = IF_CODE;
    if (lexeme[0] == 'f' && lexeme[1] == 'o' && lexeme[2] == 'r')
        nextToken = FOR_CODE;
    if (lexeme[0] == 'w' && lexeme[1] == 'h' && lexeme[2] == 'i' && lexeme[3] == 'l' && lexeme[4] == 'e')
        nextToken = WHILE_CODE;
    /* End of switch */
    // if(nextToken != EOF)printf("%s", lexeme);
    return nextToken;
} /* End of function lex */

// program
//<program> → <stmts>
void program()
{
    printf("[<program>");
    stmts();
    printf("]");
}

// statements
//<stmts> → <stmt> [; <stmts>]
void stmts()
{
    printf("[<stmts>");
    stmt();
    while (nextToken == SEMICOLON)
    {
        if (nextToken != EOF)
            printf("%s", lexeme);
        lex();
        stmts();
    }
    printf("]");
}

//<stmt> → <assign> | <if> | <for> | <while> |
void stmt()
{
    printf("[<stmt>");
    if (nextToken == IF_CODE)
        ifstmt();
    else if (nextToken == FOR_CODE)
        forstmt();
    else if (nextToken == WHILE_CODE)
        whilestmt();
    else if (nextToken == IDENT)
        assign();
    else
        error();

    printf("]");
}

// assign
//<assign> → id = <expr>
void assign()
{
    printf("[<assign>");
    id();
    // check for assign operator
    if (nextToken == ASSIGN_OP)
    {
        if (nextToken != EOF)
            printf("%s", lexeme);
        /* Get the next token */
        lex();
    }
    else
        error();
    expr();
    printf("]");
}

//<id> → <letter> {<letter> | <digit>}
void id()
{
    printf("[<id>");
    // check for letter
    if (nextToken == IDENT)
    {
        if (nextToken != EOF)
            printf("%s", lexeme);
        /* Get the next token */
        lex();
    }
    // if it isn't letter, error
    else
        error();
    printf("]");
}

/* expr
Parses strings in the language generated by the rule:
<expr> -> <term> {(+ | -) <term>}    */
void expr()
{
    printf("[<expr>");
    /* Parse the first term */
    term();
    /* As long as the next token is + or -, get
    the next token and parse the next term */
    while (nextToken == ADD_OP || nextToken == SUB_OP)
    {
        if (nextToken != EOF)
            printf("%s", lexeme);
        lex();
        term();
    }
    printf("]");
} /* End of function expr */

/* term
Parses strings in the language generated by the rule:
<term> -> <factor> {(* | /) <factor>)
 */
void term()
{
    printf("[<term>"); /* Parse the first factor */
    factor();          /* As long as the next token is * or /, get the    next token and parse the next factor */
    while (nextToken == MULT_OP || nextToken == DIV_OP)
    {
        if (nextToken != EOF)
            printf("%s", lexeme);
        lex();
        factor();
    }
    printf("]");
} /* End of function term */
void error()
{
    printf("ERROR!\n");
}
/* factor    Parses strings in the language generated by the rule:
<factor> -> id | int_constant | ( <expr )    */
void factor()
{
    printf("[<factor>"); /* Determine which RHS */
    if (nextToken == IDENT || nextToken == INT_LIT)
    {
        id();
    }
    /* If the RHS is ( <expr> ), call lex to pass over the
    left parenthesis, call expr, and check for the right    parenthesis */
    else
    {
        if (nextToken == LEFT_PAREN)
        {
            if (nextToken != EOF)
                printf("%s", lexeme);
            lex();
            expr();
            if (nextToken == RIGHT_PAREN)
            {
                if (nextToken != EOF)
                    printf("%s", lexeme);
                lex();
            }
            else
                error();
        } /* End of if (nextToken == ... */ /* It was not an id, an integer literal, or a left    parenthesis */
        else
            error();
    } /* End of else */
    printf("]");
} /* End of function factor */

/* Function ifstmt
Parses strings in the language generated by the rule:
<ifstmt> -> if (<boolexpr>)  <statement>
[else <statement>]     */
void ifstmt()
{
    printf("[<if>");
    if (nextToken != EOF)
        printf("%s", lexeme);
    lex();
    if (nextToken != LEFT_PAREN)
        error();
    else
    {
        if (nextToken != EOF)
            printf("%s", lexeme);
        lex();
        expr();
        if (nextToken != RIGHT_PAREN)
            error();
        else
        { /* Parse the then clause */
            if (nextToken != EOF)
                printf("%s", lexeme);
            lex();
            // many
            if (nextToken == LEFT_B)
            {
                if (nextToken != EOF)
                    printf("%s", lexeme);
                lex();
                stmts();
                if (nextToken != RIGHT_B)
                    error();
                else
                {
                    if (nextToken != EOF)
                        printf("%s", lexeme);
                    lex();
                }
            }
            else
                // one
                stmt();
        }
    }
    printf("]");
} /* end of ifstmt */
//<for> → for '(' <assign> ; <expr> ; <assign> ')' ' ( '{' <stmts> '}' | <stmt>)
void forstmt()
{
    printf("[<for>");
    if (nextToken != EOF)
        printf("%s", lexeme);
    lex();
    if (nextToken != LEFT_PAREN)
        error();
    else
    {
        if (nextToken != EOF)
        printf("%s", lexeme);
        lex();
        assign();
        if (nextToken != SEMICOLON) error();
        else{
            if (nextToken != EOF)
                printf("%s", lexeme);
            lex();
            expr();
            if (nextToken != SEMICOLON)error();
            else{
                if (nextToken != EOF)
                    printf("%s", lexeme);
                lex();
                assign();
            }if (nextToken != RIGHT_PAREN)error();
            else{
                if (nextToken != EOF)
                    printf("%s", lexeme);
                lex();
                // many
            if (nextToken == LEFT_B)
            {
                if (nextToken != EOF)
                    printf("%s", lexeme);
                lex();
                stmts();
                if (nextToken != RIGHT_B)
                    error();
                else
                {
                    if (nextToken != EOF)
                        printf("%s", lexeme);
                    lex();
                }
            }
            else
                // one
                stmt();
            }
        }
    }

    printf("]");
}
//<while> → while '(' <expr> ')' ( '{' <stmts> '}' | <stmt>)
void whilestmt()
{
    printf("[<while>");
    if (nextToken != EOF)
        printf("%s", lexeme);
    lex();
    if (nextToken != LEFT_PAREN)
        error();
    else
    {
        if (nextToken != EOF)
            printf("%s", lexeme);
        lex();
        expr();
        if (nextToken != RIGHT_PAREN)
            error();
        else
        { /* Parse the then clause */
            if (nextToken != EOF)
                printf("%s", lexeme);
            lex();
            // many
            if (nextToken == LEFT_B)
            {
                if (nextToken != EOF)
                    printf("%s", lexeme);
                lex();
                stmts();
                if (nextToken != RIGHT_B)
                    error();
                else
                {
                    if (nextToken != EOF)
                        printf("%s", lexeme);
                    lex();
                }
            }
            else
                // one
                stmt();
        }
    }
    printf("]");
}