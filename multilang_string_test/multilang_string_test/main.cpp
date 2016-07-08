

#include <iostream>
#include <chrono>
#include "multilang_string.hpp"
#include "old_multilang_string.hpp"

using namespace std::chrono;

template <class func>
void run(size_t count, func &&fn)
{
  for(size_t i = 0; i < count; ++i)
  {
    fn();
  }
}

template <class T>
long AddStringTest(const char * ch)
{
    T str;
    auto begin = high_resolution_clock::now();
    run(1000000, [&str, ch]()
        {
          for(int8_t i = 0; i < StringUtf8Multilang::kMaxSupportedLanguages; ++i)
          {
            str.AddString(i, ch);
          }
        });
    auto end = high_resolution_clock::now();
    return duration_cast <milliseconds>( end - begin ).count();
}

template <class T>
long ReadString64()
{
  {
    T str;

    for(int8_t i = 0; i < StringUtf8Multilang::kMaxSupportedLanguages; ++i)
    {
      str.AddString(i, "short_str");
    }

    string result;
    auto begin = high_resolution_clock::now();
    run(1000000, [&str, &result]()
        {
          for(int8_t i = 0; i < StringUtf8Multilang::kMaxSupportedLanguages; ++i)
          {
            str.GetString(i, result);
          }
        });
    auto end = high_resolution_clock::now();
    return duration_cast <milliseconds>( end - begin ).count();

  }
}

template <class T>
long ReadString3()
{
  {
    T str;
    auto first = StringUtf8Multilang::kMaxSupportedLanguages / 2;
    for(int8_t i = first; i < first + 3; ++i)
    {
      str.AddString(i, "short_str");
    }

    string result;
    auto begin = high_resolution_clock::now();
    run(1000000, [&str, &result]()
        {
          for(int8_t i = 0; i < StringUtf8Multilang::kMaxSupportedLanguages; ++i)
          {
            str.GetString(i, result);
          }
        });
    auto end = high_resolution_clock::now();
    return duration_cast <milliseconds>( end - begin ).count();
    
  }
}

template <class T>
long ForEach64()
{
    T str;
    
    for(int8_t i = 0; i < StringUtf8Multilang::kMaxSupportedLanguages; ++i)
    {
      str.AddString(i, "short_str");
    }
    
    auto begin = high_resolution_clock::now();
    run(1000000, [&str]()
        {
          for(int8_t i = 0; i < StringUtf8Multilang::kMaxSupportedLanguages; ++i)
          {
            str.ForEach([](int8_t code, const string& str) -> bool
                        {
                          if(!str.empty())
                            return str.size();
                          
                          return true;
                        });
          }
        });
    auto end = high_resolution_clock::now();
    return duration_cast <milliseconds>( end - begin ).count();
}

template <class T>
long ForEach3()
{
  T str;
  auto first = StringUtf8Multilang::kMaxSupportedLanguages / 2;
  for(int8_t i = first; i < first + 3; ++i)
  {
    str.AddString(i, "short_str");
  }
  
  auto begin = high_resolution_clock::now();
  run(1000000, [&str]()
      {
        for(int8_t i = 0; i < StringUtf8Multilang::kMaxSupportedLanguages; ++i)
        {
          str.ForEach([](int8_t code, const string& str) -> bool
                      {
                        if(!str.empty())
                          return str.size();
                        
                        return true;
                      });
        }
      });
  auto end = high_resolution_clock::now();
  return duration_cast <milliseconds>( end - begin ).count();
}

int main(int argc, const char * argv[])
{
  cout << "add short string test " << AddStringTest <StringUtf8Multilang> ("short str") << endl;
  cout << "add short string test OLD " << AddStringTest <OldStringUtf8Multilang> ("short str") << endl;
  
  const char * long_str = "long_str                                                     \
                            second line                                                      \
                            thrid line                                                       ";
  
  cout << "add long string test " << AddStringTest <StringUtf8Multilang> (long_str) << endl;
  cout << "add long string test OLD excluded due to a long execution time (more than 10 minutes)" << endl;
  //cout << "add long string test OLD" << AddStringTest <OldStringUtf8Multilang> (long_str) << endl;
  
  cout << "read strings 3 lang test " << ReadString3 <StringUtf8Multilang> () << endl;
  cout << "read strings 3 lang test OLD " << ReadString3 <OldStringUtf8Multilang> () << endl;
  
  cout << "read strings 64 lang test " << ReadString64 <StringUtf8Multilang> () << endl;
  cout << "read strings 64 lang test OLD " <<  ReadString64 <OldStringUtf8Multilang> () << endl;

  cout << "for_each 3 lang test " << ForEach3 <StringUtf8Multilang> () << endl;
  cout << "for_each 3 lang test OLD " << ForEach3 <OldStringUtf8Multilang> () << endl;
  
  cout << "for_each 64 lang test " << ForEach64 <StringUtf8Multilang> () << endl;
  cout << "for_each 64 lang test OLD " << ForEach64 <OldStringUtf8Multilang> () << endl;
  
  return 0;
}
