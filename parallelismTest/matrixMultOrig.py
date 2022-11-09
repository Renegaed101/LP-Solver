import threading 
import multiprocessing
import time 

 

def main():

    iters = 50000
    processes = []
    startTime = time.time()
    numCores = 1
    for i in range (0,numCores):
        proc = multiprocessing.Process(target=matrixMult,args=(i,iters//numCores,),daemon=True)
        proc.start()
        proc.join()
    endTime = time.time()
    print ("Using " + str(numCores) + " core/cores : " + str(iters) + " iterations took " + str(endTime - startTime) + " seconds")

    
    numCores = multiprocessing.cpu_count()//2

    startTime = time.time()
    for i in range (0,numCores):
        proc = multiprocessing.Process(target=matrixMult,args=(i,iters//numCores,),daemon=True)
        processes.append(proc)
        proc.start()
    
    for proc in processes:
        proc.join()
    
    endTime = time.time()
    print ("Using " + str(numCores) + " core/cores : " + str(iters) + " iterations took " + str(endTime - startTime) + " seconds")

    numCores = multiprocessing.cpu_count()
    processes = []

    startTime = time.time()
    for i in range (0,numCores):
        proc = multiprocessing.Process(target=matrixMult,args=(i,iters//numCores,),daemon=True)
        processes.append(proc)
        proc.start()

    for proc in processes:
        proc.join()
    
    endTime = time.time()
    print ("Using " + str(numCores) + " core/cores : " + str(iters) + " iterations took " + str(endTime - startTime) + " seconds")

    #Thread based implementation, Python's C implementation of Python (the default) 
    # uses a global interpreter lock that prevents full concurrency accross
    # multiple physical processors 
    """""
    for i in range (0,12):
        x = threading.Thread(target = matrixMult,args = (i,), daemon=True)
        x.start()
    """

    

def matrixMult(threadNum,iters):

    #print ("starting process " + str(threadNum))
    
    matrix = []
    for i in range (0,100):
        column = []
        for i in range (0,100):
            column.append(i % 15)
        matrix.append(column)

    for k in range (0,iters):
        for i in matrix:
            for j in i:
                j = j*j 

    """""
    print ("starting thread " + str(threadNum))
    
    matrix = []
    for i in range (0,100):
        column = []
        for i in range (0,100):
            column.append(i % 15)
        matrix.append(column)


    iters = 50000
    startTime = time.time()
    for k in range (0,iters):
        for i in matrix:
            for j in i:
                j = j*j
    endTime = time.time()
    print ( str(iters) + " iterations took " + str(endTime - startTime) + " seconds")
    """  

if __name__ == '__main__': 
    main()