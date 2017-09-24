/*! \file sum_series.hpp
 *  \Author Luke Paradis
 *  \date September, 2017
 *  \brief Compute and print approximations of an infinite series.
 */

#ifndef __SUM_SERIES_HPP__
#define __SUM_SERIES_HPP__

#include <queue>
#include <thread>
#include <unordered_map>
#include <functional>
#include <atomic>

/** \fn Aggregate( std::unordered_map<int,double> * complete, std::mutex * complete_mutex, std::atomic<bool> * evaluation_complete, int precision );
 *
 * \par Description
 * `Aggregate` is the target function for an aggregation thread used by `SumSeries`. Evaluation threads populate `complete` with evaluated terms from an infinite series. Since these terms may not appear `complete` in order, `Aggregate` checks for the existence of the next term in the sequence and when it finds it it adds it to the running sum and prints the preliminary approximation. It exits when there are no remain terms in `complete` to process, and the evaluation threads have completed which is singaled by `evaluation_complete`. A mutex lock is used to control access to `complete` to avoid conflicts with evaluation threads.
 *
 * \param[in,out] complete Map of (index,value) pairs of evaluated terms.
 * \param[in] complete_mutex Mutex restricting access to `complete`.
 * \param[in] evaluation_complete Flag signaling completion of evaluation threads.
 * \param[in] precision Number of digits past decimal to print for approximations.
 *
 */
void Aggregate( std::unordered_map<int,double> * complete,
                std::mutex * complete_mutex, 
                std::atomic<bool> * evaluation_complete,
                int precision );

/** \fn  void Evaluate(  std::function<double(int)> evaluate, std::queue<int> * pending, std::unordered_map<int,double> * complete, std::mutex * pending_mutex, std::mutex * complete_mutex );
 *
 * \par Description
 * `Evaluate` is the target function for an evaluation thread used by `SumSeries`. Each thread retreives the next available index from `pending`, evaluates the series at that index using the `evaluate` function, and stores the result in `complete` while mutexes are used to control access to `pending` and `complete`. The `Evaluate` function and hence the evaluation thread exits when the `pending` queue is empty.
 *
 * \param[in] evaluate Function int->double to evaluate terms of series.
 * \param[in,out] pending Queue of indecies of remaining terms.
 * \param[in,out] complete Map of (index,value) pairs of evaluated terms.
 * \param[in] pending_mutex Mutex restricting access to `pending`
 * \param[in] complete_mutex Mutex restricting access to `complete`
 *
 */
void Evaluate(  std::function<double(int)> evaluate,
                std::queue<int> * pending,
                std::unordered_map<int,double> * complete,
                std::mutex * pending_mutex,
                std::mutex * complete_mutex );

/** \fn void SumSeries( std::function<double(int)> evaluate, int num_terms, int precision );
 * \brief Function to compute and print finite sum approximations of infinite series.
 *
 * \par Description
 *
 * `SumSeries` sums terms in an infinite series and prints the first `num_terms` finite approximations in sequential order. This is accomplished using aggregation and evaluation threads with targets of `Aggregate` and `Evaluate` respectively. Evaluation threads retreive an index from an index queue, evaluate the series at that index using `evaluate` and push the results into a container storing evaluated terms. An aggregation thread checks that container of evaluated terms and sums and prints successive approximations of the series as the necessary terms are calculated by evaluation threads.
 *
 * \param[in] evaluate Function to evaluate a term of an infinite series.
 * \param[in] num_terms Number of terms to evaluate to approximate infinite series.
 * \param[in] precision Number of digits past decimal to print for approximations.
 */
void SumSeries( std::function<double(int)> evaluate,
                int num_terms,
                int precision );

#endif
