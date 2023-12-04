#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <algorithm>

using namespace std;
bool left = true;
bool right = false;
ifstream inputFile;
ofstream exOutputFile;
ofstream dyOutputFile;
vector<int> sizesVec;
vector<vector<pair<int,int>>> stocksValuesVec;
vector<int> amountVec;
vector<vector<pair<int,int>>> candidate_items;

void getInputsFromInputFile()
{
    string line;
    int lineCount = 0;
    while (getline(inputFile, line))
    {
        switch (lineCount % 4)
        {
            case 0:
                sizesVec.push_back(stoi(line));
                stocksValuesVec.push_back({});
                break;
            case 1:
                for(int i = 1; i < line.size(); i++)
                {
                    if(line[i] == '[')
                    {
                        int colonIndex = line.find(',',i);
                        if(colonIndex == string::npos) {break;}
                        int endBracketIndex = line.find(']',i);

                        int first = stoi(line.substr(i+1,colonIndex-i-1));
                        int second = stoi(line.substr(colonIndex+1,endBracketIndex-colonIndex-1));
                        pair<int,int> p = {first, second};
                        stocksValuesVec[lineCount/4].push_back(p);
                        i += (endBracketIndex - i);
                    }
                }
                break;
            case 2:
                amountVec.push_back(stoi(line));
                break;
            default:
                break;
        }
        lineCount++;
    }
}

void generateCandidateItems(vector<pair<int,int>> stocks)
{
    // Don't add if already in candidate_items
    if(find(candidate_items.begin(), candidate_items.end(), stocks) == candidate_items.end())
    {
        candidate_items.push_back(stocks);
    }
    if(stocks.size() == 1) { return; }
    
    vector<pair<int,int>> tempStock = stocks;
    for(int i = 0; i < stocks.size(); i++)
    {
        tempStock = stocks;
        tempStock.erase(tempStock.begin()+i);
        generateCandidateItems(tempStock);
    }
    return;
}

int totalValue(vector<pair<int,int>> v)
{
    int totalValue = 0;
    for(auto item = v.begin(); item != v.end(); ++item)
    {
        totalValue += item[0].first;
    }
    return totalValue;
}

bool verifyStocks(int M, vector<pair<int,int>> candidateStocks)
{
    int totalValue = 0;
    for(int i = 0; i < candidateStocks.size(); i++)
    {
        totalValue += candidateStocks[i].second;
    }
    if(totalValue <= M) {return true;}
    else { return false;}
}

int exhaustiveSearch(const vector<pair<int,int>> &stocks, int M)
{
    int best = 0;
    vector<pair<int,int>> bestCandidate;
    generateCandidateItems(stocks);
    for(int i = 0; i < candidate_items.size(); i++)
    {
        if(verifyStocks(M, candidate_items[i]))
        {
            if(best == -1 || totalValue(candidate_items[i]) > best)
            {
                best = totalValue(candidate_items[i]);
                bestCandidate = candidate_items[i];
            }
        }
    }
    return best;
}

int dynamicProgram(int n, vector<pair<int, int>> &stocks, int w)
{
    vector<vector<int>> K(n + 1, vector<int>(w + 1, 0));
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= w; j++) {
            if (i == 0 || j == 0) {
                K[i][j] = 0;
            }
            else if (stocks[i - 1].second <= j) {
                K[i][j] = max(stocks[i - 1].first + K[i - 1][j - stocks[i - 1].second], K[i - 1][j]);
            }
            else {
                K[i][j] = K[i - 1][j];
            }
        }
    }
    return K[n][w];
}

void testExhaustiveSearch()
{
    for(int testCase = 0; testCase < stocksValuesVec.size(); testCase++)
    {
        int r = 0;
        if(sizesVec[testCase] != 0)
        {
            r = exhaustiveSearch(stocksValuesVec[testCase], amountVec[testCase]);
        }
        exOutputFile << "Test " << testCase << ": " << r << endl;
        candidate_items.erase(candidate_items.begin(), candidate_items.end());
    }
}

void testDynamicSearch()
{
    for(int testCase = 0; testCase < stocksValuesVec.size(); testCase++)
    {
        int r = dynamicProgram(sizesVec[testCase], stocksValuesVec[testCase], amountVec[testCase]);
        dyOutputFile << "Test " << testCase << ": " << r << endl;
    }
}

int main()
{
    inputFile.open("input.txt");
    exOutputFile.open("exahust_output.txt");
    dyOutputFile.open("dynamic_output.txt");

    getInputsFromInputFile();
    testExhaustiveSearch();
    testDynamicSearch();
    
    inputFile.close();
    exOutputFile.close();
    dyOutputFile.close();
}