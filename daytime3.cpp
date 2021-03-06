#include <iostream>
// #include <string>
#include <boost/asio.hpp>

class tcp_server
{
  public:
    tcp_server(boost::asio::io_context& io_context)
      : acceptor_(io_context, tcp::endpoint(tcp::v4(), 13))
    {
      start_accept();
    }
};

int main()
{
  try
  {
    boost::asio::io_context io_context;
    tcp_server server( io_context );

    io_context.run();
  }
  catch( std::exception & e )
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}