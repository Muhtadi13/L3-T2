  
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

enum Heuristics{
	GINI_IMPURITY,
	INFORMATION_GAIN
};

map<string,vector<string>> allattributesValue;
vector<string> allattributes;
vector<string> allclasses;


vector<vector<string>> readCSV(string filename){
	ifstream input{filename};
	vector<vector<string>> csvRows;

	if (!input.is_open()) {
		cerr << "Couldn't read file: " << filename << "\n";
		return csvRows; 
	}
	for (string line; getline(input, line);) {
		istringstream ss(move(line));
		vector<string> row;
		if (!csvRows.empty()) {
			row.reserve(csvRows.front().size());
		}
		for (string value; getline(ss, value, ',');) {
			row.push_back(move(value));
		}
		csvRows.push_back(move(row));
	}
	return csvRows;
}
void printCSV(vector<vector<string>> &csvRows){
  	for (const vector<string>& row : csvRows) {
    	for (const string& value : row) {
      		cout << setw(10) << value;
    	}
    	cout << "\n";
  	}
}


pair<vector<vector<string>>,vector<vector<string>>> 
splitTrainAndTestDataset(vector<vector<string>> dataSet , double trainSetSize=0.8){
	vector<vector<string>> trainSet;
	vector<vector<string>> testSet;
	
	srand(time(0)); 
	for(int i = 0; i < dataSet.size(); i++){
		int x = rand() % 10 + 1;
		if(trainSet.size()>trainSetSize*dataSet.size()){
			testSet.push_back(dataSet[i]);
		}else if(testSet.size()>(1-trainSetSize)*dataSet.size()){
			trainSet.push_back(dataSet[i]);
		}else{
			if(x <= trainSetSize * 10){
				trainSet.push_back(dataSet[i]);
			} 
			else{
				testSet.push_back(dataSet[i]);
			}
		} 
	}
	return {trainSet,testSet};
}

void parseCarData(const string& fileName,
                vector<string>& classNames,
				vector<string>& attributeNames,
				map<string,vector<string>>& attributes) {
    ifstream stream(fileName);
    string line;
    bool parsingClasses = false;
    bool parsingAttributes = false;
    
    // Clear output vectors
    classNames.clear();
    attributes.clear();
	attributeNames.clear();
    
    while (getline(stream, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

		// cout<<line<<"\n";
        
        if (line.empty()) continue;
        
        if (line == "--class values") {
            parsingClasses = true;
            parsingAttributes = false;
            continue;
        }
        else if (line == "--attributes") {
            parsingClasses = false;
            parsingAttributes = true;
            continue;
        }
        
        if (parsingClasses) {
            istringstream classStream(line);
            string value;
            while (getline(classStream, value, ',')) {
                // Trim whitespace
                value.erase(0, value.find_first_not_of(" \t\r\n"));
                value.erase(value.find_last_not_of(" \t\r\n") + 1);
                if (!value.empty()) {
                    classNames.push_back(value);
                }
            }
        }
        else if (parsingAttributes) {
            size_t colonPos = line.find(':');
            if (colonPos != string::npos) {
                vector<string> attrValues;
                string valuesStr = line.substr(colonPos + 1);
				string namestr = line.substr(0,colonPos);
                istringstream valStream(valuesStr);
                string value;
                
                while (getline(valStream, value, ',')) {
                    // Trim whitespace and remove period
                    value.erase(0, value.find_first_not_of(" \t\r\n"));
                    value.erase(value.find_last_not_of(" \t\r\n.") + 1);
                    if (!value.empty()) {
                        attrValues.push_back(value);
                    }
                }
                
                if (!attrValues.empty()) {
                    attributes[namestr]=attrValues;
                }
				attributeNames.push_back(namestr);
            }
        }
    }
}