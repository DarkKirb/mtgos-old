#!/usr/bin/env python3
################################################################################
#  MTGos - A nanokernel in C++
#  Copyright (C) 2017 Morten Delenk
#
#  it under the terms of the GNU General Public License as published by
#  This program is free software: you can redistribute it and/or modify
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
################################################################################
import yaml
import argparse
import fnmatch
import os
import shutil
def main():
    parser = argparse.ArgumentParser(description="Build MTGos")
    def addBinary(flag, help, dest):
        parser.add_argument(flag, help=help, dest=dest, action="store_const", const=True, default=False)
    addBinary("--clean", "clean all temporary files", "clean")
    addBinary("--reset", "reset configuration","reset")
    parser.add_argument("--set-arch", help="Clean all files and copy the template", dest="archcopy")
    args=parser.parse_args()
    print(args)
    if args.clean:
        clean()
    if args.archcopy != None:
        shutil.copy("./templates/{arch}.yaml".format(arch=args.archcopy),"./config.yaml")
    if not os.access("config.yaml", os.F_OK):
        raise ValueError("You need to select a profile before you can build MTGos!")
    with open("config.yaml") as f:
        config=yaml.load(f)
    if args.reset:
        shutil.copy("./templates/{arch}.yaml".format(arch=config["arch"]),"./config.yaml")
        with open("config.yaml") as f:
            config=yaml.load(f)
    if not config["finished"]:
        raise ValueError("You have to finish your configuration before you can build!")

#Cleans all *.o files
def clean():
    for root, dirnames, filenames in os.walk('.'):
        for filename in fnmatch.filter(filenames, '*.o'):
            os.remove(os.path.join(root, filename))


if __name__ == "__main__":
    main()
