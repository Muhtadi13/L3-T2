#! /bin/bash
# IFS="\n"
# IFS=${IFS}#
# printf "%q" "${IFS}"

# input_filename=$(basename -- "$fullfile")
input_filename='input.txt'
mapfile all_lines < ${input_filename}
sz=${#all_lines[@]}

for ((i=0 ; i<${sz} ; i++))
do 
    sentence="${all_lines[i]}"
    words=()
    # echo $sentence
    for w in ${sentence[@]}
    do
        words+=(${w})
    done
    # echo ${#words[@]}
    # for w in ${words[@]}
    # do
    #     echo ${w}
    # done

done




# idsub_without_extension=$(echo "$idsub_without_extension" | xargs)
# echo "${id_range[0]}"
# echo "${id_range[1]}"
# echo "${#id_range[@]}"
# # echo "$idsub_without_extension"
# if [ ${#id_range[@]} -lt 2 ]; then
#     echo "Error: id_range array does not contain enough values"
#     # exit 1
# fi
# id_range[1]=$(echo "${id_range[1]}" | xargs)
# ((sum=id_range[1]))
# # sum=${id_range[0]}
# echo $sum
# sth="${id_range[1]}"
# id_range[1]=$(echo "${id_range[1]}" | xargs)
# echo $sth
# if ! [[ "${id_range[1]}" =~ ^[0-9]+$ ]]; then
#     echo "Error: Non-integer value detected"
#     printf "%q" "${sth:7:1}"
#     exit 1
# fi
# # if(())
# id_range[1]=$(echo "${id_range[1]}" | tr -d '\r')
get_basename_from_paht(){
    filepath=${1}
    filepath="${filepath##*/}"
    echo "${filepath%.*}"
}

check_in_range(){
    idsub=${1}
    extension="${idsub##*.}"
    idsub_without_extension="${idsub%.*}"
    id_range=()
    id_sentence=($(echo "${all_lines[6]}" | xargs | tr -d '\r'))
   
    for w in "${id_sentence[@]}"
    do
        id_range+=(${w})
    done
    # echo ${idsub_without_extension},${id_range[0]},${id_range[1]}
    if (( idsub_without_extension >= id_range[0] && idsub_without_extension <= id_range[1] )); then
        return 0
    else
        return 1
    fi
}

check_allowed_extension(){
    filename="${1}";
    file_extension="${1##*.}"
    allowed_extension=($(echo "${all_lines[2]}" | xargs | tr -d '\r'))
    file_extension=($(echo "${file_extension}" | xargs | tr -d '\r'))
    # echo $file_extension, ${allowed_extension[@]} 
    if echo "${allowed_extension[@]}" | grep -w "${file_extension}" ; then
        # echo "foundit"
        return 0
    else 
        return 1
    fi;

}

extract_file(){
    fullpath="${1}"
    extension="${fullpath##*.}"
    student_id=$(get_basename_from_paht "${fullpath}")

   
    working_directory=($(echo "${all_lines[5]}" | xargs | tr -d '\r'))

    mkdir -p ".${working_directory}/tmp"
    temp_dir=".${working_directory}/tmp"

    echo $student_id,$working_directory,$temp_dir

    case ${extension} in
        "zip") unzip ${fullpath} -d ${temp_dir} > /dev/null 2>&1
        ;;
        "rar")
            unrar ${fullpath} -x ${temp_dir} > /dev/null 2>&1
        ;;
        "tar")
            untar -x -f ${fullpath} -c ${temp_dir} > /dev/null 2>&1
        ;;
        *)
        echo "Invalid Format"
        rm -rf ${temp_dir}
        return 1;
        ;;
    esac
    if [ "$?" -ne 0 ]; then
        echo $?
        rm -rf "${temp_dir}"
        return 1
    fi



    for file_to_move in "${temp_dir}/${student_id}"/*; do
        echo $file_to_move
        file_to_move=($(echo "${file_to_move}" | xargs | tr -d '\r'))
        tmp_student_id=$(get_basename_from_paht $file_to_move)
        check_in_range "${tmp_student_id}"
        rng=$?
        check_allowed_extension "${file_to_move}" 
        ext=$?

        echo $file_to_move , $tmp_student_id, $rng , $ext

        if [[ ${tmp_student_id} =~ ^${student_id}\.* ]] && ! [[ ${rng} -ne 0 ]] && ! [[ ${ext} -ne 0 ]]; then
            mkdir -p ".${working_directory}/${student_id}"
            mv "${file_to_move}" ".${working_directory}/${student_id}/"
            rm -rf ${temp_dir}
            return 0
        else
            echo "Invalid file format"
            rm -rf ${temp_dir}
            return 1
        fi
    done
}

# extract_file "./root/200.zip"

directory_work(){
    file_to_move=${1}   
    file_to_move=($(echo "${file_to_move}" | xargs | tr -d '\r'))
    tmp_student_id=$(get_basename_from_paht $file_to_move)
    check_in_range "${tmp_student_id}"
    rng=$?
    check_allowed_extension "${file_to_move}" 
    ext=$?

    echo $file_to_move , $tmp_student_id, $rng , $ext

    if [[ ${tmp_student_id} =~ ^${student_id}\.* ]] && ! [[ ${rng} -ne 0 ]] && ! [[ ${ext} -ne 0 ]]; then
        mkdir -p ".${working_directory}/${student_id}"
        mv "${file_to_move}" ".${working_directory}/${student_id}/"
        rm -rf ${temp_dir}
        return 0
    else
        echo "Invalid file format"
        rm -rf ${temp_dir}
        return 1
    fi
}




execute_program(){
    filename=${1};
    extension=${filename##*.}
    student_id=${filename%.*}
       
    working_directory=($(echo "${all_lines[5]}" | xargs | tr -d '\r'))
    file_directory=".${working_directory}/${student_id}"
    output_file="${file_directory}/${student_id}_output.txt"
    filename="${file_directory}/${student_id}.cpp"

    echo $file_directory,$output_file

    case ${extension} in
        c)
        gcc ${filename} -o "${file_directory}/${student_id}.out" && "${file_directory}/${student_id}.out" > ${output_file}
        ;;
        cpp)
        g++ ${filename} -o "${file_directory}/${student_id}.out" && "${file_directory}/${student_id}.out" > ${output_file}
        ;;
        py)
        python3 ${filename} > ${output_file}
        ;;
        sh)
        bash ${filename} > ${output_file}
        ;;
        *) echo "Unrecognized file format"
        ;;
    esac
}
# execute_program "2005067.cpp"


output_match(){
    student_output_file=${1}
    expected_output_file=($(echo "${all_lines[7]}" | xargs | tr -d '\r'))
    expected_output_file=".${expected_output_file}"
    sum=0
    penalty=($(echo "${all_lines[4]}" | xargs | tr -d '\r'))
    # echo >> "${expected_output_file}"
    # echo >> "${student_output_file}"

    exec 3<"${expected_output_file}"
    exec 4<"${student_output_file}"

    line_number=1

    while IFS= read -r line1 <&3 && IFS= read -r line2 <&4; do
        # echo ${line1}
        # echo ${line2}
        if [ "$line1" != "$line2" ]; then
            # echo "${line_number}"
            sum=$(( sum + penalty ))
        fi
        ((line_number++))
    done
    while IFS= read -r line1 <&3; do
        sum=$(( sum + penalty ))
    done < "$expected_output_file"

    while IFS= read -r line2 <&4; do
        sum=$(( sum + penalty ))
    done < "$student_output_file"
    exec 3<&-
    exec 4<&-
    echo $sum
    return 0
}
# student_id=2005067
# working_directory=($(echo "${all_lines[5]}" | xargs | tr -d '\r'))
# file_directory=".${working_directory}/${student_id}"
# output_file="${file_directory}/${student_id}_output.txt"
# val=$(output_match ${output_file})
# echo ${val}

echo "id,marks,marks_deducted,total_marks,remarks" > marks.csv



issue_directory='/issues'
checked_directory='/checked'

create_or_empty_folder(){
    echo ${issue_directory}
    if [ -d "${issue_directory}" ]; then
        rm -rf "${issue_directory}"/*
    else
        echo "haha"
        mkdir "${issue_directory}"
    fi

    if [ -d "${issue_directory}" ]; then
        rm -rf "${issue_directory}"/*
    else
        mkdir "${issue_directory}"
    fi
}

create_or_empty_folder

# for eachfolder in ${working_directory}; do

#     if [[ -d "${eachfolder}" ]]; then
#         student_id=$(e+get_basename_from_paht ${eachfolder})
#         check_in_range ${student_id}
#         rng=$?
#         if [[ rng -ne 0 ]]; then
#             continue
#         fi
#     fi

# done


# echo  "gagga" $val







