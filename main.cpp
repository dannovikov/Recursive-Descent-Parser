/* File: scanner.cpp
 * Name: Daniel Novikov
 * Course: CS 3500 Software Engineering
 * This program scans an input stream and parses it character by character,
 * recognizing the tokens that the characters correspond to.
 */

#include <iostream>
#include <iomanip>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

using namespace std;

//global items:

//enum of keywords:
enum tokenType { AND, BEGIN, END, FOR, IF, NOT, OR, READ, WHILE, WRITE, COMMENT, ID, REAL, STRING, PLUS, MULTIPLICATION, ASSIGNMENT, EQUAL, GREATERTHAN, LESSTHAN, LEFTP, COMMA, RIGHTP, SEMICOLON, INVALID, DIVISION, INTEGER };

//array of messages:
static char message[][20] = { "and", "begin", "end", "for", "if", "not", "or", "read", "while", "write", "comment", "identifer", "real", "string", "plus", "multiplication", "assignment", "equal", "greater than", "less than", "left parenthesis", "comma", "right parenthesis", "semicolon", "invalid", "division", "integer" };

//token buffer
const int bufferSize = 80;
char tokenBuffer[bufferSize];

//specifies whether or not the next input token is needed
bool needToken = true;

//prototypes:
void skipSpaces();
void clearBuffer();
void displayToken(tokenType code);
tokenType getID();
tokenType getComment();
tokenType getReal();
tokenType getStrings();
tokenType getPlus();
tokenType getMul();
tokenType getAssign();
tokenType getEqual();
tokenType getGreater();
tokenType getLess();
tokenType getLP();
tokenType getRP();
tokenType getComma();
tokenType getSColon();
tokenType lexical_error();
tokenType scanner();
void program();
int getnexttoken();
void stmntlist();
void stmnt();
int peektoken();
int readtoken();
void syntax_error(int);
void match(int);
void stmnttail();
void expr();
void exprtail();
void factor();
void factortail();
void primary();
void writestmnt();
void parameter();
void exprlist();
void exprlisttail();
void readstmnt();
void idlist();
void idtail();


int main() {
    
    program();
    return 0;
}

void skipSpaces() {
    int ch;
    ch = cin.get();
    while (isspace(ch))
        ch = cin.get();
    cin.putback(ch);
}

void clearBuffer() {
    for (int i = 0; i < bufferSize; i++)
        tokenBuffer[i] = '\0';
}

void displayToken(tokenType code) {
    cout << setw(30) << left << message[code];
    for (int i = 0; i < bufferSize; i++) {
        if (tokenBuffer[i] != '\0')
            cout << tokenBuffer[i];
        else
            i = bufferSize;    //terminate loop
    }
    cout << endl;
}

tokenType getID ()
{
    static char reservedWord[ ][10] = {"AND", "BEGIN", "END", "FOR", "IF", "NOT",
        "OR", "READ", "WHILE", "WRITE"};
    tokenType code;
    int   ch, i = 0;
    ch  =  cin.get(  );
    if( isalpha( ch ))
    {
        tokenBuffer[ i++ ]  = ch;
        ch  =  cin.get(  );
        while( isalnum( ch ))
        {
            tokenBuffer[ i++ ]  = ch;
            ch  =  cin.get(  );
        }
        cin.putback( ch );
        
        /*---- find out if it is a reserved word using the binary search--*/
        int first = 0,
        mid = 4,
        last  =  9;
        bool  notFound  = true;
        while( first <=  last  && notFound )
        {
            mid   =  (first + last) / 2;
            int  answer =  strcmp( tokenBuffer, reservedWord[ mid ] );
            if(  answer == 0 )
                notFound = false;
            else if ( answer > 0 )
                first = mid + 1;
            else
                last  =  mid - 1;
        }
        if(notFound)
            code  =  ID;
        else
            code  = (tokenType) mid;
    }
    else      // it is not a letter
    {
        cin.putback( ch );
        code  =  INVALID;
    }
    return( code );
}

tokenType getComment()
{
    tokenType code;
    int   ch, i = 0;
    ch  =  cin.get(  );
    if(  ch == '/'  &&  cin.peek( ) == '*')
    {
        tokenBuffer[ i++ ]  = ch;        // store the first symbol into the buffer
        
        // read the second symbol and store it into the buffer
        ch  =  cin.get(  );
        tokenBuffer[ i++ ]  = ch;
        
        ch  = cin.get(  );
        while( !( ch ==  '*'  &&  cin.peek( ) == '/' )  &&   cin.peek( ) != EOF)
        {
            tokenBuffer[ i++ ]  = ch;        // store the first symbol into the buffer
            
            // read the second symbol and store it into the buffer
            ch  =  cin.get(  );
        }
        if(cin.peek( ) == EOF)
            code  =  INVALID;
        else
        {
            tokenBuffer[ i++ ]  = ch;        // store the first symbol into the buffer
            
            // read the second symbol and store it into the buffer
            ch  =  cin.get(  );
            tokenBuffer[ i++ ]  = ch;
            code  =  COMMENT;
        }
    }
    else            // it is not “/*”
    {
        cin.putback( ch );
        code  =  INVALID;
    }
    return( code );
}


tokenType getReal()
{
    tokenType code;
    int   ch, i = 0;
    ch  =  cin.get(  );
    if( isdigit( ch ))
    {
        tokenBuffer[ i++ ]  = ch;
        ch  =  cin.get(  );
        while( isdigit( ch ))
        {
            tokenBuffer[ i++ ]  = ch;
            ch  =  cin.get(  );
        }
        if(ch == '.' )
        {
            tokenBuffer[ i++ ]  = ch;
            ch  =  cin.get( );
            if (isdigit(ch))
            {
                tokenBuffer[ i++ ]  = ch;
                ch  =  cin.get(  );
                while( isdigit( ch ))
                {
                    tokenBuffer[ i++ ]  = ch;
                    ch  =  cin.get(  );
                }
                cin.putback( ch );
                code  =  REAL;
            }
            else
            {
                
                cin.putback( ch );
                code  =  INVALID;
            }
        }
        else      // it is not a period
        {
            cin.putback( ch );
            code  =  INVALID;
        }
    }
    else      // it is not a digit
    {
        cin.putback( ch );
        code  =  INVALID;
    }
    return( code );
}


tokenType getStrings()
{
    tokenType code;
    int ch, i = 0;
    ch = cin.get();
    if( ch == '\"' )
    {
        tokenBuffer[ i++ ]  = ch;        // store the first symbol into the buffer
        
        ch  = cin.get(  );
        while( !( ch ==  '\"' ) &&  ch != EOF)
        {
            tokenBuffer[ i++ ]  = ch;        // store the symbol into the buffer
            
            // read the second symbol and store it into the buffer
            ch  =  cin.get(  );
        }
        if(ch == EOF)
            code  =  INVALID;
        else
        {
            tokenBuffer[ i++ ]  = ch;        // store the first symbol into the buffer
            code  =  STRING;
        }
    }
    else            // it is not "
    {
        cin.putback( ch );
        code  =  INVALID;
    }
    return( code );
}

tokenType getPlus( )
{
    tokenType code;
    int   ch, i = 0;
    ch  =  cin.get(  );
    if( ch == '+')
    {
        tokenBuffer[ i++ ]  = ch;        // store it into the buffer
        code = PLUS;
    }
    else            // it is not the operator +
    {
        cin.putback( ch );
        code  =  INVALID;
    }
    return( code );
}

tokenType getMul( )
{
    tokenType code;
    int ch, i = 0;
    ch = cin.get();
    if(ch == '*')
    {
        tokenBuffer[i++]  = ch;        // store it into the buffer
        code = MULTIPLICATION;
    }
    else            // it is not the operator *
    {
        cin.putback(ch);
        code  =  INVALID;
    }
    return(code);
}

tokenType getAssign( )
{
    tokenType code;
    int   ch, i = 0;
    ch  =  cin.get(  );
    if( ch == ':' && cin.peek() == '=')
    {
        tokenBuffer[ i++ ]  = ch;        // store the first symbol into the buffer
        
        // read the second symbol and store it into the buffer
        ch  =  cin.get(  );
        tokenBuffer[ i ]  = ch;
        code =  ASSIGNMENT;
    }
    else
    {            // it is not an assignment
        cin.putback(ch);
        code  =  INVALID;
    }
    return(code);
}


tokenType getEqual()
{
    tokenType code;
    int ch, i = 0;
    ch = cin.get();
    if(ch == '=')
    {
        tokenBuffer[i++]  = ch;        // store it into the buffer
        code = EQUAL;
    }
    else            // it is not the operator =
    {
        cin.putback(ch);
        code  =  INVALID;
    }
    return(code);
}

tokenType getGreater()
{
    tokenType code;
    int ch, i = 0;
    ch = cin.get();
    if(ch == '>')
    {
        tokenBuffer[i++]  = ch;        // store it into the buffer
        code = GREATERTHAN;
    }
    else            // it is not the operator >
    {
        cin.putback(ch);
        code  =  INVALID;
    }
    return(code);
}

tokenType getLess()
{
    tokenType code;
    int ch, i = 0;
    ch = cin.get();
    if(ch == '<')
    {
        tokenBuffer[i++]  = ch;        // store it into the buffer
        code = LESSTHAN;
    }
    else            // it is not the operator <
    {
        cin.putback(ch);
        code  =  INVALID;
    }
    return(code);
}

tokenType getLP()
{
    tokenType code;
    int ch, i = 0;
    ch = cin.get();
    if(ch == '(')
    {
        tokenBuffer[i++]  = ch;        // store it into the buffer
        code = LEFTP;
    }
    else            // it is not the operator (
    {
        cin.putback(ch);
        code  =  INVALID;
    }
    return(code);
}

tokenType getRP()
{
    tokenType code;
    int ch, i = 0;
    ch = cin.get();
    if(ch == ')')
    {
        tokenBuffer[i++]  = ch;        // store it into the buffer
        code = RIGHTP;
    }
    else            // it is not the operator )
    {
        cin.putback(ch);
        code  =  INVALID;
    }
    return(code);
}

tokenType getComma()
{
    tokenType code;
    int ch, i = 0;
    ch = cin.get();
    if(ch == ',')
    {
        tokenBuffer[i++]  = ch;        // store it into the buffer
        code = COMMA;
    }
    else
    {
        cin.putback(ch);
        code  =  INVALID;
    }
    return(code);
}

tokenType getSColon()
{
    tokenType code;
    int ch, i = 0;
    ch = cin.get();
    if(ch == ';')
    {
        tokenBuffer[i++]  = ch;        // store it into the buffer
        code = SEMICOLON;
    }
    else
    {
        cin.putback(ch);
        code  =  INVALID;
    }
    return(code);
}

tokenType lexical_error()
{
    int ch, i = 0;
    ch = cin.get();
    tokenBuffer[i++]  = ch;        // store it into the buffer
    return (INVALID);
}

tokenType scanner()
{
    int character;
    tokenType code;
    
    skipSpaces();
    
    character = cin.get();
    
    if(character==EOF)
    {
        code = (tokenType)EOF;
    } else {
        cin.putback(character);
        
        //recognize identifier
        if(isalpha(cin.peek())) {
            code = getID();
        }
        
        //recognize comment
        else if (cin.peek() == '/') {
            code = getComment();
        }
        
        //recognize real
        else if(isdigit(cin.peek())) {
            code = getReal();
        }
        
        //recognize string
        else if(cin.peek() == '\"') {
            code = getStrings();
        }
        
        //recognize plus
        else if (cin.peek() == '+') {
            code = getPlus();
        }
        
        //recognize multiplication
        else if(cin.peek() == '*') {
            code = getMul();
        }
        
        //recognize assignment
        else if(cin.peek() == ':') {
            int ch = cin.get();
            if(cin.peek() == '=') {
                cin.putback(ch);
                code = getAssign();
            } else {
                code = 
                cin.putback(ch);
            }
        }
        
        //recognize equal
        else if(cin.peek() == '=') {
            code = getEqual();
        }
        
        //recognize greaterThan
        else if(cin.peek() == '>') {
            code = getGreater();
        }
        
        //recognize lessThan
        else if(cin.peek() == '>') {
            code = getLess();
        }
        
        //recognize left parenthesis
        else if(cin.peek() == '(') {
            code = getLP();
        }
        
        //recognize right parenthesis
        else if(cin.peek() == ')') {
            code = getRP();
        }
        
        //recognize comma
        else if(cin.peek() == ',') {
            code = getComma();
        }
        
        //recognize semicolon
        else if(cin.peek() == ';') {
            code = getSColon();
        }
        
        //if not recognized character
        else {
            code = lexical_error();
        }
    }
    return code;
}

void match(int token) {
    int nextInputToken = readtoken();
    
    if(nextInputToken == (int)token) {
        displayToken((tokenType)nextInputToken);
        if (nextInputToken == (int)SEMICOLON) {
            skipSpaces();
        }
    } else {
        syntax_error(token);
    }
    
    clearBuffer();
}

void syntax_error(int token) {
    cout << "Error in SYNTAX for " << message[token] << endl;
}

void program(void)
{
    match(BEGIN);
    stmntlist();
    match(END);
}

void stmntlist(void)
{
    stmnt();
    stmnttail();
}

void stmnttail() {
    int nexttoken;
    nexttoken = peektoken();
    switch(nexttoken)
    {
        case ID:
            stmnt();
            stmnttail();
        case END:
            return;
        default:
            syntax_error(nexttoken);
    }
}

void stmnt(void)
{
    int nexttoken;
    nexttoken = peektoken();
    switch(nexttoken)
    {
        case ID:
            match(ID);
            match(ASSIGNMENT);
            expr();
            match(SEMICOLON);
            break;
        case WRITE:
            match(WRITE);
            match(LEFTP);
            parameter();
            match(RIGHTP);
            match(SEMICOLON);
            break;
        case READ:
            match(READ);
            match(LEFTP);
            idlist();
            match(RIGHTP);
            match(SEMICOLON);
            break;
        default:
            syntax_error(nexttoken);
    }
}

void expr() {
    factor();
    exprtail();
}

void exprtail() {
    int nexttoken;
    nexttoken = peektoken();
    switch(nexttoken)
    {
        case PLUS:
            match(PLUS);
            factor();
            exprtail();
            break;
        case (SEMICOLON || RIGHTP):
            return;
        default:
            syntax_error(nexttoken);
    }
}

void factor() {
    primary();
    factortail();
}

void factortail() {
    int nexttoken;
    nexttoken = peektoken();
    switch(nexttoken)
    {
        case MULTIPLICATION:
            match(MULTIPLICATION);
            primary();
            factortail();
            break;
        case DIVISION:
            match(DIVISION);
            primary();
            factortail();
            break;
        case (RIGHTP || SEMICOLON || PLUS):
            return;
        default:
            syntax_error(nexttoken);
    }
}

void primary() {
    int nexttoken;
    nexttoken = peektoken();
    switch(nexttoken)
    {
        case (LEFTP):
            match(LEFTP);
            expr();
            match(RIGHTP);
            break;
        case ID:
            match(ID);
            break;
        case REAL:
            match(REAL);
            break;
        case INTEGER:
            match(INTEGER);
            break;
        default:
            syntax_error(nexttoken);
    }
}

void parameter() {
    int nexttoken;
    nexttoken = peektoken();
    switch(nexttoken)
    {
        case STRING:
            match(STRING);
            break;
        case (ID || REAL || INTEGER):
            exprlist();
            break;
        default:
            syntax_error(nexttoken);
    }
}

void exprlist() {
    int nexttoken;
    nexttoken = peektoken();
    switch(nexttoken)
    {
        case (ID || REAL || INTEGER):
            expr();
            exprlisttail();
            break;
        default:
            syntax_error(nexttoken);
    }
}

void exprlisttail() {
    int nexttoken;
    nexttoken = peektoken();
    switch(nexttoken)
    {
        case COMMA:
            match(COMMA);
            expr();
            exprlisttail();
            break;
        case RIGHTP:
            return;
        default:
            syntax_error(nexttoken);
    }
}

void idlist() {
    match(ID);
    idtail();
}

void idtail() {
    int nexttoken;
    nexttoken = peektoken();
    switch (nexttoken) {
        case COMMA:
            match(COMMA);
            match(ID);
            idtail();
            break;
        case RIGHTP:
            return;
        default:
            syntax_error(nexttoken);
    }
}

int getnexttoken() {
    static int nexttoken;
    if(needToken) {
        nexttoken = (int)scanner();
        needToken = false;
    }
    
    return nexttoken;
}

int peektoken() {
    return getnexttoken();
}

int readtoken() {
    int token = getnexttoken();
    needToken = true;
    return token;
}

void writestmnt() {
    match(WRITE);
    match(LEFTP);
    parameter();
    match(RIGHTP);
    match(SEMICOLON);
}

void readstmnt() {
    match(READ);
    match(LEFTP);
    idlist();
    match(RIGHTP);
    match(SEMICOLON);
}
