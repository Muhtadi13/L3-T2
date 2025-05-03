#include "Heuristics.cpp"

class Player{ //notun notun boardstate e notun notun player create hobe na
    public:
    int playerno;
    int maxDepthOfSearch;
    int heuristic;
    BoardState boardState; // boardstate set kore diyo

    Player(int player,int mxsearchdepth,int heurist){
        playerno=player;
        maxDepthOfSearch=mxsearchdepth;
        heuristic=heurist;
    }

    // automatic e minmax kore move korbe
    // manual e manually move korbe
    int minmax(int depth,bool ismax,BoardState curState,int alpha,int beta){

        //call thefunction with 0,0,init,-INF,+INF
        // if(curState.isLastState()){
        //     cout<<"last state e asi vayo\n";
        //     curState.printState();
        //     cout<<"last state e asi vayo\n";
        // }
        if(curState.isLastState() || depth==maxDepthOfSearch){
            Heuristics heu;
            int val = heu.getHeuristics(curState,playerno,heuristic);
            // on=false;
            // cout<<depth<<" print tmp\n";
            // curState.printState();
            // cout<<"print tmp\n";
            // cout<<val<<" "<<playerno<<" "<<heuristic<<" "<<depth<<" "<<maxDepthOfSearch<<"\n";
            return val;
        }

        //last state noy
        // tahole dekho j plyer er move se maximize korbe baki jon minimize korbe
        vector<BoardState> nxtReachableStates=curState.getAllReachableStates(); //ontoto ekta thakbei
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle(nxtReachableStates.begin(), nxtReachableStates.end(), default_random_engine(seed));
        // if(nxtReachableStates.empty()){
        //     cout<<"empty paisi\n"<<curState.playerToMove<<"\n";
        //     curState.printState();
        //     exit(0);
        // }
        if(ismax){
            int alphatmp=-INF;
            for(int i=0;i<nxtReachableStates.size();i++){
                int tmp;
                if(nxtReachableStates[i].playerToMove==playerno){
                    tmp=minmax(depth+1,true,nxtReachableStates[i],alpha,beta);
                }else{
                    tmp=minmax(depth+1,false,nxtReachableStates[i],alpha,beta);
                }
                // if(alphatmp<tmp && on){
                //     nxtReachableStates[i].printState();
                //     cout<<"alpha\n";
                // }
                // on=false;
                alphatmp=max(alphatmp,tmp);
                alpha=max(alpha,alphatmp);
                if(alpha>=beta){
                    break;
                }
            }
            return alphatmp;
        }else{
            int betatmp=INF;
            for(int i=0;i<nxtReachableStates.size();i++){
                int tmp;
                if(nxtReachableStates[i].playerToMove==playerno){
                    tmp=minmax(depth+1,true,nxtReachableStates[i],alpha,beta);
                }else{
                    tmp=minmax(depth+1,false,nxtReachableStates[i],alpha,beta);
                }
                // if(betatmp>tmp && on){
                //     nxtReachableStates[i].printState();
                //     cout<<"betta\n";
                // }
                betatmp=min(betatmp,tmp);
                beta=min(beta,betatmp);
                if(alpha>=beta){
                    break;
                }
            }
            return betatmp;
        }
    }

    BoardState moveAutomatic(BoardState bState){
        //bState er sobkisu baire thik koro
        BoardState nxtOptimal = BoardState();
        if(bState.isLastState()){ // ei check ta basically baire kora uchit
            
            nxtOptimal.playerToMove=-1;
            return nxtOptimal;
        }
        int val=-INF;
        
        vector<BoardState> allChilds=bState.getAllReachableStates();
        for(int i=0;i<allChilds.size();i++){
            int valtmp=minmax(0,true,allChilds[i],-INF,INF);
            if(valtmp>val){
                nxtOptimal=allChilds[i];
                val=valtmp;
            }
            
        }
        return nxtOptimal;
    }
    BoardState moveManual(BoardState bState,int pit){
        //bState er sobkisu baire thik koro
        // pit valid kina baire check koro
        
        BoardState nxtOptimal = BoardState();
        if(bState.isLastState()){ // ei check ta basically baire kora uchit
            
            nxtOptimal.playerToMove=-1;
            return nxtOptimal;
        }
        int val=-INF;
        nxtOptimal=bState.getNextState(pit); 
        return nxtOptimal;
    }
};

