/*
    https://leetcode.com/problems/word-break-ii/
    
    TC: O(n * w^(n/min_word_len)) 
    In worst case, each valid word will be part of the recursive call range and would make
    O(w) calls. Also remaining string length can be O(n/min_word_len).
    Constructing them takes O(n)
    
    Or it can be O(n * 2^n)
    SC: O(n * 2^n)
    
    n = length of string
    There can be 2^N no. of valid sentences
*/
class Solution {
public:
    vector<string> findSentences(string& s, int start, unordered_set<string>& words,
                                 unordered_map<int, vector<string>>& dp) {
        // Result cached, return the sentences that can be formed from start position
        if(dp.count(start))
            return dp[start];
        
        string substring;
        for(int i = start; i < s.size(); i++) {
            // Instead of calling substring multiple times, we build the substring incrementally
            substring += s[i];
            
            if(words.count(substring)) {
                // Base case: End of string reached
                if(i == s.size() - 1)
                    dp[start].emplace_back(substring);
                else {
                    vector<string> sentences_from_here = findSentences(s, i + 1,
                                                                    words, dp);
                    
                    // Add the current valid word prefix to the result obtained
                    // This will result in a sentence formed out of s[start:N-1]
                    for(auto sentence: sentences_from_here)
                        dp[start].emplace_back(substring + " " + sentence);
                }
            }
        }
        
        return dp[start];
    }
    
    vector<string> wordBreak(string s, vector<string>& wordDict) {
        // make a set of words for faster lookup
        unordered_set<string> words(wordDict.begin(), wordDict.end());
        // dp(i): Sentence that can be formed from ith position i.e in 
        // s[i : N-1] which can be segmented into valid words 
        unordered_map<int, vector<string>> dp;
        
        return findSentences(s, 0, words, dp);
    }
};


/////////////////////////////////// Doesn't solve in a good manner
/*
    https://leetcode.com/problems/word-break-ii/
    
    TC: O(N^3) [Because of memoization] [Wrong since computation is still carried out in case
    the string is valid from certain position. O(n^3) would have been true for Word Break 1 
    problem not this.]
*/
class Solution {
public:
    bool findSentences(string& s, int start, string partial, 
                       unordered_set<string>& words, unordered_set<string>& result,
                      vector<bool>& dp) {
        
        // all the chars covered
        if(start == s.size()) {
            result.emplace(partial);
            return true;
        }
        // no. of sentences formed before segmentation from current position was done
        int orig_sentences = result.size();
        for(int i = start; i < s.size(); i++) {
            // continue DFS only if valid segmentation is possible from the next char
            if(dp[i+1] == true) {
                string curr_word = s.substr(start, i - start + 1);
                // word found, fix the current word and find the next word
                if(words.find(curr_word) != words.end()) {
                    findSentences(s, i + 1, 
                                  partial + (partial.empty() ? curr_word : " " + curr_word),
                                  words, result, dp);
                }
            }
        }
        // if no sentence could be formed when segmentation started from current start,
        // make the current position as false
        dp[start] = orig_sentences != result.size();
        return dp[start];
    }
    
    vector<string> wordBreak(string s, vector<string>& wordDict) {
        // make a set of words for faster lookup
        unordered_set<string> words(wordDict.begin(), wordDict.end());
        // dp(i): indicates that s[i : N-1] can be segmented into valid words or not 
        vector<bool> dp(s.size() + 1, true);
        unordered_set<string> result;
        string partial;
        
        findSentences(s, 0, partial, words, result, dp);
        return vector<string>{result.begin(), result.end()};
    }
};
