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
        for filename in os.listdir("./baobab_results_measures_fin"):
            with open("./baobab_results_measures_fin/"+filename, 'r') as myfile:

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

    # ==============================================================
    # Speedup Graph
    # ==============================================================
    
    speedupMPITotal = {"CPUs Number": [1, 10, 20, 40, 60, 80, 100, 120], "simple":[], "static":[]}
    speedupThreadTotal = {"CPUs Number": [1, 2, 4, 8, 10, 12, 16, 20],"dynamic_100":[], "dynamic_1000":[], "static":[],"simple":[]}

    for i in range(2):
        for P, row in dfMPITotal.iterrows():
            tseq = dfMPITotal[row.index[i]][1]
            tpar = dfMPITotal[row.index[i]][P]
            speedupMPITotal[str(row.index[i])].append(tseq/tpar)

    for i in range(4):
        for P, row in dfThreadTotal.iterrows():
            tseq = dfThreadTotal[row.index[i]][1]
            tpar = dfThreadTotal[row.index[i]][P]
            speedupThreadTotal[str(row.index[i])].append(tseq/tpar)

    dfSpeedupMPI = pandas.DataFrame(speedupMPITotal)
    dfSpeedupMPI.set_index("CPUs Number", inplace=True)
    dfSpeedupThread = pandas.DataFrame(speedupThreadTotal)
    dfSpeedupThread.set_index("CPUs Number", inplace=True)

    dfSpeedupMPI.to_csv("./csv/speedupMPI.csv")
    dfSpeedupThread.to_csv("./csv/speedupThread.csv")

    speedupBestThread = {"CPUs Number": [1, 2, 4, 8, 10, 12, 16, 20], "Best Speedup":[1, 2, 4, 8, 10, 12, 16, 20]}
    dfSpeedupBestThread = pandas.DataFrame(speedupBestThread)
    dfSpeedupBestThread.set_index("CPUs Number", inplace=True)

    speedupBestMPI = {"CPUs Number": [1, 10, 20, 40, 60, 80, 100, 120], "Best Speedup":[1, 10, 20, 40, 60, 80, 100, 120]}
    dfSpeedupBestMPI = pandas.DataFrame(speedupBestMPI)
    dfSpeedupBestMPI.set_index("CPUs Number", inplace=True)
    
    graphSpeedup = dfSpeedupMPI.plot(kind='line')
    graphSpeedup.set_ylim(0, 120)
    dfSpeedupBestMPI.plot(kind='line', ax=graphSpeedup, linestyle="--")
    graphSpeedup.set_title('MPI Speedup', pad=20, fontdict={"fontsize":16,"fontweight":"bold"})
    graphSpeedup.set_xlabel("CPUs Number", labelpad=20, fontsize=12, fontweight="semibold")
    graphSpeedup.set_ylabel("Speedup", labelpad=20, fontsize=12, fontweight="semibold")

    plt.legend(loc='center left', bbox_to_anchor=(1.0, 0.5))
    plt.axis("scaled")
    graphSpeedup.get_figure().savefig("./graphs/speedupMPI.pdf", bbox_inches='tight')

    graphSpeedup = dfSpeedupThread.plot(kind='line')
    dfSpeedupBestThread.plot(kind='line', ax=graphSpeedup, linestyle="--")
    graphSpeedup.set_title('Thread Speedup', pad=20, fontdict={"fontsize":16,"fontweight":"bold"})
    graphSpeedup.set_xlabel("CPUs Number", labelpad=20, fontsize=12, fontweight="semibold")
    graphSpeedup.set_ylabel("Speedup", labelpad=20, fontsize=12, fontweight="semibold")

    plt.legend(loc='center left', bbox_to_anchor=(1.0, 0.5))
    plt.axis("scaled")

    graphSpeedup.get_figure().savefig("./graphs/speedupThread.pdf", bbox_inches='tight')



main()