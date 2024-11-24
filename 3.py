# Machine Opcode Table (MOT)
MOT = {
    "MOVER": {"type": "IS", "code": 1},
    "ADD": {"type": "IS", "code": 2},
    "MOVEM": {"type": "IS", "code": 3},
    "START": {"type": "AD", "code": 1},
    "END": {"type": "AD", "code": 2},
    "DS": {"type": "DL", "code": 1},
    "DC": {"type": "DL", "code": 2},
}

# Register Table
REGISTER_TABLE = {"AREG": 1, "BREG": 2, "CREG": 3, "DREG": 4}

# Initialize tables
symbol_table = {}
intermediate_code = []
machine_code = []

# Input ALP Code
alp_code = [
    "START 100",
    "MOVER AREG, C",
    "ADD BREG, D",
    "MOVEM AREG, E",
    "C DC 5",
    "D DS 3",
    "E DS 2",
    "END",
]

# Pass-I: Generate Symbol Table and Intermediate Code
lc = 0  # Location Counter

for line in alp_code:
    tokens = line.split()
    if tokens[0] in MOT:  # First token is an instruction or directive
        entry = MOT[tokens[0]]
        if entry["type"] == "AD":  # Assembler Directive
            if tokens[0] == "START":
                lc = int(tokens[1])  # Initialize location counter
                intermediate_code.append((lc, f"AD {entry['code']} {tokens[1]}"))
            elif tokens[0] == "END":
                intermediate_code.append((lc, f"AD {entry['code']}"))
        elif entry["type"] == "IS":  # Imperative Statement
            opcode = entry["code"]
            reg = tokens[1][:-1]  # Remove ',' from the register
            operand = tokens[2]  # Symbol (e.g., C, D, or E)
            reg_code = REGISTER_TABLE.get(reg, None)
            if operand not in symbol_table:
                symbol_table[operand] = None  # Forward reference
            intermediate_code.append((lc, f"IS {opcode} {reg_code} {operand}"))
            lc += 1
        elif entry["type"] == "DL":  # Declarative Statement
            if tokens[0] == "DC":  # Define constant
                symbol_table[tokens[1]] = lc
                intermediate_code.append((lc, f"DL {entry['code']} {tokens[2]}"))
                lc += 1
            elif tokens[0] == "DS":  # Define storage
                symbol_table[tokens[1]] = lc
                intermediate_code.append((lc, f"DL {entry['code']} {tokens[2]}"))
                lc += int(tokens[2])  # Reserve space
    else:  # First token is a label (e.g., C or D)
        symbol_table[tokens[0]] = lc
        if tokens[1] == "DC":  # Handle DC after label
            entry = MOT[tokens[1]]
            intermediate_code.append((lc, f"DL {entry['code']} {tokens[2]}"))
            lc += 1
        elif tokens[1] == "DS":  # Handle DS after label
            entry = MOT[tokens[1]]
            intermediate_code.append((lc, f"DL {entry['code']} {tokens[2]}"))
            lc += int(tokens[2])

# Pass-II: Generate Machine Code
for line in intermediate_code:
    lc, code = line
    tokens = code.split()
    if tokens[0] == "IS":  # Imperative Statement
        opcode = tokens[1]
        reg_code = tokens[2]
        operand = tokens[3]
        address = symbol_table.get(operand, 0)
        machine_code.append((lc, f"{opcode} {reg_code} {address}"))
    elif tokens[0] == "DL" and tokens[1] == "2":  # DC
        value = tokens[2]
        machine_code.append((lc, value))

# Display Outputs
print("\nInput ALP Code:")
for line in alp_code:
    print(line)

print("\nSymbol Table:")
for symbol, address in symbol_table.items():
    print(f"{symbol} -> {address}")

print("\nIntermediate Code:")
for entry in intermediate_code:
    print(entry)

print("\nGenerated Machine Code:")
for entry in machine_code:
    print(entry)
