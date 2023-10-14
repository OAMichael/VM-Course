from yaml import load, dump
try:
    from yaml import CLoader as Loader, CDumper as Dumper
except ImportError:
    from yaml import Loader, Dumper

data = load(open('../isa/isa.yml', 'r'), Loader=Loader)
# print(data)
output = dump(data, Dumper=Dumper)
print(output)
# with open('../isa/isa.yml', 'r'):
