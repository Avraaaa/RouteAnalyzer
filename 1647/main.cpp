#include <bits/stdc++.h>
#include "headers/mode.h"
#include "headers/nodesAndEdges.h"
#include "headers/csvParse.h"
#include "headers/problem1.h"
#include "headers/problem2.h"
#include "headers/problem3.h"

using namespace std;

int Dijkstra(int source, int target, int problemType, int path[], int pathEdges[]) {
    for (int i = 0; i < numNodes; i++) {
        dist[i] = INF;
        previous[i] = -1;
        prevEdge[i] = -1;
        visited[i] = 0;
    }
    dist[source] = 0;
    
    for (int count = 0; count < numNodes; count++) {
        int u = -1;
        double minDist = INF;
        for (int i = 0; i < numNodes; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }
        
        if (u == -1 || u == target) {
            break;
        }
        
        visited[u] = 1;
        
        for (int i = 0; i < numEdges; i++) {
            if (edges[i].from != u) {
                continue;
            }
            
            double edgeWeight = 0;
            int validEdge = 0;
            
            if (problemType == 1) {
                if (edges[i].mode == MODE_CAR) {
                    validEdge = 1;
                    edgeWeight = edges[i].distance;
                }
            }
            else if (problemType == 2) {
                if (edges[i].mode == MODE_CAR) {
                    validEdge = 1;
                    edgeWeight = edges[i].distance * CAR_RATE;
                }
                else if (edges[i].mode == MODE_METRO) {
                    validEdge = 1;
                    edgeWeight = edges[i].distance * METRO_RATE;
                }
            }
            else if (problemType == 3) {
                if (edges[i].mode == MODE_CAR) {
                    validEdge = 1;
                    edgeWeight = edges[i].distance * CAR_RATE;
                }
                else if (edges[i].mode == MODE_METRO) {
                    validEdge = 1;
                    edgeWeight = edges[i].distance * METRO_RATE;
                }
                else if (edges[i].mode == MODE_BIKOLPO) {
                    validEdge = 1;
                    edgeWeight = edges[i].distance * BIKOLPO_RATE;
                }
                else if (edges[i].mode == MODE_UTTARA) {
                    validEdge = 1;
                    edgeWeight = edges[i].distance * UTTARA_RATE;
                }
            }
            
            if (validEdge == 0) {
                continue;
            }
            
            int v = edges[i].to;
            double newDist = dist[u] + edgeWeight;
            
            if (newDist < dist[v]) {
                dist[v] = newDist;
                previous[v] = u;
                prevEdge[v] = i;
            }
        }
    }
    
    int pathLen = 0;
    for (int at = target; at != -1; at = previous[at]) {
        path[pathLen] = at;
        pathEdges[pathLen] = prevEdge[at];
        pathLen++;
    }
    
    return pathLen;
}

int main() {
    parseRoadmapCSV("Dataset/Roadmap-Dhaka.csv");
    parseMetroCSV("Dataset/Routemap-DhakaMetroRail.csv");
    parseBusCSV("Dataset/Routemap-BikolpoBus.csv", MODE_BIKOLPO);
    parseBusCSV("Dataset/Routemap-UttaraBus.csv", MODE_UTTARA);
    
    if (numNodes == 0) {
        printf("No nodes loaded. Please check your CSV files.\n");
        return 1;
    }

    while (true) {
        printf("          RouteAnalyzer Menu\n:");
        printf("[1] Shortest Car Route [Problem 1]\n");
        printf("[2] Cheapest Route (Car and Metro) [Problem 2]\n");
        printf("[3] Cheapest Route (All Modes) [Problem 3]\n");
        printf("[4] Quit\n");
        
        int choice;
        printf("Enter Choice your choice: ");
        scanf("%d", &choice);
        
        if (choice == 4) {
            printf("\nThank you for using RouteAnalyzer!\n");
            break;
        }
        
        switch (choice) {
            case 1:
                P1();
                break;
            case 2:
                P2();
                break;
            case 3:
                P3();
                break;

            default:
                printf("\nInvalid choice. Please try again.\n");
                break;
        }
    }
    
    return 0;
}
