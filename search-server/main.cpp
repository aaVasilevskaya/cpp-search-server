#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document {
    int id;
    double relevance;
};

class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        int count_words = words.size();
        for(const string& word:words){
            word_to_document_freqs_[word][document_id]+=1./count_words;
        }
        document_count_++;
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        const QueryWords query_words = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query_words);

        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 return lhs.relevance > rhs.relevance;
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

private:
   
    struct QueryWords {
        set <string> minus_words;
        set <string> plus_words;
    };

    int document_count_ = 0;
    map<string,set<int>> word_to_documents_;
    map<string, map<int, double>> word_to_document_freqs_;
    set<string> stop_words_;
    

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    QueryWords ParseQuery(const string& text) const {
        QueryWords query_words;
        for (const string& word : SplitIntoWordsNoStop(text)) {
            if(word[0] == '-')query_words.minus_words.insert(word.substr(1));
            else query_words.plus_words.insert(word);
        }
        return query_words;
    }

    vector<Document> FindAllDocuments(const QueryWords& query_words) const {
        vector<Document> matched_documents;
        map <int, double> document_to_relevance;
        for(const string& word:query_words.plus_words){
            if(word_to_document_freqs_.count(word)){
                int docs_with_word = (word_to_document_freqs_.at(word)).size();
                for(auto& [key, val] : word_to_document_freqs_.at(word)){
                    
                    document_to_relevance[key]+= 
                        log((static_cast<double>(document_count_))/docs_with_word) * val;
                }    
            }
        }
        for(const string& word:query_words.minus_words){
            if(word_to_document_freqs_.count(word)){
                for(auto& [key, val] : word_to_document_freqs_.at(word)){
                    document_to_relevance.erase(key);
                }    
            }
        }
        
        for (const auto& [key, val] :document_to_relevance) {
            matched_documents.push_back({key, val});
        }
        return matched_documents;
    }
};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }

    return search_server;
}

int main() {
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    for (const Document& doc : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << doc.id << ", "
             << "relevance = "s << doc.relevance << " }"s << endl;
    }
}  