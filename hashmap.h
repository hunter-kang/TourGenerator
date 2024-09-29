
//  main.cpp
//  proj4_HashMap
//
//  Created by Hunter Kang on 3/10/24.


#ifndef HASHMAP_H
#define HASHMAP_H

#include <vector>
#include <list>
#include <string> // needed for the hash function

template <typename T> class HashMap
{
    public:
        HashMap(double max_load = 0.75); // constructor
        ~HashMap(); // destructor; deletes all of the items in the hashmap
        int size() const; // return the number of associations in the hashmap
        // The insert method associates one item (key) with another (value).
        // If no association currently exists with that key, this method inserts
        // a new association into the hashmap with that key/value pair. If there is
        // already an association with that key in the hashmap, then the item
        // associated with that key is replaced by the second parameter (value).
        // Thus, the hashmap must contain no duplicate keys.
        void insert(const std::string& key, const T& value);
        // Defines the bracket operator for HashMap, so you can use your map like this: // your_map["david"] = 2.99;
        // If the key does not exist in the hashmap, this will create a new entry in // the hashmap and map it to the default value of type T (0 for builtin types). // It returns a reference to the newly created value in the map.
        T& operator[](const std::string& key);
        // If no association exists with the given key, return nullptr; otherwise,
        // return a pointer to the value associated with that key. This pointer can be // used to examine that value within the map.
        const T* find(const std::string& key) const;
        // If no association exists with the given key, return nullptr; otherwise,
        // return a pointer to the value associated with that key. This pointer can be // used to examine that value or modify it directly within the map.
        T* find(const std::string& key) {
            const auto& hm = *this;
            return const_cast<T*>(hm.find(key));
        }
    
    private:
        double m_max_Load;
        int m_size;
        std::vector<std::list<std::pair<std::string, T>>> m_map;
};

template <typename T>
HashMap<T>::HashMap(double max_load)
: m_max_Load(max_load), m_map(10), m_size(0)
{}

// no need for destructor as the list std::lists destructor will be called
template <typename T>
HashMap<T>::~HashMap()
{}

template <typename T>
int HashMap<T>::size() const
{
    return m_size;
}

template <typename T>
void HashMap<T>::insert(const std::string& key, const T& value)
{
    // Checks if max load is exceeded current load
    double curLoadFactor = static_cast<double>(this->size() + 1) / m_map.size();
    if (curLoadFactor > m_max_Load) {
        // Rehash here
        size_t newMapSize = m_map.size()*2;
        std::vector<std::list<std::pair<std::string, T>>> newMap(newMapSize);
        for (int i = 0; i != m_map.size(); i++) {
            for (typename std::list<std::pair<std::string, T>>::iterator p = m_map[i].begin(); p != m_map[i].end(); ) {
                size_t newHash = std::hash<std::string>()(p->first);
                size_t newIndex = newHash % newMap.size();
                
                // Redirect Pointers
                newMap[newIndex].splice(newMap[newIndex].end(), m_map[i], p++);
            }
        }
        m_map.swap(newMap);
    }
    
    // insert after checking if needed to rehash or not
    size_t hStr = std::hash<std::string>()(key);
    size_t index = hStr % m_map.size();
    for (typename std::list<std::pair<std::string, T>>::iterator p = m_map[index].begin(); p != m_map[index].end(); p++)
    {
        if (p->first == key)
        {
            p->second = value;
            return;
        }
    }
    
    m_map[index].push_back(std::make_pair(key, value));
    m_size++;
}

template <typename T>
T& HashMap<T>::operator[](const std::string& key)
{
     if (this->find(key) == nullptr) {
        this->insert(key, T());
    }
    return *this->find(key);
}

template <typename T>
const T* HashMap<T>::find(const std::string& key) const
{
    size_t hStr = std::hash<std::string>()(key);
    size_t index = hStr % m_map.size();
    for (typename std::list<std::pair<std::string, T>>::const_iterator p = m_map[index].begin(); p != m_map[index].end(); p++)
    {
        if (p->first == key)
        {
            return &(p->second);
        }
    }
    return nullptr;
}

#endif // HASHMAP_H
