#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <concepts>
#include <vector>
#include "storage.h"
using namespace std;

enum class TokenType { ADD, INTO, GET, FROM, REMOVE, NONE };

std::unordered_map<std::string, TokenType> token_type_catalog = {
    {"ADD", TokenType::ADD},
    {"INTO", TokenType::INTO},
    {"GET", TokenType::GET},
    {"FROM", TokenType::FROM},
    {"REMOVE", TokenType::REMOVE},
    {"NONE", TokenType::NONE}
};

class QueryParser {
    public:
        bool lexical_parsing(std::string token) {
            try {
                TokenType parsed_token = token_type_catalog.at(token);
                return true;
            } 
            catch(const std::out_of_range &e) {
                return false;
            }
        }
};

class QueryLexer {
    public:
        TokenType tokenize(std::string query) {
            // Need an algorithm to extract tokens
            int char_idx = 0;
            int word_start = 0;
            std::string extracted_token = "";
            TokenType tokenized_token;

            for (char character : query) {
                
                if (char_idx == query.length()) {
                    break;
                }

                // Refactor soon, for now make it work
                if (query[char_idx] == ' ') {
                    extracted_token = query.substr(word_start, char_idx - word_start);
                    if (extracted_token == "") {
                        word_start = char_idx + 1;
                        char_idx += 1;
                        continue;
                    }
                    QueryParser query_pipeline;
                    bool parse_token = query_pipeline.lexical_parsing(extracted_token);
                    if (parse_token == true) {
                        word_start = char_idx + 1;
                    }
                } 
                else if (char_idx + 1 == query.length() || extracted_token != "" && char_idx + 1 == query.length()) {
                    extracted_token = query.substr(word_start, char_idx - word_start + 1); 
                    TokenType tokenized_token = token_type_catalog.at(extracted_token);
                }
                char_idx += 1;
            }

        };
};
