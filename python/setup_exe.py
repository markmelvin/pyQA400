##!/usr/bin/env python
# -*- coding: iso-8859-1 -*-

import sys
from resources import *

from distutils.core import setup
import py2exe

################################################################

# If run without args, build executables, in quiet mode.
if len(sys.argv) == 1:
    sys.argv.append("py2exe")
    sys.argv.append("-q")

class Target:
    def __init__(self, **kw):
        self.__dict__.update(kw)
        # for the versioninfo resources
        self.version = APP_VERSION
        self.company_name = COMPANY_NAME
        self.copyright = COPYRIGHT
        self.name = APP_NAME


app = Target(
    # used for the versioninfo resource
    description = APP_DESCRIPTION,

    # what to build
    script = "QA400_test.py",
    dest_base = "QA400_test",
    )

data_files = [
              ('.', ['QA400API.dll', 'QAConnectionManager.dll']),
             ]

setup(
        data_files = data_files,
        options = {"py2exe": {"compressed": 1,
                   "dll_excludes" : ['QA400API.dll', 'QAConnectionManager.dll',],
                   "optimize": 2,
                   "bundle_files": 2,
                   }},
        zipfile = None,
        console = [app]
    )
