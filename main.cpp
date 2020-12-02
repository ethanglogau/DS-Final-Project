#include <iostream>
<<<<<<< HEAD
#include <map>
#include <fstream>
#include <string>

using namespace std;

class Tree {
	struct Node {
		string ID;
		Node* right;
		Node* left;
	};


};
//test
//could edit data set to only include the columns we are using to make it faster
void MakeTree(ifstream& file) {
	if (file.is_open()) {
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
			valence = stod(temp);
			//valence = temp;
			getline(file, temp, ',');
			getline(file, temp, ',');
			getline(file, temp, ']');
			getline(file, temp, ',');
			getline(file, temp, ',');
			danceability = stod(temp);
			//danceability = temp;
			getline(file, temp, ',');
			getline(file, temp, ','); 
			energy = stod(temp);
			//energy = temp;
			getline(file, temp, ',');
			getline(file, ID, ',');
			getline(file, temp, ',');
			instrumentalness = stod(temp);
			//instrumentalness = temp;
			getline(file, temp, ',');
			getline(file, temp, ',');
			getline(file, temp, ',');
			getline(file, temp, ',');
			getline(file, name, ','); // if the name has a comma in it there is a bug
			getline(file, endLine);

			cout << "Valence" << valence << endl;
			cout << "Danceability" << danceability << endl;
			cout << "Energy" << energy << endl;
			cout << "ID" << ID << endl;
			cout << "Instrumentalness" << instrumentalness << endl;
			cout << "Name" << name << endl;
			break;
		}

	}
}

int main() {
	ifstream data;
	data.open("SpotifyData.csv");
	MakeTree(data);
	//set data into 2 data structures
	data.close();
	return 0;
=======

using namespace std;

int main(){
    cout << "test" << endl;
    return 0;
>>>>>>> 0a49341d8c54efb582ea07fb6dc73cf4e01bf9cb
}