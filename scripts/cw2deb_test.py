import os
from xml.etree.ElementTree import parse

MAX_RETRY_CNT = 3

def make_deb():
    for retry_cnt in range(MAX_RETRY_CNT):
        res = os.system("ls")
        if res == 0:
            print("complete")
            return 0
        else:
            if retry_cnt == MAX_RETRY_CNT-1:
                print("fail, retry_cnt: %d"%retry_cnt)
                return 256

# res = make_deb()
# print(res)

path = "/home/syscon/catkin_ws/src/scorpion_ros/sp_slam/package.xml"
def load_xml(path):
    try:
        return parse(path).getroot()
    except Exception as e:
        print("Failed to load package.xml file [%s][%s]"%(path, str(e)))
        return None

def initialize():
    root = load_xml(path)
    print(root)
    if root is None:
        version = ""
        description = ""
        author = []
    else:
        name =root.find("name").text
        print("name: %s"%name)
        version = root.find("version").text
        print("version: %s"%version)
        description = root.find("description").text.strip()
        print("description: %s"%description)
        author = [elem.text for elem in root.findall("author")]
        print("author: %s"%author)
        # maintainer = [elem.text for elem in root.findall("maintainer")]
        # print("maintainer: %s"%maintainer)
        # print("------")
        # test = [elem.text for elem in root.findall("test")]
        # print("test: %s"%test)
        # author.extend(test)
        # print("author: %s"%author)
        # print("------")
        author.extend([elem.text for elem in root.findall("maintainer")] )
        print("author: %s"%author)

# initialize()

MODE_ENTIRE = 0
class SharedVars():
    def __init__(self, **kwargs):
        for key, value in kwargs.items():
            setattr(self, key, value)

class CreateDebian():
    def __init__(self, args):
        self.execute(args)

    def execute(self, args):
        print("first mode: %s" %args.mode)

if __name__ == "__main__":
    mode = MODE_ENTIRE
    build_mode = ""
    shared_vars = SharedVars(mode=mode, build_mode=build_mode)
    if mode in [MODE_ENTIRE]: CreateDebian(shared_vars)