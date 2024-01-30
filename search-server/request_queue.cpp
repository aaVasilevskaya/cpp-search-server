#include "request_queue.h"

RequestQueue::RequestQueue(const SearchServer& search_server)
    : search_server_(search_server){
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status){
    auto result = search_server_.FindTopDocuments(raw_query, status);
    AddResult(result, result.size());
    return result;
}
std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
    auto result = search_server_.FindTopDocuments(raw_query);
    AddResult(result, result.size());
    return result;
}
int RequestQueue::GetNoResultRequests() const {
    return requests_.size();
}

void RequestQueue::IncreaseTime(){
    if(current_min < min_in_day_)current_min++;
    else current_min = 1;
    if(requests_.front().time_request_min == current_min)requests_.pop_front();        
}
    
void RequestQueue::AddResult(const std::vector<Document>& result, int results_num){
    IncreaseTime();
    if(result.empty())requests_.push_back({results_num, current_min});
}