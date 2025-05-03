#include "Construction.cpp"

class Improvement{

public:
    Construction tour;
    Construction optTour;
    Improvement(Construction cities){
        tour=cities;
        optTour=tour;
    }
    ~Improvement(){}

    void setTour(Construction cr){
        tour=cr;
        optTour=tour;
    }

    double getDistance(int fir,int sec){ // indexes of city
        double horizontal=optTour.allCities[fir].x-optTour.allCities[sec].x;
        double vertical=optTour.allCities[fir].y-optTour.allCities[sec].y;
        return sqrt(1.0*horizontal*horizontal+1.0*vertical*vertical);
    }

    void two_OPT(){
        optTour=tour;
        // double prevCost=tour.pathCost(); 
        bool changeAble=true;

        // cout<<tour.pathCost()<<"\n";

        while(changeAble){
            changeAble=false;
            // i,i+1 er connection ar j,j+1 er connection interchange hobe; 
            // ier sathe j connect korbo
            for(int i=0;i+1<optTour.cityCount;i++){
                for(int j=i+2;j<optTour.cityCount;j++){

                    if((j+1)%optTour.cityCount==i){
                        continue;
                    }
                    
                    // reverse(optTour.path.begin()+i+1,optTour.path.begin()+j+1);
                    double nowCost=optTour.getDistance(optTour.path[i],optTour.path[j])
                                 +optTour.getDistance(optTour.path[i+1],optTour.path[(j+1)%optTour.cityCount]);
                    double prevCost=optTour.getDistance(optTour.path[i],optTour.path[i+1])
                                 +optTour.getDistance(optTour.path[j],optTour.path[(j+1)%optTour.cityCount]);
            
                    if(nowCost<prevCost-eps){
                        changeAble=true;
                        reverse(optTour.path.begin()+i+1,optTour.path.begin()+j+1);
                        break;
                    }
                }
                if(changeAble){
                    break;
                }
            }
        }
    }

    vector<int> newPathAfter3Swap(int i,int j,int k,int combo){
        // first 2 bit bujhay i ki j,j+1,k naki k+1
        // 3 bit bujhay i jekhane giese tar onno pranto ta 3rd er m naki m+1
        vector<int> tmp=optTour.path;
        if(combo&1){
            reverse(tmp.begin()+j+1,tmp.begin()+k+1);
        }
        if(combo&(1<<1)){
            reverse(tmp.begin()+i+1,tmp.begin()+j+1);   
        }
        if(combo&(1<<2)){
            for(int ind=1;j+ind<=k;ind++){
                tmp[i+ind]=optTour.path[j+ind];
            }
            for(int ind=1;i+ind<=j;ind++){
                tmp[i+k-j+ind]=optTour.path[i+ind];
            }
        }
        return tmp;
    }
    void three_OPT(){
        optTour=tour;
        double prevCost=tour.pathCost(); 
        bool changeAble=true;
        while(changeAble){
            changeAble=false;
            for(int i=0;i<optTour.cityCount;i++){
                for(int j=i+2;j<optTour.cityCount;j++){
                    for(int k=j+2;k<optTour.cityCount;k++){
                        for(int which=1;which<8;which++){
                            vector<int> tmpIndices=optTour.path;
                            optTour.path=newPathAfter3Swap(i,j,k,which);
                            double newCost=optTour.pathCost();
                            if(newCost<prevCost-eps){
                                prevCost=newCost;
                                changeAble=true;
                                break;
                            }
                            optTour.path=tmpIndices;
                        }
                        if(changeAble) break;
                    }
                    if(changeAble) break;
                }
                if(changeAble) break;
            }
        }
    }

    void shiiftNode(int prevpos,int curposprev){
        int tmp=optTour.path[prevpos];
        if(prevpos<curposprev){
            for(int i=prevpos;i<curposprev;i++){
                optTour.path[i]=optTour.path[i+1];          
            }
            optTour.path[curposprev]=tmp;
        }
        else{
            for(int i=prevpos;i>curposprev;i--){
                optTour.path[i]=optTour.path[i-1];          
            }
            optTour.path[curposprev+1]=tmp;
        }
       
    }

    void nodeShift(){
        optTour=tour;
        // double prevCost=tour.pathCost(); 
        bool changeAble=true;
        while(changeAble){
            changeAble=false;
            for(int i=0;i<optTour.cityCount;i++){
                int prev=optTour.path[(i-1+optTour.cityCount)%optTour.cityCount];
                int next=optTour.path[(i+1)%optTour.cityCount];
                double gainifjoined=optTour.getDistance(prev,next);
                double prevgain   = optTour.getDistance(prev,optTour.path[i])
                                    +optTour.getDistance(next,optTour.path[i]);
                for(int j=0;j<optTour.cityCount;j++){
                    if(optTour.path[j]==prev || j==i){
                        continue;
                    }
                    
                    double newcost = gainifjoined
                                    +optTour.getDistance(optTour.path[j],optTour.path[i])
                                    +optTour.getDistance(optTour.path[(j+1)%optTour.cityCount],optTour.path[i]);
                    double prevcost = prevgain + optTour.getDistance(optTour.path[j],optTour.path[(j+1)%optTour.cityCount]);
                    
                    if(newcost<prevcost-eps){
                        // cout<<i<<" "<<j<<" "<<prev<<" "<<next<<" "<<optTour.cityCount<<" "<<gainifjoined<<" "<<newcost<<"\n";
                        shiiftNode(i,j);
                        // optTour.print();
                        changeAble=true;
                        break;
                    }
                }
                if(changeAble){
                    break;
                }
            }
        }
    }
    double calcgain(int x0,int y0){
        int prevx0=(x0-1+optTour.cityCount)%optTour.cityCount;
        int nextx0=(x0+1)%optTour.cityCount;

        int prevy0=(y0-1+optTour.cityCount)%optTour.cityCount;
        int nexty0=(y0+1)%optTour.cityCount;

        // cout<<prevx0<<" "<<x0<<" "<<nextx0<<"\n";
        // cout<<prevy0<<" "<<y0<<" "<<nexty0<<"\n";
        double subs=0;
        double adds=0;
        if(y0==nextx0 || prevy0==nextx0){
            subs=optTour.getDistance(optTour.path[prevx0],optTour.path[x0])
                +optTour.getDistance(optTour.path[nexty0],optTour.path[y0]);
            adds=optTour.getDistance(optTour.path[prevx0],optTour.path[y0])
                +optTour.getDistance(optTour.path[nexty0],optTour.path[x0]);
        }else if(x0==nexty0 || prevx0==nexty0){
            subs=optTour.getDistance(optTour.path[prevy0],optTour.path[y0])
                +optTour.getDistance(optTour.path[nextx0],optTour.path[x0]);
            adds=optTour.getDistance(optTour.path[prevy0],optTour.path[x0])
                +optTour.getDistance(optTour.path[nextx0],optTour.path[y0]);
        }else{
            subs=optTour.getDistance(optTour.path[prevx0],optTour.path[x0])
                +optTour.getDistance(optTour.path[nexty0],optTour.path[y0])
                +optTour.getDistance(optTour.path[prevy0],optTour.path[y0])
                +optTour.getDistance(optTour.path[nextx0],optTour.path[x0]);

            adds=optTour.getDistance(optTour.path[prevx0],optTour.path[y0])
                +optTour.getDistance(optTour.path[nexty0],optTour.path[x0])
                +optTour.getDistance(optTour.path[prevy0],optTour.path[x0])
                +optTour.getDistance(optTour.path[nextx0],optTour.path[y0]);
        }
        return adds-subs;


    }
    void nodeSwap(){
        optTour=tour;
        // double prevCost=tour.pathCost(); 
        bool changeAble=true;
        // cout<<tour.pathCost()<<"dddd\n";
        while(changeAble){
            changeAble=false;
            for(int i=0;i<optTour.cityCount;i++){
                for(int j=i+1;j<optTour.cityCount;j++){
                    
                    double gain=calcgain(i,j);
                    if(gain<-eps){
                        // cout<<i<<" "<<j<<" "<<gain<<"\n";
                        swap(optTour.path[i],optTour.path[j]);
                        changeAble=true;
                        break;
                    }
                }
                if(changeAble){
                    break;
                }
            }
        }
    }
    void print(int huge,int indc,int indp){
        cout << improvement_names[huge][indp]<<" of "<<construction_names[indc]<<" :\n";
        cout << "No of Visited Cities    : " << optTour.path.size() << endl;
        cout << "Total Tour Cost        : " << optTour.pathCost() << endl;
        cout << "Visited Cities Order   : ";
        for (int i : optTour.path)cout << i << " ";
        cout << endl << endl;

    }
    void improve(int huge,int type){
        
        if(type==0){
            two_OPT();
        }else if(type==1){
            if(huge){
                nodeSwap();
            }else{
                three_OPT();
            }
        }else{
            nodeShift();
        }
    }
};
