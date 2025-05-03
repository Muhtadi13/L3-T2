#include <bits/stdc++.h>
#define lli long long
#define plli pair<lli, lli>
#define pint pair<int,int>
#define vvi vector<vector<int>>
const int MAX = 10;
const int MAXSQ = 100;

const lli MOD = 1000000007LL;
const int INF = 1000000009;

using namespace std;

vvi grid(MAX,vector<int>(MAX));
vvi targetgrid(MAX,vector<int>(MAX));
vector<int> gridarray(MAXSQ);
int n,m;
// vector<int> mapping;
// vector<int> revmapping(MAXSQ);


bool valid(int i,int j){
    return (i>=0 && i<n && j>=0 && j<m);
}

void delay_ms_rough(int val){
    int c=val;
    val*=100005;

    while(val>0){
       val--;
    }
    cout<<"\n{------>>{ elapsed "<<c<<" ms  }<<--------- \n";
}
pair<int,int> dxy[]={{0,1},{1,0},{0,-1},{-1,0}};
pair<int,int> blankpos(2,2);
class State{

    public:
    State* parent;
    int costfromtarget;
    int distancefromroot;
    vvi owngrid;
    pair<int,int> blank;
    
    State(State* par,int cost,int dist,vvi nwgrid, pair<int,int> bpos){
        parent=par;
        costfromtarget=cost;
        distancefromroot=dist;
        owngrid=nwgrid;
        blank=bpos;
    }    

};

class Compare {
public:
    bool operator()(const State* a,const State* b){
        if((a->distancefromroot+a->costfromtarget)==(b->distancefromroot+b->costfromtarget))
            return a->costfromtarget<b->costfromtarget;
        return (a->distancefromroot+a->costfromtarget)>(b->distancefromroot+b->costfromtarget);
    }
};

int merge(int l,int mid,int r){
    vector<int> tmp(r-l+1);
    
    int ind=0;
    int ind2=mid+1;
    int cnt=0;
    int tot=0;
    for(int i=l;i<=mid;i++){
        while(ind2<=r && gridarray[i]>gridarray[ind2]){
            cnt++;
            tmp[ind++]=gridarray[ind2++];
        }
        tmp[ind++]=gridarray[i];
        tot+=cnt;
    }
    while(ind2<=r){
        tmp[ind++]=gridarray[ind2++];
    }
    for(int i=0;i<=r-l;i++){
        gridarray[l+i]=tmp[i];
    }
    return tot;
}

int invcount(int l,int r){
    // cout<<l<<" "<<r<<" hghgh \n";
    if(l>=r){
        return 0;
    }
    int mid=(l+r)>>1;
    int fir=invcount(l,mid);
    int sec=invcount(mid+1,r);
    return merge(l,mid,r)+fir+sec;
}
int initialize(){
    // mapping={0,1,2,3,4,5,6,7,8,INF};
    // for(int i=0;i<n;i++){
    //     if(mapping[i]==INF) continue;
    //     revmapping[mapping[i]-1]=i;
    // }
    

}
int calc_hamming(vvi curgrid){
    int currenthamming=0;
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(curgrid[i][j]==INF) continue;

            int now=curgrid[i][j];
            //hamming_dist[now]=((now==targetgrid[i][j])?0:1);
            currenthamming+=((now==targetgrid[i][j])?0:1);//hamming_dist[now];
        }
    }
    return currenthamming;
}
int calc_manhattan(vvi curgrid){

    int currentmanhattan=0;
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(curgrid[i][j]==INF) continue;
            
            int now=curgrid[i][j];
            int row=(now-1)/m;
            int col=now-1-(row)*m;
            // manhattan_dist[now]=abs(row-i-1)+abs(col-j-1);
            currentmanhattan+=abs(row-i)+abs(col-j);//manhattan_dist[now];
        }
    }
    // cout<<currentmanhattan<<" mamama\n";
    return currentmanhattan;
}

void print(State* leaf){
    if(leaf==NULL) return;
    print(leaf->parent);
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            cout<<leaf->owngrid[i][j]<<" ";
        }cout<<"\n";
    }
}
set<vector<vector<int>>> vis;
tuple<int,int,int> dijkstra(int (*func)(vvi)){
    vis.clear();

    priority_queue<State*,vector<State*>,Compare> pq;
    // State *root=new State(NULL,calc_hamming(grid),0,grid,blankpos);
    State *root=new State(NULL,func(grid),0,grid,blankpos);
    vis.insert(grid);
    pq.push(root);
    State* finalst;
    int expanded=0;

    cout<<"Answer Path\n";

    while(!pq.empty()){
        auto a=pq.top();pq.pop();
        // cout<<a->distancefromroot<<" "<<a->costfromtarget<<" firr\n\n";

        if(a->costfromtarget==0){
            finalst=a;
            break;
        }
        expanded++;
        for(auto ch:dxy){
            int nwx=a->blank.first+ch.first;
            int nwy=a->blank.second+ch.second;
            
            if(valid(nwx,nwy)){
                auto nwgrid=a->owngrid;
                swap(nwgrid[a->blank.first][a->blank.second],nwgrid[nwx][nwy]);

                // cout<<nwx<<" "<<nwy<<" "<<calc_manhattan(nwgrid)<<" secc\n";
                if(vis.find(nwgrid)!=vis.end()){
                    continue;
                }
                vis.insert(nwgrid);
                // State* child=new State(a,calc_hamming(nwgrid),a->distancefromroot+1,nwgrid,{nwx,nwy});
                State* child=new State(a,func(nwgrid),a->distancefromroot+1,nwgrid,{nwx,nwy});
                pq.push(child);
                // break;

            }
        }
        
    }
    int explored=expanded+pq.size();
    int len=-1;
    stack<State*> ans;

    while (finalst!=NULL){
        len++;
        ans.push(finalst);
        finalst=finalst->parent;
    }
    
    while (!ans.empty()){
        finalst=ans.top();ans.pop();
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                if(finalst->owngrid[i][j]==INF){
                    cout<<"* ";
                    continue;
                }
                cout<<finalst->owngrid[i][j]<<" ";
            }cout<<"\n";
        }cout<<"\n";
    }
    // cout<<expanded<<" "<<explored<<" "<<len<<"\n";
    return {expanded,explored,len};
}
void npuzzlesolver(){
    int ind=0;
    int whichrow=0;
    
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(grid[i][j]==INF){
                whichrow=ind+1;
            }
            gridarray[ind++]=grid[i][j];
        }
    }
    for(int i=whichrow;i<ind;i++){
        gridarray[i-1]=gridarray[i];
    }
    // for(int i=0;i<ind;i++){
    //     cout<<gridarray[i]<<" ";
    // }cout<<"\n";
    gridarray[ind-1]=INF;
    int invs=invcount(0,ind-1);
    whichrow=m-(whichrow+m-1)/m;
    // for(int i=0;i<ind;i++){
    //     cout<<gridarray[i]<<" ";
    // }cout<<"\n";
    // cout<<invs<<" "<<whichrow<<"\n";

    if((m%2==1 && invs%2==1) || (m%2==0 && ((invs%2)+whichrow)%2==1)){
        cout<<"impossible\n";
        return;
    }
    cout<<"possible\n";
    int (*func)(vvi);
    func=calc_hamming;
    auto res=dijkstra(func);
    cout<<"Hamming distance----------->\n";
    cout<<"expanded: "<<get<0>(res)<<"\nexplored: "<<get<1>(res)<<"\ndistance: "<<get<2>(res)<<"\n";
    
    func=calc_manhattan;
    res=dijkstra(func);
    cout<<"Manhattan distance----------->\n";
    cout<<"expanded: "<<get<0>(res)<<"\nexplored: "<<get<1>(res)<<"\ndistance: "<<get<2>(res)<<"\n";

}
int toint(string s){
    int num=0;
    for(int i=0;i<s.size();i++){
        num*=10;
        num+=s[i]-'0';
    }
    return num;
}

int main(){
    cin>>n;
    m=n;
    grid.resize(n,vector<int> (m));
    targetgrid.resize(n,vector<int> (m));
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            string s;
            cin>>s;
            if(s=="*"){
                grid[i][j]=INF;
                blankpos.first=i;
                blankpos.second=j;
            }
            else grid[i][j]=toint(s);
        }
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            targetgrid[i][j]=m*(i)+j+1;
        }
    }
    // for(int i=0;i<n;i++){
    //     for(int j=0;j<m;j++){
    //         cout<<grid[i][j]<<" ";
    //     }cout<<"\n";
    // }
    // for(int i=0;i<n;i++){
    //     for(int j=0;j<m;j++){
    //         cout<<targetgrid[i][j]<<" ";
    //     }cout<<"\n";
    // }

    cout<<"\n";
    npuzzlesolver();
    
}