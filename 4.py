# Macro Name Table (MNT)
MNT = []
# Macro Definition Table (MDT)
MDT = []
# Positional Parameter Table (PNTAB)
PNTAB = []
# Keyword Parameter Table (KPDTAB)
KPDTAB = []
# Event Table (EVNTAB)
EVNTAB = []
# Special Symbol Name Table (SSNTAB)
SSNTAB = []
# Special Symbol Table (SSTAB)
SSTAB = []

# Example ALP Code with Macro Definitions
alp_code = [
    "MACRO ADDNUM",
    "LDA #ARG1",
    "ADD #ARG2",
    "STA RESULT",
    "SUBNUM &ARG1, &ARG2",
    "LDA &ARG1",
    "SUB &ARG2",
    "STA RESULT",
    "ENDMACRO",
    "MACRO SUBNUM",
    "LDA #ARG1",
    "SUB #ARG2",
    "STA RESULT",
    "ENDMACRO",
    "ADDNUM A, B",  # Usage of macro with positional parameters
    "SUBNUM X, Y"   # Usage of macro with positional parameters
]

# Initialize the location counter for generating machine code
lc = 0
macro_flag = False
current_macro = ""
macro_start = -1

# Process ALP Code to Generate MNT, MDT, PNTAB, KPDTAB, EVNTAB, SSNTAB, SSTAB
for line in alp_code:
    tokens = line.split()

    # Check for macro definition
    if len(tokens) > 1 and tokens[0] == "MACRO":
        current_macro = tokens[1]
        macro_flag = True
        macro_start = len(MDT) + 1  # Start of the macro definition in MDT

    elif line == "ENDMACRO":
        if macro_flag:
            # Add to MNT
            param_count = line.split(',').count(",") + 1  # Count parameters in macro usage
            MNT.append((current_macro, macro_start, 1, param_count, 0))  # Adding to MNT
            macro_flag = False

    elif macro_flag:
        # Handle macro instructions within MDT
        MDT.append(line.strip())  # Add macro instruction to MDT

    elif len(tokens) == 2 and tokens[0] == "ENDMACRO":
        continue  # End of macro definition

# Now handle the PNTAB and KPDTAB for positional and keyword parameters
# Assuming in this case that positional parameters are in the macro call (ADDNUM A, B)
for mnt_entry in MNT:
    macro_name = mnt_entry[0]
    macro_params = mnt_entry[3]  # The number of parameters
    if macro_params > 0:
        for i in range(1, macro_params + 1):
            PNTAB.append(f"ARG{i}")
    
    # Assuming we have keyword parameters in the format: &ARG2=10
    if len(PNTAB) > 0:  # Handle keyword params
        KPDTAB.append((PNTAB[-1], 10))  # Example default value of 10 for keyword param

# Generating EVNTAB (Event Table)
for line in alp_code:
    if "MACRO" in line or "ENDMACRO" in line:
        continue
    tokens = line.split()
    for token in tokens:
        if token.startswith("#") or token.startswith("&"):
            EVNTAB.append(token)  # Track the special symbols as events

# Generating SSNTAB (Special Symbol Name Table)
for token in EVNTAB:
    if token.startswith("#"):
        SSNTAB.append(token)  # Positional parameter placeholder
    elif token.startswith("&"):
        SSNTAB.append(token)  # Keyword parameter placeholder

# Generating SSTAB (Special Symbol Table)
for token in SSNTAB:
    SSTAB.append(f"Value for {token}")  # Placeholder values for special symbols

# Display output
print("\nMNT (Macro Name Table):")
for entry in MNT:
    print(entry)

print("\nMDT (Macro Definition Table):")
for entry in MDT:
    print(entry)

print("\nPNTAB (Positional Parameter Table):")
for entry in PNTAB:
    print(entry)

print("\nKPDTAB (Keyword Parameter Table):")
for entry in KPDTAB:
    print(entry)

print("\nEVNTAB (Event Table):")
for entry in EVNTAB:
    print(entry)

print("\nSSNTAB (Special Symbol Name Table):")
for entry in SSNTAB:
    print(entry)

print("\nSSTAB (Special Symbol Table):")
for entry in SSTAB:
    print(entry)
