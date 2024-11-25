#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define structures
typedef struct {
    char mnemonic[10];
    char type[3];
    int opcode;
} MOTEntry;

typedef struct {
    char value[10];
    int address;
} LiteralTableEntry;

// Define tables
MOTEntry MOT[] = {
    {"START", "AD", 1}, {"END", "AD", 2}, {"ORIGIN", "AD", 3},
    {"EQU", "AD", 4}, {"LTORG", "AD", 5}, {"MOVER", "IS", 1},
    {"MOVEM", "IS", 2}, {"ADD", "IS", 3}, {"SUB", "IS", 4},
    {"MULT", "IS", 5}, {"DIV", "IS", 6}, {"BC", "IS", 7},
    {"COMP", "IS", 8}, {"STOP", "IS", 9}, {"DC", "DL", 1},
    {"DS", "DL", 2}
};

char *REGISTERS[] = {"AREG", "BREG", "CREG", "DREG"};

// Global variables
LiteralTableEntry literalTable[100];
int poolTable[100];
int literalCount = 0, poolCount = 1, locationCounter = 0;

// Helper functions
int getRegisterCode(char *reg) {
    for (int i = 0; i < 4; i++) {
        if (strcmp(REGISTERS[i], reg) == 0) {
            return i + 1;
        }
    }
    return -1;
}

int searchMOT(char *mnemonic) {
    for (int i = 0; i < sizeof(MOT) / sizeof(MOT[0]); i++) {
        if (strcmp(MOT[i].mnemonic, mnemonic) == 0) {
            return i;
        }
    }
    return -1;
}

int searchLiteral(char *value) {
    for (int i = 0; i < literalCount; i++) {
        if (strcmp(literalTable[i].value, value) == 0) {
            return i;
        }
    }
    return -1;
}

void processLiterals() {
    for (int i = poolTable[poolCount - 1]; i < literalCount; i++) {
        if (literalTable[i].address == -1) {
            literalTable[i].address = locationCounter++;
        }
    }
    poolTable[poolCount++] = literalCount;
}

// Main function
int main() {
    char *alpCode[] = {
        "START 100",
        "MOVER AREG, ='5'",
        "MOVEM BREG, ='10'",
        "LTORG",
        "ADD AREG, ='15'",
        "END"
    };
    int numLines = sizeof(alpCode) / sizeof(alpCode[0]);

    printf("Processing ALP Code...\n");
    printf("\nInput ALP Code:\n");
    for (int i = 0; i < numLines; i++) {
        printf("%s\n", alpCode[i]);
    }

    printf("\nIntermediate Code:\n");

    for (int i = 0; i < numLines; i++) {
        char line[50], instruction[10], operand1[10], operand2[10];
        strcpy(line, alpCode[i]);

        int tokens = sscanf(line, "%s %s %s", instruction, operand1, operand2);

        int motIndex = searchMOT(instruction);
        if (motIndex != -1) {
            MOTEntry motEntry = MOT[motIndex];

            // Handle AD (Assembler Directives)
            if (strcmp(motEntry.type, "AD") == 0) {
                if (strcmp(instruction, "START") == 0) {
                    locationCounter = atoi(operand1);
                    printf("(%d, 'AD %d %d')\n", locationCounter, motEntry.opcode, locationCounter);
                } else if (strcmp(instruction, "LTORG") == 0) {
                    processLiterals();
                    printf("(%d, 'AD %d')\n", locationCounter, motEntry.opcode);
                } else if (strcmp(instruction, "END") == 0) {
                    processLiterals();
                    printf("(%d, 'AD %d')\n", locationCounter, motEntry.opcode);
                }
            }

            // Handle IS (Imperative Statements)
            else if (strcmp(motEntry.type, "IS") == 0) {
                int regCode = getRegisterCode(operand1);
                if (operand2[0] == '=') {
                    if (searchLiteral(operand2) == -1) {
                        strcpy(literalTable[literalCount].value, operand2);
                        literalTable[literalCount++].address = -1;
                    }
                    int literalIndex = searchLiteral(operand2);
                    printf("(%d, 'IS %d %d L%d')\n", locationCounter, motEntry.opcode, regCode, literalIndex + 1);
                }
                locationCounter++;
            }
        }
    }

    // Display Results
    printf("\nSymbol Table:\n");
    printf("(None for this input ALP Code)\n");

    printf("\nLiteral Table:\n");
    printf("Index   Value   Address\n");
    for (int i = 0; i < literalCount; i++) {
        printf("%d       %s     %d\n", i + 1, literalTable[i].value, literalTable[i].address);
    }

    printf("\nPool Table:\n");
    for (int i = 0; i < poolCount; i++) {
        printf("Pool %d -> %d\n", i + 1, poolTable[i]);
    }

    return 0;
}
