# Assuming this is the input data from Assignment 4 (MDT, MNT)

# Macro Name Table (MNT) - contains (Macro Name, Start index in MDT, Number of parameters)
MNT = [
    ('ADDNUM', 1, 2),  # Macro name ADDNUM, starts at index 1, has 2 parameters
    ('SUBNUM', 4, 2),  # Macro name SUBNUM, starts at index 4, has 2 parameters
]

# Macro Definition Table (MDT) - contains the actual macro body
MDT = [
    'LDA #ARG1',  # Instruction 1 for ADDNUM
    'ADD #ARG2',  # Instruction 2 for ADDNUM
    'STA RESULT',  # Instruction 3 for ADDNUM
    'SUBNUM &ARG1, &ARG2',  # Macro for SUBNUM
    'LDA &ARG1',  # Instruction 1 for SUBNUM
    'SUB &ARG2',  # Instruction 2 for SUBNUM
    'STA RESULT',  # Instruction 3 for SUBNUM
    'START',  # Add start directive
    'ADDNUM A, B',  # Macro call for ADDNUM
    'SUBNUM X, Y',  # Macro call for SUBNUM
    'END'  # End of ALP
]

# Actual Parameter Table (APTAB) - contains the actual parameters passed for each macro call
APTAB = [
    ['A', 'B'],  # Actual parameters for ADDNUM
    ['X', 'Y'],  # Actual parameters for SUBNUM
]

# Function to perform Macro Expansion
def expand_macros():
    expanded_code = []  # List to store the expanded code
    mnt_idx = 0  # To track the current macro in MNT

    # Loop through each entry in MNT
    for mnt_entry in MNT:
        macro_name, start_index, num_params = mnt_entry
        param_table = APTAB[mnt_idx]  # Get the actual parameters for this macro
        
        # For each macro definition in MDT, substitute parameters from APTAB
        macro_code = MDT[start_index:start_index + num_params]
        expanded_macro = []  # To store expanded version of the current macro

        # Process the macro body and replace parameters with actual values
        for line in macro_code:
            for i in range(num_params):
                # Replace ARG1, ARG2, etc. with the actual parameters
                line = line.replace(f'ARG{i+1}', param_table[i])
                line = line.replace(f'&ARG{i+1}', param_table[i])  # If there are reference parameters as well
            expanded_macro.append(line)
        
        # Add the expanded macro to the overall code
        expanded_code.extend(expanded_macro)

        # Increment the MNT index for the next macro in the APTAB
        mnt_idx += 1

    return expanded_code

# Calling the macro expansion function
expanded_code = expand_macros()

# Display MNT (Macro Name Table)
print("MNT (Macro Name Table):")
for entry in MNT:
    print(entry)

# Display MDT (Macro Definition Table)
print("\nMDT (Macro Definition Table):")
for line in MDT:
    print(line)

# Display APTAB (Actual Parameter Table)
print("\nAPTAB (Actual Parameter Table):")
for params in APTAB:
    print(params)

# Display the expanded ALP code
print("\nExpanded ALP Code:")
for line in expanded_code:
    print(line)
