#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- 1. DEFINITIONS ---
typedef enum {
    TOKEN_ENSURE, TOKEN_LIMIT,      // Commands
    TOKEN_ATTRIBUTE,                // Attributes (Time, Space)
    TOKEN_OPERATOR,                 // <, >, =
    TOKEN_VALUE,                    // O(N), O(1)
    TOKEN_END, TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char text[100];
} Token;

// We need a global position tracker for the parser to "eat" tokens
int current_pos = 0;
char* source_code;
Token current_token;

// --- 2. LEXER (Reused from previous step) ---
void get_next_token() {
    int i = 0;
    char current_char = source_code[current_pos];

    // Skip whitespace
    while (current_char == ' ' || current_char == '\t' || current_char == '\n') {
        current_pos++;
        current_char = source_code[current_pos];
    }

    if (current_char == '\0') {
        current_token.type = TOKEN_END;
        return;
    }

    // Operators
    if (current_char == '<' || current_char == '>' || current_char == '=') {
        current_token.type = TOKEN_OPERATOR;
        current_token.text[0] = current_char;
        current_token.text[1] = '\0';
        current_pos++;
        return;
    }

    // Alphanumeric words
    if (isalnum(current_char) || current_char == '_' || current_char == '(' || current_char == ')' || current_char == '^') {
        while (isalnum(source_code[current_pos]) || source_code[current_pos] == '_' || source_code[current_pos] == '(' || source_code[current_pos] == ')' || source_code[current_pos] == '^') {
            current_token.text[i++] = source_code[current_pos];
            current_pos++;
        }
        current_token.text[i] = '\0';

        if (strcmp(current_token.text, "ENSURE") == 0 || strcmp(current_token.text, "LIMIT") == 0) {
            current_token.type = TOKEN_ENSURE;
        } else if (strstr(current_token.text, "COMPLEXITY") != NULL) {
            current_token.type = TOKEN_ATTRIBUTE;
        } else {
            current_token.type = TOKEN_VALUE;
        }
        return;
    }
    
    // Error
    current_token.type = TOKEN_UNKNOWN;
    current_pos++;
}

// --- 3. HELPER: Big-O Comparison Logic ---
// Returns 1 if val1 < val2, 0 otherwise
int compare_complexity(char* val1, char* val2) {
    // Simple hierarchy: O(1) < O(logN) < O(N) < O(N^2)
    // We map them to integers for easy math
    int get_score(char* s) {
        if (strstr(s, "O(1)")) return 1;
        if (strstr(s, "O(logN)")) return 2;
        if (strstr(s, "O(N)")) return 3;
        if (strstr(s, "O(N^2)")) return 4;
        return 100; // Unknown/Bad
    }
    return get_score(val1) < get_score(val2);
}

// --- 4. PARSER (Recursive Descent) ---
// Grammar: Statement -> COMMAND + ATTRIBUTE + OPERATOR + VALUE

void parse_statement() {
    // A. Expect COMMAND
    if (current_token.type != TOKEN_ENSURE) {
        printf("SYNTAX ERROR: Expected 'ENSURE' or 'LIMIT' at start.\n");
        exit(1);
    }
    printf("1. Command Verified: %s\n", current_token.text);
    get_next_token(); // Eat command, move to next

    // B. Expect ATTRIBUTE
    if (current_token.type != TOKEN_ATTRIBUTE) {
        printf("SYNTAX ERROR: Expected Attribute (e.g., TIME_COMPLEXITY).\n");
        exit(1);
    }
    printf("2. Attribute Verified: %s\n", current_token.text);
    get_next_token();

    // C. Expect OPERATOR
    if (current_token.type != TOKEN_OPERATOR) {
        printf("SYNTAX ERROR: Expected Operator (<, >, =).\n");
        exit(1);
    }
    char op = current_token.text[0];
    printf("3. Operator Verified: %s\n", current_token.text);
    get_next_token();

    // D. Expect VALUE (Constraint)
    if (current_token.type != TOKEN_VALUE) {
        printf("SYNTAX ERROR: Expected Value (e.g., O(N)).\n");
        exit(1);
    }
    char constraint_val[50];
    strcpy(constraint_val, current_token.text);
    printf("4. Constraint Value Verified: %s\n", constraint_val);

    // --- SEMANTIC ANALYSIS (The "Reasoning" Part) ---
    // Let's pretend the detected code complexity is O(N^2) (Hardcoded for demo)
    char* detected_complexity = "O(N^2)"; 
    printf("\n--- LOGIC CHECK ---\n");
    printf("DETECTED CODE COMPLEXITY: %s\n", detected_complexity);
    printf("USER CONSTRAINT: Must be < %s\n", constraint_val);

    if (op == '<') {
        if (compare_complexity(detected_complexity, constraint_val)) {
            printf("RESULT: PASS (Code is optimized enough)\n");
        } else {
            printf("RESULT: FAIL (Code is too slow!)\n");
        }
    }
}

int main(int argc, char* argv[]) {
    // Default rule to parse
    char input[] = "ENSURE TIME_COMPLEXITY < O(N)";
    
    // Setup
    source_code = input;
    current_pos = 0;

    printf("--- AlgoMetric C Compiler Engine ---\n");
    printf("Parsing Rule: %s\n\n", source_code);

    // Priming the pump: Get first token
    get_next_token();
    
    // Start Parsing
    parse_statement();

    return 0;
}