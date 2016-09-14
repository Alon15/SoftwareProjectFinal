from os import listdir
from os.path import isfile, join

PROJECT_PATH = "C:\\Projects\\TheCoolestProjectEver"
SUB_FOLDERS = ["."]
REQUIRED_FORMATS = [".c", ".cpp", ".h"]
BAD_SYMBOLS = ["1", "2", "3", "4", "5", "6", "7", "8", "9"]


def BuildFileList(path):
    result = []
    for f in listdir(path):
        fileFullPath = join(path, f)
        if isfile(fileFullPath):
            dotIndex = f.rfind(".")
            if f[dotIndex:] in REQUIRED_FORMATS:
                result.append(fileFullPath)
    return result

def findBadLines(path):
    todoList = []
    rawStringList = []
    numbersList = []

    with open(path, "r") as f:
        lines = f.readlines()

    for i in range(len(lines)):
        line = lines[i]
        if line.startswith("#define") or line.startswith("#include") or line.startswith("extern"):
            continue
        if line.strip().startswith("//") or line.strip().startswith("*"):
            continue
        if line.count("\""):
            rawStringList.append(i)
        if line.upper().count("TODO"):
            todoList.append(i)
        if line.count("for"):
            continue
        for symbol in BAD_SYMBOLS:
            if symbol in line:
                lastIndex = line.find(symbol)
                while lastIndex != -1:
                    if lastIndex == 0 or not str.isalpha(line[lastIndex - 1]):
            numbersList.append(i)
                    lastIndex = line.find(symbol, lastIndex + 1)

    if len(todoList)==0 and len(rawStringList)==0 and len(numbersList)==0:
        print("[CLEAN] - %s" % path)
        return

    print("[WARNING] - %s" % path)
    if len(todoList) > 0:
        print("\t TODO statements detected:")
        for i in todoList:
            print("\t\tLine: %d - %s" % (i, lines[i].strip()))
    if len(rawStringList) > 0:
        print("\t Raw strings detected:")
        for i in rawStringList:
            print("\t\tLine: %d - %s" % (i, lines[i].strip()))
    if len(numbersList) > 0:
        print("\t Numbers detected:")
        for i in numbersList:
            print("\t\tLine: %d - %s" % (i, lines[i].strip()))

if __name__ == "__main__":
    targetFolders = []
    for folderName in SUB_FOLDERS:
        targetFolders.append(join(PROJECT_PATH, folderName))
    for tFloder in targetFolders:
        files = BuildFileList(tFloder)
        for codeFile in files:
            findBadLines(codeFile)
