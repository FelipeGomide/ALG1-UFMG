#include <vector>
#include <stdio.h>
#include <math.h>
#include <climits>

using namespace std;

//Global variables
int n_rows, n_columns, n_crystals, mask_size;

class Crystal{
    public:
        int _shine;
        bool _up, _down, _left, _right;

        Crystal(int shine, bool up, bool down, bool left, bool right);
        Crystal();
};

Crystal::Crystal(){
    _shine = -1;
}

Crystal::Crystal(int shine, bool right, bool up, bool left, bool down){
    _shine = shine;
    _up = up;
    _down = down;
    _left = left;
    _right = right;
}

void read_inputs(int n_crystals, vector<vector<Crystal>> &grid){
    for (int i = 0; i < n_crystals ; i++){
        int x, y, s, r, u, l, d;
        scanf("%d %d %d %d %d %d %d", &x, &y, &s, &r, &u, &l, &d);

        grid[x][y] = Crystal(s, r, u, l, d);
    }
}

void print_grid(vector<vector<Crystal>> &grid){
    for (unsigned long int i = 1; i < grid.size() ; i++){

        for (unsigned long int j = 1; j < grid[i].size() ;  j++){
            Crystal atual = grid[i][j];
            (atual._up) ? printf(" | \t") : printf("   \t");
        }
        printf("\n");

        for (unsigned long int j = 1; j < grid[i].size() ;  j++){
            Crystal atual = grid[i][j];
            if (atual._shine == -1) printf(" * \t");
            else {
                (atual._left) ? printf("-") : printf(" ");
                printf("%d", grid[i][j]._shine);
                (atual._right) ? printf("-\t") : printf(" \t");
            }
        }
        printf("\n");

        for (unsigned long int j = 1; j < grid[i].size() ;  j++){
            Crystal atual = grid[i][j];
            (atual._down) ? printf(" | \t") : printf("   \t");
        }
        printf("\n");
    }
}

//Checa se um par de máscaras é valido
bool valid_next_mask(int row, int mask_actual, int mask_next, vector<vector<Crystal>> &grid){
    int equal = mask_actual & mask_next;

    //Itera pelos bits da mask
    for (int i = 0; i < n_columns; i++){

        //Se um mesmo bit for 1 em duas, e elas se ligam, retorna 0
        if (((equal>>i) & 1) && (grid[row][n_columns-i]._down == 1)){
            return 0;
        }
    }
    return 1;
}

bool valid_self_mask(int row, int mask, vector<vector<Crystal>> &grid){
    //Checa o lado direito de todas colunas até a penúltima
    for (int i = 0; i < n_columns -1; i++){
        if ((mask >> i) & 1){
            //Posição do grid vazia foi escolhida
            if(grid[row][n_columns-i]._shine == -1) return 0;
        }

        if (((mask >> i) & 1) && (mask >> (i+1)) & 1){
            //Duas posições vizinhas escolhidas
            if(grid[row][n_columns-i]._left == 1) return 0;
        }
    }

    //Checa a primeira e a última coluna
    if ((mask & 1) && ((mask >> (n_columns-1)) & 1)){
        if(grid[row][1]._left == 1) return 0;
    }

    return 1;
}

int get_brilho(int row, int row_mask, vector<vector<Crystal>> &grid){
    int brilho = 0;
    for (int i = 0; i < n_columns; i++){
        if((row_mask >> i) & 1){
            brilho += grid[row][n_columns-i]._shine;
        }
    }
    return brilho;
}

int f(int row, int row_mask, int first_mask, vector<vector<vector<int>>> &dp, vector<vector<Crystal>> &grid){
    //Busca pelo elemento na DP, se já foi calculado
    int search = dp[row][row_mask][first_mask];
    if (search != -1){
        return search;
    }   

    //Se a mask for inválida, retorna 0
    if (!valid_self_mask(row, row_mask, grid)){
        dp[row][row_mask][first_mask] = INT_MIN;
        return INT_MIN;
    }

    int brilho = get_brilho(row, row_mask, grid);

    //Se não for a última coluna
    if (row < n_rows){
        //Busca o maior dos filhos
        int max = -1;
        int aux = -1;
        //Para todas combinações da bitmask
        for (int i = 0; i < mask_size; i++){
            //Se a combinação é válida
            if (valid_next_mask(row, row_mask, i, grid)){
                aux = f(row+1, i, first_mask, dp, grid);
                //Guarda o maior filho
                if (aux > max) max = aux;
            }
        }

        dp[row][row_mask][first_mask] = brilho + max;
        return brilho + max;
    }

    //Checa se a última coluna é valida com a escolha da primeira
    if (!valid_next_mask(row, row_mask, first_mask, grid)){
        dp[row][row_mask][first_mask] = INT_MIN;
        return INT_MIN;
    }

    dp[row][row_mask][first_mask] = brilho;
    return brilho;
}

int first_call(vector<vector<vector<int>>> &dp, vector<vector<Crystal>> &grid){
    int max = -1;
    int aux = -1;
    for (int i = mask_size -1; i >= 0; i--){
        aux = f(1, i, i, dp, grid);
        if (aux > max) max = aux;
    }
    return max;
}

int main(){
    scanf("%d %d %d", &n_rows, &n_columns, &n_crystals);
    mask_size = pow(2, n_columns);

    vector<vector<Crystal>> *grid = new vector<vector<Crystal>>(n_rows+1, vector<Crystal>(n_columns+1));
    vector<vector<vector<int>>> *dp;
    dp = new vector<vector<vector<int>>>(n_rows+1, vector<vector<int>>(mask_size+1, vector<int>(mask_size+1, -1)));

    read_inputs(n_crystals, *grid);
    print_grid(*grid);

    int answer = first_call(*dp, *grid);

    printf("%d", answer);
}
