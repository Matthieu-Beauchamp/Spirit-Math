import sys
import os


projectRoot = os.path.abspath(os.path.join(sys.argv[0], ".."))

if len(sys.argv) != 2:
    print("Unexpected number of command line arguments")
    exit()
    
moduleName = sys.argv[1]

if not moduleName.isalpha():
    print(f"invalid name {moduleName}")
    exit()

moduleNames = {
    "lower": moduleName.lower(),
    "cap": moduleName.capitalize(),
    "upper": moduleName.upper(),
}

renamePatterns = {
    "SPIRIT_MODULE": "SPIRIT_"+moduleNames["upper"], # CMake variables
    "spirit-module": "spirit-"+moduleNames["lower"], # lib name
    "Spirit-Module": "Spirit-"+moduleNames["cap"],   # Folder and project name
    "spirit_module": "spirit_"+moduleNames["lower"]  # macro name in test/cmake...
}

def rename(txt: str) -> str:
    for old, new in renamePatterns.items():
        txt = txt.replace(old, new)

    return txt

files = [
    "CMakeLists.txt",
    "test/CMakeLists.txt",
    "examples/CMakeLists.txt",
    "src/SPIRIT/Component/CMakeLists.txt",

    "ReadMe.md"
]

if __name__ == "__main__":
    # print(projectRoot)
    # print(moduleName)
    # print(renamePatterns)

    for fileName in files:
        path = os.path.join(projectRoot, fileName)

        with open(path, "r") as f:
            txt = f.read()

        txt = rename(txt)

        with open(path, "w") as f:
            f.write(txt)

    print("Renaming done, delete me!")
