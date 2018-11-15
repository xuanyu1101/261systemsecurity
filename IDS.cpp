#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <math.h>
#include <time.h>
#include <iomanip>
#include <unordered_map>

using namespace std;

// map for stats 
// pair<string, vector<string>>(statName, statVals - (mean) and (standardDev))
unordered_map<string, vector<string>> statsMap;

//vector<string> statVals; // vector for mean and standardDev

// map for events
// pair<string, vector<string>>(eventName, eventVals - (eventType), (minVal), (maxVal and (weight))
unordered_map<string, vector<string>> eventsMap;

//vector<string> eventVals; // vector for (eventType), (minVal), (maxVal and (weight)

// 5 days of data
vector <float> One;
vector <float> Two;
vector <float> Three;
vector <float> Four;
vector <float> Five;
vector <float> temp; // store temp data

void initialInput() {
	string line, eventName, eventType, minVal, maxVal, weight, statName, mean, standardDev;
	int counter = 0;
	ifstream eventFile("Events.txt");
	ifstream statsFile("Stats.txt");

	//=================================== Storing values from Events.txt ======================================================

	cout << "========== Printing values from Events.txt ================" << endl;

	//obtain individual event value split by delimiter ':'
	while (getline(eventFile, line)){
		stringstream ss(line);	
		vector<string> eventVals; // vector for mean and standardDev
		
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
		vector<string> statVals; // vector for mean and standardDev
		
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

// compute random number
float RandomFloat(float a, float b) {
    	float random = ((float) rand()) / (float) RAND_MAX;
    	float diff = b - a;
    	float r = random * diff;
    	return a + r;
}

// round to desire decimal places
string prd(float x, const int decDigits) {
    	stringstream ss;
    	ss << fixed;
    	ss.precision(decDigits); // number of decimal place
    	ss << x;
    	return ss.str();
}

// Creates 5 days of data
void valuesForDays(string input, int decneeded) {	
	// find from stats map according to string
	unordered_map<string, vector<string>>::const_iterator got = statsMap.find (input);

	string mean = got->second[0];
	string::size_type st_mean;     
  	float m = stof (mean,&st_mean); // convert from string to float

	string stdDev = got->second[1];
	string::size_type st_sd;     
  	float sd = stof (stdDev,&st_sd); // convert from string to float

	float min = floor (m - sd); // round down
	float max = ceil (m + sd); // round up
	float totalmean = m * 5;
	
	float total = 0;
	
	temp.clear();
	float randData; 
    	srand(time(0));
	total = 0; // reset total to 0
	

    	for(int count = 0; count < 5; count ++){

		randData = RandomFloat(max, min);
		
		// one decimal place
		if (decneeded == 1) {
			string randD = prd(randData, 1);
			string::size_type st_r;  
			randData = stof (randD,&st_r); // convert from string to float
		}
		// no decimal place 
		else {
			string randD = prd(randData, 0);
			string::size_type st_r;  
			randData = stof (randD,&st_r); // convert from string to float 
		}

		temp.push_back(randData); // store in temp data
		
    	}	

	// push to individual days
	One.push_back(temp[0]);
	Two.push_back(temp[1]);
	Three.push_back(temp[2]);
	Four.push_back(temp[3]);
	Five.push_back(temp[4]);
}

void generateDaysData() {
	cout << "+++++++++++++++++++++++" << endl;
	// creating 5 days of data
	cout << "Generating Data..." << endl;

	// float needed? 1 - Yes, 2 - No
	int decneeded = 0;
	string event;

	event = "Logins";
	valuesForDays(event,2);
	cout << "Logins data are done." << endl;

	event = "Time online";
	valuesForDays(event,1);
	cout << "Time online data are done." << endl;

	event = "Emails sent";
	valuesForDays(event,2);
	cout << "Emails sent data are done." << endl;

	event = "Emails opened";
	valuesForDays(event,3);
	cout << "Emails opened data are done." << endl;

	event = "Emails deleted";
	valuesForDays(event,4);
	cout << "Emails deleted data are done." << endl;
}

// print data from each day
void printDaysData(){
	cout << "+++++++++++++++++++++++" << endl;
	cout << "Day One: " << endl;
	for (auto i = One.begin(); i != One.end(); ++i)
	    std::cout << *i << ' ';

	cout << endl;

	cout << "Day Two: " << endl;
	for (auto i = Two.begin(); i != Two.end(); ++i)
	    std::cout << *i << ' ';

	cout << endl;

	cout << "Day Three: " << endl;
	for (auto i = Three.begin(); i != Three.end(); ++i)
	    std::cout << *i << ' ';

	cout << endl;

	cout << "Day Four: " << endl;
	for (auto i = Four.begin(); i != Four.end(); ++i)
	    std::cout << *i << ' ';

	cout << endl;

	cout << "Day Five: " << endl;
	for (auto i = Five.begin(); i != Five.end(); ++i)
	    std::cout << *i << ' ';
	
	cout << endl;
}

int main (){

	// initial input
	initialInput();
	
	// generate days data
	generateDaysData();
	
	// display values of each day
	printDaysData();


	return 0;
	
}