import multiprocessing
import time 
import sys 
import fractions
import numpy

optimizationVariables = None
slackVariables = None

def main():
    global optimizationVariables
    global slackVariables

    
    initDictionary = []
    readInput(initDictionary) 
    initDictionary = numpy.array(initDictionary)  

    optimizationVariables =  ['x' + str(i) for i in range(1,len(initDictionary[0]))] 
    slackVariables = ['w'+ str(i) for i in range(1,len(initDictionary))]

    initiallyFeasible = checkInitFeasibility(initDictionary)

    if initiallyFeasible:
        simplexMethod(initDictionary)
    else: 
        dualMethod(initDictionary)

def checkIsOptimal(dic):
    isOptimal = True
    for i in range(1,len(dic[0])):
        if dic[0][i]> 0:
            isOptimal = False
            break
    return isOptimal

def swapOptSlackVars():
    global optimizationVariables
    global slackVariables

    temp = optimizationVariables
    optimizationVariables = slackVariables
    slackVariables = temp


def dualMethod(Dic):

    originalObjFunction = [('x' + str(i),elem) for i,elem in enumerate(Dic[0]) if i > 0]

    Dic[0] = [0] * len(Dic[0])

    degenPivots = 0
    prevOptValueIncrease = -1

    dual = Dic.dot(-1).transpose()


    swapOptSlackVars()

    while True:
        startTime = time.time()
        
       
        if checkIsOptimal(dual):
            break
      
        print ("dualPivot")
        
        if degenPivots >= 20:
            dualLargestIncrease = blandsRule(dual)
        else:
            a = time.time()
            dualLargestIncrease = PARlargestIncreaseRule(dual)
            print ("Parallel Method took %f seconds" % (time.time()-a))            
        if dualLargestIncrease == None:
            print ("infeasible") # If the Dual is Unbounded, the Primal is infeasible
            exit()
        #print ("obj Value will increase by ",dualLargestIncrease[0])

        if degenPivots < 20:
            if dualLargestIncrease[0] == prevOptValueIncrease:
                degenPivots+=1
            else:
                degenPivots = 0           
            prevOptValueIncrease = dualLargestIncrease[0]
            
   
        PARpivot(dual,dualLargestIncrease[1],dualLargestIncrease[2])
      
        endTime = time.time()
        print ("Pivot took ",endTime-startTime," seconds")

    Dic = dual.dot(-1).transpose()
    
    swapOptSlackVars()

    for tuple in originalObjFunction:
        if tuple[0] in slackVariables:
            index = slackVariables.index(tuple[0]) + 1
            Dic[0] = list(map(lambda i,j: i + (j*tuple[1]), Dic[0],Dic[index]))
        else:
            index = optimizationVariables.index(tuple[0]) + 1
            Dic[0][index] += tuple[1]
            
            
    simplexMethod(Dic)
    
    
    
def simplexMethod(Dic): 
    
    degenPivots = 0
    prevOptValueIncrease = -1
    while True:
        if checkIsOptimal(Dic):
            print ("optimal\n%f" % (float(Dic[0][0])))
            printOptAssignment(Dic)
            exit()

        if degenPivots >= 20:    
            largestIncrease = blandsRule(Dic)
        else:
            largestIncrease = PARlargestIncreaseRule(Dic)
        if largestIncrease == None:
            print ("unbounded")
            exit()

        if degenPivots < 20:
            if largestIncrease[0] == prevOptValueIncrease:
                degenPivots+=1
            else:
                degenPivots = 0           
            prevOptValueIncrease = largestIncrease[0]
        
        #print (optimizationVariables)
        #print (slackVariables)
        #print(largestIncrease)
       
        PARpivot(Dic,largestIncrease[1],largestIncrease[2])

    
        #print (optimizationVariables)
        #print (slackVariables)

        #print ([str(i) for elem in Dic 
        #                   for i in elem])




def printOptAssignment(Dic):
    optimalAssignments = [(i,0) for i in optimizationVariables if i[0] == "x"] 
    optimalAssignments += [(i,Dic[index+1][0]) for index,i in enumerate(slackVariables) if i[0] == "x"]
    optimalAssignments.sort(key=lambda i: int(i[0][1:]))
    for optVar in optimalAssignments:
        print (float(optVar[1]),end=" ")
    print()



def pivot (Dic, EnterVarIndex, ExitVarIndex,start,end,intDic):

    rowLength = len(Dic[0])*2 

    def dicToIntDicRow(offset):
        for i in range (offset*rowLength,offset*rowLength + rowLength,2):
            DicJIndex = (i%rowLength)//2
            intDic[i] = Dic[offset][DicJIndex].numerator 
            intDic[i+1] = Dic[offset][DicJIndex].denominator

    for i in range(start,end):
        if i == ExitVarIndex+1:
            dicToIntDicRow(i)
            continue
        
        multiplier = Dic[i][EnterVarIndex+1]
        Dic[i][EnterVarIndex+1] = 0
        Dic[i] = list(map(lambda i,j: i + (j*multiplier),Dic[i],Dic[ExitVarIndex+1]))
        dicToIntDicRow(i)  


def PARpivot (Dic, EnterVarIndex, ExitVarIndex):
    global optimizationVariables
    global slackVariables

    #print ("pivot made")
    temp = optimizationVariables[EnterVarIndex]
    optimizationVariables[EnterVarIndex] = slackVariables[ExitVarIndex]
    slackVariables[ExitVarIndex] = temp
    
    #computing row for new slack variable 
    divisor = Dic[ExitVarIndex+1][EnterVarIndex+1] * -1
    Dic[ExitVarIndex+1] = [elem/divisor for elem in Dic[ExitVarIndex+1]]
    Dic[ExitVarIndex+1][EnterVarIndex+1] = -1/divisor
    
    processes = []
    numCores = multiprocessing.cpu_count()
    intDic = multiprocessing.RawArray('i',range(len(Dic)*len(Dic[0])*2))

    if len(Dic) >= numCores:
        for i in range(0,numCores):            
            start = (len(Dic)//numCores)*i
            if i == numCores-1:
                end = len(Dic)
            else:
                end = (len(Dic)//numCores)*(i+1)
            proc = multiprocessing.Process(target=pivot, \
                            args=(Dic,EnterVarIndex,ExitVarIndex,start,end,intDic),daemon=True)
            processes.append(proc)
            proc.start()
    else:
        for i in range (0,len(Dic)):
            start = i
            end = i+1
            proc = multiprocessing.Process(target=pivot, \
                            args=(Dic,EnterVarIndex,ExitVarIndex,start,end,intDic),daemon=True)
            processes.append(proc)
            proc.start()

    for proc in processes:
        proc.join()

    rowLen = len(Dic[0])*2
    for i in range(len(Dic)):
        for j in range(len(Dic[0])):
            Dic[i][j] = fractions.Fraction(intDic[i*rowLen + (j*2)], \
                                        intDic[i*rowLen + (j*2) + 1])
                
def blandsRule(Dic):
    print ("using blands rule")
    enteringVariable = None    
    sortedOptVariables = sortVariables(optimizationVariables)
    sortedSlackVariables = sortVariables(slackVariables)
    for var in sortedOptVariables:
        if Dic[0][var[1]] > 0:
            enteringVariable = var[1]-1
            exitingVariable = exitingVariableBlands(Dic,var[1],sortedSlackVariables)
            if exitingVariable[0] == None:
                if checkAllZeros(Dic,var[1]):
                    continue
                else:
                    return None
            break
    #print ("Entering Variable ", optimizationVariables[enteringVariable], ",Exiting Variable ",slackVariables[exitingVariable[1]])
    return  (-1,enteringVariable,exitingVariable[1])

def exitingVariableBlands(Dic,j,sortedSlackVar):
    minObjVarValue = (None,"Exiting Variable Index")
    for i in range (1,len(Dic)):
            if Dic[i][j] < 0:
               objVarValue = (Dic[i][0]/Dic[i][j]) * -1
               if   minObjVarValue[0] == None or objVarValue < minObjVarValue[0]:
                    minObjVarValue = (objVarValue,i-1)
               elif minObjVarValue[0] == None or objVarValue == minObjVarValue[0]:
                    sortedNamesOnly = list(zip(*sortedSlackVar))[0]
                    new = sortedNamesOnly.index((slackVariables[i-1]))
                    old = sortedNamesOnly.index((slackVariables[minObjVarValue[1]]))
                    if new < old:
                        minObjVarValue = (objVarValue,i-1)
    return minObjVarValue


def sortVariables(vars):
    sortedX = []
    sortedW = []
    for i,var in enumerate(vars):
        if var[0] == 'x':
            sortedX.append((var,i+1))
        else:
            sortedW.append((var,i+1))
    sortedX.sort(key=lambda i: int(i[0][1:]))
    sortedW.sort(key=lambda i: int(i[0][1:]))
    
    return sortedX + sortedW

def PARlargestIncreaseRule(Dic):
    processes = []
    numCores = multiprocessing.cpu_count()
    candidates = multiprocessing.RawArray('i',range(numCores*4))

    if len(Dic[0]) >= numCores:
        for i in range(0,numCores):
            if i == 0:
                start = 1
            else:
                start = (len(Dic[0])//numCores)*i
            if i == numCores-1:
                end = len(Dic[0])
            else:
                end = (len(Dic[0])//numCores)*(i+1)
            proc = multiprocessing.Process(target=largestIncreaseRule, \
                            args=(i,Dic,candidates,start,end),daemon=True)
            processes.append(proc)
            proc.start()
    else:
        for i in range (1,len(Dic[0])):
            start = i
            end = i+1
            proc = multiprocessing.Process(target=largestIncreaseRule, \
                            args=(i,Dic,candidates,start,end),daemon=True)
            processes.append(proc)
            proc.start()

    for proc in processes:
        proc.join()
    
    largestIncrease = -1
    largestIncreaseIndex = -1
    for i in range(0,len(candidates),4):
        if candidates[i] == -5:
            return None
        val = fractions.Fraction(candidates[i]/candidates[i+1]) 
        if val > largestIncrease:
            largestIncrease = val
            largestIncreaseIndex = i


    return (largestIncrease,candidates[largestIncreaseIndex+2],candidates[largestIncreaseIndex+3])
       
def largestIncreaseRule(index,Dic,candidates,start,end):
    
    #print ("Process %d worked from index %d to %d. Total Size %d" % (index,start,end,len(Dic[0])))
    largestIncrease = (-1,-1,-1)
    for j in range (start,end):
        if Dic[0][j] <= 0:
                continue 
        minObjVarValue = exitingVariable(Dic,j)
        if minObjVarValue[0] == None:
            if checkAllZeros(Dic,j):
                continue
            else:
                candidates[(index*4)] = -5
                exit()
        currentIncrease = Dic[0][j] * minObjVarValue[0]
        if currentIncrease >= largestIncrease[0]:
            largestIncrease = (currentIncrease,j-1,minObjVarValue[1]) 
    
    largestIncrease = (largestIncrease[0].numerator,largestIncrease[0].denominator,\
                                                largestIncrease[1],largestIncrease[2])

    for i in range(0,4):
        candidates[(index*4)+i] = largestIncrease[i] 
       


def checkAllZeros(Dic,j):
    for i in range (1,len(Dic)):
        if Dic[i][j] != 0:
            return False 
    return True

def exitingVariable(Dic,j):
    minObjVarValue = (None,"Exiting Variable Index")
    for i in range (1,len(Dic)):
            if Dic[i][j] < 0:
               objVarValue = (Dic[i][0]/Dic[i][j]) * -1
               if  minObjVarValue[0] == None or objVarValue < minObjVarValue[0]:
                    minObjVarValue = (objVarValue,i-1)
    return minObjVarValue


def checkInitFeasibility(initDic):
    """Checks if dictionary is initially feasible"""

    for i,row in enumerate(initDic):
        if i == 0:
            continue
        else: 
            if row[0] < 0:
                return False 
    return True 
            
def readInput(initDictionary):
    """Converts given input into dictionary form"""

    objFunHandled = False
    for line in  sys.stdin:
        currentLine = line.split()
        if len(currentLine) == 0:
            continue
        
      
        if objFunHandled == False: 
            currentLine = list(map(fractions.Fraction, currentLine))
            currentLine.insert(0,fractions.Fraction(0))
            initDictionary.append(currentLine)
            objFunHandled = True
        else:
            temp = fractions.Fraction(currentLine.pop()) 
            currentLine = list(map(fractions.Fraction, currentLine))
            currentLine = list(map(lambda i: i*-1, currentLine))
            currentLine.insert(0,temp)
            initDictionary.append(currentLine)


if __name__ == "__main__":
    main()