# MOT (Machine Opcode Table)
MOT = {
    "START": ("AD", 1),
    "END": ("AD", 2),
    "ORIGIN": ("AD", 3),
    "EQU": ("AD", 4),
    "LTORG": ("AD", 5),
    "MOVER": ("IS", 1),
    "MOVEM": ("IS", 2),
    "ADD": ("IS", 3),
    "SUB": ("IS", 4),
    "MULT": ("IS", 5),
    "DIV": ("IS", 6),
    "BC": ("IS", 7),
    "COMP": ("IS", 8),
    "STOP": ("IS", 9),
    "DC": ("DL", 1),
    "DS": ("DL", 2),
}

# Register Table
REGISTERS = {"AREG": 1, "BREG": 2, "CREG": 3, "DREG": 4}

# Input ALP Code
alp_code = [
    "START 100",
    "MOVER AREG, ='5'",
    "MOVEM BREG, ='10'",
    "LTORG",
    "ADD AREG, ='15'",
    "END"
]

# Initialize tables and variables
location_counter = 0
symbol_table = {}
literal_table = []
pool_table = [0]
intermediate_code = []
literal_index = 0

# Process ALP Code (Pass 1 and Pass 2 combined)
print("Processing ALP Code...")
for line in alp_code:
    tokens = line.split()
    instruction = tokens[0]

    if instruction in MOT:
        opcode_type, opcode_number = MOT[instruction]

        # Handle AD (Assembler Directives)
        if opcode_type == "AD":
            if instruction == "START":
                location_counter = int(tokens[1])
                intermediate_code.append((location_counter, f"AD {opcode_number} {tokens[1]}"))
            elif instruction == "LTORG":
                # Assign addresses to all unaddressed literals
                for i in range(pool_table[-1], len(literal_table)):
                    literal_table[i]["address"] = location_counter
                    location_counter += 1
                pool_table.append(len(literal_table))
                intermediate_code.append((location_counter, f"AD {opcode_number}"))
            elif instruction == "END":
                # Process any remaining literals
                for i in range(pool_table[-1], len(literal_table)):
                    literal_table[i]["address"] = location_counter
                    location_counter += 1
                intermediate_code.append((location_counter, f"AD {opcode_number}"))

        # Handle IS (Imperative Statements)
        elif opcode_type == "IS":
            reg_code = REGISTERS.get(tokens[1].strip(','), None)
            operand = tokens[2]

            # Handle literals
            if operand.startswith("='"):
                literal_value = operand
                if not any(lit["value"] == literal_value for lit in literal_table):
                    literal_table.append({"value": literal_value, "address": None})
                literal_index = next(i for i, lit in enumerate(literal_table) if lit["value"] == literal_value)
                intermediate_code.append((location_counter, f"IS {opcode_number} {reg_code} L{literal_index+1}"))
            location_counter += 1

        # Handle DL (Declarative Statements)
        elif opcode_type == "DL":
            symbol = tokens[0]
            if instruction == "DC":
                symbol_table[symbol] = location_counter
                intermediate_code.append((location_counter, f"DL {opcode_number} {tokens[1]}"))
                location_counter += 1
            elif instruction == "DS":
                symbol_table[symbol] = location_counter
                intermediate_code.append((location_counter, f"DL {opcode_number} {tokens[1]}"))
                location_counter += int(tokens[1])

    else:
        # Handle Labels
        symbol_table[instruction] = location_counter
        operand = tokens[1]
        if operand in MOT:
            opcode_type, opcode_number = MOT[operand]
            intermediate_code.append((location_counter, f"{opcode_type} {opcode_number}"))
            location_counter += 1

# Output Results
print("\nInput ALP Code:")
for line in alp_code:
    print(line)

print("\nSymbol Table:")
for symbol, address in symbol_table.items():
    print(f"{symbol} -> {address}")

print("\nLiteral Table:")
print("Index\tValue\tAddress")
for i, literal in enumerate(literal_table):
    print(f"{i+1}\t{literal['value']}\t{literal['address']}")

print("\nPool Table:")
for index, pool in enumerate(pool_table):
    print(f"Pool {index + 1} -> {pool}")

print("\nIntermediate Code:")
for code in intermediate_code:
    print(code)
