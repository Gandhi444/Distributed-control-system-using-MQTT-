import threading
import time

class Reapeted_Timer():
    def __init__(self, increment,callback):
        self.next_t = time.time()
        self.done=False
        self.increment = increment
        self.callback=callback
        self._run()
    def _run(self):
        self.callback()
        self.next_t+=self.increment
        if not self.done:
            threading.Timer( self.next_t - time.time(), self._run).start()
    
    def stop(self):
        self.done=True

