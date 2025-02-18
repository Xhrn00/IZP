# Contact Search Program

## Description
This C program reads contacts (names and phone numbers) from a file (seznam.txt) and allows searching based on a numeric query. It implements predictive text matching similar to T9 keyboard input, where numeric inputs correspond to possible letters in a name. The program also validates the format of the names and numbers before processing them.

## Features
- Reads names and phone numbers from an input file.
- Validates the input data.
- Supports searching by numeric input.
- Implements predictive text matching for names.
- Displays matched contacts or an appropriate message if no matches are found.

## Compilation
To compile the program, use the following command:
```sh
make
```

## Usage
Run the program with an input file:
```sh
./t9search < seznam.txt
```

### Search by Number
To search using a numeric query, pass a number as an argument:
```sh
./contact_search 38 < seznam.txt
```
This will return contacts where names match the T9 pattern for '38' or where numbers contain '38'.

## Input File Format (seznam.txt)
Each contact should be listed with a name followed by a phone number, separated by a newline:
```
John Doe
123456789
Jane Smith
987654321
```

## Error Handling
- If no input file is provided, it prints: `No file provided`
- If an invalid contact is encountered, it prints: `Skipped invalid contact: <name>`
- If no matching contact is found, it prints: `Not found`
- If too many arguments are provided, it prints: `Too many parameters passed to the search!`

## License
This project is open-source.

