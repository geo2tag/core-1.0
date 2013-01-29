#include "PerformanceCounter.h"
#include "servicelogger.h"

PerformanceCounter::PerformanceCounter(const std::string & text):m_text(text)
{
  gettimeofday(&m_a,0);
}


PerformanceCounter::~PerformanceCounter()
{
  timeval b;
  gettimeofday(&b,0);
  long int j = (b.tv_usec-m_a.tv_usec>0?b.tv_usec-m_a.tv_usec:1000000+b.tv_usec-m_a.tv_usec);
  DEBUG() << "profiling: time_of_" << m_text.c_str() << "," << b.tv_sec-m_a.tv_sec <<","<<j;
}
