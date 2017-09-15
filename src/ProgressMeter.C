#include "MeshPotato/MPUtils/ProgressMeter.h"
#include <ctime>
#include <iostream>
#include <sstream>
using namespace std;

namespace MeshPotato {
namespace MPUtils
{
  template <typename T> 
string tostr(const T& t) { std::ostringstream os; os<<t; return os.str(); }

  class ProgressMeter::Impl {
  public:
    Impl(long nb, string ttl): 
      totalnumber (nb),
       counter (0),
       currentPercent (0),
       oldPercent (0),
       estimatedTimeRemaining (0.0),
       currentUsedTime (0.0),
       estimatedTotalTime (0.0),
       title (ttl),
       startTime (time(NULL)),
       initial (true) {}

    long totalnumber;
    long counter;
    int currentPercent, oldPercent;
    float estimatedTimeRemaining;
    float currentUsedTime;
    float estimatedTotalTime;

    string title;

    time_t startTime;
    bool initial;


    string formattedTime( float seconds );

   string formattedPercent( int perc );
  };





    ProgressMeter::ProgressMeter( long nb, string ttl ) : mImpl(new Impl(nb, ttl)) 
    {
       cout << "\nProgress Meter \"" << mImpl->title << "\" active\n" << flush;
    }

   ProgressMeter::~ProgressMeter()
    {
       time_t currentTime = time(NULL);
       mImpl->currentUsedTime = currentTime-mImpl->startTime;
       cout << "\r" << mImpl->title << ":   Total time to finish: " << mImpl->formattedTime(mImpl->currentUsedTime) << " seconds                                                                                \n\n" << flush;

    }


    void ProgressMeter::update()
    {
       ++mImpl->counter;
       time_t currentTime = time(NULL);
       mImpl->currentPercent = (int)( 100.0*(float)mImpl->counter/(float)mImpl->totalnumber );
       mImpl->currentUsedTime = currentTime-mImpl->startTime;
       mImpl->estimatedTotalTime = mImpl->currentUsedTime * ((float)mImpl->totalnumber/(float)mImpl->counter);
       mImpl->estimatedTimeRemaining = mImpl->estimatedTotalTime - mImpl->currentUsedTime;
       if( mImpl->currentPercent > mImpl->oldPercent || ( mImpl->currentUsedTime > 0 && mImpl->initial) )
       {
          mImpl->initial = false;
          mImpl->oldPercent = mImpl->currentPercent;
          cout << "\r" << mImpl->title << ":   Finished " << mImpl->formattedPercent(mImpl->currentPercent) << "   Time used: " << mImpl->formattedTime(mImpl->currentUsedTime) << "   estimated Time Left: " << mImpl->formattedTime(mImpl->estimatedTimeRemaining) << "   estimated Total Time: " << mImpl->formattedTime(mImpl->estimatedTotalTime) << "                      " << flush;
       }
    }

    void ProgressMeter::reset()
    {
       mImpl->counter = 0;
       mImpl->currentPercent = mImpl->oldPercent = 0;
       mImpl->initial = true;
       mImpl->estimatedTotalTime = mImpl->estimatedTimeRemaining = mImpl->currentUsedTime = 0.0;
       mImpl->startTime = time(NULL);
    }



    string ProgressMeter::Impl::formattedTime( float seconds )
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

   string ProgressMeter::Impl::formattedPercent( int perc )
   {
      string result = tostr(perc);
      if( perc < 100 ){ result = " " + result; }
      if( perc < 10 ) { result = " " + result; }
      result = result + " %";
      return result;
   }





}
}
