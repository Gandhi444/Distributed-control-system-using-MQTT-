import threading
import time
def test():
    print("t")


threading.Timer(time.time()-1, test).start()
