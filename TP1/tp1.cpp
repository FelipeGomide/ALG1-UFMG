#include <cstdio>
#include <vector>
#include <set>
#include <stack>
#include <stdio.h>

using namespace std;

int N;
int M;

set<set<int>> clusters;
set<int> cuts;
set<pair<int,int>> forest;
static int index_count = 0;

class Vertex{
    public:
        const int index;
        int disc = -1;
        int low = -1;
        Vertex* pai = nullptr;
        bool visited = false;

        vector<int> adj;

        Vertex();
};

Vertex::Vertex() : index(index_count++){};

void create_graph(vector<Vertex> &graph){
    int u;
    int v;

    for(int i = 0; i < M; i++){
        scanf("%d %d", &u, &v);

        graph[v].adj.push_back(u);
        graph[u].adj.push_back(v);
    }
}

void print_graph(vector<Vertex> &graph){
    for (long unsigned int v = 1; v < graph.size(); v++){
        printf("Vértice: %d\nAdjacentes:\n", graph[v].index);
        for (int u : graph[v].adj){
            printf("%d\n", u);
        }
    }
}

void make_cluster_pair(stack<pair<int,int>> &s, int u, int v){
    set<int> c;
    pair<int,int> aux;

    do {
        aux = s.top();
        s.pop();
        c.insert(aux.first);
        c.insert(aux.second);
    }while (aux != pair<int,int>{u,v});

    clusters.insert(c);
}

void make_cluster_clean(stack<pair<int,int>> &s){
    set<int> c;
    pair<int,int> aux;

    while(s.size() > 0){
        aux = s.top();
        s.pop();
        c.insert(aux.first);
        c.insert(aux.second);
    }

    clusters.insert(c);
}

void make_cluster_root(stack<pair<int,int>> &s, int u){
    set<int> c;
    pair<int,int> aux;

    while(s.size() > 0){
        do{
            aux = s.top();
            s.pop();
            c.insert(aux.first);
            c.insert(aux.second);
        }while(aux.first != u);

        clusters.insert(c);
        c.clear();
    }
}

void DFS_visit(vector<Vertex> &graph, Vertex &u, stack<pair<int,int>> &s, int &count){
    u.visited = true;
    //printf("Visited %d\n", u.index);
    count += 1;
    u.low = u.disc = count;

    int children = 0;

    for (int i : u.adj){
        Vertex *v = &graph[i];
        if (u.pai == v) continue;
        if (!v->visited){
            children++;

            s.push({u.index,v->index});
            v->pai = &u;

            DFS_visit(graph, *v, s, count);

            u.low = min(u.low, v->low);

            if(v->low >= u.disc && u.pai != nullptr){
                cuts.insert(u.index);
                make_cluster_pair(s, u.index, v->index); //Biconnected component found
            } 
            
        }

        else if ((u.pai != v && v->disc < u.disc)){
            s.push({u.index,v->index});
            u.low = min(u.low, v->disc);
        }
    }
    if (u.pai == nullptr && children > 1){
        cuts.insert(u.index);
        make_cluster_root(s, u.index);
    }
}

void bic_components(vector<Vertex> &graph){
    int *count = new int;
    *count = 0;

    stack<pair<int,int>> *s = new stack<pair<int,int>>;

    for (Vertex u : graph){
        u.visited = false;
        u.pai = nullptr;
    }

    for (long unsigned int i = 1; i < graph.size(); i++){
        Vertex *u = &graph[i];
        if (!u->visited) {
            DFS_visit(graph, *u, *s, *count);
            if(!s->empty()) make_cluster_clean(*s);
        }
    }
}

void write_answer(){

    //Printing vertex count and cut vertices indexes
    printf("%zu\n", cuts.size());
    for (auto v : cuts){
        printf("%d\n", v);
    }

    //Printing clusters
    printf("%zu\n", clusters.size());
    
    int cluster_id = N+1;
    for (set<int> i : clusters){ //Iterate the clusters printing their vertices

        printf("%d ", cluster_id);

        printf("%zu ", i.size());

        for(int j : i){
            printf("%d ", j);
            for(int v : cuts){
                if(v == j) forest.insert(pair<int,int>{v,cluster_id});
            }
        }
        printf("\n");
        cluster_id++;
    }

    //Printing the forest
    int z = cuts.size() + clusters.size();
    printf("%d %zu\n", z, forest.size());
    for (auto edge : forest){
        printf("%d %d\n", edge.first, edge.second);
    }
}

int main(){
    scanf("%d", &N);
    scanf("%d", &M);
    
    vector<Vertex> *graph = new vector<Vertex>(N+1);

    create_graph(*graph);

    //print_graph(*graph);

    bic_components(*graph);

    write_answer();

    delete graph;
    
}