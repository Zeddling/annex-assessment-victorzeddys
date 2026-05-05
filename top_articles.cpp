// Dependencies
//  1. nlhomann-json
//  2. cpp-httplib
// Algorithm:
//  1. Fetch all data
//  1.a For each page, append fetched data into a vector
//  1.b If title and story_title is null, skip append
//  1.c If title is null and story_title is not null, copy story_title into title
//  2. Order by num comments decreasing
//  3. On collision, order by Article name
//  4. Return n article titles

#include <algorithm>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <httplib.h>

//  Server address of the Articles
const std::string articlesURL = "https://jsonmock.hackerrank.com";

/**
 * An article fetch from the API
 */
struct Article {
    std::string title;
    int numComments;

    //  Alter the behaviour of the < operator so that we
    //  can sort in descending order
    bool operator<(const Article& other) const {
        if (numComments == other.numComments) {
            return title > other.title;
        }

        return numComments > other.numComments;
    }
};

bool isWhitespace(std::string s){
    for(int index = 0; index < s.length(); index++){
        if(!std::isspace(s[index]))
            return false;
    }
    return true;
}

//  Define a function that handles API calls
std::vector<Article> toArticles(nlohmann::json_abi_v3_12_0::json data) {
    std::vector<Article> articles;

    for (const auto& article: data["data"]) {
        if (article["title"].is_null() && article["story_title"].is_null()) {
            continue;
        }

        std::string title = article["title"].is_null() ? "" : article.value("title", "");
        std::string storyTitle = article["story_title"].is_null() ? "" : article.value("story_title", "");

        if (isWhitespace(title) || title.empty()) {
            if (isWhitespace(storyTitle) || storyTitle.empty()) {
                continue;
            }

            title = storyTitle;
        }

        int numComments = article["num_comments"].is_null() ? 0 :  article.value("num_comments", 0);
        articles.push_back({title, numComments});
    }

    return articles;
}

/**
 * GET all articles from the API
 */
std::vector<Article> fetchArticles() {
    httplib::Client _client(articlesURL);
    int totalPages = 2;
    int i = 1;
    std::vector<Article> articles;

    while (i <= totalPages) {
        std::string url = "/api/articles?page="+std::to_string(i);
        auto res = _client.Get(url.c_str());
        auto page = nlohmann::json::parse(res->body);
        auto moreArticles = toArticles(page);
        articles.insert(articles.end(), moreArticles.begin(), moreArticles.end());

        if (page.value("total_pages", 0) != totalPages) {
            totalPages = page.value("total_pages", 1);
        }

        i++;
    }

    return articles;
}

//  Sort function

/**
 * topArticles gets all articles from the API and returns the top n results.
 * 
 * To run:
 * ```bash
 *  lang++ -std=c++17 -DCPPHTTPLIB_OPENSSL_SUPPORT top_articles.cpp -I$(brew --prefix)/include -L$(brew --prefix)/lib -lssl -lcrypto -framework CoreFoundation -framework Security -o top_articles
 *  ./top_articles
 * ```
 */
std::vector<std::string> topArticles(int limit) {
    auto articles = fetchArticles();
    std::sort(articles.begin(), articles.end());
    
    std::vector<std::string> titles;
    for (int i = 0; i < limit; i++) {
        if (i >= articles.size()) {
            break;
        }

        titles.push_back(articles[i].title);
    }

    return titles;
}
