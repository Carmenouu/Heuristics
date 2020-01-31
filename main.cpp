#include <iostream>
using namespace std;
#include "state.cpp"

int main(){
    int nbStacks = 3;
    int nbBlocs = 4;
    State state = State(nbBlocs,nbStacks);
    state.setInitial();
    cout << "Initial state s0:" << endl;
    state.display();
    state.doMove(make_pair(2,1));
    cout << "State s1 = t(s0,2->1) :" << endl;
    state.display();
    state.doMove(make_pair(0,2));
    cout << "State s2 = t(s1,0->2) :" << endl;
    state.display();
    state.doMove(make_pair(1,2));
    cout << "State s3 = t(s2,1->2) :" << endl;
    state.display();
    state.doMove(make_pair(1,2));
    cout << "State s4 = t(s3,1->2) :" << endl;
    state.display();
    state.doMove(make_pair(0,2));
    cout << "State s5 = t(s4,0->2) :" << endl;
    state.display();
    if (state.isFinal()){
        cout << "s5 is a final state..." << endl;
    }
    return 0;
}
