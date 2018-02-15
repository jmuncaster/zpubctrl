#include "simplezmq.hpp"

#include <vector>

using namespace std;

namespace simplezmq {
  Server::Server() :
    _context(),
    _reply_socket(_context, ZMQ_REP) {

    _reply_socket.bind("tcp://*:5555");
  }

  bool Server::check_for_request(function<string(const string&)> handler) {
    //  Initialize poll set
    vector<zmq::pollitem_t> items {
        { (void*)_reply_socket, 0, ZMQ_POLLIN, 0 },
    };

    if (zmq::poll(items, 0)) {
      if (items[0].revents & ZMQ_POLLIN) {
        zmq::message_t msg;
        _reply_socket.recv(&msg);
        string request_payload(static_cast<char*>(msg.data()), msg.size());
        string reply_payload = handler(request_payload);
        _reply_socket.send(reply_payload.data(), reply_payload.size());
        return true;
      }
    }

    return false;
  }

  Client::Client() :
    _context(),
    _request_socket(_context, ZMQ_REQ) {

    _request_socket.connect("tcp://localhost:5555");
  }

  string Client::request(const string& payload) {
    _request_socket.send(payload.data(), payload.size());
    zmq::message_t msg;
    _request_socket.recv(&msg);
    return string(static_cast<char*>(msg.data()), msg.size());
  }

}

