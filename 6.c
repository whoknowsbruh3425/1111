#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h> // Added for free function

typedef struct {
    char *name;
    char *regex;
} TokenSpec;

int is_keyword(const char *word) {
    const char *keywords[] = {
        "int", "char", "float", "return", "if", "else", "while",
        "for", "void", "break", "continue"
    };
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void lexical_analyzer_from_string(const char *input_string) {
    int line_number = 0;
    char line[256];
    char *lines = strdup(input_string); // Duplicate the input string
    char *line_ptr = strtok(lines, "\n"); // Split into lines by newline
    
    while (line_ptr) {
        line_number++;
        strcpy(line, line_ptr);
        int position = 0;
        size_t line_length = strlen(line);
        
        while (position < line_length) {
            char lexeme[256] = "";
            int matched = 0;

            // Skip whitespace
            while (isspace(line[position])) position++;

            // Match single-line comments
            if (line[position] == '/' && line[position + 1] == '/') {
                printf("Line %d, Lexeme: '//', Token: COMMENT_SINGLE\n", line_number);
                break; // Skip to the next line
            }
            // Match multi-line comments
            if (line[position] == '/' && line[position + 1] == '*') {
                printf("Line %d, Lexeme: '/*', Token: COMMENT_MULTI_START\n", line_number);
                position += 2; // Skip the opening '/*'
                while (position < line_length && !(line[position] == '*' && line[position + 1] == '/')) {
                    position++;
                }
                if (position < line_length) {
                    position += 2; // Skip the closing '*/'
                    printf("Line %d, Lexeme: '*/', Token: COMMENT_MULTI_END\n", line_number);
                }
                continue; // Skip to next token
            }

            // Match keywords and identifiers
            if (isalpha(line[position]) || line[position] == '_') {
                int start = position;
                while (isalnum(line[position]) || line[position] == '_') position++;
                strncpy(lexeme, line + start, position - start);
                lexeme[position - start] = '\0';
                if (is_keyword(lexeme)) {
                    printf("Line %d, Lexeme: '%s', Token: KEYWORD\n", line_number, lexeme);
                } else {
                    printf("Line %d, Lexeme: '%s', Token: IDENTIFIER\n", line_number, lexeme);
                }
                matched = 1;
            }
            // Match numbers (integer or float)
            else if (isdigit(line[position])) {
                int start = position;
                int is_float = 0;
                while (isdigit(line[position]) || line[position] == '.') {
                    if (line[position] == '.') is_float = 1;
                    position++;
                }
                strncpy(lexeme, line + start, position - start);
                lexeme[position - start] = '\0';
                if (is_float) {
                    printf("Line %d, Lexeme: '%s', Token: FLOAT\n", line_number, lexeme);
                } else {
                    printf("Line %d, Lexeme: '%s', Token: NUMBER\n", line_number, lexeme);
                }
                matched = 1;
            }
            // Match operators
            else if (strchr("+-*/=<>!", line[position])) {
                lexeme[0] = line[position++];
                lexeme[1] = '\0';
                printf("Line %d, Lexeme: '%s', Token: OPERATOR\n", line_number, lexeme);
                matched = 1;
            }
            // Match separators
            else if (strchr(";,(){}", line[position])) {
                lexeme[0] = line[position++];
                lexeme[1] = '\0';
                printf("Line %d, Lexeme: '%s', Token: SEPARATOR\n", line_number, lexeme);
                matched = 1;
            }

            // Handle unknown characters
            if (!matched && line[position]) {
                lexeme[0] = line[position++];
                lexeme[1] = '\0';
                printf("Error at line %d, position %d: Unknown token '%s'\n", line_number, position, lexeme);
            }
        }
        
        line_ptr = strtok(NULL, "\n");
    }
    free(lines); // Properly free allocated memory
}

int main() {
    const char *c_code = 
        "int main() {\n"
        "    int x = 10;\n"
        "    float y = 3.14;\n"
        "    // This is a comment\n"
        "    /* Multi-line\n"
        "       comment */\n"
        "    return 0;\n"
        "}";
    
    lexical_analyzer_from_string(c_code);
    return 0;
}
