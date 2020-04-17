#!/usr/bin/python3
import argparse
import re
import datetime
import operator
import statistics
#import math

# ./latseq_logs.py -l /home/flavien/latseq.16042020.lseq

# TODO
# stats at each point (time, number, sd,...)
# rebuild the path of a data
# output a json less rough to use

### GLOBALS ###

### UTILS ###
def epoch_to_datetime(epoch: str) -> str:
    return datetime.datetime.fromtimestamp(float(epoch)).strftime('%Y-%m-%d %H:%M:%S.%f')

def dstamp_to_epoch(dstamptime: str) -> float:
    return float(datetime.datetime.strptime(
        dstamptime, "%Y%m%d_%H%M%S.%f"
    ).timestamp())

def path_to_str(pathP: list) -> str:
    if len(pathP) < 1:
        return ""
    if len(pathP) < 2:
        return pathP[0]
    res = f"{pathP[0]} -> "
    for i in range(1, len(pathP) - 1):
        res += f"{pathP[i]} -> "
    return res + f"{pathP[-1]}"
        

### STRUCTURES ###
class latseq_log:
    def __init__(self, filenameP : str):
        self.filename       = filenameP
        self.initialized    = False
        if not self.filename:
            print("[ERROR] No filename provided")
            raise
        try:
            self.raw_inputs = list()
            self._read_log()
            self.inputs = list()
            self.dataids = []
            self._clean_log()
        except:
            print(f"[ERROR] at reading and filtering {filenameP}")
            exit
        try:
            self.points = dict() # It is basically a graph
            self._build_points()
        except:
            print("[ERROR] at yielding points' name")
        self.paths = [[],[]]
        self._build_paths()
        self.timestamps = list()
        self._build_timestamp()
        self.initialized = True

    def _read_file(self) -> str:
        try:
            with open(self.filename, 'r') as f:
                print(f"[INFO] Reading {self.filename} ...")
                return f.read()
        except IOError as e:
            print(f"[ERROR] on open({self.filename})")
            print(e)
            raise e

    def _read_log(self):
        for l in self._read_file().splitlines():
            if l: # line is not empty
                # Match pattern https://www.tutorialspoint.com/python/python_reg_expressions.htm
                if re.match(r'#.*$', l, re.M):
                    continue
                else:
                    tmp = l.split(' ')
                    if len(tmp) < 4: #TODO : rendre dynamique cette valeur avec le format donne par le header
                        print(f"[WARNING] {l} is a malformed line")
                        continue
                    self.raw_inputs.append(
                        tuple(
                            [float(tmp[0]),
                            0 if tmp[1] == 'D' else 1,
                            tmp[2],
                            tmp[3]]
                            )
                        )
    
    def _clean_log(self):
        #sort by timestamp
        self.raw_inputs.sort(key = operator.itemgetter(0))
        matchids = re.compile("([a-zA-Z]+)([0-9]+)") 
        for e in self.raw_inputs:
            #an entry is a tiestamp, a direction, an in pointm an out point, a size, a list of common data id and specific data id
            try:
                e_points = e[2].split('--')
                dataids = e[3].split(':')
                if len(dataids) < 2:
                    dataids.insert(0, 0)
                    dataids.append('')
                if len(dataids) < 3:
                    dataids.insert(0, 0)
                #if len(dataids) == 3:
                #    dataids.pop(0)
                ctmp = {}
                if dataids[1] != '':
                    for c in dataids[1].split('.'):
                        try:
                            dic = matchids.match(c).groups()
                        except:
                            continue
                        else:
                            ctmp[dic[0]] = dic[1]
                            if dic[0] not in self.dataids:
                                self.dataids.append(dic[0])
                dtmp = {}
                if dataids[2] != '':
                    for d in dataids[2].split('.'):
                        try:
                            did = matchids.match(d).groups()
                        except:
                            continue
                        else:
                            ctmp[did[0]] = did[1]
                            if did[0] not in self.dataids:
                                self.dataids.append(did[0])
                self.inputs.append((e[0], e[1], e_points[0], e_points[1], dataids[0], ctmp, dtmp))
            except:
                print(f"[ERROR] at parsing line {e}")
                raise
    
    def _build_points(self):
        # Build graph
        for e in self.raw_inputs:
            e_points = e[2].split('--')
            if e_points[0] not in self.points:
                self.points[e_points[0]] = [] # list of pointers and direction 0 for D and 1 for U
            if e_points[1] not in self.points[e_points[0]]:
                # Get combinations of dest point
                destpt = e_points[1].split('.')
                for i in range(len(destpt)):
                    tmps = ""
                    j=0
                    while j<=i:
                        tmps += f"{destpt[j]}."
                        j+=1
                    self.points[e_points[0]].append(tmps[:-1])
        # Find in et out points
        # tmpD = [x[0] for x,y in self.points if y[1]==0]
        # tmpDin = tmpD
        # tmpDout = []
        # tmpU = [x[0] for x in self.points if x[1]==1]
        # tmpUin = tmpU
        # tmpUout = []
        # for p in self.points:
        #     # case D
        #     if p[1] == 0:
        #         # if not pointed by anyone, then, it is the input
        #         for e in p[0]:
        #             tmpDin.remove(e)
        #         # if pointed but not in keys, it is the output
        #             if e not in tmpD:
        #                 tmpDout.append(e)
        #     elif p[1] == 1:
        #         # if not pointed by anyone, then, it is the input
        #         for e in p[0]:
        #             tmpUin.remove(e)
        #         # if pointed but not in keys, it is the output
        #             if e not in tmpU:
        #                 tmpUout.append(e)
        #     else:
        #         print(f"[ERROR] Unknown direction for {p[0]} : {p[1]}")
        # self.pointsInD  = tmpDin
        # self.pointsOutD = tmpDout
        # self.pointsInU  = tmpUin
        # self.pointsOutU = tmpUout
        self.pointsInD  = ["ip"]
        self.pointsOutD = ["mac.mux"]
        self.pointsInU  = ["phy.in.proc"]
        self.pointsOutU = ["ip"]
    
    def _build_paths(self):
        def _find_all_paths(graphP: dict, startP: str, endP: str, pathP=[]):
            tmppath = pathP + [startP]
            if startP == endP:
                return [tmppath]
            if startP not in graphP:
                return []
            paths = []
            for p in graphP[startP]:
                if p not in tmppath:
                    newpaths = _find_all_paths(graphP, p, endP, tmppath)
                    for newpath in newpaths:
                        paths.append(newpath)
            return paths
        # build downlink paths
        for i in self.pointsInD:
            for o in self.pointsOutD:
                self.paths[0] = _find_all_paths(self.points, i, o)
        for i in self.pointsInU:
            for o in self.pointsOutU:
                self.paths[1] = _find_all_paths(self.points, i, o)

    def _build_timestamp(self):
        self.timestamps = list(map(lambda x: x[0], self.raw_inputs))

    # GETTERS
    def get_list_of_points(self) -> list:
        return list(self.points.keys())
    
    def get_list_timestamp(self) -> list:
        if not self.timestamps:
            self._build_timestamp()
        return self.timestamps
    
    def get_log_file_stats(self) -> dict:
        return { 
            "name" : self.filename,
            "nb_entries" : len(self.raw_inputs),
            "points" : self.get_list_of_points()
            }
    
    def get_paths(self) -> tuple:
        if len(self.paths[0]) == 0 and len(self.paths[1]) == 0:
            self._build_paths()
        return (self.paths[0], self.paths[1])

    # STRINGIFY
    def print_paths(self) -> str:
        res = f"Paths found in {self.filename} \n"
        i,j = 0, 0
        for d in self.get_paths():
            if i == 0:
                res += "Downlink paths\n"
            if i == 1:
                res += "Uplink paths\n"
            for p in d:
                if p:
                    res += f"\tpath {j} : "
                    res += path_to_str(p)
                    res += "\n"
                j+=1
            i+=1  
        return res  
    
### STATISTICS ###
class latseq_stats:
    @staticmethod
    def mean_separation_time(tsLP : list):
        if len(tsLP) < 2 :
            print("[ERROR] No stats possible")
            return 0
        tmp = list()
        for i in range(len(tsLP) -1):
            tmp.append(abs(tsLP[i+1]-tsLP[i]))
        return statistics.mean(tmp)



### MAIN ###
if __name__ == "__main__":
    parser = argparse.ArgumentParser("LatSeq log processing")
    parser.add_argument(
        "-l",
        "--log",
        type=str,
        dest="logname",
        help="Log file",
        required=True
    )

    args = parser.parse_args()
    
    if args.logname:
        try:
            lseq = latseq_log(args.logname)
        except Exception as e:
            raise(e)
            print(f"[ERROR] On creating a lseq element {args.logname}")
            exit()
        else:
            print(latseq_stats.mean_separation_time(lseq.get_list_timestamp()))
            print(lseq.get_list_of_points())
            print(lseq.print_paths())

