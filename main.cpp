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
    for(int i = 0; i < NUM_OBJS; i++){
        int r = (rand() % 101) + 1;
        if(p > r) s.val[i] = s.val ? false : true;
    }
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
    
    //cout << "cp: " << cross_point << " \n";
    //cout << "s1: " << s1.show() << " \n";
    //cout << "s2: " << s2.show() << " \n";
    //cout << "r1: " << r1.show() << " \n";

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
        if(r >= POP_SIZE)r=0;
        sum += pop[r].fitness();
        r++;
        if(r == fr && sum != 0)break;
    }
    return r;
}

void newGen(Solution pop[POP_SIZE]){
    
    std::list<Solution> parents;
    std::list<Solution> descendants;

    int s1 = rouletteSelection(pop);
    int s2 = rouletteSelection(pop);
    
    parents.push_back(pop[s1]);
    parents.push_back(pop[s2]);

    std::list<Solution>::iterator it = parents.begin();
    for(it = parents.begin(); it != parents.end(); it++){
        Solution p1 = *it;
        Solution p2 = *(++it);

        Solution c1, c2;
        std::tuple<Solution, Solution> t = crossover(p1, p2);
        c1 = std::get<0>(t); c2 = std::get<1>(t);

        descendants.push_back(c1);
        descendants.push_back(c2);

        //std::cout << "p1: " << p1.show() << " p2: " << p2.show() << "\n";
        //std::cout << "c1: " << c1.show() << " c2: " << c2.show() << "\n";
    }

    int i = POP_SIZE - 1;
    for(it = descendants.begin(); it != descendants.end(); it++){
        pop[i] = *it;
        i--;
    }

    
}

void GA(){
    // read numbers
    // no need :)

    // create random solutions
    Solution pop[POP_SIZE]; 
    for(int i = 0; i < POP_SIZE; i++)pop[i] = Solution(); 

    int n = sizeof(pop)/sizeof(pop[0]);
    sort(pop, pop +n);

    // check for convergence
    // if(checkConvergence(pop, 0.9))return; // this line will be commented for a while
    
    // create next generation
    newGen(pop);
}

int main() {
    srand(time(NULL));

    Solution pop[POP_SIZE]; for(int i = 0; i < POP_SIZE; i++)pop[i] = Solution();
    std::cout << "Show: " << pop[0].show() << " fit: " << pop[0].fitness() << " car: " << pop[0].cargo() << "\n";

    newGen(pop);

    std::list<Solution> lpop; 
    lpop.insert(lpop.begin(), std::begin(pop), std::end(pop));
    std::list<Solution>::iterator it = lpop.begin();
    std::cout << "Show: " << it->show() << " fit: " << it->fitness() << " car: " << it->cargo() << "\n";

    return 0;
}