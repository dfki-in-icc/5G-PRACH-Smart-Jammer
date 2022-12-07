import os
import sys

def main():

    cmd_toasn = 'asn1c -gen-PER -no-gen-OER -fcompound-names -no-gen-example -findirect-choice -fno-include-deps -D ' + sys.argv[1] + ' ' + sys.argv[2]

    # E42 script
    # os.system(cmd_toE42)

    if( os.path.exists( sys.argv[1] ) == False ):
        cmd_create = 'mkdir ' +  sys.argv[1]
        os.system(cmd_create)

    os.system(cmd_toasn)

if __name__=="__main__":
    main()

