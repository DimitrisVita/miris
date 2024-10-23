#include "utils.h"

#define MAX_NODES 900

int main(int argc, char *argv[]) {
    //////////////////////////////////////
    // --- COMMAND LINE ARGUMENTS --- ////
    //////////////////////////////////////

    char *inputName = NULL;
    char *outputName = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0)
            inputName = argv[(i++) + 1];
        else if (strcmp(argv[i], "-o") == 0)
            outputName = argv[(i++) + 1];
    }

    if (inputName == NULL || outputName == NULL) {
        fprintf(stderr, "Usage: %s -i <input file> -o <output file>\n", argv[0]);
        exit(1);
    }

    //////////////////////////////////////
    // --- FILE INPUT/OUTPUT SETUP --- ///
    //////////////////////////////////////

    char inputFilePath[256];
    snprintf(inputFilePath, sizeof(inputFilePath), "input/%s", inputName);
    FILE *inputFile = fopen(inputFilePath, "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Could not open input file: %s\n", inputName);
        exit(1);
    }

    char outputFilePath[256];
    snprintf(outputFilePath, sizeof(outputFilePath), "output/%s", outputName);
    FILE *outputFile = fopen(outputFilePath, "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Could not open output file: %s\n", outputName);
        exit(1);
    }    

    Graph graph = createGraph();
    Hashtable hashtable = createHashtable(MAX_NODES);

    //////////////////////////////////////
    // --- FILE INPUT PROCESSING --- /////
    //////////////////////////////////////

    char line[256];
    while (fgets(line, sizeof(line), inputFile)) {
        line[strlen(line)-1] = '\0';    // remove the newline character
        utilInsertEdge(graph, hashtable, line);
    }

    //////////////////////////////////////
    // --- COMMAND LINE PROCESSING --- ///
    //////////////////////////////////////

    utilPrint(graph);

    printf(">> ");
    while (fgets(line, sizeof(line), stdin)) {
        line[strlen(line)-1] = '\0';   // remove the newline character

        char *token = strtok(line, " ");
        char *command = line + strlen(token) + 1;

        if (strcmp(token, "i") == 0 || strcmp(token, "insert") == 0)    // Usage: i(nsert) <node1> <node2> <node3> ...
            utilInsertNodes(graph, hashtable, command);
        else if (strcmp(token, "n") == 0 || strcmp(token, "insert2") == 0)  // Usage: (i)n(sert2) <from> <to> <amount> <date>
            utilInsertEdge(graph, hashtable, command);
        else if (strcmp(token, "d") == 0 || strcmp(token, "delete") == 0)   // Usage: d(elete) <node1> <node2> <node3> ...
            utilDeleteNodes(graph, hashtable, command);
        else if (strcmp(token, "l") == 0 || strcmp(token, "delete2") == 0)  // Usage: (de)l(ete2) <from> <to> <amount> <date>
            utilDeleteEdge(graph, hashtable, command);
        else if (strcmp(token, "m") == 0 || strcmp(token, "modify") == 0)   // Usage: m(odify) <from> <to> <oldAmount> <newAmount> <oldDate> <newDate>
            utilModifyEdge(graph, hashtable, command);
        else if (strcmp(token, "f") == 0 || strcmp(token, "find") == 0)    // Usage: f(ind) <node>
            utilFind(graph, hashtable, command);
        else if (strcmp(token, "r") == 0 || strcmp(token, "receiving") == 0)    // Usage: r(eceiving) <node>
            utilReceiving(graph, hashtable, command);
        else if (strcmp(token, "c") == 0 || strcmp(token, "circlefind") == 0)   // Usage: c(irclefind) <node>
            utilCircleFind(graph, hashtable, command);
        else if (strcmp(token, "fi") == 0 || strcmp(token, "findcircles") == 0)   // Usage: fi(ndcircles) <node1> <lowerBound>
            utilFindCircles(graph, hashtable, command);
        else if (strcmp(token, "t") == 0 || strcmp(token, "traceflow") == 0)    // Usage: t(raceflow) <node1> <depth>
            utilTraceflow(graph, hashtable, command);
        else if (strcmp(token, "o") == 0 || strcmp(token, "connected") == 0)    // Usage: (c)o(nected) <node1> <node2>
            utilConnected(graph, hashtable, command);
        else if (strcmp(token, "e") == 0 || strcmp(token, "exit") == 0) {   // Usage: e(xit)
            utilExit(graph, hashtable, outputFile);
            break;
        }
        else
            printf("Unrecognized command\n");

        // Print the GRAPH
        utilPrint(graph);

        printf(">> ");
    }

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}