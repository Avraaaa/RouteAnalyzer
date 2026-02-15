#include "../headers/kmlExport.h"
#include "../headers/nodesAndEdges.h"
#include <bits/stdc++.h>

using namespace std;

void exportPathToKML(int path[], int pathLen, const char* filename, 
                     const char* problemName, double totalDistance, double totalCost) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        printf("Warning: Could not create KML file %s\n", filename);
        return;
    }

    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(f, "<kml xmlns=\"http://earth.google.com/kml/2.1\">\n");
    fprintf(f, "<Document>\n");
    fprintf(f, "<Style id=\"blueLine\">\n");
    fprintf(f, "<LineStyle>\n");
    fprintf(f, "<color>ffff0000</color>\n");
    fprintf(f, "<width>4</width>\n");
    fprintf(f, "</LineStyle>\n");
    fprintf(f, "</Style>\n");
    fprintf(f, "<Placemark>\n");
    fprintf(f, "<name>Route</name>\n");
    fprintf(f, "<styleUrl>#blueLine</styleUrl>\n");
    fprintf(f, "<LineString>\n");
    fprintf(f, "<coordinates>\n");

    for (int i = pathLen - 1; i >= 0; i--) {
        int nodeId = path[i];
        fprintf(f, "%.4f,%.4f,0\n", nodes[nodeId].lon, nodes[nodeId].lat);
    }

    fprintf(f, "</coordinates>\n");
    fprintf(f, "</LineString>\n");
    fprintf(f, "</Placemark>\n");
    fprintf(f, "</Document>\n");
    fprintf(f, "</kml>\n");
    
    fclose(f);
    printf("\nKML file finished exporting: %s\n", filename);
}
