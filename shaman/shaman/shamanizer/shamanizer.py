
# SHAMANIZER : a python3 program that makes any C++ code shaman compatible
#
# work on a given file or on all the .c/.cpp/.h/.hpp in a given folder
# for each source file if it contains float/double/long double
#   replace the types
#   load shaman.h at the begining of the file
#   if it contains a printf
#       replace printf with tfm::printf
#       load tinyformat.h at the begining of the file (https://github.com/c42f/tinyformat)

cpp_extensions = (".h", ".c", ".cpp", ".hpp", ".cc", ".cxx", ".c++", ".hh", ".hxx", ".h++") # tuple required
numericTypes = [("float","Sfloat"), ("double","Sdouble"), ("long double","Slong_double")]
pathToShaman = "."
shamanHeader = "#include \"{}/shaman/Shaman.h\"".format(pathToShaman)
mpiHeader = "#include \"{}shaman/Shaman_mpi.h\"".format(pathToShaman)
printfHeader = "#include \"{}/shamanizer/tinyformat.h\"".format(pathToShaman)

mpiTypes = [("MPI_FLOAT","MPI_SFLOAT"), ("MPI_DOUBLE","MPI_SDOUBLE"), ("MPI_LONG_DOUBLE","MPI_SLONG_DOUBLE")]
mpiOperations = [("MPI_MAX","MPI_SMAX"), ("MPI_MIN","MPI_SMIN"), ("MPI_SUM","MPI_SSUM"), ("MPI_PROD","MPI_SPROD")]
mpiFunctions = [("MPI_Init","MPI_Shaman_Init"), ("MPI_Finalize","MPI_Shaman_Finalize")]

#-----------------------------------------------------------------------------
# MODIFICATION COUNT

# dictionary that contains a default value (0) for easy increment
from collections import defaultdict

# used to store the number and nature of modifications done to the code
modification_counter = defaultdict(int)

def display_modification_summary():
    """displays the modifications done to the code"""
    if len(modification_counter) == 0:
        print("Nothing was changed.")
    for k,v in modification_counter.items():
        print(v, k, "have been replaced", sep=' ')

#-----------------------------------------------------------------------------
# STRING OPERATIONS

import re

def replace_strings_in_line(strings, line):
    """returns none (if no actions were available) or some newline if it was able to apply some string replacement"""
    changed = False
    for oldString,newString in strings:
        # does not match a pattern that is stuck to an alphanumeric character ('float1' would not match for 'float' but '[float]' would)
        line, matchnumber = re.subn('(^|\W)' + oldString + '(\W|$)', '\g<1>' + newString + '\g<2>', line)
        if matchnumber > 0:
            changed = True
            modification_counter[oldString] += matchnumber
    return line if changed else None

def replace_strings_in_text(strings, lines):
    """returns none (if no actions were available) or some lines if it was able to apply some string replacement"""
    changed = False
    for i,line in enumerate(lines):
        newline = replace_strings_in_line(strings, line)
        if newline is not None:
            changed = True
            lines[i] = newline
    return lines if changed else None

#-----------------------------------------------------------------------------
# HEADER

def find_header_index(lines):
    """returns a position to insert an header into a file (just before the first header, if it exists, or on the first line)"""
    for i,line in enumerate(lines):
        if line.startswith("#include"):
            return i
    return 0

def add_header(header, lines):
    """adds a given header in the lines"""
    lines.insert(find_header_index(lines), header + '\n')

#-----------------------------------------------------------------------------
# CODE REWRITING

import os

def export_lines(filepath,lines):
    """exports the lines to a renammed file"""
    os.makedirs(os.path.dirname(filepath), exist_ok=True)
    with open(filepath, 'w') as newfile:
        newfile.writelines(lines)

def change_printf(filepath, lines):
    """modifies the printf if needed and exports the lines"""
    newlines = replace_strings_in_text([("printf","tfm::printf")], lines)
    if newlines is not None:
        add_header(printfHeader, newlines)
        print("WARNING : 'printf' found in '" + filepath + "', don't forget to compile with the 'tinyformat' library.")
        export_lines(filepath, newlines)
    else:
        export_lines(filepath, lines)

def change_mpi(filepath, lines):
    """modifies the MPI calls if needed and exports the lines"""
    newlines = replace_strings_in_text(mpiTypes + mpiOperations + mpiFunctions, lines)
    if newlines is not None:
        add_header(mpiHeader, newlines)
        print("WARNING : MPI functions found in '" + filepath + "', don't forget to compile with the 'Shaman_mpi.h' file.")
        change_printf(filepath, newlines)
    else:
        change_printf(filepath, lines)

def change_type(filepath, lines):
    """if needed, modifies the types and forward to the printf"""
    newlines = replace_strings_in_text(numericTypes, lines)
    if newlines is not None:
        add_header(shamanHeader, newlines)
        change_mpi(filepath, newlines)
    else:
        export_lines(filepath, lines)

#-----------------------------------------------------------------------------
# SHAMANIZER

from sys import argv

def list_cpp_files(rootpath):
    """list the cpp files associated with the given path"""
    if not os.path.isfile(rootpath):
        return [file.path for file in os.scandir(rootpath) if file.is_file() and file.name.endswith(cpp_extensions)]
    else:
        if not rootpath.endswith(cpp_extensions) :
            print("WARNING : '", rootpath, "' does not end with a known C++ extension.")
        return [rootpath]

def shamanize_name(rootpath,filepath):
    """adds the shaman prefix to a path produce from a root"""
    prefixpath, targetname = os.path.split(rootpath)
    shamanpath = prefixpath + '/' +  "shaman_" + targetname
    return filepath.replace(rootpath,shamanpath)

def shamanize_file(filepath, outpath):
    """deals with types, printf and code exportation"""
    with open(filepath, 'r') as file:
        lines = file.readlines()
        change_type(outpath, lines)

def shamanizer(path):
    """takes a path and addapt all associated files to Shaman"""
    print("*** SHAMANIZER ***")
    for filepath in list_cpp_files(path):
        outpath = shamanize_name(path,filepath)
        shamanize_file(filepath, outpath)
    display_modification_summary()

#-----------------------------------------------------------------------------
# MAIN

# applies shamanizer to all the arguments in path
path_list = argv[1:] # skip the name of the program
for rootpath in path_list:
    shamanizer(rootpath)

# TODO might run on all single files when given a folder with a path that ends with '/'