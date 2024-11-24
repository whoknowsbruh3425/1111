import re

# Define token types and patterns
token_specs = [
    ('KEYWORD', r'\b(int|char|float|return|if|else|while|for|void|break|continue)\b'),
    ('IDENTIFIER', r'\b[A-Za-z_][A-Za-z0-9_]*\b'),
    ('NUMBER', r'\b\d+\b'),
    ('FLOAT', r'\b\d+\.\d+\b'),
    ('OPERATOR', r'[+\-*/=<>!]+'),
    ('SEPARATOR', r'[;,\(\)\{\}]'),
    ('COMMENT_SINGLE', r'//[^\n]*'),  # Single-line comments
    ('COMMENT_MULTI', r'/\*[\s\S]*?\*/'),  # Multi-line comments
    ('WHITESPACE', r'\s+'),  # Spaces, tabs, newlines (skip)
    ('UNKNOWN', r'.')  # Catch-all for invalid characters
]

# Compile regular expressions
token_regex = '|'.join(f'(?P<{pair[0]}>{pair[1]})' for pair in token_specs)
token_re = re.compile(token_regex)

def lexical_analyzer_from_string(input_string):
    line_number = 0
    lines = input_string.splitlines()  # Split the input string into lines
    
    for line in lines:
        line_number += 1
        line = line.strip()  # Remove leading/trailing whitespaces
        position = 0
        
        # Loop through the line
        while position < len(line):
            match = token_re.match(line, position)
            if match:
                # Check for each matched token
                for token_type, _ in token_specs:
                    value = match.group(token_type)
                    if value:
                        # Handle error detection for unknown tokens
                        if token_type == 'UNKNOWN':
                            print(f"Error at line {line_number}, position {position}: Unknown token '{value}'")
                        else:
                            print(f"Line {line_number}, Lexeme: '{value}', Token: {token_type}, Token_Value: {value}")
                        break
                position = match.end()  # Move to next position after the match
            else:
                position += 1  # Increment if no match is found (error handling)
                print(f"Error at line {line_number}, position {position}: Invalid character '{line[position-1]}'")

# Example C code as a string
c_code = '''
int main() {
    int x = 10;
    float y = 3.14;
    // This is a comment
    /* Multi-line
       comment */
    return 0;
}
'''

# Example usage:
lexical_analyzer_from_string(c_code)  # Pass the C code as a string
