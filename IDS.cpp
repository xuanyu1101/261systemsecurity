#include <stdio.h>
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
#include <numeric>

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

//Store mean data for each event
vector <float> meanlogins; // Logins
vector <float> meanTO; //Time online
vector <float> meanES; // Emails Sent
vector <float> meanEO; // Emails Opened
vector <float> meanED; // Emails Deleted

float sumlogins = 0.00, Lvariance = 0.00, LstdDeviation = 0.00;
float sumTO = 0.00, TOvariance = 0.00, TOstdDeviation = 0.00;
float sumES = 0.00, ESvariance = 0.00, ESstdDeviation = 0.00;
float sumEO = 0.00, EOvariance = 0.00, EOstdDeviation = 0.00;
float sumED = 0.00, EDvariance = 0.00, EDstdDeviation = 0.00;

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
	cout << "+++++++++++++++++++++++" << endl;
	cout << "Outputting logs to Logs.txt file..." << endl;
	std::ofstream outputLogs ("Logs.txt", std::ofstream::out);
	copy(One.begin(), One.end(), ostream_iterator<float>(outputLogs, ":"));
	outputLogs << endl;
	copy(Two.begin(), Two.end(), ostream_iterator<float>(outputLogs, ":"));
	outputLogs << endl;
	copy(Three.begin(), Three.end(), ostream_iterator<float>(outputLogs, ":"));
	outputLogs << endl;	
	copy(Four.begin(), Four.end(), ostream_iterator<float>(outputLogs, ":"));
	outputLogs << endl;
	copy(Five.begin(), Five.end(), ostream_iterator<float>(outputLogs, ":"));
	outputLogs << endl;
	outputLogs.close();
	

	cout << "**********************" << endl;
	cout << "Generating Mean for number of logins..." << endl;
	meanlogins.insert (meanlogins.begin(), One[0]);
	meanlogins.insert (meanlogins.begin(), Two[0]);
	meanlogins.insert (meanlogins.begin(), Three[0]);
	meanlogins.insert (meanlogins.begin(), Four[0]);
	meanlogins.insert (meanlogins.begin(), Five[0]);
	for (auto j = meanlogins.rbegin(); j!= meanlogins.rend(); j++)
	sumlogins += *j;
	cout << "Sum of total logins in " << meanlogins.size() << " days is " << sumlogins << endl;
	//Calculating mean of Logins
	float mean_of_logins = sumlogins / meanlogins.size();
	cout << "Login Mean is " << mean_of_logins << endl;
	
	//Calculating Variance of Logins
	for (auto j = meanlogins.rbegin(); j!= meanlogins.rend(); j++)
	Lvariance += pow(*j - mean_of_logins, 2);
	Lvariance = Lvariance/5;
	//Calculating Standard Deviation of Logins
	LstdDeviation = sqrt(Lvariance);
	cout << "Standard Deviation for Login is: " << LstdDeviation << endl;
	cout << endl;
	
	cout << "**********************" << endl;
	cout << "Generating Mean for number of Time Online..." << endl;
	meanTO.insert (meanTO.begin(), One[1]);
	meanTO.insert (meanTO.begin(), Two[1]);
	meanTO.insert (meanTO.begin(), Three[1]);
	meanTO.insert (meanTO.begin(), Four[1]);
	meanTO.insert (meanTO.begin(), Five[1]);
	for (auto j = meanTO.rbegin(); j!= meanTO.rend(); j++)
	sumTO += *j;
	cout << "Sum of total Time Online in " << meanTO.size() << " days is " << sumTO << endl;
	//Calculating mean of Time Online
	float mean_of_TO = sumTO / meanTO.size();
	cout << "Time Online Mean is " << mean_of_TO << endl;
		
	//Calculating Variance of Time Online
	for (auto j = meanTO.rbegin(); j!= meanTO.rend(); j++)
	TOvariance += pow(*j - mean_of_TO, 2);
	TOvariance = TOvariance/5;
	//Calculating Standard Deviation of Time Online
	TOstdDeviation = sqrt(TOvariance);
	cout << "Standard Deviation for Time Online is: " << TOstdDeviation << endl;
	cout << endl;

	cout << "**********************" << endl;
	cout << "Generating Mean for number of Emails Sent..." << endl;
	meanES.insert (meanES.begin(), One[2]);
	meanES.insert (meanES.begin(), Two[2]);
	meanES.insert (meanES.begin(), Three[2]);
	meanES.insert (meanES.begin(), Four[2]);
	meanES.insert (meanES.begin(), Five[2]);
	for (auto j = meanES.rbegin(); j!= meanES.rend(); j++)
	sumES += *j;
	cout << "Sum of total Emails Sent in " << meanES.size() << " days is " << sumES << endl;
	cout << endl;
	//Calculating mean of Emails Sent
	float mean_of_ES = sumES / meanES.size();
	cout << "Emails Sent Mean is " << mean_of_ES << endl;

	//Calculating Variance of Emails Sent
	for (auto j = meanES.rbegin(); j!= meanES.rend(); j++)
	ESvariance += pow(*j - mean_of_ES, 2);
	ESvariance = ESvariance/5;
	//Calculating Standard Deviation of Emails Sent
	ESstdDeviation = sqrt(ESvariance);
	cout << "Standard Deviation for Time Online is: " << ESstdDeviation << endl;
	cout << endl;

	
	cout << "**********************" << endl;
	cout << "Generating Mean for number of Emails Opened..." << endl;
	meanEO.insert (meanEO.begin(), One[3]);
	meanEO.insert (meanEO.begin(), Two[3]);
	meanEO.insert (meanEO.begin(), Three[3]);
	meanEO.insert (meanEO.begin(), Four[3]);
	meanEO.insert (meanEO.begin(), Five[3]);
	for (auto j = meanEO.rbegin(); j!= meanEO.rend(); j++)
	sumEO += *j;
	cout << "Sum of total Emails Sent in " << meanEO.size() << " days is " << sumEO << endl;
	//Calculating mean of Emails Opened
	float mean_of_EO = sumEO / meanEO.size();
	cout << endl;
	cout << "Emails Sent Mean is " << mean_of_EO << endl;
	//Calculating Variance of Emails Opened
	for (auto j = meanEO.rbegin(); j!= meanEO.rend(); j++)
	EOvariance += pow(*j - mean_of_EO, 2);
	EOvariance = EOvariance/5;
	//Calculating Standard Deviation of Emails Opened
	EOstdDeviation = sqrt(EOvariance);
	cout << "Standard Deviation for Time Online is: " << EOstdDeviation << endl;
	cout << endl;


	cout << "**********************" << endl;
	cout << "Generating Mean for number of Emails Deleted..." << endl;
	meanED.insert (meanED.begin(), One[4]);
	meanED.insert (meanED.begin(), Two[4]);
	meanED.insert (meanED.begin(), Three[4]);
	meanED.insert (meanED.begin(), Four[4]);
	meanED.insert (meanED.begin(), Five[4]);
	for (auto j = meanED.rbegin(); j!= meanED.rend(); j++)
	sumED += *j;
	cout << "Sum of total Emails Deleted in " << meanED.size() << " days is " << sumED << endl;
	//Calculating mean of Emails Deleted
	float mean_of_ED = sumED / meanED.size();
	cout << "Emails Deleted Mean is " << mean_of_ED << endl;
	//Calculating Variance of Emails Deleted
	for (auto j = meanED.rbegin(); j!= meanED.rend(); j++)
	EDvariance += pow(*j - mean_of_ED, 2);
	EDvariance = EDvariance/5;
	//Calculating Standard Deviation of Emails Opened
	EDstdDeviation = sqrt(EDvariance);
	cout << "Standard Deviation for Emails Deleted is: " << EDstdDeviation << endl;
	cout << endl;
}



int main (){

	// initial input
	initialInput();
	
	// generate days data
	generateDaysData();
	
	// display values of each day
	printDaysData();

	//mean and stdDev
	//mean();


	return 0;
	
}
