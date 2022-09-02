#!/bin/sh

echo "NONE"
echo "lqcp"
time test/lqcp $1 none
echo "lqcp_array"
time test/lqcp_array $1 none
echo "lqcp_map"
time test/lqcp_map $1 none

echo "LP"

echo "lqcp"
time test/lqcp $1 lp
mv foo.lp lqcp.lp
echo "lqcp_array"
time test/lqcp_array $1 lp
mv foo.lp lqcp_array.lp
echo "lqcp_map"
time test/lqcp_map $1 lp
mv foo.lp lqcp_map.lp

diff lqcp.lp lqcp_array.lp
diff lqcp.lp lqcp_map.lp

exit 1

echo "NL"

echo "lqcp"
time test/lqcp $1 nl
mv foo.nl lqcp.nl
echo "lqcp_array"
time test/lqcp_array $1 nl
mv foo.nl lqcp_array.nl
echo "lqcp_map"
time test/lqcp_map $1 nl
mv foo.nl lqcp_map.nl

diff lqcp.nl lqcp_array.nl
