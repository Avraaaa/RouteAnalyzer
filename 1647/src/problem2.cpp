#include "../headers/problem2.h"
#include "../headers/nodesAndEdges.h"
#include "../headers/mode.h"
#include "../headers/kmlExport.h"
#include "../headers/dijkstra.h"
#include <cstdio>
#include <cmath>

void P2() {
    double srcLat, srcLon, destLat, destLon;
    

    
    //freopen("input/input2.txt","r",stdin);
    
    printf("Enter coordinates #1: Lon Lat\n");
    scanf("%lf %lf", &srcLon, &srcLat);
    printf("Enter coordinates #2: Lon Lat\n");
    scanf("%lf %lf", &destLon, &destLat);
    
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
    int pathLen = Dijkstra(source, target, 2, path, pathEdges);

    
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
        int edgeIdx = pathEdges[i - 1];
        
        double distSeg = 0;
        Mode edgeMode = MODE_CAR;
        
        if (edgeIdx >= 0 && edgeIdx < numEdges) {
            distSeg = edges[edgeIdx].distance;
            edgeMode = edges[edgeIdx].mode;
        }
        
        double rate = (edgeMode == MODE_METRO) ? METRO_RATE : CAR_RATE;
        double costSeg = distSeg * rate;
        totalDistance += distSeg;
        totalCost += costSeg;
        

    }
    
    if (fabs(nodes[target].lat - destLat) > 1e-6 || 
        fabs(nodes[target].lon - destLon) > 1e-6) {
        double walkDist = haversineDistance(nodes[target].lat, nodes[target].lon,
                                           destLat, destLon);
        totalDistance += walkDist;
    }
    
    printf("\nTotal Distance: %.3f km\n", totalDistance);
    printf("Total Cost: Tk %.2f\n\n", totalCost);
    
    exportPathToKML(path, pathLen, "output/problem2_route.kml", "Problem 2: Cheapest Route (Car + Metro)", totalDistance, totalCost);
}
