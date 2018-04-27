#pragma once

#include <map>
#include <memory_resource>
#include <set>
#include <vector>

namespace ares {
  namespace pmr {
    using unsynchronized_pool_resource = std::pmr::unsynchronized_pool_resource;
    using synchronized_pool_resource = std::pmr::synchronized_pool_resource;
    
    template <typename T>
    using vector = std::pmr::vector<T>;

    template <typename Key, typename T, typename Compare = std::less<Key>>
    using multimap = std::pmr::map<Key, T, Compare>;
    
    template <typename Key, typename T, typename Compare = std::less<Key>>
    using multimap = std::pmr::multimap<Key, T, Compare>;

    template <typename Key, typename Compare = std::less<Key>>
    using set = std::pmr::set<Key, Compare>;

    using pool_options = std::pmr::pool_options;
  }
}
