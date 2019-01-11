def extractValuesFromFiles():

    mpiResultsMapTotal = {1:{}, 10:{}, 20:{}, 40:{}, 60:{}, 80:{}, 100:{}, 120:{}}
    threadResultsMapTotal = {1:{}, 2:{}, 4:{}, 8:{}, 10:{}, 12:{}, 16:{}, 20:{}}

    try:
        for filename in os.listdir("./baobab_results_measures_v2"):
            with open("./baobab_results_measures_v2/"+filename, 'r') as myfile:

                data=myfile.read().replace('\n', '')

                # Extract values from files
                extractedNumbers = re.findall("(\d+)(?:\.|ms)", data)
                totalTime = extractedNumbers[0]

                pattern = re.findall("_\d+", filename) #-------------------------------------------
                patternMethod = re.findall("(thread|mpi)", filename)
                patternType = re.findall("(static|simple|dynamic)", filename)

                nProcParam = pattern[0][1:] #-------------------------------------------
                dispatchType = patternType[0]
                computeMethod = patternMethod[0]

                if(computeMethod == "mpi"):
                    mpiResultsMapTotal[int(nProcParam)][str(dispatchType)] = float(totalTime)
                else:
                    if str(dispatchType) == "dynamic": #-------------------------------------------
                        threadResultsMapTotal[int(nProcParam)][str(dispatchType)+str(pattern[1])] = float(totalTime)#-------------------------------------------
                    else:#-------------------------------------------
                        threadResultsMapTotal[int(nProcParam)][str(dispatchType)] = float(totalTime)#-------------------------------------------


        return (threadResultsMapTotal, mpiResultsMapTotal)

    except FileNotFoundError as err:
        print("Error : "+err)
