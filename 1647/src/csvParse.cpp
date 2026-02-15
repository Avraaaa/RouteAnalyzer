#include "../headers/csvParse.h"
#include "../headers/nodesAndEdges.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>

void trimInPlace(char *s)
{
    if (!s)
        return;

    char *p = s;
    while (*p && isspace((unsigned char)*p))
        p++;
    if (p != s)
        memmove(s, p, strlen(p) + 1);

    size_t n = strlen(s);
    while (n > 0 && isspace((unsigned char)s[n - 1]))
    {
        s[--n] = '\0';
    }
}

int splitCSV(char *line, char **tokens, int maxTokens)
{
    int count = 0;
    char *save = NULL;

    char *tok = strtok_r(line, ",", &save);

    while (tok && count < maxTokens)
    {
        trimInPlace(tok);
        tokens[count++] = tok;
        tok = strtok_r(NULL, ",", &save);
    }
    return count;
}

int isNumberToken(const char *s)
{
    if (!s)
        return 0;
    while (*s && isspace((unsigned char)*s))
        s++;
    if (*s == '\0')
        return 0;

    char *end = NULL;
    (void)strtod(s, &end);
    while (end && *end && isspace((unsigned char)*end))
        end++;

    return end && *end == '\0';
}

void parseRoadmapCSV(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        printf("Error: Could not open %s\n", filename);
        return;
    }

    char line[MAX_LINE];
    char *tokens[MAX_TOKENS];
    int roadCount = 0;


    while (fgets(line, sizeof(line), f))
    {
        size_t len = strlen(line);
        if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
            line[len - 1] = '\0';
        if (strlen(line) == 0)
            continue;

        int count = splitCSV(line, tokens, MAX_TOKENS);
        if (count < 6)
            continue;

        const char *altTok = tokens[count - 2];
        const char *lenTok = tokens[count - 1];
        if (!isNumberToken(altTok) || !isNumberToken(lenTok))
            continue;

        int coordCount = count - 3;
        if (coordCount < 4 || coordCount % 2 != 0)
            continue;

        roadCount++;

        for (int i = 1; i + 3 <= count - 2; i += 2)
        {
            double lon1 = atof(tokens[i]);
            double lat1 = atof(tokens[i + 1]);
            double lon2 = atof(tokens[i + 2]);
            double lat2 = atof(tokens[i + 3]);

            int from = findOrAddNode(lat1, lon1);
            int to = findOrAddNode(lat2, lon2);

            double segmentDist = haversineDistance(lat1, lon1, lat2, lon2);

            addEdge(from, to, MODE_CAR, segmentDist);
            addEdge(to, from, MODE_CAR, segmentDist);
        }
    }

    fclose(f);
    printf("Parsed %d road segments\n", roadCount);
}

void parseMetroCSV(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        printf("Error: Could not open %s\n", filename);
        return;
    }

    char line[MAX_LINE];
    char *tokens[MAX_TOKENS];
    int routeCount = 0;


    while (fgets(line, sizeof(line), f))
    {
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0)
            continue;

        int count = splitCSV(line, tokens, MAX_TOKENS);
        if (count < 5)
            continue;

        const char *startStation = tokens[count - 2];
        const char *endStation = tokens[count - 1];

        if (isNumberToken(startStation) || isNumberToken(endStation))
        {
            continue;
        }

        int coordCount = count - 3;
        if (coordCount < 4 || coordCount % 2 != 0)
        {
            continue;
        }

        routeCount++;

        for (int i = 1; i + 3 <= count - 2; i += 2)
        {
            double lon1 = atof(tokens[i]);
            double lat1 = atof(tokens[i + 1]);
            double lon2 = atof(tokens[i + 2]);
            double lat2 = atof(tokens[i + 3]);

            int from = findOrAddNode(lat1, lon1);
            int to = findOrAddNode(lat2, lon2);

            double segmentDist = haversineDistance(lat1, lon1, lat2, lon2);

            addEdge(from, to, MODE_METRO, segmentDist);
            addEdge(to, from, MODE_METRO, segmentDist);

            if (i == 1)
            {
                strncpy(nodes[from].name, startStation, sizeof(nodes[from].name) - 1);
                nodes[from].name[sizeof(nodes[from].name) - 1] = '\0';
            }
            if (i + 4 > count - 2)
            {
                strncpy(nodes[to].name, endStation, sizeof(nodes[to].name) - 1);
                nodes[to].name[sizeof(nodes[to].name) - 1] = '\0';
            }
        }
    }

    fclose(f);
    printf("Parsed %d metro routes\n", routeCount);
}

void parseBusCSV(const char *filename, Mode busMode)
{
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        printf("Error: Could not open %s\n", filename);
        return;
    }

    char line[MAX_LINE];
    char *tokens[MAX_TOKENS];
    int routeCount = 0;

    printf("Parsing bus CSV (%s)...\n", getModeName(busMode));

    while (fgets(line, sizeof(line), f))
    {
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0)
            continue;

        int count = splitCSV(line, tokens, MAX_TOKENS);
        if (count < 5)
            continue;

        const char *startStation = tokens[count - 2];
        const char *endStation = tokens[count - 1];

        if (isNumberToken(startStation) || isNumberToken(endStation))
        {
            continue;
        }

        int coordCount = count - 3;
        if (coordCount < 4 || coordCount % 2 != 0)
        {
            continue;
        }

        routeCount++;

        for (int i = 1; i + 3 <= count - 2; i += 2)
        {
            double lon1 = atof(tokens[i]);
            double lat1 = atof(tokens[i + 1]);
            double lon2 = atof(tokens[i + 2]);
            double lat2 = atof(tokens[i + 3]);

            int from = findOrAddNode(lat1, lon1);
            int to = findOrAddNode(lat2, lon2);

            double segmentDist = haversineDistance(lat1, lon1, lat2, lon2);

            addEdge(from, to, busMode, segmentDist);
            addEdge(to, from, busMode, segmentDist);

            if (i == 1)
            {
                strncpy(nodes[from].name, startStation, sizeof(nodes[from].name) - 1);
                nodes[from].name[sizeof(nodes[from].name) - 1] = '\0';
            }
            if (i + 4 > count - 2)
            {
                strncpy(nodes[to].name, endStation, sizeof(nodes[to].name) - 1);
                nodes[to].name[sizeof(nodes[to].name) - 1] = '\0';
            }
        }
    }

    fclose(f);
    printf("Parsed %d bus routes\n", routeCount);
}
