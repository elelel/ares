#pragma once

#include <boost/container/pmr/pool_options.hpp>
#include <boost/container/pmr/synchronized_pool_resource.hpp>
#include <boost/container/pmr/unsynchronized_pool_resource.hpp>
#include <boost/container/pmr/vector.hpp>
#include <boost/container/pmr/map.hpp>
#include <boost/container/pmr/set.hpp>

namespace ares {
  namespace pmr {
    using pool_options = boost::container::pmr::pool_options;
    
    using unsynchronized_pool_resource = boost::container::pmr::unsynchronized_pool_resource;
    using synchronized_pool_resource = boost::container::pmr::synchronized_pool_resource;
    
    template <typename T>
    using vector = boost::container::pmr::vector<T>;

    template <typename Key, typename T, typename Compare = std::less<Key>>
    using map = boost::container::pmr::map<Key, T, Compare>;
 
    template <typename Key, typename T, typename Compare = std::less<Key>>
    using multimap = boost::container::pmr::multimap<Key, T, Compare>;

    template <typename Key, typename Compare = std::less<Key>>
    using set = boost::container::pmr::set<Key, Compare>;
  }
}
