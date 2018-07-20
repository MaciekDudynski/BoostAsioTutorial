#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

class Printer
{
  public:
    Printer( boost::asio::io_context & io )
      : _strand( io ),
        _timer1( io, boost::posix_time::seconds( 1 ) ),
        _timer2( io, boost::posix_time::seconds( 1 ) ),
        _count( 0 )
    {
      _timer1.async_wait( boost::asio::bind_executor( _strand,
        boost::bind( &Printer::print1, this ) ) );
      _timer2.async_wait( boost::asio::bind_executor( _strand,
        boost::bind( &Printer::print2, this ) ) );
    }

    ~Printer()
    {
      std::cout << "Final counter = " << _count << std::endl;
    }

    void print1()
    {
      if( _count < 10 )
      {
        std::cout << "Timer 1: " << _count << std::endl;
        ++_count;

        _timer1.expires_at( _timer1.expires_at() + boost::posix_time::seconds( 1 ) );

        _timer1.async_wait( boost::asio::bind_executor( _strand,
          boost::bind( &Printer::print1, this ) ) );
      }
    }

    void print2()
    {
      if( _count < 10 )
      {
        std::cout << "Timer 2: " << _count << std::endl;
        ++_count;

        _timer2.expires_at( _timer2.expires_at() + boost::posix_time::seconds( 1 ) );

        _timer2.async_wait( boost::asio::bind_executor( _strand,
          boost::bind( &Printer::print2, this ) ) );
      }
    }

  private:
    boost::asio::io_context::strand _strand;
    boost::asio::deadline_timer _timer1;
    boost::asio::deadline_timer _timer2;
    int _count;
};

int main()
{
  boost::asio::io_context io;
  
  Printer p( io );

  boost::thread t( boost::bind( &boost::asio::io_context::run, &io ) );

  io.run();

  t.join();

  return 0;
}
