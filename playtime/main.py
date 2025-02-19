import re

# Read the content of the file
text = open("playtime/script.sbscript").read()

# Clean up the text
text = text.replace("\n", "")
text = re.sub(r'\s+', ' ', text)
text = text.strip()

# Regular expression to find functions with a return type and local scoped variables
function_pattern = r'(\w+\s+\w+\s*\(.*?\)\s*\{[^}]*\})'
functions = re.findall(function_pattern, text)

# Extract and print relevant details
for function in functions:
    # Find return type, function name, and body
    return_type_match = re.match(r'(\w+)\s+(\w+)', function)
    if return_type_match:
        return_type = return_type_match.group(1)
        function_name = return_type_match.group(2)
        
        # Extract the body of the function
        body_start = function.find('{') + 1
        body_end = function.rfind('}')
        body = function[body_start:body_end].strip()

        # Find scoped variables (simple variable declarations)
        scoped_vars = re.findall(r'\b\w+\s+\w+\s*=\s*[^;]*;', body)

        # Print function details
        print(f"Function Name: {function_name}")
        print(f"Return Type: {return_type}")
        print("Body Code:")
        print(body)
        print("Scoped Variables:")
        for var in scoped_vars:
            print(f"  - {var.strip()}")
        print()