#include <bits/stdc++.h>
#include "../headers/problem1.h"
#include "../headers/nodesAndEdges.h"
#include "../headers/mode.h"
#include "../headers/kmlExport.h"
#include "../headers/dijkstra.h"

using namespace std;

void P1() {
    double srcLat, srcLon, destLat, destLon;

    FILE* f = fopen("input/input1.txt", "r");
    if (!f) {
        printf("Error: Could not open input/input1.txt\n");
        return;
    }

    printf("Enter coordinates #1: Lon Lat\n");
    fscanf(f, "%lf %lf", &srcLon, &srcLat);
    printf("Enter coordinates #2: Lon Lat\n");
    fscanf(f, "%lf %lf", &destLon, &destLat);
    
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
    int pathLen = Dijkstra(source, target, 1, path, pathEdges);

    if (pathLen == 1 || dist[target] >= INF) {
        printf("No path found between the selected nodes.\n");
        return;
    }

    double totalDistance = 0.0;
    double totalCost = 0.0;

    if (fabs(nodes[source].lat - srcLat) > 1e-6 || 
        fabs(nodes[source].lon - srcLon) > 1e-6) {
        double walkDist = haversineDistance(srcLat, srcLon,
                                           nodes[source].lat, nodes[source].lon);
        totalDistance += walkDist;
    }
    
    for (int i = pathLen - 1; i > 0; i--) {
        int from = path[i];
        int to = path[i - 1];
        
        double distSeg = 0;
        for (int j = 0; j < numEdges; j++) {
            if (edges[j].from == from && edges[j].to == to && edges[j].mode == MODE_CAR) {
                distSeg = edges[j].distance;
                break;
            }
        }
        
        double costSeg = distSeg * CAR_RATE;
        totalDistance += distSeg;
        totalCost += costSeg;
    }
    
    if (fabs(nodes[target].lat - destLat) > 1e-6 || 
        fabs(nodes[target].lon - destLon) > 1e-6) {
        double walkDist = haversineDistance(nodes[target].lat, nodes[target].lon,
                                           destLat, destLon);
        totalDistance += walkDist;
    }
    
    printFormattedOutput(1, source, target, 0, 0, 0, 0, path, pathEdges, pathLen, totalDistance, totalCost);
    
    exportPathToKML(path, pathLen, "output/problem1_route.kml", "Problem 1: Shortest Car Route", totalDistance, totalCost);
}
