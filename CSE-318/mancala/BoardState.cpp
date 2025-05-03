#include "Constants.h"
class BoardState{    

    public:

    vector<int> stonecnt;
    int playerToMove;
    vector<int> additionalMove;
    vector<int> capturedStone;

    BoardState(){

        additionalMove.resize(2,0);
        capturedStone.resize(2,0);
        stonecnt.resize(PITCNT,STONEATEACHPITINITIALLY);
        stonecnt[PLAYERSTORAGE[0]]=stonecnt[PLAYERSTORAGE[1]]=0;
        playerToMove=0;

    };

    ~BoardState(){};

    int getStoneCntOnASide(int player){
        int sum=0;
        for(int i=PLAYERRANGE[player].first;i<PLAYERRANGE[player].second;i++){
            sum+=stonecnt[i];
        } 
        return sum;
    }

    BoardState getNextState(int pit){ //pit e asar age eta faka kina check koro
                                        
        BoardState nextState=*this;
        int cur=pit;
        int otherplayer=1-playerToMove;
        while(nextState.stonecnt[pit]>0){
            cur++;
            cur%=PITCNT;
            if(cur==PLAYERSTORAGE[otherplayer]){
                continue;
            }
            nextState.stonecnt[cur]++;
            nextState.stonecnt[pit]--;
        }
        // baire theke sob set and check hoye ashbe
        // pore somoy paile egula constant diye change koro
        
        if(pit>=PLAYERRANGE[playerToMove].first && pit<PLAYERRANGE[playerToMove].second){ // ei check ta baireo korte paro
            if(cur!=PLAYERSTORAGE[playerToMove]){
                nextState.playerToMove=otherplayer;
                int oppositepit=(PITCNT-2)-cur;

                if(cur>=PLAYERRANGE[playerToMove].first && cur<PLAYERRANGE[playerToMove].second && nextState.stonecnt[cur]==1 && nextState.stonecnt[oppositepit]>0){
                    
                    nextState.stonecnt[PLAYERSTORAGE[playerToMove]]++;
                    nextState.stonecnt[PLAYERSTORAGE[playerToMove]]+=nextState.stonecnt[oppositepit];
                    nextState.capturedStone[playerToMove]+=nextState.stonecnt[oppositepit];

                    nextState.stonecnt[cur]=0;
                    nextState.stonecnt[oppositepit]=0;
                }
            }else{
                nextState.additionalMove[playerToMove]++;
                // cout<<"addition"<<playerToMove<<"dddddddddddd\n";
            }

            bool empty1=true;
            for(int i=PLAYERRANGE[playerToMove].first;i<PLAYERRANGE[playerToMove].second;i++){
                if(nextState.stonecnt[i]>0){
                    empty1=false;
                    break;
                }
            }
            bool empty2=true;
            for(int i=PLAYERRANGE[otherplayer].first;i<PLAYERRANGE[otherplayer].second;i++){
                if(nextState.stonecnt[i]>0){
                    empty2=false;
                    break;
                }
            }
            if(empty1 || empty2){
                for(int i=PLAYERRANGE[otherplayer].first;i<PLAYERRANGE[otherplayer].second;i++){
                    nextState.stonecnt[PLAYERSTORAGE[otherplayer]]+=nextState.stonecnt[i];
                    nextState.stonecnt[i]=0;
                } 
                for(int i=PLAYERRANGE[playerToMove].first;i<PLAYERRANGE[playerToMove].second;i++){
                    nextState.stonecnt[PLAYERSTORAGE[playerToMove]]+=nextState.stonecnt[i];
                    nextState.stonecnt[i]=0;
                }   
            }
        }
        return nextState;
    }

    vector<BoardState> getAllReachableStates(){
        // jodi size 0 hoy seta baire check koro
        vector<BoardState> allChilds;
        for(int i=PLAYERRANGE[playerToMove].first;i<PLAYERRANGE[playerToMove].second;i++){
            if(stonecnt[i]>0){
                allChilds.push_back(getNextState(i));
            }
        }
        return allChilds;
    }

    bool isLastState(){
        for(int i=PLAYERRANGE[playerToMove].first;i<PLAYERRANGE[playerToMove].second;i++){
            if(stonecnt[i]>0){
                return false;
            }
        }
        int otherPlayer=1-playerToMove;
        for(int i=PLAYERRANGE[otherPlayer].first;i<PLAYERRANGE[otherPlayer].second;i++){
            if(stonecnt[i]>0){
                return false;
            }
        }
        return true;
        
    }

    void printState(){
        cout<<"------------------------------------------------------------\n\t";
        for(int i=PLAYERRANGE[1].second-1;i>=PLAYERRANGE[1].first;i--){
            cout<<stonecnt[i]<<"  ";
        }
        cout<<"\n"<<stonecnt[PLAYERSTORAGE[1]]<<"\t\t\t\t"<<stonecnt[PLAYERSTORAGE[0]]<<"\n\t";
        for(int i=PLAYERRANGE[0].first;i<PLAYERRANGE[0].second;i++){
            cout<<stonecnt[i]<<"  ";
        }
        cout<<"\n------------------------------------------------------------\n";
        
    }
};
