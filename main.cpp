//Andrea Iriarte
//COP 3530
//Project 2


#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

struct Page
{
    string name;
    int outDegree;
    float rank;
};

class Internet
{
    private:
    map<string, vector<string>> internet; // adjacency list in order of to, from
    map<string, Page> pages; //holds unique domains
    int numOfPages; 
    int powerIterations;
    map<string, float> ranks;
    
    public:
    Internet();
    Internet(int powerIterations);
    void AddEdge(string from, string to);
    void InitializeRanks();
    void CalculatePageRank();
    void SetRanks();
    void Print();
};

Internet::Internet() //default constructor
{
    numOfPages = 0;
    powerIterations = 1;
}

Internet::Internet(int powerIterations) //constructor
{
    numOfPages = 0;
    this->powerIterations = powerIterations;
}

void Internet::AddEdge(string from, string to)
{
    //if it is the first instance of either domain,
    //create a Page object and add it to the map
    
    if (pages.find(from) == pages.end()) 
    {
        Page p1;
        p1.name = from;
        p1.outDegree = 0;
        pages[from] = p1;
        numOfPages++;
    }
    
    if (pages.find(to) == pages.end())
    {
        Page p2;
        p2.name = to;
        p2.outDegree = 0;
        pages[to] = p2;
        numOfPages++;
    }
    
    pages[from].outDegree++; 
    
    internet[to].push_back(from);
    
    if (internet.find(from) == internet.end())
    {
        internet[from] = {};
    }
}

void Internet::InitializeRanks()
{
    //this is essentially the first power iteration. sets all ranks to 1/numOfPages
    
    auto iter = pages.begin();
    
    while (iter != pages.end())
    {
        iter->second.rank = 1.0/(float)numOfPages;
        
        ranks[iter->first] = iter->second.rank;
        
        iter++;
     }
}

void Internet::SetRanks()
{
    //the page object stores the rank at time t + 1, while the ranks map stores
    //the ranks at time t. in order to move on, the ranks map needs to update
    
    auto iter = pages.begin();
    
    while (iter != pages.end())
    {
        ranks[iter->first] = iter->second.rank;
        iter++;
    }
}

void Internet::CalculatePageRank()
{
    InitializeRanks();
    
    for (int i = 0; i < powerIterations - 1; i++)
    {
        auto iter = internet.begin();
        
        //this takes the to node of each node and calculates the ranks
        //by cyclying through all of the from nodes and adding the weight
        //(1 / outDegree) * rank[outNode] for each node
        
        while (iter != internet.end())
        {
            float sum = 0.0;
            
            for (string s : iter->second)
            {
                sum += ((1 / float(pages[s].outDegree)) * ranks[s]); // M * r(i)
            }
            
            pages[iter->first].rank = sum;
            
            iter++;
        }
        
        SetRanks();
    }
}

void Internet::Print()
{
    //calls CalculatePageRank and then cycles through
    //each domain in pages and prints name and rank
    
    CalculatePageRank();
    
    auto iter = pages.begin();
    
    while (iter != pages.end())
    {
        cout << iter->first << " " << std::fixed << setprecision(2) << iter->second.rank << "\n";

        iter++;
    }
}

int main()
{
    //collects data from keyboard and then uses a for loop
    //to add the desired number of edges. calls print.
    
    int powerIterations;
    int numOfInputs;
    string from;
    string to;
    
    cin >> numOfInputs;
    cin >> powerIterations;
    
    Internet internet(powerIterations);
    
    for (int i = 0; i < numOfInputs; i++)
    {
        cin >> from;
        cin >> to;
        
        internet.AddEdge(from, to);
    }
    
    internet.Print();
}
