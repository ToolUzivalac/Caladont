#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//GRAF

typedef struct {
    char **words;
    int *edges;
    int *indices;
    int n;
    int vertexCapacity;
    int edgeCount;
    int edgeCapacity;
}Graph;

Graph *createGraph(int vertexCapacity, int edgeCapacity) {
    Graph *graph = malloc(sizeof(Graph));
    if (graph == NULL) return NULL;

    if (edgeCapacity <= 0) edgeCapacity = 1;
    if (vertexCapacity <= 0) vertexCapacity = 1;
    graph->edges = malloc(sizeof(int)*edgeCapacity);
    graph->indices = malloc(sizeof(int)*(vertexCapacity+1));
    graph->words = malloc(sizeof(char *)*vertexCapacity);
    if (graph->edges ==NULL || graph->indices == NULL || graph->words == NULL) {
        free(graph->edges);free(graph->indices);free(graph->words);free(graph);
        return NULL;
    }

    graph->n = 0;
    graph->edgeCount = 0;
    graph->vertexCapacity = vertexCapacity;
    graph->edgeCapacity = edgeCapacity;
    graph->indices[0] = 0;
    for (int i = 0; i < vertexCapacity; i++) {
        graph->words[i] = NULL;
    }
    return graph;
}

void destroyGraph(Graph *graph) {
    if (graph == NULL) return;

    for (int i = 0; i < graph->n; i++) {
        free(graph->words[i]);
    }
    free(graph->words);
    free(graph->indices);
    free(graph->edges);
    free(graph);
}

int addVertex(Graph *graph, const char *word) {
    if (graph == NULL || word == NULL) return 0;
    if (graph->n == graph->vertexCapacity) {
        char **tmpw = realloc(graph->words, graph->vertexCapacity * 2 * sizeof(char *));
        if (tmpw == NULL) return 0;
        graph->words = tmpw;

        int *tmpi = realloc(graph->indices, (graph->vertexCapacity * 2 + 1) * sizeof(int));
        if (tmpi == NULL) return 0;
        graph->indices = tmpi;

        graph->vertexCapacity *= 2;
    }
    graph->words[graph->n] = malloc(sizeof(char)*(strlen(word)+1));
    if (graph->words[graph->n] == NULL) return 0;
    strcpy(graph->words[graph->n], word);
    graph->indices[graph->n + 1] = graph->indices[graph->n];
    graph->n++;

    return 1;
}

int addEdge(Graph *graph, int from, int to) {
    if (graph == NULL) return 0;
    if (from < 0 || to < 0 || from >= graph->n || to >= graph->n) return 0;
    if (graph->edgeCount == graph->edgeCapacity) {
        int *tmp = realloc(graph->edges, graph->edgeCapacity * 2 * sizeof(int));
        if (tmp == NULL) return 0;
        graph->edges = tmp;
        graph->edgeCapacity *= 2;
    }

    for (int i = graph->indices[from]; i < graph->indices[from + 1]; i++) {
        if (graph->edges[i] == to) return 0;
    }

    int idx = graph->indices[from+1];
    for (int i = graph->edgeCount; i > idx; i--) {
        graph->edges[i] = graph->edges[i-1];
    }
    graph->edges[idx] = to;

    for (int i = from+1; i <= graph->n; i++) {
        graph->indices[i]++;
    }
    graph->edgeCount++;

    return 1;
}

int removeEdge(Graph *graph, int from, int to) {
    if (graph == NULL) return 0;
    if (from < 0 || to < 0 || from >= graph->n || to >= graph->n) return 0;
    int isThere = 0, idx = -1;
    for (int i = graph->indices[from]; i < graph->indices[from + 1]; i++) {
        if (graph->edges[i] == to) {
            isThere = 1;
            idx = i;
            break;
        }
    }
    if (!isThere) return 0;

    for (int i = idx; i < graph->edgeCount - 1; i++) {
        graph->edges[i] = graph->edges[i+1];
    }
    for (int i = from+1; i <= graph->n; i++) {
        graph->indices[i]--;
    }
    graph->edgeCount--;

    return 1;
}

void printGraph(Graph *graph) {
    if (graph == NULL) return;
    printf("Cvorovi:\n");
    for (int i = 0; i < graph->n; i++) {
        printf("%d: %s\n", i, graph->words[i]);
    }

    printf("\nEdges:\n");
    for (int i = 0; i < graph->edgeCount; i++) {
        printf("%d ", graph->edges[i]);
    }
    printf("\nIndices:\n");
    for (int i = 0; i < graph->n + 1; i++) {
        printf("%d ", graph->indices[i]);
    }

    printf("\nListe susednosti:\n");
    for (int i = 0; i < graph->n; i++) {
        printf("%d (%s): ", i, graph->words[i]);
        for (int j = graph->indices[i]; j < graph->indices[i + 1]; j++) {
            printf("%d(%s) ", graph->edges[j], graph->words[graph->edges[j]]);
        }
        printf("\n");
    }
}

int removeVertex(Graph *graph, int vertex) {
    if (graph == NULL) return 0;
    if (vertex < 0 || vertex >= graph->n) return 0;

    int *newEdges = malloc(sizeof(int)*graph->edgeCapacity);
    int *newIndices = malloc(sizeof(int)*(graph->vertexCapacity + 1));
    if (newEdges == NULL || newIndices == NULL) {
        free(newEdges);
        free(newIndices);
        return 0;
    }

    int edg = 0, ind = 0;
    for (int i = 0; i < graph->n; i++) {
        if (i != vertex) {
            newIndices[ind] = edg;
            for (int j = graph->indices[i]; j < graph->indices[i + 1]; j++) {
                if (graph->edges[j] != vertex) {
                    newEdges[edg] = graph->edges[j];
                    edg++;
                }
            }
            ind++;
        }
    }
    newIndices[ind] = edg;
    for (int i = 0; i < edg; i++) {
        if (newEdges[i] > vertex) {
            newEdges[i]--;
        }
    }
    graph->edgeCount = edg;

    free(graph->edges);
    free(graph->indices);
    graph->edges = newEdges;
    graph->indices = newIndices;

    free(graph->words[vertex]);
    for (int i = vertex; i < graph->n - 1; i++) {
        graph->words[i] = graph->words[i+1];
    }
    graph->words[graph->n - 1] = NULL;

    graph->n--;
    return 1;
}

//KALADONT

int findWord(Graph *graph, const char *word) {
    if (graph == NULL || word == NULL) return -1;
    for (int i = 0; i < graph->n; i++) {
        if (strcmp(graph->words[i], word) == 0) return i;
    }
    return -1;
}

int canFollow(const char *first, const char *second) {
    if (first == NULL || second == NULL) return 0;
    int len1 = strlen(first), len2 = strlen(second);
    if (len1 < 2 || len2 < 2) return 0;
    if (first[len1-2] == second[0] && first[len1-1] == second[1]) return 1;
    return 0;
}

int buildKaladontEdges(Graph *graph) {
    if (graph == NULL) return 0;
    for (int i = 0; i < graph->n; i++) {
        for (int j = 0; j < graph->n; j++) {
            if (i != j && canFollow(graph->words[i], graph->words[j])) {
                if (!addEdge(graph, i, j)) return 0;
            }
        }
    }

    return 1;
}

Graph *loadDictionary(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) return NULL;
    int N;
    if (fscanf(f, "%d", &N) != 1 || N <= 0) {
        fclose(f);
        return NULL;
    }
    Graph *graph = createGraph(N, N*N);
    if (graph == NULL) {
        fclose(f);
        return NULL;
    }

    char buffer[100];
    for (int i = 0; i < N; i++) {
        if (fscanf(f, "%99s", buffer) != 1) {
            fclose(f);
            destroyGraph(graph);
            return NULL;
        }
        if (findWord(graph, buffer) != -1) continue;
        if (!addVertex(graph, buffer)) {
            fclose(f);
            destroyGraph(graph);
            return NULL;
        }
    }

    fclose(f);
    if (!buildKaladontEdges(graph)) {
        destroyGraph(graph);
        return NULL;
    }

    return graph;
}

int *bfsDistances(Graph *graph, int start) {
    if (graph == NULL || start < 0) return NULL;
    if (graph->n <= start) return NULL;
    int *queue = malloc(sizeof(int)*graph->n);
    if (queue == NULL) return NULL;
    int front = 0, rear = 0;
    int *dist = malloc(sizeof(int)*graph->n);
    if (dist == NULL) {
        free(queue);
        return NULL;
    }

    for (int i = 0; i < graph->n; i++) {
        dist[i] = -1;
    }
    queue[rear++] = start;
    dist[start] = 0;
    while (front != rear) {
        int u = queue[front++];
        for (int i = graph->indices[u]; i < graph->indices[u + 1]; i++) {
            if (dist[graph->edges[i]] == -1) {
                dist[graph->edges[i]] = dist[u] + 1;
                queue[rear++] = graph->edges[i];
            }
        }
    }
    free(queue);

    return dist;
}

int usefulness(Graph *graph, int start) {
    int *dist = bfsDistances(graph, start);
    if (dist == NULL) return -1;
    int use = 0;
    for (int i = 0; i < graph->n; i++) {
        if (dist[i] > 0) use++;
    }

    free(dist);
    return use;
}

int efficiency(Graph *graph, int start) {
    int *dist = bfsDistances(graph, start);
    if (dist == NULL) return -1;
    int eff = 0;
    for (int i = 0; i < graph->n; i++) {
        if (dist[i] > eff) eff = dist[i];
    }

    free(dist);
    return eff;
}

int danger(Graph *graph, int start) {
    int *dist = bfsDistances(graph, start);
    if (dist == NULL) return -1;
    int dng = graph->n + 1;
    for (int i = 0; i < graph->n; i++) {
        if (graph->indices[i] == graph->indices[i + 1] && dist[i] != -1) {
            if (dist[i] + 1 < dng) dng = dist[i] + 1;
        }
    }

    if (dng == graph->n + 1) dng = -1;
    free(dist);
    return dng;
}

int printWordProperties(Graph *graph, const char *word) {
    if (graph == NULL || word == NULL) return 0;
    int exist = findWord(graph, word);
    if (exist == -1) return 0;

    printf("\nRec: %s\nKorisnost: %d\nEfikasnost: %d\nOpasnost: %d\n\n", word, usefulness(graph, exist), efficiency(graph, exist), danger(graph, exist));
    return 1;
}

int isValidMove(Graph *graph, int current, int next, int *used) {
    if (graph == NULL || used == NULL) return 0;
    if (current < 0 || next < 0 || current >= graph->n || next >= graph->n) return 0;
    if (used[next] == 1) return 0;

    for (int i = graph->indices[current]; i < graph->indices[current + 1]; i++) {
        if (graph->edges[i] == next) return 1;
    }
    return 0;
}

int chooseRandomStart(Graph *graph) {
    if (graph == NULL || graph->n == 0) return -1;
    return rand() % graph->n;
}

int chooseComputerMove(Graph *graph, int current, int *used) {
    if (graph == NULL || used == NULL) return -1;
    if (current < 0 || current >= graph->n) return -1;
    for (int i = graph->indices[current]; i < graph->indices[current + 1]; i++) {
        if (!used[graph->edges[i]]) return graph->edges[i];
    }
    return -1;
}

void simulateGame(Graph *graph) {
    if (graph == NULL || graph->n == 0) return;

    int *used = calloc(graph->n, sizeof(int));
    if (used == NULL) return;
    int current = chooseRandomStart(graph);
    if (current == -1) {
        free(used);
        return;
    }
    used[current] = 1;
    printf("\nPocetna rec:\n%s\n\n", graph->words[current]);

    int move = chooseComputerMove(graph, current, used);
    if (move == -1) {
        printf("\nCovek nema validan potez. Kompjuter je pobedio!\n\n");
        free(used);
        return;
    }

    char buffer[100];
    int playerToMove = 1;
    while (1) {
        if (playerToMove) {
            printf("\n1. Odigraj rec\n2. Pomoc za rec\n3. Predaj se / Kraj igre\n\n");
            int a;
            if (scanf("%d", &a) != 1) {
                printf("\nNevalidna opcija!\n\n");
                while (getchar() != '\n');
                continue;
            }
            switch (a) {
                case 1: {
                    printf("\nCovek igra:\n");
                    if (scanf("%99s", buffer) != 1) {
                        printf("\nNevalidan unos!\n\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    int tmp = findWord(graph, buffer);
                    if (tmp == -1) {
                        printf("\nNe postoji data rec u recniku\n\n");
                    }else {
                        int valid = isValidMove(graph, current, tmp, used);
                        if (!valid) {
                            printf("\nRec ne postuje pravila igre, Kompjuter je pobedio!\n\n");
                            free(used);
                            return;
                        }
                        used[tmp] = 1;
                        current = tmp;
                        playerToMove = 0;
                    }

                    break;
                }
                case 2: {
                    printf("\nUnesi rec za pomoc:\n");
                    if (scanf("%99s", buffer) != 1) {
                        printf("\nNevalidan unos!\n\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    if (!printWordProperties(graph, buffer)) printf("\nNevalidna rec ili ne postoji u recniku!\n\n");
                    break;
                }
                case 3:
                    free(used);
                    printf("\nKompjuter je pobedio!\n\n");
                    return;
                default:
                    printf("\nNevalidna opcija!\n\n");
            }
        }else {
            int move = chooseComputerMove(graph, current, used);
            if (move == -1) {
                printf("\nCovek je pobedio!\n\n");
                break;
            }
            printf("\nKompjuter igra:\n%s\n\n", graph->words[move]);
            used[move] = 1;
            current = move;

            move = chooseComputerMove(graph, current, used);
            if (move == -1) {
                printf("\nCovek nema validan potez. Kompjuter je pobedio!\n\n");
                break;
            }

            playerToMove = 1;
        }
    }

    free(used);
}

void printMenu() {
    printf("===== KALADONT - MENI =====\n\n"
           "1. Ucitaj recnik iz fajla\n"
           "2. Ispisi graf\n"
           "3. Osobine reci\n"
           "4. Simulacija igre\n"
           "5. Obrisi trenutni graf\n\n"
           "6. Kreiraj prazan graf\n"
           "7. Dodaj cvor\n"
           "8. Obrisi cvor\n"
           "9. Dodaj granu\n"
           "10. Obrisi granu\n\n"
           "0. Kraj programa\n\n"
           "Izaberite opciju: ");
}

int topologicalSort(Graph *graph) {
    if (graph == NULL) return 0;
    int *inDegree = calloc(graph->n, sizeof(int));
    int *queue = malloc(graph->n * sizeof(int));
    int *order = malloc(graph->n * sizeof(int));

    if (inDegree == NULL || queue == NULL || order == NULL) {
        free(inDegree);
        free(queue);
        free(order);
        return 0;
    }

    for (int i = 0; i < graph->edgeCount; i++) {
        inDegree[graph->edges[i]]++;
    }

    int front = 0, rear = 0;
    for (int i = 0; i < graph->n; i++) {
        if (inDegree[i] == 0) queue[rear++] = i;
    }

    int count = 0;
    while (front != rear) {
        int tmp = queue[front++];
        order[count++] = tmp;

        for (int i = graph->indices[tmp]; i < graph->indices[tmp + 1]; i++) {
            int v = graph->edges[i];
            inDegree[v]--;

            if (inDegree[v] == 0) {
                queue[rear++] = v;
            }
        }
    }

    if (count != graph->n) {
        printf("\n\nGraf ima ciklus!\n\n");
        free(inDegree);
        free(queue);
        free(order);
        return 0;
    }

    for (int i = 0; i < count; i++) {
        printf("%d(%s)\n", order[i], graph->words[order[i]]);
    }

    free(inDegree);
    free(queue);
    free(order);
    return 1;
}

int main() {
    srand(time(NULL));
    int a;
    Graph *graph = NULL;
    int graphCreated = 0;

    while (1) {
        printMenu();
        if (scanf("%d", &a) != 1) {
            printf("\nNevalidna opcija!\n\n");
            while (getchar() != '\n');
            continue;
        }
        switch (a) {
            case 1: {
                destroyGraph(graph);
                graph = NULL;
                graphCreated = 0;
                char buffer[100];
                printf("\n\nUnesi ime fajla:\n");
                if (scanf("%99s", buffer) != 1) {
                    printf("\nNevalidan unos!\n\n");
                    while (getchar() != '\n');
                    continue;
                }
                graph = loadDictionary(buffer);
                if (graph == NULL) {
                    printf("\n\nGreska pri otvaranju fajla ili formiranju grafa!\n\n");
                    continue;
                }

                graphCreated = 1;
                break;
            }
            case 2:
                if (!graphCreated) {
                    printf("\n\nGraf nije ucitan!\n\n");
                    continue;
                }
                printGraph(graph);
                break;
            case 3: {
                if (!graphCreated) {
                    printf("\n\nGraf nije ucitan!\n\n");
                    continue;
                }
                printf("\n\nUnesi rec:\n");
                char buffer[100];
                if (scanf("%99s", buffer) != 1) {
                    printf("\nNevalidan unos!\n\n");
                    while (getchar() != '\n');
                    continue;
                }
                if (!printWordProperties(graph, buffer)) printf("\nNevalidna rec ili ne postoji u recniku!\n\n");
                break;
            }
            case 4:
                if (!graphCreated) {
                    printf("\n\nGraf nije ucitan!\n\n");
                    continue;
                }
                simulateGame(graph);
                break;
            case 5:
                destroyGraph(graph);
                graph = NULL;
                graphCreated = 0;
                break;
            case 6: {
                destroyGraph(graph);
                graph = NULL;
                graphCreated = 0;

                int n;
                if (scanf("%d", &n) != 1) {
                    printf("\nNevalidna opcija!\n\n");
                    while (getchar() != '\n');
                    continue;
                }

                graph = createGraph(n, n*n);
                if (graph == NULL) {
                    printf("\n\nGreska pri kreiranju grafa!\n\n");
                    continue;
                }

                char buffer[100];
                for (int i = 0; i < n; i++) {
                    printf("Unesite rec za cvor %d: ", i);
                    if (scanf("%99s", buffer) != 1) {
                        printf("\nNevalidan unos!\n\n");
                        destroyGraph(graph);
                        graph = NULL;
                        graphCreated = 0;
                        while (getchar() != '\n');
                        break;
                    }
                    if (!addVertex(graph, buffer)) {
                        printf("\nGreska pri dodavanju cvora!\n\n");
                        destroyGraph(graph);
                        graph = NULL;
                        graphCreated = 0;
                        break;
                    }
                }

                if (graph != NULL) graphCreated = 1;
                break;
            }
            case 7: {
                if (!graphCreated) {
                    printf("\n\nGraf nije ucitan!\n\n");
                    continue;
                }
                char buffer[100];
                printf("Unesite rec za novi cvor: ");
                if (scanf("%99s", buffer) != 1) {
                    printf("\nNevalidan unos!\n\n");
                    while (getchar() != '\n');
                    continue;
                }
                if (findWord(graph, buffer) != -1) {
                    printf("\nCvor sa tim imenom vec postoji!\n\n");
                    continue;
                }
                if (!addVertex(graph, buffer)) {
                    printf("\nGreska pri dodavanju cvora!\n\n");
                }
                break;
            }
            case 8: {
                if (!graphCreated) {
                    printf("\n\nGraf nije ucitan!\n\n");
                    continue;
                }
                char buffer[100];
                printf("Unesite rec koju zelite da obrisete: ");
                if (scanf("%99s", buffer) != 1) {
                    printf("\nNevalidan unos!\n\n");
                    while (getchar() != '\n');
                    continue;
                }

                int vert = findWord(graph, buffer);
                if (vert == -1) {
                    printf("\nZadata rec ne postoji u grafu!\n\n");
                    continue;
                }
                if (!removeVertex(graph, vert)) {
                    printf("\nGreska pri uklanjanju cvora!\n\n");
                }
                break;
            }
            case 9: {
                if (!graphCreated) {
                    printf("\n\nGraf nije ucitan!\n\n");
                    continue;
                }
                char buffer[100];
                int p, d;
                printf("Unesite rec - izvor: ");
                if (scanf("%99s", buffer) != 1) {
                    printf("\nNevalidan unos!\n\n");
                    while (getchar() != '\n');
                    continue;
                }
                if ((p = findWord(graph, buffer)) == -1) {
                    printf("\nZadata rec ne postoji u grafu!\n\n");
                    continue;
                }
                printf("Unesite rec - odrediste: ");
                if (scanf("%99s", buffer) != 1) {
                    printf("\nNevalidan unos!\n\n");
                    while (getchar() != '\n');
                    continue;
                }
                if ((d = findWord(graph, buffer)) == -1) {
                    printf("\nZadata rec ne postoji u grafu!\n\n");
                    continue;
                }
                if (!addEdge(graph, p, d)) {
                    printf("Grana vec postoji, ili greska pri dodavanju grane!\n\n");
                }
                break;
            }
            case 10: {
                if (!graphCreated) {
                    printf("\n\nGraf nije ucitan!\n\n");
                    continue;
                }
                char buffer[100];
                int p, d;
                printf("Unesite rec - izvor: ");
                if (scanf("%99s", buffer) != 1) {
                    printf("\nNevalidan unos!\n\n");
                    while (getchar() != '\n');
                    continue;
                }
                if ((p = findWord(graph, buffer)) == -1) {
                    printf("\nZadata rec ne postoji u grafu!\n\n");
                    continue;
                }
                printf("Unesite rec - odrediste: ");
                if (scanf("%99s", buffer) != 1) {
                    printf("\nNevalidan unos!\n\n");
                    while (getchar() != '\n');
                    continue;
                }
                if ((d = findWord(graph, buffer)) == -1) {
                    printf("\nZadata rec ne postoji u grafu!\n\n");
                    continue;
                }
                if (!removeEdge(graph, p, d)) {
                    printf("Grana ne postoji, ili greska pri uklanjanju grane!\n\n");
                }
                break;
            }
            case 11: {
                if (!graphCreated) {
                    printf("\n\nGraf nije ucitan!\n\n");
                    continue;
                }
                topologicalSort(graph);
                break;
            }
            case 0:
                destroyGraph(graph);
                return 0;
            default:
                printf("\nNevalidna opcija!\n\n");
        }
    }

    return 0;
}