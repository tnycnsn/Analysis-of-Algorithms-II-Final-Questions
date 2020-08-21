#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

class Graph{
    vector <vector<int>> capacity;
    vector <vector<int>> flow;
    int nodeCount;
    vector <int> excess;
    vector <int> height;
    public:
    void addCapacity(int, int, int);
    void push(int, int);
    void relabel(int);
    void initializePreflow(int);
    int overflowingVertex();
    int findMaxFlow();
    Graph(int);
    ~Graph();
};

Graph::Graph(int n){
    nodeCount = n;
    capacity.assign(nodeCount, vector<int>(nodeCount, 0));
}

void Graph::addCapacity(int from, int to, int cap){
    capacity[from][to] = cap;
}

int Graph::overflowingVertex(){
    // IMPLEMENT HERE
    for (int i = 1; i < nodeCount-1; i++){ //since we dont interest in excesses of the sink and source
      if (excess[i] > 0) return i;
    }
    return -1;
}

void Graph::initializePreflow(int source){
    // IMPLEMENT HERE
    height = vector<int>(nodeCount, 0);
    excess = vector<int>(nodeCount, 0);

    flow.assign(nodeCount, vector<int>(nodeCount, 0));
    height[0] = nodeCount;
    excess[0] = numeric_limits<int>::max();

    for (int i = 1; i < nodeCount; i++){
      if (capacity[0][i] != 0) {
        flow[0][i] = capacity[0][i];
        excess[i] = capacity[0][i];
        excess[0] -= excess[i];
      }
    }
}

void Graph::push(int u, int v){
    // IMPLEMENT HERE
    int deltaUV = min(excess[u], capacity[u][v] - flow[u][v]);
    flow[u][v] += deltaUV;
    flow[v][u] -= deltaUV;
    excess[u] -= deltaUV;
    excess[v] += deltaUV;
}

void Graph::relabel(int u){
    // IMPLEMENT HERE
    int minH = numeric_limits<int>::max();
    for (int i = 0; i < nodeCount; i++) {
        if (capacity[u][i] - flow[u][i] > 0)
            minH = min(minH, height[i]);
    }
    if (minH < numeric_limits<int>::max())
        height[u] = minH + 1;
    else excess[u] = 0; //all exits are full so clear the excess to proceed
}

int Graph::findMaxFlow(){
    // IMPLEMENT HERE
    int overflowing = overflowingVertex(); //return -1 where there is no overflowing vertice
    while (overflowing != -1){
      bool relabelFlag = true;
      for (int v = 0; v < nodeCount; v++){
        if(capacity[overflowing][v] > 0 || flow[overflowing][v] != 0){ //edge exists
          if (capacity[overflowing][v] == flow[overflowing][v]) continue;
          if (height[overflowing] > height[v]){
            push(overflowing, v);
            relabelFlag = false;
          }
        }
      }
      if (relabelFlag) relabel(overflowing);
      //IF excess > 0 and all exits are full then clear the excess for that overflowingVertex

      overflowing = overflowingVertex();  //search for another overflowing
    }

    return excess[nodeCount-1];
}


Graph::~Graph(){

}

int main(int argumentNumber, char *arguments[]){
    int nodeCount;
    string filename = arguments[1];
    string tempString;
    int temp, from, to, flow;

    ifstream file;
    file.open(filename);
    file >> nodeCount;
    Graph graph(nodeCount);
    file >> tempString >> tempString >> tempString;
    while(file.good()){
        file >> from >> to >> flow;
        graph.addCapacity(from, to, flow);
    }
    graph.initializePreflow(0); // 0th node is source
    cout << "Maximum flow: " << graph.findMaxFlow() << endl;
    return 0;
}
