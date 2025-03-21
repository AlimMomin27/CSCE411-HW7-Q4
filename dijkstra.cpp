#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <limits>


/*
    Using custom MPQ because STL priority queue doesn't allow for changing keys
*/

//Using templating to allow for dynamic usages for datatypes,
//I.E. std::pair<string, int> where its "Bizzel St." with a weight of 20.
template<typename T = std::pair<int,int>, class Container = std::vector<T>, class Compare = std::less<T>>
class MPQ {
    private:
    Container C;
    Compare comp;

    //private functions used to obtain position of parents or children.
    int left_child(int index) { return 2 * index + 1; }
    int right_child(int index) { return 2 * ( index + 1 ); }
    int parent(int index) { return (index - 1) / 2; }


    //upheap -- internal function
    void upheap(int index){
        if(index == 0){ return; }
        int par = parent(index);
        if(comp(C.at(par), C.at(index))){
            std::swap(C.at(par), C.at(index));
            upheap(par);
        }
    }

    //downheap

    public:
        MPQ() = default;
        MPQ( const MPQ& other ) = default;
        MPQ( MPQ&& other ) = default;
        ~MPQ() = default;
        MPQ& operator=( const MPQ& other ) = default;
        MPQ& operator=( MPQ&& other ) = default;

    //PUSH -- pushing the value into conatiner and calling upheap
    void push(T value)
    {
        C.push_back(value);
        upheap(C.size()-1);
    }

    //POP

    //UPDATE-KEY : made specifically for dijkstra, reason for custom MPQ and not using stl priority queue

    //TOP

    //EMPTY

    //PRINT
    void print_PQ(){
        for(int i = 0; i < C.size(); i++){
            std::cout << " (" << C.at(i) << ") \n";
        }
    }

};

int main(){
    MPQ<int, std::vector<int>, std::greater<int>> q;
    q.push(10);
    q.push(20);
    q.push(1);
    q.push(50);
    q.print_PQ();

    return 0;
}