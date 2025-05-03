#include "Parser.cpp"

class Node{

    public:

    bool isLeaf;
    map<string,Node> childNode;
    string attributeName;
    string classValue;

    Node(){
        isLeaf=false;
        attributeName="-1";
        classValue="-1";
        childNode=map<string,Node>();
    }

    Node(bool isLeaf){
        this->isLeaf=isLeaf;
        attributeName="-1";
        classValue="-1";
        childNode=map<string,Node>();
    }

    void addChild(Node nd,string attributeValue){
        childNode[attributeValue]=nd;
    }

    Node(const Node& nd){
        this->attributeName=nd.attributeName;
        this->childNode=nd.childNode;
        this->classValue=nd.classValue;
        this->isLeaf=nd.isLeaf;
    }
};

class DecisionTree{

public:
    vector<vector<string>> trainingData;
    Node root;
    mt19937 randomNumberGenerator;
    int bestAttributeSelectionCnt;
    Heuristics heuristics;
    
    DecisionTree(vector<vector<string>> trainingData,Heuristics h=INFORMATION_GAIN,int bestCnt=1){
        this->trainingData=trainingData;
        bestAttributeSelectionCnt=bestCnt;
        heuristics=h;
        vector<vector<string>> dummy;
        random_device rd;
        randomNumberGenerator = mt19937(rd());

        if(trainingData.size())
            this->root=constructTree(trainingData,allattributes,dummy);
    }
    ~DecisionTree(){
    }

    string toLowerString(string input) {
        for(int i=0;i<input.size();i++){
            input[i]=tolower(input[i]);
        }
        return input;
    }

    int getIndexOfAttribute(string attributeName){
        for(int i=0;i<allattributes.size();i++){
            if(toLowerString(attributeName.c_str())==toLowerString(allattributes[i])){
                return i;
            }
        }
        return -1;
    }

    vector<vector<string>> dataPortionByAttribute(vector<vector<string>> &currentdata,string  attributeName,string attributeVal){
        int index=getIndexOfAttribute(attributeName);
        vector<vector<string>> portion;
        for(auto row:currentdata){
            if(row[index]==attributeVal){
                portion.push_back(row);
            }
        }
        return portion;
    }
    map<string,int> getFrequencyOfClass(vector<vector<string>> &data){
        map<string,int> occurences;
        for(auto row:data){
            occurences[row[row.size()-1]]++;
        }
        return occurences;
    }

    double calculateEntropy(vector<vector<string>> &mydata){
        auto occurances=getFrequencyOfClass(mydata);
        double entropy=0;
        for(auto el:occurances){
            double val=1.0*el.second/mydata.size();
            entropy-=val*log2(val);
        }
        return entropy;
    }


    Node pluralityClass(vector<vector<string>> &data){
        auto occurances=getFrequencyOfClass(data);
        int mx=0;
        Node leaf(1);
        for(auto el:occurances){
            if(el.second>mx){
                mx=el.second;
                leaf.classValue=el.first;
            }
        }
        return leaf;
    }

    // oi attribute ta rekhe bakigula niche pass korle ki poriman lav hoy
    double informationGain(vector<vector<string>> &data,string attributeName,double prevEntropy){
        double curWeightedEntropy=0;
        auto attributeValues=allattributesValue[attributeName];

        for(string val:attributeValues){
            auto subset=dataPortionByAttribute(data,attributeName,val);
            double tmp=calculateEntropy(subset);
            curWeightedEntropy+=1.0*tmp*subset.size()/data.size();
        }
        return prevEntropy-curWeightedEntropy;
    }
    double calculateImpurity(vector<vector<string>> &mydata){
        auto occurances=getFrequencyOfClass(mydata);
        double impurity=1.0;
        for(auto el:occurances){
            double val=1.0*el.second/mydata.size();
            impurity-=1.0*val*val;
        }
        return impurity;
    }

    double giniImpurity(vector<vector<string>> &data,string attributeName){
        double curWeightedImpurity=0;
        auto attributeValues=allattributesValue[attributeName];

        for(string val:attributeValues){
            auto subset=dataPortionByAttribute(data,attributeName,val);
            double tmp=calculateImpurity(subset);
            curWeightedImpurity+=1.0*tmp*subset.size()/data.size();
        }
        return curWeightedImpurity;
    }
    string chooseAttributebyGain(vector<vector<string>> &myData,vector<string> &myattributeNames,int bestcnt=1){
        double curEntropy=calculateEntropy(myData);
        double mxGain=0;
        string attrName="-1";
        vector<pair<double,string>> choice; 

        for(auto name:myattributeNames){
            double gain=informationGain(myData,name,curEntropy);
            choice.push_back({gain,name});
            
            if(gain>mxGain){
                mxGain=gain;
                attrName=name;
            }
        }
        sort(choice.rbegin(),choice.rend());
        int divider=min((int)choice.size(),bestcnt);
        std::uniform_int_distribution<> distributor(0, divider-1);
        attrName = choice[distributor(randomNumberGenerator)].second;
        return attrName;
    }

    string chooseAttributebyGiniImpurity(vector<vector<string>> &myData,vector<string> &myattributeNames,int bestcnt=1){
        double curEntropy=calculateEntropy(myData);
        double minImpurity=INF;
        string attrName="-1";
        vector<pair<double,string>> choice;
        for(auto name:myattributeNames){
            double impurity=giniImpurity(myData,name);
            choice.push_back({impurity,name});
            if(impurity<minImpurity){
                minImpurity=impurity;
                attrName=name;
            }
        }
        sort(choice.begin(),choice.end());
        int divider=min((int)choice.size(),bestcnt);
        std::uniform_int_distribution<> distributor(0, divider-1);
        int ind=distributor(randomNumberGenerator);
        // cout<<bestcnt<<" "<<choice.size()<<" "<<divider<<" "<<ind<<"\n";
        attrName = choice[ind].second;
        return attrName;
    }
    Node constructTree(vector<vector<string>> &myData,vector<string> &myattributeNames,vector<vector<string>> &parentData){
        if(myData.empty()){
            return pluralityClass(parentData);
        }else if(getFrequencyOfClass(myData).size()==1){
            Node leaf(1);
            leaf.classValue=myData[0][myData[0].size()-1];
            return leaf;
        }else if(myattributeNames.empty()){
            return pluralityClass(myData);
        }else{

            Node nonleaf(0);
            if(heuristics==INFORMATION_GAIN){
                nonleaf.attributeName=chooseAttributebyGain(myData,myattributeNames,bestAttributeSelectionCnt);
            }else if(heuristics==GINI_IMPURITY){
                nonleaf.attributeName=chooseAttributebyGiniImpurity(myData,myattributeNames,bestAttributeSelectionCnt);
            }
            vector<string> passingAttributeNames;
            for(auto name:myattributeNames){
                if(toLowerString(name)==toLowerString(nonleaf.attributeName)){
                    continue;
                }
                passingAttributeNames.push_back(name);
            }

            for(auto val:allattributesValue[nonleaf.attributeName]){
                auto childData=dataPortionByAttribute(myData,nonleaf.attributeName,val);
                Node child=constructTree(childData,passingAttributeNames,myData);
                nonleaf.addChild(child,val);
            }
            return nonleaf;
        }
    }

    void printString(int depth,string str){
        for(int i=0;i<depth;i++){
            cout<<str;
        }
    }
    //give the root here
    void printDecisionTree(Node node,int depth){
        printString(depth-1,"\t");
        printString(min(depth,1),"\t");
        if(node.isLeaf){
            cout<<"$$"<<node.classValue<<"$$\n";
        }else{
            cout<<">>"<<node.attributeName<<"<<\n";
            for(auto child:node.childNode){
                printString(depth,"\t");
                cout<<"||"<<child.first<<"||\n";
                printDecisionTree(child.second,depth+1);
            }
        }
    }

    string predict(vector<string> singleEntry){
        Node cur=root;

        while(!cur.isLeaf){
            
            int index=getIndexOfAttribute(cur.attributeName);
            string attrVal=singleEntry[index];
            // cout<<cur.attributeName<<" "<<index<<" "<<cur.isLeaf<<" "<<attrVal<<"\n";
            
            // for(auto el:cur.childNode){
            //     cout<<el.first<<" "<<el.second.attributeName<<"\n";
            // }cout<<" ";
            auto tmp=cur.childNode[attrVal];
            cur=tmp;
            // cout<<cur.attributeName<<"\n";
        }
        return cur.classValue;
    }
    double accuracy(vector<vector<string>> &testSet){
        int cnt=0;
        for(int i=0;i<testSet.size();i++){
            string estimate=predict(testSet[i]);
            // cout<<estimate<<"\n";

            if(toLowerString(estimate)==toLowerString(testSet[i][testSet[i].size()-1])){
                cnt++;
            }
        }
        return 1.0*cnt/testSet.size();
    }
};