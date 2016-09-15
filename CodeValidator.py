from os import listdir
from os.path import isfile, join

PROJECT_PATH = "C:\\Users\\$$Update_This_Line$$\\SoftwareProjectFinal"
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
	outtext = ""
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
		outtext += ("[CLEAN] - %s\n" % path)
		return
	outtext += ("[WARNING] - %s\n" % path)
	if len(todoList) > 0:
		outtext += ("\t TODO statements detected:\n")
		for i in todoList:
			outtext += ("\t\tLine: %d - %s\n" % (i, lines[i].strip()))
	if len(rawStringList) > 0:
		outtext += ("\t Raw strings detected:\n")
		for i in rawStringList:
			outtext += ("\t\tLine: %d - %s\n" % (i, lines[i].strip()))
	if len(numbersList) > 0:
		outtext += ("\t Numbers detected:\n")
		for i in numbersList:
			outtext += ("\t\tLine: %d - %s\n" % (i, lines[i].strip()))
	with open('CodeValidator.txt','a') as f:
		f.write(outtext)
	print("Scan %s" % (path.split("\\")[-1]))

if __name__ == "__main__":
	with open('CodeValidator.txt','w') as f:
		f.write("")
	targetFolders = []
	for folderName in SUB_FOLDERS:
		targetFolders.append(join(PROJECT_PATH, folderName))
	for tFloder in targetFolders:
		files = BuildFileList(tFloder)
		for codeFile in files:
			findBadLines(codeFile)
	print("Check results at CodeValidator.txt")