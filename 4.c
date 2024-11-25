#include <stdio.h>
#include <string.h>

#define MAX_MACROS 10
#define MAX_PARAMS 5
#define MAX_LINES 100

// Macro Name Table Entry
typedef struct {
    char macro_name[20];
    int mdt_index;
    int param_count;
    int keyword_param_count;
} MNTEntry;

// Macro Definition Table Entry
typedef struct {
    char line[100];
} MDTEntry;

// Positional Parameter Table Entry
typedef struct {
    char param_name[20];
} PNTABEntry;

// Keyword Parameter Default Table Entry
typedef struct {
    char param_name[20];
    char default_value[20];
} KPDTABEntry;

// Expansion Time Variables Table
typedef struct {
    char var_name[20];
} EVNTABEntry;

// Sequence Symbol Name Table
typedef struct {
    char symbol_name[20];
} SSNTABEntry;

// Sequence Symbol Table Entry
typedef struct {
    char symbol_name[20];
    int value;
} SSTABEntry;

// Table instances
MNTEntry MNT[MAX_MACROS];
MDTEntry MDT[MAX_LINES];
PNTABEntry PNTAB[MAX_PARAMS];
KPDTABEntry KPDTAB[MAX_PARAMS];
EVNTABEntry EVNTAB[MAX_PARAMS];
SSNTABEntry SSNTAB[MAX_PARAMS];
SSTABEntry SSTAB[MAX_PARAMS];

// Counters for the tables
int mnt_count = 0, mdt_count = 0, pntab_count = 0, kpdt_count = 0, evntab_count = 0, ssntab_count = 0, sstab_count = 0;

// Function to define a macro
void define_macro(char* name, char body[][100], int param_count, int keyword_param_count, char params[][20], char keyword_params[][20], char defaults[][20]) {
    // Add to MNT
    strcpy(MNT[mnt_count].macro_name, name);
    MNT[mnt_count].mdt_index = mdt_count;
    MNT[mnt_count].param_count = param_count;
    MNT[mnt_count].keyword_param_count = keyword_param_count;
    mnt_count++;

    // Add positional parameters to PNTAB
    for (int i = 0; i < param_count - keyword_param_count; i++) {
        strcpy(PNTAB[pntab_count].param_name, params[i]);
        pntab_count++;
    }

    // Add keyword parameters to KPDTAB
    for (int i = 0; i < keyword_param_count; i++) {
        strcpy(KPDTAB[kpdt_count].param_name, keyword_params[i]);
        strcpy(KPDTAB[kpdt_count].default_value, defaults[i]);
        kpdt_count++;
    }

    // Add macro body to MDT
    for (int i = 0; i < param_count; i++) {
        strcpy(MDT[mdt_count].line, body[i]);
        mdt_count++;
    }

    // Add Expansion Time Variables (EVNTAB)
    for (int i = 0; i < param_count; i++) {
        sprintf(EVNTAB[evntab_count].var_name, "EVT%d", i+1);
        evntab_count++;
    }

    // Add Sequence Symbols (SSNTAB)
    sprintf(SSNTAB[ssntab_count].symbol_name, "SSN%d", sstab_count+1);
    ssntab_count++;
    
    // Add Sequence Symbols Values (SSTAB)
    SSTAB[sstab_count].value = sstab_count + 100;
    sstab_count++;
}

// Function to print MNT
void print_MNT() {
    printf("\nMacro Name Table (MNT):\n");
    printf("Macro Name | MDT Index | Positional Params | Keyword Params\n");
    for (int i = 0; i < mnt_count; i++) {
        printf("%s\t%d\t\t%d\t\t%d\n", MNT[i].macro_name, MNT[i].mdt_index, MNT[i].param_count - MNT[i].keyword_param_count, MNT[i].keyword_param_count);
    }
}

// Function to print MDT
void print_MDT() {
    printf("\nMacro Definition Table (MDT):\n");
    for (int i = 0; i < mdt_count; i++) {
        printf("%d: %s\n", i+1, MDT[i].line);
    }
}

// Function to print PNTAB
void print_PNTAB() {
    printf("\nPositional Parameter Name Table (PNTAB):\n");
    for (int i = 0; i < pntab_count; i++) {
        printf("%d: %s\n", i+1, PNTAB[i].param_name);
    }
}

// Function to print KPDTAB
void print_KPDTAB() {
    printf("\nKeyword Parameter Default Table (KPDTAB):\n");
    for (int i = 0; i < kpdt_count; i++) {
        printf("%d: %s = %s\n", i+1, KPDTAB[i].param_name, KPDTAB[i].default_value);
    }
}

// Function to print EVNTAB
void print_EVNTAB() {
    printf("\nExpansion Time Variables Table (EVNTAB):\n");
    for (int i = 0; i < evntab_count; i++) {
        printf("%d: %s\n", i+1, EVNTAB[i].var_name);
    }
}

// Function to print SSNTAB
void print_SSNTAB() {
    printf("\nSequence Symbol Name Table (SSNTAB):\n");
    for (int i = 0; i < ssntab_count; i++) {
        printf("%d: %s\n", i+1, SSNTAB[i].symbol_name);
    }
}

// Function to print SSTAB
void print_SSTAB() {
    printf("\nSequence Symbol Table (SSTAB):\n");
    for (int i = 0; i < sstab_count; i++) {
        printf("%d: %s -> %d\n", i+1, SSTAB[i].symbol_name, SSTAB[i].value);
    }
}

int main() {
    // Define first macro
    char macro_body1[3][100] = {
        "LOAD &A", "ADD &B", "STORE &C"
    };
    char positional_params1[2][20] = { "&A", "&B" };
    char keyword_params1[1][20] = { "&C" };
    char keyword_defaults1[1][20] = { "0" };

    define_macro("ADDMACRO", macro_body1, 3, 1, positional_params1, keyword_params1, keyword_defaults1);

    // Define second macro
    char macro_body2[3][100] = {
        "MULT &X", "MULSTORE &Y", "STORE &Z"
    };
    char positional_params2[2][20] = { "&X", "&Y" };
    char keyword_params2[1][20] = { "&Z" };
    char keyword_defaults2[1][20] = { "1" };

    define_macro("MULTMACRO", macro_body2, 3, 1, positional_params2, keyword_params2, keyword_defaults2);

    // Print all tables
    print_MNT();
    print_MDT();
    print_PNTAB();
    print_KPDTAB();
    print_EVNTAB();
    print_SSNTAB();
    print_SSTAB();

    return 0;
}
