#ifndef NODES_AND_EDGES_H
#define NODES_AND_EDGES_H

#include "mode.h"

#define MAX_NODES 100000
#define INF 9999999999.0

typedef struct {
    int id;
    char name[32];
    double lat;
    double lon;
} Node;

typedef struct {
    int from;
    int to;
    Mode mode;
    double distance;
} Edge;

extern Node nodes[MAX_NODES];
extern Edge edges[MAX_NODES * 10];
extern int numNodes;
extern int numEdges;

extern double dist[MAX_NODES];
extern int previous[MAX_NODES];
extern int visited[MAX_NODES];
extern int prevEdge[MAX_NODES];

int findOrAddNode(double lat, double lon);
int findNearestNode(double lat, double lon);
void addEdge(int from, int to, Mode mode, double distance);
double haversineDistance(double lat1, double lon1, double lat2, double lon2);

void printFormattedOutput(int problemNo, int source, int target, int startHour, int startMin, int deadlineHour, int deadlineMin, int path[], int pathEdges[], int pathLen, double totalDistance, double totalCost);

#endif
