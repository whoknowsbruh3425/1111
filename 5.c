#include <stdio.h>
#include <string.h>

// Define maximum sizes for MNT, MDT, and APTAB
#define MAX_MACROS 10
#define MAX_LINES 12  // Updated for 12 entries in MDT
#define MAX_PARAMS 2
#define MAX_LINE_LENGTH 50

// Macro Name Table (MNT) structure: Macro Name, Start index in MDT, Number of parameters
typedef struct {
    char macro_name[MAX_LINE_LENGTH];
    int start_index;
    int num_params;
} MNTEntry;

// Macro Definition Table (MDT) - storing the actual macro body
char MDT[MAX_LINES][MAX_LINE_LENGTH] = {
    "LDA #ARG1",   // Instruction 1 for ADDNUM
    "ADD #ARG2",   // Instruction 2 for ADDNUM
    "STA RESULT",  // Instruction 3 for ADDNUM
    "SUBNUM &ARG1, &ARG2",  // Macro for SUBNUM
    "LDA &ARG1",   // Instruction 1 for SUBNUM
    "SUB &ARG2",   // Instruction 2 for SUBNUM
    "STA RESULT",  // Instruction 3 for SUBNUM
    "START",       // Add start directive
    "ADDNUM A, B", // Macro call for ADDNUM
    "SUBNUM X, Y", // Macro call for SUBNUM
    "END"          // End of ALP
};

// Actual Parameter Table (APTAB) - contains the actual parameters passed for each macro call
char APTAB[MAX_MACROS][MAX_PARAMS][MAX_LINE_LENGTH] = {
    {"A", "B"},   // Actual parameters for ADDNUM
    {"X", "Y"},   // Actual parameters for SUBNUM
};

// Function to replace occurrences of ARG1, ARG2 in the macro code with actual parameters
void replace_parameters(char *line, char *param_table[MAX_PARAMS], int num_params) {
    for (int i = 0; i < num_params; i++) {
        char param_placeholder[10];
        sprintf(param_placeholder, "ARG%d", i + 1);
        char *pos;
        while ((pos = strstr(line, param_placeholder)) != NULL) {
            // Replace ARG1 with actual parameter
            memmove(pos + strlen(param_table[i]), pos + strlen(param_placeholder), strlen(pos) - strlen(param_placeholder) + 1);
            memcpy(pos, param_table[i], strlen(param_table[i]));
        }

        // Also replace reference parameters like &ARG1
        sprintf(param_placeholder, "&ARG%d", i + 1);
        while ((pos = strstr(line, param_placeholder)) != NULL) {
            // Replace &ARG1 with actual parameter
            memmove(pos + strlen(param_table[i]), pos + strlen(param_placeholder), strlen(pos) - strlen(param_placeholder) + 1);
            memcpy(pos, param_table[i], strlen(param_table[i]));
        }
    }
}

// Function to perform Macro Expansion
void expand_macros(MNTEntry MNT[], int MNT_size) {
    printf("\nExpanded ALP Code:\n");
    int mnt_idx = 0; // To track the current macro in MNT

    // Loop through each entry in MNT
    for (mnt_idx = 0; mnt_idx < MNT_size; mnt_idx++) {
        char *param_table[MAX_PARAMS];
        int start_index = MNT[mnt_idx].start_index;
        int num_params = MNT[mnt_idx].num_params;

        // Prepare the parameter table for this macro
        for (int i = 0; i < num_params; i++) {
            param_table[i] = APTAB[mnt_idx][i];
        }

        // Expand the macro code using the parameter table
        for (int i = start_index; i < start_index + 3; i++) { // Process 3 instructions for each macro
            char expanded_line[MAX_LINE_LENGTH];
            strcpy(expanded_line, MDT[i]);
            replace_parameters(expanded_line, param_table, num_params);
            printf("%s\n", expanded_line);
        }
    }
}

int main() {
    // Define the Macro Name Table (MNT)
    MNTEntry MNT[MAX_MACROS] = {
        {"ADDNUM", 0, 2},  // Macro name ADDNUM, starts at index 0, has 2 parameters
        {"SUBNUM", 3, 2}   // Macro name SUBNUM, starts at index 3, has 2 parameters
    };
    
    // Display MNT
    printf("MNT (Macro Name Table):\n");
    for (int i = 0; i < 2; i++) {
        printf("%s starts at index %d with %d parameters\n", MNT[i].macro_name, MNT[i].start_index, MNT[i].num_params);
    }

    // Display MDT
    printf("\nMDT (Macro Definition Table):\n");
    for (int i = 0; i < MAX_LINES; i++) {
        printf("%s\n", MDT[i]);
    }

    // Display APTAB
    printf("\nAPTAB (Actual Parameter Table):\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            printf("%s ", APTAB[i][j]);
        }
        printf("\n");
    }

    // Expand macros
    expand_macros(MNT, 2);

    return 0;
}
