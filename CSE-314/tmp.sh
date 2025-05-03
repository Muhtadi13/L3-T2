#!/bin/bash

# Define the allowed archive formats and programming languages
allowed_archived_formats="zip rar tar"
allowed_languages="c cpp py sh"

# Define the working directory (replace with your actual directory)
working_directory="/home/assignment"

# Function to extract the file extension
get_extension() {
    echo "${1##*.}"
}

# Function to check if a file is in an allowed language
check_language() {
    local file_extension="${1##*.}"
    if echo "$allowed_languages" | grep -qw "$file_extension"; then
        return 0  # Allowed language
    else
        return 1  # Not an allowed language
    fi
}

# Function to handle archive files
handle_archive() {
    local file_path="$1"
    local student_id="$2"
    local file_extension=$(get_extension "$file_path")

    # Create a directory for the student ID
    mkdir -p "$working_directory/$student_id"

    temp_dir=$(mktemp -d)

    # Extract the archive based on its type
    case "$file_extension" in
        zip)
            unzip "$file_path" -d "$temp_dir" > /dev/null 2>&1
            ;;
        rar)
            unrar x "$file_path" "$temp_dir" > /dev/null 2>&1
            ;;
        tar)
            tar -xf "$file_path" -C "$temp_dir" > /dev/null 2>&1
            ;;
        *)
            echo "Unsupported archive format: $file_extension for student $student_id"
            rm -rf "$temp_dir"
            return 1
            ;;
    esac

    # Check if the archive extraction was successful
    if [ $? -ne 0 ]; then
        echo "Failed to extract $file_path for student $student_id."
        rm -rf "$temp_dir"
        return 1
    fi

    # Move extracted files to the student's directory and check for valid files
    for extracted_file in "$temp_dir"/*; do
        extracted_filename=$(basename "$extracted_file")
        if [[ "$extracted_filename" =~ ^$student_id\.* ]]; then
            if check_language "$extracted_file"; then
                mv "$extracted_file" "$working_directory/$student_id/"
                echo "Valid submission: $extracted_filename for student $student_id."
            else
                echo "Invalid file format for student $student_id: $extracted_filename."
            fi
        else
            echo "File name $extracted_filename does not match student ID $student_id."
        fi
    done

    # Cleanup
    rm -rf "$temp_dir"
}

# Function to handle non-archived files
handle_non_archive() {
    local file_path="$1"
    local student_id="$2"
    local file_extension=$(get_extension "$file_path")

    # Create a directory for the student ID
    mkdir -p "$working_directory/$student_id"

    # Check if the file corresponds to the student ID
    if [[ "$(basename "$file_path")" =~ ^$student_id\.* ]]; then
        if check_language "$file_path"; then
            mv "$file_path" "$working_directory/$student_id/"
            echo "Valid submission: $(basename "$file_path") for student $student_id."
        else
            echo "Invalid file format for student $student_id: $(basename "$file_path")."
        fi
    else
        echo "File name $(basename "$file_path") does not match student ID $student_id."
    fi
}

# Main loop: Process each file in the working directory
for file_path in "$working_directory"/*; do
    # Get the student ID (the base name without extension)
    student_id=$(basename "$file_path" | cut -d'.' -f1)

    # Check if the file is an archive (zip, rar, tar)
    file_extension=$(get_extension "$file_path")

    if echo "$allowed_archived_formats" | grep -qw "$file_extension"; then
        # Handle archive files
        handle_archive "$file_path" "$student_id"
    else
        # Handle non-archive files
        handle_non_archive "$file_path" "$student_id"
    fi
done






