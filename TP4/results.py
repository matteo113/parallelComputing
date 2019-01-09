#!/usr/bin/env python3

import pandas
import re
import os

def extractValuesFromFiles():

    resultsMapLoop = {1:{}, 2:{}, 10:{}, 20:{}, 40:{}, 60:{}, 80:{}, 100:{}, 120:{}}
    resultsMapWrite = {1:{}, 2:{}, 10:{}, 20:{}, 40:{}, 60:{}, 80:{}, 100:{}, 120:{}}
    resultsMapTotal = {1:{}, 2:{}, 10:{}, 20:{}, 40:{}, 60:{}, 80:{}, 100:{}, 120:{}}

    try:
        for filename in os.listdir("./results"):
            with open("./results/"+filename, 'r') as myfile:

                data=myfile.read().replace('\n', '')

                # Extract values from files
                extractedNumbers = re.findall("\d+\.\d+", data)
                loopTime = extractedNumbers[0]
                writeTime = extractedNumbers[1]
                totalTime = extractedNumbers[2]

                pattern = re.findall("\d+x\d+x\d+x\d+", filename)[0]

                params = pattern.split("x")

                sizeParam = params[0]
                iterationsParam = params[2]
                nProcParam = params[3]

                resultsMapLoop[int(nProcParam)][int(sizeParam)]  = loopTime
                resultsMapWrite[int(nProcParam)][int(sizeParam)] = writeTime
                resultsMapTotal[int(nProcParam)][int(sizeParam)] = totalTime

        return (resultsMapLoop, resultsMapWrite, resultsMapTotal)

    except FileNotFoundError as err:
        print("Error : "+err)


loopDatas, writeDatas, totalDatas = extractValuesFromFiles()

resultsFrame = pandas.DataFrame(loopDatas).transpose()
resultsFrame.to_csv("loopDatas.csv")

resultsFrame = pandas.DataFrame(writeDatas).transpose()
resultsFrame.to_csv("writeDatas.csv")

resultsFrame = pandas.DataFrame(totalDatas).transpose()
resultsFrame.to_csv("totalDatas.csv")
