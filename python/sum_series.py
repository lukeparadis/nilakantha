import Queue
import threading
import multiprocessing
import sys

class Aggregator(threading.Thread):
    """Thread to sum terms from an infinite series and sequentially print approximate sums.

    The Aggregator is a thread object that monitors a dict `completed` of evaluated terms from an infinite series. It's function is to sequentially print finite approximations of the infinite sum as the necessary terms become available. These terms are evaluated asynchronously by Evaluator threads so the Aggregator must iteratively check for their availability in the `completed` dict before adding the value to the running sum and printing the result.

    Attributes:
        completed (dict(int,float)) : stored results of term computations.
        sum (float) value keeping track of the running sum.
        index (int) : index for last summed term.
        format (str): format string for printing resulting sums.
        stop (Threading.Event) : signal that evaluation threads are complete

    """
    def __init__(self,completed,stop,precision):
        """Aggregator __init__
           
        Args:
            completed (dict(int->float)) : stored results of term computations 
            stop (Threading.Event) : signal that evaluation threads are complete
            precision (int) : digits past decimal to print.
        """
        
        # Call thread's superclass constructor
        threading.Thread.__init__(self)
        
        # Save ref to shared `completed` data
        self.completed = completed

        # Start sum and index at 0
        self.sum = 0
        self.index = 0

        # Create format string based on input precision
        self.format = '{:.' + str(precision) + 'f}\n'

        # Save signal flag for evaluation comleted
        self.stop = stop

    def run(self):
        """Aggregator thread run loop.

        The Aggregator continuously checks `completed` for the next term in the series. If it finds it, it adds it to the running sum, increments the `index` counter, and removes the computed term from `completed`, and prints out the current sum of all terms up to `index`.
        
        """
        # Continue as long as there is either terms to be summed or the evaluation threads are still running.
        while self.completed or not self.stop.is_set():

            # Look for the next term to be summed
            if self.index in self.completed:

                # Add value of term to running sum
                self.sum += self.completed[self.index]

                # Remove term from `completed` container
                del self.completed[self.index]

                # Increment index for next run through loop
                self.index += 1

                # Print running sum 
                sys.stdout.write(self.format.format(self.sum))
                sys.stdout.flush()

class Evaluator(threading.Thread):
    """Thread object for evaluating terms of an infinite series.

    The Evaluator performs the actual evaluation of terms of the infinite series, taking indexes from the `pending` queue, evaluating them with function `evaluate` and storing results in the `completed` dict. 

    Note that the Queue object is thread safe and that we use queue.task_done() to communicate to the queue that we have performed all necessary work on the retreived item.

    We also set Evaluator threads to daemon mode meaning they will run in the background and can be killed by the main thread when it exits. This is acceptable as long as the the pending queue is empty when the main thread exits which is ensured by using queue.join() in SumSeries.

    Attributes
        pending (Queue(int)) : queue of indexes of unevaluated terms
        completed (dict(int->float)) : stored results of computed terms
        evaluate (function(int->float)) : function to get value of a single term of series

    """

    def __init__(self,pending,completed,evaluate):
        """Evaluator __init__ method.
      
        Stores references for shared data containers and evaluation function, calls superconstuctor and sets the thread to a daemon thread.

        Args
            pending (Queue(int)) : queue of indexes of unevaluated terms
            completed (dict(int->float)) : stored results of computed terms
            evaluate (function(int->float)) : function to get value of a single term of series

        """
        
        # Call thread's superclass constructor
        threading.Thread.__init__(self)
        
        # Store refs to shared data
        self.pending = pending
        self.completed = completed

        # Store ref to evaluation function
        self.evaluate = evaluate

        # Set the threads as daemon threads
        self.daemon = True 

    def run(self):
        """Continuously get next index, evaluate terms, and store result.
        """
        while True:

            # Get the next index in the queue and evaluate
            index = self.pending.get()
            value = self.evaluate(index)

            # Store the result in shared container
            self.completed[index] = value

            # Communicate to the queue that operation has completed.
            self.pending.task_done()

def SumSeries(evaluate, num_terms, precision):
    """Calulate and print the first `num_terms` approximations of an infinite series.

    Given a function `evaluate` which computes a given term of an infinite series, SumSeries uses Evaluator threads and an Aggregator thread to compute terms in parallel and print out approximations of the series as terms become available.

    Evaluator threads retreive the next index, evaluate the term, and store the results in a shared container. The Aggregator thread checks that shared container, sums up terms, and prints the next approximation as new each new term becomes available.

    Indexes are shared among Evaluator threads using `pending`, a Queue of term indexes. Each index we `put` on the queue increments the Queue's task count and each Evaluator call to `pending.task_done()` decrements this count. We use `pending.wait()` to pause execution until this counter hits zero, signaling that Evaluator threads have completed all evaluation. We then set the `stop` event flag to communicate to the Aggregator that all terms have been computed.
    
    Computed terms are stored in `completed`, a dict mapping term indexes to the evaluated term values. This container is shared accross multiple threads without a mutex/lock (Python dictionaries are thread safe for single operations). Evaluator threads only insert into `completed` and the Aggregator thread only accesses and deletes from `completed`. No iterators are used which may become invalid by an operation of another thread.

    Args:
        evaluate (function(int)->numeric) : function for computing nth term of infinite series.ja
        num_terms (int) : number of approximations to print/compute
        precision (int) : number of digits past decimal to print for approximations
    """

    # Create and fill a queue of term indexes for the series to be evaluated at
    pending = Queue.Queue()
    for index in range(num_terms):
        pending.put(index)

    # Empty dict for storing value of computed terms
    # Keys are integer indexes - completed[index] = value
    completed = {}
   
    # Flag for communicating to Aggregator that evaluation threads are done working.
    evaluation_complete = threading.Event()

    # Create Aggregator thread with ref to `completed` shared data
    aggregator = Aggregator(completed,evaluation_complete,precision)
    
    # Get current number of cores on the machine
    num_cores = multiprocessing.cpu_count()
    
    # Create `num_cores` Evaluator threads with refs to `pending` and `completed` shared data
    evaluators = []
    for t in range(num_cores):
        evaluator = Evaluator(pending,completed,evaluate)
        evaluators.append(evaluator)
    
    # Start Evaluator threads
    for evaluator in evaluators:
        evaluator.start()

    # Start Aggregator thread
    aggregator.start()

    # Wait until `pending` queue is empty
    pending.join()
    
    # Tell Aggregator thread that term evaluation is complete
    evaluation_complete.set()

if __name__ == '__main__':
    # Test SumSeries functionality with trivial series.
    # Note, this code is only run when this module is run directly, 
    # not when it is imported into another module.

    def evaluate(n):
        """Test evaluation function"""
        return 1
    
    # Print 10 approximations for `evaluate` with 6-digit precision
    SumSeries(evaluate,10,6)
