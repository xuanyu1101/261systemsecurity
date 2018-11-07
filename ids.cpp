#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>

using namespace std;

int main(){

	string line, eventName, eventType, minVal, maxVal, weight, statName, mean, standardDev;
	int counter = 0;
	ifstream eventFile("Events.txt");
	ifstream statsFile("Stats.txt");

	unordered_map<string, vector<string>> statsMap;
	unordered_map<string, vector<string>> eventsMap;

	//=================================== Storing values from Events.txt ======================================================

	cout << "========== Printing values from Events.txt ================" << endl;

	//obtain individual event value split by delimiter ':'
	while (getline(eventFile, line)){
		stringstream ss(line);
		vector<string> eventVals;	
		
		//push each event's values into a vector
		if(counter != 0){
			getline(ss, eventName, ':');
			getline(ss, eventType, ':');
			getline(ss, minVal, ':');
			getline(ss, maxVal, ':');
			getline(ss, weight, ':');

			if(maxVal == ""){
				maxVal = "0";
			}

			eventVals.push_back(eventType);
			eventVals.push_back(minVal);
			eventVals.push_back(maxVal);
			eventVals.push_back(weight);

			eventsMap.insert(pair<string, vector<string>>(eventName, eventVals));
			
		}		

		counter++;
	}	

	for(unordered_map<string, vector<string>>::iterator pos = eventsMap.begin(); pos != eventsMap.end(); pos++)
	{
		cout << pos->first << " ";
				
		//print data items
		for(auto i = pos->second.begin(); i != pos->second.end(); i++) 
		{
			cout << *i << " ";
		}
				
			cout << endl;

		}

	eventFile.close();
	counter = 0;

	//================================= Storing values from Stats.txt =============================================================

	cout << "========== Printing values from Stats.txt ================" << endl;

	//obtain individual event value split by delimiter ':'
	while (getline(statsFile, line)){
		stringstream ss(line);
		vector<string> statVals;	
		
		//push each event's values into a vector
		if(counter != 0){
			getline(ss, statName, ':');
			getline(ss, mean, ':');
			getline(ss, standardDev, ':');
	

			statVals.push_back(mean);
			statVals.push_back(standardDev);

			statsMap.insert(pair<string, vector<string>>(statName, statVals));
			
		}		

		counter++;
	}

	for(unordered_map<string, vector<string>>::iterator pos = statsMap.begin(); pos != statsMap.end(); pos++)
	{
		cout << pos->first << " ";
				
		//print data items
		for(auto i = pos->second.begin(); i != pos->second.end(); i++) 
		{
			cout << *i << " ";
		}
				
			cout << endl;

	}

	statsFile.close();
	
}
