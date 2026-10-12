Kaladont



A console-based implementation of the word game Kaladont, written in C. The project uses graph data structures to represent relationships between words and determine valid moves.



Features:

Console-based gameplay

Graph-based representation of word connections

Word validation using external dictionary files

Multiple dictionaries for testing different game configurations

Dynamic memory management



How It Works:

In Kaladont, each new word must begin with the last two letters of the previous word. Words and their valid connections are represented using a graph, allowing the program to determine which moves are possible from the current game state.



Technologies and Concepts

C:

Graph data structures

Dynamic memory allocation

File handling

String processing



Project Structure:

main.c – main program and game logic

recnik1.txt – word dictionary

recnik2.txt – word dictionary

recnik3.txt – word dictionary



Build and Run:

Compile the program using GCC:



gcc main.c -o kaladont

