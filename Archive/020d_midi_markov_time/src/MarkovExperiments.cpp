#include "MarkovManager.h"
#include <iostream>

int main()
{
    MarkovManager markovModel{};

    std::string out{};
    std::string ins[] = {"A", "B", "A", "C", "A", "B"};
    for (auto i=0;i<6;++i){
        markovModel.putEvent(ins[i]);
        out = markovModel.getEvent();
        if (out == ins[i]){
            std::cout << "Same " << ins[i] << ":" << out << std::endl;
        }
    }
    
    
    
    
    // for (auto i=0;i<100;++i){
    //     mm.putEvent(std::to_string(i));
    //     std::string e = mm.getEvent();
    //     int order = mm.getOrderOfLastEvent();
    //     std::cout << "Next state " << e << " order " << order << std::endl; 
 
    // }

    // std::cout << mm.getModelAsString() << std::endl;
    // for (auto i=0;i<5;++i){
    //     state_single next = mm.getEvent();
    //     int order = mm.getOrderOfLastEvent();
    //     std::cout << "Next state " << next << " order " << order << std::endl; 
    // }
}