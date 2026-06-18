#include <iostream>
#include <string>
#include <vector>
#include<tuple>
#include<utility>





using namespace std;
// A, B --> sorted in increaing order by 
vector<tuple<int,int,string>>intersection_of_A_and_B(vector<tuple<int,int,string>>A,vector<tuple<int,int,string>>B){
size_t m=A.size(),n=B.size();

vector<tuple<int,int,string>>result;
size_t i=0,j=0;
while(i<m && j<n){
   
    if(get<0>(A[i])==get<0>(B[j])){
        get<1>(A[i])=min(get<1>(A[i]),get<1>(B[j]));
        result.push_back(A[i]);
        i++;j++;
    }
    else if(get<0>(A[i])<get<0>(B[j])){
        i++;//increase my smaller element's pointer
    }
    else j++;
    

}
return result;

}


// it just return the final result of the intersection in the required format
vector<pair<string,int>>Answer_Intersection(vector<tuple<int,int,string>>Intersection_result){
vector<pair<string,int>>Answer;

for(auto &it:Intersection_result){
    Answer.push_back(make_pair(get<2>(it),get<1>(it)));//returns <file_path,freq>
}
return Answer;

}


vector<pair<string,int>>intersection(vector<vector<tuple<int,int,string>>>All_Word_Search_Results){
    if(All_Word_Search_Results.empty())
    return {};
    // u can still improve here by starting intersection by smallest size vector ,
    // but for now i am skipping that part 
    
    vector<tuple<int,int,string>>Result=All_Word_Search_Results[0];
    for(size_t i=1;i<All_Word_Search_Results.size();i++){
        Result=intersection_of_A_and_B(Result,All_Word_Search_Results[i]);
        if(Result.empty()) return {};
    }

    return Answer_Intersection(Result);//storing the final result 
    
}




