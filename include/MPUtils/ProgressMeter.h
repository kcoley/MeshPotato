

#ifndef __PROGRESSMETER_H__
#define __PROGRESSMETER_H__

#include <ctime>
#include <iostream>
#include <sstream>
using namespace std;

namespace MeshPotato {
namespace MPUtils
{


template <typename T> 
string tostr(const T& t) { std::ostringstream os; os<<t; return os.str(); }



class ProgressMeter
{
  public:


    ProgressMeter( long nb, string ttl ) :
       totalnumber (nb),
       counter (0),
       currentPercent (0),
       oldPercent (0),
       estimatedTimeRemaining (0.0),
       currentUsedTime (0.0),
       estimatedTotalTime (0.0),
       title (ttl),
       startTime (time(NULL)),
       initial (true)
    {
       cout << "\nProgress Meter \"" << title << "\" active\n" << flush;
    }

   ~ProgressMeter()
    {
       time_t currentTime = time(NULL);
       currentUsedTime = currentTime-startTime;
       cout << "\r" << title << ":   Total time to finish: " << formattedTime(currentUsedTime) << " seconds                                                                                \n\n" << flush;

    }


    void update()
    {
       ++counter;
       time_t currentTime = time(NULL);
       currentPercent = (int)( 100.0*(float)counter/(float)totalnumber );
       currentUsedTime = currentTime-startTime;
       estimatedTotalTime = currentUsedTime * ((float)totalnumber/(float)counter);
       estimatedTimeRemaining = estimatedTotalTime - currentUsedTime;
       if( currentPercent > oldPercent || ( currentUsedTime > 0 && initial) )
       {
          initial = false;
          oldPercent = currentPercent;
          cout << "\r" << title << ":   Finished " << formattedPercent(currentPercent) << "   Time used: " << formattedTime(currentUsedTime) << "   estimated Time Left: " << formattedTime(estimatedTimeRemaining) << "   estimated Total Time: " << formattedTime(estimatedTotalTime) << "                      " << flush;
       }
    }

    void reset()
    {
       counter = 0;
       currentPercent = oldPercent = 0;
       initial = true;
       estimatedTotalTime = estimatedTimeRemaining = currentUsedTime = 0.0;
       startTime = time(NULL);
    }

  private:

    long totalnumber;
    long counter;
    int currentPercent, oldPercent;
    float estimatedTimeRemaining;
    float currentUsedTime;
    float estimatedTotalTime;

    string title;

    time_t startTime;
    bool initial;


    string formattedTime( float seconds )
    {
       int days = seconds/(24*60*60);
       seconds -= days*(24*60*60);
       int hours = seconds/(60*60);
       seconds -= hours*(60*60);
       int minutes = seconds/(60);
       seconds -= minutes*60;
       int secs = seconds;

       string sdays = tostr(days);
       if( days < 10 ) { sdays = "0" + sdays; }

       string shours = tostr(hours);
       if( hours < 10 ) { shours = "0" + shours; }

       string sminutes = tostr(minutes);
       if( minutes < 10 ) { sminutes = "0" + sminutes; }

       string sseconds = tostr(secs);
       if( secs < 10 ) { sseconds = "0" + sseconds; }

       return sdays + ":" + shours + ":" + sminutes + ":" + sseconds;
   }

   string formattedPercent( int perc )
   {
      string result = tostr(perc);
      if( perc < 100 ){ result = " " + result; }
      if( perc < 10 ) { result = " " + result; }
      result = result + " %";
      return result;
   }

};




}
}
#endif
