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
//MPQ template inspired from CSCE221 PA6 (we built a MPQ using these functions so the functions are used from there)
template<typename T = std::pair<int,int>, class Container = std::vector<T>, class Compare = std::less<T>>
class MPQ {
    private:
    Container C;
    Compare comp;

    //private functions used to obtain position of parents or children.
    int left_child(int index) { return 2 * index + 1; }
    int right_child(int index) { return 2 * ( index + 1 ); }
    int parent(int index) { return (index - 1) / 2; }
    //extra function for nodes to check if they're leaf nodes 
    bool is_leaf(int index) {return left_child(index) >= C.size(); }


    //upheap -- internal function
    void upheap(int index){
        if(index == 0){ return; }
        int par = parent(index);
        if(comp(C.at(par), C.at(index))){
            std::swap(C.at(par), C.at(index));
            upheap(par);
        }
    }

    //downheap -- internal function
    void downheap(int index){
        if(is_leaf(index)){ return; }
        int l_child = left_child(index);
        int r_child = right_child(index);

        //handles vector of size > 2
        if(r_child < C.size())
        {
            if(l_child < C.size() &&  comp(C.at(index),C.at(l_child)) && comp(C.at(r_child), C.at(l_child)))
            {
                std::swap(C.at(index), C.at(l_child));
                downheap(l_child);
            }
            else if(r_child < C.size() && comp(C.at(index),C.at(r_child)) && comp(C.at(l_child), C.at(r_child)))
            {
                std::swap(C.at(index), C.at(r_child));
                downheap(r_child);
            }
        }
        else //vector of size 2 or less
        {
            if(l_child < C.size() && comp(C.at(index),C.at(l_child)))
            {
                std::swap(C.at(index), C.at(l_child));
                downheap(l_child);
            }
        }
       
    }

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

    //POP -- swapping the 0 index and last index values, pop_back to remove min/max value, downheap to restore priority queue
    void pop()
    {
        if(empty())
        {
            return;
        }
        std::swap(C[0], C[C.size()-1]);
        C.pop_back();
        downheap(0);
    }

    //UPDATE-KEY : made specifically for dijkstra, reason for custom MPQ and not using stl priority queue

    //TOP -- return the min/max value
    T top()
    {
        if(empty())
        {
            return T();
        }
        return C[0];
    }
    //EMPTY -- retruns boolean depending on size of the container
    bool empty()
    {
        return C.size() == 0;
    }

    //PRINT
    void print_PQ(){
        for(int i = 0; i < C.size(); i++)
        {
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
    std::cout << q.top() << std::endl;
    q.pop();
    std::cout << q.top() << std::endl;
    q.pop();
    std::cout << q.top() << std::endl;
    q.pop();
    std::cout << q.top() << std::endl;
    q.pop();

    return 0;
}