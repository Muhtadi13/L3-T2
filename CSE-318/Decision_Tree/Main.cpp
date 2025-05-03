#include "DecisionTree.cpp"


int main(){
    int iters=20;
    vector<vector<double>> stats(20,vector<double>(4));
    string outputfile="accuracy_results.txt";
    freopen(outputfile.c_str(),"w",stdout);

    for(int i=0;i<20;i++){
        double percentage=0.8;
        auto csvRows=readCSV("car evaluation dataset/car.data");
        // printCSV(csvRows);
        random_device rd;
        std::mt19937 randomGenerator(rd());
        shuffle(csvRows.begin(),csvRows.end(),randomGenerator);

        auto trainTest=splitTrainAndTestDataset(csvRows,percentage);
        auto trainSet=trainTest.first;
        auto testSet=trainTest.second;

        parseCarData("car evaluation dataset/car.c45-names",allclasses,allattributes,allattributesValue);

        DecisionTree dTree1(trainSet,INFORMATION_GAIN,1);
        stats[i][0]=dTree1.accuracy(testSet);
        DecisionTree dTree2(trainSet,GINI_IMPURITY,1);
        stats[i][1]=dTree2.accuracy(testSet);
        DecisionTree dTree3(trainSet,INFORMATION_GAIN,3);
        stats[i][2]=dTree3.accuracy(testSet);
        DecisionTree dTree4(trainSet,GINI_IMPURITY,3);
        stats[i][3]=dTree4.accuracy(testSet);


        for(int j=0;j<4;j++){
            cout<<stats[i][j]<<" ";
        }cout<<"\n";

        // string outputfile="./Figures/tree.txt";
        // freopen(outputfile.c_str(),"w",stdout);
        // dTree1.printDecisionTree(dTree1.root,0);
        
        // double stdDev = 0;
        // for(int i=0; i<N_ITERATIONS; i++) {
        //     stdDev += Math.pow(accuracyArray[i] - mean, 2);
        // }
        // stdDev /= N_ITERATIONS;
        // stdDev = Math.sqrt(stdDev);
    } 
}