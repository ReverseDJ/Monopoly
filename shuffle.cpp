
#include <random>
#include <algorithm>
#include "functions.h"

void mixCards(std::deque<DeckCard*> &q) {
    //for(auto i: q){
    //    std::cout<<i->cardDesc<<std::endl;
    //}
    std::vector<DeckCard*> v;
    while (!q.empty()){
        v.push_back(q.front());
        q.pop_front();
    }

    std::random_device rd;
    std::default_random_engine rng(rd());
    shuffle(std::begin(v), std::end(v), rng);

    for(auto i: v){
        q.push_front(i);
    //    std::cout<<i->cardDesc<<std::endl;
    }
}
