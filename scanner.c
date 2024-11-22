#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 100
#define MAX_TABLE_SIZE 10

typedef enum
{    IDENTIFIER,   NUMBER,OPERATOR,SPECIAL,KEYWORD,END_OF_FILE, UNKNOWN    } TokenType;
typedef struct Token
{
    char value[MAX_TOKEN_LENGTH];
    TokenType type;
    struct Token *next;
} Token;
// Function prototypes
Token *createToken(const char *value, TokenType type);
void addToken(Token **head, const char *value, TokenType type);
void printTokens(Token *head);
void freeTokens(Token *head);
TokenType identifyToken(const char *value);
int isOperator(char ch);
int isSpecialCharacter(char ch);
int isKeyword(const char *value);
// Scanner implementation
int main()
{
    FILE *file;
    char ch;
    char token[MAX_TOKEN_LENGTH];
    int tokenIndex = 0;
    Token *head = NULL;
    file = fopen("input.txt", "r");
    if (file == NULL)
    { printf("Error opening file\n");   return 1;  }
    while ((ch = fgetc(file)) != EOF)
    {  if (isspace(ch))
        {  if (tokenIndex > 0)
            {  token[tokenIndex] = '\0';
                addToken(&head, token, identifyToken(token));
                tokenIndex = 0;  }
        }
        else if (isOperator(ch) || isSpecialCharacter(ch))
        {
            if (tokenIndex > 0)
            {
                token[tokenIndex] = '\0';
                addToken(&head, token, identifyToken(token));
                tokenIndex = 0;
            }
            token[0] = ch;
            token[1] = '\0';
            addToken(&head, token, isOperator(ch) ? OPERATOR : SPECIAL);
        }
        else
        {   token[tokenIndex++] = ch;}
    }
    if (tokenIndex > 0)
    {
        token[tokenIndex] = '\0';
        addToken(&head, token, identifyToken(token));
    }
    addToken(&head, "", END_OF_FILE); // End of input
    fclose(file);
    printTokens(head);
    freeTokens(head);
    return 0;
}
Token *createToken(const char *value, TokenType type)
{
    Token *newToken = (Token *)malloc(sizeof(Token));
    if (!newToken)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    strcpy(newToken->value, value);
    newToken->type = type;
    newToken->next = NULL;
    return newToken;
}
void addToken(Token **head, const char *value, TokenType type)
{
    Token *newToken = createToken(value, type);
    if (*head == NULL)
    {
        *head = newToken;
    }
    else
    {
        Token *temp = *head;
        while (temp->next)
        {
            temp = temp->next;
        }
        temp->next = newToken;
    }
}
void printTokens(Token *head)
{
    Token *temp = head;
    while (temp)
    {
        printf("Token: %s, Type: %d\n", temp->value, temp->type);
        temp = temp->next;
    }
}
void freeTokens(Token *head)
{
    Token *temp;
    while (head)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}
// here we identify the types of tokens
TokenType identifyToken(const char *value)
{
    if (isKeyword(value))
    {
        return KEYWORD;
    }
    else if (isdigit(value[0]))
    {
        for (int i = 1; i < strlen(value); ++i)
        {
            if (!isdigit(value[i]))
            {
                return UNKNOWN;
            }
        }
        return NUMBER;
    }
    else if (isalpha(value[0]) || value[0] == '_')
    {
        for (int i = 1; i < strlen(value); ++i)
        {
            if (!isalnum(value[i]) && value[i] != '_')
            {
                return UNKNOWN;
            }
        }
        return IDENTIFIER;
    }
    else
    {
        return UNKNOWN;
    }
}

int isOperator(char ch)
{
    char operators[] = "+-*/%<=>";
    for (int i = 0; i < strlen(operators); ++i)
    {
        if (ch == operators[i])
        {
            return 1;
        }
    }
    return 0;
}
int isSpecialCharacter(char ch)
{
    char specialCharacters[] = "(){};:,'\"";
    for (int i = 0; i < strlen(specialCharacters); ++i)
    {
        if (ch == specialCharacters[i])
        {
            return 1;
        }
    }
    return 0;
}
int isKeyword(const char *value)
{
    const char *keywords[] = {"if", "else", NULL};
    for (int i = 0; keywords[i] != NULL; ++i)
    {
        if (strcmp(value, keywords[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}
