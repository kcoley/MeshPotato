

#ifndef __PROGRESSMETER_H__
#define __PROGRESSMETER_H__

#include <boost/shared_ptr.hpp>
#include <ctime>
#include <iostream>
#include <sstream>
using namespace std;

namespace MeshPotato {
namespace MPUtils
{

class ProgressMeter
{
  public:


    ProgressMeter( long nb, string ttl );

   ~ProgressMeter();

    void update();

    void reset();

  private:
    class Impl;
    boost::shared_ptr<Impl> mImpl;

};


}
}
#endif
