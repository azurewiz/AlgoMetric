#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- COMPILER DESIGN: TOKEN TYPES ---
// We define an Enum to represent the "category" of each word
typedef enum {
    TOKEN_ENSURE,     // "ENSURE"
    TOKEN_LIMIT,      // "LIMIT"
    TOKEN_ATTRIBUTE,  // "TIME_COMPLEXITY", "LOOPS", etc.
    TOKEN_OPERATOR,   // "<", ">", "="
    TOKEN_VALUE,      // "O(N)", "TRUE", numbers
    TOKEN_END,        // End of input
    TOKEN_UNKNOWN     // Syntax Error
} TokenType;

// A structure to hold the token data
typedef struct {
    TokenType type;
    char text[100];
} Token;

// --- LEXICAL ANALYZER FUNCTION ---
// Concept: Finite Automata. We read char by char to decide the state.
void get_next_token(const char* input, int* pos, Token* outToken) {
    int i = 0;
    char current_char = input[*pos];

    // 1. Skip Whitespace (Spaces/Tabs are ignored in our grammar)
    while (current_char == ' ' || current_char == '\t' || current_char == '\n') {
        (*pos)++;
        current_char = input[*pos];
    }

    // 2. Check for End of String
    if (current_char == '\0') {
        outToken->type = TOKEN_END;
        strcpy(outToken->text, "EOF");
        return;
    }

    // 3. Check for Operators (<, >, =)
    if (current_char == '<' || current_char == '>' || current_char == '=') {
        outToken->type = TOKEN_OPERATOR;
        outToken->text[0] = current_char;
        outToken->text[1] = '\0';
        (*pos)++;
        return;
    }

    // 4. Check for Alphanumeric words (Keywords, Attributes, Values)
    if (isalnum(current_char) || current_char == '_' || current_char == '(' || current_char == ')' || current_char == '^') {
        while (isalnum(input[*pos]) || input[*pos] == '_' || input[*pos] == '(' || input[*pos] == ')' || input[*pos] == '^') {
            outToken->text[i++] = input[*pos];
            (*pos)++;
        }
        outToken->text[i] = '\0';

        // CLASSIFY THE WORD (Symbol Table Lookup equivalent)
        if (strcmp(outToken->text, "ENSURE") == 0) {
            outToken->type = TOKEN_ENSURE;
        } else if (strcmp(outToken->text, "LIMIT") == 0) {
            outToken->type = TOKEN_LIMIT;
        } else if (strstr(outToken->text, "COMPLEXITY") != NULL || strcmp(outToken->text, "LOOPS") == 0) {
            outToken->type = TOKEN_ATTRIBUTE;
        } else {
            // If it's not a keyword, we assume it's a Value (like O(N) or 50)
            outToken->type = TOKEN_VALUE;
        }
        return;
    }

    // 5. Unknown Character (Syntax Error)
    outToken->type = TOKEN_UNKNOWN;
    outToken->text[0] = current_char;
    outToken->text[1] = '\0';
    (*pos)++;
}

// --- MAIN DRIVER ---
int main(int argc, char* argv[]) {
    // Input: For now, we hardcode. Later, this comes from command line args.
    const char* input_code = "ENSURE TIME_COMPLEXITY < O(N^2)";
    
    // If command line arg is provided, use that instead
    if (argc > 1) {
        input_code = argv[1];
    }

    printf("--- AlgoMetric Lexer Debugger ---\n");
    printf("Analyzing: %s\n\n", input_code);

    int pos = 0;
    Token token;

    // Loop until we finish the string
    do {
        get_next_token(input_code, &pos, &token);
        
        printf("Token: [Type: %d] | Text: '%s'\n", token.type, token.text);

    } while (token.type != TOKEN_END && token.type != TOKEN_UNKNOWN);

    return 0;
}