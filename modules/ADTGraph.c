#include "ADTGraph.h"

// Create a new graph
Graph createGraph() {
    Graph graph = (Graph )myMalloc(sizeof(struct graph));
    if (graph == NULL) {
        fprintf(stderr, "Could not allocate memory for graph.\n");
        exit(1);
    }
    graph->nodes = NULL;    // initialize the list of nodes
    graph->numNodes = 0;    // initialize the number of nodes
    return graph;
}

// Add a new node to a graph
GraphNode addGraphNode(Graph graph, char *id) {
    // Allocate memory for the new node
    GraphNode node = (GraphNode )myMalloc(sizeof(struct graph_node));
    if (node == NULL) {
        fprintf(stderr, "Could not allocate memory for graph node.\n");
        exit(1);
    }

    // Initialize the node
    node->id = strdup(id);
    node->outgoingEdges = NULL;
    node->incomingEdges = NULL;
    node->next = graph->nodes;
    node->visited = false;

    // Add the node to the graph's list of nodes
    graph->nodes = node;
    graph->numNodes++;

    return node;
}

// Add a new edge to a graph
GraphEdge addGraphEdge(Graph graph, GraphNode fromNode, GraphNode toNode, double amount, char *date) {
    // Allocate memory for the new edge
    GraphEdge edge = (GraphEdge )myMalloc(sizeof(struct graph_edge));
    if (edge == NULL) {
        fprintf(stderr, "Could not allocate memory for edge.\n");
        exit(1);
    }

    // Initialize the edge
    edge->from = fromNode;
    edge->to = toNode;
    edge->amount = amount;
    edge->date = (char *)myMalloc(strlen(date) + 1);
    if (edge->date == NULL) {
        fprintf(stderr, "Could not allocate memory for edge date.\n");
        exit(1);
    }
    strcpy(edge->date, date);

    // Add the edge to the outgoing edges list of the fromNode
    edge->nextOutgoing = fromNode->outgoingEdges;
    fromNode->outgoingEdges = edge;

    // Add the edge to the incoming edges list of the toNode
    edge->nextIncoming = toNode->incomingEdges;
    toNode->incomingEdges = edge;

    return edge;
}

// Modify the amount and date of a graph edge
GraphEdge modifyGraphEdge(Graph graph, GraphNode fromNode, GraphNode toNode, double oldAmount, double newAmount, char *oldDate, char *newDate) {
    GraphEdge edge = fromNode->outgoingEdges;   // List of outgoing edges of the fromNode
    while (edge != NULL) {
        if (edge->to == toNode && edge->amount == oldAmount && strcmp(edge->date, oldDate) == 0) {
            edge->amount = newAmount;
            myFree(edge->date, strlen(edge->date) + 1);
            edge->date = (char *)myMalloc(strlen(newDate) + 1);
            if (edge->date == NULL) {
                fprintf(stderr, "Could not allocate memory for edge date.\n");
                exit(1);
            }
            strcpy(edge->date, newDate);
            return edge;
        }

        edge = edge->nextOutgoing;  // Move to the next edge
    }
    return NULL;
}

// Delete a node from the graph
void deleteGraphNode(Graph graph, GraphNode node) {
    // Remove all incoming edges
    GraphEdge incoming = node->incomingEdges;
    while (incoming != NULL) {
        GraphEdge next = incoming->nextIncoming;
        deleteGraphEdge(graph, incoming->from, incoming->to);
        incoming = next;
    }

    // Remove all outgoing edges
    GraphEdge outgoing = node->outgoingEdges;
    while (outgoing != NULL) {
        GraphEdge next = outgoing->nextOutgoing;
        deleteGraphEdge(graph, outgoing->from, outgoing->to);
        outgoing = next;
    }

    // Remove the node from the graph's list of nodes
    GraphNode prev = NULL;
    GraphNode curr = graph->nodes;
    while (curr != NULL) {
        if (curr == node) {
            // Remove the node from the list
            if (prev == NULL)
                graph->nodes = curr->next;
            else
                prev->next = curr->next;
            myFree(node->id, strlen(node->id) + 1);
            myFree(node, sizeof(struct graph_node));
            graph->numNodes--;
            return;
        }
        prev = curr;    // Keep track of the previous node
        curr = curr->next;  // Move to the next node
    }
}

// Delete an edge from the graph
void deleteGraphEdge(Graph graph, GraphNode fromNode, GraphNode toNode) {
    // Remove the edge from the outgoing edges of the fromNode
    GraphEdge prevOutgoing = NULL;
    GraphEdge edge = fromNode->outgoingEdges;
    while (edge != NULL) {
        if (edge->to == toNode && edge->from == fromNode) {
            if (prevOutgoing == NULL)
                fromNode->outgoingEdges = edge->nextOutgoing;
            else
                prevOutgoing->nextOutgoing = edge->nextOutgoing;
            break;
        }
        prevOutgoing = edge;        // Keep track of the previous edge
        edge = edge->nextOutgoing;  // Move to the next edge
    }

    // Remove the edge from the incoming edges of the toNode
    GraphEdge prevIncoming = NULL;
    GraphEdge incomingEdge = toNode->incomingEdges;
    while (incomingEdge != NULL) {
        if (incomingEdge->from == fromNode  && incomingEdge->to == toNode) {
            if (prevIncoming == NULL)
                toNode->incomingEdges = incomingEdge->nextIncoming;
            else
                prevIncoming->nextIncoming = incomingEdge->nextIncoming;

            myFree(incomingEdge->date, strlen(incomingEdge->date) + 1);
            myFree(incomingEdge, sizeof(struct graph_edge));
            return;
        }
        prevIncoming = incomingEdge;    // Keep track of the previous edge
        incomingEdge = incomingEdge->nextIncoming;  // Move to the next edge
    }
}

// Free the graph
void freeGraph(Graph graph) {
    GraphNode node = graph->nodes;
    while (node != NULL) {
        GraphNode next = node->next;
        deleteGraphNode(graph, node);
        node = next;
    }
    myFree(graph, sizeof(struct graph));    // Free the graph
}

// Print graph
void printGraph(Graph graph) {
    GraphNode node = graph->nodes;
    while (node != NULL) {
        printf("Node: %s\n", node->id);
        printf("  Incoming edges:\n");
        GraphEdge edge = node->incomingEdges;
        while (edge != NULL) {
            printf("    From: %s, Amount: %f, Date: %s\n", edge->from->id, edge->amount, edge->date);
            edge = edge->nextIncoming;
        }
        printf("  Outgoing edges:\n");
        edge = node->outgoingEdges;
        while (edge != NULL) {
            printf("    To: %s, Amount: %f, Date: %s\n", edge->to->id, edge->amount, edge->date);
            edge = edge->nextOutgoing;
        }
        node = node->next;
    }
}