#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <limits>
#include <string>


//implemented customer comparator for pair elements, (keeping std::less in the MPQ template for singular datatype examples )
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
    void upheap(int index)
    {
        if(index == 0){ return; }
        int par = parent(index);
        if(comp(C.at(par), C.at(index))){
            std::swap(C.at(par), C.at(index));
            upheap(par);
        }
    }

    //downheap -- internal function
    void downheap(int index)
    {
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

    //UPDATE-KEY -- made specifically for dijkstra, reason for custom MPQ and not using stl priority queue
    // returns true, if the queue item is found and successfully updated
    bool updateKey(T queue_item, int weight)
    {
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
                return true;
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

//Dijkstra Class
template<typename T>
class Dijkstra {
    //using 3 of the same data_type to have meaningful difference when specifying datatypes for structures.
    using edge = std::pair<T, int>;
    using queue_item = std::pair<T, int>;
    using parent_and_weight = std::pair<T, int>;
    // using a unordered_map for adj_list, contains a pair that has the node being pointed to and its edge weight
    using adjacency_list = std::vector<edge>;

    MPQ<queue_item, std::vector<queue_item>, custom_comp<T, int>> Q;
    std::unordered_map<T, parent_and_weight> S; // tracks a node's parent includes the nodes total weight

    public:
    //G is a 1D vector, index represents the vertex ID and the elemnt (value in the vector) represents the starting weight (inf or 0)
    std::unordered_map<T, parent_and_weight> dijkstra(std::vector<T> G, std::unordered_map<T, adjacency_list> adj_list, T source_node)
    {
        // int parent  = source_node;
        //enqueue minPQ with source vertex with weight 0 and other vertexs weight inf weight
        for(int i = 0; i < G.size(); i++)
        {
            if(G[i] == source_node)
            {
                Q.push(std::pair<T,int>(G[i], 0));
            }
            else
            {
                Q.push(std::pair<T,int>(G[i], INT_MAX));
            }
        }

        while( !Q.empty() )
        {
            queue_item u = Q.top();
            std::cout << " REMOVED " << u.first <<  "FROM Q\n";
            Q.pop(); //remove item from top of the queue
            

            for( edge v : adj_list[u.first]){
                //Call RELAX --> update Key for my implementation
                bool updated = Q.updateKey(v, (u.second + v.second));
                std::cout << "UPDATE: " << updated << " FOR " << v.first << std::endl;
                if(updated){
                    S[v.first] = parent_and_weight(u.first, {u.second + v.second});
                }
            }
            Q.print_PQ();

        }
        return S;
    }
};


int main(){
    /* TESTING MPQ
    MPQ<std::pair<std::string, int>, std::vector<std::pair<std::string,int>>, custom_comp<std::string, int>> q;
    q.push(std::pair("A", 10));
    q.push(std::pair("B", 20));
    q.push(std::pair("C", 1));
    q.push(std::pair("D", 50));
    q.print_PQ();
    q.updateKey({"B", 0}, 2);
    q.print_PQ();
    */
    //TESTING DIJKSTRA
    Dijkstra<std::string> Djk;
    std::vector<std::string> streets = {"University Drive", "Ross St", "Houston St", "George Bush Drive", "Wellborn St"};
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> adj;
    adj["University Drive"] = {{"Houston St", 2}, {"Wellborn St", 5}};
    adj["Houston St"] = {{"Ross St", 7}};
    adj["Ross St"] = {};
    adj["Wellborn St"] = {{"Ross St", 1}, {"George Bush Drive", 10}};
    adj["George Bush Drive"] = {};

    std::unordered_map<std::string, std::pair<std::string, int>> returned_set = Djk.dijkstra(streets, adj, "University Drive");

    for(std::pair<std::string, std::pair<std::string, int>> pr : returned_set)
    {
        std::cout << "(" << pr.first << ", P[" << pr.second.first << "], " << pr.second.second << ")\n";
    }

    return 0;
}
