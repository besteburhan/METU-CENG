#include "GameParser.h"
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE 
*/
/**
     * Parse the file with given name and create players accordingly.
     *
     * GameParser DOES NOT have any responsibility over these Players.
     *
     * Note: The file will always exists, and there will be no erroneous input.
     *
     * @param filename The name of the file to be parsed.
     * @return  pair.first: Board size.
     *          pair.second: The vector of the constructed players.
     */
std::pair<int, std::vector<Player *> *> GameParser::parseFileWithName(const std::string& filename){
	std::pair<int, std::vector<Player*>*>  pairRes;
	std::string line;
    std::ifstream file (filename.c_str());
    int countPl=-1;
    pairRes.second = new std::vector<Player*>();
    if (file.is_open())
	{
		for(int i=0; std::getline (file,line) && !file.eof() ; i++)
    	{
    		if(i==0){
    			line.erase(0,line.find(":")+2);
    			std::string boardSizeStr = line.substr(0 , line.size());
    			pairRes.first = std::stoi(boardSizeStr);
    		}
    		else if(i ==1){
    			line.erase(0,line.find(":")+2);
    			std::string countPlStr = line.substr(0 , line.size());
    			countPl = std::stoi(countPlStr);
    		}
    		else{
    			std::string idStr = line.substr(0,line.find("::"));
    			line.erase(0, line.find("::") + 2);
    			std::string typePl = line.substr(0,line.find("::"));
    			line.erase(0, line.find("::") + 2);
    			int x = std::stoi(line.substr(0,line.find("::")));
    			line.erase(0, line.find("::") + 2);
    			int y = std::stoi(line.substr(0,line.size())) ;
    			uint id = std::stoi(idStr);
    			if(typePl=="Berserk"){
    			    Player * p = new Berserk(id,x,y);
    				pairRes.second->push_back(p);
    			}
    			else if(typePl=="Tracer"){
    				Player *p = new Tracer(id,x,y);
    				pairRes.second->push_back(p);
    			}
    			else if(typePl=="Pacifist"){
    				Player *p = new Pacifist(id,x,y);
    				pairRes.second->push_back(p);
    			}
    			else if(typePl=="Ambusher"){
    				Player *p = new Ambusher(id,x,y);
    				pairRes.second->push_back(p);
    			}
    			else if(typePl=="Dummy"){
    				Player *p = new Dummy(id,x,y);
    				pairRes.second->push_back(p);
    			}
    		}
      		
    	}
    	file.close();
  	}
  	return pairRes;
  }