#include "../headers/kmlExport.h"
#include "../headers/nodesAndEdges.h"
#include <cstdio>
#include <ctime>


void exportPathToKML(int path[], int pathLen, const char* filename, 
                     const char* problemName, double totalDistance, double totalCost) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        printf("Warning: Could not create KML file %s\n", filename);
        return;
    }
    

    time_t now = time(NULL);
    char timeStr[100];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&now));
    

    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(f, "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n");
    fprintf(f, "<Document>\n");
    fprintf(f, "  <name>%s</name>\n", problemName);
    fprintf(f, "  <description>\n");
    fprintf(f, "    Generated: %s\n", timeStr);
    fprintf(f, "    Total Distance: %.3f km\n", totalDistance);
    fprintf(f, "    Total Cost: Tk %.2f\n", totalCost);
    fprintf(f, "  </description>\n\n");
    

    fprintf(f, "  <Style id=\"routeLine\">\n");
    fprintf(f, "    <LineStyle>\n");
    fprintf(f, "      <color>ff0000ff</color>\n");
    fprintf(f, "      <width>4</width>\n");
    fprintf(f, "    </LineStyle>\n");
    fprintf(f, "  </Style>\n\n");
    
    fprintf(f, "  <Style id=\"startPoint\">\n");
    fprintf(f, "    <IconStyle>\n");
    fprintf(f, "      <color>ff00ff00</color>\n");
    fprintf(f, "      <scale>1.2</scale>\n");
    fprintf(f, "    </IconStyle>\n");
    fprintf(f, "  </Style>\n\n");
    
    fprintf(f, "  <Style id=\"endPoint\">\n");
    fprintf(f, "    <IconStyle>\n");
    fprintf(f, "      <color>ff0000ff</color>\n");
    fprintf(f, "      <scale>1.2</scale>\n");
    fprintf(f, "    </IconStyle>\n");
    fprintf(f, "  </Style>\n\n");
    

    

    fprintf(f, "  <Placemark>\n");
    fprintf(f, "    <name>Route Path</name>\n");
    fprintf(f, "    <styleUrl>#routeLine</styleUrl>\n");
    fprintf(f, "    <LineString>\n");
    fprintf(f, "      <tessellate>1</tessellate>\n");
    fprintf(f, "      <coordinates>\n");
    

    for (int i = pathLen - 1; i >= 0; i--) {
        int nodeId = path[i];
        fprintf(f, "        %.6f,%.6f,0\n", nodes[nodeId].lon, nodes[nodeId].lat);
    }
    
    fprintf(f, "      </coordinates>\n");
    fprintf(f, "    </LineString>\n");
    fprintf(f, "  </Placemark>\n\n");
    

    int startNode = path[pathLen - 1];
    fprintf(f, "  <Placemark>\n");
    fprintf(f, "    <name>Start: %s</name>\n", nodes[startNode].name);
    fprintf(f, "    <description>Starting point</description>\n");
    fprintf(f, "    <styleUrl>#startPoint</styleUrl>\n");
    fprintf(f, "    <Point>\n");
    fprintf(f, "      <coordinates>%.6f,%.6f,0</coordinates>\n", 
            nodes[startNode].lon, nodes[startNode].lat);
    fprintf(f, "    </Point>\n");
    fprintf(f, "  </Placemark>\n\n");
    

    int endNode = path[0];
    fprintf(f, "  <Placemark>\n");
    fprintf(f, "    <name>End: %s</name>\n", nodes[endNode].name);
    fprintf(f, "    <description>Destination</description>\n");
    fprintf(f, "    <styleUrl>#endPoint</styleUrl>\n");
    fprintf(f, "    <Point>\n");
    fprintf(f, "      <coordinates>%.6f,%.6f,0</coordinates>\n", 
            nodes[endNode].lon, nodes[endNode].lat);
    fprintf(f, "    </Point>\n");
    fprintf(f, "  </Placemark>\n\n");
        


    fprintf(f, "</Document>\n");
    fprintf(f, "</kml>\n");
    
    fclose(f);
    printf("\nKML file finished exporting: %s\n", filename);
}
