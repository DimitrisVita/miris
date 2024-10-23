#include "utils.h"

// insert into the graph structure 1 or more nodes with specific ids.
// Usage: i(nsert) <node1> <node2> <node3> ...
void utilInsertNodes(Graph graph, Hashtable hashtable, char *command) {
    char *commandCopy = strdup(command);  // make a copy of the command
    if (commandCopy == NULL) {
        fprintf(stderr, "Could not allocate memory for command copy.\n");
        exit(1);
    }

    // Buffers to store successful and error nodes
    size_t bufferSize = 1024;
    char *successfulNodes = (char *)myMalloc(bufferSize);
    char *errorNodes = (char *)myMalloc(bufferSize);
    if (successfulNodes == NULL || errorNodes == NULL) {
        fprintf(stderr, "Could not allocate memory for successful and error nodes buffers.\n");
        exit(1);
    }
    successfulNodes[0] = '\0';
    errorNodes[0] = '\0';

    // Insert the nodes
    char *id = strtok(commandCopy, " ");
    while (id != NULL) {
        // Check if the node already exists
        if (getHashNode(hashtable, id) != NULL) {
            id = strtok(NULL, " ");  // Get the next token
            continue;
        }

        // Add the node to the graph
        GraphNode node = addGraphNode(graph, id);
        if (node != NULL) {
            strcat(successfulNodes, id); strcat(successfulNodes, " ");
            addHashNode(hashtable, id, node);   // Add the node to the hashtable
        } else {
            strcat(errorNodes, id); strcat(errorNodes, " ");
        }

        id = strtok(NULL, " ");  // Get the next token
    }

    if (strlen(successfulNodes) > 0)
        printf("Succ: %s\n", successfulNodes);
    if (strlen(errorNodes) > 0)
        printf("IssueWith: %s\n", errorNodes);
    
    myFree(successfulNodes, bufferSize);
    myFree(errorNodes, bufferSize);
    myFree(commandCopy, strlen(command) + 1);
}

// introduce an edge with direction from Ni to Nj with labelsum + date if either
// Ni or Nj does not exist in the graph, do the appropriate node insertion first.
// Usage: (i)n(sert2) <from> <to> <amount> <date>
void utilInsertEdge(Graph graph, Hashtable hashtable, char *command) {
    char *commandCopy = strdup(command);
    if (commandCopy == NULL) {
        fprintf(stderr, "Could not allocate memory for command copy.\n");
        exit(1);
    }

    // Parse the command
    char *from = strtok(commandCopy, " ");
    char *to = strtok(NULL, " ");
    char *amountStr = strtok(NULL, " ");
    char *date = strtok(NULL, " ");
    if (from == NULL || to == NULL || date == NULL || amountStr == NULL) {
        printf("Usage: (i)n(sert2) <from> <to> <amount> <date>\n");
        myFree(commandCopy, strlen(commandCopy) + 1);
        return;
    }

    double amount = strtod(amountStr, NULL);

    // Check if the nodes exist, if not, insert them
    GraphNode fromNode = getHashNode(hashtable, from);
    GraphNode toNode = getHashNode(hashtable, to);
    if (fromNode == NULL) {
        fromNode = addGraphNode(graph, from);
        addHashNode(hashtable, from, fromNode);
    }
    if (toNode == NULL) {
        toNode = addGraphNode(graph, to);
        addHashNode(hashtable, to, toNode);
    }

    // Add the edge to the graph
    GraphEdge edge = addGraphEdge(graph, fromNode, toNode, amount, date);
    if (edge == NULL)
        printf("IssueWith: %s %s\n", from, to);

    myFree(commandCopy, strlen(commandCopy) + 1);
}

// delete from graph listed nodes Ni, Nj, Nk, etc
// Usage: d(elete) <node1> <node2> <node3> ...
void utilDeleteNodes(Graph graph, Hashtable hashtable, char *command) {
    char *commandCopy = strdup(command);
    if (commandCopy == NULL) {
        fprintf(stderr, "Could not allocate memory for command copy.\n");
        exit(1);
    }

    // Buffer to store non-existing nodes
    size_t bufferSize = 1024;
    char *nonExistingNodes = (char *)myMalloc(bufferSize);
    if (nonExistingNodes == NULL) {
        fprintf(stderr, "Could not allocate memory for non-existing nodes buffer.\n");
        exit(1);
    }
    nonExistingNodes[0] = '\0';

    // Delete the nodes
    char *id = strtok(commandCopy, " ");
    while (id != NULL) {
        GraphNode node = getHashNode(hashtable, id);
        if (node != NULL) {
            deleteGraphNode(graph, node);
            deleteHashNode(hashtable, id);
        } else
            strcat(nonExistingNodes, id); strcat(nonExistingNodes, " ");

        id = strtok(NULL, " ");
    }

    if (strlen(nonExistingNodes) > 0)
        printf("Non-existing node(s): %s\n", nonExistingNodes);

    myFree(nonExistingNodes, bufferSize);
    myFree(commandCopy, strlen(commandCopy) + 1);
}

// remove the edge between Ni and Nj; if there are more than one edges, remove one of the edges.
// Usage: (de)l(ete2) <from> <to> <amount> <date>
void utilDeleteEdge(Graph graph, Hashtable hashtable, char *command) {
    char *commandCopy = strdup(command);
    if (commandCopy == NULL) {
        fprintf(stderr, "Could not allocate memory for command copy.\n");
        exit(1);
    }

    // Parse the command
    char *from = strtok(commandCopy, " ");
    char *to = strtok(NULL, " ");
    if (from == NULL || to == NULL) {
        printf("Usage: (de)l(ete2) <from> <to> <amount> <date>\n");
        myFree(commandCopy, strlen(commandCopy) + 1);
        return;
    }

    // Check if the nodes exist
    GraphNode fromNode = getHashNode(hashtable, from);
    GraphNode toNode = getHashNode(hashtable, to);
    if (fromNode == NULL || toNode == NULL) {
        printf("Non-existing node(s): %s %s\n", fromNode == NULL ? from : "", toNode == NULL ? to : "");
        myFree(commandCopy, strlen(commandCopy) + 1);
        return;
    }

    // Delete the edge
    deleteGraphEdge(graph, fromNode, toNode);

    myFree(commandCopy, strlen(commandCopy) + 1);
}

// update the values of a specific edge between Ni and Nj
// Usage: m(odify) <from> <to> <oldAmount> <newAmount> <oldDate> <newDate>
void utilModifyEdge(Graph graph, Hashtable hashtable, char *command) {
    char *commandCopy = strdup(command);
    if (commandCopy == NULL) {
        fprintf(stderr, "Could not allocate memory for command copy.\n");
        exit(1);
    }

    // Parse the command
    char *from = strtok(commandCopy, " ");
    char *to = strtok(NULL, " ");
    char *oldAmountStr = strtok(NULL, " ");
    char *newAmountStr = strtok(NULL, " ");
    char *oldDate = strtok(NULL, " ");
    char *newDate = strtok(NULL, " ");
    if (from == NULL || to == NULL || oldDate == NULL || newDate == NULL || oldAmountStr == NULL || newAmountStr == NULL) {
        printf("Usage: m(odify) <from> <to> <oldAmount> <newAmount> <oldDate> <newDate>\n");
        myFree(commandCopy, strlen(commandCopy) + 1);
        return;
    }

    double oldAmount = strtod(oldAmountStr, NULL);
    double newAmount = strtod(newAmountStr, NULL);

    // Check if the nodes exist
    GraphNode fromNode = getHashNode(hashtable, from);
    GraphNode toNode = getHashNode(hashtable, to);
    if (fromNode == NULL || toNode == NULL) {
        printf("Non-existing node(s): %s %s\n", fromNode == NULL ? from : "", toNode == NULL ? to : "");
        myFree(commandCopy, strlen(commandCopy) + 1);
        return;
    }

    // Modify the edge
    GraphEdge edge = modifyGraphEdge(graph, fromNode, toNode, oldAmount, newAmount, oldDate, newDate);
    if (edge == NULL)
        printf("Not existing edge: %s %s %f %s\n", from, to, oldAmount, oldDate);

    myFree(commandCopy, strlen(commandCopy) + 1);
}

// find all outgoing edges from Ni
// Usage: f(ind) <node>
void utilFind(Graph graph, Hashtable hashtable, char *command) {
    char *commandCopy = strdup(command);
    if (commandCopy == NULL) {
        fprintf(stderr, "Could not allocate memory for command copy.\n");
        exit(1);
    }

    // Parse the command
    char *id = strtok(commandCopy, " ");
    if (id == NULL) {
        printf("Usage: f(ind) <node>\n");
        myFree(commandCopy, strlen(commandCopy) + 1);
        return;
    }

    // Check if the node exists
    GraphNode node = getHashNode(hashtable, id);
    if (node == NULL) {
        printf("Non-existing node: %s\n", id);
        myFree(commandCopy, strlen(commandCopy) + 1);
        return;
    }

    // Print the outgoing edges
    for (GraphEdge edge = node->outgoingEdges; edge != NULL; edge = edge->nextOutgoing)
        printf("%s %f %s\n", edge->to->id, edge->amount, edge->date);

    myFree(commandCopy, strlen(commandCopy) + 1);
}

// find all incoming to Ni edges
// Usage: r(eceiving) <node>
void utilReceiving(Graph graph, Hashtable hashtable, char *command) {
    char *commandCopy = strdup(command);
    if (commandCopy == NULL) {
        fprintf(stderr, "Could not allocate memory for command copy.\n");
        exit(1);
    }

    // Parse the command
    char *id = strtok(commandCopy, " ");
    if (id == NULL) {
        printf("Usage: r(eceiving) <node>\n");
        myFree(commandCopy, strlen(commandCopy) + 1);
        return;
    }

    // Check if the node exists
    GraphNode node = getHashNode(hashtable, id);
    if (node == NULL) {
        printf("Non-existing node: %s\n", id);
        myFree(commandCopy, strlen(commandCopy) + 1);
        return;
    }

    // Print the incoming edges
    for (GraphEdge edge = node->incomingEdges; edge != NULL; edge = edge->nextIncoming)
        printf("%s %f %s\n", edge->from->id, edge->amount, edge->date);

    myFree(commandCopy, strlen(commandCopy) + 1);
}

// Helper function to perform DFS for cycle detection and flow tracing
void dfs(GraphNode node, GraphNode startNode, GraphNode *pathNodes, GraphEdge *pathEdges, int *pathIndex, double minAmount, int maxLength) {
    // Detect cycle if the max length is infinite, otherwise trace flow
    bool detectCycle = (maxLength == __INT_MAX__);

    if (node->visited) {
        // Check if the node is the start node
        if (detectCycle && node == startNode) {
            // Print the cycle
            for (int i = 0; i < *pathIndex; i++)
                printf("%s, ", pathNodes[i]->id);
            printf("%s\n", startNode->id);
        }
        return;
    }

    // Check if the path length exceeds the maximum length
    if (*pathIndex > maxLength) {
        return;
    }

    // Mark the node as visited
    node->visited = true;
    pathNodes[*pathIndex] = node;

    // Check all outgoing edges
    for (GraphEdge edge = node->outgoingEdges; edge != NULL; edge = edge->nextOutgoing) {
        if (edge->amount >= minAmount) {
            // Add the edge to the path
            pathEdges[*pathIndex] = edge;
            (*pathIndex)++;
            dfs(edge->to, startNode, pathNodes, pathEdges, pathIndex, minAmount, maxLength);
            (*pathIndex)--; // Backtrack
        }
    }

    // Trace flow
    if (!detectCycle) {
        for (int i = 0; i < *pathIndex; i++)    // Print the path
            printf("%s, ", pathNodes[i]->id);
        if (*pathIndex > 0)
            printf("%s\n", node->id);
    }

    node->visited = false;
}

// find circles Ni is involved in if any
// Usage: c(irclefind) <node>
void utilCircleFind(Graph graph, Hashtable hashtable, char *command) {
    char *commandCopy = strdup(command);  // make a copy of the command
    if (commandCopy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    char *id = strtok(commandCopy, " ");
    if (id == NULL) {
        printf("Usage: c(irclefind) <node>\n");
        return;
    }

    // Check if the node exists
    GraphNode node = getHashNode(hashtable, id);
    if (node == NULL) {
        printf("Non-existing node: %s\n", id);
        return;
    }

    // Reset visited status for all nodes
    for (GraphNode current = graph->nodes; current != NULL; current = current->next)
        current->visited = false;

    // Allocate memory for the path
    GraphNode *pathNodes = myMalloc(graph->numNodes * sizeof(GraphNode));
    GraphEdge *pathEdges = myMalloc(graph->numNodes * sizeof(GraphEdge));
    if (pathNodes == NULL || pathEdges == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        myFree(pathNodes, graph->numNodes * sizeof(GraphNode));
        myFree(pathEdges, graph->numNodes * sizeof(GraphEdge));
        myFree(commandCopy, strlen(commandCopy) + 1);
        return;
    }

    // Perform DFS to detect cycles
    int pathIndex = 0;
    dfs(node, node, pathNodes, pathEdges, &pathIndex, 0.0, __INT_MAX__);

    myFree(pathNodes, graph->numNodes * sizeof(struct graph_node));
    myFree(pathEdges, graph->numNodes * sizeof(struct graph_edge));
    myFree(commandCopy, strlen(commandCopy) + 1);
}

// find circular relationships in which Ni is involved and moves at least k units of funds.
// Usage: fi(ndcircles) <node1> <lowerBound>
void utilFindCircles(Graph graph, Hashtable hashtable, char *command) {
    char *commandCopy = strdup(command);
    if (commandCopy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    char *id = strtok(commandCopy, " ");
    char *amountStr = strtok(NULL, " ");
    double minAmount;   // check if the amount is a valid double
    if (id == NULL || amountStr == NULL || ((minAmount = atof(amountStr)) < 0)) {
        printf("Usage: fi(ndcircles) <node1> <lowerBound>\n");
        myFree(commandCopy, strlen(commandCopy) + 1);
        return;
    }

    // Check if the node exists
    GraphNode node = getHashNode(hashtable, id);
    if (node == NULL) {
        printf("Non-existing node: %s\n", id);
        myFree(commandCopy, strlen(commandCopy) + 1);
        return;
    }

    // Reset visited status for all nodes
    for (GraphNode current = graph->nodes; current != NULL; current = current->next)
        current->visited = false;

    // Allocate memory for the path
    GraphNode *pathNodes = myMalloc(graph->numNodes * sizeof(GraphNode));
    GraphEdge *pathEdges = myMalloc(graph->numNodes * sizeof(GraphEdge));
    if (pathNodes == NULL || pathEdges == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        myFree(pathNodes, graph->numNodes * sizeof(GraphNode));
        myFree(pathEdges, graph->numNodes * sizeof(GraphEdge));
        myFree(commandCopy, strlen(commandCopy) + 1);
        return;
    }

    // Perform DFS to detect cycles
    int pathIndex = 0;
    dfs(node, node, pathNodes, pathEdges, &pathIndex, minAmount, __INT_MAX__);

    myFree(pathNodes, graph->numNodes * sizeof(GraphNode));
    myFree(pathEdges, graph->numNodes * sizeof(GraphEdge));    
    myFree(commandCopy, strlen(commandCopy) + 1);
}


// find all paths of length upto m for which a flow of transfers can be establsihed from Ni
// Usage: t(raceflow) <node1> <depth>
void utilTraceflow(Graph graph, Hashtable hashtable, char *command) {
    char *commandCopy = strdup(command);
    if (commandCopy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Parse the command
    char *id = strtok(commandCopy, " ");
    char *maxLengthStr = strtok(NULL, " ");
    int maxLength;
    if (id == NULL || maxLengthStr == NULL || ((maxLength = atoi(maxLengthStr)) < 0)) {
        printf("Usage: t(raceflow) <node> <maxLength>\n");
        myFree(commandCopy, strlen(commandCopy) + 1);
        return;
    }

    // Check if the node exists
    GraphNode node = getHashNode(hashtable, id);
    if (node == NULL) {
        printf("Non-existing node: %s\n", id);
        myFree(commandCopy, strlen(commandCopy) + 1);
        return;
    }

    // Reset visited status for all nodes
    for (GraphNode current = graph->nodes; current != NULL; current = current->next)
        current->visited = false;

    // Allocate memory for the path
    GraphNode *pathNodes = myMalloc((maxLength + 1) * sizeof(GraphNode));
    GraphEdge *pathEdges = myMalloc((maxLength + 1) * sizeof(GraphEdge));
    if (pathNodes == NULL || pathEdges == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        myFree(pathNodes, (maxLength + 1) * sizeof(GraphNode));
        myFree(pathEdges, (maxLength + 1) * sizeof(GraphEdge));
        myFree(commandCopy, strlen(commandCopy) + 1);
        return;
    }

    // Perform DFS to trace flow
    int pathIndex = 0;
    dfs(node, node, pathNodes, pathEdges, &pathIndex, 0.0, maxLength);

    myFree(pathNodes, (maxLength + 1) * sizeof(struct graph_node));
    myFree(pathEdges, (maxLength + 1) * sizeof(struct graph_edge));
    myFree(commandCopy, strlen(commandCopy) + 1);
}

// Helper function for DFS to find a path between two nodes
bool dfsConnected(GraphNode currentNode, GraphNode targetNode, GraphNode *path, int *pathIndex) {
    // Mark the current node as visited
    currentNode->visited = true;
    path[(*pathIndex)++] = currentNode;

    // Check if the target node is reached
    if (currentNode == targetNode) {
        for (int i = 0; i < *pathIndex; i++) {  // Print the path
            printf("%s", path[i]->id);
            if (i < *pathIndex - 1)
                printf(", ");
        }
        printf("\n");
        return true;    // Path found
    }

    // Check all outgoing edges
    GraphEdge edge = currentNode->outgoingEdges;
    while (edge != NULL) {
        if (!edge->to->visited)
            if (dfsConnected(edge->to, targetNode, path, pathIndex))    // Recursively check the next node
                return true;    // Path found
        edge = edge->nextOutgoing;
    }

    currentNode->visited = false;
    (*pathIndex)--; // Backtrack
    return false;   // Path not found
}

// If exist, present a path (1) between Ni and Nj
// Usage: (c)o(nnected) <node1> <node2>
void utilConnected(Graph graph, Hashtable hashtable, char *command) {
    char *commandCopy = strdup(command);
    if (commandCopy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Parse the command
    char *id1 = strtok(commandCopy, " ");
    char *id2 = strtok(NULL, " ");
    if (id1 == NULL || id2 == NULL) {
        printf("Usage: c(onnected) <node1> <node2>\n");
        myFree(commandCopy, strlen(commandCopy) + 1);
        return;
    }

    // Check if the nodes exist
    GraphNode node1 = getHashNode(hashtable, id1);
    GraphNode node2 = getHashNode(hashtable, id2);
    if (node1 == NULL || node2 == NULL) {
        printf("Non-existing node(s): %s %s\n", node1 == NULL ? id1 : "", node2 == NULL ? id2 : "");
        myFree(commandCopy, strlen(commandCopy) + 1);
        return;
    }

    // Reset visited status for all nodes
    for (GraphNode current = graph->nodes; current != NULL; current = current->next)
        current->visited = false;

    // Allocate memory for the path
    GraphNode *path = myMalloc(graph->numNodes * sizeof(GraphNode));
    if (path == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        myFree(commandCopy, strlen(commandCopy) + 1);
        return;
    }

    int pathIndex = 0;

    // Perform DFS to find one path between the two nodes
    dfsConnected(node1, node2, path, &pathIndex);

    // Free allocated memory
    myFree(path, graph->numNodes * sizeof(GraphNode));
    myFree(commandCopy, strlen(commandCopy) + 1);
}

// terminate the program
// Usage: e(xit)
void utilExit(Graph graph, Hashtable hashtable, FILE *outputFile) {
    printf("Allocated bytes: %lu\n", allocatedBytes);
    utilWriteToFile(graph, outputFile);
    freeGraph(graph);
    freeHashtable(hashtable);
    return;
}

// write the graph to the output file
void utilWriteToFile(Graph graph, FILE *outputFile) {
    // Set all nodes as unvisited
    for (GraphNode current = graph->nodes; current != NULL; current = current->next) {
        current->visited = false;
    }

    // Write all edges to the file
    for (GraphNode node = graph->nodes; node != NULL; node = node->next) {
        GraphEdge edge = node->outgoingEdges;
        while (edge != NULL) {
            // Write the edge to the file
            fprintf(outputFile, "%s %s %f %s\n", edge->from->id, edge->to->id, edge->amount, edge->date);
            edge->from->visited = true;
            edge->to->visited = true;

            edge = edge->nextOutgoing;
        }
    }

    // Write the nodes that are not connected to any other node
    for (GraphNode current = graph->nodes; current != NULL; current = current->next)
        if (!current->visited)
            fprintf(outputFile, "%s\n", current->id);
}