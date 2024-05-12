#include <stdio.h>
#include <queue>
#include <vector>

using namespace std;

int N, M, J, T, K;

const int inf = 0x3f3f3f3f;

vector<int> monstros;
vector<vector<int>> trajetos;

vector<int> *trajeto_jogador;

class simple_vertex{
    public:
        vector<int> adj;
        int dist = inf;
        int pai;
        bool usado;
};

class vertex{
    public:
        vector<pair<int,int>> adj;

        vector<bool> cor;
        bool trancado; //Existe um monstro estático

        vector<bool> disponibilidade; //Instantes de tempo com monstro ñ estático
        vector<int> pais; //Pai em cada instante de tempo
        vector<int> dist; //Dist em cada instante de tempo

        vertex();
};

vertex::vertex(){
    cor = vector<bool>(T+1, 0);
    trancado = false;
    disponibilidade = vector<bool>(T+1, 0);
    pais = vector<int>(T+1, 0);
    dist = vector<int>(T+1, inf);
}

vector<vertex> *graph;
vector<simple_vertex> *transposto;

void le_monstros(int j){
    int aux;
    for (int i = 0; i < j; i++){
        scanf("%d", &aux);
        monstros.push_back(aux);
    }
}

void popula_grafos(vector<vertex> &graph, vector<simple_vertex> &transposto){
    for (int i = 0; i < M; i++){
        int x,y,c;
        scanf("%d%d%d", &x, &y, &c);

        graph[x].adj.push_back(pair<int,int>(y,c));

        transposto[y].adj.push_back(x);
    }
    for (int i = 1; i <= N; i++){
        graph[i].adj.push_back(pair<int,int>(i,1));
    }
}

void BFS(vector<simple_vertex> &g, int s){ //Essa parece mais certa
    queue<int> fila;
    fila.push(s);

    g[s].usado = 1;
    g[s].pai = -1;
    g[s].dist = 0;

    while(!fila.empty()){
        int v = fila.front();
        fila.pop();

        for (int u : g[v].adj){
            int p;
            p = g[u].pai;

            if (!g[u].usado || (p > v && g[p].dist == g[v].dist)){

                g[u].usado = true;
                fila.push(u);

                g[u].dist = g[v].dist + 1;
                g[u].pai = v;
                //printf("Acessado: %d, dist:%d, pai:%d\n", u, g[u].dist, v);
            }
        }
    }
}

vector<int> caminho_BFS(vector<simple_vertex> &g, int u){
    vector<int> caminho;
    
    if (!g[u].usado){
        // printf("\n1 %d", u);
        caminho.push_back(u);
        return caminho;
    }
    
    for (int v = u; v != -1; v = g[v].pai)
        caminho.push_back(v);
    return caminho;

    // printf("\n%zu", caminho.size());
    
    // for(int i: caminho)
    //     printf(" %d", i);
}

void trajeto_monstros(){
    for (int i : monstros){
        vector<int> trajeto;
        trajeto = caminho_BFS(*transposto, i);

        trajetos.push_back(trajeto);
    }
}

void imprime_monstros(){
    for (vector<int> i : trajetos){
        unsigned long tam = i.size();

        if(tam == 1){
            printf("\n1 %d",i[0]);
            continue;
        }

        printf("\n%zu", tam);
        for (int j : i){
            printf(" %d", j);
        }
    }
}

void marca_bloqueios(vector<vertex> &g){
    for (vector<int> i : trajetos){
        unsigned long tam = i.size();

        if(tam == 1){
            g[i[0]].trancado = 1;
            continue;
        }

        for (unsigned int j = 0 ; j < i.size() ; j++){
            int local = i[j];

            g[local].disponibilidade[j] = 1;
        }
    }

    bool chegou = 0;
    for (int k = 0; k <= T ; k++){
        if (chegou) g[1].disponibilidade[k] = true;
        if (g[1].disponibilidade[k] == 1) chegou = true;
    }
};

class node{
    public:
        int dist;
        int turn;
        int vertex;
        int res; //resources

        node(int d, int t, int v, int r){
            dist = d;
            turn = t;
            vertex = v;
            res = r;      
        };      
};

bool operator>(node const& obj1, node const& obj2){
    if (obj1.dist == obj2.dist) return (obj1.turn > obj2.turn);
    return (obj1.dist > obj2.dist);
};

//Função auxiliar do Dijkstra, confere se o passo pode ser realizado.
bool acessivel(vector<vertex> &g, node n, pair<int,int> u){
    if (n.res < u.second) return 0; //Sem recursos suficientes

    if (g[u.first].trancado) return 0; //Monstro estático

    if (g[u.first].disponibilidade[n.turn]) return 0; //Monstros móveis
    if (g[u.first].disponibilidade[n.turn+1]) return 0;

    if (n.turn == T) return 0; //Acabaram turnos

    return 1;
}

void Dijkstra(vector<vertex> &g, int s){
    priority_queue<node, vector<node>, greater<node>> f; //distância, turno, índice
    g[s].dist[0] = 0;
    
    f.push(node(0,0,s,K));

    if (g[s].trancado) return;
    if (g[s].disponibilidade[0] == 1) return;

    while (!f.empty()){
        node aux = f.top();
        f.pop();

        int v = aux.vertex;
        int d = aux.dist;
        int t = aux.turn;

        if(g[v].dist[t] < d && g[v].cor[t])
            continue;
        
        g[v].cor[t] = 1;

        for(pair<int,int> u : g[v].adj){
            int distu =  g[u.first].dist[t+1]; 
            int distv =  g[v].dist[t];
            int w = u.second;

            if(acessivel(g, aux, u) && distu > distv + w){

                g[u.first].dist[t+1] = distv + w;
                distu = g[u.first].dist[t+1];

                g[u.first].pais[t+1] = v;

                f.push(node(distu, t+1, u.first, aux.res-w+K));
            }
        }
    }
};

class resposta{
    public:
        int turn = 0;
        int dist = inf;
        int vertex = 0;
        bool venceu = 0;
        bool morreu = 0;
};

void confere_vitoria(vector<vertex> &g, resposta &r){
    int min = inf;
    int time = 0;

    for(int i = 1; i <= T ; i++){
        if(g[N].dist[i] < min){
            min = g[N].dist[i];
            time = i;
        }
    }

    if (min != inf) {
        r.venceu = 1;
        r.turn = time;
        r.dist = min;
        r.vertex = N;
    }
    return;
}

void melhor_derrota(vector<vertex> &g, resposta &r){
    int min_dist = inf;
    int opt_vertex = 0;
    int max_turn = 0;

    for (int t = T; t > 0 ; t--){
        for (int n = 1; n <= N; n++){
            if (g[n].dist[t] < min_dist){
                min_dist = g[n].dist[t];
                opt_vertex = n;
                max_turn = t;
            }
        }
        if (min_dist != inf) break;
    }

    if (max_turn != T) r.morreu = 1;

    if(min_dist == inf) { 
        //Caso de borda, jogador morre no turno 1, e fica preso no vértice 1
        min_dist = 1;
        max_turn = 1;
        opt_vertex = 1;
        r.morreu = 0;
    }

    r.vertex = opt_vertex;
    r.dist = min_dist;
    r.turn = max_turn;

    return;
}

void trajeto_resposta(vector<vertex> &g, resposta &r, vector<int> &t){
    t[0] = 1;

    int atual = r.vertex;
    int proximo;

    for (int i = r.turn; i > 0 ; i--){
        int proximo = g[atual].pais[i];

        t[i] = atual;
        atual = proximo;
    }
}

void imprime_jogador(resposta &r){
    if(r.morreu){
        printf("\n%d %d\n", r.dist+1, r.turn+1);
    }
    else{
        printf("\n%d %d\n", r.dist, r.turn);
    }

    for (auto i : *trajeto_jogador){
        printf("%d ", i);
    }

    if(r.morreu)
        printf("%d", trajeto_jogador->back());
}

int main(){
    scanf("%d%d%d%d%d", &N,&M,&J,&T,&K);

    graph = new vector<vertex>(N+1);
    transposto = new vector<simple_vertex>(N+1);

    le_monstros(J); //Recebe e guarda os endereços iniciais dos monstros
    popula_grafos(*graph, *transposto); //Recebe entradas e cria lista adj do normal e transposto

    BFS(*transposto, 1); //Faz a BFS descobrindo as distancias no transposto
    trajeto_monstros(); //Descobre e guarda o trajeto de cada monstro

    delete transposto;

    marca_bloqueios(*graph);
    Dijkstra(*graph, 1);

    resposta *resultado = new resposta;

    confere_vitoria(*graph, *resultado);
    
    if(resultado->venceu == 0){
        melhor_derrota(*graph, *resultado);
    }

    trajeto_jogador = new vector<int>(resultado->turn+1);
    trajeto_resposta(*graph, *resultado, *trajeto_jogador);

    printf("%d", resultado->venceu); //Primeiro valor se é possível chegar no fim
    imprime_monstros(); //Imprime os trajetos dos monstros
    imprime_jogador(*resultado);

    delete graph;
}