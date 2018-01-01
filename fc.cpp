#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

std::random_device rd;  
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dist(0.0,1.0); // uniform real generator

int main (int argc, char* argv[]) {
  
  if(argv[1] == NULL) {
    cout << "File missing!" << endl;
    return EXIT_FAILURE;
  }
  
  ifstream file;
  cout << "Reading\t\t" << argv[1] << endl;
  file.open(argv[1]);

  uint N;
  file >> N;
  cout << "Forecasting \t\t" << N << "days..." << endl;

  //
  string label;
  float value;
  string marker;
  
  // parsing daily events
  cout << "\nDaily events:\n";
  uint no_daily_events = 0 ;
  float sum_daily_value = 0.;
  file >> marker; // daily
  while (marker.compare("hebdo") != 0) {
    file >> label;
    if (label.compare("hebdo") == 0) {
      break;
    }
    else {
      file >> value;
      sum_daily_value += value;
      cout << label << ": " << value << endl;
    }
    no_daily_events++;
  }

  // parsing weekly events
  cout << "\nWeekly events:\n";
  uint no_weekly_events = 0;
  float sum_weekly_value = 0.;
  while (marker.compare("mensuel") != 0) {
    file >> label;
    if (label.compare("mensuel") == 0) {
      break;
    } else {
      file >> value;
      sum_weekly_value += value;
      cout << label << ": " << value << endl;
    }
    no_weekly_events++;
  }

  // parsing monthly events
  cout << "\nMonthly events:\n";
  uint no_monthly_events = 0;
  float sum_monthly_value = 0.;
  while (marker.compare("ponctuel") != 0) {
    file >> label;
    if (label.compare("ponctuel") == 0) {
      break;
    } else {
      file >> value;
      sum_monthly_value += value;
      cout << label << ": " << value << endl;
    }
    no_monthly_events++;
  }
  
  // parsing ponctual events
  cout << "\nPunctual events:\n";
  uint no_punctual_events = 0;
  uint index = 0;
  vector<float> annual_events(365,0.);
  while (marker.compare("random") != 0) {
    file >> label;
    if (label.compare("random") == 0) {
      break;
    } else {
      file >> value;
      file >> index;
      if( index > 0 ) {
	annual_events[index-1] += value;
      } else {
	cout << "Error: one of the punctual event has an incorrect value" << endl;
	return EXIT_FAILURE;
      }
      cout << label << ": " << value << " at day " << index << endl;
    }
    no_punctual_events++;
  }
  
  // parsing random events
  cout << "\nRandom events:\n";
  vector<pair<float,float>> random_events;
  uint no_random_events = 0;
  float proba = 0;
  while (!file.eof()) {
    file >> label;
    file >> value;
    file >> proba;
    cout << label << ": " << value << " with proba " << proba << endl;
    random_events.push_back(make_pair(value,proba));
    no_random_events++;
  }

  cout << "\n";
  cout <<  no_daily_events << " daily events sum=" << sum_daily_value << endl;
  cout <<  no_weekly_events << " weekly events sum=" << sum_weekly_value << endl;
  cout <<  no_monthly_events << " monthly events sum=" << sum_monthly_value << endl;


  float day_before = 0.;
  cout << "Enter initial value: ";
  cin >> day_before;
  float today;
  uint no_random_triggers = 0;
  // computing every day values
  for (int i = 1; i <= N; i++) {
    today = day_before + sum_daily_value;
    if (i % 7 == 0) { today += sum_weekly_value; }
    if (i % 30 == 0) { today += sum_monthly_value; }
    today += annual_events[i % 365];

    float pick_proba = dist(gen);
    for (auto & pair : random_events) {
      if ( pick_proba < pair.second) {
	today += pair.first;
	no_random_triggers++;	
      }
    }
    //cout << i << "\t" << today << endl;
    day_before = today;
  }
  cout << "After " << N << " days: " << today << endl;
  
  file.close();
  return EXIT_SUCCESS;
}

// Local Variables:
// compile-command: "g++ fc.cpp -o fc.exe"
// End:
