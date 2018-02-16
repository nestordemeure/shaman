
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
    """returns a function name without the template parameters"""
    if name.startswith("operator<"):
        return "operator<"
    else:
        try:
            return name[:name.index('<')]
        except ValueError:
            return name

def abreviate_file_name(file_name):
    """Returns a file name without its path"""
    index_path = file_name.rindex('/')
    return file_name[index_path+1:]

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
        functions.sort(reverse=True) # sort by call number
        # displays the result
        for function_calls,function_name,lines in functions:
            file_name = abreviate_file_name(file_of_function[function_name])
            write("{}\t{} (file {})".format(function_calls,function_name,file_name))
            lines.sort(reverse=True) # sort by call number
            for call_number,function_line,operation_name in lines:
                abreviated_opname = remove_template_parameters(operation_name)
                write("{}\t\t{} (line {})".format(call_number,abreviated_opname,function_line))

#------------------------------------------------------------------------------
# BREAKPOINT CLASS

def get_frame_seq():
    """Returns an iterator on the frames"""
    frame = gdb.newest_frame()
    while frame is not None:
        yield frame
        frame = frame.older()

#-----

breaknumber = 0

class ShamanDebugBreakpoint(gdb.Breakpoint):
    """breakpoint that displays basic informations"""
    def stop(self):
        """called when the breakpoint is triggered"""

        # progress counter to prove that gdb is not dead
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
        function_file = function_position.symtab.filename

        # avoids functions inside shaman
        while function_file.endswith("Shaman_Decl.h"):
            operation = function
            function = next(frames)
            function_position = function.find_sal()
            function_file = function_position.symtab.filename

        # useful informations
        function_name = function.name()
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