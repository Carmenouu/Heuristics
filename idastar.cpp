/*
Copyright 2018 Pierre-Edouard Portier
peportier.me
​
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
​
    http://www.apache.org/licenses/LICENSE-2.0
​
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
)
*/

#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <utility>
#include <list>
#include <functional>
#include <cmath>
#include <algorithm>
#include <utility>
#include <iomanip>
#include <limits>
#include <chrono>

#include "state.cpp"

using namespace std;


typedef function<int( const State& pos )> Heuristic;
typedef State::Move Move ;

void
addNeighbor( State &currentState, Move &move, 
             vector< pair< Move,int > > &neighbors,
             list<State>& path, int h )
{
  currentState.doMove( move );
  if( find( path.begin(), path.end(), currentState ) == path.end() )
  {
    neighbors.push_back( make_pair( move, currentState.heuristic(h) ));
  }
  currentState.undoMove( move );
}



void
search( State& currentState,
        int          ub, // upper bound over which exploration must stop
        int&         nub,
        list<State>& path,
        list<State>& bestPath,
        int          h,  // Heuristic
        int&         nbVisitedState )

{
  nbVisitedState++;

  int f; // under-estimation of optimal length
  int g = path.size() - 1; // size of the current path to currentState

  if( currentState.isFinal() )
  {
    bestPath = path;
    return;
  }

  // generate the neighbors
  int nbStacks = currentState.getNbStacks();
  int stack, stack2 ;
  vector< pair< Move,int > > neighbors;
  neighbors.clear();
  
  for (stack=0 ; stack<nbStacks ; stack++) //Boucle parcourant les piles
  {
	if (!currentState.emptyStack(stack))
	{
	  for (stack2=0 ; stack2<nbStacks ; stack2++) // Seconde boucle parcourant les autres piles (afin de voir si le move est possible)
	  {
        if (stack2 != stack)
		{
	    Move move = make_pair(stack, stack2);
	    addNeighbor( currentState, move, neighbors, path, h );
		}
	  }
	}
  }


  // sort the neighbors by heuristic value

  sort( neighbors.begin(), neighbors.end(),
      [](const pair<Move,int> &left, const pair<Move,int> &right) 
      {
        return left.second < right.second;
      } );


  for( const pair<Move,int> &p : neighbors )
  {
    f = g + 1 + p.second;
    if( f > ub )
    {
      if( f < nub )
      {
        nub = f; // update the next upper bound
      }
    }
    else
    {
      currentState.doMove( p.first );
      path.push_back( currentState );
      search( currentState, ub, nub, path, bestPath, h, nbVisitedState );
      path.pop_back();
      currentState.undoMove( p.first );
      if( !bestPath.empty() ) return;
    }
  }
}



void
ida( State&        initialState, 
     int    	   h,		 // heuristic 
     list<State>&  bestPath, // path from source to destination
     int&          nbVisitedState )
{
  int ub;                      // current upper bound
  int nub = initialState.heuristic(h); // next upper bound
  list<State> path;
  path.push_back( initialState ); // the path to the target starts with the source

  while( bestPath.empty() && nub != numeric_limits<int>::max() )
  {
    ub = nub;
    nub = numeric_limits<int>::max();

    cout << "upper bound: " << ub;
    search( initialState, ub, nub, path, bestPath, h, nbVisitedState );
    cout << " ; nbVisitedState: " << nbVisitedState << endl;
  }
}

int
main()
{
  int nbStacks = 3 ;
  int nbBlocks = 4 ;
  State state = State(nbBlocks, nbStacks);
  list<State> bestPath;
  int nbVisitedState = 0;
  state.setInitial();

  auto start = std::chrono::high_resolution_clock::now();
  ida( state, 0, bestPath, nbVisitedState );
  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = finish - start;
  cout << "Elapsed time: " << elapsed.count() << " s\n";
  cout << "nb moves: " << bestPath.size()-1 << endl;
  cout << "nb visited states: " << nbVisitedState << endl;
  
  return 0;
}
