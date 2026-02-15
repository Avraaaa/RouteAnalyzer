#include "../headers/nodesAndEdges.h"
#include <bits/stdc++.h>

using namespace std;

Node nodes[MAX_NODES];
Edge edges[MAX_NODES * 10];
int numNodes = 0;
int numEdges = 0;

double dist[MAX_NODES];
int previous[MAX_NODES];
int visited[MAX_NODES];
int prevEdge[MAX_NODES];

double haversineDistance(double lat1, double lon1, double lat2, double lon2) {
    double dLat = (lat2 - lat1) * PI / 180.0;
    double dLon = (lon2 - lon1) * PI / 180.0;
    
    lat1 = lat1 * PI / 180.0;
    lat2 = lat2 * PI / 180.0;
    
    double a = sin(dLat/2) * sin(dLat/2) +
               cos(lat1) * cos(lat2) *
               sin(dLon/2) * sin(dLon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    
    return EARTH_RADIUS_KM * c;
}

int findOrAddNode(double lat, double lon) {
    for (int i = 0; i < numNodes; i++) {
        if (fabs(nodes[i].lat - lat) < 1e-6 && 
            fabs(nodes[i].lon - lon) < 1e-6) {
            return i;
        }
    }
    
    nodes[numNodes].id = numNodes;
    nodes[numNodes].lat = lat;
    nodes[numNodes].lon = lon;
    strcpy(nodes[numNodes].name, "Node");
    
    return numNodes++;
}

int findNearestNode(double lat, double lon) {
    if (numNodes == 0) return -1;
    
    int nearest = 0;
    double minDist = haversineDistance(lat, lon, nodes[0].lat, nodes[0].lon);
    
    for (int i = 1; i < numNodes; i++) {
        double d = haversineDistance(lat, lon, nodes[i].lat, nodes[i].lon);
        if (d < minDist) {
            minDist = d;
            nearest = i;
        }
    }
    
    return nearest;
}

void addEdge(int from, int to, Mode mode, double distance) {
    if (numEdges >= MAX_NODES * 10) {
        printf("Maximum edges reached!\n");
        return;
    }
    
    edges[numEdges].from = from;
    edges[numEdges].to = to;
    edges[numEdges].mode = mode;
    edges[numEdges].distance = distance;
    numEdges++;
}

void printFormattedOutput(int problemNo, int source, int target, int startHour, int startMin, int deadlineHour, int deadlineMin, int path[], int pathEdges[], int pathLen, double totalDistance, double totalCost) {
    if (problemNo < 1 || problemNo > 6) return;
    
    char outputFilename[32];
    sprintf(outputFilename, "output/output%d.txt", problemNo);
    FILE* fp = fopen(outputFilename, "w");
    if (!fp) {
        printf("Error opening output file %s\n", outputFilename);
        return;
    }
    
    fprintf(fp, "Problem No: %d\n", problemNo);
    fprintf(fp, "Source: (%.6f, %.6f)\n", nodes[source].lon, nodes[source].lat);
    fprintf(fp, "Destination: (%.6f, %.6f)\n", nodes[target].lon, nodes[target].lat);

    if (problemNo >= 4) {
        int currentMin = startHour * 60 + startMin;
        int arrivalMin = currentMin;
        
        char startAmPm[3];
        int displayStartHour = startHour;
        if (startHour >= 12) { strcpy(startAmPm, "PM"); if (startHour > 12) displayStartHour -= 12; } 
        else { strcpy(startAmPm, "AM"); if (startHour == 0) displayStartHour = 12; }
        
        fprintf(fp, "Start Time: %d:%02d %s\n", displayStartHour, startMin, startAmPm);
        
        if (problemNo == 6) {
             char deadlineAmPm[3];
             int displayDeadlineHour = deadlineHour;
             if (deadlineHour >= 12) { strcpy(deadlineAmPm, "PM"); if (deadlineHour > 12) displayDeadlineHour -= 12; } 
             else { strcpy(deadlineAmPm, "AM"); if (deadlineHour == 0) displayDeadlineHour = 12; }
             fprintf(fp, "Deadline: %d:%02d %s\n", displayDeadlineHour, deadlineMin, deadlineAmPm);
        }
        fprintf(fp, "\n");

        double totalTimeMin = 0;
        
        for (int i = pathLen - 1; i > 0; i--) {
            int edgeIdx = pathEdges[i - 1];
            if (edgeIdx >= 0 && edgeIdx < numEdges) {
                double dist = edges[edgeIdx].distance;
                double speed = SPEED_CAR;
                double rate = CAR_RATE;
                const char* modeStr = "Ride Car";
                
                if (edges[edgeIdx].mode == MODE_METRO) { speed = SPEED_METRO; rate = METRO_RATE; modeStr = "Ride Metro"; }
                else if (edges[edgeIdx].mode == MODE_BIKOLPO) { speed = SPEED_BUS; rate = BIKOLPO_RATE; modeStr = "Ride Bikolpo Bus"; }
                else if (edges[edgeIdx].mode == MODE_UTTARA) { speed = SPEED_BUS; rate = UTTARA_RATE; modeStr = "Ride Uttara Bus"; }
                else if (edges[edgeIdx].mode == MODE_WALK) { speed = SPEED_WALK; rate = 0; modeStr = "Walk"; }
                
                double timeHours = dist / speed;
                double timeMinExact = timeHours * 60.0;
                totalTimeMin += timeMinExact;
                double cost = dist * rate;
                
                int sH = (currentMin / 60) % 24;
                int sM = currentMin % 60;
                char sAP[3];
                int dSH = sH; 
                if (sH >= 12) { strcpy(sAP, "PM"); if (sH > 12) dSH -= 12; } 
                else { strcpy(sAP, "AM"); if (sH == 0) dSH = 12; }

                fprintf(fp, "[%d:%02d %s] %s from %s (%.6f, %.6f) to %s (%.6f, %.6f), Distance: %.3f km, Time: %.1f min, Cost: ৳%.2f\n",
                        dSH, sM, sAP, modeStr,
                        nodes[edges[edgeIdx].from].name, nodes[edges[edgeIdx].from].lon, nodes[edges[edgeIdx].from].lat,
                        nodes[edges[edgeIdx].to].name, nodes[edges[edgeIdx].to].lon, nodes[edges[edgeIdx].to].lat,
                        dist, timeMinExact, cost);
                
                currentMin += (int)round(timeMinExact);
            }
        }
        
        arrivalMin += (int)round(totalTimeMin);
        int reachHour = (arrivalMin / 60) % 24;
        int reachMinute = arrivalMin % 60;
        char reachAmPm[3];
        int displayReachHour = reachHour;
        if (reachHour >= 12) { strcpy(reachAmPm, "PM"); if (reachHour > 12) displayReachHour -= 12; } 
        else { strcpy(reachAmPm, "AM"); if (reachHour == 0) displayReachHour = 12; }
        
        fprintf(fp, "\nArrival Time: %d:%02d %s\n", displayReachHour, reachMinute, reachAmPm);
        
        if (problemNo == 6) {
             int deadlineTotalMin = deadlineHour * 60 + deadlineMin;
             int arrivalTotalMin = arrivalMin; 
             

             if (arrivalTotalMin <= deadlineTotalMin) {
                 int diff = deadlineTotalMin - arrivalTotalMin;
                 fprintf(fp, "Status: ON TIME (%d minutes early)\n", diff);
             } else {
                 int diff = arrivalTotalMin - deadlineTotalMin;
                 fprintf(fp, "Status: LATE (%d minutes late)\n", diff);
             }
        }
        
        fprintf(fp, "Total Distance: %.3f km\n", totalDistance);
        fprintf(fp, "Total Travel Time: %.1f minutes (%.1f hours)\n", totalTimeMin, totalTimeMin / 60.0);
        fprintf(fp, "Total Cost: ৳%.2f\n", totalCost);

    } else {
        fprintf(fp, "\n");
        for (int i = pathLen - 1; i > 0; i--) {
            int edgeIdx = pathEdges[i - 1];
            if (edgeIdx >= 0 && edgeIdx < numEdges) {
                double dist = edges[edgeIdx].distance;
                double rate = CAR_RATE;
                const char* modeStr = "Ride Car";
                
                if (edges[edgeIdx].mode == MODE_METRO) { rate = METRO_RATE; modeStr = "Ride Metro"; }
                else if (edges[edgeIdx].mode == MODE_BIKOLPO) { rate = BIKOLPO_RATE; modeStr = "Ride Bikolpo Bus"; }
                else if (edges[edgeIdx].mode == MODE_UTTARA) { rate = UTTARA_RATE; modeStr = "Ride Uttara Bus"; }
                else if (edges[edgeIdx].mode == MODE_WALK) { rate = 0; modeStr = "Walk"; }
                
                double cost = dist * rate;
                
                fprintf(fp, "%s from %s (%.6f, %.6f) to %s (%.6f, %.6f), Distance: %.3f km, Cost: ৳%.2f\n",
                        modeStr,
                        nodes[edges[edgeIdx].from].name, nodes[edges[edgeIdx].from].lon, nodes[edges[edgeIdx].from].lat,
                        nodes[edges[edgeIdx].to].name, nodes[edges[edgeIdx].to].lon, nodes[edges[edgeIdx].to].lat,
                        dist, cost);
            }
        }
        fprintf(fp, "\nTotal Distance: %.3f km\n", totalDistance);
        fprintf(fp, "Total Cost: ৳%.2f\n", totalCost);
    }
    
    if (problemNo == 1) fprintf(fp, "Exported path to route.kml\n"); 
    else fprintf(fp, "Exported path to route_problem%d.kml\n", problemNo); 

    fprintf(fp, "\n-------Mr Efficient--------\n"
                "[1] Shortest Car Route [Problem 1]\n"
                "[2] Cheapest Route(Car and Metro) [Problem 2]\n"
                "[3] Cheapest Route(Car, Metro and Bus) [Problem 3]\n"
                "[4] Cheapest Route with Time Schedule [Problem 4]\n"
                "[5] Quit\n"
                "-----------------------------\n"
                "Enter Choice: -----------------------------\n");
    
    fclose(fp);
    printf("Output generated in output/output%d.txt\n", problemNo);
}