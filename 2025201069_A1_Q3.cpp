#include<iostream>
#include<map>
#include<vector>
using namespace std;

struct VecCmp{
    bool operator()(const vector<int>& a, const vector<int>& b)const{
        if(a[0]!=b[0]){
            return a[0]<b[0];
        }
        else if(a[1]!=b[1]){
            return a[1]>b[1];
        }
        return a[2]<b[2];
    }
};


void display(map<vector<int>, bool, VecCmp> &m){
    map<int, int> heights;
    heights[0]=1;
    int max_h=0;

    for(auto p:m){
        vector<int> v=p.first;
        if(v[2]==0){
        heights[v[1]]+=1;
        }
        else{
            heights[-v[1]]-=1;
            if(heights[-v[1]]==0){
                heights.erase(-v[1]);
            }
        }
        if(heights.rbegin()->first>max_h){
            max_h=heights.rbegin()->first;
            cout<< v[0]<< " "<< max_h<< endl;
        }
        else if(heights.rbegin()->first<max_h)
        {
            max_h=heights.rbegin()->first;
            cout<< v[0]<< " "<< max_h<< endl;
        }

    }
}

int main(){
    vector<vector<int>> queries;
    int q;
    map<vector<int>, bool, VecCmp> pq;
    map<int ,int> heights;
    cin>> q;
    for(int i=0;i<q;i++){
        
        int x;
        cin>> x;
        if(x==1){
            display(pq);
        }
        else if(x==0){
            vector<int> start;
            vector<int> end;
            int x1, y1, h1;
            cin>> x1>> y1>> h1;
            start.push_back(x1);
            start.push_back(h1);
            start.push_back(0);
            end.push_back(y1);
            end.push_back(-h1);
            end.push_back(1);
            pq[start]=true;
            pq[end]=true;
        }
        else{
            cout<< "Invalid flag "<< endl;
            return 0;
        }
    }

}