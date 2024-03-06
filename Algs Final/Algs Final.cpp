#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <iomanip>
#include <vector>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;
// Number of vertices in the graph
#define V 23

int NO_PARENT = -1;

/*
Can be used to wait for an input to not clear screen
*/
void WaitForEnter() {
    char c = getchar();
    while (c != '\n')
        c = getchar();
}

/*
Converts a character to an integer that can be used in the adjacency matrix
*/
int charToInt(char c) {
    if (c >= 'a') {
        return c - 'a';
    }
    else {
        return c - 'A';
    }
}

/*
Loads data from a txt file and converts to an adjacency matrix (each cell is a distance,
with the collumn denoting destination, and the row denoting starting point.
*/
void loadData(vector<vector<int>> &map) {
    string line;
    string delimiter = ",";
    ifstream file("MapData.txt");
    int i = 0;
    int j = 0;
    size_t pos = 0;
    string currSeg;
    vector<vector<int>> outVector;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            vector<int> currLine;
            while ((pos = line.find(delimiter)) != std::string::npos) {
                currSeg = line.substr(0, pos);
                currLine.push_back(stoi(currSeg));
                line.erase(0, pos + delimiter.length());
                j++;
            }
            outVector.push_back(currLine);
            i++;
        }
        map = outVector;
    }
}

/*
Outputs the shortest path, placing "->" between points that arent the last one
*/
void printPath(int currentVertex, vector<int> parents, int i){
    if (currentVertex == NO_PARENT) {
        return;
    }
    printPath(parents[currentVertex], parents, i+1);
    if (i==0) {

        cout << currentVertex << " ";
    }
    else {
        cout << currentVertex << " -> ";
    }
}

void printSolution(int startVertex, vector<int> distances,vector<int> parents, int destVertex)
{
    int nVertices = distances.size();
    printPath(destVertex, parents, 0);
}

/*
This function uses Dijkstra's Algorithm to find shortest path to another given point 
*/
void shortestPath(vector<vector<int> > map, int startVertex, int destVertex)
{
    int nVertices = map[0].size(); //Gets number of vertices in the map
    vector<int> shortestDistances(nVertices); 
    vector<bool> added(nVertices);
    for (int index = 0; index < nVertices; index++) { //Sets default values
        shortestDistances[index] = INT_MAX;
        added[index] = false;
    }
    shortestDistances[startVertex] = 0; //Sets the start distance's distance to itself as 0
    vector<int> parents(nVertices); 
    parents[startVertex] = NO_PARENT; //Sets there to be no vertices leading to the start point
    for (int i = 1; i < nVertices; i++) { //The bulk of Dijkstra's algorithm
        int nearestVertex = -1;
        int shortestDistance = INT_MAX;
        for (int index = 0; index < nVertices; index++) {
            if (!added[index] && shortestDistances[index] < shortestDistance) {
                nearestVertex = index;
                shortestDistance = shortestDistances[index];
            }
        }
        added[nearestVertex] = true;
        for (int index = 0; index < nVertices;
            index++) {
            int edgeDistance = map[nearestVertex][index];

            if (edgeDistance > 0 && ((shortestDistance + edgeDistance) < shortestDistances[index])) {
                parents[index] = nearestVertex;
                shortestDistances[index]= shortestDistance + edgeDistance;
            }
        }
    }
    printSolution(startVertex, shortestDistances, parents, destVertex); //Outputs the shortest path
}
int main()
{
    bool running = true;
    char choice = '\0';
    char stations[4] = { 'h','k', 'q', 't' };
    vector<vector<int>> map;
    loadData(map);

    while (running) { //Keeps program looping until it is told not to
        if (choice == 'y' || choice == 'Y' || choice == '\0') {
            cout << "Choose Starting Point: \n";
            cin >> choice;
            for (char c : stations) { //Runs the algorithm for each charging station
                cout << "Shortest Path to " << char(toupper(c)) << ": ";
                shortestPath(map, charToInt(choice), charToInt(c));
                cout << "\n";
            }
            cout << "Press Enter to Continue";
            char c=getchar();
            WaitForEnter();
            system("CLS");
        }
        else if (choice == 'n' || choice == 'N') {
            running = false;
            system("CLS");
        }
        else {
            cout << "Choose New Point? (Y/N) \n";
            cin >> choice;
            system("CLS");
        }
    }
    return 0;
}