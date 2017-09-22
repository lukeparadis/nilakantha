#ifndef __SUM_SERIES_HPP__
#define __SUM_SERIES_HPP__

#include <queue>
#include <thread>
#include <unordered_map>
#include <functional>

void Aggregate( std::queue<int> * pending, 
                std::unordered_map<int,double> * complete,
                std::mutex * pending_mutex,
                std::mutex * complete_mutex, 
                volatile bool * stop, 
                int precision );

void Evaluate( std::function<double(int)> evaluate,
                std::queue<int> * pending,
                std::unordered_map<int,double> * complete,
                std::mutex * pending_mutex,
                std::mutex * complete_mutex );

void SumSeries( std::function<double(int)> evaluate,
                int num_terms,
                int precision );

#endif
