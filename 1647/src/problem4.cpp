#include "../headers/problem4.h"
#include "../headers/nodesAndEdges.h"
#include "../headers/mode.h"
#include "../headers/kmlExport.h"
#include "../headers/dijkstra.h"
#include <bits/stdc++.h>

using namespace std;

void P4() {
    double srcLat, srcLon, destLat, destLon;
    int startHour, startMin;

    FILE* f = fopen("input/input4.txt", "r");
    if (!f) {
        printf("Error: Could not open input/input4.txt\n");
        return;
    }
    
    printf("Enter coordinates #1: Lon Lat\n");
    fscanf(f, "%lf %lf", &srcLon, &srcLat);
    printf("Enter coordinates #2: Lon Lat\n");
    fscanf(f, "%lf %lf", &destLon, &destLat);
    fscanf(f, "%d %d", &startHour, &startMin);
    
    fclose(f);
    
    int source = findNearestNode(srcLat, srcLon);
    int target = findNearestNode(destLat, destLon);
    
    if (source == -1 || target == -1) {
        printf("Error: Could not find nodes\n");
        return;
    }

    for (int i = 0; i < numNodes; i++) {
        dist[i] = INF;
        previous[i] = -1;
        visited[i] = 0;
    }
    
    int path[MAX_NODES];
    int pathEdges[MAX_NODES];
    int pathLen = Dijkstra(source, target, 4, path, pathEdges);

    if (dist[target] >= INF) {
        printf("No path found between the selected nodes.\n");
        return;
    }

    double totalDistance = 0.0;
    double totalCost = dist[target];
    for (int i = pathLen - 1; i > 0; i--) {
        int edgeIdx = pathEdges[i - 1];
        if (edgeIdx >= 0 && edgeIdx < numEdges) {
            totalDistance += edges[edgeIdx].distance;
        }
    }
    
    printFormattedOutput(4, source, target, startHour, startMin, 0, 0, path, pathEdges, pathLen, totalDistance, totalCost);
    
    exportPathToKML(path, pathLen, "output/problem4_route.kml", "Problem 4: Cheapest Route with Schedule", totalDistance, totalCost);
}
