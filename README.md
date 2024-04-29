# Pokémon Card Parser
![Pokemon Logo](https://upload.wikimedia.org/wikipedia/commons/9/98/International_Pok%C3%A9mon_logo.svg)

## 📖 Project Overview
Welcome to the world of Pokémon card data parsing! This project is an exciting exploration into the realm of file I/O, memory management, and data structures using the C programming language. The Pokémon Card Parser program reads a `.csv` file containing Pokémon card data and a separate `.csv` file with their respective ability data. It efficiently stores the data using dynamic arrays of pointers to structures, detects and removes invalid entries, sorts the data, and beautifully prints out the Pokémon information.

## 🎯 Learning Outcomes
By completing this project, you will gain hands-on experience with:
- Writing a Makefile to compile a C program
- Using file stream functions to open and read files
- Creating a program that parses file content for display
- Utilizing memory allocator functions effectively

## 🗂 Repository Structure
- `parser.c`: The main source code file containing the implementation of the Pokémon Card Parser
- `Makefile`: The Makefile used to compile the program
- `pokemon.csv`: The input `.csv` file containing Pokémon card data
- `abilities.csv`: The input `.csv` file containing ability data

## 🚀 Getting Started
To get started with the Pokémon Card Parser, follow these steps:
1. Clone the repository:
`git clone https://github.com/your-username/pokemon-card-parser.git`
2. Navigate to the project directory:
`cd pokemon-card-parser`
3. Compile the program using the provided Makefile:
`make`
4. Run the program with the desired input file and offset:
`./parser pokemon.csv 0`

## 📋 Specifications
The Pokémon Card Parser program adheres to the following specifications:
- Parses a `.csv` file containing Pokémon card data and a separate `.csv` file with ability data
- Detects and removes invalid Pokémon entries
- Stores Pokémon and ability data using dynamic arrays of pointers to structures
- Sorts the Pokémon data based on a specific criteria
- Prints out the Pokémon information in a visually appealing format

## 📝 Note
- The program must be compiled using the flags `std=gnu11`, `-Werror`, and `-Wall`
- The `pretty_print()`, `parse_pokemon()`, and `parse_ability()` functions must be implemented as per the provided stubs
- The output of the program should match the expected format to pass the autograder

## 🤝 Contributing
Contributions to the Pokémon Card Parser project are welcome! If you have any ideas, suggestions, or bug fixes, please open an issue or submit a pull request.

## 📧 Contact
For any questions or inquiries, please contact [Your Name] at [Your Email].

Happy parsing and exploring the world of Pokémon cards! 🎉
