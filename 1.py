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

# Input ALP Code
alp_code = [
    "START 100",
    "MOVER BREG, C",
    "C DC 1",
    "D DS 2",
    "END",
]

# Process ALP Code
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
            operand = tokens[2]  # Symbol (e.g., C)
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

# Display Output
print("\nInput ALP Code:")
for line in alp_code:
    print(line)

print("\nSymbol Table:")
for symbol, address in symbol_table.items():
    print(f"{symbol} -> {address}")

print("\nIntermediate Code:")
for entry in intermediate_code:
    print(entry)
