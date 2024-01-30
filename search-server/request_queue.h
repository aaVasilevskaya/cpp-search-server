#pragma once

#include "search_server.h"
#include <deque>
#include <vector>
class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server);
    
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
        auto result = search_server_.FindTopDocuments(raw_query, document_predicate);
        AddResult(result, result.size());
        return result;
        
    }
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);
    
    std::vector<Document> AddFindRequest(const std::string& raw_query);
    
    int GetNoResultRequests() const;

    private:
    
    struct QueryResult {
       int results;
       int time_request_min;
    };
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    int current_min = 0;
    const SearchServer& search_server_;
    
    void IncreaseTime();
    
     void AddResult(const std::vector<Document>& result, int results_num);
}; 