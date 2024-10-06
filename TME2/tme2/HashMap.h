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
    size_t count;

public:
    HashMap(size_t cap,size_t count) :count(count),capacity(cap) {
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
        count++;
        return false;
    }
    size_t countt(){return count;}
    std::vector<std::pair<K, V>> extract(){
    	std::vector<std::pair<K,V>> vec;
    		vec.reserve(capacity);
    		for (auto& list: buckets){
    			for (auto& elt: list){
    			
    				vec.emplace_back(elt.getKey(), elt.getValue());
    				
    			}
    			
    		}
    		return vec;
    }
    std::vector<std::pair<K, V>> copyEntriesToVector() {
            std::vector<std::pair<K, V>> vec;
            vec.reserve(count); 
            for (auto it = begin(); it != end(); ++it) {
                vec.emplace_back(it->getKey(), it->getValue()); 
            }
            return vec;
        }
    class iterator {
        private:
            typename buckets_t::iterator vit; 
             buckets_t & buckets;  
            typename std::forward_list<Entry>::iterator lit; 
        public:
        
               iterator(typename buckets_t::iterator v,buckets_t& b):vit(v)
			   	   	   ,buckets(b){
            	   if (v != buckets.end()) {
            	   
            		   lit = v->begin();
            	   
               
               }
            	   while (vit != buckets.end() && vit->empty()){
            	                                                     
            		   ++vit;
            	    }
            	                                                      
            	   if (vit != buckets.end()) {
            	          lit = vit->begin(); 
            	              }  
               }
               iterator& operator++() {
                           ++lit; 
                           if (lit == vit->end()) {
                               ++vit; 
                               
                                   while (vit != buckets.end() && vit->empty()){
                                                   ++vit;
                               }
                                                   if (vit != buckets.end()) {
                                                       lit = vit->begin(); 
                                                   }
                                               
                               
                           }
                           return *this;
                       }
               bool operator!=(const iterator& other) const {
                           return vit != other.vit || lit != other.lit;
                       }
               Entry& operator*() {
                           return *lit;
                       }
               Entry* operator->() {
                           return &(*lit); 
                       }
               
    }; 
    iterator begin() {
          return iterator(buckets.begin(), buckets);
      }

      iterator end() {
          return iterator(buckets.end(), buckets);
      }
};