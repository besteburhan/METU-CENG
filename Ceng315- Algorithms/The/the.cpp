#include <iostream>
#include <vector>
#include <utility>      // std::pair, std::make_pair
#include <fstream>
#include <string>
#include <climits>
#include <stack>
#include <queue>
using namespace std;
int Ammo,rooms;
vector<int> odd_locked;
vector<int> even_locked;


int extract_min(vector<int> cost,vector<bool> visited){

	int min = INT_MAX;
	int min_index = -1;
	for(int i =1;i<rooms+1;i++){
		if(!visited[i] && cost[i] < min){
			min = cost[i];
			min_index= i;
		}
	}
	return min_index;
}

vector<int> shortest_path(vector <vector <pair <int,int>>> &adj,  int src,  vector <stack <int>> &parents,  vector <pair <int,int>> contains_ammo, vector<int> path){
	for(auto p: path){
		for(auto &a: contains_ammo){
			if(a.first == p)a.second = 0;
		}
	}
	parents.clear();
	parents.resize(rooms+1);
	stack<int> a ;
	a.push(-1);
	vector <stack <int>> temp(rooms+1, a);
	parents=temp;

	vector<bool> visited(rooms+1,false);
	vector<int> cost(rooms+1,INT_MAX);
	bool odd_time[rooms+1]={true};
	cost[src]=0;
	int curr = src;
	odd_time[curr]=true;
	for(int count=0; count < rooms ; count++){
		 for(auto &p: contains_ammo){ 
		 	if (p.first == curr && p.second > 0){
		 		p.second *= -1;// collect the ammo
		 		break;
		 	}
		 }
 		visited[curr] = true;
 		for( auto pr : adj[curr]){
 			bool locked=false;
			int neighbour= pr.first;
			int edge = pr.second;
			if(!odd_time[curr]){ 
					for(int i=0 ;i<odd_locked.size();i++){
						int not_move=odd_locked[i];
						if(neighbour==not_move){
							locked=true;
							break;
						}
					}
			}
			if(odd_time[curr]){
					for(int i=0 ;i<even_locked.size();i++){
						int not_move=even_locked[i];
						if(neighbour==not_move){
							locked=true;
							break;
						}
					}
			}
			if(locked) continue;
			int new_cost = cost[curr] + edge;
			for(const auto &p : contains_ammo){
					if(p.first == neighbour && p.second>0 ){
						new_cost -= p.second;
					}
			}

			if(cost[neighbour] > new_cost){//relaxation
					if(parents[curr].top() == neighbour) {
						visited[neighbour]=false; 
						count--;
					}
					cost[neighbour] = new_cost;
					if(parents[neighbour].top()!=-1 ){
						parents[neighbour].pop();
					}
					
					parents[neighbour].push(curr);
					odd_time[neighbour]=!odd_time[curr];
			}

 		}
		curr = extract_min(cost,visited);
		if(curr==-1) break;
		

 	}
 	return cost;
}


int main(int argc, char **argv){
	int   chamber, key, scientist;
	int count_odd, count_even;
	int corridors, num_ammo;

	ifstream file;
	file.open("the3.inp");
	file >> Ammo >> rooms >> chamber >> key >> scientist ;
	file >> count_odd;
	odd_locked.resize(count_odd);
	for(int i=0; i< count_odd ; i++) file >> odd_locked[i];
	file >> count_even;
	even_locked.resize(count_even);
	for(int i=0; i<count_even ; i++) file >> even_locked[i];
	// ammo,roomcount,chamber,key,scientist,odd/even period room count
	vector <vector <pair <int,int>>> adj(rooms+1);	// indexes stands for room number
	file >> corridors;
	for(int i=0; i < corridors ; i++){
		int r1,r2,edg;
		file >> r1 >> r2 >> edg;
		adj[r1].push_back(make_pair(r2,edg));
		adj[r2].push_back(make_pair(r1,edg));
	}
	//adjecency_rooms(edges)
	file >> num_ammo; // can be maximum 2
	vector <pair <int,int>> contains_ammo;
	contains_ammo.resize(num_ammo);
	for(int i=0;i<num_ammo; i++) {
		int r1,a;
		file >> r1 >> a ;
		contains_ammo.push_back(make_pair(r1,a));
	}
	file.close();

	stack<int> a ;
	a.push(-1);
	vector <stack <int>> parents(rooms+1, a);
	int cost_path=0;
	vector<int> cost;
	stack<int> path1;
	stack<int> path2;
	stack<int> path3;
	vector<int> path;
	cost=shortest_path(adj, 1,  parents,  contains_ammo, path);
	if(parents[key].size()>=2){
		cost_path+=cost[key];
		path1.push(key);
		int p=parents[key].top();
		while(parents[p].size()>=2){
			path1.push(p);
			int tmp = parents[p].top();
			parents[p].pop();
			p = tmp;
		}
		path1.push(p);
	}
		while(!path1.empty()){
		path.push_back(path1.top());
		path1.pop();
	}

	cost=shortest_path(adj, key,  parents,  contains_ammo, path);
	if(parents[scientist].size()>=2){
		cost_path+=cost[scientist];
		path2.push(scientist);
		int p=parents[scientist].top();
		while(parents[p].size()>=2){
			if(p==key) break;
			path2.push(p);
			int tmp = parents[p].top();
			parents[p].pop();
			p = tmp;
		}
	}while(!path2.empty()){
		path.push_back(path2.top());
		path2.pop();
	}

	cost=shortest_path(adj, scientist,  parents,  contains_ammo, path);
	if(parents[chamber].size()>=2){
		cost_path+=cost[chamber];
		path3.push(chamber);
		int p=parents[chamber].top();
		while(parents[p].size()>=2){
			if(p==scientist) break;
			path3.push(p);
			int tmp = parents[p].top();
			parents[p].pop();
			p = tmp;
		}while(!path3.empty()){
		path.push_back(path3.top());
		path3.pop();
	}


	}
	std::ofstream the3_out("the3.out");
	if(the3_out.is_open())
    {
        the3_out<<Ammo-cost_path<<std::endl;
        the3_out<<path.size()<<std::endl;
        for(auto p: path){
		the3_out << p << " ";
	}
        the3_out.flush();
        the3_out.close();
    }
}