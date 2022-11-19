#include <iostream>
#include <list>
#include <string>
#include <tuple>
#include <algorithm>

using namespace std;

const int MAX_GENERATIONS = 100;
const int NUM_OBJS = 6;
const int POP_SIZE = 100;
const int MAX_CARGO = 10;

class ValCar {
    public:
        int valor;
        int carga;
        ValCar(int val, int car){
            valor = val;
            carga = car;
        }
};

const ValCar OBJ_ATR[NUM_OBJS] = {ValCar(1,3), ValCar(2,3), ValCar(3,5), ValCar(4,2), ValCar(10,1), ValCar(5,5)};

class Solution {
    public:
        int val[NUM_OBJS];
        Solution(){
            for(int i = 0; i < NUM_OBJS; i++)val[i]=rand() % 2;
        }
        string show(){
            string st = "";
            for(int i = 0; i < NUM_OBJS; i++) st += to_string(val[i]);
            return st;
        }
        int fitness(){
            int sum = 0, car = 0;
            for (int i = 0; i < NUM_OBJS; i++){
                sum += val[i]*OBJ_ATR[i].valor;
                car += val[i]*OBJ_ATR[i].carga;

            }
            if(car > MAX_CARGO){
                makeValid();
                return this->fitness();
            }
            return sum;
        }
        int cargo(){
            int sum = 0;
            for (int i = 0; i < NUM_OBJS; i++)sum += val[i]*OBJ_ATR[i].carga;
            return sum;
        }
        void makeValid(){ // if solution is not valid, make it so
            srand(time(NULL));
            while (this->cargo() > MAX_CARGO){
                int r = rand() % (NUM_OBJS); // random from 0 to 5
                if(this->val[r]) this->val[r] = false;
            }
        }
        bool operator < (Solution &obj){
            return this->fitness() < obj.fitness();
        }
};

Solution mutate(Solution s, int p){
    srand(time(NULL));
    for(int i = 0; i < NUM_OBJS; i++){
        int r = (rand() % 101) + 1;
        if(p > r) s.val[i] = s.val ? false : true;
    }
    return s;
}

std::tuple<Solution, Solution> crossover(Solution s1, Solution s2) {
    srand(time(NULL));
    Solution r1 = s1;
    Solution r2 = s2;

    int cross_point = rand() % (NUM_OBJS);
    for(int i = 0; i < NUM_OBJS; i++){
        r1.val[i] = (i < cross_point)? s1.val[i] : s2.val[i];
        r2.val[i] = (i < cross_point)? s2.val[i] : s1.val[i];
    }

    return {r1, r2};    
}

bool checkConvergence (Solution pop[POP_SIZE], float f){
    int parcel = (int)(POP_SIZE * f);
    list<std::tuple<int, int>> dict_list = {};

    for(int i = 0; i < POP_SIZE; i++){
        int fit = pop[i].fitness();
        
        bool found = false;
        for (tuple<int, int> &tup : dict_list){
            if (get<0>(tup) == fit){
                get<1>(tup) += 1;
                found = true;
                std::cout << "Parada é: " << parcel << " current count: " << std::get<1>(tup) << "\n";
                if(get<1>(tup) >= parcel) return true;
            }
        }
        if(!found){
            std::tuple<int, int> t = {fit, 1};
            dict_list.push_back(t);
            
            std::cout << "Did not find fitness: " << std::get<0>(t) << " current count: " << std::get<1>(t) << "\n";
        }
    }
    return false;
}

void newGen(Solution pop[POP_SIZE], int selected[2]){
    
}

void rouletteSelection (Solution pop[POP_SIZE]) {
    int selected[2] = {-1,-1};
    int total_fitness = 0;
    
    for(int i = 0; i < POP_SIZE; i++)total_fitness += pop[i].fitness();

    srand(time(NULL));
    int r = rand() % POP_SIZE;

    int sum = 0;
    while (sum < total_fitness){
        if(r >= POP_SIZE)r=0;
        sum += pop[r].fitness();
        
        //cout << "r = " << r << " | sum = " << sum << " | total_fitness = " << total_fitness << "\n"; 
        if(sum >= total_fitness){

            if(selected[0] < 0){
                selected[0] = r;
                sum = 0;
                //srand(time(NULL));
                r = rand() % POP_SIZE;
            }
            if(selected[1] < 0 && r != selected[0]){
                selected[1] = r;
                break;
            }
            else {
                //srand(time(NULL));
                r = rand() % POP_SIZE;
            }
        }
        r++;
    }

    newGen(pop, selected);
    //std::cout << "Selected randomly: " << selected[0] << " and " << selected[1] << "\n";
}

void GA(){
    // read numbers
    // no need :)

    // create random solutions
    srand(time(NULL));
    Solution pop[POP_SIZE]; 
    for(int i = 0; i < POP_SIZE; i++)pop[i] = Solution(); 

    // check for convergence
    // if(checkConvergence(pop, 0.9))return; // this line will be commented for a while
    
    // create next generation
    rouletteSelection(pop);
}

int main() {
    srand(time(NULL));

    Solution pop[POP_SIZE]; for(int i = 0; i < POP_SIZE; i++)pop[i] = Solution();
    std::cout << "Show: " << pop[0].show() << " fit: " << pop[0].fitness() << " car: " << pop[0].cargo() << "\n";


    rouletteSelection(pop);
    std::cout << "Hi";

    return 0;
}