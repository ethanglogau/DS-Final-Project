#include "Tree.h"

#include <iostream>

#include <unordered_map>
#include <fstream>
#include <string>
#include <set>
#include <algorithm>
#include <map>
#include <fstream>
#include <string>


using namespace std;

//test
//could edit data set to only include the columns we are using to make it faster
void MakeTree(ifstream& file, Tree& valenceTree, Tree& danceTree, Tree& energyTree, Tree& instrumentalTree, unordered_map<string, vector<double>>& IDs) {
    if (file.is_open()) {
        //cout << "accessed" << endl;
        string endLine;
        getline(file, endLine); // get rid of header line
        double valence; // 1st column
        double energy; // 7th column
        double danceability; // 5th column
        double instrumentalness; // 10th column
        string temp;
        string ID; // 9th column
        string name; // 15th column
        while (!file.eof()) {
            getline(file, temp, ',');
            try {
                valence = stod(temp);
            }
            catch (const invalid_argument& e) {
                break;
            }
            //cout << "accessed2" << endl;
            //valence = temp;
            //getline(file, temp, ',');
            //getline(file, temp, ',');
            //getline(file, temp, ']');
            //getline(file, temp, ',');

            getline(file, temp, ',');
            danceability = stod(temp);

            //danceability = temp;
            //getline(file, temp, ',');
            getline(file, temp, ',');
            energy = stod(temp);

            //energy = temp;
            //getline(file, temp, ',');
            getline(file, ID, ',');
            getline(file, temp, ',');
            instrumentalness = stod(temp);

            //instrumentalness = temp;
            //getline(file, temp, ',');
            //getline(file, temp, ',');
            //getline(file, temp, ',');
            //getline(file, temp, ',');
            getline(file, name); // if the name has a comma in it there is a bug
            //getline(file, endLine);

            //cout << "valance " << valence << endl;
            //cout << "danceability " << danceability << endl;
            //cout << "energy " << energy << endl;
            //cout << "ID " << ID << endl;
            //cout << "instrumentalness " << instrumentalness << endl;
            //cout << "name " << name << endl;

            valenceTree.insert(ID, valence);
            danceTree.insert(ID, danceability);
            energyTree.insert(ID, energy);
            instrumentalTree.insert(ID, instrumentalness);
            IDs[ID].push_back(valence);
            IDs[ID].push_back(danceability);
            IDs[ID].push_back(energy);
            IDs[ID].push_back(instrumentalness);
        }

    }
}

//recursive function to fill set
void songSuggestionSetRec(Tree::Node* curr, set<string>& suggested, double upper, double lower) {
    if (curr == nullptr) {

    }
    else {
        //if value is inside of range and the node isn't null insert the ID into the set then call function again
        if (curr->left != nullptr && curr->left->value <= upper && curr->left->value >= lower) {
            suggested.insert(curr->left->ID);
            songSuggestionSetRec(curr->left, suggested, upper, lower);
        }
        if (curr->right != nullptr && curr->right->value <= upper && curr->right->value >= lower) {
            suggested.insert(curr->right->ID);
            songSuggestionSetRec(curr->right, suggested, upper, lower);
        }
    }
}
//creates set for the given value paramter
//sets will contain the ID's of songs
//this function is somewhat inconcsistent might need to be changed in some ways like finding where to start
set<string> songSuggestionSet(Tree tree, double value, double range) {
    //traverse the tree
    //new set is created to contain the similar values
    set<string> suggested;
    //the range for the values to be added to the set is from lowerLimit to upperLimit
    double lowerLimit = value - range;
    if (lowerLimit < 0)
        lowerLimit = 0;
    double upperLimit = value + range;
    if (upperLimit > 1)
        upperLimit = 1;
    Tree::Node* temp = tree.root;
    //finds the highest element in the tree that is within the range
    if (value < tree.root->value) {
        while (temp->value > upperLimit)
            temp = temp->left;
    }
    else {
        while (temp->value < lowerLimit)
            temp = temp->right;
    }
    //inserts the first ID into the set then calls the recursive function to add the rest of the values
    suggested.insert(temp->ID);
    songSuggestionSetRec(temp, suggested, upperLimit, lowerLimit);
    //returns the new set of similar values
    return suggested;
}

set<string> intersection(set<string> s1, set<string>s2) {
    set<string> intersect;
    set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), inserter(intersect, intersect.begin()));
    return intersect;
}

//function calculates and returns a set which is the intersection of the 4 sets
//checks if the sets are greater than 1 so the given song is not the only one in the set
set<string> smallestIntersection(set<string> valenceSet, set<string> danceSet, set<string> energySet, set<string> instrumentalSet) {

    set<string> intersect1;
    set<string> intersect2;
    set<string> intersect3;

    if (valenceSet.size() > 1) {
        if (danceSet.size() > 1) {
            intersect1 = intersection(valenceSet, danceSet);
            if (energySet.size() > 1) {
                intersect2 = intersection(intersect1, energySet);
                if (instrumentalSet.size() > 1) {
                    intersect3 = intersection(intersect2, instrumentalSet);
                }
            }
            else {
                if (instrumentalSet.size() > 1) {
                    intersect2 = intersection(intersect1, instrumentalSet);
                }
            }
        }
        else {
            if (energySet.size() > 1) {
                intersect1 = intersection(valenceSet, energySet);
                if (instrumentalSet.size() > 1) {
                    intersect3 = intersection(intersect2, instrumentalSet);
                }
            }
        }
    }
    else {
        if (danceSet.size() > 1) {
            if (energySet.size() > 1) {
                intersect1 = intersection(danceSet, energySet);
                if (instrumentalSet.size() > 1) {
                    intersect2 = intersection(intersect1, instrumentalSet);
                }
            }
            else {
                if (instrumentalSet.size() > 1) {
                    intersect1 = intersection(danceSet, instrumentalSet);
                }
            }
        }
        else {
            if (energySet.size() > 1) {
                if (instrumentalSet.size() > 1) {
                    intersect1 = intersection(energySet, instrumentalSet);
                }
            }
        }
    }

    if (intersect3.size() > 0) {
        return intersect3;
    }
    else if (intersect2.size() > 0) {
        return intersect2;
    }
    else if (intersect1.size() > 0) {
        return intersect1;
    }
    else {
        return valenceSet;
    }

}

//Tree::Node* treeTraversal(Tree::Node* root) {

//}

int main() {

    ifstream data;
    data.open("SpotifyData3.csv");

    Tree valenceTree = Tree();
    Tree danceTree = Tree();
    Tree energyTree = Tree();
    Tree instrumentalTree = Tree();

    //given the name or ID we need to be able to find the 4 values easily
    //make a map to solve this problem
    unordered_map<string, vector<double>> IDs; 
    //Vector pos 0 = valence, 1 = dance, 2 = energy, 3 = instrumental

    //"Graphs": <double bucket/category, vector <pair <string ID, double value>>>
    ///FIXME is the second double necessary? If we can find the values based solely off ID then just store that
    unordered_map<double, vector<pair<string, double>>> valGraph;
    unordered_map<double, vector<pair<string, double>>> danceGraph;
    unordered_map<double, vector<pair<string, double>>> energyGraph;
    unordered_map<double, vector<pair<string, double>>> instrumentalGraph;
    MakeTree(data, valenceTree, danceTree, energyTree, instrumentalTree, IDs);
    data.close();


    //valenceTree.PrintInOrder();
    //prompt user to input names of songs they like: if possible make it a search engine where they can add and remove
    cout << "Insert ID" << endl;
    string ID;
    cin >> ID;

    //cout << IDs["4BJqT0PrAfrxzMOxytFOIz"][0];
    double range = 0.05;
    //these are the sets storing the similar values to the ID entered
    set<string> valenceSet = songSuggestionSet(valenceTree, IDs[ID][0], range);
    set<string> danceSet = songSuggestionSet(danceTree, IDs[ID][1], range);
    set<string> energySet = songSuggestionSet(energyTree, IDs[ID][2], range);
    set<string> instrumentalSet = songSuggestionSet(instrumentalTree, IDs[ID][3], range);
    cout << valenceSet.size() << " " << danceSet.size() << " " << energySet.size() << " " << instrumentalSet.size() << endl;
    
    set<string> suggestable = smallestIntersection(valenceSet, danceSet, energySet, instrumentalSet);

    //cout << intersect1.size() << " " << intersect2.size() << " " << intersect3.size() << endl;
    cout << suggestable.size() << endl;

    return 0;
}