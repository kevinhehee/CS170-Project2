#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <set>

using namespace std;




// double leaveOneOutCrossValidation(vector<vector<double>> &data, set<double> &currFeatureSet, int k) {

    

//     return 0;
// }

double leaveOneOutCrossValidation(const vector<vector<double>> &data, set<double> featureSet, int k) {
    int numberClassifiedCorrectly = 0;

    featureSet.insert(k);
    cout << "TESTING FEATURES: ";
    for (auto &entry: featureSet) {
        cout << entry << " ";
    }
    cout << "\n";
    // cout << "\n\n\n";

    for (int i = 0; i < data.size(); i++) {

        double currClass = data.at(i).at(0);

        vector<double> currFeatures;
        for (int feature : featureSet) {
            currFeatures.push_back(data[i][feature]);
        }

        // cout << "Looping over " << i << endl;
        // cout << "Object " << i << " is class " << currClass << endl;

        double nearestNeighborDistance = INT_MAX;
        double nearestNeighborLocation = INT_MAX;
        double nearestNeighborClass = INT_MAX;

        for (int j = 0; j < data.size(); j++) {

            if (i == j) {
                continue;
            }

            // cout << "Ask if " << i << " is nearest neighbor with " << j << endl;
            
            vector<double> neighborFeatures;
            for (int feature : featureSet) {
                neighborFeatures.push_back(data[j][feature]);
            }
        
            double sum = 0;
            for (int l = 0; l < currFeatures.size(); l++ ) {
                sum += pow(currFeatures.at(l) - neighborFeatures.at(l), 2);
            }
            double distance = sqrt(sum);

            if (nearestNeighborDistance >= distance) {
                nearestNeighborDistance = distance;
                nearestNeighborLocation = j;
                nearestNeighborClass = data.at(j).at(0);
            }
        }
        // cout << "Object " << i << " is class " << objectClass << endl;
        // cout << "Its nearest neighbor is " << nearestNeighborLocation << " which is class " << nearestNeighborClass << endl;
        // cout << endl;
        if (currClass == nearestNeighborClass) {
            numberClassifiedCorrectly++;
        }
    }


    cout << numberClassifiedCorrectly << " / " << data.size() << endl;

    double accuracy = ((double) numberClassifiedCorrectly) / data.size();
    return accuracy;
}


void forwardSelectionFeatureSearch(const vector<vector<double>> &data) {

    set<double> currFeatureSet;

    
    set<double> bestFeatureSet;
    double maxTotalAccuracy = 0;

    for (int i = 1; i < data.at(0).size(); i++) {
        cout << "On level " << i << " of the search" << endl;
        cout << "Current feature set: " << endl;
        for (auto& entry: currFeatureSet) {
            cout << entry << " ";
        }
        cout << endl;

        int featureToAdd = -1;
        double maxLevelAccuracy = 0;
        
        for (int k = 1; k < data.at(0).size(); k++) {
            if (currFeatureSet.count(k)) {
                continue;
            }
            cout << "--Considering adding feature " << k << endl;

            double accuracy = leaveOneOutCrossValidation(data, currFeatureSet, k);
            cout << "Accuracy is " << accuracy << endl << endl;
            if (accuracy > maxLevelAccuracy) {
                maxLevelAccuracy = accuracy;
                featureToAdd = k;
            }

        }
        cout << "On level " << i + 1 << ", added feature " << featureToAdd << " to the current set\n\n";
        
        currFeatureSet.insert(featureToAdd);
        if (maxLevelAccuracy > maxTotalAccuracy) {
            maxTotalAccuracy = maxLevelAccuracy;

            bestFeatureSet = currFeatureSet;
            cout << "GOAT FEATURE SET!: ";
            for (auto& entry: bestFeatureSet) {
                cout << entry << " ";
            }
            cout << endl << endl;
        }
        
    }

    cout << "Best feature set: " << endl;
    for (auto& entry: bestFeatureSet) {
        cout << entry << " ";
    }
    cout << endl;


}

int main(int argc, char* argv[]) {
    cout << "Opening file: " << argv[1] << endl;
    string inputFile = argv[1];
    ifstream inFS(inputFile);

    if (!inFS.is_open()) {
        cout << "File not opened!" << endl;
        return 1;
    }

    string line;


    int lineNum = 1;

    vector<vector<double>> data;

    while (getline(inFS, line)) {

        istringstream iss(line);

        vector<double> values;
        
        double val;
        while (iss >> val) {
            values.push_back(val);
        }

        data.push_back(values);

    }

    forwardSelectionFeatureSearch(data);





    return 0;
}