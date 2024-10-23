#pragma once

#include "ADTGraph.h"
#include "ADTHash.h"

// insert into the graph structure 1 or more nodes with specific ids
void utilInsertNodes(Graph graph, Hashtable hashtable, char *command);

// introduce an edge with direction from Ni to Nj with labelsum + date if either
// Ni or Nj does not exist in the graph, do the appropriate node insertion first.
void utilInsertEdge(Graph graph, Hashtable hashtable, char *command);

// delete from graph listed nodes Ni, Nj, Nk, etc
void utilDeleteNodes(Graph graph, Hashtable hashtable, char *command);

// remove the edge between Ni and Nj; if there are more than one edges, remove one of the edges.
void utilDeleteEdge(Graph graph, Hashtable hashtable, char *command);

// update the values of a specific edge between Ni and Nj
void utilModifyEdge(Graph graph, Hashtable hashtable, char *command);

// find all outgoing edges from Ni
void utilFind(Graph graph, Hashtable hashtable, char *command);

// find all incoming to Ni edges
void utilReceiving(Graph graph, Hashtable hashtable, char *command);

// Helper function to perform DFS for cycle detection and flow tracing
void dfs(GraphNode node, GraphNode startNode, GraphNode *pathNodes, GraphEdge *pathEdges, int *pathIndex, double minAmount, int maxLength);

// find circles Ni is involved in if any
void utilCircleFind(Graph graph, Hashtable hashtable, char *command);

// find circular relationships in which Ni is involved and moves at least k units of funds.
void utilFindCircles(Graph graph, Hashtable hashtable, char *command);

// find all paths of length upto m for which a flow of transfers can be establsihed from Ni
void utilTraceflow(Graph graph, Hashtable hashtable, char *command);

// Helper function for DFS to find a path between two nodes
bool dfsConnected(GraphNode currentNode, GraphNode targetNode, GraphNode *path, int *pathIndex);

// If exist, present a path (1) between Ni and Nj
void utilConnected(Graph graph, Hashtable hashtable, char *command);

// terminate the program
void utilExit(Graph graph, Hashtable hashtable, FILE *outputFile);

// write the graph to the output file
void utilWriteToFile(Graph graph, FILE *outputFile);


void utilPrint(Graph graph);