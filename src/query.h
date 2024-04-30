#ifndef QUERY_H
#define QUERY_H

#include <iostream>

class Query {
public:
  virtual const std::string &to_json() = 0;
};

#endif