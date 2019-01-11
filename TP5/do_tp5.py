#!/usr/bin/env python3

import pandas
import re
import os
import matplotlib.pyplot as plt
from sklearn import datasets, linear_model

def extractValuesFromFiles():

    mpiResultsMapTotal = {1:{}, 10:{}, 20:{}, 40:{}, 60:{}, 80:{}, 100:{}, 120:{}}
    threadResultsMapTotal = {1:{}, 2:{}, 4:{}, 8:{}, 10:{}, 12:{}, 16:{}, 20:{}}

    try:
        for filename in os.listdir("./baobab_results_measures"):
            with open("./baobab_results_measures/"+filename, 'r') as myfile:

                data=myfile.read().replace('\n', '')

                # Extract values from files
                extractedNumbers = re.findall("(\d+)(?:\.|ms)", data)
                totalTime = extractedNumbers[0]

                pattern = re.findall("_\d+", filename)
                patternMethod = re.findall("(thread|mpi)", filename)
                patternType = re.findall("(static|simple|dynamic)", filename)

                nProcParam = pattern[0][1:]
                dispatchType = patternType[0]
                computeMethod = patternMethod[0]

                if(computeMethod == "mpi"):
                    mpiResultsMapTotal[int(nProcParam)][str(dispatchType)] = float(totalTime)
                else:
                    if str(dispatchType) == "dynamic":
                        threadResultsMapTotal[int(nProcParam)][str(dispatchType)+str(pattern[1])] = float(totalTime)
                    else:
                        threadResultsMapTotal[int(nProcParam)][str(dispatchType)] = float(totalTime)


        return (threadResultsMapTotal, mpiResultsMapTotal)

    except FileNotFoundError as err:
        print("Error : "+err)


def main():

    # Extract values
    threadTotalDatas, mpiTotalDatas = extractValuesFromFiles()

    # Get Dataframes
    dfThreadTotal = pandas.DataFrame(threadTotalDatas).transpose()
    dfMPITotal = pandas.DataFrame(mpiTotalDatas).transpose()

    print(dfThreadTotal)
    print(dfMPITotal)

    dfThreadTotal.columns = { "Dynamic  (Thread) l = 10", "Dynamic  (Thread) l = 100", "Dynamic  (Thread) l = 1000", "Dynamic  (Thread) l = 10000", "Simple (Thread)", "Static (Thread)"}
    dfMPITotal.columns = {"Simple (MPI)", "Static (MPI)"}

    # Compute regression datas


    # Generate CSVs for archiving
    dfThreadTotal.to_csv("./csv/threadTotal.csv")
    dfMPITotal.to_csv("./csv/mpiTotal.csv")


    # ==============================================================
    # Execution Time Graph
    # ==============================================================

    graphExecTime = dfThreadTotal.plot(kind='line', logy=True)
    dfMPITotal.plot(kind='line', ax=graphExecTime, linestyle="--")
    graphExecTime.set_title('Execution Time (Logarithmic scale on Y axis)', pad=20, fontdict={"fontsize":16,"fontweight":"bold"})
    graphExecTime.set_xlabel("CPUs Number", labelpad=20, fontsize=12, fontweight="semibold")
    graphExecTime.set_ylabel("Execution time in miliseconds", labelpad=20, fontsize=12, fontweight="semibold")
    plt.legend(loc='center left', bbox_to_anchor=(1.0, 0.5))
    graphExecTime.get_figure().savefig("./graphs/execTime.pdf", bbox_inches='tight')

    exit(0)


    # ==============================================================
    # Speedup Graph
    # ==============================================================

    estimatedGammasTotal = []
    estimatedGammasLoopPlusWrite = []

    cols = {2000, 3000, 4000}
    dfLoop.columns = cols
    dfLoopPlusWrite.columns = cols
    dfTotal.columns = cols

    for i in cols:
        estimatedGammasTotal.append(1 - (dfLoop[i][1]/dfTotal[i][1]))
        estimatedGammasLoopPlusWrite.append(1 - (dfLoop[i][1]/dfLoopPlusWrite[i][1]))

    estimatedGammaLoopPlusWrite = 0
    estimatedGammaTotal = 0

    for e in estimatedGammasLoopPlusWrite:
        estimatedGammaLoopPlusWrite += e
    for e in estimatedGammasTotal:
        estimatedGammaTotal += e

    estimatedGammaLoopPlusWrite = estimatedGammaLoopPlusWrite/len(estimatedGammasLoopPlusWrite)
    estimatedGammaTotal = estimatedGammaTotal/len(estimatedGammasTotal)

    print("Estimated Gamma Loop + Write = "+str(estimatedGammaLoopPlusWrite))
    print("Estimated Gamma Total = "+str(estimatedGammaTotal))

    # Estimated Gamma Loop + Write
    speedupEstimatedGammaLoopPlusWrite = {"CPUs Number": [1, 2, 10, 20, 40, 60, 80, 100, 120], "Estimated Gamma Loop + Write Speedup (γ = "+str(estimatedGammaLoopPlusWrite)+")": []}
    for p in speedupEstimatedGammaLoopPlusWrite["CPUs Number"]:
        speedupEstimatedGammaLoopPlusWrite["Estimated Gamma Loop + Write Speedup (γ = "+str(estimatedGammaLoopPlusWrite)+")"].append(1 / (estimatedGammaLoopPlusWrite + ((1-estimatedGammaLoopPlusWrite)/p)))

    # Estimated Gamma Total
    speedupEstimatedGammaTotal = {"CPUs Number": [1, 2, 10, 20, 40, 60, 80, 100, 120], "Estimated Gamma Total Speedup (γ = "+str(estimatedGammaTotal)+")": []}
    for p in speedupEstimatedGammaTotal["CPUs Number"]:
        speedupEstimatedGammaTotal["Estimated Gamma Total Speedup (γ = "+str(estimatedGammaTotal)+")"].append(1 / (estimatedGammaTotal + ((1-estimatedGammaTotal)/p)))

    # Theoretical Gamma 0.1
    speedupTheoretical01 = {"CPUs Number": [1, 2, 10, 20, 40, 60, 80, 100, 120], "Theoretical Speedup for γ = 0.1": []}
    for p in speedupTheoretical01["CPUs Number"]:
        speedupTheoretical01["Theoretical Speedup for γ = 0.1"].append(1 / (0.1 + ((1-0.1)/p)))

    # Theoretical Gamma 0.05
    speedupTheoretical005 = {"CPUs Number": [1, 2, 10, 20, 40, 60, 80, 100, 120], "Theoretical Speedup for γ = 0.05": []}
    for p in speedupTheoretical005["CPUs Number"]:
        speedupTheoretical005["Theoretical Speedup for γ = 0.05"].append(1 / (0.05 + ((1-0.05)/p)))

    speedupLoop = {"CPUs Number": [1, 2, 10, 20, 40, 60, 80, 100, 120], "2000x2000 Main Loop Speedup":[], "3000x3000 Main Loop Speedup":[], "4000x4000 Main Loop Speedup":[]}
    speedupLoopPlusWrite = {"CPUs Number": [1, 2, 10, 20, 40, 60, 80, 100, 120], "2000x2000 Main Loop + Write Speedup":[], "3000x3000 Main Loop + Write Speedup":[], "4000x4000 Main Loop + Write Speedup":[]}

    dfLoop.columns = {"2000x2000 Main Loop Speedup", "3000x3000 Main Loop Speedup", "4000x4000 Main Loop Speedup"}
    dfLoopPlusWrite.columns = {"2000x2000 Main Loop + Write Speedup", "3000x3000 Main Loop + Write Speedup", "4000x4000 Main Loop + Write Speedup"}

    for i in range(3):
        for P, row in dfLoop.iterrows():
            tseq = dfLoop[row.index[i]][1]
            tpar = dfLoop[row.index[i]][P]
            speedupLoop[str(row.index[i])].append(tseq/tpar)
        for P, row in dfLoopPlusWrite.iterrows():
            tseq = dfLoopPlusWrite[row.index[i]][1]
            tpar = dfLoopPlusWrite[row.index[i]][P]
            speedupLoopPlusWrite[str(row.index[i])].append(tseq/tpar)

    dfSpeedupLoop = pandas.DataFrame(speedupLoop)
    dfSpeedupLoop.set_index("CPUs Number", inplace=True)

    dfSpeedupLoopPlusWrite = pandas.DataFrame(speedupLoopPlusWrite)
    dfSpeedupLoopPlusWrite.set_index("CPUs Number", inplace=True)

    dfSpeedupTheoritical01 = pandas.DataFrame(speedupTheoretical01)
    dfSpeedupTheoritical01.set_index("CPUs Number", inplace=True)

    dfSpeedupTheoritical005 = pandas.DataFrame(speedupTheoretical005)
    dfSpeedupTheoritical005.set_index("CPUs Number", inplace=True)


    dfSpeedupEstimatedGammaLoopPlusWrite = pandas.DataFrame(speedupEstimatedGammaLoopPlusWrite)
    dfSpeedupEstimatedGammaLoopPlusWrite.set_index("CPUs Number", inplace=True)

    dfSpeedupEstimatedGammaTotal = pandas.DataFrame(speedupEstimatedGammaTotal)
    dfSpeedupEstimatedGammaTotal.set_index("CPUs Number", inplace=True)


    dfSpeedupLoop.to_csv("./csv/speedupLoop.csv")
    dfSpeedupLoopPlusWrite.to_csv("./csv/speedupLoopPlusWrite.csv")
    dfSpeedupTheoritical01.to_csv("./csv/speedupTheoritical01.csv")
    dfSpeedupTheoritical005.to_csv("./csv/speedupTheoritical005.csv")
    dfSpeedupEstimatedGammaLoopPlusWrite.to_csv("./csv/speedupEstimatedGammaLoopPlusWrite.csv")
    dfSpeedupEstimatedGammaTotal.to_csv("./csv/speedupEstimatedGammaTotal.csv")

    graphSpeedup = dfSpeedupLoop.plot(kind='line')
    dfSpeedupLoopPlusWrite.plot(kind='line', ax=graphSpeedup, linestyle="--")
    dfSpeedupTheoritical01.plot(kind='line', ax=graphSpeedup, linestyle=":")
    dfSpeedupTheoritical005.plot(kind='line', ax=graphSpeedup, linestyle=":")
    dfSpeedupEstimatedGammaLoopPlusWrite.plot(kind='line', ax=graphSpeedup, linestyle="-.")
    dfSpeedupEstimatedGammaTotal.plot(kind='line', ax=graphSpeedup, linestyle="-.")

    graphSpeedup.set_title('Speedup', pad=20, fontdict={"fontsize":16,"fontweight":"bold"})
    graphSpeedup.set_xlabel("CPUs Number", labelpad=20, fontsize=12, fontweight="semibold")
    graphSpeedup.set_ylabel("Speedup", labelpad=20, fontsize=12, fontweight="semibold")

    plt.legend(loc='center left', bbox_to_anchor=(1.0, 0.5))

    graphSpeedup.get_figure().savefig("./graphs/speedup.pdf", bbox_inches='tight')

main()
