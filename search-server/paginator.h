#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

template <typename It>
class IteratorRange{
    public:
    IteratorRange() = default;
    IteratorRange(It begin, It end){
        begin_ = begin;
        end_ = end;
    }
    
    It begin()const{
        return begin_;
    }
    
    It end()const{
        return end_;
    }

    It size(){
        return end_-begin_;
    }

    private:
    It begin_;
    It end_;
};

template <typename It>
class Paginator{
    public:
    Paginator(It container_begin, It container_end, size_t page_size){
        container_range_ = IteratorRange<It>(container_begin, container_end);
        page_size_ = page_size;
        SplitOnPages();        
    }
    auto begin()const{
        return pages_.begin();
    }

    auto end()const{
        return pages_.end();
    }

    private:
    IteratorRange<It> container_range_;
    IteratorRange<It> pages_range_;
    size_t page_size_;
    std::vector<IteratorRange<It>> pages_; 

    void SplitOnPages(){
        It current = container_range_.begin();
        It end = container_range_.end();       
        while (current != end) {
            It next = current;
            advance(next, std::min(page_size_, static_cast<size_t>(distance(current, end))));
            pages_.emplace_back(current, next);
            current = next;
        }
    }
};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator<typename Container::const_iterator>(begin(c), end(c), page_size);
}

template <typename It>
std::ostream& operator<<(std::ostream& out, const IteratorRange<It>& iter){
        
        for (auto it = iter.begin(); it != iter.end(); ++it) {
            out << *it;
        }

    return out;
}