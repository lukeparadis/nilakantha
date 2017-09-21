import Queue
import threading
import multiprocessing

class Aggregator(threading.Thread):

    def __init__(self,completed,stop,precision):
        threading.Thread.__init__(self)
        self.completed = completed
        self.sum = 0
        self.index = 0
        self.format = '{:.' + str(precision) + 'f}'
        self.stop = stop

    def run(self):
        while self.completed or not self.stop.is_set():
            if self.index in self.completed:
                self.sum += self.completed[self.index]
                del self.completed[self.index]
                self.index += 1
                print self.format.format(self.sum)

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

class SeriesSummer(object):

    def __init__(self,evaluate,num_terms,precision,completed={}):
        
        self.pending = Queue.Queue()
        self.stop = threading.Event()

        for index in range(1,num_terms):
            if index not in completed:
                self.pending.put(index)

        self.aggregator = Aggregator(completed,self.stop,precision)

        num_cores = multiprocessing.cpu_count()

        self.evaluators = []
        for t in range(num_cores-1):
            evaluator = Evaluator(self.pending,completed,evaluate)
            self.evaluators.append(evaluator)

    def run(self):

        for evaluator in self.evaluators:
            evaluator.start()

        self.aggregator.start()
        self.pending.join()
        self.stop.set()

