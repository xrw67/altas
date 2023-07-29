#include "bbt/http/client.h"

#include <iostream>

#include "asio.hpp"
#include "bbt/base/fmt.h"
#include "bbt/base/log.h"
#include "bbt/http/response.h"

namespace bbt {
namespace http {

using asio::ip::tcp;

Status Get(const std::string& address, const std::string& port, Response* rep) {
  try {
    asio::io_context io_context;

    // Get a list of endpoints corresponding to the server name.
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve(address, port);

    // Try each endpoint until we successfully establish a connection.
    tcp::socket socket(io_context);
    asio::connect(socket, endpoints);

    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "GET "
                   << "/"
                   << " HTTP/1.0\r\n";
    request_stream << "Host: " << address << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    // Send the request.
    asio::write(socket, request);

    // Read the response status line. The response streambuf will automatically
    // grow to accommodate the entire line. The growth may be limited by passing
    // a maximum size to the streambuf constructor.
    asio::streambuf response;
    asio::read_until(socket, response, "\r\n");

    // Check that response is OK.
    std::istream response_stream(&response);
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
      return bbt::InvalidArgumentError("Invalid response");
    }

    if (status_code != 200) {
      return bbt::InvalidArgumentError(
          bbt::format("Response returned with status code {}", status_code));
    }

    Response reply;
    reply.status = Response::ok;
    // Read the response headers, which are terminated by a blank line.
    asio::read_until(socket, response, "\r\n\r\n");

    // Process the response headers.
    std::string header;
    while (std::getline(response_stream, header) && header != "\r") {
      BBT_LOG(DEBUG, "{}", header);
    }

    // Write whatever content we already have to output.
    if (response.size() > 0) {
      std::cout << &response;
    }

    // Read until EOF, writing data to output as we go.
    asio::error_code error;
    while (asio::read(socket, response, asio::transfer_at_least(1), error)) {
      std::cout << &response;
    }
    if (error != asio::error::eof) {
      throw asio::system_error(error);
    }
  } catch (std::exception& e) {
    return bbt::CancelledError(e.what());
  }
  return bbt::OkStatus();
}

}  // namespace http
}  // namespace bbt