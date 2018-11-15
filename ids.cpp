#include <stdio.h>
<<<<<<< HEAD
=======
#include <stdlib.h>
#include <cmath>
>>>>>>> parent of d6e4177... Done
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

<<<<<<< HEAD
// map for stats 
// pair<string, vector<string>>(statName, statVals - (mean) and (standardDev))
unordered_map<string, vector<string>> statsMap;
=======


// map for stats 
// pair<string, vector<string>>(statName, statVals - (mean) and (standardDev))
static unordered_map<string, vector<string>> statsMap;
>>>>>>> parent of d6e4177... Done

//vector<string> statVals; // vector for mean and standardDev

// map for events
// pair<string, vector<string>>(eventName, eventVals - (eventType), (minVal), (maxVal and (weight))
<<<<<<< HEAD
unordered_map<string, vector<string>> eventsMap;
=======
static unordered_map<string, vector<string>> eventsMap;

//vector<string> eventVals; // vector for (eventType), (minVal), (maxVal and (weight)

static vector<vector<float> > trainingData;
static vector<string> indexVec;
static vector<pair<float,float>> meanSDvec;	//vector of pair(mean, stdDev) of each event

>>>>>>> parent of d6e4177... Done

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
<<<<<<< HEAD
void valuesForDays(string input, int decneeded) {	
=======
float getVal(string input, int decneeded) {	
>>>>>>> parent of d6e4177... Done
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

<<<<<<< HEAD
		temp.push_back(randData); // store in temp data
=======
		//add to randData (to make it either 0.5 or 1.0)
		randData = randData + (diff/10);
	}
	
	// no decimal place 
	else {
		string randD = prd(randData, 0);
		string::size_type st_r;  
		randData = stof (randD,&st_r); // convert from string to float 
	}

	return randData;
		
	// push to individual days
	//One.push_back(temp[0]);
	//Two.push_back(temp[1]);
	//Three.push_back(temp[2]);
	//Four.push_back(temp[3]);
	//Five.push_back(temp[4]);
}

void generateTrngData(string numOfDays) {
	int days = stoi(numOfDays);
	string eventName;
	vector<string> eventVals;
	vector<float> daysVal;
	int decneeded = 0;
	float data;
	
	//storing the eventNames found in stats.txt into a vector to correspond to the index
	for(auto& index: statsMap){
		indexVec.push_back(index.first);
	}
>>>>>>> parent of d6e4177... Done
		
    	}	

	// push to individual days
	One.push_back(temp[0]);
	Two.push_back(temp[1]);
	Three.push_back(temp[2]);
	Four.push_back(temp[3]);
	Five.push_back(temp[4]);
}

<<<<<<< HEAD
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
=======
// print data from each day
void printDaysData(){
	
	cout << "========================================" << endl;
	cout << "	ACTIVITY STIMULATION ENGINE	" << endl;
	cout << "========================================" << endl;

	//display generated data for x days on screen
	for(int i =0; i < trainingData.size(); i++){
		cout << "Day: " << i+1 << endl;
		cout << endl;

		for(int i =0; i < indexVec.size(); i++){
			cout << indexVec[i] << ":";
		}

		cout << endl;
		
		for(int j=0; j < trainingData[i].size(); j++)
		{
			cout << trainingData[i][j] << ":";
		}
		
		cout << endl;
		cout << endl;
	}

	//storing of data for x days in Logs.txt
	cout << "\n[LOGS STORED IN Logs.txt]" << endl;

	std::ofstream outputLogs ("Logs.txt", std::ofstream::out);

	//obtain the index (eventName) corresponding to the values stored in each vector in trainingData
	//i.e trainingdata[i][0] = Emails Deleted
	//trainingdata[i][1] = Emails opened
	//trainingdata[i][2] = Emails sent
	//trainingdata[i][3] = Time Online
	//trainingdata[i][4] = Logins
	//to store in the first line of Logs.txt

	for(int i =0; i < indexVec.size(); i++){
		outputLogs << indexVec[i] << ":";
	}
	
	outputLogs<< endl;

	for(int i =0; i < trainingData.size(); i++){
		for(int j=0; j < trainingData[i].size(); j++)
		{
			outputLogs << trainingData[i][j] << ":";
		}
		outputLogs << endl;
	}
>>>>>>> parent of d6e4177... Done

	cout << endl;
<<<<<<< HEAD

	cout << "Day Two: " << endl;
	for (auto i = Two.begin(); i != Two.end(); ++i)
	    std::cout << *i << ' ';

	cout << endl;
=======
	
}

void calMeanStdDev(){

	float sum, mean, variance, stdDev;
	
	cout << "=============================" << endl;
	cout << "	ANALYSIS ENGINE		" << endl;
	cout << "=============================" << endl;

	for(int i = 0; i < statsMap.size(); i++){
		//reset values
		sum = 0;
		variance = 0;

		cout << "Generating mean & standard deviation for " << indexVec[i] << endl;
		
		for(int j = 0; j < trainingData.size(); j++){
			sum += trainingData[j][i];
		}

		mean = sum/trainingData.size();
		
		cout << "Mean: " << mean << endl;

		for(int k = 0; k < trainingData.size(); k++){
			variance += pow(trainingData[k][i] - mean, 2);
		}

		variance = variance/trainingData.size();
		stdDev = sqrt(variance);
		
		cout << "Standard Deviation: " << stdDev << endl;

		meanSDvec.push_back(make_pair(mean, stdDev)); //vector storing the mean and standard deviation for each event
		
		cout << endl;	
	}
	
	cout << endl;
}

void calAnomaly(){

	cout << "=============================" << endl;
	cout << "	ALERT ENGINE		" << endl;
	cout << "=============================" << endl;

	//Anomaly calculation = (absolute(mean-dailycount)/std dev)* weight
	float dailyTH;
	int eventWeight, totWeight;
	unordered_map<string, vector<string>>::const_iterator itr;
	
	for(int i = 0; i < trainingData.size(); i++){

		//reset values
		dailyTH = 0;
		totWeight = 0;

		for(int j = 0; j < statsMap.size(); j++){
			
		    //find the eventName in eventsMap for the events in stats.txt
		    itr = eventsMap.find(indexVec[j]);
		    
		    if( itr != eventsMap.end() )
		    {
			eventWeight = stoi(itr->second[3]);
			totWeight = totWeight+eventWeight;
		    }
			
		    //sum of anomaly counters for each event to get the daily threshold value
		    dailyTH += (abs(meanSDvec[j].first-trainingData[i][j])/meanSDvec[j].second)*eventWeight;
		}

		//acceptable threshold is 2 times of total weights of all events
		totWeight = totWeight*2;
>>>>>>> parent of d6e4177... Done

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

<<<<<<< HEAD
int main (){

	// initial input
	initialInput();
	
	// generate days data
	generateDaysData();
	
	// display values of each day
	printDaysData();
=======
int main(int argc, char* argv[]){

	srand(time(NULL));

	// initial input
	//argv[1] = Event.txt 
	//argv[2] = Stats.txt
	//argv[3] = x days of data to generate

	initialInput(argv[1], argv[2]);	//reading in of files and storing into an unordered_map
	generateTrngData(argv[3]);	//generate training data for x days
	 
	// display values of each day for training data
	printDaysData();

	//mean and stdDev
	calMeanStdDev();
	
	//Anamoly Counter
	calAnomaly();
>>>>>>> parent of d6e4177... Done


	return 0;
	
}