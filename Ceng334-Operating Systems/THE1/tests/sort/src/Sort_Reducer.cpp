#include <iostream>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

vector<int> merge(vector<int> vec1, vector<int> vec2); 

int main(int argc, char* argv[]){
	int reducer_id,mapper_id,number,status;
	vector<int> from_mapper,from_reducer,sorted;
	char tmp_char,tmp_num[20];
	/*stringstream ss;
	ss << getpid();
	string s = "Log_Reducer" + ss.str() + ".txt";
	ofstream os(s.c_str());
	*/
	
	if (argc < 2){
	//	os << "Need to add Reducer ID as argument" << endl;
		exit(0);
	}
	
	reducer_id = atoi(argv[1]);
	
	
	//Check if reducer is connected to correct mapper
	scanf("Mapper ID: %d",&mapper_id);
	if (mapper_id!=reducer_id){
		//os << "Mapper ID and Reducer ID does not match" << endl;
		exit(0);
	}
	
	//Read from mapper
	while(1){
		cin >> number;
		if(cin.eof())
			break;
		from_mapper.push_back(number);
	}
	
	//Read from reducer
	status = fcntl(2, F_GETFL);
	if(!(status & O_RDWR)) {
		//os << "Continue reading from stderr" << endl;
		tmp_num[0] = '\0';		
		while(reducer_id != 0 && read(2,&tmp_char,1)){
			if(tmp_char == ' ' || tmp_char == '\n'){
				if(strlen(tmp_num) > 0){
					from_reducer.push_back(atoi(tmp_num));
					tmp_num[0] = '\0';
				}
				
			}else{
				int l = strlen(tmp_num);
				tmp_num[l] = tmp_char;
				tmp_num[l+1] = '\0';			
			}
		}
	}
	else {
		//os << "stderr is not opened read_only. Stop reading " << reducer_id << endl;
	}
	
	//Merge the vectors
	sorted = merge(from_mapper,from_reducer);
	
	// Write the elements to stdout
	for(int i = 0;i < sorted.size();i++)
		cout << sorted[i] << " ";
	cout << endl;
	return 0;
}

vector<int> merge(vector<int> vec1, vector<int> vec2){
	int i1 = 0, i2 = 0;
	vector<int> res;
	while(i1 < vec1.size() && i2 < vec2.size()){
		if (vec1[i1] < vec2[i2])
			res.push_back(vec1[i1++]);
		else
			res.push_back(vec2[i2++]);
	}
	while(i1 < vec1.size())
		res.push_back(vec1[i1++]);
	while(i2 < vec2.size())
		res.push_back(vec2[i2++]);
	return res;
}


