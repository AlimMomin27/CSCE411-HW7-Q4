#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <limits>


/*
    Using custom MPQ because STL priority queue doesn't allow for changing keys
*/

template<typename T = std::pair<int,int>, class Container = std::vector<T>, class Compare = std::less<T>>
class MPQ {
    private:
    Container C;
    Compare comp;

    //upheap

    //downheap

    public:
        MPQ() = default;
        MPQ( const MPQ& other ) = default;
        MPQ( MPQ&& other ) = default;
        ~MPQ() = default;
        MPQ& operator=( const MPQ& other ) = default;
        MPQ& operator=( MPQ&& other ) = default;

    //PUSH

    //POP

    //UPDATE-KEY : made specifically for dijkstra, reason for custom MPQ and not using stl priority queue

    //TOP

    //EMPTY

};