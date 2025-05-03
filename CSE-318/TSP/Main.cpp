#include "Improvement.cpp"

int main(){

    for(int huge=0;huge<2;huge++){
        int up=fileNames.size();
        string csv_file="tsp"+to_string(huge);
        if(!huge){
            up=18;
        }
        csv_file+=".csv";
        freopen(csv_file.c_str(),"w",stdout);
        cout<<"Filename,StartCity,Construction Heuristics,Construction Time(ms),Improvement Heuristics,Improvement Time(ms),Cost,Cost Reduction,Path\n";
        for(int cnt=0;cnt<up;cnt++){
            string name="TSP_DATA/"+fileNames[cnt];
            freopen(name.c_str(),"r",stdin);
            string line;
            unordered_map<string, string> metadata;

            while(getline(cin, line)){
                stringstream ss(line);
                string key, value;
                if (line.find(":") != string::npos) {
                    getline(ss, key, ':');
                    getline(ss, value);
                    key.erase(key.find_last_not_of(" \t") + 1);
                    value.erase(0, value.find_first_not_of(" \t"));
                    metadata[key] = value;
                }
                else if (line == "NODE_COORD_SECTION"){
                    break;
                }
            }
            int cityCnt=0;
            cityCnt=stoi(metadata["DIMENSION"]);
            vector<Point> cities(cityCnt);
            while(getline(cin, line)){
                stringstream ss(line);
                if (!line.empty() && line!="EOF"){
                    int ct;
                    ss >> ct;
                    ct--;
                    ss >> cities[ct].x >> cities[ct].y;
                }else if (line == "EOF") {
                    break;
                }
            }
        
            srand(time(0));
            int startCity=rand()%cityCnt;
            
            for(int j=0;j<CONSTRUCTION_COUNT;j++){
                Construction con(cities);
                con.startCity=startCity;
                auto startTime = chrono::steady_clock::now();
                con.construct(j);
                auto endTime = chrono::steady_clock::now();
                auto diff = endTime - startTime;
                auto tim=chrono::duration<double, milli>(diff).count();
                cout<<fileNames[cnt]<<","<<startCity<<","<<construction_names[j]<<","<<tim<<","<<"None,0,"<<con.pathCost()<<",0,"<<con.path[0];
                for(int ind=1;ind<con.cityCount;ind++){
                    cout<<">"<<con.path[ind];
                }
                cout<<"\n";
                int concost=con.pathCost();

                for(int i=0;i<PERTURBATIVE_COUNT;i++){

                    Improvement imp(con);
                    // imp.setTour(const_heuris[j]);
                    startTime = chrono::steady_clock::now();
                    imp.improve(huge,i);
                    endTime = chrono::steady_clock::now();
                    diff = endTime - startTime;
                    auto tim2=chrono::duration<double, milli>(diff).count();
                    cout<<fileNames[cnt]<<","<<startCity<<","<<construction_names[j]<<","<<tim<<","<<improvement_names[huge][i]<<","<<tim2<<","<<imp.optTour.pathCost()<<","<<concost-imp.optTour.pathCost()<<","<<imp.optTour.path[0];
                    for(int ind=1;ind<imp.optTour.cityCount;ind++){
                        cout<<">"<<imp.optTour.path[ind];
                    }
                    cout<<"\n";
                } 
            }
        }
    }
    
}
