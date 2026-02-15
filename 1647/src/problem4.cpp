#include "../headers/problem4.h"
#include "../headers/nodesAndEdges.h"
#include "../headers/mode.h"
#include "../headers/kmlExport.h"
#include "../headers/dijkstra.h"
#include <cstdio>
#include <cmath>

void P4() {
    double srcLat, srcLon, destLat, destLon;
    int startHour, startMin;
    

    
    //freopen("input/input4.txt","r",stdin);
    
    printf("Enter coordinates #1: Lon Lat\n");
    scanf("%lf %lf", &srcLon, &srcLat);
    printf("Enter coordinates #2: Lon Lat\n");
    scanf("%lf %lf", &destLon, &destLat);
    scanf("%d %d", &startHour, &startMin);
    
    int source = findNearestNode(srcLat, srcLon);
    int target = findNearestNode(destLat, destLon);
    
    if (source == -1 || target == -1) {
        printf("Error: Could not find nodes\n");
        return;
    }
    

    
    for (int i = 0; i < numNodes; i++) {
        dist[i] = INF;
        prev[i] = -1;
        prevEdge[i] = -1;
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
    
    exportPathToKML(path, pathLen, "output/problem4_route.kml", "Problem 4: Cheapest Route with Schedule", totalDistance, totalCost);
}
