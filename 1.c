#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINES 100
#define MAX_SYMBOLS 100

// Machine Opcode Table (MOT)
typedef struct {
    char instruction[10];
    char type[3];
    int code;
} MOTEntry;

MOTEntry MOT[] = {
    {"MOVER", "IS", 1},
    {"ADD", "IS", 2},
    {"MOVEM", "IS", 3},
    {"START", "AD", 1},
    {"END", "AD", 2},
    {"DS", "DL", 1},
    {"DC", "DL", 2}
};

// Register Table
typedef struct {
    char reg[10];
    int code;
} RegisterEntry;

RegisterEntry REGISTER_TABLE[] = {
    {"AREG", 1},
    {"BREG", 2},
    {"CREG", 3},
    {"DREG", 4}
};

// Symbol Table
typedef struct {
    char symbol[10];
    int address;
} Symbol;

Symbol symbol_table[MAX_SYMBOLS];
int symbol_count = 0;

// Intermediate Code Entry
typedef struct {
    int location;
    char code[50];
} IntermediateCode;

IntermediateCode intermediate_code[MAX_LINES];
int intermediate_count = 0;

// Helper Functions
int searchMOT(char *instruction) {
    for (int i = 0; i < sizeof(MOT) / sizeof(MOTEntry); i++) {
        if (strcmp(MOT[i].instruction, instruction) == 0) {
            return i;
        }
    }
    return -1;
}

int searchRegisterTable(char *reg) {
    for (int i = 0; i < sizeof(REGISTER_TABLE) / sizeof(RegisterEntry); i++) {
        if (strcmp(REGISTER_TABLE[i].reg, reg) == 0) {
            return REGISTER_TABLE[i].code;
        }
    }
    return -1;
}

int searchSymbolTable(char *symbol) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].symbol, symbol) == 0) {
            return i;
        }
    }
    return -1;
}

void addToSymbolTable(char *symbol, int address) {
    strcpy(symbol_table[symbol_count].symbol, symbol);
    symbol_table[symbol_count].address = address;
    symbol_count++;
}

void addIntermediateCode(int location, char *code) {
    intermediate_code[intermediate_count].location = location;
    strcpy(intermediate_code[intermediate_count].code, code);
    intermediate_count++;
}

// Main Function
int main() {
    char *alp_code[] = {
        "START 100",
        "MOVER BREG, C",
        "C DC 1",
        "D DS 2",
        "END"
    };
    int line_count = 5;
    int lc = 0; // Location Counter

    for (int i = 0; i < line_count; i++) {
        char line[50];
        strcpy(line, alp_code[i]);
        char *tokens[5];
        int token_count = 0;

        char *token = strtok(line, " ,");
        while (token) {
            tokens[token_count++] = token;
            token = strtok(NULL, " ,");
        }

        int motIndex = searchMOT(tokens[0]);
        if (motIndex != -1) {
            MOTEntry entry = MOT[motIndex];
            if (strcmp(entry.type, "AD") == 0) {
                if (strcmp(tokens[0], "START") == 0) {
                    lc = atoi(tokens[1]);
                    char code[50];
                    sprintf(code, "AD %d %s", entry.code, tokens[1]);
                    addIntermediateCode(lc, code);
                } else if (strcmp(tokens[0], "END") == 0) {
                    char code[50];
                    sprintf(code, "AD %d", entry.code);
                    addIntermediateCode(lc, code);
                }
            } else if (strcmp(entry.type, "IS") == 0) {
                int reg_code = searchRegisterTable(tokens[1]);
                if (searchSymbolTable(tokens[2]) == -1) {
                    addToSymbolTable(tokens[2], -1);
                }
                char code[50];
                sprintf(code, "IS %d %d %s", entry.code, reg_code, tokens[2]);
                addIntermediateCode(lc, code);
                lc++;
            } else if (strcmp(entry.type, "DL") == 0) {
                addToSymbolTable(tokens[1], lc);
                char code[50];
                sprintf(code, "DL %d %s", entry.code, tokens[2]);
                addIntermediateCode(lc, code);
                lc += (strcmp(tokens[0], "DS") == 0) ? atoi(tokens[2]) : 1;
            }
        } else {
            addToSymbolTable(tokens[0], lc);
            int motIndexLabel = searchMOT(tokens[1]);
            if (motIndexLabel != -1) {
                MOTEntry labelEntry = MOT[motIndexLabel];
                char code[50];
                sprintf(code, "DL %d %s", labelEntry.code, tokens[2]);
                addIntermediateCode(lc, code);
                lc += (strcmp(tokens[1], "DS") == 0) ? atoi(tokens[2]) : 1;
            }
        }
    }

    printf("\nInput ALP Code:\n");
    for (int i = 0; i < line_count; i++) {
        printf("%s\n", alp_code[i]);
    }

    printf("\nSymbol Table:\n");
    for (int i = 0; i < symbol_count; i++) {
        printf("%s -> %d\n", symbol_table[i].symbol, symbol_table[i].address);
    }

    printf("\nIntermediate Code:\n");
    for (int i = 0; i < intermediate_count; i++) {
        printf("%d %s\n", intermediate_code[i].location, intermediate_code[i].code);
    }

    return 0;
}
