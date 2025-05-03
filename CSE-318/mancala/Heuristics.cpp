#include "Boardstate.cpp"
class Heuristics{
    
    public:
    int getHeuristics(BoardState boardState,int forwhom,int heuristicInd){
        int store=PLAYERSTORAGE[forwhom];
        int oppositestore=PLAYERSTORAGE[1-forwhom];
        if(heuristicInd==1){
            return boardState.stonecnt[store]-boardState.stonecnt[oppositestore];
        }else if(heuristicInd==2){
            int fir=boardState.getStoneCntOnASide(forwhom);
            int sec=boardState.getStoneCntOnASide(1-forwhom);
            return W1*(boardState.stonecnt[store]-boardState.stonecnt[oppositestore])+W2*(fir-sec);
        }else if(heuristicInd==3){
            int fir=boardState.getStoneCntOnASide(forwhom);
            int sec=boardState.getStoneCntOnASide(1-forwhom);
            int thi=boardState.additionalMove[forwhom];
            
            return W1*(boardState.stonecnt[store]-boardState.stonecnt[oppositestore])
                    +W2*(fir-sec)+W3*thi;
        }else if(heuristicInd==4){
            int fir=boardState.getStoneCntOnASide(forwhom);
            int sec=boardState.getStoneCntOnASide(1-forwhom);
            int thi=boardState.additionalMove[forwhom];
            int fourth=boardState.capturedStone[forwhom];
            // cout<<fir<<" "<<sec<<" "<<thi<<" "<<fourth<<"\n";
            return W1*(boardState.stonecnt[store]-boardState.stonecnt[oppositestore])
                    +W2*(fir-sec)+W3*thi+W4*fourth;
        }else{
            return MAX;       
        }
    }
};