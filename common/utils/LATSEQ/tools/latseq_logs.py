#!/usr/bin/python3

# -----------------------------------------------------------
# Process latseq logs to get statistics and visualization
#
# (C) 2020 Flavien Ronteix--Jacquet, Lannion, France
# Released under MIT License
# email flavien.ronteixjacquet@orange.com
# -----------------------------------------------------------

"""Process latseq logs module

This modules is used to process latseq logs and provides
some useful statistics and stats

Example:
    ./latseq_logs.py -l /home/flavien/latseq.21042020.lseq

Attributes:
    none

TODO
    * stats at each point (time, number, sd,...)
    * find ALL in and out points (dynamically)
    * rebuild the path of a data
    * output a json more practical to use
    * APIify with flask to be called easily by the others modules
        https://programminghistorian.org/en/lessons/creating-apis-with-python-and-flask#creating-a-basic-flask-application

"""

import argparse
import re
import datetime
import operator
import statistics
# import math

#
# GLOBALS
#

#
# UTILS
#


def epoch_to_datetime(epoch: str) -> str:
    """Convert an epoch to datetime"""
    return datetime.datetime.fromtimestamp(
        float(epoch)).strftime('%Y-%m-%d %H:%M:%S.%f')


def dstamp_to_epoch(dstamptime: str) -> float:
    """Convert a dstamptime to float epoch"""
    return float(datetime.datetime.strptime(
        dstamptime, "%Y%m%d_%H%M%S.%f"
    ).timestamp())


def path_to_str(pathP: list) -> str:
    """Use to get a string representing a path from a list"""
    if len(pathP) < 1:
        return ""
    if len(pathP) < 2:
        return pathP[0]
    res = f"{pathP[0]} -> "
    for i in range(1, len(pathP) - 1):
        res += f"{pathP[i]} -> "
    return res + f"{pathP[-1]}"


#
# STRUCTURES
#
class latseq_log:
    """class for log processing associated to a log file

    Args:
        logpathP (str): path to the log file

    Attributes:
        logpath (str): path to the log file
        initialized (bool): become true when the __init__ is successfully done
        raw_inputs (:obj:`list` of :obj:`str`): list of lines from logpath file
        inputs (:obj:`list` of :obj:`str`): list of lines after a first pass
            of processing from raw_inputs
        dataids (:obj:`list` of :obj:`str`): list of dataids found in the logs
        points (:obj:`dict` of :obj:`list`): list of points
            with all connections
            possible associated to this point
        paths (:obj:`list` of :obj:`list`):
            list[0] is a list of all DownLink paths possibles
            list[1] is a list of all UpLink paths possibles
        timestamps (:obj:`list` of float): list of timestamps in the logs
    """
    def __init__(self, logpathP: str):
        self.logpath = logpathP
        self.initialized = False
        if not self.logpath:
            raise AssertionError("Error, no logpath provided")
        try:
            self.raw_inputs = list()
            self._read_log()
        except FileNotFoundError as e:
            raise e
        except IOError:
            raise IOError(f"Error in Reading {logpathP}")
        else:
            try:
                self.inputs = list()
                self.dataids = list()
                self._clean_log()
            except Exception:
                raise ValueError(f"Error in Cleaning or Filtering {logpathP}")
        try:
            self.points = dict()  # It is basically a graph
            self._build_points()
        except Exception:
            raise Exception("Error at getting points' name")
        try:
            self.paths = [[], []]
            self._build_paths()
        except Exception as e:
            raise e
        self.timestamps = list()
        self._build_timestamp()
        self.initialized = True

    def _read_file(self) -> str:
        """Read the content of the file pointed by logpath
        """
        try:
            with open(self.logpath, 'r') as f:
                print(f"[INFO] Reading {self.logpath} ...")
                return f.read()
        except IOError as e:
            print(f"[ERROR] on open({self.logpath})")
            print(e)
            raise e

    def _read_log(self):
        """Read log file `logpath` to fill up `raw_inputs` with cleaned string entries
        """
        for l in self._read_file().splitlines():
            if l:  # line is not empty
                # Match pattern
                # https://www.tutorialspoint.com/python/python_reg_expressions.htm
                if re.match(r'#.*$', l, re.M):
                    continue
                else:
                    tmp = l.split(' ')
                    # TODO : rendre dynamique cette valeur avec
                    # le format donne par le header
                    if len(tmp) < 4:
                        print(f"[WARNING] {l} is a malformed line")
                        continue
                    self.raw_inputs.append(tuple([
                        float(tmp[0]),
                        0 if tmp[1] == 'D' else 1,
                        tmp[2],
                        tmp[3]]))

    def _clean_log(self):
        """Clean logs from `raw_inputs` to `inputs`
        """
        # sort by timestamp
        self.raw_inputs.sort(key=operator.itemgetter(0))
        # patterns to detect
        match_ids = re.compile("([a-zA-Z]+)([0-9]+)")
        # match_emptyrnti = re.compile("rnti65535")
        for e in self.raw_inputs:
            # an entry is a timestamp, a direction,
            # an in point an out point, a size,
            # a list of glibal context data id and local data id

            # skip lines which matches the following re
            if re.search("rnti65535", e[3]):
                continue

            # process line
            try:
                e_points = e[2].split('--')
                dataids = e[3].split(':')
                if len(dataids) < 2:
                    dataids.insert(0, 0)
                    dataids.append('')
                if len(dataids) < 3:
                    continue
                ptmp = {}
                # properties values
                if dataids[0] != '':
                    for p in dataids[0].split('.'):
                        try:
                            dip = match_ids.match(p).groups()
                        except Exception:
                            continue
                        else:
                            ptmp[dip[0]] = dip[1]
                # global context ids
                ctmp = {}
                if dataids[1] != '':
                    for c in dataids[1].split('.'):
                        try:
                            # dic[0] is the global context identifier
                            # dic[1] the value associated
                            dic = match_ids.match(c).groups()
                        except Exception:
                            continue
                        else:
                            ctmp[dic[0]] = dic[1]
                            if dic[0] not in self.dataids:
                                self.dataids.append(dic[0])
                dtmp = {}
                # local context ids
                if dataids[2] != '':
                    for d in dataids[2].split('.'):
                        try:
                            # did[0] is the local context identifier
                            # did[1] the value associated
                            did = match_ids.match(d).groups()
                        except Exception:
                            continue
                        else:
                            dtmp[did[0]] = did[1]
                            if did[0] not in self.dataids:
                                self.dataids.append(did[0])
                self.inputs.append((
                        e[0],
                        e[1],
                        e_points[0],
                        e_points[1],
                        ptmp,
                        ctmp,
                        dtmp))
            except Exception:
                raise ValueError(f"Error at parsing line {e}")

    def _build_points(self):
        """Build graph of measurement `points` and find in and out points
        """
        # Build graph
        for e in self.raw_inputs:
            e_points = e[2].split('--')
            if e_points[0] not in self.points:
                # list of pointers and direction 0 for D and 1 for U
                self.points[e_points[0]] = []
            if e_points[1] not in self.points[e_points[0]]:
                # Get combinations of dest point
                destpt = e_points[1].split('.')
                for i in range(len(destpt)):
                    tmps = ""
                    j = 0
                    while j <= i:
                        tmps += f"{destpt[j]}."
                        j += 1
                    self.points[e_points[0]].append(tmps[:-1])
            if e_points[1] not in self.points:
                self.points[e_points[1]] = []
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
        self.pointsInD = ["ip"]
        self.pointsOutD = ["phy.out.proc"]
        self.pointsInU = ["phy.in.proc"]
        self.pointsOutU = ["ip"]

    def _build_paths(self):
        """Build all possible `paths` in the graph `points`
        """
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
        if len(self.paths[0]) == 0 and len(self.paths[1]) == 0:
            raise Exception("Error no paths found in Downlink nor in Uplink")
        elif len(self.paths[0]) == 0:
            raise Exception("Error, no path found in Downlink")
        elif len(self.paths[1]) == 0:
            raise Exception("Error, no path found in Uplink")

    def _build_timestamp(self):
        """Build `timestamps` a :obj:`list` of float of timestamp
        """
        self.timestamps = list(map(lambda x: x[0], self.raw_inputs))

    def rebuild_packets_journey(self):
        self.journeys = dict()
        self.out_journeys = list()
        if not self.initialized:
            try:
                self(self.logpath)
            except Exception:
                raise Exception("Impossible to rebuild packet because the module has not been initialized correctly")

        for p in self.inputs:  # for all input, try to build the journeys
            if p[1] == 0:  # Downlink
                tmpIn = self.pointsInD
                tmpOut = self.pointsOutD
            else:  # Uplink
                tmpIn = self.pointsInU
                tmpOut = self.pointsOutU

            if p[2] in tmpIn:  # this is a packet in arrival
                newid = len(self.journeys)
                self.journeys[newid] = dict()
                self.journeys[newid]['glob'] = p[5]  # global ids as a first filter
                self.journeys[newid]['ts_in'] = p[0]  # timestamp of arrival
                self.journeys[newid]['set'] = list()  # set measurements of ids in inputs for this journey
                self.journeys[newid]['set'].append(self.inputs.index(p))
                self.journeys[newid]['set_ids'] = dict()
                tmp_list = [f"id{newid}"]
                for g in p[5]:
                    tmp_list.append(f"{g}{p[5][g]}")
                for l in p[6]:
                    tmp_list.append(f"{l}{p[6][l]}")
                self.journeys[newid]['set_ids'][self.journeys[newid]['set'][-1]] = tmp_list
                self.journeys[newid]['next_points'] = self.points[p[2]]  # list of possible next points
                self.journeys[newid]['completed'] = False  # True if the journey is complete
            else:  # this packet should be already followed somewhere
                matched_key = None
                matched_ids_list = list()
                for i in self.journeys:  # for all keys (ids) in journeys
                    matched_ids_list = []
                    if self.journeys[i]['completed']:  # if the journey, not necessary to go further
                        continue  # for i in self.journeys
                    match_glob = True
                    for k in p[5]:  # for all global ids, first filter
                        if k in self.journeys[i]['glob']:
                            if p[5][k] != self.journeys[i]['glob'][k]:
                                match_glob = False
                                break  # for i in self.journeys
                        # else: Case where drb1 was not addes at the beginning
                        #     match_glob = False
                    if not match_glob:  # global ids do not match
                        continue  # for i in self.journeys

                    if not p[2] in self.journeys[i]['next_points']:  # not expected next measurement point
                        continue  # for i in self.journeys

                    last_lids_list = self.inputs[self.journeys[i]['set'][-1]][6]
                    match_local = True
                    for k_lid in p[6]:  # for all local ids in measurement point
                        if k_lid in last_lids_list:  # if the local ids are present in the 2 points
                            if p[6][k_lid] != last_lids_list[k_lid]:  # the local id k_lid do not match
                                match_local = False
                            else:
                                matched_ids_list.append(f"{k_lid}{p[6][k_lid]}")
                    if not match_local:
                        continue

                    matched_key = i
                    break

                # At this point we should have the good i for this packet
                # Then add id
                if matched_key is None:  # We cant add this input to a journey
                    continue  # continue to the next input
                
                tmp_list = list(self.journeys[matched_key]['set_ids'][self.journeys[matched_key]['set'][-1]])
                self.journeys[matched_key]['set'].append(self.inputs.index(p))
                
                tmp_list.extend(x for x in matched_ids_list if x not in tmp_list)
                self.journeys[matched_key]['set_ids'][self.journeys[matched_key]['set'][-1]] = tmp_list
                if self.points[p[3]] in tmpOut:  # this is the last input before the great farewell
                    self.journeys[matched_key]['next_points'] = None
                    self.journeys[matched_key]['ts_out'] = p[0]
                    self.journeys[matched_key]['completed'] = True
                else:  # this is not the last input
                    self.journeys[matched_key]['next_points'] = self.points[p[2]]

        for j in self.journeys:  # retrieves all journey to build out_journeys
            for e in self.journeys[j]['set']: # for all elements in set of ids
                e_tmp = self.inputs[e]
                self.out_journeys.append((
                    e_tmp[0],
                    e_tmp[1],
                    f"{e_tmp[2]}--{e_tmp[3]}",
                    e_tmp[4],
                    '.'.join(self.journeys[j]['set_ids'][e])
                ))
        for e in self.yield_clean_inputs():
            print(e)


    # GETTERS
    def get_list_of_points(self) -> list:
        return list(self.points.keys())

    def get_list_timestamp(self) -> list:
        if not self.timestamps:
            self._build_timestamp()
        return self.timestamps

    def get_log_file_stats(self) -> dict:
        return {
            "name": self.logpath,
            "nb_entries": len(self.raw_inputs),
            "points": self.get_list_of_points()
            }

    def get_paths(self) -> tuple:
        if len(self.paths[0]) == 0 and len(self.paths[1]) == 0:
            self._build_paths()
        return (self.paths[0], self.paths[1])

    def yield_clean_inputs(self):
        try:
            for e in self.out_journeys:
                yield f"{e[0]} {e[1]} (len{e[3]['len']}) {e[2]} {e[4]}"
        except Exception as e:
            raise e

    def yield_journeys(self):
        try:
            for j in self.journeys:
                yield j
        except Exception as e:
            raise e

    # STRINGIFY
    def print_paths(self) -> str:
        res = f"Paths found in {self.logpath} \n"
        i, j = 0, 0
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
                j += 1
            i += 1
        return res


#
# STATISTICS
#
class latseq_stats:
    """Class of static methods for statistics stuff for latseq
    """
    @staticmethod
    def mean_separation_time(tsLP: list) -> float:
        """Function to return means time separation between logs

        Args:
            TsLP (:obj:`list` of float): the list of timestamp

        Returns:
            float : mean time separation between log entries

        Raises:
            ValueError: The len of list is < 2
        """
        if len(tsLP) < 2:
            raise ValueError("The length of tsLP is inferior to 2")
        tmp = list()
        for i in range(len(tsLP)-1):
            tmp.append(abs(tsLP[i+1]-tsLP[i]))
        return statistics.mean(tmp)

#
# MAIN
#

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
            lseq.rebuild_packets_journey()
