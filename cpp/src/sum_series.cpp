/*! \file sum_series.cpp
 *  \Author Luke Paradis
 *  \data September, 2017
 *  \brief Compute and print approximations of an infinite series.
 */
#include "sum_series.hpp"
#include <numeric>
#include <iostream>
#include <string>
#include <atomic>

void Aggregate( std::unordered_map<int,double> * complete,
                std::mutex * complete_mutex,
                std::atomic<bool> * evaluation_complete,
                int precision ) {
  
  // Initialize sum and index to zero
  double sum = 0;
  int index = 0;
  
  // Generate format string using desired precision
  std::string format = "%." + std::to_string(precision) + "f\n";
  
  // Iterator for searching for next available index
  std::unordered_map<int,double>::iterator iter;
  
  // Flag for stopping condition
  bool stop = false;

  while (!stop) {
    
    // Exclude other threads from accessing `complete`
    complete_mutex->lock();
  
    // Stop loop iteration if the `complete` container is empty and 
    // all terms have been evaluated by evaluation threads.
    if( complete->size() == 0 && evaluation_complete->load() ) {

      // Set flag to kill while loop
      stop = true;

      // Release lock on `complete`
      complete_mutex->unlock();

      // Return to while loop condition
      continue;
    }
  
    // Search for next index in `complete` container
    iter = complete->find(index);

    if( iter != complete->end() ){

      // Add new term to running approximation
      sum += iter->second;

      // Increment index for next loop.
      index++;

      // Remove evaluated term from `complete`
      complete->erase(iter);

      // Print current approximation  
      printf( format.c_str(), sum );
    }
  
    // Release lock on `complete`
    complete_mutex->unlock();

  }

};

void Evaluate(  std::function<double(int)> evaluate,
                std::queue<int> * pending,
                std::unordered_map<int,double> * complete,
                std::mutex * pending_mutex,
                std::mutex * complete_mutex ) {
  
  // While loop condition set to true when pending queue is empty
  bool stop = false; 
 
  while (!stop) {
    
    // Restrict access to `pending` queue.
    pending_mutex->lock();
   
    // If `pending` queue is empty, stop loop.
    if( pending->size() == 0 ) {

      // Set flag to kill while loop
      stop = true;

      // Release lock on `pending`
      pending_mutex->unlock();

      // Return to while loop condition
      continue;
    }
   
    // Get and remove next index from `pending` queue. C++ STL queue has typical 
    // `popping` separated into two  operations.
    int index = pending->front();
    pending->pop();

    // Release access to `pending` queue
    pending_mutex->unlock();
  
    // Evaluate series at `index`
    double value = evaluate(index);
  
    // Restrict access to `complete` map
    complete_mutex->lock();

    // Insert the (index,value) pair into `complete` map
    complete->insert(std::pair<int,double>(index, value));

    // Release access to `complete` map
    complete_mutex->unlock();

  }

};

void SumSeries( std::function<double(int)> evaluate,
                int num_terms,
                int precision ) {

  // Queue of indexes to evaluate series at and fill with integers.
  std::queue<int> pending;
  for( int k = 0; k < num_terms; k++ )
    pending.push(k);

  // Map of (index,value) pairs to store results of evaluated terms of an infinite series.
  std::unordered_map<int,double> complete;

  // Create mutex locks for `pending` and `complete` containers to restrict
  // access to them to one thread at a time.
  std::mutex pending_mutex;
  std::mutex complete_mutex;
  
  // Number of threads set to number of available cores.
  int num_threads = std::thread::hardware_concurrency();
  
  // Vector for storing references to evaluation threads.
  std::vector<std::thread> evaluation_threads;

  for( int k = 0; k < num_threads; k++ ) {
    // Create a thread with Evaluate as the target function followed by list of arguments to Evaluate
    evaluation_threads.push_back( 
        std::thread( Evaluate, evaluate, &pending, &complete, &pending_mutex, &complete_mutex ));
  };
 
  // Create a flag for signaling to the aggregation thread that all evaluation threads have 
  // completed evaluating the series. Volatile 
  std::atomic<bool> evaluation_complete(false);

  // Create a thread with Aggregate as the target function followed by list of arguments to Aggregate 
  std::thread aggregation_thread( 
      Aggregate, &complete, &complete_mutex, &evaluation_complete, precision );

  // Wait for all evaluation threads to complete.
  for( std::thread & evaluation_thread : evaluation_threads ) {
    evaluation_thread.join();
  };

  // Communicate to aggregation thread that all evaluation has complete
  evaluation_complete.store(true);

  // Wait for aggregation thread to complete summing/printing
  aggregation_thread.join();

};

