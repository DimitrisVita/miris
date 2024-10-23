#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "myAllocation.h"

typedef struct graph* Graph;
typedef struct graph_node* GraphNode;
typedef struct graph_edge* GraphEdge;

struct graph_edge {
    GraphNode from;
    GraphNode to;
    double amount;
    char *date;
    GraphEdge nextOutgoing; // pointer to the next outgoing edge at outgoinfEdges list
    GraphEdge nextIncoming; // pointer to the next incoming edge at incomingEdges list
};

struct graph_node {
    char *id;   // id of the GraphNode
    GraphEdge outgoingEdges;    // list of outgoing edges
    GraphEdge incomingEdges;    // list of incoming edges
    GraphNode next; // pointer to the next GraphNode in the graph's list
    bool visited;   // flag to mark visited nodes
};

struct graph {
    GraphNode nodes;    // list of nodes in the graph
    int numNodes;   // number of nodes in the graph
};

// Create a new graph
Graph createGraph();

// Add a new node to the graph
GraphNode addGraphNode(Graph graph, char *id);

// Add a new edge to the graph
GraphEdge addGraphEdge(Graph graph, GraphNode fromNode, GraphNode toNode, double amount, char *date);

// Modify the amount and date of a graph edge
GraphEdge modifyGraphEdge(Graph graph, GraphNode fromNode, GraphNode toNode, double oldAmount, double newAmount, char *oldDate, char *newDate);

// Delete a node from the graph
void deleteGraphNode(Graph graph, GraphNode node);

// Delete an edge from the graph
void deleteGraphEdge(Graph graph, GraphNode fromNode, GraphNode toNode);

// Free the graph
void freeGraph(Graph graph);

// Print the graph
void printGraph(Graph graph);
