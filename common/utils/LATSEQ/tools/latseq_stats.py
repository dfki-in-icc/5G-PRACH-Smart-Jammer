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

TODO

"""

import sys
import argparse
import subprocess
import json
import datetime
import statistics
import numpy
# import math

#
# GLOBALS
#
S_TO_MS = 1000
QUANTILES = [0.1, 0.25, 0.5, 0.75, 0.9]

#
# FUNCTIONS
#

def output_function(outP: dict, flagP=False, data_nameP=""):
    """output wrapper

    Arguments:
        outP (:obj:`dict`): object to out
        flagP (bool): Print string if True, Json otherwise
    """
    if flagP:
        sys.stdout.write(latseq_stats.str_statistics(data_nameP, outP))
    else:
        sys.stdout.write(json.dumps(outP))

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
            str: the formatted statistics
        """
        res_str = f"Stats for {statsNameP}\n"
        for s in statsP:
            if 'dir' in statsP[s]:
                dir = statsP[s]['dir']
            else:
                dir = s
            if dir == 'D':
                res_str += "Values \t\t | \t Downlink\n"
                res_str += "------ \t\t | \t --------\n"
            elif dir == 'U':
                res_str += "Values \t\t | \t Uplink\n"
                res_str += "------ \t\t | \t ------\n"
            else:
                continue
            keysD = statsP[s].keys()
            if 'size' in keysD:
                res_str += f"Size \t\t | \t {statsP[s]['size']}\n"
            if 'mean' in keysD:
                res_str += f"Average \t | \t {float(statsP[s]['mean']):.3}\n"
            if 'stdev' in keysD:
                res_str += f"StDev \t\t | \t {float(statsP[s]['stdev']):.3}\n"
            if 'max' in keysD:
                res_str += f"Max \t\t | \t {float(statsP[s]['max']):.3}\n"
            if 'quantiles' in keysD:
                if len(statsP[s]['quantiles']) == 5:
                    res_str += f"[75..90%] \t | \t {float(statsP[s]['quantiles'][4]):.3}\n"
                    res_str += f"[50..75%] \t | \t {float(statsP[s]['quantiles'][3]):.3}\n"
                    res_str += f"[25..50%] \t | \t {float(statsP[s]['quantiles'][2]):.3}\n"
                    res_str += f"[10..25%] \t | \t {float(statsP[s]['quantiles'][1]):.3}\n"
                    res_str += f"[0..10%] \t | \t {float(statsP[s]['quantiles'][0]):.3}\n"
                else:
                    for i in range(len(statsP[s]['quantiles']),0,-1):
                        res_str += f"Quantiles {i-1}\t | \t {statsP[s]['quantiles'][i-1]:.3}\n"
            if 'min' in keysD:
                res_str += f"Min \t\t | \t {float(statsP[s]['min']):.3}\n"
        return res_str

    # GLOBAL-BASED
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
        tmp_t = list()
        if not times[0]:
            times[0].append((0,0))
        if not times[1]:
            times[1].append((0,0))
        tmp_t.append([t[1] for t in times[0]])
        tmp_t.append([t[1] for t in times[1]])
        res = {'D' : {}, 'U': {}}
        for d in res:
            dint = 0 if d == "D" else 1
            res[d] = {
                'size': len(times[dint]),
                'min': min(tmp_t[dint]),
                'max': max(tmp_t[dint]),
                'mean': numpy.average(tmp_t[dint]),
                'stdev': numpy.std(tmp_t[dint]),
                'quantiles': numpy.quantile(tmp_t[dint], QUANTILES).tolist()
            }
            # 'times': times[dint]
        return res

    @staticmethod
    def journeys_latency_per_point_statistics(journeysP: dict, pathsP: dict) -> dict:
        """Function calculate statistics on journey's latency by points

        Args:
            journeysP (:obj:`dict` of journey): dictionnary of journey
            pathsP (:obj:`dict` of paths): dictionnray of path

        Returns:
            :obj:`dict`: statistics
        """
        times_paths = {}
        # For all journeys
        for j in journeysP:
            if not journeysP[j]['completed']:
                continue
            else:
                pass
        res = {'D' : {}, 'U': {}}
        for d in res:
            dint = 0 if d == "D" else 1
            res[d] = {
                'size': len(times[dint]),
                'min': min(tmp_t[dint]),
                'max': max(tmp_t[dint]),
                'mean': numpy.average(tmp_t[dint]),
                'stdev': numpy.std(tmp_t[dint]),
                'quantiles': numpy.quantile(tmp_t[dint], QUANTILES).tolist()
            }
            # 'times': times[dint]
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
        res = {'D': {}, 'U': {}}
        for d in res:
            dint = 0 if d == "D" else 1
            for e0 in times[dint]:
                res[d][e0] = {
                    'dir': d,
                    'size': len(times[dint][e0]),
                    'min': min(times[dint][e0]),
                    'max': max(times[dint][e0]),
                    'mean': numpy.average(times[dint][e0]),
                    'stdev': numpy.std(times[dint][e0]),
                    'quantiles': numpy.quantile(times[dint][e0], QUANTILES).tolist()
                }
        return res

#
# MAIN
#
if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        "./latseq_stats.py",
        description="LatSeq Analysis Module - Statistics script")
    parser.add_argument(
        "-l",
        "--log",
        type=str,
        dest="logname",
        help="Log file",
    )
    parser.add_argument(
        "-P",
        "--print",
        dest="print_stats",
        action='store_true',
        help="Print statistics instead of return a json report"
    )
    parser.add_argument(
        "-j",
        "--journeys",
        dest="stat_journeys",
        action='store_true',
        help="Request stat journeys in the case of command line script"
    )
    parser.add_argument(
        "-jpp",
        "--jperpoints",
        dest="stat_journeys_points",
        action='store_true',
        help="Request stat on journeys per points in the case of command line script"
    )
    parser.add_argument(
        "-p",
        "--points",
        dest="stat_points",
        action='store_true',
        help="Request stat points in the case of command line script"
    )
    args = parser.parse_args()
    if not args.stat_journeys and not args.stat_points and not args.stat_journeys_points:
        sys.stdout.write("No action requested\n")
        exit()

    list_meas_json = []
    if args.logname:
        if args.logname.split('.')[-1] == 'lseqj':  # We do statistics from a log journey files
            pass
        if args.logname.split('.')[-1] == 'json':  # We do statistics from a json file
            try:
                with open(args.logname, 'r') as j:
                    for l in j.read().splitlines():
                        if l.startswith('#') or l.startswith('['):
                            continue
                        list_meas_json.append(l)
            except IOError:
                raise IOError(f"[Error] at openeing ({args.logname})")
    else:
        # Else, we read stdin
        for meas in sys.stdin.readlines():  # For all lines in stdin
            # Clean all lines begins with # or [
            if meas.startswith('#') or meas.startswith('['):
                continue
            list_meas_json.append(meas)

    if args.stat_journeys:
        journeys = {}
        # handle errors
        for j in list_meas_json:
            tmp_j = json.loads(j)
            journeys[tmp_j['uid']] = tmp_j
        output_function(latseq_stats.journeys_latency_statistics(journeys), args.print_stats, "Journeys latency stats")
    elif args.stat_journeys_points:
        journeys = {}
        # to a dict
        tmp_j = {}
        for jpp in list_meas_json:
            tmp_j = json.loads(jpp)
            journeys[tmp_j['uid']] = tmp_j
        # call latseq_logs to get path
        # TODO : gerer les erreurs
        # CA NE MARCHE PAS, avec run() et Popen me fait un Broken pipe...
        # La gestion des subprocess sous python c'est tout de même un peu rodeo !!!

        # print(journeys['32']['file'])
        # cmd = ["./latseq_logs.py", "-r", f"-l /home/flavien/latseq.simple.lseq"]
        # print(" ".join(cmd))
        # latseq_route_process = subprocess.run(cmd, check=True, stdout=subprocess.PIPE, universal_newlines=True)
        # print(latseq_route_process)
        # On va dire que ca marche
        paths = {}
        with open("routes.json", 'r') as fp:
            paths = json.load(fp)
        
        #output_function()
    elif args.stat_points:
        points = {}
        for p in list_meas_json:
            tmp_p = json.loads(p)
            point_name = list(tmp_p.keys())[0]
            points[point_name] = tmp_p[point_name]
        tmp_stats_points = latseq_stats.points_latency_statistics(points)
        for dir in tmp_stats_points:
            for p in tmp_stats_points[dir]:
                output_function({p: tmp_stats_points[dir][p]}, args.print_stats, f"Point Latency for {p}")
 