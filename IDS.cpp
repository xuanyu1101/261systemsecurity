#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <cmath>
#include <string.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iterator>
#include <vector>
#include <math.h>
#include <time.h>
#include <iomanip>
#include <unordered_map>
#include <limits>
#include <numeric>

using namespace std;

// map for events distribution found in stats file
static unordered_map<string, vector<string>> statsMap;	//training data 
static unordered_map<string, vector<string>> liveStatsMap; //live data

// map for events
// pair<string, vector<string>>(eventName, eventVals - (eventType), (minVal), (maxVal and (weight))
static unordered_map<string, vector<string>> eventsMap;

// vectors storing generated data for x days 
static vector<vector<float> > trainingData; //training data
static vector<vector<float> > liveData; //live data

// vectors storing event name that is the index of each generated value in stats map
static vector<string> indexVec; //training data
static vector<string> liveIndexVec; //live data

// vectors of pair(mean, stdDev) of each event 
static vector<pair<float,float>> meanSDvec; //training data	
static vector<pair<float,float>> livemeanSDvec; //live data

//Storing data in events.txt and stats.txt into the respective vectors and maps
//Arguments: eFile = input event file name
//	     sFile = input stats file name

void initialInput(string eFile, string sFile) {

	cout << "=============================" << endl;
	cout << "	INITIAL ENGINE	" << endl;
	cout << "=============================" << endl;

	string line, eventName, eventType, minVal, maxVal, weight, statName, mean, standardDev;
	int counter = 0;
	int numOfEvents;
	ifstream eventFile(eFile);
	ifstream statsFile(sFile);

	//=================================== Storing values from Events.txt ======================================================

	cout << "1. Printing values from Events.txt" << endl;
	cout << endl;

	//obtain individual event value split by delimiter ':'
	while (getline(eventFile, line)){
		stringstream ss(line);	
		vector<string> eventVals; // vector for event values 

		if(counter == 0){
			numOfEvents = stoi(line);
		}
	
		
		//push each event's values into a vector
		else if(counter <= numOfEvents){
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

	cout << "Event Type MinVal MaxVal Weight\n" << endl;

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
	cout << endl;
	counter = 0;

	//================================= Storing values from Stats.txt =============================================================

	cout << "2. Printing values from Stats.txt" << endl;
	cout << endl;

	//obtain individual event value split by delimiter ':'
	while (getline(statsFile, line)){
		stringstream ss(line);	
		vector<string> statVals; // vector for mean and standardDev
		

		if(counter == 0){
			numOfEvents = stoi(line);
		}

		//push each event's values into a vector
		else if(counter <= numOfEvents){
			getline(ss, statName, ':');
			getline(ss, mean, ':');
			getline(ss, standardDev, ':');

			statVals.push_back(mean);
			statVals.push_back(standardDev);

			statsMap.insert(pair<string, vector<string>>(statName, statVals));
			
		}		

		counter++;
	}

	
	cout << "Event Mean StdDev\n" << endl;

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
	cout << endl;
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

//get current system date & time
string datetime()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%d-%m-%Y %H-%M-%S",timeinfo);
    return string(buffer);
}

// Creates 5 days of data
// Arguments:	input = obtain event name
//		decneeded = determine if event is continuous or discrete 

float getVal(string input, int decneeded) {	
	// find from stats map according to string
	unordered_map<string, vector<string>>::const_iterator got = statsMap.find(input);

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
	
	float randData; 
	total = 0; // reset total to 0
	


	randData = RandomFloat(max, min);
		
	// one decimal place
	if (decneeded == 1) {

		string randD = prd(randData, 1); //convert float to string

		//round the last value of the string
		char ch = *randD.rbegin(); //get last character of string
		string LastChar;
		LastChar.push_back(ch); //convert char to string

		string::size_type st_r;  
		float chFloat = stof(LastChar,&st_r); //convert from string to float
		randData = stof (randD,&st_r); // convert from string to float
		
		float diff; //variable for difference to add to randData
		
		//if below 0.5, round up to 0.5
		if (chFloat < 5){
			//find out the difference to be added to randData
			diff = 5 - chFloat;
		}
	
		//if above 0.5, round up to single integer
		else if (chFloat > 5){
			diff = 10 - chFloat;
		}

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
}

//Generate training data
//Arguments: numOfDays = obtain number of days of data to generate 

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
		
	
	//generate events for x days of data
	for(int i = 0; i < days; i++){

		for (auto& event: statsMap) {

		    //find the event stats in eventsMap for the events in stats.txt
		    unordered_map<string, vector<string>>::const_iterator itr = eventsMap.find(event.first);
		    
		    if( itr != eventsMap.end() )
		    {
			eventVals = itr->second;

			    //check if event is continuous or discrete	
			    // float needed? 1 - Yes, 0 - No	    
			    if(eventVals[0] == "D")
				decneeded = 0; 
			    else
				decneeded = 1;
			
		    
			    eventName = event.first;
			    data = getVal(eventName, decneeded);
			    daysVal.push_back(data);
		    }
		}

		trainingData.push_back(daysVal);
		daysVal.clear();
	}
}

//generate Live Data
//Arguments: numOfDays = obtain number of days of data to generate
//	     sFile = obtain input stats file

void generateLiveData(int numOfDays, string sFile) {
	//check if liveStatsMap exists. If exists, delete contents.
	if(liveStatsMap.size() > 0){
		liveStatsMap.clear();
	}

	//check if liveindexVec exists. If exists, delete contents.
	if(liveIndexVec.size() > 0){
		liveIndexVec.clear();
	}

	//check if liveData exists. If exists, delete contents.
	if (liveData.size() > 0){
		liveData.clear();
	}

	int days = numOfDays;
	int counter = 0;
	int numOfEvents;
	string eventName, line, statName, mean, standardDev;
	vector<string> statVals; // vector for mean and standardDev
	
	vector<string> eventVals;
	vector<float> daysVal;
	int decneeded = 0;
	float data;
	
	//storing of live stats from newstats.txt into vector
	//obtain individual event value split by delimiter ':'

	ifstream statsFile(sFile);

	while (getline(statsFile, line)){
		stringstream ss(line);	
		

		if(counter == 0){
			numOfEvents = stoi(line);
		}

		//push each event's values into a vector
		else if(counter <= numOfEvents){
			getline(ss, statName, ':');
			getline(ss, mean, ':');
			getline(ss, standardDev, ':');

			statVals.push_back(mean);
			statVals.push_back(standardDev);

			liveStatsMap.insert(pair<string, vector<string>>(statName, statVals));
			
		}		

		counter++;
	}

	statsFile.close();

	//storing the eventNames found in newstats.txt into a vector to correspond to the index
	for(auto& index: liveStatsMap){
		liveIndexVec.push_back(index.first);
	}
		
	
	//generate events for x days of data
	for(int i = 0; i < days; i++){

		
		for (auto& event: liveStatsMap) {

		    //find the event stats in eventsMap for the events in stats.txt
		    unordered_map<string, vector<string>>::const_iterator itr = eventsMap.find(event.first);
		    
		    if( itr != eventsMap.end() )
		    {
			eventVals = itr->second;

			    //check if event is continuous or discrete	
			    // float needed? 1 - Yes, 0 - No	    
			    if(eventVals[0] == "D")
				decneeded = 0; 
			    else
				decneeded = 1;
			
		    
			    eventName = event.first;
			    data = getVal(eventName, decneeded);
			    daysVal.push_back(data);  
		    }
		
		}

		liveData.push_back(daysVal);
		daysVal.clear();
	}

}

// print data from each day 
//Arguments: dataVec = vector of generated data
//	     idxVec = vector of corresponding indexes found in stats file
//	     outputFile = file to store generated data 

void printDaysData(vector<vector<float> > dataVec, vector<string> idxVec, string outputFile){
	
	cout << "========================================" << endl;
	cout << "	ACTIVITY SIMULATION ENGINE	" << endl;
	cout << "========================================" << endl;

	//display generated data for x days on screen
	for(int i =0; i < dataVec.size(); i++){
		cout << "Day: " << i+1 << endl;
		cout << endl;

		for(int i =0; i < idxVec.size(); i++){
			cout << idxVec[i] << ":";
		}

		cout << endl;
		
		for(int j=0; j < dataVec[i].size(); j++)
		{
			cout << dataVec[i][j] << ":";
		}
		
		cout << endl;
		cout << endl;
	}

	cout << "[ EVENTS GENERATION FOR " << dataVec.size() << " DAYS HAVE COMPLETED. ]" << endl;

	//storing of data for x days in Logs.txt
	cout << "\n[LOGS STORED IN " << outputFile << "]" << endl;

	std::ofstream outputLogs (outputFile, std::ofstream::out);

	//obtain the index (eventName) corresponding to the values stored in each vector in trainingData
	//i.e trainingdata[i][0] = Emails Deleted
	//trainingdata[i][1] = Emails opened
	//trainingdata[i][2] = Emails sent
	//trainingdata[i][3] = Time Online
	//trainingdata[i][4] = Logins
	//to store in the first line of Logs.txt

	for(int i =0; i < idxVec.size(); i++){
		outputLogs << idxVec[i] << ":";
	}
	
	outputLogs<< endl;

	for(int i =0; i < dataVec.size(); i++){
		for(int j=0; j < dataVec[i].size(); j++)
		{
			outputLogs << dataVec[i][j] << ":";
		}
		outputLogs << endl;
	}

	outputLogs.close();
	cout << endl;
	
}

//Calculate mean and standard deviation
//Arguments: instatsMap = vector of events found in stats file
//	     dataVec = vector of generated data
//	     idxVec = vector of corresponding indexes found in stats file
//	     isTrainingData = check if user is testing training data or live data to store in the respective vectors
//	     outputFile = file to store the mean and stand deviation values for each event

void calMeanStdDev(unordered_map<string, vector<string>> instatsMap, vector<vector<float> > dataVec, vector<string> idxVec, bool isTrainingData, string outputFile){

	std::ofstream oFile(outputFile, std::ofstream::out);
	oFile << "Event:Mean:Standard Deviation:" << endl;

	//check if liveStatsMap exists. If exists, delete contents.
	if(livemeanSDvec.size() > 0){
		livemeanSDvec.clear();
	}

	float sum, mean, variance, stdDev;
	
	cout << "=============================" << endl;
	cout << "	ANALYSIS ENGINE		" << endl;
	cout << "=============================" << endl;


	//loop through map containing the stats from stats file.
	//get standard deviation and mean across generated x days' data for each event in the stats file
	for(int i = 0; i < instatsMap.size(); i++){
		//reset values
		sum = 0;
		variance = 0;

		cout << "Generating mean & standard deviation for " << idxVec[i] << endl;
		
		for(int j = 0; j < dataVec.size(); j++){
			sum += dataVec[j][i];
		}

		//calculate mean
		mean = sum/dataVec.size();
		
		//print in 2 dp, but store in vector as precise as possible so to not affect future calculations
		cout << "Mean: " << stof(prd(mean, 2)) << endl; 

		for(int k = 0; k < dataVec.size(); k++){
			variance += pow(dataVec[k][i] - mean, 2);
		}

		//obtain standard deviation
		variance = variance/dataVec.size();
		stdDev = sqrt(variance);
		
		//print in 2 dp, but store in vector as precise as possible so to not affect future calculations
		cout << "Standard Deviation: " << stof(prd(stdDev, 2)) << endl; 

		if(isTrainingData){
			meanSDvec.push_back(make_pair(mean, stdDev)); //vector storing the mean and standard deviation for each event
		}

		else
			livemeanSDvec.push_back(make_pair(mean, stdDev)); //vector storing the mean and standard deviation for each event

		cout << endl;	


		//store mean and standard deviation (in 2 d.p) for each event in a logfile
		oFile << idxVec[i] << ":" << stof(prd(mean, 2)) << ":" << stof(prd(stdDev, 2)) << ":" << endl;
	}
	
	cout << "\n[MEAN AND STANDARD DEVIATION CALCULATIONS FOR EACH EVENT STORED IN " << outputFile << "]" << endl;
	cout << endl;
}

//Calculate Daily Threshold and determine if there's anomaly
//Arguments: dataVec = vector of generated data
//	     idxVec = vector of corresponding indexes found in stats file
//	     instatsMap = map of events found in stats file
//	     meanSDVector = vector containin mean and standard deviation of generated data of each event

void calAnomaly(vector<vector<float> > dataVec, vector<string> idxVec, unordered_map<string, vector<string>> instatsMap, vector<pair<float,float>> meanSDVector){

	cout << "=============================" << endl;
	cout << "	ALERT ENGINE		" << endl;
	cout << "=============================" << endl;

	//Anomaly calculation = (absolute(mean-dailycount)/std dev)* weight
	float dailyTH;
	int eventWeight, totWeight;
	bool zeroDivError = false;
	unordered_map<string, vector<string>>::const_iterator itr;
	
	
	for(int i = 0; i < dataVec.size(); i++){

		//reset values
		dailyTH = 0;
		totWeight = 0;

		for(int j = 0; j < instatsMap.size(); j++){
			
		    //find the eventName in eventsMap for the events in stats.txt
		    itr = eventsMap.find(idxVec[j]);
		    
		    if( itr != eventsMap.end() )
		    {
			eventWeight = stoi(itr->second[3]);
			totWeight = totWeight+eventWeight;
		    }
		
		    //check if standard deviation = 0, if 0 will result in zero divison error. Handle exception. 
		    if(meanSDVector[j].second != 0){
		    //sum of anomaly counters for each event to get the daily threshold value
		    dailyTH += (abs(meanSDVector[j].first-dataVec[i][j])/meanSDVector[j].second)*eventWeight;
		    }

		    else{
			zeroDivError = true;
		    }
		}

		//handle zero division error
		if(zeroDivError != false){
			cout << "Unable to calculate anomaly due to zero division error! Please exit and run program again. " << endl;
			break;
		}

		else{
			//acceptable threshold is 2 times of total weights of all events
			totWeight = totWeight*2;

			
			//print in 2 dp
			cout << "Daily Threshold for day " << i+1 << ": " << stof(prd(dailyTH, 2)) << endl;
			cout << "Acceptable threshold per day is: " << totWeight << endl;

			//check if daily threshold exceeds acceptable threshold
			if(dailyTH > totWeight){
				cout << "[WARNING]: Day " << i+1 << " data is NOT OK." << endl;
			}

			else
				cout << "[NORMAL]: Day " << i+1 << " data is OK." << endl; 

			cout << endl;
		}
	}		

	
	cout << endl;
}

//main function
int main(int argc, char* argv[]){

	srand(time(NULL));

	string input, newStatsFile;
	int numOfDays;

	// initial input
	//argv[1] = Event.txt 
	//argv[2] = Stats.txt
	//argv[3] = x days of data to generate

	initialInput(argv[1], argv[2]);	//reading in of files and storing into an unordered_map
	generateTrngData(argv[3]);	//generate training data for x days
	 
	// display values of each day for training data
	printDaysData(trainingData, indexVec, "TrainingLogs.txt");

	//mean and stdDev
	calMeanStdDev(statsMap, trainingData, indexVec, true, "meanSDforTrainingData.txt");
	
	//Anamoly Counter
	calAnomaly(trainingData, indexVec, statsMap, meanSDvec);

	cout << "Training of IDS has completed. " << endl;

	//pause screen
	cin.ignore();
	cout << "Please press any key to continue..." << endl;
	cin.get();
	//system("pause");

	system("clear");
	
	//Menu option that allows user to input a file and number of days unless 'q' is specified
	do{

		bool valid = false;
		cout << endl;
		cout << "===================== IDS Program ============================ " << endl;
		cout << "		   [ Enter q to quit! ] " << endl;
		cout << endl;
		cout << "Enter a file: ";
		
		getline(cin, input);
		newStatsFile = input;
		
		//check if user enters 'q' 
		if (strcmp (input.c_str(), "q") != 0){			

			do{
				cout << "Enter number of days: ";
				cin >> numOfDays;
				if (cin.good())
				{
				    valid = true; //if user enters a number, get out of loop and continue
				}
				
				else
				{
				    cin.clear();
				    cin.ignore(numeric_limits<streamsize>::max(),'\n');
				    cout << "Invalid input; please re-enter." << endl;
				    cout << endl;
				}
			    } while (!valid);

			cout << endl;

			ifstream file(newStatsFile);

 			if (file.is_open()) {
				generateLiveData(numOfDays, newStatsFile);

				string dateTime = datetime();
				string outputFile = "logs-" + dateTime + ".txt"; 
				string calculatedFile = "meanSD-" + dateTime + ".txt";
				printDaysData(liveData, liveIndexVec, outputFile);
				calMeanStdDev(liveStatsMap, liveData, liveIndexVec, false, calculatedFile);
				calAnomaly(liveData, liveIndexVec, liveStatsMap, livemeanSDvec);
				
			}

			else{
				cout << "Invalid file entered. " << endl;
				cout << endl;
			}

		}	

		cin.clear(); 
		cin.ignore(numeric_limits<streamsize>::max(),'\n');		

	}while (strcmp (input.c_str(),"q") != 0);


	return 0;
	
}
