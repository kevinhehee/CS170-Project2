#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <set>

using namespace std;

// void printSet(const set<int> &s) {
//     cout << "{";
//     int counter = 0;
//     for (int val: s) {
//         cout << val;;
//         if (++counter != s.size()) {
//             cout << ",";
//         }
//     }
//     cout << "}";
// }

ostream& operator<<(ostream& out, const set<int> &s) {
    out << "{";
    int counter = 0;
    for (int val: s) {
        out << val;;
        if (++counter != s.size()) {
            out << ",";
        }
    }
    out << "}";
    return out;

}

vector<double> getFeatures(const vector<vector<double>> &data, const int featureObjInd, const set<int> &featureSet) {
    vector<double> featureValues;
    for (int feature : featureSet) {
        featureValues.push_back(data.at(featureObjInd).at(feature));
    }
    return featureValues;
}

double getEuclideanDistance(const vector<double> &currFeatures, const vector<double> &neighborFeatures) {
    double sum = 0.0;
    for (int l = 0; l < currFeatures.size(); l++ ) {
        double difference = currFeatures.at(l) - neighborFeatures.at(l);
        sum += difference * difference;
    }
    return sqrt(sum);
}

double leaveOneOutCrossValidation(const vector<vector<double>> &data, const set<int> &featureSet) {
    int numberClassifiedCorrectly = 0;

    for (int currInd = 0; currInd < data.size(); currInd++) {
        double currClass = data.at(currInd).at(0);
        vector<double> currFeatures = getFeatures(data, currInd, featureSet);

        double nearestNeighborDistance = INT_MAX;
        double nearestNeighborLocation = -1;
        double nearestNeighborClass = -1;

        for (int neighborInd = 0; neighborInd < data.size(); neighborInd++) {
            if (currInd == neighborInd) {
                continue;
            }

            vector<double> neighborFeatures = getFeatures(data, neighborInd, featureSet);

            double distance = getEuclideanDistance(currFeatures, neighborFeatures);
            if (distance < nearestNeighborDistance - 1e-6) {
                nearestNeighborDistance = distance;
                nearestNeighborLocation = neighborInd;
                nearestNeighborClass = data.at(neighborInd).at(0);
            }
        }

        if (fabs(currClass - nearestNeighborClass) < 1e-6) {
            numberClassifiedCorrectly++;
        }
    }


    double accuracy = ((double) numberClassifiedCorrectly) / data.size() * 100;
    cout << "\tUsing feature(s) " << featureSet << " accuracy is "<< accuracy << "%\n";
    return accuracy;
}


void forwardSelectionFeatureSearch(const vector<vector<double>> &data) {
    set<int> currFeatureSet;
    set<int> bestFeatureSet;
    double maxTotalAccuracy = 0;

    leaveOneOutCrossValidation(data, currFeatureSet);

    for (int searchLevel = 1; searchLevel < data.at(0).size(); searchLevel++) {
        int featureToAdd = -1;
        double maxLevelAccuracy = 0;
        
        for (int feature = 1; feature < data.at(0).size(); feature++) {
            if (currFeatureSet.count(feature)) {
                continue;
            }
            currFeatureSet.insert(feature);
            double accuracy = leaveOneOutCrossValidation(data, currFeatureSet);
            currFeatureSet.erase(feature);

            if (accuracy > maxLevelAccuracy) {
                maxLevelAccuracy = accuracy;
                featureToAdd = feature;
            }
        }

        currFeatureSet.insert(featureToAdd);
        cout << "\nFeature set " << currFeatureSet << " was best, accuracy is " << maxLevelAccuracy << "%\n\n";

        if (maxLevelAccuracy > maxTotalAccuracy) {
            maxTotalAccuracy = maxLevelAccuracy;
            bestFeatureSet = currFeatureSet;
        }
    }

    cout << "Finished search!! The best feature subset is " << bestFeatureSet << ", which has an accuracy of " << maxTotalAccuracy << "%\n";
}

void backwardEliminationFeatureSearch(const vector<vector<double>> &data) {
    set<int> bestFeatureSet;
    set<int> currFeatureSet;
    for (int feature = 1; feature < data.at(0).size(); feature++) {
        currFeatureSet.insert(feature);
    }

    double maxTotalAccuracy = 0;

    leaveOneOutCrossValidation(data, currFeatureSet);

    for (int searchLevel = 1; searchLevel < data.at(0).size(); searchLevel++) {

        double maxLevelAccuracy = 0;
        int featureToRemove = -1;
        
        for (int feature = 1; feature < data.at(0).size(); feature++) {
            if (!currFeatureSet.count(feature)) {
                continue;
            }
            currFeatureSet.erase(feature);
            double accuracy = leaveOneOutCrossValidation(data, currFeatureSet);
            currFeatureSet.insert(feature);

            if (accuracy > maxLevelAccuracy) {
                maxLevelAccuracy = accuracy;
                featureToRemove = feature;
            }

        }
        currFeatureSet.erase(featureToRemove);
        cout << "\nFeature set " << currFeatureSet << " was best, accuracy is " << maxLevelAccuracy << "%\n\n";

        if (maxLevelAccuracy > maxTotalAccuracy) {
            maxTotalAccuracy = maxLevelAccuracy;
            bestFeatureSet = currFeatureSet;
        }
    }

    cout << "Finished search!! The best feature subset is " << bestFeatureSet << ", which has an accuracy of " << maxTotalAccuracy << "%\n";
}

vector<vector<double>> readData(const string &inputFile) {
    ifstream inFS(inputFile);

    if (!inFS.is_open()) {
        cout << "File not opened!\n";
        exit(1);
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
    return data;
}

int main(int argc, char* argv[]) {
    string inputFile = "";
    int choice = -1;

    cout << "Welcome to Kevin He's Feature Selection Program\n";
    if (argc == 3) {
        cout << "Opening file: " << argv[1] << "\n\n";
        inputFile = argv[1];
        choice = stoi(argv[2]);
    } else {
        cout << "Type in the name of the file to test: ";
        cin >> inputFile;
        cout << "\n";

        cout << "Type the number of the algorithm you want to run:\n";
        cout << "\t1) Forward Selection\n";
        cout << "\t2) Backward Elimination\n\n";
        cin >> choice;
    }

    vector<vector<double>> data = readData(inputFile);

    cout << "\n\nThis dataset has " << data.at(0).size() - 1 << " features (not including the class attribute), with " << data.size() << " instances.\n\n";

    if (choice == 1) {
        cout << "Beginning Search.\n\n";
        auto startTime = chrono::high_resolution_clock::now();
        forwardSelectionFeatureSearch(data);
        auto endTime = chrono::high_resolution_clock::now();

        chrono::duration<double> elapsedSeconds = endTime - startTime;
        double minutes = elapsedSeconds.count() / 60.0;

        cout << std::fixed << std::setprecision(2) << "\nForward Selection took " << elapsedSeconds.count() << " seconds or " << minutes << " minutes.\n";
    }
    else if (choice == 2) {
        cout << "Beginning Search.\n\n";
        auto startTime = chrono::high_resolution_clock::now();
        backwardEliminationFeatureSearch(data);
        auto endTime = chrono::high_resolution_clock::now();
        
        chrono::duration<double> elapsedSeconds = endTime - startTime;
        double minutes = elapsedSeconds.count() / 60.0;

        cout << std::fixed << std::setprecision(2) << "\nBackward Elimination took " << elapsedSeconds.count() << " seconds or " << minutes << " minutes\n";
    }

    return 0;
}