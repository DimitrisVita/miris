# MIRIS - Money Transfers

## Description
The `miris` application allows the management and monitoring of small money transfers between users through a graph structure.

## Features
1. Creation of a dynamically changing directed (multi-)graph.
2. Cycle detection, fund flow tracing, and structure printing.
3. Addition and deletion of nodes and edges during execution.
4. Connectivity checks and user group highlighting.
5. Memory release at the end of execution.

## Commands
- `i(nsert) Ni [Nj Nk ...]`: Insert new nodes.
- `i(nsert) Ni Nj sum date`: Insert an edge with amount and date.
- `d(elete) Ni [Nj Nk ...]`: Delete nodes and their edges.
- `d(elete) Ni Nj`: Delete an edge between two nodes.
- `m(odify) Ni Nj sum sum1 date date1`: Change the amount and date of an edge.
- `f(ind) Ni`: Display outgoing transactions of a node.
- `r(eceiving) Ni`: Display incoming transactions of a node.
- `c(irclefind) Ni`: Find cycles involving the node.
- `f(indcircles) Ni k`: Find circular transactions with a minimum amount of k.
- `t(raceflow) Ni m`: Find fund flows with a maximum length of m.
- `c(o)nnected) Ni Nj`: Check connectivity between two nodes.
- `e(xit)`: Terminate and save the graph state.

## Execution Instructions
The application is executed with the command:
```
./miris -i inputfile -o outfile
```
- `-i`: Input file with the initial set of nodes and transfers.
- `-o`: Output file with the final state of the graph.

## Makefile
To compile the project, run:
```
make
```
This will generate the `miris` executable. To clean up the compiled files, run:
```
make clean
```

## Example Input Files
Example input files are provided in the `input` directory. These files contain initial sets of nodes and transfers that can be used to test the application.
