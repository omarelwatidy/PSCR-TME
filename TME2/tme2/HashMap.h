#include <vector>
#include <iostream>
#include <forward_list>

template <typename K, typename V>
class HashMap {
public:
    class Entry {
        K key;
        V value;
    public:
        Entry(const K& key, const V& value) : key(key), value(value) {}
        
        const K& getKey() const { return key; }
        V& getValue() { return value; }
        void setValue(const V& val) { value = val; }
    };

private:
    typedef std::vector<std::forward_list<Entry>> buckets_t;
    buckets_t buckets;
    size_t capacity;

public:
    HashMap(size_t cap) : capacity(cap) {
        buckets.resize(cap);
    }

    V* get(const K& key) {
        size_t h = std::hash<K>()(key);
        size_t index = h % buckets.size();

        for (auto& entry : buckets[index]) {
            if (entry.getKey() == key) {
                return &entry.getValue();  
            }
        }
        return nullptr;  
    }

    bool put(const K& key, const V& value) {
        size_t h = std::hash<K>()(key);
        size_t index = h % buckets.size();

        for (auto& entry : buckets[index]) {
            if (entry.getKey() == key) {
                entry.setValue(value);
                return true;  
            }
        }

       
        buckets[index].emplace_front(key, value);
        return false;
    }
};