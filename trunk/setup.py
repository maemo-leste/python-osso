from distutils.core import setup
from distutils.extension import Extension
from subprocess import Popen, PIPE
import glob

from Cython.Distutils import build_ext

# Based on snippet from http://code.activestate.com/recipes/502261/
def pkgconfig(*packages, **kw):
    flag_map = {'-I': 'include_dirs', '-L': 'library_dirs', '-l': 'libraries'}
    command = Popen(["pkg-config", "--libs", "--cflags"] + list(packages), stdout=PIPE)
    for token in command.communicate()[0].split():
        kw.setdefault(flag_map.get(token[:2]), []).append(token[2:])
    kw["extra_compile_args"] = ["-Werror"]
    return kw

extensions = []
for pyx in glob.glob("osso/*.pyx"):
    if pyx == "osso/helper.pyx":
        continue
    ext_name = pyx[:-4].replace('/', '.')
    extensions.append(Extension(ext_name, [pyx], **pkgconfig("libosso")))

setup(name = "osso",
      version = "0.4",
      packages = ["osso"],
      ext_modules = extensions,
      cmdclass = {'build_ext': build_ext})
