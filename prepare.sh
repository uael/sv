# prepare.sh --
#

set -ex

(cd .. ; sh autogen.sh)
sh ../configure.sh

### end of file
