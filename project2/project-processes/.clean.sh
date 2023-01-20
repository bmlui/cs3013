
msg="WARNING: This script removes source files, test results, and binary files. As such, it should only be run by the teaching staff."
builtin echo -e "\e[31m$msg\e[0m"

read -r -p "Are you sure you want to run it? [y/N] " response

case "$response" in
  [yY][eE][sS]|[yY]) 
    :
    ;;
  *)
    exit 1 
    ;;
esac



WORKING_DIR=$(pwd)
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

if [[ "$WORKING_DIR" != "$SCRIPT_DIR" ]]; then
  echo "Please rerun this script from: $SCRIPT_DIR"
  exit 1
fi


echo "Test output to remove:"
find test* -maxdepth 2 -type d -name "tests-out" -exec echo " {}" \;


read -r -p "Remove test output? [y/N] " response
case "$response" in
  [yY][eE][sS]|[yY]) 
    # Remove the test results 
    find test* -maxdepth 2 -type d -name "tests-out" -exec rm -rf "{}" \;
    ;;
  *)
    :
    ;;
esac

echo "C source files to remove:"
find . -maxdepth 1 -type f -name "*.c" -exec echo " {}" \;

read -r -p "Remove top-level C source files? [y/N] " response
case "$response" in
  [yY][eE][sS]|[yY]) 
    # Remove any source files in the top-level
    find . -maxdepth 1 -type f -name "*.c" -exec rm "{}" \; 
    ;;
  *)
    :
    ;;
esac

echo "Binary files to remove:"
find . -maxdepth 2 -type f -exec grep -IL . "{}" \;

read -r -p "Remove binary files (includes all non-text files such as ELF, empty, jpg, etc)? [y/N] " response
case "$response" in
  [yY][eE][sS]|[yY]) 
    #Remove binary files. Includes:
    # - vim .swp
    # - empty files, e.g., wcat.c without any text.
    # - elf binaries.
    find . -maxdepth 2 -type f -exec grep -IL . "{}" \; | xargs -L1 -I{} rm "{}"
    ;;
  *)
    :
    ;;
esac

read -r -p "Remove .DS_Store and .swp files? [y/N] " response
case "$response" in
  [yY][eE][sS]|[yY]) 
    # swp might already be removed by the previous commands   
    find . -maxdepth 5 -type f -name ".DS_Store" -exec rm -if "{}" \;
    find . -maxdepth 5 -type f -name ".swp" -exec rm -if "{}" \;
    ;;
  *)
    :
    ;;
esac


read -r -p "Create placeholder C files? [y/N] " response
case "$response" in
  [yY][eE][sS]|[yY]) 
    #Create the new c files
    echo "Place code here." > wshell.c
    ;;
  *)
    :
    ;;
esac




