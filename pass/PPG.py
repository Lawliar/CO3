import argparse
parser = argparse.ArgumentParser()
parser.add_argument("-b", type=int,required=True,
                    help="how many files to be merged in a batch")
parser.add_argument("-i",required=True,
                    help="input dir")
parser.add_argument("-o",required=True,help="output dir")
