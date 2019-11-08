
# SHAMAN's NUMERICAL PROFILER
#
# use gdb's python API to get informations from the numerical debugging
# the information is then displayed in a readable format
#
# usage : $ gdb -quiet --command shaman_profiler.py --args ./executable args

import gdb
from collections import defaultdict

#------------------------------------------------------------------------------
# PRINTING

def remove_template_parameters(name):
    """returns a function name without the template parameters : fun<T> -> fun"""
    if name.endswith('>'): # if the name might end with a template
        template_number = 0
        for i in reversed(range(len(name))):
            if name[i] is '>': # we are going inside a template
                template_number += 1
            elif name[i] is '<': # we are going outside a template
                template_number -= 1
                if template_number == 0: # we went outside the outer template
                    return name[:i]
    return name

def abreviate_file_name(file_name):
    """Returns a file name without its path : /path/to/file.cpp -> file.cpp"""
    index_path = file_name.rindex('/')
    return file_name[index_path+1:]

def common_substring(string1, string2):
    """return the common substring between two string (starting at 0)"""
    min_length = min(len(string1), len(string2))
    i = 0
    while (i < min_length) and (string1[i] == string2[i]):
        i += 1
    return string1[:i]

def common_path(list):
    """return the minimum common path between the values in a dictionnary.items()"""
    common = None
    for function,path in list:
        if common is None: common = path
        else: common = common_substring(common, path)
    return None

def program_name():
    """outputs the name of the current program"""
    progspace = gdb.current_progspace()
    return abreviate_file_name(progspace.filename)

#------------------------------------------------------------------------------
# CALL TREE

call_tree = defaultdict(int)
file_of_function = {}

def group_by_function(dict_data):
    """takes the raw call_tree items and produces a list of call_number*function_name*lines"""
    functions = {}
    callnumber_of_function = defaultdict(int)
    # compute the call number and group the lines by function
    for (function_name,function_line,operation_name), call_number in dict_data:
        callnumber_of_function[function_name] += call_number
        operators = (call_number,function_line,operation_name)
        if function_name in functions:
            functions[function_name].append(operators)
        else:
            functions[function_name] = [operators]
    # build the result as a list that contains the number of calls
    functions_list = [ (callnumber_of_function[name],name,lines) for name,lines in functions.items()]
    return functions_list

#-----

def export_call_tree(filepath):
    """exports the profile of the program"""
    with open(filepath, 'w') as file:
        def write(text): file.write(text + '\n')
        write("*** SHAMAN PROFILE ***")
        # gets the data in a clean format
        functions = group_by_function(call_tree.items())
        functions.sort(reverse=True)  # sort by call number
        # displays the result
        #root_length = len(common_path(file_of_function.items()))
        for function_calls,function_name,lines in functions:
            write("-----") #write('\n')
            file_name = file_of_function[function_name]#[root_length:]
            write("{}\t{} (file {})".format(function_calls,function_name,file_name))
            lines.sort(reverse=True)  # sort by call number
            for call_number, function_line, operation_name in lines:
                abreviated_opname = remove_template_parameters(operation_name)
                write("{}\t\t{} (line {})".format(call_number, abreviated_opname, function_line))

#------------------------------------------------------------------------------
# BREAKPOINT CLASS

def get_frame_seq():
    """Returns an iterator on the frames"""
    frame = gdb.newest_frame()
    while frame is not None:
        yield frame
        frame = frame.older()

def file_in_shaman(path):
    """returns true if a file is in shaman"""
    return path.endswith("shaman/src/Operators.h") or path.endswith("shaman/src/Functions.h")

#-----

breaknumber = 0

class ShamanDebugBreakpoint(gdb.Breakpoint):
    """breakpoint that displays basic informations"""
    def stop(self):
        """called when the breakpoint is triggered"""

        # progress counter to prove that gdb is not dead when the number of cancelations is huge
        global breaknumber
        breaknumber += 1
        if breaknumber % 100000 == 0:
            # WARNING the 50th+ print will kill gdb on older versions
            gdb.write("{}th numerical unstability detected\n".format(breaknumber))

        # raw information
        frames = get_frame_seq()
        next(frames) # we skip 'NumericalDebugger::unstability()'
        operation = next(frames)
        function = next(frames)
        function_position = function.find_sal()

        # avoids functions inside Shaman (such as +=)
        function_file = function_position.symtab.filename
        while file_in_shaman(function_file):
            operation = function
            function = next(frames)
            function_position = function.find_sal()
            function_file = function_position.symtab.filename

        # avoid functions called by the stl (such as std::min)
        function_name = function.name()
        while function_name.startswith("std::") or function_file.endswith("stl_algobase.h"):
            operation = function
            function = next(frames)
            function_position = function.find_sal()
            function_file = function_position.symtab.filename
            function_name = function.name()

        # useful informations
        operation_name = operation.name()
        function_line = function_position.line

        # saving the data
        call_tree[(function_name,function_line,operation_name)] += 1
        if not function_name in file_of_function:
            file_of_function[function_name] = function_file

        # keep going after hitting the breakpoint
        return False

#------------------------------------------------------------------------------
# RUN GDB

# Ensure shared libraries are already loaded
gdb.execute("start")

# Set our breakpoint, which might be in some shared library, hence the "start" previously
ShamanDebugBreakpoint("NumericalDebugger::unstability")

# Runs the program
gdb.execute("continue")

# exports the data collected
export_call_tree(program_name() + "_shaman_profile.txt")

# quit gdb
gdb.execute("quit")
