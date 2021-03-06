#pragma once

#include "constants.hpp"
#include <functional>
#include <memory>
#include <string>

namespace zpubctrl {

  // Server publishes data stream and accepts synchronous control requests.
  class Server {
    public:
      Server(int pub_port = default_data_port, int ctrl_port = default_ctrl_port);
      ~Server();

      // Polls ctrl socket for request. If there is a request, respond with reply.
      // @param request_handler: Callback that is called if and only if there is a request.
      // @returns true if a request was handled.
      typedef std::function<std::string(const std::string&)> request_callback_t;
      bool wait_for_request(int timeout_ms = forever, request_callback_t request_handler = nullptr);

      // Publish a message to the pub socket
      void publish_data(const std::string& payload);

    private:
      struct Detail;
      std::unique_ptr<Detail> _detail;
  };
}

