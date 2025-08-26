#include<iostream>
#include<vector>
using namespace std;
 
struct TrieNode{
    TrieNode* arr[26];
    bool end;
    TrieNode(){
        for(int i=0;i<26;i++){
            arr[i]=NULL;  
        }
        end=false;
    }
};

void insert_Trie(TrieNode *root, string s){
    TrieNode* curr=root;
    for(auto ch:s){
        if(curr->arr[ch-'a']==NULL){
            curr->arr[ch-'a']=new TrieNode();
        }
        curr=curr->arr[ch-'a'];
    }
    curr->end=true;
    return;
}

bool search_Trie(TrieNode* root, string s){
    TrieNode* curr=root;
    for(auto ch: s){
        if(curr->arr[ch-'a']==NULL){
            return false;
        }
        curr=curr->arr[ch-'a'];
    }
    return curr->end;
}

void auto_complete(TrieNode* curr, string s, vector<string> &ans){
    if(curr->end){
        ans.push_back(s);

    }
    for(int i=0;i<26;i++){
        if(curr->arr[i]==NULL){
            continue;
        }
        auto_complete(curr->arr[i], s+char(97+i), ans);
    }
}

bool starts_with(TrieNode* root, string s, vector<string> &ans){
    TrieNode* curr=root;
    for(auto ch: s){
        if(curr->arr[ch-'a']==NULL){
            return false;
        }
        curr=curr->arr[ch-'a'];
    }
    auto_complete(curr, s, ans);
    return true;
    
}

vector<int> find_levenshtein_distance(string s1, string s2, vector<int> &v){
    int index=s2.size()-1;
    int n=s1.size();// x-axis string
    vector<int> ans;
    ans.push_back(v[0]+1);
    for(int i=1;i<=n;i++){
        if(s2[index]==s1[i-1]){
            ans.push_back(v[i-1]);
        }
        else{
            int x1=v[i-1]+1;
            int x2=v[i]+1;
            int x3=ans[i-1]+1;
            ans.push_back(min(x1, min(x2, x3)));
        }
    }
    return ans;
}

void autocomplete(string source, string dest, vector<string> &ans, TrieNode* node, vector<int> v){
    if(v.back()<=3 && node->end==true){
        ans.push_back(dest);
    }
    for(int i=0;i<26;i++){
        if(node->arr[i]==NULL){
            continue;
        }
        vector<int> temp=find_levenshtein_distance(source, string(1,97+i), v);
        string temp1=dest+char(97+i);
        
        autocomplete(source, dest+char(97+i), ans, node->arr[i], temp);
        
    }
}

int main(){
    int n,q;
    cin>>n>> q;
    TrieNode* root=new TrieNode();
    for(int i=0;i<n;i++){
        string s;
        cin>> s;
        
        insert_Trie(root, s);
    }

    for(int j=0;j<q;j++){
        int flag;
        cin>> flag;
        if(flag==1){
            string b;
            cin>> b;
            cout<< search_Trie(root, b)<< endl;
        }
        else if(flag==2){
            vector<string> ans;
            string c;
            cin>> c;
            starts_with(root, c, ans);
            cout<< ans.size()<< endl;
            for(auto z:ans){
                cout<<z<< endl;
            }
        }
        else if(flag==3){
            vector<string> ans;
            string d;
            cin>> d;
            vector<int> v;
            for(int i1=0;i1<=d.size();i1++){
                v.push_back(i1);
            }
            autocomplete(d, "", ans, root, v);
            cout<< ans.size()<< endl;
            for(auto z2: ans){
                cout<< z2<< endl;
            }
        }
    }

    // vector<string> ans;
    // string s;
    // cin>> s;
    // bool x=starts_with(root, s, ans);
    // if(x){
    //     for(auto s1: ans){
    //         cout<< s1<< endl;
    //     }
    // }
    // else{
    //     cout<<"no words matching the prefix :"<< s<< endl;
    // }
    // vector<string> ans1;
    // string source;
    // cin>> source;
    // vector<int> v;
    // for(int i=0;i<=source.size();i++){
    //     v.push_back(i);
    // }
    // autocomplete(source, "", ans1, root, v);
    // printf("strings with levenshtein distance<=3: \n");
    // for(auto t: ans1){
    //     cout<< t<< endl;
    // }
    return 0;
    
}
