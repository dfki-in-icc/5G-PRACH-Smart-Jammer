#!/usr/bin/python3

# -----------------------------------------------------------
# Process latseq logs to get statistics and visualization
#
# (C) 2020 Flavien Ronteix--Jacquet, Lannion, France
# Released under MIT License
# email flavien.ronteixjacquet@orange.com
# -----------------------------------------------------------

"""Calculate statistics on a latseq_logs output

Example:
    ./latseq_logs.py -l /home/flavien/latseq.simple.lseq -j | ./latseq_stats.py -j

Attributes:
    none

TODO

"""

import sys
import argparse
import json
import datetime
import statistics
import numpy
# import math

#
# GLOBALS
#
S_TO_MS = 1000

#
# FUNCTIONS
#

#
# STATISTICS
#
class latseq_stats:
    """Class of static methods for statistics stuff for latseq
    """
    # PRESENTATION
    @staticmethod
    def str_statistics(statsNameP: str, statsP: dict) -> str:
        """Stringify a statistics

        A statistics here embedded size, average, max, min, quantiles, stdev

        Args:
            statsNameP (str): the title for this stats
            statsP (str): a dictionnary with statistics

        Returns:
            str: the output statistics
        """
        res_str = f"Stats for {statsNameP}\n"
        for dir in statsP:
            if dir == '0':
                res_str += "Values \t\t | \t Downlink\n"
                res_str += "------ \t\t | \t --------\n"
            elif dir == '1':
                res_str += "Values \t\t | \t Uplink\n"
                res_str += "------ \t\t | \t ------\n"
            else:
                continue
            keysD = statsP[dir].keys()
            if 'size' in keysD:
                res_str += f"Size \t\t | \t {statsP[dir]['size']}\n"
            if 'mean' in keysD:
                res_str += f"Average \t | \t {float(statsP[dir]['mean']):.3}\n"
            if 'stdev' in keysD:
                res_str += f"StDev \t\t | \t {float(statsP[dir]['stdev']):.3}\n"
            if 'max' in keysD:
                res_str += f"Max \t\t | \t {float(statsP[dir]['max']):.3}\n"
            if 'quantiles' in keysD:
                if len(statsP[dir]['quantiles']) == 5:
                    res_str += f"[75..90%] \t | \t {float(statsP[dir]['quantiles'][4]):.3}\n"
                    res_str += f"[50..75%] \t | \t {float(statsP[dir]['quantiles'][3]):.3}\n"
                    res_str += f"[25..50%] \t | \t {float(statsP[dir]['quantiles'][2]):.3}\n"
                    res_str += f"[10..25%] \t | \t {float(statsP[dir]['quantiles'][1]):.3}\n"
                    res_str += f"[0..10%] \t | \t {float(statsP[dir]['quantiles'][0]):.3}\n"
                else:
                    for i in range(len(statsP[dir]['quantiles']),0,-1):
                        res_str += f"Quantiles {i-1}\t | \t {statsP[dir]['quantiles'][i-1]:.3}\n"
            if 'min' in keysD:
                res_str += f"Min \t\t | \t {float(statsP[dir]['min']):.3}\n"
        return res_str

    # GLOBAL_BASED
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

    # JOURNEYS-BASED
    @staticmethod
    def journeys_latency_statistics(journeysP: dict) -> dict:
        """Function calculate statistics on journey's latency

        Args:
            journeysP (:obj:`dict` of journey): dictionnary of journey

        Returns:
            :obj:`dict`: statistics
        """
        times = [[],[]]
        for j in journeysP:
            if not journeysP[j]['completed']:
                continue
            times[journeysP[j]['dir']].append((
                j,
                (journeysP[j]['ts_out'] - journeysP[j]['ts_in'])*S_TO_MS))
        # {'size': {105, 445}, 'mean': {0.7453864879822463, 19.269811539422896}, 'min': {0.04315376281738281, 0.00476837158203125}, 'max': {8.366107940673828, 445.9710121154785}, 'stdev': {1.6531425844726746, 61.32162047000048}}
        tmp_t = list()
        if not times[0]:
            times[0].append((0,0))
        if not times[1]:
            times[1].append((0,0))
        tmp_t.append([t[1] for t in times[0]])
        tmp_t.append([t[1] for t in times[1]])
        res = {'0' : {}, '1': {}}
        for d in res:
            res[d] = {
                'size': len(times[int(d)]),
                'min': min(tmp_t[int(d)]),
                'max': max(tmp_t[int(d)]),
                'mean': numpy.average(tmp_t[int(d)]),
                'stdev': numpy.std(tmp_t[int(d)]),
                'quantiles': numpy.quantile(tmp_t[int(d)], [0.1, 0.25, 0.5, 0.75, 0.9]),
                'times': times[int(d)]
            }
        return res


    # POINTS-BASED
    @staticmethod
    def points_latency_statistics(pointsP: dict) -> dict:
        """Function calculate statistics on points' latency

        Args:
            pointsP (:obj:`dict` of points): dictionnary of point

        Returns:
            :obj:`dict`: statistics
        """
        times = [dict(), dict()]
        for p in pointsP:
            if 'duration' not in pointsP[p]:
                continue
            tmp_p = [v * S_TO_MS for v in list(pointsP[p]['duration'].values())]
            if 0 in pointsP[p]['dir']:
                times[0][p] = tmp_p
            if 1 in pointsP[p]['dir']:
                times[1][p] = tmp_p
        res = {'0': {}, '1': {}}
        for d in res:
            dint=int(d)
            for e0 in times[dint]:
                res[d][e0] = {
                    'size': len(times[dint][e0]),
                    'min': min(times[dint][e0]),
                    'max': max(times[dint][e0]),
                    'mean': numpy.average(times[dint][e0]),
                    'stdev': numpy.std(times[dint][e0]),
                    'quantiles': numpy.quantile(times[dint][e0], [0.1, 0.25, 0.5, 0.75, 0.9]),
                    'durations': times[dint][e0]
                }
        return res

#
# MAIN
#
if __name__ == "__main__":
    parser = argparse.ArgumentParser("LatSeq statistics processing")
    parser.add_argument(
        "-l",
        "--log",
        type=str,
        dest="logname",
        help="Log file",
    )
    parser.add_argument(
        "-j",
        "--journeys",
        dest="stat_journeys",
        action='store_true',
        help="Request stat journeys in the case of command line script"
    )
    args = parser.parse_args()

    if args.logname:
        if args.logname.split('.')[-1] == 'lseqj':  # We do statistics from a log journey files
            pass

    # Else, we read stdin
    list_meas_json = []
    for meas in sys.stdin.readlines():  # For all lines in stdin
        # Clean all lines begins with # or [
        if meas.startswith('#') or meas.startswith('['):
            continue
        list_meas_json.append(meas)
    
    # print(lseq.get_list_of_points())
    # print(lseq.paths_to_str())
    if args.stat_journeys:
        journeys = {}
        # handle errors
        for j in list_meas_json:
            tmp_j = json.loads(j)
            journeys[tmp_j['set_ids']['uid']] = tmp_j
        print(latseq_stats.str_statistics("Journeys latency", latseq_stats.journeys_latency_statistics(journeys)))
    # print("Latency for points")
    # tmp_stats_points = latseq_stats.points_latency_statistics(lseq.points)
    # for dir in tmp_stats_points:
    #     for p in tmp_stats_points[dir]:
    #         print(latseq_stats.str_statistics(f"Point Latency for {p}", {dir : tmp_stats_points[dir][p]}))
