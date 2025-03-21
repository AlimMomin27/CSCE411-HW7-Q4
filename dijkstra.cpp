#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <limits>
#include <string>


//implemented customer comparator for pair elements, keeping std::less for singular datatype examples
template <typename T, typename V>
struct custom_comp{
    bool operator()( const std::pair<T,V>& A, const std::pair<T,V>& B ){
        return A.second > B.second;
    }
};


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
    // returns true, if the queue item is found and successfully updated
    bool updateKey(T queue_item, int weight){
        for(int i = 0; i < C.size(); i++)
        {
            if(C.at(i).first == queue_item.first)
            {
                C[i].second = weight;
                //checking the children nodes because updating key will corrupt the MPQ structure
                if(left_child(i) < C.size() && ((C.at(left_child(i)).second < weight) || (right_child(i) < C.size() && C.at(right_child(i)).second < weight  )))
                {
                    downheap(i);
                }
                //checks parents as well, if the key is less than the parent then upheap to correct position.
                else if(weight < C.at(parent(i)).second)
                {
                    upheap(i);
                }
            }
        }
        return false;
    }

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
            std::cout << " (" << C.at(i).first << ", " << C.at(i).second << ") \n";
        }
        std::cout << std::endl;
    }

};

int main(){
    MPQ<std::pair<std::string, int>, std::vector<std::pair<std::string,int>>, custom_comp<std::string, int>> q;
    q.push(std::pair("A", 10));
    q.push(std::pair("B", 20));
    q.push(std::pair("C", 1));
    q.push(std::pair("D", 50));
    q.print_PQ();
    q.updateKey({"B", 0}, 2);
    q.print_PQ();

    return 0;
}
