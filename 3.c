#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 100
#define MAX_LENGTH 50

// Structures for Machine Opcode Table (MOT), Symbol Table, and Intermediate Code
typedef struct {
    char mnemonic[MAX_LENGTH];
    char type[3]; // IS, AD, DL
    int code;
} MOTEntry;

typedef struct {
    char symbol[MAX_LENGTH];
    int address;
} SymbolTableEntry;

typedef struct {
    int lc;
    char code[MAX_LENGTH];
} IntermediateCodeEntry;

// Machine Opcode Table (MOT)
MOTEntry mot[] = {
    {"MOVER", "IS", 1},
    {"ADD", "IS", 2},
    {"MOVEM", "IS", 3},
    {"START", "AD", 1},
    {"END", "AD", 2},
    {"DS", "DL", 1},
    {"DC", "DL", 2},
};

#define MOT_SIZE (sizeof(mot) / sizeof(MOTEntry))

// Register Table
typedef struct {
    char name[MAX_LENGTH];
    int code;
} RegisterTableEntry;

RegisterTableEntry registers[] = {
    {"AREG", 1},
    {"BREG", 2},
    {"CREG", 3},
    {"DREG", 4},
};

#define REGISTER_TABLE_SIZE (sizeof(registers) / sizeof(RegisterTableEntry))

// Global Variables
SymbolTableEntry symbol_table[MAX_LINES];
IntermediateCodeEntry intermediate_code[MAX_LINES];
int symbol_table_size = 0;
int intermediate_code_size = 0;

// Function Prototypes
int find_in_mot(const char *mnemonic);
int find_in_registers(const char *reg);
int find_in_symbol_table(const char *symbol);
void add_to_symbol_table(const char *symbol, int address);
void pass1(char alp_code[MAX_LINES][MAX_LENGTH], int lines);
void pass2();

// Main Function
int main() {
    // Input ALP Code
    char alp_code[MAX_LINES][MAX_LENGTH] = {
        "START 100",
        "MOVER AREG, C",
        "ADD BREG, D",
        "MOVEM AREG, E",
        "C DC 5",
        "D DS 3",
        "E DS 2",
        "END"
    };
    int lines = 8;

    // Perform Pass-I
    pass1(alp_code, lines);

    // Display Symbol Table
    printf("\nSymbol Table:\n");
    for (int i = 0; i < symbol_table_size; i++) {
        printf("%s -> %d\n", symbol_table[i].symbol, symbol_table[i].address);
    }

    // Display Intermediate Code
    printf("\nIntermediate Code:\n");
    for (int i = 0; i < intermediate_code_size; i++) {
        printf("(%d, '%s')\n", intermediate_code[i].lc, intermediate_code[i].code);
    }

    // Perform Pass-II
    printf("\nGenerated Machine Code:\n");
    pass2();

    return 0;
}

// Find a mnemonic in MOT
int find_in_mot(const char *mnemonic) {
    for (int i = 0; i < MOT_SIZE; i++) {
        if (strcmp(mot[i].mnemonic, mnemonic) == 0) {
            return i;
        }
    }
    return -1;
}

// Find a register in Register Table
int find_in_registers(const char *reg) {
    for (int i = 0; i < REGISTER_TABLE_SIZE; i++) {
        if (strcmp(registers[i].name, reg) == 0) {
            return registers[i].code;
        }
    }
    return -1;
}

// Find a symbol in Symbol Table
int find_in_symbol_table(const char *symbol) {
    for (int i = 0; i < symbol_table_size; i++) {
        if (strcmp(symbol_table[i].symbol, symbol) == 0) {
            return i;
        }
    }
    return -1;
}

// Add a symbol to the Symbol Table
void add_to_symbol_table(const char *symbol, int address) {
    int index = find_in_symbol_table(symbol);
    if (index == -1) { // Add new symbol
        strcpy(symbol_table[symbol_table_size].symbol, symbol);
        symbol_table[symbol_table_size].address = address;
        symbol_table_size++;
    } else { // Update address if already exists
        symbol_table[index].address = address;
    }
}

// Pass-I: Generate Symbol Table and Intermediate Code
void pass1(char alp_code[MAX_LINES][MAX_LENGTH], int lines) {
    int lc = 0; // Location Counter

    for (int i = 0; i < lines; i++) {
        char line[MAX_LENGTH];
        strcpy(line, alp_code[i]);
        char *tokens[MAX_LENGTH];
        int token_count = 0;

        char *token = strtok(line, " ");
        while (token != NULL) {
            tokens[token_count++] = token;
            token = strtok(NULL, " ");
        }

        int mot_index = find_in_mot(tokens[0]);
        if (mot_index != -1) { // Found in MOT
            MOTEntry entry = mot[mot_index];
            if (strcmp(entry.type, "AD") == 0) { // Assembler Directive
                if (strcmp(tokens[0], "START") == 0) {
                    lc = atoi(tokens[1]);
                    sprintf(intermediate_code[intermediate_code_size++].code, "AD %d %s", entry.code, tokens[1]);
                } else if (strcmp(tokens[0], "END") == 0) {
                    sprintf(intermediate_code[intermediate_code_size++].code, "AD %d", entry.code);
                }
            } else if (strcmp(entry.type, "IS") == 0) { // Imperative Statement
                int reg_code = find_in_registers(strtok(tokens[1], ","));
                char *operand = tokens[2];
                if (find_in_symbol_table(operand) == -1) {
                    add_to_symbol_table(operand, -1);
                }
                sprintf(intermediate_code[intermediate_code_size++].code, "IS %d %d %s", entry.code, reg_code, operand);
                lc++;
            } else if (strcmp(entry.type, "DL") == 0) { // Declarative Statement
                if (strcmp(tokens[0], "DC") == 0) {
                    add_to_symbol_table(tokens[1], lc);
                    sprintf(intermediate_code[intermediate_code_size++].code, "DL %d %s", entry.code, tokens[2]);
                    lc++;
                } else if (strcmp(tokens[0], "DS") == 0) {
                    add_to_symbol_table(tokens[1], lc);
                    sprintf(intermediate_code[intermediate_code_size++].code, "DL %d %s", entry.code, tokens[2]);
                    lc += atoi(tokens[2]);
                }
            }
        } else { // Label
            add_to_symbol_table(tokens[0], lc);
            if (strcmp(tokens[1], "DC") == 0) {
                sprintf(intermediate_code[intermediate_code_size++].code, "DL 2 %s", tokens[2]);
                lc++;
            } else if (strcmp(tokens[1], "DS") == 0) {
                sprintf(intermediate_code[intermediate_code_size++].code, "DL 1 %s", tokens[2]);
                lc += atoi(tokens[2]);
            }
        }
    }
}

// Pass-II: Generate Machine Code
void pass2() {
    for (int i = 0; i < intermediate_code_size; i++) {
        char code[MAX_LENGTH];
        strcpy(code, intermediate_code[i].code);
        char *tokens[MAX_LENGTH];
        int token_count = 0;

        char *token = strtok(code, " ");
        while (token != NULL) {
            tokens[token_count++] = token;
            token = strtok(NULL, " ");
        }

        if (strcmp(tokens[0], "IS") == 0) { // Imperative Statement
            int address = symbol_table[find_in_symbol_table(tokens[3])].address;
            printf("(%d, '%s %s %d')\n", intermediate_code[i].lc, tokens[1], tokens[2], address);
        } else if (strcmp(tokens[0], "DL") == 0 && strcmp(tokens[1], "2") == 0) { // DC
            printf("(%d, '%s')\n", intermediate_code[i].lc, tokens[2]);
        }
    }
}
