import sys
import os
import glob
import string
import time

def isBlankLine(line):
    for ch in line:
        if ch in [' ',' ',' '] :
            continue
        else:
            return False
    return True

sys.argv.append('G:\leslie\01github\ofxUtils')
sys.argv.append('data/copyright.txt')
if len(sys.argv) < 3:
    exit()

base_dir = sys.argv[1]
#filenames = os.listdir(sys.argv[1])
filesRead = r"G:\leslie\01github\ofxUtils\*.cpp"
filenames = glob.glob(filesRead)
print filenames
commentfilename = sys.argv[2]

author = " *  Created by Leslie Yang on " + time.strftime("%Y-%m-%d",time.localtime(time.time())) + "\n *\n *"

# read the comments
f = file(commentfilename)
commentLines = f.readlines()
commentLines.insert(0, author);
f.close()
print string.join(commentLines, '')

# add the comment into each source file
for filename in filenames:
    srcfilename = filename
    #srcfilename = base_dir + "/" + srcfilename
    f = file(srcfilename)
    srcfileLines = f.readlines()
    f.close()
    # filter out the previous comment header
    for line in srcfileLines:
        if isBlankLine(line) :
            continue
        if line[0] == '#' :
            srcfileLines = commentLines + srcfileLines
            srcfileLines.insert(0, "/*\n *  " + filename + "\n *\n")
            f = file(srcfilename, 'w')
            f.writelines(srcfileLines)
            print 'Add comment to '+srcfilename
            f.close()
        break
    
