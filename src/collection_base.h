#ifndef COLLECTION_BASE_H
#define COLLECTION_BASE_H

#include <boost/date_time.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

template <class T> class CollectionBase {
public:
  boost::uuids::uuid id;
  boost::gregorian::date created_at;
  boost::gregorian::date updated_at;
  virtual const std::string &to_json() = 0;
  virtual T from_json(const std::string &j) = 0;
};

#endif