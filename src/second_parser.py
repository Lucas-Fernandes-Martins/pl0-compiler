import xml.etree.ElementTree as ET
import sys
import os

def parse_jff_mealy(filename):
    """
    Parses a JFLAP Mealy machine .jff file and prints transitions.

    Args:
        filename (str): The path to the .jff file.

    Returns:
        None. Prints the output to stdout.
    """
    if not os.path.exists(filename):
        print(f"Error: File not found: {filename}")
        return

    try:
        tree = ET.parse(filename)
        root = tree.getroot()

        # Basic check if it looks like a JFLAP Mealy file
        file_type = root.find('type')
        if file_type is None or file_type.text != 'mealy':
            print(f"Warning: File '{filename}' might not be a Mealy machine JFF file (type='{file_type.text if file_type is not None else 'None'}'). Proceeding anyway.")

        automaton = root.find('automaton')
        if automaton is None:
            print(f"Error: <automaton> tag not found in {filename}")
            return

        # Print the header
        print("state,symbol,next_state,output")

        # Find all transition elements and extract data
        for transition in automaton.findall('transition'):
            from_state = transition.find('from')
            to_state = transition.find('to')
            read_symbol = transition.find('read')
            transout_output = transition.find('transout') # Output for Mealy

            # Ensure all required elements are present within the transition
            if None in [from_state, to_state, read_symbol, transout_output]:
                print("Warning: Skipping incomplete transition element.")
                continue
            if None in [from_state.text, to_state.text, read_symbol.text, transout_output.text]:
                 print(f"Warning: Skipping transition with missing text data: from={from_state.text}, to={to_state.text}, read={read_symbol.text}, output={transout_output.text}")
                 continue


            # Format and print the output line
            print(f"{from_state.text},{read_symbol.text},{to_state.text},{transout_output.text}")

    except ET.ParseError as e:
        print(f"Error parsing XML file {filename}: {e}")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

# --- Main execution block ---
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python parse_jff.py <filename.jff>")
        sys.exit(1) # Exit with an error code

    jff_file = sys.argv[1]
    parse_jff_mealy(jff_file)



