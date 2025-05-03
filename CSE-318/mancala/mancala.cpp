#include "Player.cpp"

int main(){
    int x=0;
    while(true){
        cout<<"Select 1 Option\n";
        cout<<"1.Human vs Human\n";
        cout<<"2.Human vs AI\n";
        cout<<"3.AI vs AI\n";
        cout<<"4.show stats between AI\n";
        cin>>x;
        if(x>=1 && x<=4){
            break;
        }
        cout<<"Invalid Choice\n";
    }

    cout<<"pits are numbered as\n";
    cout<<"Player 1\n";
    for(int i=PLAYERRANGE[0].second-1;i>=PLAYERRANGE[0].first;i--){
        cout<<i+1<<" ";
    }
    cout<<"\nPlayer 0\n";
    for(int i=PLAYERRANGE[0].first;i<PLAYERRANGE[0].second;i++){
        cout<<i+1<<" ";
    }cout<<"\n";

    BoardState currentState=BoardState();
    currentState.playerToMove=0;
    currentState.printState();

    vector<Player> player;
    player.push_back(Player(0,8,4));
    player.push_back(Player(1,8,1));
    

    if(x==1){
        while(!currentState.isLastState()){

            int pitno;
            cout<<"\nplayer "<<currentState.playerToMove+1<<" enter pit number\n";
            cin>>pitno;
            pitno--;
            pitno+=(PITCNT/2)*currentState.playerToMove;
            while(currentState.stonecnt[pitno]<=0){
                cout<<"\nPit Empty.\nplayer "<<currentState.playerToMove+1<<" enter pit number\n";
                cin>>pitno;
                pitno--;
                pitno+=(PITCNT/2)*currentState.playerToMove;
            }
            currentState=player[currentState.playerToMove].moveManual(currentState,pitno);
            currentState.printState();
        }
        int res0=currentState.stonecnt[PLAYERSTORAGE[0]];
        int res1=currentState.stonecnt[PLAYERSTORAGE[1]];
        if(res0>res1){
            cout<<"\nplayer 1 won\n";
        }else if(res0<res1){
            cout<<"\nPlayer 2 won\n";
        }else{
            cout<<"Draw\n";
        }
    }else if(x==2){
        int who=0;
        while(true){
            cout<<"Choose -> Who will move first\n";
            cout<<"1.You\n";
            cout<<"2.AI\n";
            cin>>who;
            if(who==1){
                currentState.playerToMove=0;
                break;
            }else if(who==2){
                currentState.playerToMove=1;
                break;
            }
            cout<<"Invalid Choice\n";
        }
        
        while(!currentState.isLastState()){
            if(currentState.playerToMove==1){
                cout<<"AI Made Move\n";
                currentState=player[currentState.playerToMove].moveAutomatic(currentState);  
            }else{
                int pitno;
                cout<<"\nHuman player enter pit number\n";
                cin>>pitno;
                pitno--;
                pitno+=(PITCNT/2)*currentState.playerToMove;
                while(currentState.stonecnt[pitno]<=0){
                    cout<<"\nPit Empty.\nplayer "<<currentState.playerToMove<<" enter pit number\n";
                    cin>>pitno;
                    pitno--;
                    pitno+=(PITCNT/2)*currentState.playerToMove;
                }
                currentState=player[currentState.playerToMove].moveManual(currentState,pitno);
            }
            currentState.printState();
        }
        int res0=currentState.stonecnt[PLAYERSTORAGE[0]];
        int res1=currentState.stonecnt[PLAYERSTORAGE[1]];
        if(res0>res1){
            cout<<"\nHuman player won\n";
        }else if(res0<res1){
            cout<<"\nAI won\n";
        }else{
            cout<<"Draw\n";
        }

    }else if(x==3){
        int who=0;
        while(true){
            cout<<"Choose -> Who will move first\n";
            cout<<"1.AI-1\n";
            cout<<"2.AI-2\n";
            cin>>who;
            if(who==1){
                currentState.playerToMove=0;
                break;
            }else if(who==2){
                currentState.playerToMove=1;
                break;
            }
            cout<<"Invalid Choice\n";
        }
        while(!currentState.isLastState()){
            cout<<"\n";
            if(currentState.playerToMove==1){
                cout<<"AI-2 Made Move\n";  
            }else{
                cout<<"AI-1 Made Move\n";
            }
            currentState=player[currentState.playerToMove].moveAutomatic(currentState);
            currentState.printState();
        }
        int res0=currentState.stonecnt[PLAYERSTORAGE[0]];
        int res1=currentState.stonecnt[PLAYERSTORAGE[1]];
        if(res0>res1){
            cout<<"\nAI-1 won\n";
        }else if(res0<res1){
            cout<<"\nAI-2 won\n";
        }else{
            cout<<"Draw\n";
        }
    }else{
        int num;
        cout<<"Enter Number of games\n";
        cin>>num;
        while(num--){
            BoardState currentState2=BoardState();
            currentState2.playerToMove=0;
        
            while(!currentState2.isLastState()){
                currentState2=player[currentState2.playerToMove].moveAutomatic(currentState2);
            }
            int res0=currentState2.stonecnt[PLAYERSTORAGE[0]];
            int res1=currentState2.stonecnt[PLAYERSTORAGE[1]];
            if(res0>res1){
                cout<<"\nAI-1 won"<<endl;
            }else if(res0<res1){
                cout<<"\nAI-2 won"<<endl;
            }else{
                cout<<"Draw"<<endl;
            }
        }
    }
}
