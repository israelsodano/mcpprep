#ifndef REPOSITORY_H
#define REPOSITORY_H
#include "collection_base.h"
#include "query.h"

#include <boost/optional/optional.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/pool.hpp>
#include <optional>
#include <simdjson.h>
#include <type_traits>

namespace repository {
template <typename T,
          typename = std::enable_if_t<std::is_base_of_v<CollectionBase<T>, T>>>
class Repository {
private:
  mongocxx::client &_cli;
  mongocxx::v_noabi::database _db;
  mongocxx::v_noabi::collection _cl;

public:
  Repository(mongocxx::pool::entry &e, const std::string db,
             const std::string cl)
      : _cli(*e), _db(_cli.database(db)), _cl(_db.collection(cl)) {}
  ~Repository() = default;

  template <typename Q,
            typename = std::enable_if_t<std::is_base_of_v<Query, Q>>>
  std::shared_ptr<T> get(Q &q) {

    boost::optional<bsoncxx::document::value> r =
        _cl.find_one(get_value_of_query(q));
    if (!r.has_value()) {
      return nullptr;
    }
    std::string j = bsoncxx::to_json(r.value());
    T cr = get_collection_of_value(j);
    return std::make_shared<T>(cr);
  }

  template <typename Q,
            typename = std::enable_if_t<std::is_base_of_v<Query, Q>>>
  std::vector<T> get_all(Q &q) {
    std::vector<T> r;
    mongocxx::v_noabi::cursor c = _cl.find(get_value_of_query(q));
    for (auto &&i : c) {
      std::string j = bsoncxx::to_json(i);
      auto cl = get_collection_of_value(j);
      r.push_back(cl);
    }
    return r;
  }
  inline void insert(T &cll) { _cl.insert_one(get_value_of_collection(cll)); }
  void insert(std::vector<T> clls) {
    std::vector<bsoncxx::v_noabi::document::value> bds;
    for (auto &&i : clls) {
      bds.push_back(get_value_of_collection(i));
    }
    _cl.insert_many(bds.begin(), bds.end());
  }
  template <typename Q,
            typename = std::enable_if_t<std::is_base_of_v<Query, Q>>>
  void replace(Q &q, T &cll) {
    _cl.replace_one(get_value_of_query(q), get_value_of_collection(cll),
                    mongocxx::v_noabi::options::replace().upsert(true));
  }
  template <typename Q,
            typename = std::enable_if_t<std::is_base_of_v<Query, Q>>>
  void remove(Q &q) {
    _cl._delete_one(nullptr, get_value_of_query(q));
  }

private:
  template <typename Q>
  bsoncxx::v_noabi::document::value get_value_of_query(Q &q) {
    auto pq = &q;
    Query *bq = dynamic_cast<Query *>(pq);
    return bsoncxx::from_json(bq->to_json());
  }

  bsoncxx::v_noabi::document::value get_value_of_collection(T &cll) {
    auto pcll = &cll;
    CollectionBase<T> *c = dynamic_cast<CollectionBase<T> *>(pcll);
    return bsoncxx::from_json(c->to_json());
  }

  T get_collection_of_value(const std::string &j) {
    T cr;
    CollectionBase<T> *bcr = dynamic_cast<CollectionBase<T> *>(&cr);
    cr = bcr->from_json(j);
    return cr;
  }
};
} // namespace repository
#endif