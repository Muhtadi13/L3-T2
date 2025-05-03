#!/bin/bash

# Define allowed programming languages and archive formats
allowed_languages="c cpp py sh"
allowed_archived_formats="zip rar tar"

# Set the working directory, expected output file, and penalty per missing line
working_directory="/home/assignment"
expected_output_file="/home/assignment/expected_output.txt"
penalty_per_line=5

# Function to check if a file is an allowed archive format
is_archive_format() {
    local file_extension="${1##*.}"
    echo "$allowed_archived_formats" | grep -qw "$file_extension"
}

# Function to extract archive files
extract_archive() {
    local file_path="$1"
    local student_id="$2"
    local temp_dir="$working_directory/$student_id"

    mkdir -p "$temp_dir"

    case "${file_path##*.}" in
        zip) unzip "$file_path" -d "$temp_dir" > /dev/null ;;
        rar) unrar x "$file_path" "$temp_dir" > /dev/null ;;
        tar) tar -xf "$file_path" -C "$temp_dir" > /dev/null ;;
        *) echo "Unsupported archive format for $file_path" ;;
    esac
}

# Function to check if a file is in allowed programming languages
is_allowed_language() {
    local file_name="$1"
    local ext="${file_name##*.}"
    echo "$allowed_languages" | grep -qw "$ext"
}

# Function to run the student's program and capture the output
run_student_program() {
    local student_id="$1"
    local program_file="$2"
    local output_file="$working_directory/$student_id/${student_id}_output.txt"

    # Check the file extension and run the appropriate command
    case "${program_file##*.}" in
        c) gcc "$program_file" -o "$working_directory/$student_id/$student_id.out" && "$working_directory/$student_id/$student_id.out" > "$output_file" ;;
        cpp) g++ "$program_file" -o "$working_directory/$student_id/$student_id.out" && "$working_directory/$student_id/$student_id.out" > "$output_file" ;;
        py) python3 "$program_file" > "$output_file" ;;
        sh) bash "$program_file" > "$output_file" ;;
        *) echo "Unrecognized program format for $program_file";;
    esac
}

# Function to compare the student's output with the expected output
compare_output() {
    local student_id="$1"
    local student_output="$working_directory/$student_id/${student_id}_output.txt"
    local penalty=0

    # Compare the student's output with the expected output and apply penalties
    if [ -f "$student_output" ]; then
        diff -u "$expected_output_file" "$student_output" | grep '^-' | while read -r line; do
            penalty=$((penalty + penalty_per_line))
        done
        echo "Student $student_id has $penalty penalty marks."
    else
        echo "Output file for student $student_id not found!"
    fi
}

# Process each student submission in the working directory
for file in "$working_directory"/*; do
    # Extract the student ID from the file name (e.g., 2005001.zip -> 2005001)
    student_id=$(basename "$file" | cut -d'.' -f1)

    # If the submission is archived, extract it
    if is_archive_format "$file"; then
        echo "Processing archived file $file for student $student_id"
        extract_archive "$file" "$student_id"
    else
        # Create a directory named after the student ID if not archived
        echo "Processing unarchived file $file for student $student_id"
        mkdir -p "$working_directory/$student_id"
        mv "$file" "$working_directory/$student_id/"
    fi

    # Check the extracted or moved files
    found_valid_program=false
    for submission_file in "$working_directory/$student_id"/*; do
        # Check if the file corresponds to the student ID and is in an allowed language
        if [[ "$(basename "$submission_file")" == *"$student_id"* ]] && is_allowed_language "$submission_file"; then
            echo "Valid submission: $(basename "$submission_file") for student $student_id"
            found_valid_program=true

            # Run the student's program and generate output
            run_student_program "$student_id" "$submission_file"

            # Compare the output with the expected output and apply penalties
            compare_output "$student_id"
        fi
    done

    if ! $found_valid_program; then
        echo "No valid program found for student $student_id. Penalty applied."
    fi
done
