#include "sum_series.hpp"
#include <numeric>
#include <iostream>
#include <string>

void Aggregate( std::queue<int> * pending,
                std::unordered_map<int,double> * complete,
                std::mutex * pending_mutex,
                std::mutex * complete_mutex,
                volatile bool * evaluation_complete,
                int precision ) {

  double sum = 0;
  int index = 0;
  
  std::string format = ("%." + std::to_string(precision) + "f\n").c_str();

  std::unordered_map<int,double>::iterator iter;

  bool stop = false;
  while (!stop) {
    
    complete_mutex->lock();

    //std::cout << complete_->size() << std::endl;
    if( complete->size() == 0 && *evaluation_complete ) {
      stop = true;
      complete_mutex->unlock();
      continue;
    }

    iter = complete->find(index);
    if( iter != complete->end() ){
      sum += iter->second;
      index++;
      complete->erase(iter);
      printf( format.c_str(), sum );
    }

    complete_mutex->unlock();

  }

};

void Evaluate(  std::function<double(int)> evaluate,
                std::queue<int> * pending,
                std::unordered_map<int,double> * complete,
                std::mutex * pending_mutex,
                std::mutex * complete_mutex ) {

  bool stop = false; 
  
  while (!stop) {

    pending_mutex->lock();
    
    if( pending->size() == 0 ) {
      stop = true;
      pending_mutex->unlock();
      continue;
    }
    
    int index = pending->front();
    pending->pop();

    pending_mutex->unlock();

    double value = evaluate(index);

    complete_mutex->lock();
    //std::cout << id_ << "\t" << index << "\t" << value << std::endl;
    complete->insert(std::pair<int,double>(index,value));
    complete_mutex->unlock();

  }

};

void SumSeries( std::function<double(int)> evaluate,
                int num_terms,
                int precision ) {

  std::queue<int> pending;
  for( int k = 0; k < num_terms; k++ )
    pending.push(k);

  std::unordered_map<int,double> complete;
  std::mutex pending_mutex;
  std::mutex complete_mutex;

  int num_threads = std::thread::hardware_concurrency();

  std::vector<std::thread> evaluation_threads;

  for( int k = 0; k < num_threads; k++ ) {
    //std::thread t0( Evaluate, evaluate, &pending, &complete, &pending_mutex, &complete_mutex );
    evaluation_threads.push_back( std::thread( Evaluate, evaluate, &pending, &complete, &pending_mutex, &complete_mutex ));
  };
  
  volatile bool evaluation_complete = false;

  std::thread aggregation_thread( Aggregate, &pending, &complete, &pending_mutex, &complete_mutex, &evaluation_complete, precision );

  for( std::thread & evaluation_thread : evaluation_threads ) {
    evaluation_thread.join();
  };

  evaluation_complete = true;
  aggregation_thread.join();

};

