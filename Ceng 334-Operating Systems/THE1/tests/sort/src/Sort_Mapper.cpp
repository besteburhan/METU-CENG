#include <iostream>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unistd.h>

using namespace std;


int main(int argc, char* argv[]){
	int mapper_id, number;
	vector<int> numbers;

	/*stringstream ss;
	ss << getpid();
	string s = "Log_Reducer" + ss.str() + ".txt";
	ofstream os(s.c_str());
	*/
	if (argc < 2){
		//os << "Need to add Mapper ID as argument" << endl;
		exit(0);
	}

	mapper_id = atoi(argv[1]);
	
	
	// Read input from stdin
	while(1){
		cin >> number;
		if(cin.eof())
			break;
		numbers.push_back(number);
	}
	
	//Sort the input
	sort(numbers.begin(),numbers.end());
	
	//Print Mapper ID
	cout << "Mapper ID: " << mapper_id << endl;
	
	//Print the sorted input
	
	for(int i = 0;i < numbers.size();i++){
		cout << numbers[i] << " ";
	}
	cout << endl;
	
	return 0;

}

