#include<bits/stdc++.h>
#define ll long long
#define MAX 1e16
#define pint pair<int,int>
#define CONSTRUCTION_COUNT 3
#define PERTURBATIVE_COUNT 3
#define eps 1e-6
using namespace std;
using namespace chrono;

struct Point{
    double x=0,y=0;
};

vector<string> construction_names={"Nearest Neighbour","Nearest Insertion","Cheapest Insertion"};
vector<vector<string>> improvement_names={{"2-OPT","3-OPT","Node Shift"},{"2-OPT","Node Swap","Node Shift"}};
// vector<string> improvement_names={"2-OPT","3-OPT","Node Shift"};
std::vector<std::string> fileNames = {
    "berlin52.tsp",
    "bier127.tsp",
    "ch130.tsp",
    "eil51.tsp",
    "eil76.tsp",
    "eil101.tsp",
    "kroA100.tsp",
    "kroB100.tsp",
    "kroC100.tsp",
    "kroD100.tsp",
    "kroE100.tsp",
    "lin105.tsp",
    "pr76.tsp",
    "pr124.tsp",
    "pr144.tsp",
    "rat99.tsp",
    "st70.tsp",
    "ch150.tsp",

    "rat195.tsp",
    "lin318.tsp",
    "a280.tsp",
};
class Construction
{
    public:
    int startCity;
    vector<int> path;
    int cityCount;
    vector<bool> visited;
    vector<Point> allCities; 

    Construction(vector<Point> cities){
        allCities=cities;
        cityCount=cities.size();
        visited.resize(cityCount);
        path.clear();
        startCity=0;
    }
    Construction(const Construction &cr){
        allCities=cr.allCities;
        cityCount=cr.cityCount;
        visited=cr.visited;
        startCity=cr.startCity;
        path=cr.path;
    }
    Construction(){};
    ~Construction(){}
    double getDistance(int fir,int sec){ // indexes of city
        double horizontal=allCities[fir].x-allCities[sec].x;
        double vertical=allCities[fir].y-allCities[sec].y;
        return sqrt(1.0*horizontal*horizontal+1.0*vertical*vertical);
    }
    void unvisitAll(){
        for(int i=0;i<cityCount;i++){
            visited[i]=false;
        }
    }
    int nextNearestUnvisited(int cur){
        double mx=MAX;
        int ret=-1;
        for(int i=0;i<cityCount;i++){
            if(i==cur || visited[i]){
                continue;
            }
            double dist=getDistance(cur,i);
            if(mx>dist){
                mx=dist;
                ret=i;
            }
        }
        // cout<<cur<<" "<<ret<<" "<<allCities[cur].x<<" "<<allCities[cur].y<<" "<<mx<<"\n";
        return ret;
    }
    void nearestNeighbour(){
        path.clear();
        unvisitAll();
        int cur=startCity;
        for(int i=0;i<cityCount;i++){
            path.push_back(cur);
            visited[cur]=true;
            cur=nextNearestUnvisited(cur);
        }
    }

    int nearestFromAllVisited(){
        double mx=MAX;
        int ret=-1;
        for(int i=0;i<cityCount;i++){
            if(!visited[i]){
                continue;
            }
            int near=nextNearestUnvisited(i);
            if(getDistance(i,near)<mx){
                mx=getDistance(i,near);
                ret=near;
            }
        }
        return ret;
    }

    void insertIntoPath(int prevLocation,int cityToInsert){
        path.push_back(cityToInsert);
        visited[cityToInsert]=true;
        for(int i=path.size()-2;i>=0;i--){
            if(i==prevLocation){
                break;
            }
            swap(path[i],path[i+1]);
        }
    }

    int minCostEdgeInsertion(int cityToInsert){
        double mx=MAX;
        int prev=-1;
        // cout<<cityToInsert<<" \n";
        for(int i=0;i<path.size();i++){
            double newDistance=getDistance(path[i],cityToInsert)
                                +getDistance(cityToInsert,path[(i+1)%path.size()])
                                -getDistance(path[i],path[(i+1)%path.size()]);
            // cout<<path[i]<<"------->"<<newDistance<<"\n";
            if(newDistance<mx){
                mx=newDistance;
                prev=i;
            }
        }
        return prev;
    }


    void nearestInsertion(){ //clear visited before calling
        path.clear();
        unvisitAll();

        int cur=startCity;
        path.push_back(cur);
        visited[cur]=true;
        cur=nextNearestUnvisited(cur);
        path.push_back(cur);
        visited[cur]=true;

        for(int i=2;i<cityCount;i++){
            cur=nearestFromAllVisited();
            int location=minCostEdgeInsertion(cur);
            // cout<<location<<" "<<cur<<"\n";
            insertIntoPath(location,cur);
        }
    }

    pint insertCheapCostIncreaseNode(int prevCity){
        double mx=MAX;
        int node=-1;
        int prevloc=-1;

        for(int i=0;i<cityCount;i++){
            if(visited[i]){
                continue;
            }
            for(int j=0;j<path.size();j++){
                double newDistance=getDistance(path[j],i)
                                    +getDistance(i,path[(j+1)%path.size()])
                                    -getDistance(path[j],path[(j+1)%path.size()]);
                if(newDistance<mx){
                    mx=newDistance;
                    prevloc=j;
                    node=i;
                }
            }
        }
        return {prevloc,node};
    }

    void cheapestInsertion(){
        path.clear();
        int cur=startCity;
        path.push_back(cur);
        visited[cur]=true;
        cur=nextNearestUnvisited(cur);
        path.push_back(cur);
        visited[cur]=true;
        for(int i=2;i<cityCount;i++){
            pint loc_city=insertCheapCostIncreaseNode(cur);
            cur=loc_city.first;
            insertIntoPath(cur,loc_city.second);
        }
    }

    double pathCost(){
        double ret=0;
        for(int i=0;i<cityCount;i++){
            ret+=getDistance(path[i],path[(i+1)%cityCount]);
        }
        return ret;
    }

    void print(int ind){
        cout << construction_names[ind] <<" :\n";
        cout << "No of Visited Cities    : " << path.size() << endl;
        cout << "Total Tour Cost        : " << pathCost() << endl;
        cout << "Visited Cities Order   : ";
        for (int i : path)cout << i << " ";
        cout << endl << endl;

    }

    void construct(int type){
        if(type==0){
            nearestNeighbour();
        }else if(type==1){
            nearestInsertion();
        }else{
            cheapestInsertion();
        }
    }
};










