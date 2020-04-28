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
    * Rebuild_packet with multithreading because the algorithmc complexity is huge...

"""

import argparse
import re
import datetime
import operator
import statistics
from copy import deepcopy
import pickle
# import math

#
# GLOBALS
#

# trick to reduce complexity
# Asumption : packet spend at maximum DEPTH_TO_SEARCH_PKT measure in the system
DEPTH_TO_SEARCH_PKT = 500
DEPTH_TO_SEARCH_FORKS = 20
# TODO : Too larg, I match between um and am

S_TO_MS = 1000
#
# UTILS
#


def epoch_to_datetime(epoch: float) -> str:
    """Convert an epoch to datetime"""
    return datetime.datetime.fromtimestamp(
        epoch).strftime('%Y%m%d_%H%M%S.%f')


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

def dict_ids_to_str(idsP: dict) -> str:
    return '.'.join([f"{k}{v}" for k,v in idsP.items()])


def make_immutable_list(listP: list) -> tuple:
    return tuple(listP)


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
            self.points = dict()  # "next" is basically a graph
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
                            if did[0] not in dtmp:
                                dtmp[did[0]] = did[1]
                            else:  # case we have multiple value for the same id
                                if isinstance(dtmp[did[0]], list):
                                    dtmp[did[0]].append(did[1])
                                else:
                                    tmpl = [dtmp[did[0]], did[1]]
                                    del dtmp[did[0]]
                                    dtmp[did[0]] = tmpl
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
        self.inputs = make_immutable_list(self.inputs)

    def _build_points(self):
        """Build graph of measurement `points` and find in and out points
        """
        # Build graph
        for e in self.raw_inputs:
            e_points = e[2].split('--')
            if e_points[0] not in self.points:
                # list of pointers and direction 0 for D and 1 for U
                self.points[e_points[0]] = {}
                self.points[e_points[0]]['next'] = []
                self.points[e_points[0]]['count'] = 0
                self.points[e_points[0]]['dir'] = [e[1]]
            if e_points[1] not in self.points[e_points[0]]['next']:
                # Get combinations of dest point
                destpt = e_points[1].split('.')
                for i in range(len(destpt)):
                    tmps = ""
                    j = 0
                    while j <= i:
                        tmps += f"{destpt[j]}."
                        j += 1
                    self.points[e_points[0]]['next'].append(tmps[:-1])
            if e_points[1] not in self.points:
                self.points[e_points[1]] = {}
                self.points[e_points[1]]['next'] = []
                self.points[e_points[1]]['count'] = 1
                self.points[e_points[1]]['dir'] = [e[1]]
            self.points[e_points[0]]['count'] += 1
            if e[1] not in self.points[e_points[0]]['dir']:
                self.points[e_points[0]]['dir'].append(e[1])
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
        self.pointsInD = ["ip", "rlc.tx.am"]
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
            for p in graphP[startP]['next']:
                if p not in tmppath:
                    newpaths = _find_all_paths(graphP, p, endP, tmppath)
                    for newpath in newpaths:
                        paths.append(newpath)
            return paths
        # build downlink paths
        for i in self.pointsInD:
            for o in self.pointsOutD:
                self.paths[0].extend(_find_all_paths(self.points, i, o))
        for i in self.pointsInU:
            for o in self.pointsOutU:
                self.paths[1].extend(_find_all_paths(self.points, i, o))
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

    def rebuild_packets_journey_seq(self):
        """Rebuild the packets journey sequentially from a list of measure
        Algorithm:
            Process each packet sequentially and try to put into journey
        Attributes:
            journeys (:obj:`dict`): the dictionnary containing dictionnaries
            out_journeys (:obj:`list`): the list of journeys prepare for output
        """
        def _measure_ids_in_journey(p_gids: list, p_lids: list, j_gids: list, j_last_element: dict) -> dict:
            """Returns the dict of common identifiers if the measure is in the journey
            Otherwise returns an empty dictionnary
            """
            # for all global ids, first filter
            for k in p_gids:
                if k in j_gids:
                    if p_gids[k] != j_gids[k]:
                        return {}  # False
                else:  # The global context id is not in the contet of this journey, continue
                    return {}  # False
            res_matched = {}
            # for all local ids in measurement point
            for k_lid in p_lids:
                if k_lid in j_last_element[6]:  # if the local ids are present in the 2 points
                    # Case : multiple value for the same identifier
                    if isinstance(j_last_element[6][k_lid], list):
                        match_local_in_list = False
                        for v in j_last_element[6][k_lid]:
                            if p_lids[k_lid] == v:  # We want only one matches the id
                                match_local_in_list = True
                                res_matched[k_lid] = v
                                # remove the multiple value for input to keep only the one used
                                j_last_element[6][k_lid] = v
                                break  # for v in j_last_lids[k_lid]
                        if not match_local_in_list:
                            return {}
                    # Case : normal case, one value per identifier
                    else:
                        if p_lids[k_lid] != j_last_element[6][k_lid]:  # the local id k_lid do not match
                            return {}
                        else:
                            res_matched[k_lid] = p_lids[k_lid]
            return res_matched

        self.journeys = dict()
        self.out_journeys = list()
        if not self.initialized:
            try:
                self(self.logpath)
            except Exception:
                raise Exception("Impossible to rebuild packet because the module has not been initialized correctly")
        
        total_i = len(self.inputs)
        current_i = 0
        for p in self.inputs:  # for all input, try to build the journeys
            current_i += 1
            if current_i % 100 == 0:
                print(f"{current_i} / {total_i}")
            # if current_i > 3000:
            #     break
            # p[0] float : ts
            # p[1] int : direction
            # p[2] str : src point
            # p[3] str : dst point
            # p[4] dict : properties ids
            # p[5] dict : global ids
            # p[6] dict : local ids
            if p[1] == 0:  # Downlink
                tmpIn = self.pointsInD
                tmpOut = self.pointsOutD
            else:  # Uplink
                tmpIn = self.pointsInU
                tmpOut = self.pointsOutU

            if p[2] in tmpIn:  # this is a packet in arrival, create a new journey
                newid = len(self.journeys)
                self.journeys[newid] = dict()
                self.journeys[newid]['dir'] = p[1]
                self.journeys[newid]['glob'] = p[5]  # global ids as a first filter
                self.journeys[newid]['ts_in'] = p[0]  # timestamp of arrival
                self.journeys[newid]['set'] = list()  # set measurements of ids in inputs for this journey
                self.journeys[newid]['set'].append(self.inputs.index(p))
                self.journeys[newid]['set_ids'] = dict()
                # tmp_list = [f"uid{newid}"]
                # for l in p[6]:
                #     tmp_list.append(f"{l}{p[6][l]}")
                self.journeys[newid]['set_ids'] = {'uid': newid}
                self.journeys[newid]['set_ids'].update(p[6])
                self.journeys[newid]['next_points'] = self.points[p[2]]['next']  # list of possible next points
                self.journeys[newid]['last_point'] = p[2]
                self.journeys[newid]['completed'] = False  # True if the journey is complete
            else:  # this packet should be already followed somewhere
                matched_key = None
                matched_ids = dict()
                matched_seg = False
                seg_new_journey = dict()
                for i in self.journeys:  # for all keys (ids) in journeys
                    # Case : journey already completed
                    # Assumption : no segmentation at the out points
                    if self.journeys[i]['completed']:
                        continue  # for i in self.journeys

                    # Case : wrong direction
                    if p[1] != self.journeys[i]['dir']:
                        continue  # for i in self.journeys

                    # Case : segmentation
                    # False Asumption : all the segmentation will occurs before the measure of the next point
                    if p[2] == self.journeys[i]['last_point']:
                        if len(self.journeys[i]['set']) > 1:
                            matched_ids = _measure_ids_in_journey(
                                p[5],
                                p[6],
                                self.journeys[i]['glob'],
                                self.inputs[self.journeys[i]['set'][-2]])
                            if matched_ids:  # this is a fork from the point before the last point, a segmentation
                                matched_seg = True
                                seg_new_journey = deepcopy(self.journeys[i]) # deep copy the journey
                                seg_new_journey['set'].pop() # we remove the last element of the set because we forked
                                # TODO: what to do when the value is exactly the same ?
                                seg_new_journey['set'].append(self.inputs.index(p))
                                seg_new_journey['set_ids'].update(matched_ids)
                                seg_new_journey['last_point'] = p[2]
                                break  # for i in self.journeys

                    # Case : not expected next measurement point
                    if self.journeys[i]['next_points'] is not None and not p[2] in self.journeys[i]['next_points']:
                        continue  # for i in self.journeys

                    # if len(p[5]) > 0:  # case where global context is irrelevant, lower layers
                    # Case : packet as candidate to join this journey
                    matched_ids = _measure_ids_in_journey(
                        p[5],
                        p[6],
                        self.journeys[i]['glob'],
                        self.inputs[self.journeys[i]['set'][-1]])
                    if not matched_ids:  # No match
                        continue  # for i in self.journeys

                    matched_key = i
                    break

                # Case : A segmentation has been found,
                # add seg_new_journeys to the dict
                if matched_seg:
                    newidseg = len(self.journeys)
                    self.journeys[newidseg] = seg_new_journey
                    continue # for p in self.inputs

                # Case : This measure could not be added to a journey
                if matched_key is None:
                    continue  # for p in self.inputs
                
                # At this point, we have a journey id to add this measure
                self.journeys[matched_key]['set'].append(self.inputs.index(p))
                self.journeys[matched_key]['set_ids'].update(matched_ids)
                
                if p[3] in tmpOut:  # this is the last input before the great farewell
                    self.journeys[matched_key]['next_points'] = None
                    self.journeys[matched_key]['ts_out'] = p[0]
                    self.journeys[matched_key]['completed'] = True
                else:  # this is not the last input
                    self.journeys[matched_key]['next_points'] = self.points[p[2]]['next']
                    self.journeys[matched_key]['last_point'] = p[2]

        # retrieves all journey to build out_journeys
        for j in self.journeys:
            # Case : The journey is incomplete
            if not self.journeys[j]['completed']:
                continue
            for e in self.journeys[j]['set']: # for all elements in set of ids
                e_tmp = self.inputs[e]
                tmp_str = f"uid{j}.{dict_ids_to_str(self.journeys[j]['glob'])}.{dict_ids_to_str(e_tmp[6])}"
                self.out_journeys.append((
                    epoch_to_datetime(e_tmp[0]),
                    'D' if e_tmp[1] == 0 else 'U',
                    f"{e_tmp[2]}--{e_tmp[3]}",
                    e_tmp[4],
                    tmp_str))
        try:
            with open("latseq.lseqj", 'w+') as f:
                print(f"[INFO] Writing latseq.lseqj ...")
                for e in self.yield_clean_inputs():
                    f.write(f"{e}\n")
        except IOError as e:
            print(f"[ERROR] on open({self.logpath})")
            print(e)
            raise e

    def rebuild_packets_journey_recursively(self):
        """Rebuild the packets journey from a list of measure recursively
        Algorithm:
            for each input packet, try to rebuild the journey
        Attributes:
            journeys (:obj:`dict`): the dictionnary containing journeys
                journeys[i] (:obj:`dict`): a journey
                    journeys[i]['dir'] (int): 0 if a Downlink journey, 1 otherwise
                    journeys[i]['glob'] (:obj:`dict`): the globals context ids to match necessary
                    journeys[i]['completed'] (bool): True if the journey is compete, e.g. journey from an in to an out point
                    journeys[i]['ts_in'] (float): timestamp at which the journey begins
                    journeys[i]['ts_out'] (float): timestamp at which the journey ends if `completed`
                    journeys[i]['next_points'] (:obj:`list`): the next points' identifier expected
                    journeys[i]['set'] (:obj:`list`): list of measures in `input` corresponding to this journey
                    journeys[i]['set_ids'] (:obj:`list`): the last measurement point identifier added
            out_journeys (:obj:`list`): the list of measurement point like `raw_inputs` but ordered and filtered
        """
        self.journeys = dict()
        self.out_journeys = list()
        if not self.initialized:
            try:
                self(self.logpath)
            except Exception:
                raise Exception("Impossible to rebuild packet because the module has not been initialized correctly")
        
        total_i = len(self.inputs)
        current_i = 0
        nb_meas = len(self.inputs)
        list_meas = list(range(nb_meas))
        pointer = 0
        local_pointer = 0

        def _measure_ids_in_journey(p_gids: list, p_lids: list, j_gids: list, j_last_element: dict) -> dict:
            """Returns the dict of common identifiers if the measure is in the journey
            Otherwise returns an empty dictionnary
            """
            # for all global ids, first filter
            for k in p_gids:
                if k in j_gids:
                    if p_gids[k] != j_gids[k]:
                        return {}  # False
                else:  # The global context id is not in the contet of this journey, continue
                    return {}  # False
            res_matched = {}
            # for all local ids in measurement point
            for k_lid in p_lids:
                if k_lid in j_last_element[6]:  # if the local ids are present in the 2 points
                    # Case : multiple value for the same identifier
                    if isinstance(j_last_element[6][k_lid], list):
                        match_local_in_list = False
                        for v in j_last_element[6][k_lid]:
                            if p_lids[k_lid] == v:  # We want only one matches the id
                                match_local_in_list = True
                                res_matched[k_lid] = v
                                # remove the multiple value for input to keep only the one used
                                j_last_element[6][k_lid] = v
                                break  # for v in j_last_lids[k_lid]
                        if not match_local_in_list:
                            return {}
                    # Case : normal case, one value per identifier
                    else:
                        if p_lids[k_lid] != j_last_element[6][k_lid]:  # the local id k_lid do not match
                            return {}
                        else:
                            res_matched[k_lid] = p_lids[k_lid]
            return res_matched

        def _get_next(listP: list, endP: int, pointerP: int) -> int:
            pointerP += 1
            while pointerP not in listP and pointerP < endP:
                pointerP += 1
            return pointerP

        def _rec_rebuild(pointerP: int, local_pointerP: int, parent_journey_id: int):
            # rebuild its path
            max_local_pointer = min(pointerP + DEPTH_TO_SEARCH_PKT, nb_meas)
            while not self.journeys[parent_journey_id]['completed'] and local_pointerP < max_local_pointer:
                # print(f"{pointerP}:{local_pointerP}")
                tmp_p = self.inputs[local_pointerP]

                # Case : wrong direction
                if tmp_p[1] != self.journeys[parent_journey_id]['dir']:
                    local_pointerP = _get_next(list_meas, nb_meas, local_pointerP)
                    continue
                
                # Case : the measurement point is too far away
                # and tmp_p[2] not in self.journeys[parent_journey_id]['last_points']
                if tmp_p[2] not in self.journeys[parent_journey_id]['next_points']:
                    local_pointerP = _get_next(list_meas, nb_meas, local_pointerP)
                    continue

                # Case : the measurement point is an input
                if tmp_p[1] == 0:  # Downlink
                    if tmp_p[2] in self.pointsInD:
                        local_pointerP = _get_next(list_meas, nb_meas, local_pointerP)
                        continue
                else:  # Uplink
                    if tmp_p[2] in self.pointsInU:
                        local_pointerP = _get_next(list_meas, nb_meas, local_pointerP)
                        continue


                # Case : Concatenation
                # TODO : gestion de la concatenation

                # Case : Normal
                # Here get the first occurence who is matching
                matched_ids = _measure_ids_in_journey(
                    tmp_p[5],
                    tmp_p[6],
                    self.journeys[parent_journey_id]['glob'],
                    self.inputs[self.journeys[parent_journey_id]['set'][-1]]
                )
                if not matched_ids:
                    local_pointerP = _get_next(list_meas, nb_meas, local_pointerP)
                    continue
                #if pointer == 246:
                #    print("246")
                # Case : find a match
                print(f"Add {local_pointerP} to {parent_journey_id}")
                if local_pointerP not in list_meas:
                    print(f"error at removing : {local_pointerP}")
                else:
                    list_meas.remove(local_pointerP)
                seg_local_pointer = local_pointerP
                local_pointerP = _get_next(list_meas, nb_meas, local_pointerP)
                # Case : search for segmentation
                # Find all forks possible
                # increases a lot the complexity
                max_seg_pointer = min(local_pointerP + DEPTH_TO_SEARCH_FORKS, nb_meas - 1)
                while seg_local_pointer < max_seg_pointer:
                    seg_local_pointer = _get_next(list_meas, nb_meas, seg_local_pointer)
                    seg_tmp_p = self.inputs[seg_local_pointer]
                    if seg_tmp_p[1] != self.journeys[parent_journey_id]['dir']:
                        continue
                    if seg_tmp_p[2] != tmp_p[2]:
                        continue
                    seg_matched_ids = _measure_ids_in_journey(
                        seg_tmp_p[5],
                        seg_tmp_p[6],
                        self.journeys[parent_journey_id]['glob'],
                        self.inputs[self.journeys[parent_journey_id]['set'][-1]])
                    # Case : find a segmentation
                    if seg_matched_ids:
                        # Create a new path
                        # TODO: what to do when the value is exactly the same ?
                        segid = len(self.journeys)
                        self.journeys[segid] = deepcopy(self.journeys[parent_journey_id])
                        self.journeys[segid]['set_ids']['uid'] = segid
                        self.journeys[segid]['set'].append(self.inputs.index(seg_tmp_p))
                        self.journeys[segid]['set_ids'].update(matched_ids)
                        print(f"Add {seg_local_pointer} to {segid}")
                        list_meas.remove(seg_local_pointer)
                        if seg_tmp_p[3] in tmpOut:  # this is the last input before the great farewell
                            self.journeys[segid]['next_points'] = None
                            self.journeys[segid]['ts_out'] = seg_tmp_p[0]
                            self.journeys[segid]['completed'] = True
                            continue
                        self.journeys[segid]['next_points'] = self.points[seg_tmp_p[2]]['next']
                        # self.journeys[segid]['last_points'].append(seg_tmp_p[2])
                        seg_local_pointer_next = _get_next(list_meas, nb_meas, seg_local_pointer)
                        pointerP, seg_local_pointer = _rec_rebuild(seg_local_pointer, seg_local_pointer_next, segid)
                        pointerP = _get_next(list_meas, nb_meas, pointerP)


                # At this point, we have completed all the possible fork
                self.journeys[parent_journey_id]['set'].append(self.inputs.index(tmp_p))
                self.journeys[parent_journey_id]['set_ids'].update(matched_ids)
            
                if tmp_p[3] in tmpOut:  # this is the last input before the great farewell
                    self.journeys[parent_journey_id]['next_points'] = None
                    self.journeys[parent_journey_id]['ts_out'] = tmp_p[0]
                    self.journeys[parent_journey_id]['completed'] = True
                    continue

                self.journeys[parent_journey_id]['next_points'] = self.points[tmp_p[2]]['next']
                # self.journeys[parent_journey_id]['last_points'].append(tmp_p[2])
            return (pointerP, local_pointerP)

        # for all input, try to build the journeys
        while pointer < nb_meas:
            current_i += 1
            # if current_i % 100 == 0:
            #     print(f"{current_i} / {total_i}")
            # if pointer > 2000:
            #     break
            # p[0] float : ts
            # p[1] int : direction
            # p[2] str : src point
            # p[3] str : dst point
            # p[4] dict : properties ids
            # p[5] dict : global ids
            # p[6] dict : local ids
            p = self.inputs[pointer]
            if p[1] == 0:  # Downlink
                tmpIn = self.pointsInD
                tmpOut = self.pointsOutD
            else:  # Uplink
                tmpIn = self.pointsInU
                tmpOut = self.pointsOutU

            if p[2] not in tmpIn:
                pointer += 1
                while pointer not in list_meas and pointer < nb_meas:
                    pointer += 1
                continue

            # this is a packet in arrival, create a new journey
            newid = len(self.journeys)
            self.journeys[newid] = dict()
            self.journeys[newid]['dir'] = p[1]
            self.journeys[newid]['glob'] = p[5]  # global ids as a first filter
            self.journeys[newid]['ts_in'] = p[0]  # timestamp of arrival
            self.journeys[newid]['set'] = list()  # set measurements of ids in inputs for this journey
            self.journeys[newid]['set'].append(self.inputs.index(p))
            self.journeys[newid]['set_ids'] = dict()
            # tmp_list = [f"uid{newid}"]
            # for l in p[6]:
            #     tmp_list.append(f"{l}{p[6][l]}")
            self.journeys[newid]['set_ids'] = {'uid': newid}
            self.journeys[newid]['set_ids'].update(p[6])
            self.journeys[newid]['next_points'] = self.points[p[2]]['next']  # list of possible next points
            # self.journeys[newid]['last_points'] = [p[2]]
            self.journeys[newid]['completed'] = False  # True if the journey is complete
            list_meas.remove(pointer)  # Remove from the list
            local_pointer = pointer + 1
            while local_pointer not in list_meas and local_pointer < nb_meas:
                local_pointer += 1
            pointer, _ = _rec_rebuild(pointer, local_pointer, newid)
            pointer = _get_next(list_meas, nb_meas, pointer)

        # retrieves all journey to build out_journeys
        for j in self.journeys:
            # Case : The journey is incomplete
            if not self.journeys[j]['completed']:
                continue
            for e in self.journeys[j]['set']: # for all elements in set of ids
                e_tmp = self.inputs[e]
                tmp_uid = self.journeys[j]['set_ids']['uid']
                tmp_str = f"uid{tmp_uid}.{dict_ids_to_str(self.journeys[j]['glob'])}.{dict_ids_to_str(e_tmp[6])}"
                self.out_journeys.append((
                    e_tmp[0],
                    'D' if e_tmp[1] == 0 else 'U',
                    f"{e_tmp[2]}--{e_tmp[3]}",
                    e_tmp[4],
                    tmp_str))
                # points latency
                tmp_point = self.points[e_tmp[2]]
                if 'duration' not in tmp_point:
                    tmp_point['duration'] = {}
                if e_tmp[2] in self.pointsInD or e_tmp[2] in self.pointsInU:  # Is an in points
                    tmp_point['duration'][tmp_uid] = 0
                else:  # Is a mid point because out could not be in e_tmp[2]
                    current_index = self.journeys[j]['set'].index(e)
                    prev_ts = self.inputs[self.journeys[j]['set'][current_index - 1]][0]
                    tmp_point['duration'][tmp_uid] = e_tmp[0] - prev_ts
        self.out_journeys.sort(key=operator.itemgetter(0))
        print(f"{nb_meas - len(self.out_journeys)} orphans / {nb_meas}")
        try:
            with open("latseq.lseqj", 'w+') as f:
                print(f"[INFO] Writing latseq.lseqj ...")
                for e in self.yield_clean_inputs():
                    f.write(f"{e}\n")
        except IOError as e:
            print(f"[ERROR] on open({self.logpath})")
            print(e)
            raise e


    # GETTERS
    def get_filename(self) -> str:
        return self.logpath.split('/')[-1]

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
                yield f"{epoch_to_datetime(e[0])} {e[1]} (len{e[3]['len']})\t{e[2]}\t{e[4]}"
                # yield f"{e[0]} {e[1]} {e[2]} {e[4]}"
        except Exception as e:
            raise e

    def yield_journeys(self):
        try:
            for j in self.journeys:
                yield j
        except Exception as e:
            raise e

    # STRINGIFY
    def paths_to_str(self) -> str:
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
            with open(f"lseq_data_{args.logname.split('/')[-1]}.pkl", 'rb') as fin:
                try:
                    lseq = pickle.load(fin)
                except EOFError:
                    raise FileNotFoundError
        except FileNotFoundError:
            try:
                lseq = latseq_log(args.logname)
            except Exception as e:
                raise(e)
                print(f"[ERROR] On creating a lseq element {args.logname}")
                exit()
        if not hasattr(lseq, 'journeys') or not lseq.journeys:
            lseq.rebuild_packets_journey_recursively()
        with open(f"lseq_data_{lseq.get_filename()}.pkl", 'wb') as fout:
            pickle.dump(lseq, fout, pickle.HIGHEST_PROTOCOL)
        
        print(lseq.get_list_of_points())
            print(lseq.print_paths())
            lseq.rebuild_packets_journey_recursively()
