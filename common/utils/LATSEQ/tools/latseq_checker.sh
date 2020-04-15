#!/bin/bash

OAI_PATH=$1
LATSEQ_PATH="$OAI_PATH/common/utils/LATSEQ"
LATSEQ_TOKEN="LATSEQ_P("
LATSEQ_H_INC="#include \"common/utils/LATSEQ/latseq.h\""
NB_TOKEN_BEFORE_VARARGS=2
errors=0

usage() {
    echo "the absolute path to oai should be given as first argument"
    echo -e "-v \t : to increase verbosity"
    echo -e "-p \t : print internal variables of latseq"
    exit
}

# function to get latseq info in sources
get_latseq_info () {
    if [[ ! -z "$VERBOSE" ]] ; then echo "${FUNCNAME[0]}"; fi
    ENTRY_MAX_LEN=$(cat $LATSEQ_PATH/latseq.h | grep MAX_LEN_DATA_ID | tr -s ' ' | cut -d " " -f3)
}

# function to check one line of LATSEQ_P
# $1 : path to a source file
# $2 : line contains LATSEQ_P
check_latseq_p() {
    if [[ ! -z "$VERBOSE" ]] ; then echo "checking line... $1:$2"; fi
    # check the presence of include latseq 0 if found
    if [ `grep "$LATSEQ_H_INC" $1 >/dev/null; echo $?` -eq 1 ]; then 
        echo -e "\e[31m\e[1m[INCLUDE]\t$1\e[21m :\n\t$LATSEQ_H_INC is missing\e[0m";
        errors=$((errors+1));
    fi
    # check if LATSEQ_P not empty
    if [[ "$2" != *");" ]] ; then 
        echo -e "\e[31m\e[1m[EMPTY]\t\t$1:$2\e[21m :\n\tLATSEQ_P empty or multilined\e[0m";
        errors=$((errors+1));
        exit
    fi
    # check number of argument
    nbArgsFmt=$(echo $2 | grep -o "%d" | wc -l)
    nbArgsGiven=$((`echo $2 | tr -dc ',' | wc -c` + 1 - NB_TOKEN_BEFORE_VARARGS))
    if [ "$nbArgsFmt" -ne "$nbArgsGiven" ]; then
        echo -e "\e[31m\e[1m[NB_VAR]\t$1:$2\e[21m :\n\tnumber of arguments for format ($nbArgsFmt) and given ($nbArgsGiven) are different\e[0m";
        errors=$((errors+1));
    fi
    # check the variable length
    #   get the entry length in sources
    if [[ -z "$ENTRY_MAX_LEN" ]]; then get_latseq_info;fi
    #   compute approximated length
    fmtLen=$(echo $2 | cut -d ',' -f2)
    fmtLen=${#fmtLen}
    computedLen=$((fmtLen + nbArgsFmt*4))
    if [ "$computedLen" -gt $((nbArgsFmt*ENTRY_MAX_LEN)) ]; then
        echo -e "\e[31m\e[1m[DATAID_LEN]\t$1:$2\e[21m :\n\tNot enough space reserved for data id ($computedLen vs $ENTRY_MAX_LEN)\e[0m";
        errors=$((errors+1));
    fi

}

# Print internal variables of latseq
print_intvar() {
    echo "${FUNCNAME[0]}"
}

# function to update header of latseq log file in latseq sources
update_latseq_header() {
    if [[ ! -z "$VERBOSE" ]] ; then echo "${FUNCNAME[0]}"; fi
}

F_OLD=""

if [ $# -eq 0 ]; then usage; fi

case $2 in
    "-v")
        VERBOSE=1;;
    "-p")
        print_intvar
        exit;;
esac

# get all files where are LATSEQ_P
grep --exclude-dir={common,targets} --include=\*.c -rnw $OAI_PATH -e $LATSEQ_TOKEN | cut -d: -f1 | while read F
do
    if [[ "$F" != "$F_OLD" ]]; then 
        if [[ ! -z "$VERBOSE" ]] ; then echo "-----"; fi
        grep $F -e $LATSEQ_TOKEN | while read L; do check_latseq_p "$F" "$L"; done
    fi
    F_OLD=$F
done
echo "LatSeq checker finished with $errors error(s)"