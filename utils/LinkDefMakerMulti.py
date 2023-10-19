#!/usr/bin/python3 -Wall
#
# A little script to generate linkdef files for each of the classes that needs a ROOT dictionary.
# you can use this to generate a single linkdef per class, or for many classes, depending
# on how you call it.

import sys

def main():
    if len(sys.argv) < 2:
        print("Usage: LinkDefMakerMulti.py <linkdef_filename> <classname1> <classname2> ... ")
        print("   ex: LinkDefMakerMulti.py  LinkDef.h <derp1> <derp2>")
        return -1

    opfName = sys.argv[1]

    classes = []
    for x in range(2, len(sys.argv)):
        classes.append(sys.argv[x])

    # open file and print away
    opf = open(opfName, "w")
    opf.write("#include <map>\n")
    opf.write("#ifdef __CLING__\n")
    opf.write("#pragma link off all globals;\n")
    opf.write("#pragma link off all classes;\n")
    opf.write("#pragma link off all functions;\n")
    opf.write("#pragma link C++ nestedclasses;\n")
    opf.write("#pragma link C++ nestedtypedefs;\n")
    opf.write("#pragma link C++ class std::map<int, double>;\n")
    for cl in classes:
        opf.write("#pragma link C++ class {0}+;\n".format(cl))
        opf.write("#pragma link C++ class std::vector<{0}>+;\n".format(cl))

    opf.write("#endif")
    opf.close()

if __name__== "__main__":
    main()
