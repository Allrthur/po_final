#include <iostream>
#include <list>
#include <string>
using namespace std;

const int MAX_GENERATIONS = 100;
const int NUM_OBJS = 6;
const int POP_SIZE = 100;

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
            int sum = 0;
            for (int i = 0; i < NUM_OBJS; i++)sum += val[i]*OBJ_ATR[i].valor;
            return sum;
        }
        int cargo(){
            int sum = 0;
            for (int i = 0; i < NUM_OBJS; i++)sum += val[i]*OBJ_ATR[i].carga;
            return sum;
        }
        bool operator < (Solution &obj){
            return this->fitness() < obj.fitness();
        }
};

void newGen(Solution pop[POP_SIZE], int settings){
    //switch(settings){
    //    case 0:
    //    case 1:
    //    default:
    //
    //}
}

Solution mutate(Solution s, int p){
    return s;
}

void GA(){
    // read numbers
    // no need :)

    // create random solutions
    srand(time(NULL));
    Solution pop[POP_SIZE]; 
    for(int i = 0; i < POP_SIZE; i++)pop[i] = Solution(); 

    // check for convergence
    // this line will be commented for a while
    
    // create next generation
    newGen(pop, 0); 


}

int main() {
    srand(time(NULL));

    Solution pop[POP_SIZE]; for(int i = 0; i < POP_SIZE; i++)pop[i] = Solution();
    std::cout << "fit: " << pop[0].fitness() << " car: " << pop[0].cargo() << "\n";

    Solution a, b = Solution();

    std::cout << "comparing a: " << a.fitness() << " to b: " << b.fitness() << " result: " << (a < b) << "\n";
    std::cout << "True: " << true << " False: " << false;
    return 0;
}