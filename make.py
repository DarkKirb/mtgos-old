#!/usr/bin/env python3
##########################################################################
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
##########################################################################
import yaml
import argparse
import fnmatch
import os
import shutil
import random


def main():
    parser = argparse.ArgumentParser(description="Build MTGos")

    def addBinary(flag, help, dest):
        parser.add_argument(flag, help=help, dest=dest,
                            action="store_const", const=True, default=False)
    addBinary("--clean", "clean all temporary files", "clean")
    addBinary("--reset", "reset configuration", "reset")
    addBinary("--ci", "force build", "ci")
    parser.add_argument(
        "--set-arch", help="Clean all files and copy the template", dest="archcopy")
    args = parser.parse_args()
    print(args)
    if args.clean:
        clean()
    if args.archcopy != None:
        shutil.copy(
            "./templates/{arch}.yaml".format(arch=args.archcopy), "./config.yaml")
    if not os.access("config.yaml", os.F_OK):
        raise ValueError(
            "You need to select a profile before you can build MTGos!")
    with open("config.yaml") as f:
        config = yaml.load(f)
    if args.reset:
        shutil.copy(
            "./templates/{arch}.yaml".format(arch=config["arch"]), "./config.yaml")
        with open("config.yaml") as f:
            config = yaml.load(f)
    if not config["finished"] and not args.ci:
        raise ValueError(
            "You have to finish your configuration before you can build!")
    buildid = " -DBUILDID=" + hex(random.getrandbits(64)) + " "
    config["cflags"] += buildid
    config["cxxflags"] += buildid
    config["asflags"] += buildid
    print("Parsing the kernel config file")
    flags = getdefs(config)
    config["cflags"] += flags
    config["cxxflags"] += flags
    config["asflags"] += flags
    print("Starting build")
    wd = os.getcwd()
    for d in config["search_dirs"]:
        os.chdir(d)
        print("Building in " + d)
        build(config)
        os.chdir(wd)
    print("Linking mtgos")
    link(config, "mtgos.elf", "kernel")
    print("Creating ISO")
    shutil.copy("mtgos.elf", "iso")
    shutil.copy("font.bin", "iso")
    os.system("grub-mkrescue -o bootable.iso iso")
    print("All done!")
# Cleans all *.o files


def clean():
    for root, dirnames, filenames in os.walk('.'):
        for filename in fnmatch.filter(filenames, '*.o'):
            os.remove(os.path.join(root, filename))


def getCFlags(config):
    cflags = config["cflags"]
    for f in config["includes"]:
        cflags += " -I" + f
    return cflags


def getCXXFlags(config):
    cflags = config["cxxflags"]
    for f in config["includes"]:
        cflags += " -I" + f
    return cflags


def getASFlags(config):
    cflags = config["asflags"]
    for f in config["includes"]:
        cflags += " -I" + f
    return cflags


def build(config):
    for root, dirnames, filenames in os.walk('.'):
        for filename in fnmatch.filter(filenames, '*.c'):
            compileC(config, os.path.join(root, filename))
        for filename in fnmatch.filter(filenames, '*.cpp'):
            compileCXX(config, os.path.join(root, filename))
        for filename in fnmatch.filter(filenames, '*.S'):
            assemble(config, os.path.join(root, filename))


def getdefs(config):
    def getd(builtin, prefix=""):
        for n, v in builtin.items():
            if isinstance(v, dict):
                for f in getd(v, prefix + n + "_"):
                    yield f
            else:
                if v:
                    print("Using flag -D" + prefix + n)
                    yield prefix + n
    flags = ""
    for f in getd(config["builtins"]):
        flags += "-D" + f + " "
    return flags


def compileC(config, f):
    cflags = getCFlags(config)
    try:
        if os.stat(f).st_mtime < os.stat(f + ".o").st_mtime:
            print(f + " is up to date.")
            return
    except:
        pass
    print("Compiling C file " + f)
    if os.system("{prefix}gcc {cflags} -c -o {f}.o {f}".format(prefix=config["prefix"], cflags=cflags, f=f)):
        raise ValueError("Failed compiling {f} via {prefix}gcc {cflags} -c -o {f}.o {f}".format(
            prefix=config["prefix"], cflags=cflags, f=f))


def compileCXX(config, f):
    cxxflags = getCXXFlags(config)
    try:
        if os.stat(f).st_mtime < os.stat(f + ".o").st_mtime:
            print(f + " is up to date.")
            return
    except:
        pass
    print("Compiling C++ file " + f)
    if os.system("{prefix}g++ {cxxflags} -c -o {f}.o {f}".format(prefix=config["prefix"], cxxflags=cxxflags, f=f)):
        raise ValueError("Failed compiling {f} via {prefix}g++ {cxxflags} -c -o {f}.o {f}".format(
            prefix=config["prefix"], cxxflags=cxxflags, f=f))


def assemble(config, f):
    asflags = getASFlags(config)
    try:
        if os.stat(f).st_mtime < os.stat(f + ".o").st_mtime:
            print(f + " is up to date.")
            return
    except:
        pass
    print("Assembling file " + f)
    if os.system("{prefix}gcc {asflags} -c -o {f}.o {f}".format(prefix=config["prefix"], asflags=asflags, f=f)):
        raise ValueError("Failed assembling {f} via {prefix}gcc {asflags} -c -o {f}.o {f}".format(
            prefix=config["prefix"], asflags=asflags, f=f))


def link(config, f, d):
    print("Linking " + f)
    objs = []
    for root, dirnames, filenames in os.walk(d):
        for filename in fnmatch.filter(filenames, '*.o'):
            objs.append(os.path.join(root, filename))
    objs = ' '.join(objs)
    if os.system("{prefix}g++ {ldflags} -o {f} {objs} -lgcc".format(prefix=config["prefix"], ldflags=config["ldflags"], f=f, objs=objs)):
        raise ValueError("Failed linking {f} via {prefix}g++ {ldflags} -o {f} {objs} -lgcc".format(
            prefix=config["prefix"], ldflags=config["ldflags"], f=f, objs=objs))

if __name__ == "__main__":
    main()
