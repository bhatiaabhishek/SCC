//
//  main.cpp
//  SCC
//
//  Created by Abhishek Bhatia on 12/31/17.
//  Copyright © 2017 Abhishek Bhatia. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
using namespace std;

class Graph {
    
public:
    Graph(int num_nodes); // Constructor
    int num_nodes;
    vector<vector<int>> adj;
    vector<bool> visited;
    void DFS(int s,vector<int> &fin_time, int &t, map<int,vector<int>> &scc, int origin);
    
    
};

Graph::Graph(int num_nodes) {
    this->num_nodes = num_nodes;
    vector<int> empty;
    adj = vector<vector<int>> (num_nodes+1, empty); // index 0 is a dummy
    visited = vector<bool> (num_nodes+1,false); // Initialize all to false
}

void Graph::DFS(int s, vector<int> &fin_time, int &t, map<int,vector<int>> &scc, int origin) {
    visited[s] = true; // Set this node explored
    //leader[s] = origin;
    scc[origin].push_back(s);
    //cout << "visiting " << s << endl;
    vector<int>::iterator it;
    for(it=adj[s].begin(); it != adj[s].end();++it) {
        if (visited[*it] != true) {
            DFS(*it, fin_time, t, scc, origin);
        }
    }
    t++;
    fin_time[t] = s;
}

void DFS_Loop (Graph &G, vector<int> &fin_time, map<int, vector<int>> &scc) {
    int t = 0; //to keep track of finishing times
    int origin = 0;
    vector<int> sequence = fin_time;
    // The finishing time sequence tells us the order in which to loop over the vertices.
    
    vector<int>::reverse_iterator it_r;
    //cout << "size of fin = " << fin_time.size() << endl;
    for(it_r=sequence.rbegin();it_r!=sequence.rend();it_r++) {
        //cout << "Iterating over = " << *it_r << endl;
        if(!G.visited[*it_r]) {
            //cout << "Iterating over not visited = " << *it_r << endl;
            origin = *it_r;
            G.DFS(*it_r, fin_time, t, scc, origin);
        }
    }
}

map<int, vector<int>> find_scc(Graph &G, Graph &G_rev) {
    //vector<int> leader(G.num_nodes+1,0);
    vector<int> fin_time(G.num_nodes+1,0);
    map<int,vector<int>> scc;
    vector<int>::iterator it;
    int i = 0;
    for(it=fin_time.begin();it!=fin_time.end();++it) {
        
        *it = i++;
    }
    DFS_Loop(G_rev,fin_time,scc);
    scc.clear();
    DFS_Loop(G,fin_time,scc);
    
    
    
    return scc;
}
void Initialize_Graph (ifstream &File, Graph &Mygraph, Graph &Mygraph_rev) {
    
    string line;
    if (File.is_open()) {
        while(!File.eof())
        {
            getline(File,line);
            
            if (!line.empty()) {
                //cout << line << endl;
                stringstream ss(line);
                int start_node;
                int end_node;
                ss >> start_node;
                ss >> end_node;
                
                Mygraph.adj[start_node].push_back(end_node);
                Mygraph_rev.adj[end_node].push_back(start_node);
                }
            
        }
        File.close();
    }
    else {
        cout << "file not found" << endl;
    }
    
    //return Mygraph;
}

void PrintLargest(map<int,vector<int>> &scc) {
    
    map<int,vector<int>>::iterator it;
    vector<int> scc_sizes;
    for(it=scc.begin(); it != scc.end();++it) {
        if(it->second.size()>1) {
            scc_sizes.push_back(it->second.size());
        }
    }
    
    sort(scc_sizes.begin(),scc_sizes.end(), greater<int>());
    
    for (auto i = scc_sizes.begin();i!=scc_sizes.end();++i) {
        cout << *i << " ";
    }
}
int main(int argc, const char * argv[]) {
    
    ifstream File;
    File.open("/Users/abhatia/Documents/Algorithms/SCC/SCC.txt");
    //File.open("/Users/abhatia/Documents/Algorithms/SCC/testcase.txt");
    int n = 875714;
    Graph Mygraph(n);
    Graph Mygraph_rev(n);
    Initialize_Graph(File, Mygraph, Mygraph_rev);
    
    map<int,vector<int>> scc;
    scc = find_scc(Mygraph, Mygraph_rev);
    PrintLargest(scc);
    cout << endl;
    //Mygraph.DFS(3);
    //Mygraph_rev.DFS(3);
    return 0;
}
