import Queue
import threading
import multiprocessing
import sys

class Aggregator(threading.Thread):

    def __init__(self,completed,stop,precision):
        threading.Thread.__init__(self)
        self.completed = completed
        self.sum = 0
        self.index = 0
        self.format = '{:.' + str(precision) + 'f}\n'
        self.stop = stop

    def run(self):
        while self.completed or not self.stop.is_set():
            if self.index in self.completed:
                self.sum += self.completed[self.index]
                del self.completed[self.index]
                self.index += 1
                sys.stdout.write(self.format.format(self.sum))
                sys.stdout.flush()

class Evaluator(threading.Thread):

    def __init__(self,pending,completed,evaluate):

        threading.Thread.__init__(self)
        self.pending = pending
        self.completed = completed
        self.evaluate = evaluate
        self.daemon = True 

    def run(self):

        while True:

            index = self.pending.get()
            value = self.evaluate(index)
            self.completed[index] = value
            self.pending.task_done()

def SumSeries(evaluate, num_terms, precision):

    pending = Queue.Queue()
    completed = {}
    evaluation_complete = threading.Event()

    for index in range(0,num_terms):
        pending.put(index)

    aggregator = Aggregator(completed,evaluation_complete,precision)

    num_cores = multiprocessing.cpu_count()

    evaluators = []

    for t in range(num_cores-1):
        evaluator = Evaluator(pending,completed,evaluate)
        evaluators.append(evaluator)

    for evaluator in evaluators:
        evaluator.start()

    aggregator.start()
    pending.join()
    evaluation_complete.set()

if __name__ == '__main__':

    def evaluate(n):
        return n

    SumSeries(evaluate,10,6)
