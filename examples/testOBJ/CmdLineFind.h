//========================================
//
//  Routines to find items on the command line
//
//
//
//
//
//
//=========================================

#ifndef __CMDLINEFIND__
#define __CMDLINEFIND__

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include "Vector.h"

namespace lux{

class CmdLineFind
{
  public:
   CmdLineFind( int argC, char** argV )
   {
      for( int i=0;i<argC;i++ )
      {
         items.push_back( argV[i] );
      }
   }

   ~CmdLineFind(){}

   //
   //   Find first occurance of a command line option
   //


   const float find( const std::string& tag, const float deflt, const std::string help = "no help" )
   {
      float result = deflt;
      std::vector<size_t> locate = find( tag );
      if( !locate.empty() && locate[0] < items.size()-1 )
      {
         result = atof( items[locate[0]+1].c_str() );
      }
      registerTag( tag, deflt, result, help );
      return result;
   }

   const int find( const std::string& tag, const int deflt, const std::string help = "no help" )
   {
      int result = deflt;
      std::vector<size_t> locate = find( tag );
      if( !locate.empty() && locate[0] < items.size()-1 )
      {
         result = atoi( items[locate[0]+1].c_str() );
      }
      registerTag( tag, deflt, result, help );
      return result;
   }

   const std::string find( const std::string& tag, const std::string deflt, const std::string help = "no help" )
   {
      std::string result = deflt;
      std::vector<size_t> locate = find( tag );
      if( !locate.empty() && locate[0] < items.size()-1 )
      {
         result = items[locate[0]+1];
      }
      registerTag( tag, deflt, result, help );
      return result;
   }


   const Vector find( const std::string& tag, const Vector deflt, const std::string help = "no help" )
   {
      Vector result = deflt;
      std::vector<size_t> locate = find( tag );
      if( !locate.empty() && locate[0] < items.size()-1 )
      {
         result = Vector(   atof( items[locate[0]+1].c_str() ),   atof( items[locate[0]+2].c_str() ),   atof( items[locate[0]+3].c_str() )  );
      }
      registerTag( tag, deflt, result, help );
      return result;
   }



   //
   //   Find first occurance of a command line option, compared to a menu of possible values
   //


   const float findMenu( const std::string& tag, const std::vector<float>& deflt, const std::string help = "no help" )
   {
      float result = deflt[0];
      std::vector<size_t> locate = find( tag );
      if( !locate.empty() && locate[0] < items.size()-1 )
      {
         float input = atof( items[locate[0]+1].c_str() );
	 for( size_t i=0;i<deflt.size();i++ )
	 {
            if( input == deflt[i] ){ result = input; }
	 }
      }
      registerTag( tag, deflt, result, help );
      return result;
   }


   const int findMenu( const std::string& tag, const std::vector<int>& deflt, const std::string help = "no help" )
   {
      int result = deflt[0];
      std::vector<size_t> locate = find( tag );
      if( !locate.empty() && locate[0] < items.size()-1 )
      {
         int input = atoi( items[locate[0]+1].c_str() );
	 for( size_t i=0;i<deflt.size();i++ )
	 {
            if( input == deflt[i] ){ result = input;  }
	 }
      }
      registerTag( tag, deflt, result, help );
      return result;
   }

   const std::string findMenu( const std::string& tag, const std::vector<std::string>& deflt, const std::string help = "no help" )
   {
      std::string result = deflt[0];
      std::vector<size_t> locate = find( tag );
      if( !locate.empty() && locate[0] < items.size()-1 )
      {
         std::string input = items[locate[0]+1];
	 for( size_t i=0;i<deflt.size();i++ )
	 {
            if( input == deflt[i] ){ result = input; }
	 }
      }
      registerTag( tag, deflt, result, help );
      return result;
   }


   //
   //   Find every occurance of a command line option
   //



   const std::vector<Vector> findMultiple( const std::string& tag, const Vector deflt, const std::string help = "no help" )
   {
      std::vector<Vector> result;
      std::vector<size_t> locate = find( tag );
      for( size_t i=0;i<locate.size();i++ )
      {
         if( locate[i] < items.size()-1 )
	 {
            result.push_back( Vector(   atof( items[locate[i]+1].c_str() ),   atof( items[locate[i]+2].c_str() ),   atof( items[locate[i]+3].c_str() )  ) );
	 }
	 else
	 {
	    result.push_back( deflt );
	 }
      }
      registerTag( tag, result, help );
      return result;
   }



   const std::vector<float> findMultiple( const std::string& tag, const float deflt, const std::string help = "no help" )
   {
      std::vector<float> result;
      std::vector<size_t> locate = find( tag );
      for( size_t i=0;i<locate.size();i++ )
      {
         if( locate[i] < items.size()-1 )
	 {
	    result.push_back( atof( items[locate[i]+1].c_str() ) );
	 }
         else
         {
            result.push_back( deflt );
         }
      }
      registerTag( tag, result, help );
      return result;
   }

   const std::vector<int> findMultiple( const std::string& tag, const int deflt, const std::string help = "no help" )
   {
      std::vector<int> result;
      std::vector<size_t> locate = find( tag );
      for( size_t i=0;i<locate.size();i++ )
      {
         if( locate[i] < items.size()-1 )
	 {
	    result.push_back( atoi( items[locate[i]+1].c_str() ) );
	 }
         else
         {
            result.push_back( deflt );
         }
      }
      registerTag( tag, result, help );
      return result;
   }

   const std::vector<std::string> findMultiple( const std::string tag, const std::string& deflt, const std::string help = "no help" )
   {
      std::vector<std::string> result;
      std::vector<size_t> locate = find( tag );
      for( size_t i=0;i<locate.size();i++ )
      {
         if( locate[i] < items.size()-1 )
	 {
	    result.push_back(  items[locate[i]+1] );
	 }
         else
         {
            result.push_back( deflt );
         }
      }
      registerTag( tag, result, help );
      return result;
   }

   //
   //   Find first occurance of a command line option with an array of values after the option
   //


   const std::vector<float> findArray( const std::string& tag, const std::vector<float>& deflt, const std::string help = "no help" )
   {
      std::vector<float> result = deflt;
      std::vector<size_t> locate = find( tag );
      if( !locate.empty() && locate[0] < items.size()-deflt.size() )
      {
	 for( size_t i=0;i<deflt.size();i++ )
	 {
            result[i] = atof( items[locate[0]+1+i].c_str() );
	 }
      }
      registerTag( tag, deflt, result, help );
      return result;
   }

   const std::vector<int> findArray( const std::string& tag, const std::vector<int>& deflt, const std::string help = "no help" )
   {
      std::vector<int> result = deflt;
      std::vector<size_t> locate = find( tag );
      if( !locate.empty() && locate[0] < items.size()-deflt.size() )
      {
	 for( size_t i=0;i<deflt.size();i++ )
	 {
            result[i] = atoi( items[locate[0]+1+i].c_str() );
	 }
      }
      registerTag( tag, deflt, result, help );
      return result;
   }

   const std::vector<std::string> findArray( const std::string& tag, const std::vector<std::string>& deflt, const std::string help = "no help" )
   {
      std::vector<std::string> result = deflt;
      std::vector<size_t> locate = find( tag );
      if( !locate.empty() && locate[0] < items.size()-deflt.size() )
      {
	 for( size_t i=0;i<deflt.size();i++ )
	 {
            result[i] = items[locate[0]+1+i];
	 }
      }
      registerTag( tag, deflt, result, help );
      return result;
   }

   const bool findFlag( const std::string& tag, const std::string help = "no help" )
   {
      std::vector<size_t> locate = find( tag );
      const bool result = !locate.empty();
      if( result )
      { 
         registerTag( tag, std::string("FALSE"), std::string("TRUE"), help );
      }
      else
      {
         registerTag( tag, std::string("no default"), std::string("FALSE"), help );
      }
      return result;
   }

   void usage( const std::string& tag ) 
   {
       std::vector<size_t> locate = find( tag );
       if( !locate.empty() )
       {
           registerTag( tag, "Obtain command line help" );
	   std::cout << "========================================\n";
           std::cout << "CmdLine Usage:\n\n";
	   std::cout << "Option\tDefault\tHelp\n";
	   for( size_t i=0;i<cmdLineList.size();i++ )
	   {
              std::cout << cmdLineList[i] << "\t" << cmdLineDefaultsList[i] << "\t" << cmdLineHelpList[i] << std::endl;  
	   }
	   std::cout << "========================================\n";
	   exit(0);
       }
       std::cout << "To obtain help, use the command line option " << tag << std::endl;
   }

   void printFinds() const
   {
      if( cmdLineList.empty() ){ return; }
      std::cout << "========================================\n";
      std::cout << "CmdLine Values:\n\n";
      std::cout << "Option\tValue\n";
      for( size_t i=0;i<cmdLineList.size();i++ )
      {
         std::cout << cmdLineList[i] << "\t" << cmdLineValuesList[i] << std::endl;  
      }
      std::cout << "========================================\n";
   }

   const std::vector<std::string> listFinds() const
   {
      std::vector<std::string> finds;
      if( cmdLineList.empty() ){ return finds; }
      for( size_t i=0;i<cmdLineList.size();i++ )
      {
         std::string fnd = cmdLineList[i] + "   " + cmdLineValuesList[i];
	 finds.push_back(fnd);
      }
      return finds;
   }

   const std::map<std::string, std::string> mapFinds() const
   {
      std::map<std::string, std::string> finds;
      if( cmdLineList.empty() ){ return finds; }
      for( size_t i=0;i<cmdLineList.size();i++ )
      {
         finds[cmdLineList[i]] = cmdLineValuesList[i];
      }
      return finds;
   }




  private:

   std::vector<std::string> items;

   std::vector<std::string> cmdLineList;
   std::vector<std::string> cmdLineDefaultsList;
   std::vector<std::string> cmdLineValuesList;
   std::vector<std::string> cmdLineHelpList;



   const std::vector<size_t> find( const std::string& tag ) const
   {
      std::vector<size_t> finds;
      for( size_t i=0;i<items.size();i++ )
      {
          if( tag == items[i] ){ finds.push_back(i); }
      }
      return finds;
   }


   void registerTag( const std::string& tag, const std::string& help )
   {
      cmdLineList.push_back(tag);
      cmdLineDefaultsList.push_back( "" );
      cmdLineValuesList.push_back( "" ); 
      cmdLineHelpList.push_back(help);
   }


   template <typename T>
   void registerTag( const std::string& tag, const T& def, const T& val, const std::string& help )
   {
      cmdLineList.push_back(tag);
      cmdLineDefaultsList.push_back( tostr(def) );
      if( def != val ){ cmdLineValuesList.push_back( tostr(val) ); }
      else { cmdLineValuesList.push_back( tostr(val) + " (default)" );  }
      cmdLineHelpList.push_back(help);
   }

   void registerTag( const std::string& tag, const Vector& def, const Vector& val, const std::string& help )
   {
      cmdLineList.push_back(tag);
      cmdLineDefaultsList.push_back( tostr( "[ " + tostr(def[0]) + " " + tostr(def[1]) + " " + tostr(def[2]) + " ]" ) );
      if( def != val ){ cmdLineValuesList.push_back( "[ " + tostr(val[0]) + " " + tostr(val[1]) + " " + tostr(val[2]) + " ]" ); }
      else { cmdLineValuesList.push_back(  "[ " + tostr(val[0]) + " " + tostr(val[1]) + " " + tostr(val[2]) + " ] (default)" );  }
      cmdLineHelpList.push_back(help);
   }


   template <typename T>
   void registerTag( const std::string& tag, const std::vector<T>& def, const std::vector<T>& val, const std::string& help )
   {
      cmdLineList.push_back(tag);
      std::string defls = "array: ";
      for( size_t i=0;i<def.size();i++ )
      {
         defls += tostr( def[i] );
	 if( i < def.size()-1 ){ defls += " "; }
      }
      cmdLineDefaultsList.push_back( defls );
      std::string values = "";
      for( size_t i=0;i<val.size();i++ )
      {
         values += tostr( val[i] );
	 if( i < val.size()-1 ){ values += " "; }
      }
      cmdLineValuesList.push_back( values );
      cmdLineHelpList.push_back(help);
   }

   template <typename T>
   void registerTag( const std::string& tag, const std::vector<T>& def, const T& val, const std::string& help )
   {
      cmdLineList.push_back(tag);
      std::string defls = "menu: ";
      for( size_t i=0;i<def.size();i++ )
      {
         defls += tostr( def[i] );
	 if( i < def.size()-1 ){ defls += " "; }
      }
      cmdLineDefaultsList.push_back( defls );
      cmdLineValuesList.push_back( tostr(val) );
      cmdLineHelpList.push_back(help);
   }


   template <typename T>
   void registerTag( const std::string& tag, const std::vector<T>& val, const std::string& help )
   {
      cmdLineList.push_back(tag);
      cmdLineDefaultsList.push_back( "no default" );

      std::string values = "";
      for( size_t i=0;i<val.size();i++ )
      {
         values += tostr(val[i]);
	 if( i < val.size()-1 ){ values += " "; }
      }
      if( val.empty() ){ values = "(no values)";}
      cmdLineValuesList.push_back( values );
      cmdLineHelpList.push_back(help);
   }

   void registerTag( const std::string& tag, const std::vector<Vector>& val, const std::string& help )
   {
      cmdLineList.push_back(tag);
      cmdLineDefaultsList.push_back( "no default" );

      std::string values = "";
      for( size_t i=0;i<val.size();i++ )
      {
         values += "[ " + tostr(val[i][0]) + " " + tostr(val[i][1]) + " " + tostr(val[i][2]) + " ]";
	 if( i < val.size()-1 ){ values += " "; }
      }
      if( val.empty() ){ values = "(no values)";}
      cmdLineValuesList.push_back( values );
      cmdLineHelpList.push_back(help);
   }




   template <typename T> 
   std::string tostr(const T& t) { std::stringstream os; os<<t; return os.str(); }
};


}
#endif
