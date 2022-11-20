#include <iostream>
#include <list>
#include <string>
#include <tuple>
#include <algorithm>

using namespace std;

const int MAX_GENERATIONS = 100;
const int NUM_OBJS = 12;
const int POP_SIZE = 50;
const int MAX_CARGO = 10;

class ValCar {
    public:
        int valor;
        int carga;
        ValCar(int val, int car){
            valor = val;
            carga = car;
        }
        ValCar(){
            valor = rand() % MAX_CARGO;
            carga = rand() % MAX_CARGO;
        }
};

const ValCar OBJ_ATR[NUM_OBJS] = {
    ValCar(), ValCar(), ValCar(), ValCar(), ValCar(), ValCar(), ValCar(), ValCar(), ValCar(), ValCar(), ValCar(), ValCar()
};

class Solution {
    public:
        int val[NUM_OBJS];
        Solution(){
            for(int i = 0; i < NUM_OBJS; i++)val[i]=rand() % 2;
        }
        string show(){
            string st = "";
            for(int i = 0; i < NUM_OBJS; i++) st += to_string(val[i]);
            return "Solution: " + st + " | fitness: " + to_string(this->fitness());
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
            while (this->cargo() > MAX_CARGO){
                int r = rand() % (NUM_OBJS); // random from 0 to 5
                if(this->val[r]) this->val[r] = false;
            }
        }
        void validate(){
            if(this->cargo() > MAX_CARGO){
                makeValid();
            }
        }
        bool operator < (Solution &obj){
            return this->fitness() < obj.fitness();
        }
};

Solution mutate(Solution s, int p){
    for(int i = 0; i < NUM_OBJS; i++){
        int r = (rand() % 101) + 1;
        if(p > r) s.val[i] = s.val ? false : true;
    }
    s.validate();
    std::cout << "Inside mutation: " << s.show() << "\n";
    return s;
}

std::tuple<Solution, Solution> crossover(Solution s1, Solution s2) {
    Solution r1 = s1;
    Solution r2 = s2;

    int cross_point = rand() % (NUM_OBJS);
    for(int i = 0; i < NUM_OBJS; i++){
        r1.val[i] = (i < cross_point)? s1.val[i] : s2.val[i];
        r2.val[i] = (i < cross_point)? s2.val[i] : s1.val[i];
    }
    
    std::cout << "Inside crossover, cp: " << cross_point << " \n";
    std::cout << "s1: " << s1.show() << " \n";
    std::cout << "s2: " << s2.show() << " \n";
    std::cout << "r1: " << r1.show() << " \n";
    std::cout << "r2: " << r2.show() << " \n";

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
                if(get<1>(tup) >= parcel) return true;
            }
            
        }
        if(!found){
            std::tuple<int, int> t = {fit, 1};
            dict_list.push_back(t);
        }
    }
    return false;
}

int rouletteSelection (Solution pop[POP_SIZE]) {
    int total_fitness = 0;
    for(int i = 0; i < POP_SIZE; i++)total_fitness += pop[i].fitness();

    int r = rand() % POP_SIZE, fr = r;

    int sum = 0;
    while (sum < total_fitness){
        if(r >= POP_SIZE-1)r=0;
        sum += pop[r].fitness();
        r++;
        if(r == fr && sum != 0)break;
    }
    return r;
}

int groupSelection4 (Solution pop[POP_SIZE]){
    int rl = rand() % 101; // from 0 to 3
    int r = rand() % POP_SIZE/4; // from 0 to 11   

    if(rl <= 50){ //rl = 3 50%
        rl = 3;
        r = rand() % (POP_SIZE - 3*POP_SIZE/4); // from 0 to 12
    }
    else if(rl <= 50+30 ){ //rl = 2 30% 
        rl = 2;
    }
    else if(rl <= 50+30+15){ //rl = 1 15%
        rl = 1;
    }
    else{ //rl = 0 5% 
        rl = 0;
    } 
    
    std::cout << rl*POP_SIZE/4 + r << "\n";
    return rl*POP_SIZE/4 + r;
}

void newGen(Solution pop[POP_SIZE]){
    
    std::list<Solution> parents;
    std::list<Solution> descendants;

    int s1 = rouletteSelection(pop);
    int s2 = rouletteSelection(pop);
    //int s1 = groupSelection4(pop);
    //int s2 = groupSelection4(pop);

    std::cout << "Inside newGen: \n";
    std::cout << "s1: " << pop[s1].show() << " \n";
    std::cout << "s2: " << pop[s2].show() << " \n";
    
    parents.push_back(pop[s1]);
    parents.push_back(pop[s2]);

    std::list<Solution>::iterator it = parents.begin();
    for(it = parents.begin(); it != parents.end(); it++){
        Solution p1 = *it;
        Solution p2 = *(++it);

        Solution c1, c2;
        std::tuple<Solution, Solution> t = crossover(p1, p2);
        c1 = std::get<0>(t); c2 = std::get<1>(t);
        mutate(c1, 50); mutate(c2, 50); 

        descendants.push_back(c1);
        descendants.push_back(c2);

    }

    // assumes sorted list
    int i = 0;
    std::cout << "Inside newGen, these are the new descendants: \n";
    for(it = descendants.begin(); it != descendants.end(); it++){
        std::cout << it->show() << "\n"; 
        pop[i] = *it;
        i++;
    }

}

void GA(){
    // read numbers
    // no need :)

    // create random solutions
    Solution pop[POP_SIZE]; 
    for(int i = 0; i < POP_SIZE; i++)pop[i] = Solution(); 

    for(int i = 1; i < MAX_GENERATIONS; i++){
        int n = sizeof(pop)/sizeof(pop[0]);
        sort(pop, pop +n);

        std::cout << "Generation " << i << ":\n";
        for(int i = 0; i < POP_SIZE; i++)std::cout << pop[i].show() << " \n";
        // check for convergence
        if(checkConvergence(pop, 0.9)){
            std::cout << "Converged \n";
            break;
        }
        
        // create next generation
        newGen(pop);

    }

    std::cout << "Last Generation: \n";
    for(int i = 0; i < POP_SIZE; i++)std::cout << pop[i].show() << " \n";
}

int main() {
    srand(time(NULL));

    Solution pop[POP_SIZE]; for(int i = 0; i < POP_SIZE; i++)pop[i] = Solution();
    
    int n = sizeof(pop)/sizeof(pop[0]);
    sort(pop, pop +n);

    //GA();

    while(true)groupSelection4(pop);

    return 0;
}