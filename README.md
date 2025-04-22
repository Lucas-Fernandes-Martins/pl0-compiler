# C Lexical Analyzer Project

## Description

This project implements a lexical analyzer (scanner) in C. It reads a source code file, processes it character by character, and groups sequences of characters into meaningful units called **tokens** (or lexemes).

The analyzer uses a Deterministic Finite Automaton (DFA) approach. The DFA's transition table is generated from a JFLAP (`.jff`) file using a Python script and stored in a CSV file (`../data/var.csv`). The C code then reads this CSV file to simulate the DFA and perform the lexical analysis on the input source code.

## Prerequisites

To build and run this project, you will need:

*   **Make:** A build automation tool (usually pre-installed on Linux/macOS).
*   **GCC:** The GNU Compiler Collection (or another compatible C compiler like Clang).
*   **Python 3:** Required to run the `parser.py` script which generates the DFA data.

## Setup

1.  **Clone the repository:**
    ```bash
    git clone <your-repository-url>
    cd <repository-directory>
    ```
2.  **(Optional but Recommended First Step): Generate the DFA data:**
    The C code relies on the `../data/var.csv` file. Generate it from the JFLAP file using:
    ```bash
    make parse
    ```
    You typically only need to run this once, or whenever the `../state_machines/automata_compiler.jff` file is modified.

## Building the Project

To compile the lexical analyzer, simply run `make` in the project's root directory:

```bash
make
