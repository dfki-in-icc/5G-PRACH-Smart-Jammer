#!/usr/bin/python3
import argparse
import re
import datetime

# python3 latseq_logs.py -l /home/flavien/oai/oai-laurent/common/utils/LATSEQ/lseq_stats/latseq.07042020.lseq

# TODO
# stats at each point (time, number, sd,...)
# rebuild the path of a data
# output a json less rough to use

### GLOBALS ###
RAW_INPUTS = []

### UTILS ###
def read_file(filepath: str) -> str:
    try:
        with open(filepath, 'r') as f:
            print(f"[INFO] Reading {filepath} ...")
            return f.read()
    except IOError as e:
        print(f"[ERROR] on open({filepath})")
        print(e)
        raise e

def epoch_to_datetime(epoch: str) -> str:
    return datetime.datetime.fromtimestamp(float(epoch)).strftime('%Y-%m-%d %H:%M:%S.%f')

def dstamp_to_epoch(dstamptime: str) -> float:
    return float(datetime.datetime.strptime(
        dstamptime, "%Y%m%d_%H%M%S.%f"
    ).timestamp())


### PROCESS LOG ###
def read_log(fpath: str) -> list:
    res = []
    for l in read_file(fpath).splitlines():
        if l: # line is not empty
            # Match pattern https://www.tutorialspoint.com/python/python_reg_expressions.htm
            if re.match(r'#.*$', l, re.M):
                continue
            else:
                tmp = l.split(' ')
                if len(tmp) < 4:
                    print(f"[WARNING] {l} is a malformed line")
                    continue
                res.append(
                    tuple(
                        [float(tmp[0]),
                        0 if tmp[1] == 'D' else 1,
                        tmp[2],
                        tmp[3]]
                        )
                    )
    return res

def get_timestamp() -> list:
    return list(map(lambda x: x[0], RAW_INPUTS))

def process_raw_inputs():
    print(get_timestamp())

### STATISTICS ###
def mean_separation_time_between_log_by_points():
    l = get_timestamp()
    print(l[-1] - l[0])
    print(len(l))
    return (l[-1] - l[0])/len(l)

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
            RAW_INPUTS = read_log(args.logname)
        except Exception as e:
            raise(e)
            print(f"[ERROR] On reading lines of logfile {args.logname}")
            exit()
        else:
            print(mean_separation_time_between_log_by_points())
            #process_raw_inputs()
