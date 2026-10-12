Caladont

A console-based implementation of the word game Kaladont, written in C. The project models words and their valid transitions as a directed graph, providing graph manipulation, word analysis, and interactive gameplay against a computer opponent.



Game Rules

Each word must begin with the final two letters of the previously played word. A word cannot be used more than once during the same game. The player who cannot make a valid move loses.



For example:



kaladont -> on...

Features

Loads words from external dictionary files

Automatically builds valid directed connections between words

Supports interactive gameplay against a computer opponent

Prevents previously played words from being reused

Selects a random starting word

Calculates graph-based properties for individual words

Supports manual insertion and removal of vertices and edges

Detects cycles using topological sorting

Manages all graph data using dynamic memory allocation

Graph Representation

Each word is represented as a vertex in a directed graph. An edge from word A to word B exists when the final two letters of A match the first two letters of B.



The graph is stored using a compact adjacency-list representation:



words stores the word associated with each vertex

edges stores adjacent vertex indices

indices stores the starting position of each vertex's adjacency list

The data structures dynamically expand using realloc when additional capacity is required.



Word Analysis

The program uses breadth-first search to calculate three properties for a selected word:



Usefulness – number of other words reachable from the selected word

Efficiency – greatest shortest-path distance to a reachable word

Danger – shortest distance to a reachable word with no outgoing moves

These properties can help the player evaluate possible moves during the game.



Computer Opponent

The computer selects the first available valid move from the current word's adjacency list. The game continues until either the player or the computer has no valid unused word.



Dictionary Format

Each dictionary file must contain the number of words on the first line, followed by the words themselves.



Example:



5

kaladont

ontario

riba

banana

narod

Duplicate words are ignored while loading the dictionary.



Project Structure

.

├── main.c

├── recnik1.txt

├── recnik2.txt

├── recnik3.txt

└── README.md

Build and Run

Using GCC

Compile the project:



gcc main.c -o caladont

Run it on Windows:



./caladont.exe

Run it on Linux or macOS:



./caladont

Technologies and Concepts

C

Directed graphs

Compact adjacency lists

Breadth-first search

Topological sorting

Dynamic memory allocation

File handling

String processing

Console input validation

Author

Mihajlo Stefanović



GitHub

LinkedIn

